#ifndef GRAPH_H
#define GRAPH_H
#include "data.h"

typedef struct GNode {
    Persona data;
    int visited;
    int num_edges;
    int capacity;
    struct GNode **edges;
} GNode;

typedef struct {
    GNode **nodes;
    int count;
    int capacity;
} Graph;

Graph* graph_create(int capacity);
void graph_insert(Graph *g, Persona p);
Persona* graph_search(Graph *g, const char *nombre);
void graph_traverse(Graph *g);
void graph_destroy(Graph *g);

#endif
