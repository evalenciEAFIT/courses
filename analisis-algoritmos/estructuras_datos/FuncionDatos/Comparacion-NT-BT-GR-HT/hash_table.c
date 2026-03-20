#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hash_table.h"

// Función de Hashing 'djb2' por Dan Bernstein
// Una de las funciones Hash más rápidas y efectivas para cadenas de texto generadas aleatoriamente
static unsigned int hash_func(const char *str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // equivale a hash * 33 + c
    return hash % HASH_SIZE; // Asegura que encaje al tamaño del Array en base a módulo
}

// Inicialización e hidratación en ceros a los cubículos principales de la tabla (Buckets)
HashTable* ht_create() {
    HashTable *ht = malloc(sizeof(HashTable));
    ht->table = calloc(HASH_SIZE, sizeof(HashNode*));
    return ht;
}

// Inserción, Resolviendo colisiones usando "Encadenamiento Abierto" (Linear Chaining)
// Si algo cae en el mismo Hash Index (Bucket), se crea como un hilo encadenado desde ahí de nodos apuntándose.
void ht_insert(HashTable *ht, Persona p) {
    unsigned int idx = hash_func(p.nombre);
    HashNode *node = malloc(sizeof(HashNode));
    node->data = p;
    node->next = ht->table[idx];
    ht->table[idx] = node;
}

// Búsqueda O(1) tiempo promedio. Busca el bucket usando el hash y explora la lista por si hubo colisiones.
Persona* ht_search(HashTable *ht, const char *nombre) {
    unsigned int idx = hash_func(nombre);
    HashNode *curr = ht->table[idx];
    while (curr) {
        if (strcmp(curr->data.nombre, nombre) == 0) return &curr->data; // match
        curr = curr->next;
    }
    return NULL;
}

// Recorrer los datos de la tabla (O(N)), visitando en desorden todos los cubos y cadenas adentro
void ht_traverse(HashTable *ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode *curr = ht->table[i];
        while (curr) {
            volatile int temp = curr->data.edad; // Lectura ficticia
            (void)temp;
            curr = curr->next;
        }
    }
}

// Limpiar y descargar la memoria
void ht_destroy(HashTable *ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode *curr = ht->table[i];
        while (curr) {
            HashNode *tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }
    free(ht->table);
    free(ht);
}
