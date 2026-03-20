#include <stdlib.h>
#include <string.h>
#include "b_tree.h"

// Crea un nuevo árbol B equilibrado instanciando la raíz base (BTree)
BTree* bt_create() {
    BTree *t = malloc(sizeof(BTree));
    t->root = calloc(1, sizeof(BNode));
    t->root->is_leaf = 1; // El primer nodo interno inicia siendo una hoja vacía
    return t;
}

// Función auxiliar que divide (split) un subnodo si el mismo sobrepasó su capacidad límite designada.
static void split_child(BNode *parent, int i, BNode *y) {
    BNode *z = calloc(1, sizeof(BNode));
    z->is_leaf = y->is_leaf;
    int t = B_ORDER / 2; // Para B_ORDER = 4, t=2. 
    z->num = t - 1; // Pasará a tener la mitad estricta de elementos
    
    // Copia la mitad derecha de las claves del nodo lleno 'y' y los datos al nuevo nodo 'z'
    for(int j=0; j < z->num; j++) {
        strcpy(z->keys[j], y->keys[j + t]);
        z->data[j] = y->data[j + t];
    }
    
    // Si la pieza o nodo dividido NO es una hoja en la capa inferor, también copia sus respectivos "hijos huérfanos" a 'z'
    if (!y->is_leaf) {
        for(int j=0; j < t; j++) {
            z->child[j] = y->child[j + t];
        }
    }
    y->num = t - 1; // El nodo original se queda también con la exacto mitad
    
    // Abre espacio en el padre subiendo los índices para insertar el nuevo hijo dividido ('z')
    for(int j=parent->num; j >= i+1; j--) {
        parent->child[j+1] = parent->child[j];
    }
    parent->child[i+1] = z; // Enlace interno a z reconstruido
    
    // Sube "físicamente" la clave mayor desde el nodo que estamos dividiendo hacia el array principal del árbol padre
    for(int j=parent->num - 1; j >= i; j--) {
        strcpy(parent->keys[j+1], parent->keys[j]);
        parent->data[j+1] = parent->data[j];
    }
    strcpy(parent->keys[i], y->keys[t - 1]);
    parent->data[i] = y->data[t - 1];
    parent->num++; // Ahora el padre tiene un índice/clave más que antes (por este motivo al subir de nivel en nivel la inserción del BTree es lenta al equilibrarlo a repetición constante)
}

// Asignar claves o records ordenadamente para insertar elementos a un nodo dado asumiendo que el nodo no está lleno completamente
static void insert_non_full(BNode *x, Persona k) {
    int i = x->num - 1;
    if (x->is_leaf) { // Modo de hoja terminal inferior
        // Intercambia hacia la derecha desplazando elementos al comparar alfabéticamente (Ascendiente) para buscar un hueco a nuestro nombre de persona "k"
        while (i >= 0 && strcmp(k.nombre, x->keys[i]) < 0) {
            strcpy(x->keys[i+1], x->keys[i]);
            x->data[i+1] = x->data[i];
            i--;
        }
        // Inserta la nueva persona en el espacio desocupado del B-tree
        strcpy(x->keys[i+1], k.nombre);
        x->data[i+1] = k;
        x->num++;
    } else { // Si el fragmento de BTree que evaluamos no fue hoja, pasarlo a su hijo correspondiente
        // Encontrar cual de los 'n' hijos de acuerdo a la dirección alfabética encaja
        while (i >= 0 && strcmp(k.nombre, x->keys[i]) < 0) i--;
        i++;
        
        // Antes de insertarlo abajo del hijo revisamos si ese hijo está full, hay que prevenir su división prematura
        if (x->child[i]->num == B_ORDER - 1) {
            split_child(x, i, x->child[i]); // Reventamos el nodo con un Split superior
            // Y volvemos a ajustar el offset index 'i' en caso un split elevase la clave mayor rebotada a mis variables
            if (strcmp(k.nombre, x->keys[i]) > 0) i++;
        }
        insert_non_full(x->child[i], k);
    }
}

// Inserción Global o de Cascada que llama todo el motor del árbol B
void bt_insert(BTree *tree, Persona p) {
    BNode *r = tree->root;
    // Si la matriz principal del root esta saturada según el Max (Orden - 1)
    if (r->num == B_ORDER - 1) {
        BNode *s = calloc(1, sizeof(BNode)); // Crear un nivel más arriba (raíz master vacía)
        tree->root = s;
        s->is_leaf = 0;
        s->child[0] = r; // Bajamos de nivel jerárquico al array saturado 
        split_child(s, 0, r); // Con una nueva master, fragmentamos (split) aquel root antiguo partido en dos y subimos una clave
        insert_non_full(s, p); // Operamos e insertamos limpiamente en la rama
    } else {
        insert_non_full(r, p); // Nada que dividir, prosigue natural
    } // Nota: Gracias a este rebalance estructural del árbol se garantiza matemáticamente el acceso "O(log N)".
}

// Motor de Búsqueda recursiva: explora el índice multinivel como un diccionario 
static Persona* search_node(BNode *x, const char *nombre) {
    int i = 0;
    // Navegar en los márgenes de los keys del nodo cruzando cada puerta que tenga alfabetismo mayor al mío hasta cruzar el final del array o ser inferior.
    while (i < x->num && strcmp(nombre, x->keys[i]) > 0) i++;
    
    // Si dimos con exactitud en la clavija, retornar el dato asociado del mismo índice
    if (i < x->num && strcmp(nombre, x->keys[i]) == 0) return &x->data[i];
    
    // Si llegamos a tocar fondo hacia una hoja y el número de índice tampoco es... El Elemento no existe. Devuelve Null.
    if (x->is_leaf) return NULL;
    
    // Usar la ranura libre cruzada "i" para descender un escalón en la capa BTree al target correspondiente (Divide and Conquer Log N) 
    return search_node(x->child[i], nombre);
}

Persona* bt_search(BTree *tree, const char *nombre) {
    return search_node(tree->root, nombre);
}

// Utilidad recursiva Inorder
static void traverse_node(BNode *x) {
    int i;
    // El "Arboles B" clásicos exigen intercalar hijos y llaves. El B+ sería solo navegar por las hojas derechas/izquierdas interconectadas.
    for (i = 0; i < x->num; i++) {
        if (!x->is_leaf) traverse_node(x->child[i]); // Visita al Hijo inferior recursivamente de forma ordenada de izquierda a derecha
        volatile int temp = x->data[i].edad; // Lectura ficticia
        (void)temp;
    }
    // Visita el brazo del último hijo al extricto límite derecho de la rama actual para cubrirla al 100%.
    if (!x->is_leaf) traverse_node(x->child[i]); 
}

void bt_traverse(BTree *tree) {
    if (tree->root) traverse_node(tree->root);
}

// Limpiar el subarbol recursivamente para prevenir memory leaks
static void destroy_node(BNode *x) {
    if (!x) return;
    if (!x->is_leaf) {
        // En los arboles B hay Nx+1 ramas filiales
        for (int i = 0; i <= x->num; i++) {
            destroy_node(x->child[i]);
        }
    }
    free(x); // Apagar y quemar memoria
}

void bt_destroy(BTree *tree) {
    destroy_node(tree->root);
    free(tree);
}
