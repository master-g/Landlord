/*
 *  medalgo.h
 *  Landlord
 *
 *  Created by Master.G on 14-3-30.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#ifndef LANDLORD_MEDALGO_H_
#define LANDLORD_MEDALGO_H_

/*
 * ************************************************************
 * function
 * ************************************************************
 */

/* search function */
typedef int (*MEDAlgoFunc_Find)(void* payload, void* context);

/* destroy function */
typedef void (*MEDAlgoFunc_Delete)(void *payload);

void MEDAlgo_StandardFree(void *payload);

/*
 * ************************************************************
 * list
 * ************************************************************
 */

typedef struct medlist_t
{
    void *payload;
    struct medlist_t *next;
    
} medlist_t;

/* functions */

medlist_t *MEDList_Create(void);

void MEDList_Destroy(medlist_t **l, MEDAlgoFunc_Delete delfunc);

int MEDList_Length(medlist_t *l);

void MEDList_PushFront(medlist_t **l, medlist_t *node);

void MEDList_PushBack(medlist_t **l, medlist_t *node);

int MEDList_Remove(medlist_t **l, medlist_t *node);

medlist_t *MEDList_Find(medlist_t *l, void *context, MEDAlgoFunc_Find finder);

void MEDList_Test(void);

/*
 * ************************************************************
 * stack
 * ************************************************************
 */
#define MEDStack_Push(s, n) MEDList_PushFront((s), (n))

typedef medlist_t medstack_t;

medstack_t *MEDStack_Pop(medstack_t **s);

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

typedef struct medtree_t
{
    void *payload;
    struct medtree_t *root;
    struct medtree_t *child;
    struct medtree_t *sibling;
    
} medtree_t;

medtree_t *MEDTree_Create(void);

void MEDTree_Destroy(medtree_t **t, MEDAlgoFunc_Delete delfunc);

void MEDTree_AddChild(medtree_t **t, medtree_t *node);

void MEDTree_AddSibling(medtree_t **t, medtree_t *node);

void MEDTree_Test(void);

#endif /* LANDLORD_MEDALGO_H_ */
