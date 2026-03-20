#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "data.h"
#include "hash_table.h"
#include "nary_tree.h"
#include "graph.h"
#include "b_tree.h"

// Macro o Constante en precompilación estática para la asignación dinámica de 10 Millones de Elementos
#define N_DATOS 10000000

// Reloj especial auxiliar basado en precisión CLOCK_MONOTONIC para contar los microsegundos reales
// saltando paradas del OS para asegurar el cálculo del Execution Time de cada bloque
double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

int main() {
    // Generador (Semilla aleatoria 12345). Garantizar una misma semilla hace que toda inserción comparativa y "randomness" 
    // sea universal sin afectar el performance y tiempo real en los 4 algoritmos bajo exáctamente la mismas variables y combinatoria caótica
    srand(12345);
    Persona *datos = malloc(sizeof(Persona) * N_DATOS);
    
    printf("Generando %d datos...\n", N_DATOS);
    generar_datos(datos, N_DATOS); // Llenado inicial Random -> data.c
    
    // Sacar de la memoria principal el nombre exacto de la persona de la posición inter-medio de todo el arreglo 
    // en este caso sería datos[N_DATOS/2].nombre lo elegimos para buscar su coincidencia, lo que simularía de 
    // la forma más imparcial/neutra el mejor/peor de los casos si todos estuvieran ordenados o muy distantes
    char *dato_buscar = datos[N_DATOS / 2].nombre;
    printf("Dato a buscar: %s\n\n", dato_buscar);
    
    double t0, t1; // Variables Timer
    
    // =========================================================
    // 1. COMPARACIÓN CON LA TABLA HASH (COMPLEJIDAD HASH O(1)) 
    // =========================================================
    printf("--- 1. TABLA HASH ---\n");
    HashTable *ht = ht_create();
    
    t0 = get_time();
    for (int i = 0; i < N_DATOS; i++) ht_insert(ht, datos[i]);
    t1 = get_time();
    printf("  Inserccion: %f s\n", t1 - t0);
    
    t0 = get_time();
    Persona *res_ht = ht_search(ht, dato_buscar);
    t1 = get_time();
    printf("  Busqueda:   %f s (Encontrado: %s)\n", t1 - t0, res_ht ? res_ht->nombre : "No");
    
    t0 = get_time();
    ht_traverse(ht);
    t1 = get_time();
    printf("  Recorrido:  %f s\n\n", t1 - t0);
    
    // =========================================================
    // 2. COMPARACIÓN CON ARBOL N-ARIO (N=10)
    // =========================================================
    printf("--- 2. ARBOL N-ARIO (N=%d) ---\n", MAX_CHILDREN);
    NTree *nt = nt_create(N_DATOS);
    
    t0 = get_time();
    for (int i = 0; i < N_DATOS; i++) nt_insert(nt, datos[i]);
    t1 = get_time();
    printf("  Inserccion: %f s\n", t1 - t0);
    
    t0 = get_time();
    Persona *res_nt = nt_search(nt, dato_buscar);
    t1 = get_time();
    printf("  Busqueda:   %f s (Encontrado: %s)\n", t1 - t0, res_nt ? res_nt->nombre : "No");
    
    t0 = get_time();
    nt_traverse(nt);
    t1 = get_time();
    printf("  Recorrido:  %f s\n\n", t1 - t0);
    
    // =========================================================
    // 3. COMPARACIÓN CON GRAFO TIPO (ADJACENCIA LIST / NODO VECINAL / LIFO / BREADTH-FIRST)
    // =========================================================
    printf("--- 3. GRAFO (Adyacencia) ---\n");
    Graph *g = graph_create(N_DATOS);
    
    t0 = get_time();
    for (int i = 0; i < N_DATOS; i++) graph_insert(g, datos[i]);
    t1 = get_time();
    printf("  Inserccion: %f s\n", t1 - t0);
    
    t0 = get_time();
    Persona *res_g = graph_search(g, dato_buscar);
    t1 = get_time();
    printf("  Busqueda:   %f s (Encontrado: %s)\n", t1 - t0, res_g ? res_g->nombre : "No");
    
    t0 = get_time();
    graph_traverse(g);
    t1 = get_time();
    printf("  Recorrido:  %f s\n\n", t1 - t0);
    
    // =========================================================
    // 4. COMPARACIÓN DEL ÁRBOL B EQUILIBRADO MULTI-CLAVES (SIMULAR ÁRBOL B+)
    // =========================================================
    printf("--- 4. ARBOL B (Orden 4) ---\n");
    BTree *bt = bt_create();
    
    t0 = get_time();
    for (int i = 0; i < N_DATOS; i++) bt_insert(bt, datos[i]);
    t1 = get_time();
    printf("  Inserccion: %f s\n", t1 - t0);
    
    t0 = get_time();
    Persona *res_bt = bt_search(bt, dato_buscar);
    t1 = get_time();
    printf("  Busqueda:   %f s (Encontrado: %s)\n", t1 - t0, res_bt ? res_bt->nombre : "No");
    
    t0 = get_time();
    bt_traverse(bt);
    t1 = get_time();
    printf("  Recorrido:  %f s\n\n", t1 - t0);
    
    // =========================================================
    // FINAL - LIMPIEZA TOTAL EN GARBAGE CLEANER PARA LIBERAR RAM DEL RUNNABLE C
    // =========================================================
    ht_destroy(ht);
    nt_destroy(nt);
    graph_destroy(g);
    bt_destroy(bt);
    free(datos);
    
    printf("Pruebas finalizadas.\n");
    return 0; // Terminado con código de éxito puro
}
