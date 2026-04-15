/* ============================================================
 *  zstd.h  —  Zstandard pedagógico
 *
 *  CONCEPTO:
 *  Zstd añade una etapa de transformación antes de la codificación de
 *  entropía. En esta versión se usa BWT + Huffman para ilustrar cómo una
 *  transformada previa mejora la compresión.
 * ============================================================ */

#ifndef ZSTD_H
#define ZSTD_H

#include <stdint.h>
#include <stdlib.h>
#include "huffman.h"

typedef enum { ZSTD_SILENT = 0, ZSTD_VERBOSE = 1 } ZSTDMode;

typedef struct {
    uint8_t  *data;
    long      size;
    long      bit_count;
    HuffNode *tree_root;
    long      original_index;
} ZstdResult;

typedef struct {
    uint8_t *data;
    long     size;
} ZstdDecompResult;

ZstdResult zstd_compress(const uint8_t *input, long in_size, ZSTDMode mode);
ZstdDecompResult zstd_decompress(const uint8_t *input,
                                 long comp_size,
                                 long bit_count,
                                 HuffNode *tree_root,
                                 long original_index,
                                 long original_size,
                                 ZSTDMode mode);

#endif /* ZSTD_H */
