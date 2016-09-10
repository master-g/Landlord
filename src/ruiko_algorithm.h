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

/* ************************************************************
 * function type
 * ************************************************************/

/* free function */
typedef void (*rk_algo_free)(void *payload);

/* search function */
typedef int (*rk_algo_search)(void *payload, void *context);

/* visit function */
typedef void (*rk_tree_visitor)(void *payload);

/* ************************************************************
 * list
 * ************************************************************/

typedef struct _rk_list_node_s {
  void *payload;
  struct _rk_list_node_s *prev;
  struct _rk_list_node_s *next;

} rk_list_node_t;

typedef struct _rk_list_s {
  int count;
  rk_list_node_t *first;
  rk_list_node_t *last;

} rk_list_t;

/* functions */

#define rk_list_count(l) ((l)->count)
#define rk_list_first(l) ((l)->first != NULL ? (l)->first->payload : NULL)
#define rk_list_last(l) ((l)->last != NULL ? (l)->last->payload : NULL)
#define rk_list_empty(l) ((l)->count == 0)

#define rk_list_foreach(L, S, M, V) rk_list_node_t *_rkfn_ = NULL; \
  rk_list_node_t *V = NULL; \
  for (V = _rkfn_ = L->S; _rkfn_ != NULL; V = _rkfn_ = _rkfn_->M)

rk_list_t *rk_list_create(void);

void rk_list_destroy(rk_list_t *list);

void rk_list_clear(rk_list_t *l);

void rk_list_clear_destroy(rk_list_t *list);

void rk_list_push(rk_list_t *list, void *payload);

void *rk_list_pop(rk_list_t *list);

void rk_list_unshift(rk_list_t *list, void *payload);

void *rk_list_shift(rk_list_t *list);

void rk_list_concat(rk_list_t *head, rk_list_t *tail);

void *rk_list_remove(rk_list_t *list, rk_list_node_t *node);

void *rk_list_search(rk_list_t *list, void *context, rk_algo_search search);

/* ************************************************************
 * tree
 * ************************************************************/

typedef struct _rk_tree_node_s {
  void *payload;
  struct _rk_tree_node_s *parent;
  struct _rk_tree_node_s *child;
  struct _rk_tree_node_s *sibling;

} rk_tree_t;

rk_tree_t *rk_tree_create(void *payload);

void rk_tree_clear(rk_tree_t *tree);

void rk_tree_destroy(rk_tree_t *tree);

void rk_tree_clear_destroy(rk_tree_t *tree);

rk_tree_t *rk_tree_add_child(rk_tree_t *node, void *payload);

rk_tree_t *rk_tree_add_sibling(rk_tree_t *node, void *payload);

void rk_tree_dump(rk_tree_t *tree, rk_list_t *list);

void rk_tree_dump_leaves(rk_tree_t *tree, rk_list_t *list);

void rk_tree_levelorder(rk_tree_t *tree, rk_tree_visitor visitor);

void history_purge();

#ifdef __cplusplus
}
#endif

#endif /* RUIKO_ALGORITHM_H */
