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
  center_node->parent = direction == 'L' ? center_node->right : center_node->left;
  if (original_parent == t->nil)
  {
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
    center_node->right->parent = original_parent;
    node_t *temp = center_node->right->left;
    center_node->right->left = center_node;
    center_node->right = temp;
    if (temp != t->nil)
    {
      temp->parent = center_node;
    }
  }
  else
  {
    center_node->left->parent = original_parent;
    node_t *temp = center_node->left->right;
    center_node->left->right = center_node;
    center_node->left = temp;
    if (temp != t->nil)
    {
      temp->parent = center_node;
    }
  }
  return 0;
}

void fix_insert_violate(rbtree *t, node_t *center_node)
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
      fix_insert_violate(t, grand_parent);
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
      fix_insert_violate(t, center_node);
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
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
    fix_insert_violate(t, new_node);
  }

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  node_t *x = t->root;
  while (x != t->nil)
  {
    if (key < x->key)
    {
      x = x->left;
    }
    else if (key > x->key)
    {
      x = x->right;
    }
    else
    {
      return x;
    }
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t)
{
  if (t->root == t->nil)
  {
    return NULL;
  }
  node_t *x = t->root;
  while (x->left != t->nil)
  {
    x = x->left;
  }
  return x;
}

node_t *rbtree_max(const rbtree *t)
{
  if (t->root == t->nil)
  {
    return NULL;
  }
  node_t *x = t->root;
  while (x->right != t->nil)
  {
    x = x->right;
  }
  return x;
}

int rbtree_erase(rbtree *t, node_t *p)
{

  return 0;
}

int rbtree_inorder(node_t *node_now, const rbtree *t, int max_num, key_t *arr, int cnt)
{
  if (node_now->left != t->nil)
  {
    cnt = rbtree_inorder(node_now->left, t, max_num, arr, cnt);
  }
  arr[cnt++] = node_now->key;
  if (cnt >= max_num)
  {
    return cnt;
  }
  if (node_now->right != t->nil)
  {
    cnt = rbtree_inorder(node_now->right, t, max_num, arr, cnt);
  }
  return cnt;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  rbtree_inorder(t->root, t, n, arr, 0);
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
  rbtree_insert(t, 8);
  rbtree_insert(t, 5);
  rbtree_insert(t, 15);
  rbtree_insert(t, 12);
  rbtree_insert(t, 9);
  rbtree_insert(t, 13);
  rbtree_insert(t, 19);
  rbtree_insert(t, 23);
  rbtree_insert(t, 10);
  // preorder_print(t->root, t);
  // node_t *found = rbtree_find(t, 10);
  // printf("found key:%d color:%d parent:%d", found->key, found->color, found->parent->key);
  // node_t *min_node = rbtree_min(t);
  // node_t *max_node = rbtree_max(t);
  // printf("min: key:%d color:%d\n", min_node->key, min_node->color);
  // printf("max: key:%d color:%d\n", max_node->key, max_node->color);
  // int arr[] = {0, 0, 0, 0, 0, 0, 0, 0};
  // rbtree_to_array(t, arr, 8);
  // for (int i = 0; i < 8; i++)
  // {
  //   printf("%d\n", arr[i]);
  // }
  // delete_rbtree(t);
  return 0;
}