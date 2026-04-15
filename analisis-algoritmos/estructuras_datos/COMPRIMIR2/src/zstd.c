/* ============================================================
 *  zstd.c  —  Implementación pedagógica de Zstandard
 * ============================================================ */

#include "zstd.h"
#include "bwt.h"
#include "huffman.h"
#include "utils.h"
#include <stdlib.h>

ZstdResult zstd_compress(const uint8_t *input, long in_size, ZSTDMode mode) {
    ZstdResult result = { NULL, 0, 0, NULL, 0 };
    if (!input || in_size <= 0) return result;

    BWTMode bwt_mode = mode == ZSTD_VERBOSE ? BWT_VERBOSE : BWT_SILENT;
    HuffMode huff_mode = mode == ZSTD_VERBOSE ? HUFF_VERBOSE : HUFF_SILENT;

    BWTResult bwt = bwt_transform(input, in_size, bwt_mode);
    if (!bwt.data) return result;

    HuffResult huff = huff_compress(bwt.data, bwt.size, huff_mode);
    free(bwt.data);
    if (!huff.data) return result;

    result.data = huff.data;
    result.size = huff.size;
    result.bit_count = huff.bit_count;
    result.tree_root = huff.tree_root;
    result.original_index = bwt.original_index;
    return result;
}

ZstdDecompResult zstd_decompress(const uint8_t *input,
                                 long comp_size,
                                 long bit_count,
                                 HuffNode *tree_root,
                                 long original_index,
                                 long original_size,
                                 ZSTDMode mode) {
    ZstdDecompResult result = { NULL, 0 };
    if (!input || comp_size <= 0 || !tree_root || original_size < 0) return result;

    HuffMode huff_mode = mode == ZSTD_VERBOSE ? HUFF_VERBOSE : HUFF_SILENT;
    BWTMode bwt_mode = mode == ZSTD_VERBOSE ? BWT_VERBOSE : BWT_SILENT;

    HuffDecompResult huff = huff_decompress(input, comp_size,
                                            bit_count, tree_root,
                                            huff_mode);
    if (!huff.data) return result;

    BWTDecompResult decomp = bwt_inverse(huff.data, huff.size,
                                        original_index, bwt_mode);
    free(huff.data);
    if (!decomp.data) return result;

    result.data = decomp.data;
    result.size = decomp.size;
    return result;
}
