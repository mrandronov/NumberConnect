#include "SDL2_gfxPrimitives.h"

#include "component/connector.h"
#include "util/error.h"
#include "util/color.h"
#include "util/sound.h"

void
cnctr_link_draw( Connector_Link* self )
{
        thickLineRGBA( config->renderer,
                        self->start_x, self->start_y,
                        self->end_x, self->end_y,
                        10,
                        select_color.r, select_color.g, select_color.b, select_color.a );
}

void
cnctr_link_destroy( Connector_Link* self )
{
        self->cell = NULL; // Destroyed by board.
        free( self );
        self = NULL;
}

Connector_Link*
cnctr_link_create( Cell* current_cell )
{
        Connector_Link*         link = ( Connector_Link* ) malloc( sizeof( Connector_Link ) );

        link->start_x = current_cell->box.x + ( current_cell->box.w / 2 );
        link->start_y = current_cell->box.y + ( current_cell->box.h / 2 );
        link->end_x = config->mouse_x;
        link->end_y = config->mouse_y;
        link->cell = current_cell;

        link->render = cnctr_link_draw;
        link->destroy = cnctr_link_destroy;

        return link;
}

void
chain_node_destroy( Chain_Node* self )
{
        self->link->destroy( self->link );
        self->next = NULL;
        self->prev = NULL;
        free( self );
        self = NULL;
}

Chain_Node*
chain_node_create( Connector_Link* link, Chain_Node* next, Chain_Node* prev )
{
        Chain_Node*             node = ( Chain_Node* ) malloc( sizeof( Chain_Node ) );

        node->link = link;
        node->next = next;
        node->prev = prev;

        return node;
}

void
cnctr_chain_draw( Connector_Chain* self )
{
        if ( !self->enabled ) {
                return;
        }

        Chain_Node*             p = self->head;

        while ( p != NULL )
        {
                if ( p == self->tail )
                {
                        p->link->end_x = config->mouse_x;
                        p->link->end_y = config->mouse_y;
                }

                p->link->render( p->link );
                p = p->next;
        }
}

void
add_link_from_cell( Connector_Chain* self, Cell* cell )
{
        int             new_center_x = 0;
        int             new_center_y = 0;
        cell->get_center( cell, &new_center_x, &new_center_y );
        self->tail->link->end_x = new_center_x;
        self->tail->link->end_y = new_center_y;

        self->add_link( self, cell );
        current_cell = cell;
        current_cell->color = &orange;
}

bool
is_cell_visited( Connector_Chain* self, Cell* cell )
{
        for ( int i = 0; i < self->length; i++ )
        {
                if ( self->visited[ i ].row == cell->row &&
                                self->visited[ i ].col == cell->col )
                {
                        return true;
                }
        }
        return false;
}

bool
is_same_cell_pos( Cell* cell, Board_Pos *pos )
{
        return ( cell->row == pos->row ) && ( cell->col == pos->col );
}

void
cnctr_chain_is_next_nghbr( Connector_Chain* self )
{
        if ( !self->enabled ) {
                return;
        }

        // TODO: Wrap this into a function.
        int             center_x = 0;
        int             center_y = 0;

        current_cell->get_center( current_cell, &center_x, &center_y );

        int             dx = config->mouse_x - center_x;
        int             dy = config->mouse_y - center_y;

        double          theta = atan2( (double) dy, (double) dx );

        theta *= -180 / M_PI;

        double          distance = sqrt( pow( dx, 2 ) + pow( dy, 2 ) );

        if ( distance <= 65.0f )
        {
                return;
        }

        Cell*           new_cell = NULL;
        int             cur_row = current_cell->row;
        int             cur_col = current_cell->col;
        // TODO: Refactor the following conditional chain, complete mess right here
        if ( -22.5 < theta && theta < 22.5 )
        {
                /* Left cell */
                if ( cur_col == BOARD_WIDTH - 1 )
                {
                        return;
                }
                new_cell = &board->cells[ cur_row ][ cur_col + 1 ];
        }
        else if ( -67.5 < theta && theta < -22.5 )
        {
                /* Bottom Left cell */
                if ( cur_col == BOARD_WIDTH - 1 ||
                                cur_row == BOARD_HEIGHT - 1 )
                {
                        return;
                }

                new_cell = &board->cells[ cur_row + 1 ][ cur_col + 1 ];
        }
        else if ( -112.5 < theta && theta < -67.5 )
        {
                /* Bottom cell */
                if ( cur_row == BOARD_HEIGHT - 1 )
                {
                        return;
                }
                new_cell = &board->cells[ cur_row + 1 ][ cur_col ];
        }
        else if ( -158.5 < theta && theta < -112.5 )
        {
                /* Bottom Right cell */
                if ( cur_col == 0 ||
                                cur_row == BOARD_HEIGHT - 1 )
                {
                        return;
                }

                new_cell = &board->cells[ cur_row + 1 ][ cur_col - 1 ];
        }
        else if ( 67.5 < theta && theta < 112.5 )
        {
                /* Top cell */
                if ( cur_row == 0 )
                {
                        return;
                }
                new_cell = &board->cells[ cur_row - 1 ][ cur_col ];
        }
        else if ( 158.5 > theta && theta > 67.5 )
        {
                /* Top Right cell */
                if ( cur_col == 0 ||
                                cur_row == 0 )
                {
                        return;
                }

                new_cell = &board->cells[ cur_row - 1 ][ cur_col - 1 ];
        }
        else if ( ( -158.5 > theta || 158.5 < theta ) )
        {
                /* Right cell */
                if ( cur_col == 0 )
                {
                        return;
                }
                new_cell = &board->cells[ cur_row ][ cur_col - 1 ];
        }
        else if ( 67.5 > theta && theta > 22.5 )
        {
                /* Top Left cell */
                if ( cur_col == BOARD_WIDTH - 1 ||
                                cur_row == 0 )
                {
                        return;
                }

                new_cell = &board->cells[ cur_row - 1 ][ cur_col + 1 ];
        }

        // Check tile values to see if they are equal or double the current chain head.
        if ( !new_cell->tile )
        {
                return;
        }

        // Skip adding new_cell if it has already been visited in the chain
        // but only if it was not the previous one. If it is, then we move
        // the chain back one link to that previous cell.
        if ( is_cell_visited( self, new_cell ) )
        {
                Board_Pos*              prev = &self->visited[ self->length - 2 ];
                if ( is_same_cell_pos( current_cell, prev ) )
                {
                        return;
                }

                if ( is_same_cell_pos( new_cell, prev ) )
                {
                        self->total -= num_set[ self->head->link->cell->tile->index ]; // uh oh
                        self->remove_link( self );
                        current_cell = self->tail->link->cell;
                }

                return;
        }

        // Complicated logic to say true if new_cell value is same as current cell OR
        // the length of the chain is > 1 AND new_cell value is double the current cell.
        bool            connectable = ( new_cell->tile->index == current_cell->tile->index );
        connectable |= ( ( self->length >= 2 ) && new_cell->tile->index == current_cell->tile->index + 1 );

        if ( !connectable )
        {
                return;
        }

        if ( !new_cell )
        {
                return;
        }

        add_link_from_cell( self, new_cell );
        new_cell = NULL;
        /* cell_connect_sound->play( cell_connect_sound ); */
}

void
cnctr_chain_destroy( Connector_Chain* self )
{
        Chain_Node*             p = self->head;

        while ( p != NULL )
        {
                p->link->destroy( p->link );
                p = p->next;
        }
        free( self );
        self = NULL;
}

void
cnctr_chain_add_link( Connector_Chain* self, Cell* cell )
{
        assert_msg( ( cell == NULL ), "Adding NULL cell to new chain link." );

        Chain_Node* node = chain_node_create( cnctr_link_create( cell ), NULL, NULL );

        // Update the chain's visited array
        self->visited[ self->length ].row = cell->row;
        self->visited[ self->length ].col = cell->col;

        self->total += num_set[ current_cell->tile->index ];

        if ( self->length == 0 ) {
                self->head = node; 
                self->tail = node;
                self->length++;

                return;
        }

        Chain_Node*             p = self->head;

        while ( p->next != NULL )
        {
                p = p->next;
        }

        p->next = node;
        node->prev = p;
        self->tail = node;

        self->length++;
}

void
cnctr_chain_remove_link( Connector_Chain* self )
{
        if ( self->length == 0 )
        {
                return;
        }

        if ( self->head == self->tail )
        {
                chain_node_destroy( self->head );
                self->head = NULL;
                self->tail = NULL;
        }
        else
        {
                Chain_Node*             temp = self->tail->prev;
                chain_node_destroy( self->tail );

                self->tail = temp;
                self->tail->next = NULL;
        }

        self->length--;
}

void
cnctr_chain_clear_tiles( Connector_Chain* self )
{
        for ( int i = 0; i < self->length; i++ )
        {
                int             row = self->visited[ i ].row;
                int             col = self->visited[ i ].col;

                board->cells[ row ][ col ].tile = NULL;
        }
}

void
cnctr_chain_clear( Connector_Chain* self )
{
        while ( self->length != 1 )
        {
                self->remove_link( self );
        }
        current_cell = NULL;

        for ( int i = 0; i < BOARD_HEIGHT; i++ )
        {
                for ( int j = 0; j < BOARD_WIDTH; j++ )
                {
                        board->cells[ i ][ j ].color = &highlight_color;
                }
        }
}

Connector_Chain*
cnctr_chain_create()
{
        Connector_Chain*        chain = ( Connector_Chain* ) malloc( sizeof( Connector_Chain ) );

        chain->length = 0;
        chain->total = 0;
        chain->enabled = false;
        chain->visited = ( Board_Pos* ) malloc( sizeof( Board_Pos ) * BOARD_SIZE ); 

        chain->render = cnctr_chain_draw;
        chain->on_click = cnctr_chain_is_next_nghbr;
        chain->add_link = cnctr_chain_add_link;
        chain->remove_link = cnctr_chain_remove_link;
        chain->clear_tiles = cnctr_chain_clear_tiles;
        chain->clear = cnctr_chain_clear;
        chain->check_neighbor = cnctr_chain_is_next_nghbr;
        chain->destroy = cnctr_chain_destroy;

        return chain;
}

