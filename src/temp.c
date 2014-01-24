/*
 *  temp.c
 *  Landlord
 *
 *  Created by Master.G on 14-1-23.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "common.h"
#include "card.h"

/*
 * http://scim.brad.ac.uk/staff/pdf/picowlin/AISB2011.pdf
 * http://en.wikipedia.org/wiki/Monte-Carlo_tree_search
 */

/*
 * ************************************************************
 * hand graph
 * ************************************************************
 */

/*           <--link-->[CardC]<--
 * -->[CardA]<--link-->[CardB]<--
 *           <--link-->[CardD]<--
 */

/*
 * link between graph nodes
 */
typedef struct link_t
{
    uint8_t type;       /* card type */
    void    *prev;      /* prev node */
    void    *next;      /* next node */
    
} link_t;

/*
 * link list
 */
typedef struct link_list_t
{
    link_t              *link;
    struct link_list_t  *prev;
    struct link_list_t  *next;
    
} link_list_t;

/*
 * graph node
 */
typedef struct graph_t
{
    uint8_t         card;
    link_list_t     *links;
    struct graph_t  *prev;
    struct graph_t  *next;
    
} graph_t;

/*
 * parse a card array into hands graph
 */
graph_t *Graph_Parse(card_array_t *array);

/*
 * destroy a hands graph
 */
void Graph_Destroy(graph_t *graph);

/*
 * ************************************************************
 * hand graph
 * ************************************************************
 */

void LinkList_Destroy(link_list_t *links)
{
    link_list_t *cur;
    link_list_t *tmp;
    
    for (cur = links; cur != NULL; )
    {
        tmp = cur;
        cur = cur->next;
        
        free(tmp);
    }
}

void Graph_Destroy(graph_t *graph)
{
    graph_t *cur;
    graph_t *tmp;
    
    for (cur = graph; cur != NULL; )
    {
        tmp = cur;
        cur = cur->next;
        
        LinkList_Destroy(tmp->links);
        free(tmp);
    }
}

graph_t *Graph_Parse(card_array_t *array)
{
    return NULL;
}