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
#include "common.h"

#define WINVER
#ifndef WINVER
#include <execinfo.h>

/* ************************************************************
 * debug
 * ************************************************************/

typedef struct _history_entry_s {
  void *addr;
  void *callstack[128];
  int frames;
  struct _history_entry_s *next;

} history_entry_t;

typedef struct _history_s {
  history_entry_t *first;

} history_t;

static history_t history = {NULL};

void history_mark(void *addr) {
  history_entry_t *entry = calloc(1, sizeof(history_entry_t));
  entry->addr = addr;
  entry->frames = backtrace(entry->callstack, 128);
  entry->next = history.first;
  history.first = entry;
}

void history_unmark(void *addr) {
  history_entry_t *prev = NULL, *iter = history.first;

  while (iter != NULL && iter->addr != addr) {
    prev = iter;
    iter = iter->next;
  }

  if (iter != NULL) {
    if (prev != NULL) {
      prev->next = iter->next;
    } else {
      history.first = iter->next;
    }

    free(iter);
  }
}

void history_purge() {
  history_entry_t *iter = history.first;

  while (iter != NULL) {
    int i;
    char **strs = backtrace_symbols(iter->callstack, iter->frames);

    printf("-------------------------------\n");
    for (i = 0; i < iter->frames; i++) {
      printf("%s\n", strs[i]);
    }
    free(strs);

    iter = iter->next;
  }
}
#else
void history_mark(void *addr) {}
void history_unmark(void *addr) {}
void history_purge() {}
#endif

/* ************************************************************
 * list
 * ************************************************************/

rk_list_t *rk_list_create(void) {
  rk_list_t *list = calloc(1, sizeof(rk_list_t));
  history_mark(list);
  return list;
}

void rk_list_destroy(rk_list_t *list) {
  history_unmark(list);

  rk_list_foreach(list, first, next, cur) {
    if (cur->prev) {
      free(cur->prev);
    }
  }

  free(list->last);
  free(list);
}

void rk_list_clear(rk_list_t *list) {
  rk_list_foreach(list, first, next, cur) {
    free(cur->payload);
  }
}

void rk_list_clear_destroy(rk_list_t *list) {
  if (list) {
    rk_list_clear(list);
    rk_list_destroy(list);
  }
}

void rk_list_push(rk_list_t *list, void *payload) {
  rk_list_node_t *node = calloc(1, sizeof(rk_list_node_t));
  check_mem(node);

  node->payload = payload;

  if (list->last == NULL) {
    list->first = node;
    list->last = node;
  } else {
    list->last->next = node;
    node->prev = list->last;
    list->last = node;
  }

  list->count++;

  error:
  return;
}

void *rk_list_pop(rk_list_t *list) {
  rk_list_node_t *node = list->last;
  return node != NULL ? rk_list_remove(list, node) : NULL;
}

void rk_list_unshift(rk_list_t *list, void *payload) {
  rk_list_node_t *node = calloc(1, sizeof(rk_list_node_t));
  check_mem(node);

  node->payload = payload;
  if (list->last == NULL) {
    list->first = node;
    list->last = node;
  } else {
    node->next = list->first;
    list->first->prev = node;
    list->first = node;
  }

  list->count++;

  error:
  return;
}

void *rk_list_shift(rk_list_t *list) {
  rk_list_node_t *node = list->first;
  return node != NULL ? rk_list_remove(list, node) : NULL;
}

void rk_list_concat(rk_list_t *head, rk_list_t *tail) {
  if (tail->count == 0) {
    return;
  }

  if (head->last == NULL) {
    head->first = tail->first;
    head->last = tail->last;
  } else {
    head->last->next = tail->first;
    tail->first->prev = head->last;
    head->last = tail->last;
  }

  head->count += tail->count;
}

void *rk_list_remove(rk_list_t *list, rk_list_node_t *node) {
  void *result = NULL;

  check(list->first && list->last, "remove from an empty list");
  check(node, "remove NULL from list");

  if (node == list->first && node == list->last) {
    list->first = NULL;
    list->last = NULL;
  } else if (node == list->first) {
    list->first = node->next;
    check(list->first != NULL, "invalid list with non-null first");
    list->first->prev = NULL;
  } else if (node == list->last) {
    list->last = node->prev;
    check(list->last != NULL, "invalid list with non-null last");
    list->last->next = NULL;
  } else {
    rk_list_node_t *after = node->next;
    rk_list_node_t *before = node->prev;
    after->prev = before;
    before->next = after;
  }

  list->count--;
  result = node->payload;
  free(node);

  error:
  return result;
}

void *rk_list_search(rk_list_t *list, void *context, rk_algo_search search) {
  int found = 0;

  check(list->first && list->last, "search from an empty list");

  {
    rk_list_foreach(list, first, next, cur) {
      if (search(cur->payload, context)) {
        found = 1;
        break;
      }
    }

    error:
    return found == 1 ? cur->payload : NULL;
  }
}

/* ************************************************************
 * tree
 * ************************************************************/

rk_tree_t *rk_tree_create(void *payload) {
  rk_tree_t *tree = (rk_tree_t *) calloc(1, sizeof(rk_tree_t));
  check_mem(tree);
  tree->payload = payload;

  error:
  return tree;
}

void _rk_tree_free(void *p) {
  free(p);
}

void rk_tree_clear(rk_tree_t *tree) {
  rk_tree_levelorder(tree, _rk_tree_free);
}

void rk_tree_destroy(rk_tree_t *tree) {
  rk_list_t *list = rk_list_create();
  rk_tree_dump(tree, list);
  {
    rk_list_foreach(list, first, next, cur) {
      rk_tree_t *tn = cur->payload;
      free(tn);
    }
  }

  rk_list_destroy(list);
}

void rk_tree_clear_destroy(rk_tree_t *tree) {
  rk_list_t *list = rk_list_create();
  rk_tree_dump(tree, list);
  {
    rk_list_foreach(list, first, next, cur) {
      rk_tree_t *tn = cur->payload;
      free(tn->payload);
      free(tn);
    }
  }

  rk_list_destroy(list);
}

rk_tree_t *rk_tree_add_child(rk_tree_t *node, void *payload) {
  rk_tree_t *newnode = calloc(1, sizeof(rk_tree_t));
  check_mem(newnode);

  newnode->payload = payload;
  if (node->child) {
    newnode->sibling = node->child;
  }

  node->child = newnode;
  newnode->parent = node;

  error:
  return newnode;
}

rk_tree_t *rk_tree_add_sibling(rk_tree_t *node, void *payload) {
  rk_tree_t *newnode = calloc(1, sizeof(rk_tree_t));
  check_mem(newnode);

  newnode->payload = payload;
  newnode->sibling = node->sibling;
  node->sibling = newnode;
  newnode->parent = node->parent;

  error:
  return newnode;
}

void rk_tree_dump(rk_tree_t *tree, rk_list_t *list) {
  rk_list_t *q = NULL;

  q = rk_list_create();
  rk_list_unshift(q, tree);

  while (!rk_list_empty(q)) {
    rk_tree_t *child = NULL;
    rk_tree_t *v = rk_list_shift(q);

    rk_list_push(list, v);

    child = v->child;
    while (child) {
      rk_list_unshift(q, child);
      child = child->sibling;
    }
  }

  rk_list_destroy(q);
}

void rk_tree_dump_leaves(rk_tree_t *tree, rk_list_t *list) {
  rk_list_t *stack = NULL;
  rk_tree_t *node = NULL;
  rk_tree_t *temp = NULL;

  stack = rk_list_create();
  check_mem(stack);

  rk_list_push(stack, tree);

  while (!rk_list_empty(stack)) {
    node = rk_list_pop(stack);
    temp = node->child;

    while (temp != NULL) {
      rk_list_push(stack, temp);
      temp = temp->sibling;
    }

    if (node->child == NULL)
      rk_list_push(list, node);
  }

  rk_list_destroy(stack);

  error:
  return;
}

void rk_tree_levelorder(rk_tree_t *tree, rk_tree_visitor visitor) {
  rk_list_t *q = NULL;

  q = rk_list_create();
  rk_list_unshift(q, tree);

  while (!rk_list_empty(q)) {
    rk_tree_t *child = NULL;
    rk_tree_t *v = rk_list_shift(q);
    visitor(v->payload);

    child = v->child;
    while (child) {
      rk_list_unshift(q, child);
      child = child->sibling;
    }
  }

  rk_list_destroy(q);
}
