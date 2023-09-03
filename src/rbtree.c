#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

node_t *get_new_node(key_t key);
void rotate_left(rbtree *tree, node_t *node_x);
void show_tree(node_t *node);

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *new_node = get_new_node(key);
  node_t *insert_point = NULL;
  node_t *cur_node = t -> root;

  while(cur_node != NULL){
      insert_point = cur_node;
    
      if(new_node -> key < cur_node -> key){
          cur_node = cur_node -> left;
      } else {
          cur_node = cur_node -> right;
      }
  }

  new_node -> parent = insert_point;

  if(insert_point == NULL){
      t -> root = new_node;
  } else if(new_node -> key < insert_point -> key){
      insert_point -> left = new_node;
  } else {
      insert_point -> right = new_node;
  }
  // 새로운 노드는 red를 갖는다

  // 1. 루트 노를 시작으로 key 값을 비교해 left, right 타고 내려가서 insert 할 위치를 찾는다.
  // 2. 1번 과정을 진행 하면서 다른 포인터 변수에 현재 찾으면서 내려가는 주소 한 칸 뒤를 저장한다. (1번과 같이 진행)
  // (새로 insert 할 노드 자리를 기억해야하기 때문에)
  // 3 - 1. 1, 2번 과정을 끝냈는데 뒤 따라가던 포인터변수가 NULL(NIL) 이라면 비어있는 트리이기 때문에 루트에 삽입한다.
  // 3 - 2. key 값에 따라서 뒤 따라오던 포인터 변수의 left, right 어디에 삽입할지 결정한다.
  // (새로 삽입된 노드는 리프노드가 되기 때문에 자식 노드는 자연스럽게 NIL 노드를 갖는다.)
  // 4. RB Insert-Fixup 실행

  // 1. 루트 노드는 black 이여야 한다.
  // 2. red-red 위반 사항에 대해서 신경 써야한다.
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

node_t *get_new_node(key_t key){
  node_t *new_node = (node_t *) malloc(sizeof(node_t));

  new_node -> left = new_node -> right = new_node ->parent = NULL;
  new_node -> color = RBTREE_RED;
  new_node -> key = key;

  return new_node;
}

void rotate_left(rbtree *tree, node_t *node_x){
    node_t *node_y = node_x -> right;
    node_x -> right = node_y -> left;
    node_y -> left -> parent = node_x;
    node_y -> parent = node_x -> parent;

    if(node_x -> parent == NULL){
        tree -> root = node_y;
    } else if(node_x == node_x -> parent -> left){
        node_y -> parent -> left = node_y;
    } else {
        node_y -> parent -> right = node_y;
    }

    node_y -> left = node_x;
    node_x -> parent = node_y;
}

void show_tree(node_t *node){
    if(node == NULL){
        return;
    }

    if(node -> left != NULL){
        show_tree(node -> left);
    }

    printf("%d\n", node -> key);

    if(node -> right != NULL){
        show_tree(node -> right);
    }
}

// function left_rotate
// function  right_rotate

// (A4 용지 그림보고 할 것)
// 1. y 노드를 기억한다.
// 2. x의 오른쪽 자식은 '베타'가 된다. (x-y 연결을 끊는 개념)
// 3. y의 왼쪽 자식('베타')의 부모를 x로 한고, x의 오른쪽 자식은 y의 왼쪽 자식('베타')가 된다.
// 4 - 1. x의 부모를 y로 두고, x의 부모노드가 y의 부모노드가 된다.
// (x의 부모가 NIL 이라면(=root), y가 트리의 새로운 root가 된다.)
// 4 - 2. x가 부모의 왼쪽이면 y가 왼쪽, 오른쪽이면 오른쪽이 된다.
// 5. x가 y의 왼쪽 자식이 되고, y가 x의 부모가 된다.
// [ x의 부모, y의 부모, x의 오른쪽 자식, y의 왼쪽 자식, 부모의 자식 부분 ] ===> 주소 값 변경이 필요함

// function inerst_fixup