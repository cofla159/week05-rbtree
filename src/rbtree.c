#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void)
{
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));
  if (t == NULL)
  {
    return NULL;
  }
  t->nil = (node_t *)calloc(1, sizeof(node_t));
  if (t->nil == NULL)
  {
    free(t);
    return NULL;
  }
  t->nil->color = RBTREE_BLACK;
  t->nil->key = 0.0 / 0.0;
  t->root = t->nil;
  return t;
}

void free_node(node_t *node, rbtree *t)
{
  if (node->left != t->nil)
  {
    free_node(node->left, t);
  }
  if (node->right != t->nil)
  {
    free_node(node->right, t);
  }
  free(node);
}

void delete_rbtree(rbtree *t)
{
  if (t->root != t->nil)
  {
    free_node(t->root, t);
  }
  free(t->nil);
  free(t);
}

void *rotate(node_t *center_node, rbtree *t, char direction)
{
  node_t *original_parent = center_node->parent;
  center_node->parent = direction == 'L' ? center_node->right : center_node->left; // 8의 부모 = 12
  if (original_parent == t->nil)
  { // 8의 부모가 닐노드면 루트의 자식이 12
    t->root = direction == 'L' ? center_node->right : center_node->left;
  }
  else
  {
    if (original_parent->left == center_node)
    {
      original_parent->left = direction == 'L' ? center_node->right : center_node->left;
    }
    else
    {
      original_parent->right = direction == 'L' ? center_node->right : center_node->left;
    }
  }
  if (direction == 'L')
  {
    center_node->right->parent = original_parent; // 12의 부모=루트
    node_t *temp = center_node->right->left;      // 9
    center_node->right->left = center_node;       // 12의 왼쪽=8
    center_node->right = temp;                    // 8의 오른쪽 = 9
    if (temp != t->nil)
    {
      temp->parent = center_node; // 9의 부모=8
    }
  }
  else
  {
    center_node->left->parent = original_parent; // 12의 부모 = 8
    node_t *temp = center_node->left->right;
    center_node->left->right = center_node; // 12의 오른쪽=15
    center_node->left = temp;               // 15의 왼쪽 = 13
    if (temp != t->nil)
    {
      temp->parent = center_node; // 13의 부모 = 15
    }
  }
  return 0;
}

void fix_violate(rbtree *t, node_t *center_node)
{
  if (center_node->parent->color == RBTREE_RED)
  {
    node_t *grand_parent = center_node->parent->parent;
    node_t *uncle = center_node->parent == grand_parent->right ? grand_parent->left : grand_parent->right;
    if (uncle->color == RBTREE_RED) // case1
    {
      center_node->parent->color = 1 - center_node->parent->color;
      uncle->color = 1 - uncle->color;
      grand_parent->color = 1 - grand_parent->color;
      fix_violate(t, grand_parent);
    }
    else
    {
      if (grand_parent->right == center_node->parent && center_node->parent->left == center_node) // case2 '>'
      {
        rotate(center_node->parent, t, 'R');
        center_node->color = RBTREE_BLACK;
        center_node->parent->color = RBTREE_RED;
        rotate(center_node->parent, t, 'L');
      }
      else if (grand_parent->left == center_node->parent && center_node->parent->right == center_node) // case2 '<'
      {
        rotate(center_node->parent, t, 'L');
        center_node->color = RBTREE_BLACK;
        center_node->parent->color = RBTREE_RED;
        rotate(center_node->parent, t, 'R');
      }
      else if (grand_parent->right == center_node->parent && center_node->parent->right == center_node) // case3 '\'
      {
        rotate(grand_parent, t, 'L');
        center_node->parent->color = RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
      }
      else // case3 '/'
      {
        center_node->parent->color = RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
        rotate(grand_parent, t, 'R');
      }
      fix_violate(t, center_node);
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  node_t *new_node = (node_t *)malloc(sizeof(node_t));
  new_node->key = key;
  new_node->color = RBTREE_RED;
  new_node->left = t->nil;
  new_node->right = t->nil;
  node_t *new_parent = t->nil;
  node_t *x = t->root;
  while (x != t->nil)
  {
    new_parent = x;
    if (key > x->key)
    {
      x = x->right;
    }
    else
    {
      x = x->left;
    }
  }
  new_node->parent = new_parent;
  if (new_parent == t->nil) // case0
  {
    t->root = new_node;
    new_node->color = RBTREE_BLACK;
  }
  else
  {
    if (new_node->key < new_parent->key)
    {
      new_parent->left = new_node;
    }
    else
    {
      new_parent->right = new_node;
    }
    fix_violate(t, new_node);
  }

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

void preorder_print(node_t *root, rbtree *t)
{
  char node_color;
  if (root->color == RBTREE_RED)
  {
    node_color = 'r';
  }
  else
  {
    node_color = 'b';
  }
  printf("key: %d, color: %c\n", root->key, node_color);
  if (root->left != t->nil)
  {
    preorder_print(root->left, t);
  }
  if (root->right != t->nil)
  {
    preorder_print(root->right, t);
  }
}
int main()
{
  rbtree *t = new_rbtree();
  rbtree_insert(t, 50);
  rbtree_insert(t, 30);
  rbtree_insert(t, 10);
  rbtree_insert(t, 5);
  rbtree_insert(t, 20);
  rbtree_insert(t, 65);
  rbtree_insert(t, 98);
  preorder_print(t->root, t);

  delete_rbtree(t);
  return 0;
}