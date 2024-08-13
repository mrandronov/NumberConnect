#ifndef __LIB_CONNECTOR_H__
#define __LIB_CONNECTOR_H__

#include "config.h"
#include "cell.h"

config_t*       config;
Cell*           current_cell;

typedef struct cnctr_link_t 
{
        int             start_x;
        int             start_y;
        int             end_x;
        int             end_y;
        Cell*           cell;

        void            ( *render )( struct cnctr_link_t* );
        void            ( *destroy )( struct cnctr_link_t* );
} Connector_Link;

typedef struct chain_node_t
{
        Connector_Link*         link;
        struct chain_node_t*    next;
        struct chain_node_t*    prev;
} Chain_Node;

typedef struct
{
        int             row;
        int             col;
} Board_Pos;

typedef struct cnctr_chain_t
{
        int             length;
        int             total;
        bool            enabled;
        Chain_Node*     head;
        Chain_Node*     tail;
        Board_Pos*      visited;

        void            ( *render )( struct cnctr_chain_t* );
        void            ( *on_click )( struct cnctr_chain_t* );
        void            ( *add_link )( struct cnctr_chain_t*, Cell* cell );
        void            ( *remove_link )( struct cnctr_chain_t* );
        void            ( *clear_tiles )( struct cnctr_chain_t* );
        void            ( *clear )( struct cnctr_chain_t* );
        void            ( *check_neighbor )( struct cnctr_chain_t* );
        void            ( *destroy )( struct cnctr_chain_t* );
} Connector_Chain;

Connector_Link*         cnctr_link_create( Cell* current_cell );
Connector_Chain*        cnctr_chain_create();

#endif /* __LIB_CONNECTOR_H__ */
