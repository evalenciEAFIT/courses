#ifndef NARY_TREE_H
#define NARY_TREE_H
#include "data.h"

typedef struct NTreeNode {
    Persona data;
    int num_children;
    struct NTreeNode *children[MAX_CHILDREN];
} NTreeNode;

typedef struct {
    NTreeNode *root;
    NTreeNode **queue;
    int head;
    int tail;
} NTree;

NTree* nt_create(int capacity);
void nt_insert(NTree *tree, Persona p);
Persona* nt_search(NTree *tree, const char *nombre);
void nt_traverse(NTree *tree);
void nt_destroy(NTree *tree);

#endif
