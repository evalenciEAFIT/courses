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
#include "lz78.h"
#include "lzw.h"
#include "lz4.h"
#include "deflate.h"
#include "zstd.h"
#include "bwt.h"

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
    printf(BOLD BBLUE "  MENÚ PRINCIPAL — LABORATORIO DE COMPRESIÓN\n" RESET);
    PRINT_THIN_SEP();

    if (st->data) {
        printf("  Archivo cargado : " BGREEN "%s" RESET " (%ld bytes)\n",
               st->filepath, st->size);
    } else {
        printf("  " BRED "[Sin archivo seleccionado]" RESET "\n");
    }

    printf("  Modo paso a paso: %s\n\n",
           st->verbose ? BGREEN "ACTIVADO" RESET : BYELLOW "desactivado" RESET);

    /* ── Gestión de Datos ───────────────────────────────────────────────── */
    printf(BOLD BYELLOW "  GESTIÓN DE DATOS\n" RESET);
    printf("  " BCYAN "[1]" RESET " Seleccionar / cambiar archivo\n");
    printf("  " BCYAN "[14]" RESET " Crear archivo de prueba\n");
    printf("\n");

    /* ── Algoritmos Individuales ────────────────────────────────────────── */
    printf(BOLD BGREEN "  ALGORITMOS INDIVIDUALES\n" RESET);
    printf("  " BCYAN "[2]" RESET " RLE — Run-Length Encoding (rachas repetidas)\n");
    printf("  " BCYAN "[3]" RESET " Huffman — Codificación de entropía\n");
    printf("  " BCYAN "[4]" RESET " LZ77 — Ventana deslizante\n");
    printf("  " BCYAN "[5]" RESET " LZ78 — Diccionario de frases\n");
    printf("  " BCYAN "[6]" RESET " LZW — Diccionario dinámico\n");
    printf("  " BCYAN "[7]" RESET " LZ4 — Compresión rápida\n");
    printf("  " BCYAN "[8]" RESET " Deflate — LZ77 + Huffman (como ZIP)\n");
    printf("  " BCYAN "[9]" RESET " Zstd — BWT + Huffman (como Zstandard)\n");
    printf("  " BCYAN "[10]" RESET " BWT — Burrows-Wheeler Transform\n");
    printf("\n");

    /* ── Comparaciones ──────────────────────────────────────────────────── */
    printf(BOLD BMAGENTA "  COMPARACIONES\n" RESET);
    printf("  " BCYAN "[11]" RESET " Comparar todos los algoritmos\n");
    printf("  " BCYAN "[12]" RESET " Comparar algoritmos LZ\n");
    printf("\n");

    /* ── Herramientas Pedagógicas ───────────────────────────────────────── */
    printf(BOLD BCYAN "  HERRAMIENTAS PEDAGÓGICAS\n" RESET);
    printf("  " BCYAN "[13]" RESET " Ver estructuras de datos\n");
    printf("  " BCYAN "[15]" RESET " Activar/Desactivar modo paso a paso\n");
    printf("\n");

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
    printf("  " BCYAN "[4]" RESET " Texto de frases crecientes (ideal para LZ78)\n");
    printf("  " BCYAN "[5]" RESET " Texto con frases repetidas y extensión (ideal para LZW)\n");
    printf("  " BCYAN "[6]" RESET " Texto rotacional / repetitivo (ideal para BWT)\n");
    printf("  " BCYAN "[7]" RESET " Combinado\n");
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
        case 4:
            /* Frases crecientes → ideal para LZ78 */
            fprintf(f, "A ");
            fprintf(f, "AB ");
            fprintf(f, "ABA ");
            fprintf(f, "ABABABA ");
            fprintf(f, "ABABABABABA ");
            fprintf(f, "ABABABABABABABABA \n");
            fprintf(f, "ABABABABABABABABABABABA \n");
            printf(BGREEN "  ✔ Creado: texto de frases crecientes para LZ78.\n" RESET);
            break;
        case 5:
            /* Frases repetidas con extensión → ideal para LZW */
            fprintf(f, "TOBEORNOTTOBEORNOTTOBEORNOT ");
            fprintf(f, "TOBEORNOTTOBEORNOTTOBEORNOT ");
            fprintf(f, "TOBEORNOTTOBEORNOTTOBEORNOT \n");
            fprintf(f, "TOBEORNOTTOBEORNOTTOBEORNOTTOBEORNOT ");
            printf(BGREEN "  ✔ Creado: texto con frases repetidas para LZW.\n" RESET);
            break;
        case 6:
            /* Texto rotacional → ideal para BWT */
            for (int i = 0; i < 5; i++)
                fprintf(f, "banana_bandana_abacaba_\n");
            fprintf(f, "abracadabra_abra_cadabra_abracadabra\n");
            printf(BGREEN "  ✔ Creado: texto rotacional para BWT.\n" RESET);
            break;
        default:
        case 7:
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

    printf(BOLD BCYAN "\n  ┌─ LZ78 (Lempel-Ziv 1978) ─────────────────────────────────────\n" RESET);
    printf("  │  Concepto   : diccionario de frases {índice, símbolo}\n");
    printf("  │  Token      : { uint16_t index, uint8_t next_byte }\n");
    printf("  │  Salida     : LZ78Token[]  (%zu bytes/token)\n", sizeof(LZ78Token));
    printf("  │  Memoria    : O(n) — diccionario de frases dinámico\n");
    printf(BOLD BCYAN "  └──────────────────────────────────────────────────────────────\n" RESET);

    printf(BOLD BCYAN "\n  ┌─ LZW (Lempel-Ziv-Welch) ────────────────────────────────────\n" RESET);
    printf("  │  Concepto   : diccionario inicial de 256 símbolos codificados\n");
    printf("  │  Token      : { uint16_t code }\n");
    printf("  │  Salida     : LZWToken[]  (%zu bytes/token)\n", sizeof(LZWToken));
    printf("  │  Memoria    : O(n) — diccionario creciente de frases\n");
    printf(BOLD BCYAN "  └──────────────────────────────────────────────────────────────\n" RESET);

    printf(BOLD BMAGENTA "\n  ┌─ BWT (Burrows-Wheeler Transform) ───────────────────────────\n" RESET);
    printf("  │  Concepto   : ordena rotaciones de la cadena\n");
    printf("  │  Salida     : última columna + índice original\n");
    printf("  │  Uso        : preprocesamiento antes de RLE/Huffman\n");
    printf("  │  Memoria    : O(n) — matriz implícita de rotaciones\n");
    printf(BOLD BMAGENTA "  └──────────────────────────────────────────────────────────────\n" RESET);

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
    printf(DIM "       Reemplaza rachas de bytes iguales por un contador.\n" RESET);
    printf(DIM "       Simple y rápido, pero solo efectivo en datos con repeticiones consecutivas.\n" RESET);

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
    printf(DIM "       Asigna códigos binarios variables basados en frecuencia.\n" RESET);
    printf(DIM "       Símbolos comunes usan menos bits — óptimo para entropía.\n" RESET);

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
    printf(DIM "       Busca coincidencias en una ventana deslizante del historial.\n" RESET);
    printf(DIM "       Emite referencias (distancia, longitud) para patrones repetidos.\n" RESET);

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

static Metrics run_lz78(const AppState *st) {
    Metrics m = { "LZ78", st->size, 0, 0.0, 0.0 };
    Timer t;
    LZ78Mode mode = st->verbose ? LZ78_VERBOSE : LZ78_SILENT;

    PRINT_SEPARATOR();
    printf(BOLD BCYAN "  ─── Algoritmo: LZ78 ───\n" RESET);
    printf(DIM "       Construye un diccionario de frases únicas.\n" RESET);
    printf(DIM "       Emite índices de frases previas, extendiendo dinámicamente.\n" RESET);

    timer_start(&t);
    LZ78Result comp = lz78_compress(st->data, st->size, mode);
    timer_stop(&t);
    m.compress_time_us = timer_elapsed_us(&t);

    if (!comp.tokens) {
        printf(BRED "  Error en LZ78 compresión.\n" RESET);
        return m;
    }
    m.compressed_bytes = comp.raw_size;

    timer_start(&t);
    LZ78DecompResult decomp = lz78_decompress(comp.tokens, comp.count,
                                             st->size, mode);
    timer_stop(&t);
    m.decompress_time_us = timer_elapsed_us(&t);

    int ok = (decomp.size == st->size &&
              memcmp(decomp.data, st->data, st->size) == 0);
    printf("  Integridad: %s\n", ok ? BGREEN "✔ OK" RESET : BRED "✘ FALLO" RESET);

    print_metrics(&m);
    write_file("samples/comprimidos/out_lz78.bin", (uint8_t *)comp.tokens, comp.raw_size);
    write_file("samples/descomprimidos/out_lz78_decomp.bin", decomp.data, decomp.size);
    printf(DIM "  Archivos: samples/comprimidos/out_lz78.bin\n" RESET);
    printf(DIM "            samples/descomprimidos/out_lz78_decomp.bin\n" RESET);

    free(comp.tokens);
    free(decomp.data);
    return m;
}

static Metrics run_lzw(const AppState *st) {
    Metrics m = { "LZW", st->size, 0, 0.0, 0.0 };
    Timer t;
    LZWMode mode = st->verbose ? LZW_VERBOSE : LZW_SILENT;

    PRINT_SEPARATOR();
    printf(BOLD BCYAN "  ─── Algoritmo: LZW ───\n" RESET);
    printf(DIM "       Diccionario inicial de bytes únicos, crece con frases.\n" RESET);
    printf(DIM "       Emite códigos de frases, adaptándose al contenido.\n" RESET);

    timer_start(&t);
    LZWResult comp = lzw_compress(st->data, st->size, mode);
    timer_stop(&t);
    m.compress_time_us = timer_elapsed_us(&t);

    if (!comp.tokens) {
        printf(BRED "  Error en LZW compresión.\n" RESET);
        return m;
    }
    m.compressed_bytes = comp.raw_size;

    timer_start(&t);
    LZWDecompResult decomp = lzw_decompress(comp.tokens, comp.count,
                                           st->size, mode);
    timer_stop(&t);
    m.decompress_time_us = timer_elapsed_us(&t);

    int ok = (decomp.size == st->size &&
              memcmp(decomp.data, st->data, st->size) == 0);
    printf("  Integridad: %s\n", ok ? BGREEN "✔ OK" RESET : BRED "✘ FALLO" RESET);

    print_metrics(&m);
    write_file("samples/comprimidos/out_lzw.bin", (uint8_t *)comp.tokens, comp.raw_size);
    write_file("samples/descomprimidos/out_lzw_decomp.bin", decomp.data, decomp.size);
    printf(DIM "  Archivos: samples/comprimidos/out_lzw.bin\n" RESET);
    printf(DIM "            samples/descomprimidos/out_lzw_decomp.bin\n" RESET);

    free(comp.tokens);
    free(decomp.data);
    return m;
}

static Metrics run_bwt(const AppState *st) {
    Metrics m = { "BWT", st->size, 0, 0.0, 0.0 };
    Timer t;
    BWTMode mode = st->verbose ? BWT_VERBOSE : BWT_SILENT;

    PRINT_SEPARATOR();
    printf(BOLD BMAGENTA "  ─── Algoritmo: BWT ───\n" RESET);
    printf(DIM "       Reordena bytes creando rotaciones y ordenándolas.\n" RESET);
    printf(DIM "       Agrupa símbolos similares para mejorar compresores posteriores.\n" RESET);

    timer_start(&t);
    BWTResult comp = bwt_transform(st->data, st->size, mode);
    timer_stop(&t);
    m.compress_time_us = timer_elapsed_us(&t);

    if (!comp.data) {
        printf(BRED "  Error en BWT transformacion.\n" RESET);
        return m;
    }
    m.compressed_bytes = comp.size + sizeof(uint32_t);

    timer_start(&t);
    BWTDecompResult decomp = bwt_inverse(comp.data, comp.size,
                                         comp.original_index, mode);
    timer_stop(&t);
    m.decompress_time_us = timer_elapsed_us(&t);

    int ok = (decomp.size == st->size &&
              memcmp(decomp.data, st->data, st->size) == 0);
    printf("  Integridad: %s\n", ok ? BGREEN "✔ OK" RESET : BRED "✘ FALLO" RESET);

    print_metrics(&m);

    long header_size = 4;
    long out_size = comp.size + header_size;
    uint8_t *out_buf = (uint8_t *)malloc(out_size);
    if (out_buf) {
        out_buf[0] = (uint8_t)(comp.original_index >> 0);
        out_buf[1] = (uint8_t)(comp.original_index >> 8);
        out_buf[2] = (uint8_t)(comp.original_index >> 16);
        out_buf[3] = (uint8_t)(comp.original_index >> 24);
        memcpy(out_buf + header_size, comp.data, comp.size);
        write_file("samples/comprimidos/out_bwt.bin", out_buf, out_size);
        free(out_buf);
    }
    write_file("samples/descomprimidos/out_bwt_decomp.bin", decomp.data, decomp.size);
    printf(DIM "  Archivos: samples/comprimidos/out_bwt.bin\n" RESET);
    printf(DIM "            samples/descomprimidos/out_bwt_decomp.bin\n" RESET);

    free(comp.data);
    free(decomp.data);
    return m;
}

static Metrics run_lz4(const AppState *st) {
    Metrics m = { "LZ4", st->size, 0, 0.0, 0.0 };
    Timer t;
    LZ4Mode mode = st->verbose ? LZ4_VERBOSE : LZ4_SILENT;

    PRINT_SEPARATOR();
    printf(BOLD BCYAN "  ─── Algoritmo: LZ4 ───\n" RESET);
    printf(DIM "       Compresor rápido sin codificación de entropía.\n" RESET);
    printf(DIM "       Ideal para datos en tiempo real donde la velocidad importa más que el ratio.\n" RESET);

    timer_start(&t);
    LZ4Result comp = lz4_compress(st->data, st->size, mode);
    timer_stop(&t);
    m.compress_time_us = timer_elapsed_us(&t);

    if (!comp.data) {
        printf(BRED "  Error en LZ4 compresión.\n" RESET);
        return m;
    }
    m.compressed_bytes = comp.size;

    timer_start(&t);
    LZ4DecompResult decomp = lz4_decompress(comp.data, comp.size, st->size, mode);
    timer_stop(&t);
    m.decompress_time_us = timer_elapsed_us(&t);

    int ok = (decomp.size == st->size &&
              memcmp(decomp.data, st->data, st->size) == 0);
    printf("  Integridad: %s\n", ok ? BGREEN "✔ OK" RESET : BRED "✘ FALLO" RESET);

    print_metrics(&m);
    write_file("samples/comprimidos/out_lz4.bin", comp.data, comp.size);
    write_file("samples/descomprimidos/out_lz4_decomp.bin", decomp.data, decomp.size);
    printf(DIM "  Archivos: samples/comprimidos/out_lz4.bin\n" RESET);
    printf(DIM "            samples/descomprimidos/out_lz4_decomp.bin\n" RESET);

    free(comp.data);
    free(decomp.data);
    return m;
}

static Metrics run_deflate(const AppState *st) {
    Metrics m = { "Deflate", st->size, 0, 0.0, 0.0 };
    Timer t;
    DeflateMode mode = st->verbose ? DEF_VERBOSE : DEF_SILENT;

    PRINT_SEPARATOR();
    printf(BOLD BMAGENTA "  ─── Algoritmo: Deflate ───\n" RESET);
    printf(DIM "       Combina LZ77 (búsqueda de patrones) con Huffman (codificación óptima).\n" RESET);
    printf(DIM "       Usado en ZIP, gzip, PNG — balance perfecto entre ratio y velocidad.\n" RESET);

    timer_start(&t);
    DeflateResult comp = deflate_compress(st->data, st->size, mode);
    timer_stop(&t);
    m.compress_time_us = timer_elapsed_us(&t);

    if (!comp.data) {
        printf(BRED "  Error en Deflate compresión.\n" RESET);
        return m;
    }
    m.compressed_bytes = comp.size;

    timer_start(&t);
    DeflateDecompResult decomp = deflate_decompress(comp.data,
                                                   comp.size,
                                                   comp.bit_count,
                                                   comp.tree_root,
                                                   st->size,
                                                   mode);
    timer_stop(&t);
    m.decompress_time_us = timer_elapsed_us(&t);

    int ok = (decomp.size == st->size &&
              memcmp(decomp.data, st->data, st->size) == 0);
    printf("  Integridad: %s\n", ok ? BGREEN "✔ OK" RESET : BRED "✘ FALLO" RESET);

    print_metrics(&m);
    write_file("samples/comprimidos/out_deflate.bin", comp.data, comp.size);
    write_file("samples/descomprimidos/out_deflate_decomp.bin", decomp.data, decomp.size);
    printf(DIM "  Archivos: samples/comprimidos/out_deflate.bin\n" RESET);
    printf(DIM "            samples/descomprimidos/out_deflate_decomp.bin\n" RESET);

    huff_free_tree(comp.tree_root);
    free(comp.data);
    free(decomp.data);
    return m;
}

static Metrics run_zstd(const AppState *st) {
    Metrics m = { "Zstd", st->size, 0, 0.0, 0.0 };
    Timer t;
    ZSTDMode mode = st->verbose ? ZSTD_VERBOSE : ZSTD_SILENT;

    PRINT_SEPARATOR();
    printf(BOLD BGREEN "  ─── Algoritmo: Zstd ───\n" RESET);
    printf(DIM "       Usa BWT para reordenar datos antes de Huffman.\n" RESET);
    printf(DIM "       Mejora ratios agrupando símbolos similares — moderno y eficiente.\n" RESET);

    timer_start(&t);
    ZstdResult comp = zstd_compress(st->data, st->size, mode);
    timer_stop(&t);
    m.compress_time_us = timer_elapsed_us(&t);

    if (!comp.data) {
        printf(BRED "  Error en Zstd compresión.\n" RESET);
        return m;
    }
    m.compressed_bytes = comp.size;

    timer_start(&t);
    ZstdDecompResult decomp = zstd_decompress(comp.data,
                                              comp.size,
                                              comp.bit_count,
                                              comp.tree_root,
                                              comp.original_index,
                                              st->size,
                                              mode);
    timer_stop(&t);
    m.decompress_time_us = timer_elapsed_us(&t);

    int ok = (decomp.size == st->size &&
              memcmp(decomp.data, st->data, st->size) == 0);
    printf("  Integridad: %s\n", ok ? BGREEN "✔ OK" RESET : BRED "✘ FALLO" RESET);

    print_metrics(&m);
    write_file("samples/comprimidos/out_zstd.bin", comp.data, comp.size);
    write_file("samples/descomprimidos/out_zstd_decomp.bin", decomp.data, decomp.size);
    printf(DIM "  Archivos: samples/comprimidos/out_zstd.bin\n" RESET);
    printf(DIM "            samples/descomprimidos/out_zstd_decomp.bin\n" RESET);

    huff_free_tree(comp.tree_root);
    free(comp.data);
    free(decomp.data);
    return m;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  COMPARACIÓN DE TODOS LOS ALGORITMOS
 * ═══════════════════════════════════════════════════════════════════════════ */
static void comparar_todos(const AppState *st) {
    clear_screen();
    print_banner();
    printf(BOLD BWHITE "  ▶ COMPARACIÓN DE TODOS LOS ALGORITMOS\n\n" RESET);

    /* Ejecutar todos los algoritmos en modo silencioso para no contaminar la tabla */
    AppState silent = *st;
    silent.verbose = 0;

    Metrics metrics[9];
    metrics[0] = run_rle(&silent);
    metrics[1] = run_huffman(&silent);
    metrics[2] = run_lz77(&silent);
    metrics[3] = run_lz78(&silent);
    metrics[4] = run_lzw(&silent);
    metrics[5] = run_lz4(&silent);
    metrics[6] = run_deflate(&silent);
    metrics[7] = run_zstd(&silent);
    metrics[8] = run_bwt(&silent);

    printf("\n");
    PRINT_SEPARATOR();
    printf(BOLD BWHITE "  TABLA COMPARATIVA\n" RESET);
    print_comparison_table(metrics, 9);

    /* Determinar el ganador en ratio y tiempo */
    int   best_ratio_idx = 0, best_time_idx = 0;
    double best_ratio = -1e9, best_time = 1e18;

    for (int i = 0; i < 9; i++) {
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

static void comparar_lz(const AppState *st) {
    clear_screen();
    print_banner();
    printf(BOLD BWHITE "  ▶ COMPARACIÓN DE LOS LZ\n\n" RESET);

    AppState silent = *st;
    silent.verbose = 0;

    Metrics metrics[5];
    metrics[0] = run_lz77(&silent);
    metrics[1] = run_lz78(&silent);
    metrics[2] = run_lzw(&silent);
    metrics[3] = run_lz4(&silent);
    metrics[4] = run_zstd(&silent);

    printf("\n");
    PRINT_SEPARATOR();
    printf(BOLD BWHITE "  TABLA COMPARATIVA LZ\n" RESET);
    print_comparison_table(metrics, 5);

    int   best_ratio_idx = 0, best_time_idx = 0;
    double best_ratio = -1e9, best_time = 1e18;

    for (int i = 0; i < 5; i++) {
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
                    clear_screen(); print_banner();
                    Metrics m = run_lz78(&st);
                    (void)m;
                    printf(DIM "\n  ENTER para volver...\n" RESET);
                    while (getchar() != '\n');
                }
                break;

            case 6:
                if (!st.data) {
                    printf(BRED "  Primero carga un archivo (opción 1).\n" RESET);
                    printf(DIM "  ENTER...\n" RESET);
                    while (getchar() != '\n');
                } else {
                    clear_screen(); print_banner();
                    Metrics m = run_lzw(&st);
                    (void)m;
                    printf(DIM "\n  ENTER para volver...\n" RESET);
                    while (getchar() != '\n');
                }
                break;

            case 7:
                if (!st.data) {
                    printf(BRED "  Primero carga un archivo (opción 1).\n" RESET);
                    printf(DIM "  ENTER...\n" RESET);
                    while (getchar() != '\n');
                } else {
                    clear_screen(); print_banner();
                    Metrics m = run_lz4(&st);
                    (void)m;
                    printf(DIM "\n  ENTER para volver...\n" RESET);
                    while (getchar() != '\n');
                }
                break;

            case 8:
                if (!st.data) {
                    printf(BRED "  Primero carga un archivo (opción 1).\n" RESET);
                    printf(DIM "  ENTER...\n" RESET);
                    while (getchar() != '\n');
                } else {
                    clear_screen(); print_banner();
                    Metrics m = run_deflate(&st);
                    (void)m;
                    printf(DIM "\n  ENTER para volver...\n" RESET);
                    while (getchar() != '\n');
                }
                break;

            case 9:
                if (!st.data) {
                    printf(BRED "  Primero carga un archivo (opción 1).\n" RESET);
                    printf(DIM "  ENTER...\n" RESET);
                    while (getchar() != '\n');
                } else {
                    clear_screen(); print_banner();
                    Metrics m = run_zstd(&st);
                    (void)m;
                    printf(DIM "\n  ENTER para volver...\n" RESET);
                    while (getchar() != '\n');
                }
                break;

            case 10:
                if (!st.data) {
                    printf(BRED "  Primero carga un archivo (opción 1).\n" RESET);
                    printf(DIM "  ENTER...\n" RESET);
                    while (getchar() != '\n');
                } else {
                    clear_screen(); print_banner();
                    Metrics m = run_bwt(&st);
                    (void)m;
                    printf(DIM "\n  ENTER para volver...\n" RESET);
                    while (getchar() != '\n');
                }
                break;

            case 11:
                if (!st.data) {
                    printf(BRED "  Primero carga un archivo (opción 1).\n" RESET);
                    printf(DIM "  ENTER...\n" RESET);
                    while (getchar() != '\n');
                } else {
                    comparar_todos(&st);
                }
                break;

            case 12:
                if (!st.data) {
                    printf(BRED "  Primero carga un archivo (opción 1).\n" RESET);
                    printf(DIM "  ENTER...\n" RESET);
                    while (getchar() != '\n');
                } else {
                    comparar_lz(&st);
                }
                break;

            case 13:
                ver_estructuras();
                break;

            case 14:
                clear_screen(); print_banner();
                crear_archivo_prueba(&st);
                printf(DIM "\n  ENTER para continuar...\n" RESET);
                while (getchar() != '\n');
                break;

            case 15:
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
