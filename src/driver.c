#include "rbtree.c"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    rbtree *tree = new_rbtree();
    int num[] = {20, 10, 50, 30, 80, 40, 35, 25};

    for (int i = 0; i < 8; i++) {
        rbtree_insert(tree, num[i]);
    }

    show_tree(tree, tree -> root);
    rbtree_find(tree, 6);
    rbtree_find(tree, 11);
    rbtree_find(tree, 1);

    rbtree_min(tree);
    rbtree_max(tree);
}