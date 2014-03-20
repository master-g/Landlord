/*
 *  medlist.c
 *  Landlord
 *
 *  Created by Master.G on 14-3-30.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "medlist.h"
#include "common.h"

medlist_t *MEDList_Create(void)
{
    medlist_t *l = (medlist_t *)calloc(1, sizeof(medlist_t));
    return l;
}

void MEDList_Destroy(medlist_t *l, MEDListFunc_Delete delfunc)
{
    medlist_t *node, *temp;
    
    if (l->next == NULL)
    {
        delfunc(l->payload);
        free(l);
        return;
    }
    
    node = l->next;
    temp = l;
    while (node->next != NULL)
    {
        delfunc(temp->payload);
        free(temp);
        temp = node;
        node = node->next;
    }
    
    delfunc(node->payload);
    free(node);
}

int MEDList_Length(medlist_t *l)
{
    int length = 0;
    medlist_t *tl = l;
    
    while (tl != NULL)
    {
        length++;
        tl = tl->next;
    }
    
    return length;
}

void MEDList_PushFront(medlist_t **l, medlist_t *node)
{
    if (*l == NULL)
    {
        *l = node;
    }
    else
    {
        node->next = (*l)->next;
        (*l)->next = node;
    }
}

void MEDList_PushBack(medlist_t **l, medlist_t *node)
{
    medlist_t *tail = *l;
    
    if (*l == NULL)
    {
        *l = node;
    }
    else
    {
        while (tail->next != NULL)
            tail = tail->next;
        
        tail->next = node;
    }
}

void MEDList_Remove(medlist_t **l, medlist_t *node)
{
    medlist_t *temp, *prev;
    
    if (*l == node)
    {
        (*l) = (*l)->next;
        return;
    }
    
    if ((*l)->next == NULL && *l != node)
        return;
    
    prev = *l;
    temp = (*l)->next;
    
    while (temp->next != NULL)
    {
        if (temp == node)
        {
            prev->next = temp->next;
            break;
        }
        
        prev = temp;
        temp = temp->next;
    }
}

medlist_t *MEDList_Find(medlist_t *l, void *context, MEDListFunc_Find finder)
{
    int found = 0;
    medlist_t *result = NULL;
    result = l;
    
    while (result != NULL)
    {
        if (finder(result->payload, context))
        {
            found = 1;
            break;
        }
        
        result = result->next;
    }
    
    return (found != 0) ? result : NULL;
}
