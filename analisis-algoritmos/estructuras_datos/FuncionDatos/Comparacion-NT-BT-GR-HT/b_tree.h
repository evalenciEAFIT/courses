#ifndef B_TREE_H
#define B_TREE_H
#include "data.h"

#define B_ORDER 4

typedef struct BNode {
    int num;
    int is_leaf;
    char keys[B_ORDER - 1][50];
    Persona data[B_ORDER - 1]; 
    struct BNode *child[B_ORDER];
} BNode;

typedef struct {
    BNode *root;
} BTree;

BTree* bt_create();
void bt_insert(BTree *tree, Persona p);
Persona* bt_search(BTree *tree, const char *nombre);
void bt_traverse(BTree *tree);
void bt_destroy(BTree *tree);

#endif
