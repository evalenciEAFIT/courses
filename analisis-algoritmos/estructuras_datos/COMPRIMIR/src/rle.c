/* ============================================================
 *  rle.c  —  Implementación del algoritmo RLE
 * ============================================================ */

#include "rle.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

/* ───────────────────────────── Constantes ──────────────────────────────── */
#define RLE_MAX_RUN 255   /* Máximo contador por run (cabe en uint8_t) */

/* ═══════════════════════════════════════════════════════════════════════════
 *  VISUALIZACIÓN DE LA ESTRUCTURA INTERNA
 * ═══════════════════════════════════════════════════════════════════════════ */
void rle_print_structure(const uint8_t *compressed, long size) {
    printf(BOLD BCYAN "\n  ┌─ Estructura interna RLE ──────────────────────────────\n" RESET);
    printf(BOLD "  │ Formato de cada par: [CONTADOR(1B)] [BYTE(1B)]\n" RESET);
    PRINT_THIN_SEP();
    printf(DIM "  Idx   Count  Byte   Char   Representación visual\n" RESET);
    PRINT_DASH_SEP();

    for (long i = 0; i < size - 1; i += 2) {
        uint8_t count = compressed[i];
        uint8_t byte  = compressed[i + 1];
        char    disp  = (byte >= 32 && byte < 127) ? (char)byte : '?';

        /* barra de bloques proporcional al contador */
        printf("  %-5ld " BYELLOW "%3d  " BCYAN "0x%02X   " BWHITE "'%c' "
               RESET "   " BGREEN, i / 2, count, byte, disp);
        int bar = (count > 40) ? 40 : count;
        for (int b = 0; b < bar; b++) printf("▪");
        if (count > 40) printf("…");
        printf(RESET " x%d\n", count);
    }
    printf(BOLD BCYAN "  └──────────────────────────────────────────────────────\n" RESET);
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  COMPRESIÓN
 * ═══════════════════════════════════════════════════════════════════════════ */
RLEResult rle_compress(const uint8_t *input, long in_size, RLEMode mode) {
    RLEResult result = { NULL, 0 };
    if (!input || in_size <= 0) return result;

    /* Peor caso: sin ninguna repetición → 2 bytes por byte original */
    uint8_t *out = (uint8_t *)malloc(in_size * 2 + 2);
    if (!out) return result;

    long out_idx = 0;

    if (mode == RLE_VERBOSE) {
        printf(BOLD BBLUE "\n  ╔══ RLE COMPRESIÓN — Paso a paso ══════════════════╗\n" RESET);
        printf(DIM "  Recorre los datos buscando rachas (runs) del mismo byte.\n" RESET);
        printf(DIM "  Cada racha se guarda como [cantidad][byte].\n\n" RESET);
    }

    long i = 0;
    while (i < in_size) {
        uint8_t current = input[i];
        long    count   = 1;

        /* Cuenta cuántos bytes iguales consecutivos hay */
        while (i + count < in_size
               && input[i + count] == current
               && count < RLE_MAX_RUN) {
            count++;
        }

        /* Guarda el par (count, byte) */
        out[out_idx++] = (uint8_t)count;
        out[out_idx++] = current;

        if (mode == RLE_VERBOSE) {
            char disp = (current >= 32 && current < 127) ? (char)current : '?';
            printf("  Pos " BYELLOW "%4ld" RESET " → racha '"
                   BGREEN "%c" RESET "' (0x%02X) × "
                   BCYAN "%3ld" RESET " → par [%3ld , 0x%02X]\n",
                   i, disp, current, count, count, current);

            if (i % 16 == 15) {          /* cada 16 runs ofrece pausa */
                pause_step("¿Seguir viendo?");
            }
        }

        i += count;
    }

    result.data = out;
    result.size = out_idx;

    if (mode == RLE_VERBOSE) {
        printf(BOLD BBLUE "  ╚══ FIN COMPRESIÓN ═══════════════════════════════╝\n" RESET);
        printf("  Bytes de entrada : " BYELLOW "%ld" RESET "\n", in_size);
        printf("  Bytes de salida  : " BCYAN   "%ld" RESET "\n", out_idx);
        rle_print_structure(out, out_idx > 64 ? 64 : out_idx);
    }

    return result;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  DESCOMPRESIÓN
 * ═══════════════════════════════════════════════════════════════════════════ */
RLEResult rle_decompress(const uint8_t *input, long in_size, RLEMode mode) {
    RLEResult result = { NULL, 0 };
    if (!input || in_size <= 0 || in_size % 2 != 0) return result;

    /* El tamaño máximo descomprimido es RLE_MAX_RUN × (in_size/2) */
    long max_out = (long)RLE_MAX_RUN * (in_size / 2);
    uint8_t *out = (uint8_t *)malloc(max_out + 1);
    if (!out) return result;

    long out_idx = 0;

    if (mode == RLE_VERBOSE) {
        printf(BOLD BBLUE "\n  ╔══ RLE DESCOMPRESIÓN — Paso a paso ═══════════════╗\n" RESET);
        printf(DIM "  Lee pares [cantidad][byte] y expande cada uno.\n\n" RESET);
    }

    for (long i = 0; i < in_size - 1; i += 2) {
        uint8_t count = input[i];
        uint8_t byte  = input[i + 1];
        char    disp  = (byte >= 32 && byte < 127) ? (char)byte : '?';

        if (mode == RLE_VERBOSE) {
            printf("  Par [%3d, '%c'(0x%02X)] → expande " BGREEN,
                   count, disp, byte);
            int show = count > 20 ? 20 : count;
            for (int k = 0; k < show; k++) printf("%c", disp);
            if (count > 20) printf("…(%d más)", count - 20);
            printf(RESET "\n");
        }

        /* Expande el par */
        memset(out + out_idx, byte, count);
        out_idx += count;
    }

    out[out_idx] = '\0';
    result.data  = out;
    result.size  = out_idx;

    if (mode == RLE_VERBOSE) {
        printf(BOLD BBLUE "  ╚══ FIN DESCOMPRESIÓN ════════════════════════════╝\n" RESET);
        printf("  Bytes comprimidos : " BYELLOW "%ld" RESET "\n", in_size);
        printf("  Bytes restaurados : " BCYAN   "%ld" RESET "\n", out_idx);
    }

    return result;
}
