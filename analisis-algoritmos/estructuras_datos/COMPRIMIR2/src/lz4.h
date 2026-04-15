/* ============================================================
 *  lz4.h  —  Algoritmo LZ4 simplificado
 *
 *  CONCEPTO:
 *  LZ4 es un compresor rápido y sin codificación de entropía. Busca
 *  coincidencias en un historial y emite referencias cortas junto con
 *  literales. Su ventaja es la velocidad y la simplicidad.
 *
 *  EN ESTA VERSIÓN PEDAGÓGICA:
 *    - Se usa una ventana de búsqueda deslizante.
 *    - Se emite un token rápido cuando encuentra un match >= 4.
 *    - No usa Huffman ni codificación adicional.
 * ============================================================ */

#ifndef LZ4_H
#define LZ4_H

#include <stdint.h>
#include <stdlib.h>

typedef enum { LZ4_SILENT = 0, LZ4_VERBOSE = 1 } LZ4Mode;

typedef struct {
    uint8_t *data;
    long     size;
} LZ4Result;

typedef struct {
    uint8_t *data;
    long     size;
} LZ4DecompResult;

LZ4Result lz4_compress(const uint8_t *input, long in_size, LZ4Mode mode);
LZ4DecompResult lz4_decompress(const uint8_t *input, long comp_size,
                               long original_size, LZ4Mode mode);

#endif /* LZ4_H */
