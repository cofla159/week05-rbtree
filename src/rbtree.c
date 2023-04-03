#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  if (p == NULL)
  {
    return NULL;
  }
  p->root = (node_t *)calloc(1, sizeof(node_t));
  if (p->root == NULL)
  {
    free(p);
    return NULL;
  }
  p->root->color = RBTREE_BLACK;
  node_t *sentinel_node = (node_t *)calloc(1, sizeof(node_t));
  if (sentinel_node == NULL)
  {
    free(p);
    free(p->root);
    return NULL;
  }
  sentinel_node->color = RBTREE_BLACK;
#define SENTINEL sentinel_node
  p->nil = SENTINEL;
  p->root = p->nil;
  return p;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}