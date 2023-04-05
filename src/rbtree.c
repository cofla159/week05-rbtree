#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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

  return new_node;
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

node_t *find_smaller_max(const node_t *p, rbtree *t)
{
  if (p->left == t->nil)
  {
    return t->nil;
  }
  node_t *x = p->left;
  while (x->right != t->nil)
  {
    x = x->right;
  }
  return x;
}
void fix_erase_violate(rbtree *t, node_t *doubly_black)
{
  if (doubly_black->parent == t->nil)
  {
    return;
  }
  if (doubly_black->color == RBTREE_RED)
  {
    doubly_black->color = RBTREE_BLACK;
  }
  else
  {
    node_t *sibling;
    bool is_sibling_right_child;
    if (doubly_black->parent->right == doubly_black)
    {
      sibling = doubly_black->parent->left;
      is_sibling_right_child = false;
    }
    else
    {
      sibling = doubly_black->parent->right;
      is_sibling_right_child = true;
    }
    if (sibling->color == RBTREE_RED)
    { // case1
      color_t temp_color = sibling->color;
      sibling->color = 1 - sibling->parent->color;
      sibling->parent->color = 1 - temp_color;
      rotate(sibling->parent, t, is_sibling_right_child ? 'L' : 'R');
      fix_erase_violate(t, doubly_black);
    }
    else if (sibling->color == RBTREE_BLACK && sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK) // case2
    {
      // sibling->color == RBTREE_RED;
      fix_erase_violate(t, doubly_black->parent);
    }
    else if (sibling->color == RBTREE_BLACK)
    {
      if (is_sibling_right_child && sibling->left->color == RBTREE_RED && sibling->right->color == RBTREE_BLACK)
      { // case3, 오른쪽 형제
        color_t temp_color = sibling->color;
        sibling->color = 1 - sibling->left->color;
        sibling->left->color = 1 - temp_color;
        rotate(sibling, t, 'R');
        fix_erase_violate(t, doubly_black);
      }
      else if (!is_sibling_right_child && sibling->right->color == RBTREE_RED && sibling->left->color == RBTREE_BLACK)
      { // case3, 왼쪽 형제
        color_t temp_color = sibling->color;
        sibling->color = 1 - sibling->right->color;
        sibling->right->color = 1 - temp_color;
        rotate(sibling, t, 'L');
        fix_erase_violate(t, doubly_black);
      }
      else if (is_sibling_right_child && sibling->right->color == RBTREE_RED)
      { // case4, 오른쪽 형제
        sibling->color = sibling->parent->color;
        sibling->right->color = RBTREE_BLACK;
        sibling->parent->color = RBTREE_BLACK;
        rotate(sibling->parent, t, 'L');
      }
      else
      { // case4, 왼쪽 형제
        sibling->color = sibling->parent->color;
        sibling->left->color = RBTREE_BLACK;
        sibling->parent->color = RBTREE_BLACK;
        rotate(sibling->parent, t, 'R');
      }
    }
  }
}

int rbtree_erase(rbtree *t, node_t *p)
{
  node_t *substitute;
  node_t *doubly_black;
  if (p->left == t->nil || p->right == t->nil)
  {
    substitute = p->left == t->nil ? p->right : p->left;
    doubly_black = substitute;
    if (p->parent == t->nil)
    {
      t->root = substitute; // 루트 삭제할 때
    }
    else if (p == p->parent->right) // 삭제하는 p가 오른쪽 자식일 때
    {
      p->parent->right = substitute; // z 없애고 대체제 붙여주기
    }
    else // 삭제하는 p가 왼쪽 자식일 때
    {
      p->parent->left = substitute;
    }
  }
  else
  {
    substitute = find_smaller_max(p, t); // 12
    doubly_black = substitute->left;     // nil
    if (p->parent == t->nil)
    {
      t->root = substitute; // 루트 삭제할 때
    }
    else if (p == p->parent->right) // 삭제하는 p가 오른쪽 자식일 때
    {
      if (substitute->parent == p)
      {
        doubly_black->parent = substitute; // 책에서는 x.p = y
        p->parent->right = substitute;
        substitute->parent = p->parent;
        substitute->right = p->right;
        p->right->parent = substitute;
      }
      else
      {
        substitute->parent->right = substitute->left;
        substitute->left->parent = substitute->parent;
        p->parent->right = substitute;
        substitute->parent = p->parent;
        substitute->left = p->left;
        p->left->parent = substitute;
        substitute->right = p->right;
      }
    }
    else // 삭제하는 p가 왼쪽 자식일 때: 걍 l/r 반전만 시킴.. 다시보기
    {
      if (substitute->parent == p)
      {
        doubly_black->parent = substitute; // 책에서는 x.p = y
        p->parent->left = substitute;
        substitute->parent = p->parent;
        substitute->left = p->left;
        p->left->parent = substitute;
      }
      else
      {
        substitute->parent->left = substitute->right;
        substitute->right->parent = substitute->parent;
        p->parent->left = substitute;
        substitute->parent = p->parent;
        substitute->right = p->right;
        p->right->parent = substitute;
        substitute->left = p->left;
      }
    }
  }
  substitute->parent = p->parent; // 대체제의 부모도 이어주기
  color_t erased_color = substitute->color;
  substitute->color = p->color; // 원래의 색 덮어씌우기
  if (erased_color == RBTREE_BLACK)
  {
    fix_erase_violate(t, doubly_black);
  }
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
  printf("key: %d, color: %c, pointer: %p\n", root->key, node_color, root);
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
  // rbtree_insert(t, 12);
  // rbtree_insert(t, 8);
  // rbtree_insert(t, 1);
  // rbtree_insert(t, 9);
  // rbtree_insert(t, 10);
  // rbtree_insert(t, 15);
  // node_t *b = rbtree_insert(t, 13);
  // rbtree_insert(t, 23);

  // // node_t *found = rbtree_find(t, 10);
  // // printf("found key:%d color:%d parent:%d", found->key, found->color, found->parent->key);
  // // node_t *min_node = rbtree_min(t);
  // // node_t *max_node = rbtree_max(t);
  // // printf("min: key:%d color:%d\n", min_node->key, min_node->color);
  // // printf("max: key:%d color:%d\n", max_node->key, max_node->color);
  // // int arr[] = {0, 0, 0, 0, 0, 0, 0, 0};
  // // rbtree_to_array(t, arr, 8);
  // // for (int i = 0; i < 8; i++)
  // // {
  // //   printf("%d\n", arr[i]);
  // // }
  // rbtree_erase(t, b);
  // preorder_print(t->root, t);
  const key_t arr[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
  const size_t n = sizeof(arr) / sizeof(arr[0]);
  for (int i = 0; i < n; i++)
  {
    node_t *p = rbtree_insert(t, arr[i]);
  }
  preorder_print(t->root, t);

  for (int i = 0; i < n; i++)
  {
    node_t *p = rbtree_find(t, arr[i]);
    printf("arr[%d] = %d\n", i, p->key);
    rbtree_erase(t, p);
    preorder_print(t->root, t);
    printf("\n");
  }
  delete_rbtree(t);
  return 0;
}