#include "component/cell.h"
#include "component/number_set.h"
#include "util/color.h"
#include "util/rect.h"

void
cell_draw( Cell* self )
{
        if ( self->animation != NULL )
        {
                self->animation->step( self->animation );
        }

        render_rect( &self->box, *self->color );

        if ( self->tile )
        {
                self->tile->box.x = self->box.x;
                self->tile->box.y = self->box.y;

                self->tile->render( self->tile );
        }
}

void
cell_get_center( Cell* self, int* center_x, int* center_y )
{
        *center_x = self->box.x + ( self->box.w / 2 );
        *center_y = self->box.y + ( self->box.h / 2 );
}

bool
cell_hover( Cell* self )
{
        int             mouse_x = config->mouse_x;
        int             mouse_y = config->mouse_y;
        SDL_Rect        box = self->box;

        if ( ( mouse_x >= box.x && mouse_x <= box.x + box.w ) &&
                        ( mouse_y >= box.y && mouse_y <= box.y + box.h ) )
        {
                return true;
        }
        return false;
}

void
cell_destroy( Cell* self )
{
        self = NULL;
}

Cell*
cell_create( int x, int y, int row, int col )
{
        Cell*           cell = malloc( sizeof( Cell ) );

        cell->row = row;
        cell->col = col;

        // This should be made dynamic in the future.
        cell->box = ( SDL_Rect ) { .x = x + CELL_PADDING, .y = y + CELL_PADDING, .w = CELL_BOX_WIDTH, .h = CELL_BOX_HEIGHT };
        cell->color = &highlight_color;

        cell->render = cell_draw;
        cell->get_center = cell_get_center;
        cell->on_hover = cell_hover;
        cell->destroy = cell_destroy;

        int             val = rand() % 6;

        cell->tile = &tile_set[ val ];

        return cell;
}

void
cell_board_draw( Board* self )
{
        for ( int i = 0; i < BOARD_HEIGHT; i++ )
        {
                for ( int j = 0; j < BOARD_WIDTH; j++ )
                {
                        Cell*           curr_cell = &self->cells[ i ][ j ];

                        curr_cell->render( curr_cell );
                }
        }
}

Cell*
cell_board_hover( Board* self )
{
        for ( int i = 0; i < BOARD_HEIGHT; i++ )
        {
                for ( int j = 0; j < BOARD_WIDTH; j++ )
                {
                        Cell*           curr_cell = &self->cells[ i ][ j ];

                        if ( curr_cell->on_hover( curr_cell ) )
                        {
                                return curr_cell;
                        }
                }
        }
        return NULL;
}

void
cell_board_trickle_down( Board* self )
{
        // First column only, for now.
        for ( int j = 0; j < BOARD_WIDTH; j++ )
        {
                while( true )
                {
                        int i = BOARD_HEIGHT - 1;

                        // Find the first empty cell
                        while ( i >= 0 && self->cells[ i ][ j ].tile )
                        {
                                i--;
                        }
                        int             cur_empty = i;

                        if ( cur_empty == -1 )
                        {
                                // No empty cells in this column.
                                break;
                        }

                        i--;
                        while( i >= 0 && !self->cells[ i ][ j ].tile )
                        {
                                i--;
                        }
                        int             next_full = i;

                        if ( next_full == - 1 )
                        {
                                // No more tiled cells left in the column.
                                // Fill in the remaining cells with random
                                // tiles in the playable range.

                                while( cur_empty >= 0 )
                                {
                                        int             index = ( rand() % 6 ) + 0;
                                        Cell*           cur_cell = &self->cells[ cur_empty ][ j ];
                                        float           end_y = cur_cell->box.y;

                                        cur_cell->box.y -= 900;
                                        cur_cell->animation = animation_create( &cur_cell->box, cur_cell->box.x, end_y, 0.5f );

                                        cur_cell->tile = &tile_set[ index ];

                                        cur_empty--;
                                }

                                break;
                        }

                        // Now move the full cell to the empty one.
                        self->cells[ cur_empty ][ j ].tile = self->cells[ next_full ][ j ].tile;
                        self->cells[ next_full ][ j ].tile = NULL;

                        // And create the animation for it
                        Cell*           cur_cell = &self->cells[ cur_empty ][ j ];
                        float           end_y = cur_cell->box.y;

                        cur_cell->box.y = self->cells[ next_full ][ j ].box.y;
                        cur_cell->animation = animation_create( &cur_cell->box, cur_cell->box.x, end_y, 0.5f );
                }
        }
}

void
cell_board_destroy( Board* self )
{
        for ( int i = 0; i < self->height; i++ )
        {
                for ( int j = 0; j < self->width; j++ )
                {
                        Cell*           curr_cell = &self->cells[ i ][ j ];

                        curr_cell->destroy( curr_cell );
                }
        }

        free( self );
        self = NULL;
}

Board*
cell_board_create()
{
        Board*          board = ( Board* ) malloc( sizeof( Board ) );

        board->width = BOARD_WIDTH;
        board->height = BOARD_HEIGHT;

        board->cells = ( Cell** ) malloc( sizeof( Cell* ) * BOARD_HEIGHT );

        int             start_x = CELL_WIDTH;
        int             start_y = CELL_HEIGHT;

        for ( int i = 0; i < board->height; i++ )
        {
                int             y = start_y + ( CELL_HEIGHT * i );

                board->cells[ i ] = ( Cell* ) malloc( sizeof( Cell ) * BOARD_WIDTH );

                for ( int j = 0; j < board->width; j++ )
                {
                        int             x = start_x + ( CELL_WIDTH * j );

                        board->cells[ i ][ j ] = *cell_create( x, y, i, j );
                }
        }

        board->render = cell_board_draw;
        board->on_hover = cell_board_hover;
        board->trickle_down = cell_board_trickle_down;
        board->destroy = cell_board_destroy;

        return board;
}

