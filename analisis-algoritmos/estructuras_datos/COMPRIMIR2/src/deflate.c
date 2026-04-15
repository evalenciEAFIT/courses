/* ============================================================
 *  deflate.c  —  Implementación pedagógica de Deflate
 * ============================================================ */

#include "deflate.h"
#include "lz.h"
#include "huffman.h"
#include "utils.h"
#include <stdlib.h>

DeflateResult deflate_compress(const uint8_t *input, long in_size,
                               DeflateMode mode) {
    DeflateResult result = { NULL, 0, 0, NULL };
    if (!input || in_size <= 0) return result;

    LZMode lz_mode = mode == DEF_VERBOSE ? LZ_VERBOSE : LZ_SILENT;
    HuffMode huff_mode = mode == DEF_VERBOSE ? HUFF_VERBOSE : HUFF_SILENT;

    LZResult lz = lz_compress(input, in_size, lz_mode);
    if (!lz.tokens) return result;

    HuffResult huff = huff_compress((const uint8_t *)lz.tokens,
                                    lz.raw_size,
                                    huff_mode);
    free(lz.tokens);
    if (!huff.data) {
        return result;
    }

    result.data = huff.data;
    result.size = huff.size;
    result.bit_count = huff.bit_count;
    result.tree_root = huff.tree_root;
    return result;
}

DeflateDecompResult deflate_decompress(const uint8_t *input,
                                       long comp_size,
                                       long bit_count,
                                       HuffNode *tree_root,
                                       long original_size,
                                       DeflateMode mode) {
    DeflateDecompResult result = { NULL, 0 };
    if (!input || comp_size <= 0 || !tree_root || original_size < 0) {
        return result;
    }

    HuffMode huff_mode = mode == DEF_VERBOSE ? HUFF_VERBOSE : HUFF_SILENT;
    HuffDecompResult huff = huff_decompress(input, comp_size,
                                            bit_count, tree_root,
                                            huff_mode);
    if (!huff.data) return result;

    if (huff.size % sizeof(LZ77Token) != 0) {
        free(huff.data);
        return result;
    }

    long token_count = huff.size / sizeof(LZ77Token);
    LZDecompResult decomp = lz_decompress((const LZ77Token *)huff.data,
                                         token_count,
                                         original_size,
                                         huff_mode == HUFF_VERBOSE ? LZ_VERBOSE : LZ_SILENT);
    free(huff.data);
    if (!decomp.data) return result;

    result.data = decomp.data;
    result.size = decomp.size;
    return result;
}
