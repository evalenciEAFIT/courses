/* ============================================================
 *  deflate.h  —  Deflate pedagógico (LZ77 + Huffman)
 *
 *  CONCEPTO:
 *  Deflate combina una etapa LZ77 con codificación de Huffman. Esta
 *  implementación usa el LZ77 del proyecto y luego comprime los tokens
 *  resultantes con Huffman.
 * ============================================================ */

#ifndef DEFLATE_H
#define DEFLATE_H

#include <stdint.h>
#include <stdlib.h>
#include "huffman.h"

typedef enum { DEF_SILENT = 0, DEF_VERBOSE = 1 } DeflateMode;

typedef struct {
    uint8_t  *data;
    long      size;
    long      bit_count;
    HuffNode *tree_root;
} DeflateResult;

typedef struct {
    uint8_t *data;
    long     size;
} DeflateDecompResult;

DeflateResult    deflate_compress(const uint8_t *input,
                                  long in_size,
                                  DeflateMode mode);
DeflateDecompResult deflate_decompress(const uint8_t *input,
                                       long comp_size,
                                       long bit_count,
                                       HuffNode *tree_root,
                                       long original_size,
                                       DeflateMode mode);

#endif /* DEFLATE_H */
