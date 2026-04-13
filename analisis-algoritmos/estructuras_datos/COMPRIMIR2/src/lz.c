/* ============================================================
 *  lz.c  —  Implementación del algoritmo LZ77
 * ============================================================ */

#include "lz.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

/* ═══════════════════════════════════════════════════════════════════════════
 *  VISUALIZACIÓN DE LA VENTANA DESLIZANTE
 * ═══════════════════════════════════════════════════════════════════════════ */
void lz_print_window(const uint8_t *buf, long win_start, long win_size,
                     long pos, long match_dist, long match_len) {
    /* Muestra una ventana compacta: últimos min(win_size, 32) bytes */
    long show_start = (win_start > 32) ? win_start - 32 : 0;
    long match_start = pos - match_dist;

    printf("  " DIM "Ventana: [" RESET);
    for (long i = show_start; i < pos; i++) {
        char c = (buf[i] >= 32 && buf[i] < 127) ? (char)buf[i] : '.';
        if (i >= match_start && match_len > 0 && i < match_start + match_len)
            printf(BGREEN "%c" RESET, c);  /* resalta el match */
        else
            printf(DIM "%c" RESET, c);
    }
    printf(DIM "] → [" RESET);
    /* lookahead */
    for (long i = pos; i < pos + 10 && buf[i]; i++) {
        char c = (buf[i] >= 32 && buf[i] < 127) ? (char)buf[i] : '.';
        printf(BYELLOW "%c" RESET, c);
    }
    printf(DIM "]...\n" RESET);
    (void)win_size; /* no usado directamente aquí */
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  VISUALIZACIÓN DE TOKENS
 * ═══════════════════════════════════════════════════════════════════════════ */
void lz_print_tokens(const LZ77Token *tokens, long count, long max_show) {
    printf(BOLD BCYAN "\n  ┌─ Tokens LZ77 generados ─────────────────────────────\n" RESET);
    printf(BOLD "  │ Formato: [DIST(16b)] [LEN(8b)] [NEXT(8b)]\n" RESET);
    printf(BOLD "  │ DIST=0,LEN=0 → literal    DIST>0 → back-reference\n" RESET);
    PRINT_THIN_SEP();
    printf(DIM "  %-5s  %-6s  %-4s  %-8s  Interpretación\n" RESET,
           "Idx", "Dist", "Len", "Next");
    PRINT_DASH_SEP();

    long show = (count < max_show) ? count : max_show;
    for (long i = 0; i < show; i++) {
        const LZ77Token *t = &tokens[i];
        char disp = (t->next_byte >= 32 && t->next_byte < 127)
                    ? (char)t->next_byte : '?';

        printf("  %-5ld ", i);
        if (t->dist == 0 && t->length == 0) {
            /* Literal */
            printf(BYELLOW "%-6s  %-4s " RESET "'" BWHITE "%c" RESET "'  "
                   DIM "(literal)" RESET "\n",
                   "—", "—", disp);
        } else {
            /* Back-reference */
            printf(BCYAN "%-6d  %-4d " RESET "'" BWHITE "%c" RESET "'  "
                   BGREEN "←%d copia %d, luego '%c'" RESET "\n",
                   t->dist, t->length, disp,
                   t->dist, t->length, disp);
        }
    }
    if (count > max_show)
        printf(DIM "  ... (%ld tokens más)\n" RESET, count - max_show);
    printf(BOLD BCYAN "  └──────────────────────────────────────────────────────\n" RESET);
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  BÚSQUEDA DEL MATCH MÁS LARGO EN LA VENTANA
 *  Ingénua: O(WINDOW_SIZE * LOOKAHEAD_MAX) — didáctica y correcta
 * ═══════════════════════════════════════════════════════════════════════════ */
static void find_best_match(const uint8_t *buf, long pos, long size,
                             uint16_t *out_dist, uint8_t *out_len) {
    long best_len  = 0;
    long best_dist = 0;

    /* Inicio de la ventana de búsqueda */
    long win_start = (pos > LZ_WINDOW_SIZE) ? pos - LZ_WINDOW_SIZE : 0;

    for (long i = win_start; i < pos; i++) {
        long match_len = 0;
        /* Cuántos bytes coinciden desde buf[i] y buf[pos] */
        while (match_len < LZ_LOOKAHEAD_MAX
               && pos + match_len < size
               && buf[i + match_len] == buf[pos + match_len]) {
            match_len++;
        }
        if (match_len > best_len) {
            best_len  = match_len;
            best_dist = pos - i;
        }
    }

    *out_dist = (best_len >= LZ_MIN_MATCH) ? (uint16_t)best_dist : 0;
    *out_len  = (best_len >= LZ_MIN_MATCH) ? (uint8_t)best_len   : 0;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  COMPRESIÓN LZ77
 * ═══════════════════════════════════════════════════════════════════════════ */
LZResult lz_compress(const uint8_t *input, long in_size, LZMode mode) {
    LZResult result = { NULL, 0, 0 };
    if (!input || in_size <= 0) return result;

    /* Reservamos como máximo in_size tokens (uno por byte) */
    LZ77Token *tokens = (LZ77Token *)malloc(sizeof(LZ77Token) * (in_size + 1));
    if (!tokens) return result;

    long tok_count  = 0;
    long pos        = 0;
    int  steps_show = 0;

    if (mode == LZ_VERBOSE) {
        printf(BOLD BBLUE "\n  ╔══ LZ77 COMPRESIÓN — Paso a paso ══════════════════╗\n" RESET);
        printf(DIM
               "  Ventana: %d bytes de historial\n"
               "  Match mínimo: %d bytes para emitir referencia\n\n"
               RESET, LZ_WINDOW_SIZE, LZ_MIN_MATCH);
    }

    while (pos < in_size) {
        uint16_t dist = 0;
        uint8_t  mlen = 0;

        find_best_match(input, pos, in_size, &dist, &mlen);

        /* Byte siguiente al match (o literal si no hay match) */
        long next_pos = pos + (mlen >= LZ_MIN_MATCH ? mlen : 0);
        uint8_t next_byte = (next_pos < in_size) ? input[next_pos] : 0;

        if (mode == LZ_VERBOSE && steps_show < 20) {
            steps_show++;
            if (dist > 0 && mlen >= LZ_MIN_MATCH) {
                printf("  Pos %4ld: match ← dist=" BCYAN "%3d" RESET
                       " len=" BGREEN "%3d" RESET " next='" BWHITE "%c" RESET "'\n",
                       pos, dist, mlen,
                       (next_byte >= 32 && next_byte < 127) ? (char)next_byte : '?');
                lz_print_window(input, pos > LZ_WINDOW_SIZE ? pos - LZ_WINDOW_SIZE : 0,
                                LZ_WINDOW_SIZE, pos, dist, mlen);
            } else {
                char lb = (input[pos] >= 32 && input[pos] < 127)
                          ? (char)input[pos] : '?';
                printf("  Pos %4ld: " BYELLOW "literal" RESET " '%c'\n", pos, lb);
            }
            if (steps_show == 20)
                printf(DIM "  ... (sólo se muestran los primeros 20 pasos)\n" RESET);
        }

        LZ77Token tok;
        tok.dist      = dist;
        tok.length    = mlen;
        tok.next_byte = next_byte;
        tokens[tok_count++] = tok;

        /* Avanzar: el match + el byte literal */
        if (dist > 0 && mlen >= LZ_MIN_MATCH)
            pos += mlen + 1;
        else
            pos += 1;
    }

    result.tokens   = tokens;
    result.count    = tok_count;
    result.raw_size = (long)(tok_count * sizeof(LZ77Token));

    if (mode == LZ_VERBOSE) {
        printf(BOLD BBLUE "  ╚══ FIN COMPRESIÓN LZ77 ════════════════════════════╝\n" RESET);
        printf("  Tokens generados: " BCYAN "%ld" RESET
               " × %zu bytes = " BYELLOW "%ld bytes" RESET "\n",
               tok_count, sizeof(LZ77Token), result.raw_size);
        lz_print_tokens(tokens, tok_count, 32);
    }

    return result;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  DESCOMPRESIÓN LZ77
 * ═══════════════════════════════════════════════════════════════════════════ */
LZDecompResult lz_decompress(const LZ77Token *tokens, long count,
                              long original_size, LZMode mode) {
    LZDecompResult result = { NULL, 0 };
    if (!tokens || count <= 0) return result;

    /* Buffer de salida: estimamos el tamaño original + margen */
    long buf_size = (original_size > 0) ? original_size + 1024 :
                    count * (LZ_LOOKAHEAD_MAX + 1) + 1;
    uint8_t *out = (uint8_t *)calloc(buf_size, 1);
    if (!out) return result;

    long out_pos = 0;
    int  shown   = 0;

    if (mode == LZ_VERBOSE) {
        printf(BOLD BBLUE "\n  ╔══ LZ77 DESCOMPRESIÓN — Paso a paso ════════════════╗\n" RESET);
        printf(DIM "  Lee tokens y expande referencias al buffer de salida.\n\n" RESET);
    }

    for (long i = 0; i < count; i++) {
        const LZ77Token *t = &tokens[i];

        if (t->dist > 0 && t->length >= LZ_MIN_MATCH) {
            /* Back-reference: copia 'length' bytes desde 'dist' atrás */
            long src = out_pos - t->dist;
            if (src < 0) src = 0;   /* protección */

            if (mode == LZ_VERBOSE && shown < 20) {
                shown++;
                printf("  Token[%3ld]: " BCYAN "ref" RESET
                       " dist=%d len=%d → copia '", i, t->dist, t->length);
                for (int k = 0; k < t->length && k < 10; k++) {
                    char c = (out[src + k] >= 32) ? (char)out[src + k] : '?';
                    printf(BGREEN "%c" RESET, c);
                }
                printf("'\n");
            }

            for (uint8_t k = 0; k < t->length; k++) {
                if (out_pos >= buf_size - 1) break;
                out[out_pos++] = out[src + k];
            }
        } else {
            if (mode == LZ_VERBOSE && shown < 20) {
                shown++;
                char c = (t->next_byte >= 32) ? (char)t->next_byte : '?';
                printf("  Token[%3ld]: " BYELLOW "lit" RESET " '%c'\n", i, c);
            }
        }

        /* Emite el byte literal que cierra el token */
        if (t->next_byte != 0 || (t->dist == 0 && t->length == 0)) {
            if (out_pos < buf_size - 1)
                out[out_pos++] = t->next_byte;
        }

        if (mode == LZ_VERBOSE && shown == 20 && i == 20)
            printf(DIM "  ... (sólo se muestran los primeros 20 tokens)\n" RESET);
    }

    out[out_pos] = '\0';
    result.data  = out;
    result.size  = out_pos;

    if (mode == LZ_VERBOSE) {
        printf(BOLD BBLUE "  ╚══ FIN DESCOMPRESIÓN LZ77 ════════════════════════════╝\n" RESET);
        printf("  Bytes restaurados: " BCYAN "%ld" RESET "\n", out_pos);
    }

    return result;
}
