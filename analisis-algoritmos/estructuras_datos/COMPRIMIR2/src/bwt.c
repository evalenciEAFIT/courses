/* ============================================================
 *  bwt.c  —  Implementación del Burrows-Wheeler Transform
 * ============================================================ */

#include "bwt.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

static const uint8_t *bwt_src = NULL;
static long           bwt_len = 0;

static int bwt_compare_rotations(const void *a, const void *b) {
    long i = *(const long *)a;
    long j = *(const long *)b;

    for (long k = 0; k < bwt_len; k++) {
        uint8_t ca = bwt_src[(i + k) % bwt_len];
        uint8_t cb = bwt_src[(j + k) % bwt_len];
        if (ca < cb) return -1;
        if (ca > cb) return 1;
    }
    return 0;
}

static void bwt_print_matrix(const uint8_t *input, long size,
                             const long *order, long show) {
    printf(BOLD BCYAN "\n  ┌─ Matriz de rotaciones BWT ──────────────────────────\n" RESET);
    printf(BOLD "  │ Se ordenan todas las rotaciones del bloque.\n" RESET);
    PRINT_THIN_SEP();
    printf(DIM "  %-6s %-10s %-10s %-10s  Última columna\n" RESET,
           "Idx", "Rotación", "Prefijo", "Sufijo");
    PRINT_DASH_SEP();

    for (long i = 0; i < show && i < size; i++) {
        long pos = order[i];
        printf("  %-6ld %-10ld ", i, pos);
        for (long k = 0; k < 8 && k < size; k++) {
            uint8_t c = input[(pos + k) % size];
            printf("%c", (c >= 32 && c < 127) ? c : '.');
        }
        printf(" ");
        long suffix_start = pos + size - 8;
        if (suffix_start < 0) suffix_start += size * ((-suffix_start / size) + 1);
        for (long k = 0; k < 8 && k < size; k++) {
            uint8_t c = input[(suffix_start + k) % size];
            printf("%c", (c >= 32 && c < 127) ? c : '.');
        }
        printf("   " BWHITE "%c" RESET "\n",
               input[(pos + size - 1) % size]);
    }
    printf(BOLD BCYAN "  └────────────────────────────────────────────────────\n" RESET);
}

BWTResult bwt_transform(const uint8_t *input, long in_size, BWTMode mode) {
    BWTResult result = { NULL, 0, -1 };
    if (!input || in_size <= 0) return result;

    if ((unsigned long)in_size > UINT32_MAX) return result;

    long *order = (long *)malloc(sizeof(long) * in_size);
    if (!order) return result;

    bwt_src = input;
    bwt_len = in_size;

    for (long i = 0; i < in_size; i++) order[i] = i;
    qsort(order, in_size, sizeof(long), bwt_compare_rotations);

    uint8_t *out = (uint8_t *)malloc(in_size);
    if (!out) {
        free(order);
        return result;
    }

    for (long i = 0; i < in_size; i++) {
        long idx = order[i];
        long last_pos = (idx + in_size - 1) % in_size;
        out[i] = input[last_pos];
        if (idx == 0) result.original_index = i;
    }

    result.data = out;
    result.size = in_size;

    if (mode == BWT_VERBOSE) {
        printf(BOLD BBLUE "\n  ╔══ BWT — Transformada de Burrows-Wheeler ══════════╗\n" RESET);
        printf(DIM "  Ordena las rotaciones de la cadena y extrae la última columna.\n" RESET);
        printf(DIM "  Índice original: %ld\n\n" RESET, result.original_index);
        bwt_print_matrix(input, in_size, order, 8);

        printf(BOLD BBLUE "  ╚══ FIN TRANSFORMADA BWT ═══════════════════════════╝\n" RESET);
        printf("  Resultado últimas bytes: ");
        for (long i = 0; i < (in_size < 16 ? in_size : 16); i++)
            printf("%02X ", out[i]);
        printf("\n");
    }

    free(order);
    return result;
}

BWTDecompResult bwt_inverse(const uint8_t *transformed, long size,
                            long original_index, BWTMode mode) {
    BWTDecompResult result = { NULL, 0 };
    if (!transformed || size <= 0 || original_index < 0 || original_index >= size)
        return result;

    long *next = (long *)malloc(sizeof(long) * size);
    if (!next) return result;

    uint32_t counts[256] = {0};
    uint32_t occ[256]    = {0};
    uint32_t first[256]  = {0};

    for (long i = 0; i < size; i++)
        counts[transformed[i]]++;

    for (int i = 1; i < 256; i++)
        first[i] = first[i - 1] + counts[i - 1];

    for (long i = 0; i < size; i++) {
        uint8_t c = transformed[i];
        next[first[c] + occ[c]] = i;
        occ[c]++;
    }

    uint8_t *out = (uint8_t *)malloc(size + 1);
    if (!out) {
        free(next);
        return result;
    }

    long pos = original_index;
    for (long i = size - 1; i >= 0; i--) {
        out[i] = transformed[pos];
        pos = next[pos];
    }
    out[size] = '\0';

    result.data = out;
    result.size = size;

    if (mode == BWT_VERBOSE) {
        printf(BOLD BBLUE "\n  ╔══ BWT — Inversa de Burrows-Wheeler ═════════════╗\n" RESET);
        printf(DIM "  Reconstruye la cadena original usando la última columna.\n" RESET);
        printf(DIM "  Índice original: %ld\n\n" RESET, original_index);
        printf("  Texto restaurado (primeros 64 bytes):\n  ");
        for (long i = 0; i < size && i < 64; i++) {
            uint8_t c = out[i];
            printf("%c", (c >= 32 && c < 127) ? c : '.');
        }
        printf("\n");
        printf(BOLD BBLUE "  ╚══ FIN INVERSA BWT ═══════════════════════════════╝\n" RESET);
    }

    free(next);
    return result;
}
