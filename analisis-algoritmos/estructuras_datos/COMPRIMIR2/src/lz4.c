/* ============================================================
 *  lz4.c  —  Implementación pedagógica de LZ4
 * ============================================================ */

#include "lz4.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

#define LZ4_WINDOW_SIZE 32768
#define LZ4_MIN_MATCH 4
#define LZ4_MARKER 0xFF

static void emit_literal(uint8_t value, uint8_t **out, long *out_pos) {
    if (value == LZ4_MARKER) {
        (*out)[(*out_pos)++] = LZ4_MARKER;
        (*out)[(*out_pos)++] = 0;
        (*out)[(*out_pos)++] = LZ4_MARKER;
    } else {
        (*out)[(*out_pos)++] = value;
    }
}

static long find_match(const uint8_t *input, long pos, long in_size,
                       long *match_dist) {
    long best_len = 0;
    long start = pos - LZ4_WINDOW_SIZE;
    if (start < 0) start = 0;

    for (long i = start; i < pos; i++) {
        long len = 0;
        while (pos + len < in_size && input[i + len] == input[pos + len] && len < 255) {
            len++;
        }
        if (len > best_len) {
            best_len = len;
            *match_dist = pos - i;
        }
    }
    return best_len;
}

LZ4Result lz4_compress(const uint8_t *input, long in_size, LZ4Mode mode) {
    LZ4Result result = { NULL, 0 };
    if (!input || in_size <= 0) return result;

    long max_out = in_size * 2 + 16;
    uint8_t *out = malloc(max_out);
    if (!out) return result;

    long in_pos = 0, out_pos = 0;
    while (in_pos < in_size) {
        long match_dist = 0;
        long match_len = find_match(input, in_pos, in_size, &match_dist);
        if (match_len >= LZ4_MIN_MATCH) {
            if (mode == LZ4_VERBOSE) {
                printf("  LZ4: emit match dist=%ld len=%ld\n", match_dist, match_len);
            }
            out[out_pos++] = LZ4_MARKER;
            out[out_pos++] = (uint8_t)match_len;
            out[out_pos++] = (uint8_t)(match_dist >> 8);
            out[out_pos++] = (uint8_t)(match_dist & 0xFF);
            in_pos += match_len;
        } else {
            emit_literal(input[in_pos], &out, &out_pos);
            in_pos++;
        }
    }

    result.data = realloc(out, out_pos);
    if (result.data) {
        result.size = out_pos;
    } else {
        result.data = out;
        result.size = out_pos;
    }
    return result;
}

LZ4DecompResult lz4_decompress(const uint8_t *input, long comp_size,
                               long original_size, LZ4Mode mode) {
    LZ4DecompResult result = { NULL, 0 };
    if (!input || comp_size <= 0 || original_size < 0) return result;

    uint8_t *out = malloc(original_size);
    if (!out) return result;

    long in_pos = 0, out_pos = 0;
    while (in_pos < comp_size && out_pos < original_size) {
        uint8_t token = input[in_pos++];
        if (token == LZ4_MARKER) {
            if (in_pos >= comp_size) break;
            uint8_t len = input[in_pos++];
            if (len == 0) {
                if (in_pos >= comp_size) break;
                out[out_pos++] = input[in_pos++];
                continue;
            }
            if (in_pos + 1 >= comp_size) break;
            uint16_t dist = ((uint16_t)input[in_pos] << 8) | input[in_pos + 1];
            in_pos += 2;
            if (dist == 0 || dist > out_pos) break;
            if (mode == LZ4_VERBOSE) {
                printf("  LZ4: decode match dist=%u len=%u\n", dist, len);
            }
            for (uint8_t i = 0; i < len && out_pos < original_size; i++) {
                out[out_pos] = out[out_pos - dist];
                out_pos++;
            }
        } else {
            out[out_pos++] = token;
        }
    }

    if (out_pos != original_size) {
        free(out);
        return result;
    }

    result.data = out;
    result.size = out_pos;
    return result;
}
