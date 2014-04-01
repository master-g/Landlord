/*
 *  medalgo.c
 *  Landlord
 *
 *  Created by Master.G on 14-3-30.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#include "common.h"
#include "medalgo.h"

void MEDAlgo_StandardFree(void *payload)
{
    free(payload);
}

/*
 * ************************************************************
 * list
 * ************************************************************
 */

medlist_t *MEDList_Create(void)
{
    medlist_t *l = (medlist_t *)malloc(sizeof(medlist_t));
    return l;
}

void MEDList_Destroy(medlist_t **l, MEDAlgoFunc_Delete delfunc)
{
    medlist_t *current = *l;
    medlist_t *next;
    
    while (current != NULL)
    {
        next = current->next;
        if (delfunc != NULL)
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

int MEDList_Remove(medlist_t **l, medlist_t *node)
{
    int found = 0;
    medlist_t *temp, *prev;
    
    if (*l == node)
    {
        (*l) = (*l)->next;
        return 1;
    }
    
    if ((*l)->next == NULL && *l != node)
        return 0;
    
    prev = *l;
    temp = (*l)->next;
    
    while (temp != NULL)
    {
        if (temp == node)
        {
            prev->next = temp->next;
            found = 1;
            break;
        }
        
        prev = temp;
        temp = temp->next;
    }
    
    return found;
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

void _MEDTreeStack_PushNode(medstack_t **s, medtree_t *node)
{
    medstack_t *snode = (medstack_t *)malloc(sizeof(medstack_t));
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
    _MEDTreeStack_PushNode(&s, tnode);
    
    while (s != NULL)
    {
        snode = MEDStack_Pop(&s);
        tnode = (medtree_t *)snode->payload;
        if (delfunc != NULL)
            delfunc(tnode->payload);
        free(snode);
        
        temp = tnode->child;
        while (temp != NULL)
        {
            _MEDTreeStack_PushNode(&s, temp);
            temp = temp->sibling;
        }
        
        free(tnode);
    }
}

void MEDTree_AddChild(medtree_t **t, medtree_t *node)
{
    medtree_t *child = NULL;
    node->root = *t;
    
    if (*t != NULL)
    {
        child = (*t)->child;
        if (child == NULL)
        {
            (*t)->child = node;
        }
        else
        {
            while (child->sibling != NULL)
                child = child->sibling;
            
            child->sibling = node;
        }
    }
    else
    {
        *t = node;
    }
}

void MEDTree_AddSibling(medtree_t **t, medtree_t *node)
{
    medtree_t *sibling = NULL;
    
    if (*t != NULL)
    {
        node->root = (*t)->root;
        
        sibling = *t;
        while (sibling->sibling != NULL)
            sibling = sibling->sibling;
        
        sibling->sibling = node;
    }
    else
    {
        node->root = NULL;
        (*t) = node;
    }
}

void MEDTree_DumpLeafToStack(medtree_t *t, medstack_t **s)
{
    medstack_t *tempstack = NULL;
    medstack_t *snode = NULL;
    medtree_t *tnode = NULL;
    medtree_t *temp = NULL;
    
    tnode = t;
    _MEDTreeStack_PushNode(&tempstack, tnode);
    
    while (tempstack != NULL)
    {
        snode = MEDStack_Pop(&tempstack);
        tnode = (medtree_t *)snode->payload;
        free(snode);
        
        temp = tnode->child;
        while (temp != NULL)
        {
            _MEDTreeStack_PushNode(&tempstack, temp);
            temp = temp->sibling;
        }
        
        /* leaf */
        if (tnode->child == NULL)
            _MEDTreeStack_PushNode(s, tnode);
    }
}

/*
 * ************************************************************
 * silencer
 * ************************************************************
 */

void silent_printf(const char *fmt, ...)
{
    
}
