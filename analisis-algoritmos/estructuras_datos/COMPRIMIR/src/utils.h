/* ============================================================
 *  utils.h  —  Utilidades compartidas para el proyecto COMPRIMIR
 *  Incluye: colores ANSI, timer de alta resolución, helpers de
 *  visualización y medición de compresión.
 * ============================================================ */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

/* ───────────────────────────── ANSI Colors ───────────────────────────────── */
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"
#define ITALIC      "\033[3m"
#define UNDERLINE   "\033[4m"

/* Colores de texto */
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

/* Colores brillantes */
#define BRED        "\033[91m"
#define BGREEN      "\033[92m"
#define BYELLOW     "\033[93m"
#define BBLUE       "\033[94m"
#define BMAGENTA    "\033[95m"
#define BCYAN       "\033[96m"
#define BWHITE      "\033[97m"

/* Fondos */
#define BG_BLACK    "\033[40m"
#define BG_BLUE     "\033[44m"
#define BG_CYAN     "\033[46m"
#define BG_GREEN    "\033[42m"
#define BG_MAGENTA  "\033[45m"

/* ───────────────────────────── Macros UI ─────────────────────────────────── */
#define LINE_DOUBLE "═══════════════════════════════════════════════════════════"
#define LINE_SINGLE "───────────────────────────────────────────────────────────"
#define LINE_DASH   "- - - - - - - - - - - - - - - - - - - - - - - - - - - - -"

#define PRINT_SEPARATOR()   printf(CYAN LINE_DOUBLE RESET "\n")
#define PRINT_THIN_SEP()    printf(DIM LINE_SINGLE RESET "\n")
#define PRINT_DASH_SEP()    printf(DIM LINE_DASH RESET "\n")

/* ───────────────────────────── Timer ─────────────────────────────────────── */
typedef struct {
    struct timespec start;
    struct timespec end;
} Timer;

/* Inicia el temporizador */
static inline void timer_start(Timer *t) {
    clock_gettime(CLOCK_MONOTONIC, &t->start);
}

/* Detiene el temporizador */
static inline void timer_stop(Timer *t) {
    clock_gettime(CLOCK_MONOTONIC, &t->end);
}

/* Devuelve el tiempo transcurrido en microsegundos */
static inline double timer_elapsed_us(const Timer *t) {
    double s  = (double)(t->end.tv_sec  - t->start.tv_sec)  * 1e6;
    double ns = (double)(t->end.tv_nsec - t->start.tv_nsec) / 1e3;
    return s + ns;
}

/* ───────────────────────────── Métricas ─────────────────────────────────── */
typedef struct {
    const char *algorithm;          /* Nombre del algoritmo          */
    long  original_bytes;           /* Tamaño original en bytes      */
    long  compressed_bytes;         /* Tamaño comprimido en bytes    */
    double compress_time_us;        /* Tiempo de compresión (µs)     */
    double decompress_time_us;      /* Tiempo de descompresión (µs)  */
} Metrics;

/* Calcula la ratio de compresión (%) */
static inline double compression_ratio(long orig, long comp) {
    if (orig == 0) return 0.0;
    return (1.0 - (double)comp / (double)orig) * 100.0;
}

/* Imprime una barra de progreso horizontal */
void print_bar(double ratio, int width);

/* Imprime el cuadro de métricas de un algoritmo */
void print_metrics(const Metrics *m);

/* Imprime la tabla comparativa de todos los algoritmos */
void print_comparison_table(const Metrics metrics[], int count);

/* Lee un archivo completo en memoria; devuelve el buffer (liberar con free) */
uint8_t *read_file(const char *path, long *out_size);

/* Escribe un buffer en disco */
int write_file(const char *path, const uint8_t *data, long size);

/* Pausa pedagógica con mensaje */
void pause_step(const char *msg);

/* Imprime bytes como hex dump (máx max_bytes) */
void hex_dump(const uint8_t *data, long size, long max_bytes);

/* Borra la pantalla */
void clear_screen(void);

/* Imprime el banner principal */
void print_banner(void);

#endif /* UTILS_H */
