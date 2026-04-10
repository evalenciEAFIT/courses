/* ============================================================
 *  main.c  —  Orquestador pedagógico de algoritmos de compresión
 *
 *  Menú principal:
 *    1. Seleccionar archivo a comprimir
 *    2. Elegir algoritmo (RLE / Huffman / LZ77 / Todos)
 *    3. Activar/desactivar modo paso a paso
 *    4. Ver estructura de datos del algoritmo
 *    5. Comparar los tres algoritmos (tabla + barras)
 *    6. Salir
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "utils.h"
#include "rle.h"
#include "huffman.h"
#include "lz.h"

/* ───────────────────────────── Estado global ───────────────────────────── */
typedef struct {
    char    filepath[512];
    uint8_t *data;
    long     size;
    int      verbose;   /* 1 = paso a paso activado */
} AppState;

/* ───────────────────────────── Helpers de menú ─────────────────────────── */
static void print_menu_principal(const AppState *st) {
    PRINT_SEPARATOR();
    printf(BOLD BBLUE "  MENÚ PRINCIPAL\n" RESET);
    PRINT_THIN_SEP();

    if (st->data) {
        printf("  Archivo cargado : " BGREEN "%s" RESET " (%ld bytes)\n",
               st->filepath, st->size);
    } else {
        printf("  " BRED "[Sin archivo seleccionado]" RESET "\n");
    }

    printf("  Modo paso a paso: %s\n\n",
           st->verbose ? BGREEN "ACTIVADO" RESET : BYELLOW "desactivado" RESET);

    printf("  " BCYAN "[1]" RESET " Seleccionar / cambiar archivo\n");
    printf("  " BCYAN "[2]" RESET " RLE — Run-Length Encoding\n");
    printf("  " BCYAN "[3]" RESET " Huffman Coding\n");
    printf("  " BCYAN "[4]" RESET " LZ77 (ventana deslizante)\n");
    printf("  " BCYAN "[5]" RESET " " BOLD "Comparar los 3 algoritmos" RESET "\n");
    printf("  " BCYAN "[6]" RESET " Ver estructura de datos\n");
    printf("  " BCYAN "[7]" RESET " Crear archivo de prueba\n");
    printf("  " BCYAN "[8]" RESET " Activar/Desactivar paso a paso\n");
    printf("  " BCYAN "[0]" RESET " Salir\n");
    PRINT_SEPARATOR();
    printf(BOLD "  Opción: " RESET);
}

static int pedir_opcion(void) {
    char buf[32];
    if (!fgets(buf, sizeof(buf), stdin)) return -1;
    return atoi(buf);
}

/* ───────────────────────────── Carga de archivo ────────────────────────── */
static void cargar_archivo(AppState *st) {
    PRINT_SEPARATOR();
    printf(BOLD "  Ingresa la ruta del archivo: " RESET);

    char path[512];
    if (!fgets(path, sizeof(path), stdin)) return;
    /* Eliminar salto de línea */
    path[strcspn(path, "\n")] = '\0';

    if (st->data) { free(st->data); st->data = NULL; }

    st->data = read_file(path, &st->size);
    if (st->data) {
        snprintf(st->filepath, sizeof(st->filepath), "%s", path);
        printf(BGREEN "  ✔ Archivo cargado: %ld bytes\n" RESET, st->size);
        printf("\n  Primeros bytes:\n");
        hex_dump(st->data, st->size, 64);
    }
}

/* ───────────────────────────── Crear archivo de prueba ─────────────────── */
static void crear_archivo_prueba(AppState *st) {
    PRINT_SEPARATOR();
    printf(BOLD BBLUE "  Crear archivo de prueba\n" RESET);
    PRINT_THIN_SEP();
    printf("  " BCYAN "[1]" RESET " Texto con muchas repeticiones (ideal para RLE)\n");
    printf("  " BCYAN "[2]" RESET " Texto natural (ideal para Huffman)\n");
    printf("  " BCYAN "[3]" RESET " Texto con patrones repetidos (ideal para LZ77)\n");
    printf("  " BCYAN "[4]" RESET " Combinado\n");
    printf(BOLD "  Opción: " RESET);

    int opt = pedir_opcion();
    const char *path = "samples/fuente/test_input.txt";

    /* Crea las carpetas necesarias */
    system("mkdir -p samples/fuente samples/comprimidos samples/descomprimidos");

    FILE *f = fopen(path, "wb");
    if (!f) { printf(BRED "  Error al crear archivo.\n" RESET); return; }

    switch (opt) {
        case 1:
            /* Muchas repeticiones → perfecto para RLE */
            for (int i = 0; i < 50; i++) fprintf(f, "AAAAAAAAAA");
            for (int i = 0; i < 50; i++) fprintf(f, "BBBBBBBBBB");
            for (int i = 0; i < 30; i++) fprintf(f, "CCCCCCCCCC");
            fprintf(f, "AAABBBCCC FIN\n");
            printf(BGREEN "  ✔ Creado: texto con repeticiones.\n" RESET);
            break;
        case 2:
            /* Texto natural → Huffman lo maneja bien */
            fprintf(f,
              "En un lugar de la Mancha, de cuyo nombre no quiero acordarme, "
              "no ha mucho tiempo que vivia un hidalgo de los de lanza en astillero, "
              "adarga antigua, rocin flaco y galgo corredor. "
              "Una olla de algo mas vaca que carnero, salpicon las mas noches, "
              "duelos y quebrantos los sabados, lantejas los viernes, "
              "algun palomino de anadidura los domingos, consumian las tres partes "
              "de su hacienda. ");
            for (int i = 0; i < 5; i++)
                fprintf(f, "El algoritmo de Huffman asigna codigos mas cortos "
                           "a los caracteres mas frecuentes. ");
            printf(BGREEN "  ✔ Creado: texto natural.\n" RESET);
            break;
        case 3:
            /* Patrones repetidos a distancia → LZ77 */
            fprintf(f, "abcdefgh ");
            for (int i = 0; i < 20; i++) fprintf(f, "abcdefgh ");
            fprintf(f, "LZ77 busca coincidencias en el historial reciente. ");
            for (int i = 0; i < 10; i++)
                fprintf(f, "LZ77 busca coincidencias en el historial reciente. ");
            printf(BGREEN "  ✔ Creado: texto con patrones.\n" RESET);
            break;
        default:
        case 4:
            /* Combinado */
            fprintf(f, "AAAAAABBBBBBBCCCCC ");
            fprintf(f, "La compresion de datos es un tema fascinante ");
            for (int i = 0; i < 15; i++)
                fprintf(f, "patron_repetido_xyz ");
            fprintf(f, "AAAAAAAAABBBBBBBBBB FIN\n");
            printf(BGREEN "  ✔ Creado: archivo combinado.\n" RESET);
            break;
    }
    fclose(f);

    /* Cargar automáticamente */
    if (st->data) { free(st->data); st->data = NULL; }
    st->data = read_file(path, &st->size);
    if (st->data) {
        strncpy(st->filepath, path, sizeof(st->filepath) - 1);
        printf("  Archivo cargado automáticamente: " BCYAN "%ld bytes\n" RESET, st->size);
    }
}

/* ───────────────────────────── Ver estructura de datos ─────────────────── */
static void ver_estructuras(void) {
    clear_screen();
    PRINT_SEPARATOR();
    printf(BOLD BBLUE "  ESTRUCTURAS DE DATOS — Resumen comparativo\n" RESET);
    PRINT_SEPARATOR();

    printf(BOLD BGREEN "\n  ┌─ RLE (Run-Length Encoding) ─────────────────────────────────\n" RESET);
    printf("  │  Estructura : Arreglo de pares { uint8_t count, uint8_t byte }\n");
    printf("  │  Memoria    : O(n) — sin estructuras auxiliares\n");
    printf("  │  Stream     : [3][A] [5][B] [2][C] ...\n");
    printf("  │  Idx acceso : O(n) — recorrido lineal\n");
    printf(BOLD BGREEN "  └──────────────────────────────────────────────────────────────\n" RESET);

    printf(BOLD BMAGENTA "\n  ┌─ Huffman Coding ─────────────────────────────────────────────\n" RESET);
    printf("  │  Fase 1: long freq[256]         — tabla de frecuencias\n");
    printf("  │  Fase 2: MinHeap (array[512])   — cola de prioridad mínima\n");
    printf("  │        : HuffNode (árbol binario) — nodos con {sym, freq, left, right}\n");
    printf("  │  Fase 3: HuffCode table[256]    — string de '0'/'1' por símbolo\n");
    printf("  │  Salida : bits empaquetados en uint8_t[]\n");
    printf("  │  Memoria: O(k) — k = número de símbolos únicos (max 256)\n");
    printf(BOLD BMAGENTA "  └──────────────────────────────────────────────────────────────\n" RESET);

    printf(BOLD BCYAN "\n  ┌─ LZ77 (Lempel-Ziv 1977) ──────────────────────────────────────\n" RESET);
    printf("  │  Concepto   : ventana deslizante (historial + lookahead)\n");
    printf("  │  Ventana    : últimos %d bytes procesados\n", LZ_WINDOW_SIZE);
    printf("  │  Lookahead  : siguiente %d bytes a codificar\n", LZ_LOOKAHEAD_MAX);
    printf("  │  Token      : { uint16_t dist, uint8_t len, uint8_t next }\n");
    printf("  │  Salida     : LZ77Token[]  (%zu bytes/token)\n", sizeof(LZ77Token));
    printf("  │  Memoria    : O(window_size + lookahead)\n");
    printf(BOLD BCYAN "  └──────────────────────────────────────────────────────────────\n" RESET);

    printf(BOLD BYELLOW "\n  ┌─ Comparativa de aplicaciones ───────────────────────────────\n" RESET);
    printf("  │  Algoritmo   Mejor para            Ejemplos reales\n");
    printf("  │  ─────────────────────────────────────────────────────────\n");
    printf("  │  RLE         Imágenes planas        BMP, PCX, fax TIFF\n");
    printf("  │  Huffman     Texto, como post-paso  ZIP, JPEG, MP3, DEFLATE\n");
    printf("  │  LZ77        Texto, código fuente   gzip, PNG, zlib, ZIP\n");
    printf(BOLD BYELLOW "  └──────────────────────────────────────────────────────────────\n" RESET);

    printf(DIM "\n  Presiona ENTER para volver...\n" RESET);
    while (getchar() != '\n');
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  RUNNERS INDIVIDUALES DE CADA ALGORITMO
 * ═══════════════════════════════════════════════════════════════════════════ */

static Metrics run_rle(const AppState *st) {
    Metrics m = { "RLE", st->size, 0, 0.0, 0.0 };
    Timer t;
    RLEMode mode = st->verbose ? RLE_VERBOSE : RLE_SILENT;

    PRINT_SEPARATOR();
    printf(BOLD BGREEN "  ─── Algoritmo: RLE ───\n" RESET);

    /* Comprimir */
    timer_start(&t);
    RLEResult comp = rle_compress(st->data, st->size, mode);
    timer_stop(&t);
    m.compress_time_us = timer_elapsed_us(&t);

    if (!comp.data) {
        printf(BRED "  Error en RLE compresión.\n" RESET);
        return m;
    }

    m.compressed_bytes = comp.size;

    /* Descomprimir (verifica integridad) */
    timer_start(&t);
    RLEResult decomp = rle_decompress(comp.data, comp.size,
                                      st->verbose ? RLE_VERBOSE : RLE_SILENT);
    timer_stop(&t);
    m.decompress_time_us = timer_elapsed_us(&t);

    /* Verificar integridad */
    int ok = (decomp.size == st->size &&
              memcmp(decomp.data, st->data, st->size) == 0);
    printf("  Integridad: %s\n", ok ? BGREEN "✔ OK" RESET : BRED "✘ FALLO" RESET);

    print_metrics(&m);

    /* Guardar archivos */
    write_file("samples/comprimidos/out_rle.bin", comp.data, comp.size);
    write_file("samples/descomprimidos/out_rle_decomp.bin", decomp.data, decomp.size);
    printf(DIM "  Archivos: samples/comprimidos/out_rle.bin\n" RESET);
    printf(DIM "            samples/descomprimidos/out_rle_decomp.bin\n" RESET);

    free(comp.data);
    free(decomp.data);
    return m;
}

static Metrics run_huffman(const AppState *st) {
    Metrics m = { "Huffman", st->size, 0, 0.0, 0.0 };
    Timer t;
    HuffMode mode = st->verbose ? HUFF_VERBOSE : HUFF_SILENT;

    PRINT_SEPARATOR();
    printf(BOLD BMAGENTA "  ─── Algoritmo: Huffman ───\n" RESET);

    /* Comprimir */
    timer_start(&t);
    HuffResult comp = huff_compress(st->data, st->size, mode);
    timer_stop(&t);
    m.compress_time_us = timer_elapsed_us(&t);

    if (!comp.data) {
        printf(BRED "  Error en Huffman compresión.\n" RESET);
        return m;
    }
    m.compressed_bytes = comp.size;

    /* Descomprimir */
    timer_start(&t);
    HuffDecompResult decomp = huff_decompress(comp.data, comp.size,
                                               comp.bit_count,
                                               comp.tree_root,
                                               mode);
    timer_stop(&t);
    m.decompress_time_us = timer_elapsed_us(&t);

    int ok = (decomp.size == st->size &&
              memcmp(decomp.data, st->data, st->size) == 0);
    printf("  Integridad: %s\n", ok ? BGREEN "✔ OK" RESET : BRED "✘ FALLO" RESET);

    print_metrics(&m);

    write_file("samples/comprimidos/out_huffman.bin", comp.data, comp.size);
    write_file("samples/descomprimidos/out_huffman_decomp.bin", decomp.data, decomp.size);
    printf(DIM "  Archivos: samples/comprimidos/out_huffman.bin\n" RESET);
    printf(DIM "            samples/descomprimidos/out_huffman_decomp.bin\n" RESET);

    huff_free_tree(comp.tree_root);
    free(comp.data);
    free(decomp.data);
    return m;
}

static Metrics run_lz77(const AppState *st) {
    Metrics m = { "LZ77", st->size, 0, 0.0, 0.0 };
    Timer t;
    LZMode mode = st->verbose ? LZ_VERBOSE : LZ_SILENT;

    PRINT_SEPARATOR();
    printf(BOLD BCYAN "  ─── Algoritmo: LZ77 ───\n" RESET);

    /* Comprimir */
    timer_start(&t);
    LZResult comp = lz_compress(st->data, st->size, mode);
    timer_stop(&t);
    m.compress_time_us = timer_elapsed_us(&t);

    if (!comp.tokens) {
        printf(BRED "  Error en LZ77 compresión.\n" RESET);
        return m;
    }
    m.compressed_bytes = comp.raw_size;

    /* Descomprimir */
    timer_start(&t);
    LZDecompResult decomp = lz_decompress(comp.tokens, comp.count,
                                          st->size, mode);
    timer_stop(&t);
    m.decompress_time_us = timer_elapsed_us(&t);

    int ok = (decomp.size == st->size &&
              memcmp(decomp.data, st->data, st->size) == 0);
    printf("  Integridad: %s\n", ok ? BGREEN "✔ OK" RESET : BRED "✘ FALLO" RESET);

    print_metrics(&m);

    /* Guardar los tokens como binario */
    write_file("samples/comprimidos/out_lz77.bin", (uint8_t *)comp.tokens, comp.raw_size);
    write_file("samples/descomprimidos/out_lz77_decomp.bin", decomp.data, decomp.size);
    printf(DIM "  Archivos: samples/comprimidos/out_lz77.bin\n" RESET);
    printf(DIM "            samples/descomprimidos/out_lz77_decomp.bin\n" RESET);

    free(comp.tokens);
    free(decomp.data);
    return m;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  COMPARACIÓN DE LOS TRES ALGORITMOS
 * ═══════════════════════════════════════════════════════════════════════════ */
static void comparar_todos(const AppState *st) {
    clear_screen();
    print_banner();
    printf(BOLD BWHITE "  ▶ COMPARACIÓN DE LOS TRES ALGORITMOS\n\n" RESET);

    /* Ejecutar los tres en modo silencioso para no contaminar la tabla */
    AppState silent = *st;
    silent.verbose = 0;

    Metrics metrics[3];
    metrics[0] = run_rle(&silent);
    metrics[1] = run_huffman(&silent);
    metrics[2] = run_lz77(&silent);

    printf("\n");
    PRINT_SEPARATOR();
    printf(BOLD BWHITE "  TABLA COMPARATIVA\n" RESET);
    print_comparison_table(metrics, 3);

    /* Determinar el ganador en ratio y tiempo */
    int   best_ratio_idx = 0, best_time_idx = 0;
    double best_ratio = -1e9, best_time = 1e18;

    for (int i = 0; i < 3; i++) {
        double r = compression_ratio(metrics[i].original_bytes,
                                     metrics[i].compressed_bytes);
        if (r > best_ratio) { best_ratio = r; best_ratio_idx = i; }
        if (metrics[i].compress_time_us < best_time) {
            best_time = metrics[i].compress_time_us;
            best_time_idx = i;
        }
    }

    printf(BOLD "\n  🏆 Mejor ratio    : " BGREEN "%s" RESET BOLD " (%.2f%%)\n" RESET,
           metrics[best_ratio_idx].algorithm, best_ratio);
    printf(BOLD "  ⚡ Más rápido     : " BGREEN "%s" RESET BOLD " (%.2f µs)\n\n" RESET,
           metrics[best_time_idx].algorithm, best_time);

    printf(DIM "  Presiona ENTER para volver al menú...\n" RESET);
    while (getchar() != '\n');
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  MAIN
 * ═══════════════════════════════════════════════════════════════════════════ */
int main(void) {
    AppState st;
    memset(&st, 0, sizeof(st));
    st.verbose = 0;

    clear_screen();
    print_banner();
    printf(BOLD DIM "  Bienvenido al laboratorio de compresión EDA.\n\n" RESET);
    printf(DIM "  Este programa permite explorar paso a paso los algoritmos\n"
               "  RLE, Huffman y LZ77, comparar tiempos y ratios de compresión.\n\n" RESET);
    printf(DIM "  Presiona ENTER para continuar...\n" RESET);
    while (getchar() != '\n');

    int running = 1;
    while (running) {
        clear_screen();
        print_banner();
        print_menu_principal(&st);

        int opt = pedir_opcion();

        switch (opt) {
            case 1:
                clear_screen();
                cargar_archivo(&st);
                printf(DIM "\n  ENTER para continuar...\n" RESET);
                while (getchar() != '\n');
                break;

            case 2:
                if (!st.data) {
                    printf(BRED "  Primero carga un archivo (opción 1).\n" RESET);
                    printf(DIM "  ENTER...\n" RESET);
                    while (getchar() != '\n');
                } else {
                    clear_screen(); print_banner();
                    Metrics m = run_rle(&st);
                    (void)m;
                    printf(DIM "\n  ENTER para volver...\n" RESET);
                    while (getchar() != '\n');
                }
                break;

            case 3:
                if (!st.data) {
                    printf(BRED "  Primero carga un archivo (opción 1).\n" RESET);
                    printf(DIM "  ENTER...\n" RESET);
                    while (getchar() != '\n');
                } else {
                    clear_screen(); print_banner();
                    Metrics m = run_huffman(&st);
                    (void)m;
                    printf(DIM "\n  ENTER para volver...\n" RESET);
                    while (getchar() != '\n');
                }
                break;

            case 4:
                if (!st.data) {
                    printf(BRED "  Primero carga un archivo (opción 1).\n" RESET);
                    printf(DIM "  ENTER...\n" RESET);
                    while (getchar() != '\n');
                } else {
                    clear_screen(); print_banner();
                    Metrics m = run_lz77(&st);
                    (void)m;
                    printf(DIM "\n  ENTER para volver...\n" RESET);
                    while (getchar() != '\n');
                }
                break;

            case 5:
                if (!st.data) {
                    printf(BRED "  Primero carga un archivo (opción 1).\n" RESET);
                    printf(DIM "  ENTER...\n" RESET);
                    while (getchar() != '\n');
                } else {
                    comparar_todos(&st);
                }
                break;

            case 6:
                ver_estructuras();
                break;

            case 7:
                clear_screen(); print_banner();
                crear_archivo_prueba(&st);
                printf(DIM "\n  ENTER para continuar...\n" RESET);
                while (getchar() != '\n');
                break;

            case 8:
                st.verbose = !st.verbose;
                printf("\n  Modo paso a paso: %s\n",
                       st.verbose ? BGREEN "ACTIVADO" RESET
                                  : BYELLOW "desactivado" RESET);
                printf(DIM "  ENTER...\n" RESET);
                while (getchar() != '\n');
                break;

            case 0:
                running = 0;
                break;

            default:
                printf(BRED "  Opción inválida.\n" RESET);
                break;
        }
    }

    /* Limpieza */
    if (st.data) free(st.data);

    clear_screen();
    print_banner();
    printf(BOLD BGREEN "  ¡Hasta luego! Sigue explorando algoritmos.\n\n" RESET);
    return 0;
}
