/* ============================================================
 *  lzw.h  —  Algoritmo LZW (Lempel-Ziv-Welch)
 *
 *  CONCEPTO:
 *  LZW construye un diccionario extendiendo frases ya vistas, pero sin
 *  enviar el símbolo explícitamente junto al índice. Sólo emite códigos.
 *
 *  ESTRUCTURA DE SALIDA:
 *    - Token: { uint16_t code }
 *    - Diccionario inicial de 256 símbolos + nuevas frases
 *
 *  COMPLEJIDAD:
 *    Tiempo : O(n^2) para implementación ingenua
 *    Espacio: O(n)
 *
 *  USO PEDAGÓGICO:
 *    ✔ Codificación sin transmitir símbolos separados
 *    ✔ Base de GIF y muchos compresores clásicos
 * ============================================================ */

#ifndef LZW_H
#define LZW_H

#include <stdint.h>
#include <stdlib.h>

typedef enum { LZW_SILENT = 0, LZW_VERBOSE = 1 } LZWMode;

typedef struct {
    uint16_t code;
} LZWToken;

typedef struct {
    LZWToken *tokens;
    long      count;
    long      raw_size;
} LZWResult;

typedef struct {
    uint8_t *data;
    long     size;
} LZWDecompResult;

LZWResult      lzw_compress(const uint8_t *input, long in_size, LZWMode mode);
LZWDecompResult lzw_decompress(const LZWToken *tokens, long count,
                               long original_size, LZWMode mode);

void lzw_print_tokens(const LZWToken *tokens, long count, long max_show);

#endif /* LZW_H */
