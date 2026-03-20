#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "data.h"

#define HASH_SIZE 100003 // Número primo

typedef struct HashNode {
    Persona data;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode **table;
} HashTable;

HashTable* ht_create();
void ht_insert(HashTable *ht, Persona p);
Persona* ht_search(HashTable *ht, const char *nombre);
void ht_traverse(HashTable *ht);
void ht_destroy(HashTable *ht);

#endif
