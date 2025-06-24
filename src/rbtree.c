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

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
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
