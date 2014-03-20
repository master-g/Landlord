/*
 *  medlist.c
 *  Landlord
 *
 *  Created by Master.G on 14-3-30.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "common.h"
#include "medlist.h"

medlist_t *MEDList_Create(void)
{
    medlist_t *l = (medlist_t *)calloc(1, sizeof(medlist_t));
    return l;
}

void MEDList_Destroy(medlist_t **l, MEDListFunc_Delete delfunc)
{
    medlist_t *current = *l;
    medlist_t *next;
    
    while (current != NULL)
    {
        next = current->next;
        delfunc(current->payload);
        free(current);
        current = next;
    }
    
    *l = NULL;
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
    node->next = *l;
    *l = node;
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

/*
 *
 */

void _MEDList_Destroy(void *payload)
{
    
}

void MEDList_Test(void)
{
    medlist_t *a, *b, *c;
    a = MEDList_Create();
    b = MEDList_Create();
    c = MEDList_Create();
    
    MEDList_PushBack(&a, b);
    MEDList_PushFront(&a, c);
    
    MEDList_Destroy(&a, _MEDList_Destroy);
}
