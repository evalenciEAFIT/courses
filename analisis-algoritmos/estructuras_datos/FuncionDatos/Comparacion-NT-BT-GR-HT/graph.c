#include <stdlib.h>
#include <string.h>
#include "graph.h"

// Creamos la estructura principal de nuestro Grafo (Adyacencia base)
Graph* graph_create(int capacity) {
    Graph *g = malloc(sizeof(Graph)); // Capacidad teórica asignada
    g->nodes = malloc(sizeof(GNode*) * capacity);
    g->count = 0;
    g->capacity = capacity;
    return g; // Estará compuesto por un catálogo gigante de Nodos y sus conexiones internas
}

// Inserción, la inserción genera un nuevo nodo con propiedades sin visitar inicialmente.
void graph_insert(Graph *g, Persona p) {
    GNode *node = malloc(sizeof(GNode));
    node->data = p;
    node->visited = 0;
    node->num_edges = 0;
    node->capacity = 2; // Inicia con capacidad de relacionarse con 2 nodos por defecto
    node->edges = malloc(sizeof(GNode*) * node->capacity);
    
    g->nodes[g->count] = node;
    
    // Para no dejar al nodo "aislado" y sin ninguna arista
    // Forzamos a que tenga una relación directa bidireccional con su Nodo temporalmente más cercano (el insertado anterior)
    if (g->count > 0) {
        GNode *prev = g->nodes[g->count - 1];
        if (prev->num_edges == prev->capacity) {
            prev->capacity *= 2; // Multiplica la capacidad del array de relaciones
            prev->edges = realloc(prev->edges, sizeof(GNode*) * prev->capacity);
        }
        prev->edges[prev->num_edges++] = node; // "Mi antecesor me conoce"
        node->edges[node->num_edges++] = prev; // "Yo conozco a mi antecesor"
    }
    g->count++;
}

// Búsqueda tipo Depth-First Search (DFS) basica simulando un paseo entre los arcos/aristas referenciando entre sí
Persona* graph_search(Graph *g, const char *nombre) {
    // 1. Resetear todos a estatus "No visitado" porque de lo contrario entraría en un loop infinito o bucle cíclico en Grafos
    for (int i = 0; i < g->count; i++) g->nodes[i]->visited = 0;
    if (g->count == 0) return NULL;
    
    // 2. Usar un LIFO (Pila/Stack) de los nodos para explorar su descendencia
    GNode **stack = malloc(sizeof(GNode*) * g->count);
    int top = 0;
    stack[top++] = g->nodes[0];
    g->nodes[0]->visited = 1;
    
    Persona *found = NULL;
    
    while (top > 0) {
        GNode *u = stack[--top];
        if (strcmp(u->data.nombre, nombre) == 0) {
            found = &u->data; // Lo encontró exitosamente navegando red de conexiones
            break;
        }
        // 3. Agregar los vecinos que son parte de la conexión de la persona a que los revise luego en el pop()
        for (int i = 0; i < u->num_edges; i++) {
            GNode *v = u->edges[i];
            if (!v->visited) {
                v->visited = 1;
                stack[top++] = v;
            }
        }
    }
    free(stack);
    return found;
}

// Emula un paseo/caminata sobre todas las relaciones a los nodos. 
void graph_traverse(Graph *g) {
    for (int i = 0; i < g->count; i++) g->nodes[i]->visited = 0;
    if (g->count == 0) return;
    
    // Breadth-First Search (BFS) Traversal con Queue (Cola) a manera de recorrido ordenado
    GNode **q = malloc(sizeof(GNode*) * g->count);
    int head = 0, tail = 0;
    
    q[tail++] = g->nodes[0];
    g->nodes[0]->visited = 1;
    
    while (head < tail) {
        GNode *u = q[head++];
        volatile int temp = u->data.edad; // Simulacro
        (void)temp;
        for (int i = 0; i < u->num_edges; i++) {
            GNode *v = u->edges[i];
            // Si el amigo del Grafo aún no ha sido visto lo metemos y exploramos sus otros amigos más adelante
            if (!v->visited) {
                v->visited = 1;
                q[tail++] = v;
            }
        }
    }
    free(q);
}

// Borrado general para Memory Leaks
void graph_destroy(Graph *g) {
    for (int i = 0; i < g->count; i++) {
        free(g->nodes[i]->edges); 
        free(g->nodes[i]);
    }
    free(g->nodes);
    free(g);
}
