/* ============================================================
 *  huffman.h  —  Algoritmo de Huffman
 *
 *  CONCEPTO:
 *  La codificación de Huffman asigna códigos binarios de longitud
 *  variable a cada símbolo. Los símbolos más frecuentes reciben
 *  códigos más cortos (principio de entropía de Shannon).
 *
 *  ALGORITMO (3 fases):
 *   1. FRECUENCIAS  — Cuenta cuántas veces aparece cada byte (256 posibles).
 *   2. ÁRBOL        — Construye un árbol binario mínimo con una cola de
 *                     prioridad (min-heap). Los dos nodos de menor frecuencia
 *                     se fusionan iterativamente hasta formar un solo árbol.
 *   3. CODIFICACIÓN — Recorre el árbol: ir a la izquierda = 0, derecha = 1.
 *                     La hoja de cada símbolo determina su código.
 *
 *  ESTRUCTURAS DE DATOS:
 *    HuffNode  — Nodo del árbol (puede ser hoja o nodo interno)
 *    MinHeap   — Cola de prioridad mínima (array de punteros + heapify)
 *    CodeTable — Tabla de 256 entradas con el código y su longitud en bits
 *
 *  COMPLEJIDAD:
 *    Tiempo : O(n + k log k)  donde n = tamaño datos, k = símbolos únicos
 *    Espacio: O(k)            para el árbol y la tabla de códigos
 *
 *  CUÁNDO USARLO:
 *    ✔ Texto natural (muchos símbolos con distintas frecuencias)
 *    ✔ Como etapa final de ZIP, DEFLATE, JPEG, MP3
 *    ✘ Datos aleatorios uniformes (no mejora la compresión)
 * ============================================================ */

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdint.h>
#include <stdlib.h>

typedef enum { HUFF_SILENT = 0, HUFF_VERBOSE = 1 } HuffMode;

/* ───── Nodo del árbol de Huffman ───────────────────────────────────────── */
typedef struct HuffNode {
    uint8_t          symbol;      /* Símbolo (sólo en hojas)             */
    long             freq;        /* Frecuencia acumulada del subárbol    */
    struct HuffNode *left;        /* Subárbol izquierdo  (código bit '0') */
    struct HuffNode *right;       /* Subárbol derecho    (código bit '1') */
    int              is_leaf;     /* 1 si es hoja, 0 si es nodo interno   */
} HuffNode;

/* ───── Cola de prioridad mínima (Min-Heap) ────────────────────────────── */
#define HEAP_MAX 512
typedef struct {
    HuffNode *nodes[HEAP_MAX];
    int       size;
} MinHeap;

/* ───── Tabla de códigos (256 símbolos posibles) ────────────────────────── */
#define MAX_CODE_LEN 256
typedef struct {
    char code[MAX_CODE_LEN];  /* Cadena de '0' y '1' (null-terminated)  */
    int  length;              /* Longitud en bits                        */
} HuffCode;

/* ───── Resultado de la compresión ─────────────────────────────────────── */
typedef struct {
    uint8_t  *data;           /* Datos comprimidos (bits empaquetados)   */
    long      size;           /* Tamaño en bytes                         */
    long      bit_count;      /* Número exacto de bits válidos           */
    HuffNode *tree_root;      /* Raíz del árbol (necesaria para decomp.) */
    HuffCode  table[256];     /* Tabla de códigos usada                  */
    long      freq[256];      /* Frecuencias originales                  */
} HuffResult;

/* ───── API pública ─────────────────────────────────────────────────────── */

/* Comprime los datos de entrada con Huffman */
HuffResult huff_compress(const uint8_t *input, long in_size, HuffMode mode);

/* Descomprime datos Huffman usando la raíz del árbol original */
typedef struct {
    uint8_t *data;
    long     size;
} HuffDecompResult;

HuffDecompResult huff_decompress(const uint8_t *compressed, long comp_size,
                                 long bit_count, HuffNode *tree_root,
                                 HuffMode mode);

/* Libera el árbol de Huffman recursivamente */
void huff_free_tree(HuffNode *node);

/* Imprime el árbol de forma visual (indentado) */
void huff_print_tree(const HuffNode *node, int depth, char branch);

/* Imprime la tabla de frecuencias */
void huff_print_freq(const long freq[256]);

/* Imprime la tabla de códigos generada */
void huff_print_table(const HuffCode table[256], const long freq[256]);

#endif /* HUFFMAN_H */
