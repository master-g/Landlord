/*
The MIT License (MIT)

Copyright (c) 2014 Master.G

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef LANDLORD_MEDALGO_H_
#define LANDLORD_MEDALGO_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

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

typedef struct medlist_s
{
    void *payload;
    struct medlist_s *next;
    
} medlist_t;

/* functions */

medlist_t *MEDList_Create(void);

void MEDList_Destroy(medlist_t **l, MEDAlgoFunc_Delete delfunc);

int MEDList_Length(medlist_t *l);

void MEDList_PushFront(medlist_t **l, medlist_t *node);

void MEDList_PushBack(medlist_t **l, medlist_t *node);

int MEDList_Remove(medlist_t **l, medlist_t *node);

medlist_t *MEDList_Find(medlist_t *l, void *context, MEDAlgoFunc_Find finder);

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

typedef struct medtree_s
{
    void *payload;
    struct medtree_s *root;
    struct medtree_s *child;
    struct medtree_s *sibling;
    
} medtree_t;

medtree_t *MEDTree_Create(void);

void MEDTree_Destroy(medtree_t **t, MEDAlgoFunc_Delete delfunc);

void MEDTree_AddChild(medtree_t **t, medtree_t *node);

void MEDTree_AddSibling(medtree_t **t, medtree_t *node);

void MEDTree_DumpLeafToStack(medtree_t *t, medstack_t **stack);

#ifdef __cplusplus
}
#endif

#endif /* LANDLORD_MEDALGO_H_ */
