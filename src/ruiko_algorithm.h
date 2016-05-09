/*
The MIT License (MIT)

Copyright (c) 2014 Master.G

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in 
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef RUIKO_ALGORITHM_H
#define RUIKO_ALGORITHM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "memtracker.h"

/*
 * ************************************************************
 * function type
 * ************************************************************
 */

/* search function */
typedef int (*rk_algo_search)(void *payload, void *context);

/* free function */
typedef void (*rk_algo_free)(void *payload);

/* default dealloctor */
void rk_algo_def_free(void *payload);

/*
 * ************************************************************
 * memory
 * ************************************************************
 */

typedef void* (*rk_mallocfunc)(unsigned long);
typedef void* (*rk_reallocfunc)(void*, unsigned long);
typedef void (*rk_freefunc)(void*);

extern rk_mallocfunc rk_def_malloc;
extern rk_reallocfunc rk_def_realloc;
extern rk_freefunc rk_def_free;

void rk_algo_set_malloc(rk_mallocfunc);
void rk_algo_set_realloc(rk_reallocfunc);
void rk_algo_set_free(rk_freefunc);

/*
 * ************************************************************
 * helper function
 * ************************************************************
 */

int rk_next_comb(int comb[], int k, int n);

/*
 * ************************************************************
 * list
 * ************************************************************
 */

typedef struct _rk_list_s {
  void *payload;
  struct _rk_list_s *prev;
  struct _rk_list_s *next;

} rk_list_t;

/* functions */

rk_list_t *rk_list_new(void);

void rk_list_delete(rk_list_t **l, rk_algo_free delfunc);

int rk_list_len(rk_list_t *l);

rk_list_t *rk_list_tail(rk_list_t *l);

void rk_list_push_front(rk_list_t **l, rk_list_t *node);

void rk_list_push_back(rk_list_t **l, rk_list_t *node);

void rk_list_append(rk_list_t *l, rk_list_t *node);

void rk_list_concat(rk_list_t **l, rk_list_t *seg);

int rk_list_remove(rk_list_t **l, rk_list_t *node);

rk_list_t *rk_list_search(rk_list_t *l, void *context, rk_algo_search search);

/*
 * ************************************************************
 * stack
 * ************************************************************
 */
#define rk_stack_push(s, n) rk_list_push_front((s), (n))

typedef rk_list_t rk_stack_t;

rk_stack_t *rk_stack_pop(rk_stack_t **s);

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

typedef struct _rk_tree_s {
  void *payload;
  struct _rk_tree_s *root;
  struct _rk_tree_s *child;
  struct _rk_tree_s *sibling;

} rk_tree_t;

rk_tree_t *rk_tree_new(void);

void rk_tree_delete(rk_tree_t **t, rk_algo_free delfunc);

void rk_tree_add_child(rk_tree_t **t, rk_tree_t *node);

void rk_tree_add_sibling(rk_tree_t **t, rk_tree_t *node);

void rk_tree_dump_leaf(rk_tree_t *t, rk_stack_t **stack);

#ifdef __cplusplus
}
#endif

#endif /* RUIKO_ALGORITHM_H */
