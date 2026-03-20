#include <stdlib.h>
#include <string.h>
#include "nary_tree.h"

// Construye un nuevo árbol N-ario (cada nodo tendra un subarreglo de hijos N maximos)
NTree* nt_create(int capacity) {
    NTree *tree = malloc(sizeof(NTree));
    tree->root = NULL;
    // Creamos explícitamente una cola (Queue) en contexto de memoria para poder balancear la estructura rápido
    tree->queue = malloc(sizeof(NTreeNode*) * capacity);
    tree->head = 0;
    tree->tail = 0;
    return tree;
}

// Inserción secuencial (N-dimensional), utiliza la estructura de cola para no tener que 
// recalcular quién es el padre vacio a cada rato. Se agrega un hijo en el "padre en frente de la cola"
void nt_insert(NTree *tree, Persona p) {
    NTreeNode *node = malloc(sizeof(NTreeNode));
    node->data = p;
    node->num_children = 0;
    
    if (!tree->root) {
        tree->root = node;
        tree->queue[tree->tail++] = node; // Raíz entra en la cola también
        return;
    }
    
    // Obtenemos al padre y le sub-anidamos este nodo nuevo como hijo
    NTreeNode *parent = tree->queue[tree->head];
    parent->children[parent->num_children++] = node;
    tree->queue[tree->tail++] = node; // el hijo podría ser un padre más adelante, meter en la cola
    
    // Y verificamos si este padre ya llegó a MAX_CHILDREN
    // Si llegó, se expulsa el índice cabeza, y el próximo hijo adoptará como padre a otro nodo del Queue
    if (parent->num_children == MAX_CHILDREN) {
        tree->head++; 
    }
}

// Búsqueda en Grafo Árbol usando modelo de DFS de Profundidad Simple pero al revés hasta un match
static Persona* nt_search_rec(NTreeNode *node, const char *nombre) {
    if (!node) return NULL;
    // ¿Es el nodo base lo que busco?
    if (strcmp(node->data.nombre, nombre) == 0) return &node->data;
    
    // Entrar en recursión con todos sus cientos de hijos (hasta N niveles de profundidad)
    for (int i = 0; i < node->num_children; i++) {
        Persona *res = nt_search_rec(node->children[i], nombre);
        if (res) return res; // match encontrado desde la cascada
    }
    return NULL;
}

Persona* nt_search(NTree *tree, const char *nombre) {
    return nt_search_rec(tree->root, nombre);
}

// Recorrido DFS Recursivo a todo nivel del sistema N-Nodes descendiente
static void nt_traverse_rec(NTreeNode *node) {
    if (!node) return;
    volatile int temp = node->data.edad; // Simulacro de lectura computacional a la memoria
    (void)temp;
    for (int i = 0; i < node->num_children; i++) {
        nt_traverse_rec(node->children[i]);
    }
}

void nt_traverse(NTree *tree) {
    nt_traverse_rec(tree->root);
}

static void nt_destroy_rec(NTreeNode *node) {
    if (!node) return;
    for (int i = 0; i < node->num_children; i++) {
        nt_destroy_rec(node->children[i]);
    }
    free(node);
}

void nt_destroy(NTree *tree) {
    nt_destroy_rec(tree->root);
    free(tree->queue);
    free(tree);
}
