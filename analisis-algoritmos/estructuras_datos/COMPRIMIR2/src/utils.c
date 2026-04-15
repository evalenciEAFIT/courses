/* ============================================================
 *  utils.c  —  Implementación de las utilidades compartidas
 * ============================================================ */

#include "utils.h"

/* ───────────────────────────── Barra de progreso ────────────────────────── */
void print_bar(double ratio, int width) {
    int filled = (int)(ratio / 100.0 * width);
    if (filled < 0) filled = 0;
    if (filled > width) filled = width;

    const char *color = (ratio >= 50.0) ? BGREEN :
                        (ratio >= 20.0) ? BYELLOW : BRED;

    printf("%s[", color);
    for (int i = 0; i < width; i++) {
        if (i < filled) printf("█");
        else            printf("░");
    }
    printf("] %6.2f%%" RESET, ratio);
}

/* ───────────────────────────── Métricas individuales ───────────────────── */
void print_metrics(const Metrics *m) {
    double ratio = compression_ratio(m->original_bytes, m->compressed_bytes);
    double savings = (double)(m->original_bytes - m->compressed_bytes);

    PRINT_THIN_SEP();
    printf(BOLD BBLUE "  Algoritmo : " RESET BOLD BWHITE "%s\n" RESET, m->algorithm);
    PRINT_THIN_SEP();
    printf("  %-25s %s%ld bytes%s\n",
           "Tamaño original:",   BYELLOW, m->original_bytes,   RESET);
    printf("  %-25s %s%ld bytes%s\n",
           "Tamaño comprimido:", BCYAN,   m->compressed_bytes, RESET);
    printf("  %-25s %s%.0f bytes%s\n",
           "Ahorro:",            savings >= 0 ? BGREEN : BRED, savings, RESET);
    printf("  %-25s ", "Ratio de compresión:");
    print_bar(ratio < 0 ? 0 : ratio, 30);
    printf("\n");
    printf("  %-25s %s%.2f µs%s\n",
           "Tiempo comprimir:",   BMAGENTA, m->compress_time_us,   RESET);
    printf("  %-25s %s%.2f µs%s\n",
           "Tiempo descomprimir:", BMAGENTA, m->decompress_time_us, RESET);
}

/* ───────────────────────────── Tabla comparativa ───────────────────────── */
void print_comparison_table(const Metrics metrics[], int count) {
    PRINT_SEPARATOR();
    printf(BOLD BG_BLUE BWHITE
           "  %-12s  %10s  %10s  %10s  %12s  %12s  "
           RESET "\n",
           "Algoritmo", "Original", "Comprim.", "Ratio (%)",
           "T.Comp(µs)", "T.Decomp(µs)");
    PRINT_THIN_SEP();

    for (int i = 0; i < count; i++) {
        const Metrics *m = &metrics[i];
        double ratio = compression_ratio(m->original_bytes, m->compressed_bytes);
        const char *rc = (ratio >= 50.0) ? BGREEN :
                         (ratio >= 20.0) ? BYELLOW : BRED;
        printf("  " BOLD BCYAN "%-12s" RESET
               "  %10ld  %10ld  " "%s%10.2f" RESET
               "  %12.2f  %12.2f\n",
               m->algorithm,
               m->original_bytes,
               m->compressed_bytes,
               rc, ratio,
               m->compress_time_us,
               m->decompress_time_us);
    }
    PRINT_SEPARATOR();
}

/* ───────────────────────────── I/O de archivos ─────────────────────────── */
uint8_t *read_file(const char *path, long *out_size) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, RED "  [ERROR] No se pudo abrir: %s\n" RESET, path);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    uint8_t *buf = (uint8_t *)malloc(size + 1);
    if (!buf) {
        fprintf(stderr, RED "  [ERROR] Sin memoria para leer: %s\n" RESET, path);
        fclose(f);
        return NULL;
    }
    fread(buf, 1, size, f);
    buf[size] = '\0';
    fclose(f);
    *out_size = size;
    return buf;
}

int write_file(const char *path, const uint8_t *data, long size) {
    FILE *f = fopen(path, "wb");
    if (!f) {
        fprintf(stderr, RED "  [ERROR] No se pudo escribir: %s\n" RESET, path);
        return -1;
    }
    fwrite(data, 1, size, f);
    fclose(f);
    return 0;
}

/* ───────────────────────────── Pausa pedagógica ─────────────────────────── */
void pause_step(const char *msg) {
    printf(BYELLOW "\n  ▶ %s" RESET, msg);
    printf(DIM " [Presiona ENTER para continuar]" RESET);
    while (getchar() != '\n');
}

/* ───────────────────────────── Hex dump ─────────────────────────────────── */
void hex_dump(const uint8_t *data, long size, long max_bytes) {
    long show = (size < max_bytes) ? size : max_bytes;
    printf(DIM "  Offset  ");
    for (int i = 0; i < 16; i++) printf("%02X ", i);
    printf(RESET "\n");
    PRINT_DASH_SEP();

    for (long i = 0; i < show; i += 16) {
        printf(DIM "  %06lX  " RESET, i);
        for (long j = i; j < i + 16; j++) {
            if (j < show) printf(BCYAN "%02X " RESET, data[j]);
            else          printf("   ");
        }
        printf(" |");
        for (long j = i; j < i + 16 && j < show; j++) {
            char c = (char)data[j];
            printf(BWHITE "%c" RESET, (c >= 32 && c < 127) ? c : '.');
        }
        printf("|\n");
    }
    if (size > max_bytes) {
        printf(DIM "  ... (%ld bytes más)\n" RESET, size - max_bytes);
    }
}

/* ───────────────────────────── Pantalla / Banner ───────────────────────── */
void clear_screen(void) {
    printf("\033[2J\033[H");
}

void print_banner(void) {
    PRINT_SEPARATOR();
    printf(BOLD BMAGENTA
    "  ██████╗ ██████╗ ███╗   ███╗██████╗ ██████╗ ██╗███╗   ███╗██╗██████╗ \n"
    " ██╔════╝██╔═══██╗████╗ ████║██╔══██╗██╔══██╗██║████╗ ████║██║██╔══██╗\n"
    " ██║     ██║   ██║██╔████╔██║██████╔╝██████╔╝██║██╔████╔██║██║██████╔╝\n"
    " ██║     ██║   ██║██║╚██╔╝██║██╔═══╝ ██╔══██╗██║██║╚██╔╝██║██║██╔══██╗\n"
    " ╚██████╗╚██████╔╝██║ ╚═╝ ██║██║     ██║  ██║██║██║ ╚═╝ ██║██║██║  ██║\n"
    "  ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚═╝     ╚═╝  ╚═╝╚═╝╚═╝     ╚═╝╚═╝╚═╝  ╚═╝\n"
    RESET);
    printf(BOLD BCYAN
    "           Laboratorio Pedagógico de Algoritmos de Compresión\n"
    RESET);
    printf(DIM
    "           RLE • Huffman • LZ77 • LZ78 • LZW • LZ4 • Deflate • Zstd • BWT\n"
    "           Explora, compara y entiende cómo funcionan los compresores modernos\n"
    RESET);
    PRINT_SEPARATOR();
    printf("\n");
}
