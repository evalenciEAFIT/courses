/* ============================================================
 *  bwt.h  —  Algoritmo Burrows-Wheeler Transform (BWT)
 *
 *  CONCEPTO:
 *  BWT es una transformada de bloques que reordena los bytes de la
 *  cadena para agrupar símbolos similares. El resultado no está
 *  comprimido por sí solo, pero facilita etapas posteriores como
 *  Run-Length Encoding o Huffman.
 *
 *  ESTRUCTURA DE SALIDA:
 *    - Última columna de la matriz de rotaciones ordenadas
 *    - Índice de la rotación original
 *
 *  COMPLEJIDAD:
 *    Tiempo : O(n log n) para ordenar las rotaciones
 *    Espacio: O(n)
 *
 *  CUÁNDO USARLO:
 *    ✔ Antes de aplicar codificadores de entropía o RLE
 *    ✘ Directamente como compresión final
 * ============================================================ */

#ifndef BWT_H
#define BWT_H

#include <stdint.h>
#include <stdlib.h>

typedef enum { BWT_SILENT = 0, BWT_VERBOSE = 1 } BWTMode;

/* Resultado de la transformada BWT */
typedef struct {
    uint8_t *data;       /* Última columna transformada */
    long     size;       /* Tamaño de la columna */
    long     original_index; /* Índice de la rotación original */
} BWTResult;

/* Resultado de la reconstrucción inversa */
typedef struct {
    uint8_t *data;       /* Datos restaurados */
    long     size;       /* Tamaño original */
} BWTDecompResult;

BWTResult      bwt_transform(const uint8_t *input, long in_size, BWTMode mode);
BWTDecompResult bwt_inverse(const uint8_t *transformed, long size,
                            long original_index, BWTMode mode);

#endif /* BWT_H */
