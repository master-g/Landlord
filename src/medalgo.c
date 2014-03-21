/*
 *  medalgo.c
 *  Landlord
 *
 *  Created by Master.G on 14-3-30.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "common.h"
#include "medalgo.h"

/*
 * ************************************************************
 * list
 * ************************************************************
 */

medlist_t *MEDList_Create(void)
{
    medlist_t *l = (medlist_t *)calloc(1, sizeof(medlist_t));
    return l;
}

void MEDList_Destroy(medlist_t **l, MEDAlgoFunc_Delete delfunc)
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

medlist_t *MEDList_Find(medlist_t *l, void *context, MEDAlgoFunc_Find finder)
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
 * ************************************************************
 * stack
 * ************************************************************
 */

medstack_t *MEDStack_Pop(medstack_t **s)
{
    medstack_t *node = *s;
    
    if ((*s)->next == NULL)
        *s = NULL;
    else
        *s = (*s)->next;
    
    return node;
}

/*
 * ************************************************************
 * queue
 * ************************************************************
 */

/*
 * ************************************************************
 * tree
 * ************************************************************
 */

medtree_t *MEDTree_Create(void)
{
    return (medtree_t *)calloc(1, sizeof(medtree_t));
}

void _MEDTree_PushNode(medstack_t **s, medtree_t *node)
{
    medstack_t *snode = (medstack_t *)calloc(1, sizeof(medstack_t));
    snode->payload = node;
    MEDStack_Push(s, snode);
}

void MEDTree_Destroy(medtree_t **t, MEDAlgoFunc_Delete delfunc)
{
    medstack_t *s = NULL;
    medstack_t *snode = NULL;
    medtree_t *tnode = NULL;
    medtree_t *temp = NULL;
    
    tnode = *t;
    _MEDTree_PushNode(&s, tnode);
    
    while (s != NULL)
    {
        snode = MEDStack_Pop(&s);
        tnode = (medtree_t *)snode->payload;
        delfunc(tnode->payload);
        free(snode);
        
        temp = tnode->child;
        while (temp != NULL)
        {
            _MEDTree_PushNode(&s, temp);
            temp = temp->sibling;
        }
        
        free(tnode);
    }
}

/*
 * Test
 */

void _MEDList_Destroy(void *payload)
{
    printf("free %p\n", payload);
    free(payload);
}

void MEDList_Test(void)
{
    int i = 0;
    medlist_t *a, *b, *c;
    a = MEDList_Create();
    a->payload = calloc(1, sizeof(int));
    b = MEDList_Create();
    b->payload = calloc(1, sizeof(int));
    c = MEDList_Create();
    c->payload = calloc(1, sizeof(int));
    
    MEDStack_Push(&a, b);
    MEDStack_Push(&a, c);
    
    for (i = 0; i < 3; i++)
    {
        b = MEDStack_Pop(&a);
        b->next = NULL;
        MEDList_Destroy(&b, _MEDList_Destroy);
    }
    
    printf("%p\n", a);
}

void _MEDTree_Free(void *payload)
{
    printf("free %p\n", payload);
    free(payload);
}

void MEDTree_Test(void)
{
    medtree_t *a, *b, *c, *d, *e, *f;
    
    a = MEDTree_Create();
    a->payload = calloc(1, sizeof(int));
    b = MEDTree_Create();
    b->payload = calloc(1, sizeof(int));
    c = MEDTree_Create();
    c->payload = calloc(1, sizeof(int));
    d = MEDTree_Create();
    d->payload = calloc(1, sizeof(int));
    e = MEDTree_Create();
    e->payload = calloc(1, sizeof(int));
    f = MEDTree_Create();
    f->payload = calloc(1, sizeof(int));
    
    a->child = b;
    b->sibling = c;
    c->child = d;
    d->sibling = e;
    e->sibling = f;
    
    MEDTree_Destroy(&a, _MEDTree_Free);
}
