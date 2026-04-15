/* ============================================================
 *  lz78.c  —  Implementación del algoritmo LZ78
 * ============================================================ */

#include "lz78.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    uint8_t *data;
    long     len;
} LZ78DictEntry;

static long lz78_find_longest(const LZ78DictEntry *dict, long dict_count,
                              const uint8_t *input, long pos, long in_size) {
    long best_index = 0;
    long best_len   = 0;

    for (long i = 0; i < dict_count; i++) {
        const LZ78DictEntry *entry = &dict[i];
        if (entry->len > in_size - pos) continue;
        if (entry->len <= best_len) continue;
        if (memcmp(entry->data, input + pos, entry->len) == 0) {
            best_len   = entry->len;
            best_index = i + 1; /* 1-based index en token */
        }
    }
    return (best_index << 32) | (uint32_t)best_len;
}

void lz78_print_tokens(const LZ78Token *tokens, long count, long max_show) {
    printf(BOLD BCYAN "\n  ┌─ Tokens LZ78 generados ─────────────────────────────\n" RESET);
    printf(BOLD "  │ Formato: [Índice(16b)] [Símbolo(8b)]\n" RESET);
    PRINT_THIN_SEP();
    printf(DIM "  %-5s %-8s %-8s  Interpretación\n" RESET,
           "Idx", "Index", "Símbolo");
    PRINT_DASH_SEP();

    long show = (count < max_show) ? count : max_show;
    for (long i = 0; i < show; i++) {
        const LZ78Token *t = &tokens[i];
        char disp = (t->next_byte >= 32 && t->next_byte < 127)
                    ? (char)t->next_byte : '?';
        printf("  %-5ld %-8u %-8s  ", i, t->index,
               t->index == 0 ? "(vacío)" : "");
        printf("" BWHITE "%c" RESET "\n", disp);
    }
    if (count > max_show)
        printf(DIM "  ... (%ld tokens más)\n" RESET, count - max_show);
    printf(BOLD BCYAN "  └────────────────────────────────────────────────────\n" RESET);
}

LZ78Result lz78_compress(const uint8_t *input, long in_size, LZ78Mode mode) {
    LZ78Result result = { NULL, 0, 0 };
    if (!input || in_size <= 0) return result;

    LZ78DictEntry *dict = (LZ78DictEntry *)calloc(in_size, sizeof(LZ78DictEntry));
    if (!dict) return result;

    LZ78Token *tokens = (LZ78Token *)malloc(sizeof(LZ78Token) * (in_size + 1));
    if (!tokens) { free(dict); return result; }

    long dict_count = 0;
    long pos = 0;
    long token_count = 0;

    if (mode == LZ78_VERBOSE) {
        printf(BOLD BBLUE "\n  ╔══ LZ78 COMPRESIÓN — Paso a paso ═════════════════╗\n" RESET);
        printf(DIM "  Construye frases como diccionario de cadenas.\n" RESET);
    }

    while (pos < in_size) {
        long packed = lz78_find_longest(dict, dict_count, input, pos, in_size);
        long index = (packed >> 32) & 0xFFFFFFFF;
        long match_len = packed & 0xFFFFFFFF;

        uint8_t next_byte = input[pos + match_len];
        tokens[token_count].index = (uint16_t)index;
        tokens[token_count].next_byte = next_byte;

        long new_len = match_len + 1;
        uint8_t *new_data = (uint8_t *)malloc(new_len);
        if (!new_data) break;
        if (match_len > 0) memcpy(new_data, input + pos, match_len);
        new_data[match_len] = next_byte;

        dict[dict_count].data = new_data;
        dict[dict_count].len  = new_len;
        dict_count++;

        if (mode == LZ78_VERBOSE) {
            char disp = (next_byte >= 32 && next_byte < 127) ? (char)next_byte : '?';
            printf("  Token[%3ld]: index=%4u len=%4ld next='%c'\n",
                   token_count, tokens[token_count].index, match_len, disp);
            if (token_count % 8 == 7)
                pause_step("Seguir comprimiendo...");
        }

        pos += new_len;
        token_count++;
    }

    result.tokens = tokens;
    result.count = token_count;
    result.raw_size = (long)(token_count * sizeof(LZ78Token));

    if (mode == LZ78_VERBOSE) {
        printf(BOLD BBLUE "  ╚══ FIN COMPRESIÓN LZ78 ══════════════════════════╝\n" RESET);
        printf("  Tokens: " BYELLOW "%ld" RESET " → bytes=%ld\n",
               token_count, result.raw_size);
        lz78_print_tokens(tokens, token_count, 32);
    }

    for (long i = 0; i < dict_count; i++) free(dict[i].data);
    free(dict);
    return result;
}

LZ78DecompResult lz78_decompress(const LZ78Token *tokens, long count,
                                  long original_size, LZ78Mode mode) {
    LZ78DecompResult result = { NULL, 0 };
    if (!tokens || count <= 0) return result;

    uint8_t *out = (uint8_t *)malloc(original_size + 1);
    if (!out) return result;

    LZ78DictEntry *dict = (LZ78DictEntry *)calloc(count, sizeof(LZ78DictEntry));
    if (!dict) { free(out); return result; }

    long out_pos = 0;

    for (long i = 0; i < count; i++) {
        uint16_t idx = tokens[i].index;
        uint8_t next_byte = tokens[i].next_byte;

        long seq_len = 0;
        if (idx > 0) {
            seq_len = dict[idx - 1].len;
            memcpy(out + out_pos, dict[idx - 1].data, seq_len);
            out_pos += seq_len;
        }

        out[out_pos++] = next_byte;

        long new_len = seq_len + 1;
        uint8_t *new_data = (uint8_t *)malloc(new_len);
        if (!new_data) break;
        if (idx > 0) memcpy(new_data, dict[idx - 1].data, seq_len);
        new_data[seq_len] = next_byte;

        dict[i].data = new_data;
        dict[i].len  = new_len;

        if (mode == LZ78_VERBOSE && i < 20) {
            char disp = (next_byte >= 32 && next_byte < 127) ? (char)next_byte : '?';
            printf("  Token[%3ld]: reconstruye frase len=%ld next='%c'\n",
                   i, seq_len, disp);
        }
    }

    out[out_pos] = '\0';
    result.data = out;
    result.size = out_pos;

    if (mode == LZ78_VERBOSE) {
        printf(BOLD BBLUE "  ╚══ FIN DESCOMPRESIÓN LZ78 ═══════════════════════╝\n" RESET);
        printf("  Bytes restaurados: " BCYAN "%ld" RESET "\n", out_pos);
    }

    for (long i = 0; i < count; i++) free(dict[i].data);
    free(dict);
    return result;
}
