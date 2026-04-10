/* ============================================================
 *  lz.h  —  Algoritmo LZ77 (Lempel-Ziv 1977)
 *
 *  CONCEPTO:
 *  LZ77 es la base de casi todos los compresores modernos (ZIP, gzip,
 *  PNG, zlib). Usa un mecanismo de "ventana deslizante": cuando encuentra
 *  una cadena que ya apareció antes, la reemplaza con una referencia
 *  (distancia, longitud) al lugar donde está en el historial.
 *
 *  MECÁNICA:
 *    ┌───────────────────────┬───────────────────┐
 *    │   VENTANA (historial) │  LOOKAHEAD BUFFER  │
 *    │   (bytes anteriores)  │  (siguiente a cod.)│
 *    └───────────────────────┴───────────────────┘
 *                            ↑ posición actual
 *
 *    Para cada posición, busca el match más largo en la ventana.
 *    Emite un TOKEN que puede ser:
 *      - Literal   : (0, 0, byte)      → byte que no encontró en ventana
 *      - Referencia: (dist, len, byte) → "retrocede 'dist', copia 'len' bytes,
 *                                         luego emite 'byte'"
 *
 *  ESTRUCTURA DE DATOS:
 *    LZ77Token — token empaquetado: { uint16_t dist, uint8_t len, uint8_t next }
 *    El flujo de salida es un arreglo de estos tokens.
 *
 *  PARÁMETROS CONFIGURABLES:
 *    WINDOW_SIZE   — tamaño de la ventana de búsqueda (cuánto historial)
 *    LOOKAHEAD_MAX — longitud máxima de match buscado
 *
 *  COMPLEJIDAD:
 *    Tiempo : O(n * WINDOW_SIZE)  ingénua; O(n log n) con hash tables
 *    Espacio: O(WINDOW_SIZE)
 *
 *  CUÁNDO USARLO:
 *    ✔ Archivos con secuencias repetidas no consecutivas (texto, código)
 *    ✔ Como base de gzip, PNG, zlib, DEFLATE
 *    ✘ Archivos ya comprimidos (JPEG, MP3)
 * ============================================================ */

#ifndef LZ_H
#define LZ_H

#include <stdint.h>
#include <stdlib.h>

/* ───── Parámetros de la ventana deslizante ────────────────────────────── */
#define LZ_WINDOW_SIZE   4096   /* Tamaño máximo del historial en bytes   */
#define LZ_LOOKAHEAD_MAX  255   /* Longitud máxima de un match            */
#define LZ_MIN_MATCH        3   /* Longitud mínima para emitir referencia */

typedef enum { LZ_SILENT = 0, LZ_VERBOSE = 1 } LZMode;

/* ───── Token LZ77 ─────────────────────────────────────────────────────── */
/* Empaquetado en 4 bytes:
 *   [dist 16 bits] [len 8 bits] [next_byte 8 bits]
 * Si dist == 0 && len == 0 → literal (solo next_byte importa).
 */
typedef struct {
    uint16_t dist;       /* Distancia al inicio del match en el historial */
    uint8_t  length;     /* Longitud del match                            */
    uint8_t  next_byte;  /* Byte siguiente al match (o literal si dist=0) */
} LZ77Token;

/* ───── Resultado de la compresión ─────────────────────────────────────── */
typedef struct {
    LZ77Token *tokens;   /* Secuencia de tokens (liberar con free)        */
    long       count;    /* Número de tokens                              */
    long       raw_size; /* Tamaño en bytes de los tokens empaquetados    */
} LZResult;

typedef struct {
    uint8_t *data;
    long     size;
} LZDecompResult;

/* ───── API pública ─────────────────────────────────────────────────────── */

LZResult       lz_compress  (const uint8_t *input, long in_size, LZMode mode);
LZDecompResult lz_decompress(const LZ77Token *tokens, long count,
                              long original_size, LZMode mode);

/* Muestra la lista de tokens con interpretación pedagógica */
void lz_print_tokens(const LZ77Token *tokens, long count, long max_show);

/* Visualiza la ventana deslizante en un momento dado */
void lz_print_window(const uint8_t *buf, long win_start, long win_size,
                     long pos, long match_dist, long match_len);

#endif /* LZ_H */
