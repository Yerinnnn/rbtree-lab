#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  // 트리 구조체 메모리 할당
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed

  // nil 노드 생성
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));
  nil_node->color = RBTREE_BLACK;  // nil은 항상 black
  // nil_node->left = nil_node->right = nil_node->parent = NULL;  // 명시적으로 적어주는 게 좋을까, 아닐까?

  // 성종 오빠가 추가한 부분
  // nil_node->key = 0;  // 오빠는 malloc을 사용했고, 그럴 때는 이 멤버를 사용하지 않아도 꼭 초기화 해주는 편임 (malloc 할 때는 모든 멤버를 꼭 초기화 해주는 게 좋음 -> 어떤 에러가 발생할지 모르니까)
  // nil_node->left = nil_node;
  // nil_node->right = nil_node;
  // nil_node->parent = nil_node;

  // 트리 설정
  p->nil = nil_node;
  p->root = nil_node;

  return p;
}

void delete_node(rbtree *t, node_t *n) {
  if (n == t->nil) return;
  
  delete_node(t, n->left);     // 왼쪽 서브트리 제거
  delete_node(t, n->right);    // 오른쪽 서브트리 제거
  free(n);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (!t) return;
  delete_node(t, t->root);
  free(t->nil);
  free(t);
}

void left_rotate(rbtree *t, node_t *pivot) {
  node_t *right_child = pivot->right;

  pivot->right = right_child->left;
  if (right_child->left != t->nil) {
    right_child->left->parent = pivot;
  }

  right_child->parent = pivot->parent;

  if (pivot->parent == t->nil) {
    t->root = right_child;
  } else if (pivot == pivot->parent->left) {
    pivot->parent->left = right_child;
  } else {
    pivot->parent->right = right_child;
  }

  right_child->left = pivot;
  pivot->parent = right_child;
}

void right_rotate(rbtree *t, node_t *pivot) {
  node_t *left_child = pivot->left;

  // pivot의 왼쪽 서브트리를 left_child의 오른쪽 서브트리로 바꿈
  pivot->left = left_child->right;
  if (left_child->right != t->nil) {
    left_child->right->parent = pivot;
  }
  
  // left_child의 부모를 pivot의 부모로 변경
  left_child->parent = pivot->parent; 

  // pivot이 루트였다면 트리의 루트 갱신
  if (pivot->parent == t->nil) {
    t->root = left_child;
  } else if (pivot == pivot->parent->left) {
    pivot->parent->left = left_child;
  } else {
    pivot->parent->right = left_child;
  }

  // left_child의 오른쪽 자식을 pivot으로
  left_child->right = pivot;
  pivot->parent = left_child;
}

void insert_fixup(rbtree *t, node_t *current) {
  while (current->parent->color == RBTREE_RED) {
    node_t *parent = current->parent;
    node_t *grandparent = parent->parent;

    // 부모가 할아버지의 왼쪽 자식일 경우
    if (parent == grandparent->left) {
      node_t *uncle = grandparent->right;
      
      // Case 1: 부모와 삼촌이 RED
      if (uncle->color == RBTREE_RED) {
        // 부모/삼촌 모두 BLACK으로 바꾸고, 할아버지를 RED로 변경
        parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        grandparent->color = RBTREE_RED;

        // 할아버지에서 다시 시작
        current = grandparent;
      }
      // Case 2/3: 삼촌이 BLACK
      else {
        // Case 2: 꺾인 상태 (current가 오른쪽 자식)
        if (current == parent->right) {
          current = parent;
          // 왼쪽으로 회전
          left_rotate(t, current);
        }

        // Case 3: 일직선
        parent->color = RBTREE_BLACK;
        grandparent->color = RBTREE_RED;
        // 오른쪽으로 회전
        right_rotate(t, grandparent);
      }
    }
    // 부모가 할아버지의 오른쪽 자식일 경우
    else {
      node_t *uncle = grandparent->left;

      // Case 1: 부모와 삼촌이 RED
      if (uncle->color == RBTREE_RED) {
        parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        grandparent->color = RBTREE_RED;

        current = grandparent;
      }
      // Case 2/3: 삼촌이 BLACK
      else {
        // Case 2: 꺾인 상태 (current가 왼쪽 자식)
        if (current == parent->left) {
          current = parent;
          right_rotate(t, current);
        }

        // Case 3: 일직선
        parent->color = RBTREE_BLACK;
        grandparent->color = RBTREE_RED;
        left_rotate(t, grandparent);
      }
    }
  }
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *current = t->root;
  node_t *parent = t->nil;

  while (current != t->nil) {
    parent = current;
    if (key < current->key) {
      current = current->left;
    } else {
      current = current->right;
    }
  }

  node_t *new_node = calloc(1, sizeof(node_t));
  new_node->key = key;
  new_node->color = RBTREE_RED;
  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->parent = parent;

  // 트리가 비어 있는 경우,
  if (parent == t->nil) {
    // new_node를 트리의 루트로 지정
    t->root = new_node;
  } else if (key < parent->key) {
    parent->left = new_node;
  } else {
  parent->right = new_node;
  }

  insert_fixup(t, new_node);
  
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
