/* ============================================================
 *  lzw.c  —  Implementación del algoritmo LZW
 * ============================================================ */

#include "lzw.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    uint8_t *data;
    long     len;
} LZWDictEntry;

static long lzw_find_longest(const LZWDictEntry *dict, long dict_count,
                             const uint8_t *input, long pos, long in_size) {
    long best_index = 0;
    long best_len = 0;

    for (long i = 0; i < dict_count; i++) {
        const LZWDictEntry *entry = &dict[i];
        if (entry->len > in_size - pos) continue;
        if (entry->len <= best_len) continue;
        if (memcmp(entry->data, input + pos, entry->len) == 0) {
            best_len = entry->len;
            best_index = i;
        }
    }
    return best_index;
}

void lzw_print_tokens(const LZWToken *tokens, long count, long max_show) {
    printf(BOLD BCYAN "\n  ┌─ Tokens LZW generados ──────────────────────────────\n" RESET);
    printf(BOLD "  │ Formato: [Código(16b)]\n" RESET);
    PRINT_THIN_SEP();
    printf(DIM "  %-5s %-6s  Interpretación\n" RESET, "Idx", "Código");
    PRINT_DASH_SEP();

    long show = (count < max_show) ? count : max_show;
    for (long i = 0; i < show; i++) {
        printf("  %-5ld %-6u\n", i, tokens[i].code);
    }
    if (count > max_show)
        printf(DIM "  ... (%ld tokens más)\n" RESET, count - max_show);
    printf(BOLD BCYAN "  └────────────────────────────────────────────────────\n" RESET);
}

LZWResult lzw_compress(const uint8_t *input, long in_size, LZWMode mode) {
    LZWResult result = { NULL, 0, 0 };
    if (!input || in_size <= 0) return result;

    long max_dict = 65536;
    LZWDictEntry *dict = (LZWDictEntry *)calloc(max_dict, sizeof(LZWDictEntry));
    if (!dict) return result;

    for (int i = 0; i < 256; i++) {
        dict[i].data = (uint8_t *)malloc(1);
        dict[i].data[0] = (uint8_t)i;
        dict[i].len = 1;
    }
    long dict_count = 256;

    LZWToken *tokens = (LZWToken *)malloc(sizeof(LZWToken) * (in_size + 1));
    if (!tokens) {
        for (int i = 0; i < 256; i++) free(dict[i].data);
        free(dict);
        return result;
    }

    long pos = 0;
    long token_count = 0;

    if (mode == LZW_VERBOSE) {
        printf(BOLD BBLUE "\n  ╔══ LZW COMPRESIÓN — Paso a paso ══════════════════╗\n" RESET);
        printf(DIM "  Construye frases codificadas por códigos fijos.\n" RESET);
    }

    while (pos < in_size) {
        long match = lzw_find_longest(dict, dict_count, input, pos, in_size);
        long match_len = dict[match].len;
        uint16_t code = (uint16_t)match;

        pos += match_len;
        if (pos < in_size && dict_count < max_dict) {
            long new_len = match_len + 1;
            uint8_t *new_data = (uint8_t *)malloc(new_len);
            if (!new_data) break;
            memcpy(new_data, dict[match].data, match_len);
            new_data[match_len] = input[pos];
            dict[dict_count].data = new_data;
            dict[dict_count].len = new_len;
            dict_count++;
        }

        tokens[token_count].code = code;
        if (mode == LZW_VERBOSE) {
            printf("  Token[%3ld]: código=%u len=%ld\n",
                   token_count, code, match_len);
            if (token_count % 8 == 7)
                pause_step("Seguir comprimiendo...");
        }

        token_count++;
    }

    result.tokens = tokens;
    result.count = token_count;
    result.raw_size = (long)(token_count * sizeof(LZWToken));

    if (mode == LZW_VERBOSE) {
        printf(BOLD BBLUE "  ╚══ FIN COMPRESIÓN LZW ═══════════════════════════╝\n" RESET);
        printf("  Tokens: " BYELLOW "%ld" RESET " → bytes=%ld\n",
               token_count, result.raw_size);
        lzw_print_tokens(tokens, token_count, 32);
    }

    for (long i = 0; i < dict_count; i++) free(dict[i].data);
    free(dict);
    return result;
}

LZWDecompResult lzw_decompress(const LZWToken *tokens, long count,
                               long original_size, LZWMode mode) {
    LZWDecompResult result = { NULL, 0 };
    if (!tokens || count <= 0) return result;

    long max_dict = 65536;
    LZWDictEntry *dict = (LZWDictEntry *)calloc(max_dict, sizeof(LZWDictEntry));
    if (!dict) return result;

    for (int i = 0; i < 256; i++) {
        dict[i].data = (uint8_t *)malloc(1);
        dict[i].data[0] = (uint8_t)i;
        dict[i].len = 1;
    }
    long dict_count = 256;

    uint8_t *out = (uint8_t *)malloc(original_size + 1);
    if (!out) {
        for (int i = 0; i < 256; i++) free(dict[i].data);
        free(dict);
        return result;
    }

    long out_pos = 0;
    long prev_code = -1;

    for (long i = 0; i < count; i++) {
        uint16_t code = tokens[i].code;
        if (code >= dict_count) {
            /* Caso especial: entrada como 'kwk' */
            long prev_len = dict[prev_code].len;
            dict[dict_count].len = prev_len + 1;
            dict[dict_count].data = (uint8_t *)malloc(prev_len + 1);
            memcpy(dict[dict_count].data, dict[prev_code].data, prev_len);
            dict[dict_count].data[prev_len] = dict[prev_code].data[0];
            dict_count++;
        }

        const LZWDictEntry *entry = &dict[code];
        memcpy(out + out_pos, entry->data, entry->len);
        out_pos += entry->len;

        if (prev_code >= 0 && dict_count < max_dict) {
            long prev_len = dict[prev_code].len;
            uint8_t first_char = entry->data[0];
            long new_len = prev_len + 1;
            uint8_t *new_data = (uint8_t *)malloc(new_len);
            memcpy(new_data, dict[prev_code].data, prev_len);
            new_data[prev_len] = first_char;
            dict[dict_count].data = new_data;
            dict[dict_count].len = new_len;
            dict_count++;
        }

        if (mode == LZW_VERBOSE && i < 20) {
            printf("  Token[%3ld]: decodifica código=%u len=%ld\n",
                   i, code, entry->len);
        }

        prev_code = code;
    }

    out[out_pos] = '\0';
    result.data = out;
    result.size = out_pos;

    if (mode == LZW_VERBOSE) {
        printf(BOLD BBLUE "  ╚══ FIN DESCOMPRESIÓN LZW ════════════════════════╝\n" RESET);
        printf("  Bytes restaurados: " BCYAN "%ld" RESET "\n", out_pos);
    }

    for (long i = 0; i < dict_count; i++) free(dict[i].data);
    free(dict);
    return result;
}
