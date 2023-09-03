#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    rbtree *R = (rbtree*) malloc(sizeof(rbtree));
    int num[] = {1, 5, 10, 8, 32, 200, 123, 4123, 5345, 6777, 788, 2, 0, 2};

    for (int i = 13; i >= 0; i--) {
        printf("\n");
        rbtree_insert(R, num[i]);
    }

    show_tree(R);
}