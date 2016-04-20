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

#include "ruiko_algorithm.h"
#include "memtracker.h"
#include <stdlib.h>

/*
 * ************************************************************
 * memory
 * ************************************************************
 */

void *_rk_def_malloc(size_t size) {
  return malloc(size);
}

void *_rk_def_realloc(void *ptr, size_t newsize) {
  return realloc(ptr, newsize);
}

void _rk_def_free(void *ptr) {
  free(ptr);
}

rk_mallocfunc rk_def_malloc = _rk_def_malloc;
rk_reallocfunc rk_def_realloc = _rk_def_realloc;
rk_freefunc rk_def_free = _rk_def_free;

void rk_algo_set_malloc(rk_mallocfunc func) {
  rk_def_malloc = func;
}

void rk_algo_set_realloc(rk_reallocfunc func) {
  rk_def_realloc = func;
}

void rk_algo_set_free(rk_freefunc func) {
  rk_def_free = func;
}

void rk_algo_def_free(void* ptr) {
  free(ptr);
}

/*
 * ************************************************************
 * helper function
 * ************************************************************
 */

/*
   https://compprog.wordpress.com/2007/10/17/generating-combinations-1/
   next_comb(int comb[], int k, int n)
   Generates the next combination of n elements as k after comb

   comb => the previous combination ( use (0, 1, 2, ..., k) for first)
   k => the size of the subsets to generate
   n => the size of the original set

   Returns: 1 if a valid combination was found
   0, otherwise
 */
int rk_next_comb(int comb[], int k, int n) {
  int i = k - 1;

  ++comb[i];

  while ((i > 0) && (comb[i] >= n - k + 1 + i)) {
    --i;
    ++comb[i];
  }

  if (comb[0] > n - k) /* Combination (n-k, n-k+1, ..., n) reached */
    return 0;          /* No more combinations can be generated */

  /* comb now looks like (..., x, n, n, n, ..., n).
     Turn it into (..., x, x + 1, x + 2, ...) */
  for (i = i + 1; i < k; ++i) comb[i] = comb[i - 1] + 1;

  return 1;
}

/*
 * ************************************************************
 * list
 * ************************************************************
 */

rk_list_t *rk_list_new(void) {
  rk_list_t *l = (rk_list_t *) rk_def_malloc(sizeof(rk_list_t));
  l->next = NULL;
  l->prev = NULL;

  return l;
}

void rk_list_delete(rk_list_t **l, rk_algo_free delfunc) {
  rk_list_t *current = *l;
  rk_list_t *next;

  while (current != NULL) {
    next = current->next;

    if (delfunc != NULL) delfunc(current->payload);
    rk_def_free(current);
    current = next;
  }

  *l = NULL;
}

int rk_list_len(rk_list_t *l) {
  int length = 0;
  rk_list_t *tl = l;

  while (tl != NULL) {
    length++;
    tl = tl->next;
  }

  return length;
}

rk_list_t *rk_list_tail(rk_list_t *l) {
  rk_list_t *tail = l;

  if (l == NULL) {
    return l;
  }

  while (tail->next != NULL) {
    tail = tail->next;
  }

  return tail;
}

void rk_list_push_front(rk_list_t **l, rk_list_t *node) {
  if (*l != NULL) {
    node->prev = (*l)->prev;
    (*l)->prev = node;
  }
  node->next = *l;
  *l = node;
}

void rk_list_push_back(rk_list_t **l, rk_list_t *node) {
  rk_list_t *tail = *l;

  if (*l == NULL) {
    *l = node;
  }
  else {
    while (tail->next != NULL) tail = tail->next;

    tail->next = node;
    node->prev = tail;
  }
}

void rk_list_append(rk_list_t *l, rk_list_t *node) {
  l->next = node;
  node->prev = l;
}

void rk_list_concat(rk_list_t **l, rk_list_t *seg) {
  rk_list_t *tail;

  if (seg == NULL)
    return;

  if (*l == NULL) {
    *l = seg;
  } else {
    tail = rk_list_tail(*l);
    tail->next = seg;
    seg->prev = tail;
  }
}

int rk_list_remove(rk_list_t **l, rk_list_t *node) {
  int found = 0;
  rk_list_t *temp;

  /* 1. remove header */
  if (*l == node) {
    (*l) = (*l)->next;
    if (*l != NULL) (*l)->prev = NULL;
    return 1;
  }

  /* only one node, not found */
  if (((*l)->next == NULL) && (*l != node)) return 0;

  /* search in list */
  temp = *l;
  while (temp != NULL) {
    if (temp == node) {
      if (temp->prev != NULL) {
        temp->prev->next = temp->next;
      }
      if (temp->next != NULL) {
        temp->next->prev = temp->prev;
      }
      found = 1;
      break;
    }

    temp = temp->next;
  }

  return found;
}

rk_list_t *rk_list_search(rk_list_t *l, void *context, rk_algo_search search) {
  int found = 0;
  rk_list_t *result = NULL;

  result = l;

  while (result != NULL) {
    if (search(result->payload, context)) {
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

rk_stack_t *rk_stack_pop(rk_stack_t **s) {
  rk_stack_t *node = *s;

  *s = (*s)->next;
  if (*s != NULL) {
    (*s)->prev = NULL;
  }

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

void _rk_tree_stack_push(rk_stack_t **s, rk_tree_t *node) {
  rk_stack_t *snode = (rk_stack_t *) rk_def_malloc(sizeof(rk_stack_t));
  snode->prev = NULL;
  snode->next = NULL;

  snode->payload = node;
  rk_stack_push(s, snode);
}

rk_tree_t *rk_tree_new(void) {
  rk_tree_t *tree = (rk_tree_t *) rk_def_malloc(sizeof(rk_tree_t));
  tree->root = NULL;
  tree->child = NULL;
  tree->sibling = NULL;

  return tree;
}

void rk_tree_delete(rk_tree_t **t, rk_algo_free delfunc) {
  rk_stack_t *s = NULL;
  rk_stack_t *snode = NULL;
  rk_tree_t *tnode = NULL;
  rk_tree_t *temp = NULL;

  tnode = *t;
  _rk_tree_stack_push(&s, tnode);

  while (s != NULL) {
    snode = rk_stack_pop(&s);
    tnode = (rk_tree_t *) snode->payload;

    if (delfunc != NULL) delfunc(tnode->payload);
    rk_def_free(snode);

    temp = tnode->child;

    while (temp != NULL) {
      _rk_tree_stack_push(&s, temp);
      temp = temp->sibling;
    }

    rk_def_free(tnode);
  }
}

void rk_tree_add_child(rk_tree_t **t, rk_tree_t *node) {
  rk_tree_t *child = NULL;

  node->root = *t;

  if (*t != NULL) {
    child = (*t)->child;

    if (child == NULL) {
      (*t)->child = node;
    }
    else {
      while (child->sibling != NULL) child = child->sibling;

      child->sibling = node;
    }
  }
  else {
    *t = node;
  }
}

void rk_tree_add_sibling(rk_tree_t **t, rk_tree_t *node) {
  rk_tree_t *sibling = NULL;

  if (*t != NULL) {
    node->root = (*t)->root;

    sibling = *t;

    while (sibling->sibling != NULL) sibling = sibling->sibling;

    sibling->sibling = node;
  }
  else {
    node->root = NULL;
    (*t) = node;
  }
}

void rk_tree_dump_leaf(rk_tree_t *t, rk_stack_t **s) {
  rk_stack_t *tempstack = NULL;
  rk_stack_t *snode = NULL;
  rk_tree_t *tnode = NULL;
  rk_tree_t *temp = NULL;

  tnode = t;
  _rk_tree_stack_push(&tempstack, tnode);

  while (tempstack != NULL) {
    snode = rk_stack_pop(&tempstack);
    tnode = (rk_tree_t *) snode->payload;
    rk_def_free(snode);

    temp = tnode->child;

    while (temp != NULL) {
      _rk_tree_stack_push(&tempstack, temp);
      temp = temp->sibling;
    }

    /* leaf */
    if (tnode->child == NULL) _rk_tree_stack_push(s, tnode);
  }
}
