#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

void delete_all(rbtree *tree, node_t* node, node_t *nil);
void rotate_left(rbtree *tree, node_t *node_x);
void rotate_right(rbtree *tree, node_t *y);
void rebuild_after_insert(rbtree *tree, node_t *new_node);
node_t *get_new_node(rbtree *tree, key_t key);
node_t *get_root_node(const rbtree *tree);
node_t *get_nil_node(const rbtree *tree);
void show_single_node(node_t *node);
void show_tree(rbtree *tree, node_t *node);
void rebuild_after_erase(rbtree *t, node_t *x);
void rbtree_transplant(rbtree *t, node_t * u, node_t * v);

rbtree *new_rbtree(void) {
    rbtree *tree = (rbtree *) malloc(sizeof(rbtree));
    node_t *nil = (node_t *) malloc(sizeof(node_t));

    nil -> color = RBTREE_BLACK;
    tree -> nil = tree -> root = nil;

    return tree;
}

void delete_rbtree(rbtree *t) {
    node_t *root = get_root_node(t);
    node_t *nil = get_nil_node(t);

    if(root != t -> nil){
        delete_all(t, root, nil);
    }

    free(t -> nil);
    free(t);
}

void delete_all(rbtree *tree, node_t* node, node_t *nil){
    if(node -> left != nil){
        delete_all(tree, node -> left, nil);
    }

    if(node -> right != nil){
        delete_all(tree, node -> right, nil);
    }

    free(node);
    node = NULL;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    node_t *nil = get_nil_node(t);
    node_t *new_node = get_new_node(t, key);
    node_t *insert_point = nil;    // 삽입할 위치를 기억하기 위해서 탐색중인 노드의 이전 노드에 위치함
    node_t *cur_node = get_root_node(t);

    // 삽입 위치 탐색 반복문 key 값에 따라서 트리를 타고 내려감
    while(cur_node != nil){
        insert_point = cur_node;
    
       if(key < cur_node -> key){
           cur_node = cur_node -> left;
       } else {
           cur_node = cur_node -> right;
       }
    }

    new_node -> parent = insert_point;

    if(insert_point == nil){    // 첫 노드인 경우 루트 노드로 설정
       t -> root = new_node;
    } else if(key < insert_point -> key){    // 삽입 위치에서 오른쪽, 왼쪽 자식이 될지 구분
       insert_point -> left = new_node;
    } else {
       insert_point -> right = new_node;
    }

    rebuild_after_insert(t, new_node);
    return t -> root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
    node_t *cur_node = get_root_node(t);
    t -> nil -> key = key;

    while(cur_node -> key != key){
        cur_node = key > cur_node -> key ? cur_node -> right : cur_node -> left;
    }

    return cur_node == t -> nil ? NULL : cur_node;
}

node_t *rbtree_min(const rbtree *t) {
    node_t *cur_node = get_root_node(t);
 
    while(cur_node -> left != t -> nil){
        cur_node = cur_node -> left;
    }

    return cur_node;
}

node_t *rbtree_max(const rbtree *t) {
    node_t *cur_node = get_root_node(t);

    while(cur_node -> right != t -> nil){
        cur_node = cur_node -> right;
    }

    return cur_node;
}

void rbtree_transplant(rbtree *t, node_t * u, node_t * v) {
    if(u -> parent == t -> nil) {
        t -> root = v;
    } else if(u == u -> parent -> left) {
        u -> parent -> left = v;
    } else {
        u -> parent -> right = v;
    }
  
    v -> parent = u -> parent;
}

void rebuild_after_erase(rbtree *t, node_t *x) {
    node_t *w = NULL;
    
    while(x != t -> root && x -> color == RBTREE_BLACK) {
        if(x == x -> parent -> left){
            w = x -> parent -> right;
            
            // case 1 ~ 4
            if(w -> color == RBTREE_RED){    // case1: 형제 w가 RED
                w -> color = RBTREE_BLACK;
                x -> parent -> color = RBTREE_RED;
                rotate_left(t, x -> parent);
                w = x -> parent -> right;
            }

            // case2: 형제 w의 두 자식노드가 BLACK
            if(w -> left -> color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK) {
                w -> color = RBTREE_RED;
                x = x -> parent;
            } else {    // case3: 형제 w는 BLACK, w의 left 자식은 RED, w의 right 자식은 BLACK
                if(w -> right -> color == RBTREE_BLACK) {
                    w -> left -> color = RBTREE_BLACK;
                    w -> color = RBTREE_RED;
                    rotate_right(t, w);
                    w = x -> parent -> right;
                }

                w -> color = x -> parent -> color;
                x -> parent -> color = RBTREE_BLACK;
                w -> right -> color = RBTREE_BLACK;
                rotate_left(t, x -> parent);
                x = t -> root;
            }
        } else {    // case 5 ~8
            w = x -> parent -> left;

            // case5 : 형제 w가 RED
            if(w -> color == RBTREE_RED){
                w -> color = RBTREE_BLACK;
                x -> parent -> color = RBTREE_RED;
                rotate_right(t, x -> parent);
                w = x -> parent -> left;
            }

            // case6 : 형제 w는 BLACK, w의 두 지식노드가 BLACK
            if(w -> right -> color == RBTREE_BLACK && w -> left -> color == RBTREE_BLACK) {
                w -> color = RBTREE_RED;
                x = x -> parent;
            } else { // case7: 형제 w는 BLACK, w의 left 자식은 RED, right 자식은 BLACK
                if (w -> left -> color == RBTREE_BLACK) {
                    w -> right -> color = RBTREE_BLACK;
                    w -> color = RBTREE_RED;
                    rotate_left(t, w);
                    w = x -> parent -> left;
                }

                // case8 : 형제 w는 BLACK, w의 right 자식은 RED
                w -> color = x -> parent -> color;
                x -> parent -> color = RBTREE_BLACK;
                w -> left -> color = RBTREE_BLACK;
                rotate_right(t, x -> parent);
                x = t -> root;
            }
        }
    }

    x -> color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
    node_t *nil = get_nil_node(t);
    node_t *x = NULL;
    node_t *y = NULL;
    color_t y_org_color;

    y = p;
    y_org_color = y -> color;

    if(p -> left == nil) {          // left child node가 없음
        x = p -> right;
        rbtree_transplant(t, p, p -> right);
    } else if(p -> right == nil) {  // right child node가 없음
        x = p -> left;
        rbtree_transplant(t, p, p -> left);
    } else {                        // child node가 2개일 경우
        y = p -> right;             // successor를 찾기 위해서 해당 node의 right부터 서브트리 탐색
        
        while(y -> left != nil){
            y = y -> left;          // 삭제될 타겟이 successor
        }

        y_org_color = y -> color;
        x = y -> right;             // x: y의 자식으로 y삭제시 x가 y자리를 대체해야함

        if(y -> parent == p) {
            x -> parent = y;
        } else {
            rbtree_transplant(t, y, y -> right);
            y -> right = p -> right;
            y -> right -> parent = y;
        }

        rbtree_transplant(t, p, y);
        y -> left = p -> left;
        y -> left -> parent = y;
        y -> color = p -> color;
    }

    if(y_org_color == RBTREE_BLACK) {   // 이진검색 트리의 조건을 만족하기 때문에 RED노드를 삭제하는건 문제가 되지 않음 
        rebuild_after_erase(t, x);
    }

    free(p);
    return 0;
}

int rbtree_inorder(node_t *nil, node_t *root, key_t *arr, const size_t n,int idx) {
  if(root == nil || idx == n){
      return idx;
  }

  idx = rbtree_inorder(nil, root -> left, arr, n, idx);

  if(idx < n){
      arr[idx++] = root -> key;
  }

  idx = rbtree_inorder(nil, root -> right, arr, n, idx);

  return idx;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    if (t -> root == t -> nil){
        return 0;
    }

    rbtree_inorder(t -> nil, t -> root, arr, n, 0);
    return 0;
}

void rotate_left(rbtree *t, node_t *x){
    node_t *y = x -> right;
    node_t *nil = get_nil_node(t);
    x -> right = y -> left;          // y의 left node를 x의 right node로 만든다.
    y -> parent = x -> parent;       // y의 left node의 부모를 x로 설정한다.

    if (y -> left != nil){
        y -> left -> parent = x;     // y의 parent node를 x의 parent node로 설정한다.
    }

    if(x -> parent == nil){
        t -> root = y;               // x의 부모가 nil node인 경우 y를 root node로 설정한다.
    } else if(x == x -> parent -> left){
        x -> parent -> left = y;     // x가 x부모의 left node면 y가 x 부모의 left node가 되고
    } else {
        x -> parent -> right = y;    // x가 x부모의 right node면 y가 x 부모의 right node가 된다.
    }

    y -> left = x;      // y의 left node는 x가 된다.
    x -> parent = y;    // x의 paernt node는 y가 된다.
}

void rotate_right(rbtree *t, node_t *x){
    node_t *y = x -> left;
    node_t *nil = get_nil_node(t);
    x -> left = y -> right;
    y -> parent = x -> parent;

    if (y -> right != nil){
        y -> right -> parent = x;
    }

    if(x -> parent == nil){
        t -> root = y;
    } else if(x == x -> parent -> right){
        x -> parent -> right = y;
    } else {
        x -> parent -> left = y;
    }

    y -> right = x;
    x -> parent = y;
}

void rebuild_after_insert(rbtree *t, node_t *z){                // T: tree, z: new node
    node_t *y = t -> nil;

    while(z -> parent -> color == RBTREE_RED){                  // z의 parent node가 RED 라면
        if(z -> parent == z -> parent -> parent -> left){       // case: 1, 2, 3
            y = z -> parent -> parent -> right;                 // y: uncle node

            if(y -> color == RBTREE_RED){                       // case1: parent & uncle node가 RED, grand parent node가 BLACK
                z -> parent -> color = RBTREE_BLACK;            // parent node를 BLACK으로
                y -> color = RBTREE_BLACK;                      // uncle node를 BLACK으로 변경
                z -> parent -> parent -> color = RBTREE_RED;    // grand parent node를 RED로 변경

                z = z -> parent -> parent;                      // z를 다시 갱신 후 while 시작
            } else {    
                if(z == z -> parent -> right){                  // case: 2, 3
                    z = z -> parent;                            // z의 부모를 z로 설정 (case 2)
                    rotate_left(t, z);                          // z를 기준으로 left_rotate (case 2)
                }

                z -> parent -> color = RBTREE_BLACK;            // z의 parent node를 BLACK으로 설정(case 3)
                z -> parent -> parent -> color = RBTREE_RED;    // z의 grand parent node를 BLACK으로 설정(case 3)
                rotate_right(t, z -> parent -> parent);         // grand parent 기준으로 right_rotate
            }
        } else {                                                // case 4, 5, 6
            y = z -> parent -> parent -> left;

            if(y -> color == RBTREE_RED){
                z -> parent -> color = RBTREE_BLACK;
                y -> color = RBTREE_BLACK;
                z -> parent -> parent -> color = RBTREE_RED;

                z = z -> parent -> parent;
            } else {
                if(z == z -> parent -> left){
                    z = z -> parent;
                    rotate_right(t, z);
                }

                z -> parent -> color = RBTREE_BLACK;
                z -> parent -> parent -> color = RBTREE_RED;
                rotate_left(t, z -> parent -> parent);
            }
        }
    }

    t -> root -> color = RBTREE_BLACK;                          // case1의 상태에서 탈출 시 root가 RED로 끝날 수 있음
}

node_t *get_new_node(rbtree *tree, key_t key){
    node_t *new_node = (node_t *) malloc(sizeof(node_t));

    new_node -> left = new_node -> right = new_node -> parent = tree -> nil;
    new_node -> color = RBTREE_RED;
    new_node -> key = key;

    return new_node;
}

node_t *get_root_node(const rbtree *tree){
    return tree -> root;
}

node_t *get_nil_node(const rbtree *tree){
    return tree -> nil;
}

void show_single_node(node_t *node){
    printf("(key = %d", node -> key);
    printf("    color = %s)\n", node -> color == 0 ? "RED" : "BLACK");
}

void show_tree(rbtree *tree, node_t *node){
    node_t *nil = get_nil_node(tree);

    if(node == nil){
        return;
    }

    if(node -> left != nil){
        show_tree(tree, node -> left);
    }

    printf("%d, %s\n", node -> key, node -> color == 0 ? "RED" : "BLACK");

    if(node -> right != nil){
        show_tree(tree, node -> right);
    }
}
