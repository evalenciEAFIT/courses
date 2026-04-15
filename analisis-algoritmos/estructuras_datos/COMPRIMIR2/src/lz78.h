/* ============================================================
 *  lz78.h  —  Algoritmo LZ78 (Lempel-Ziv 1978)
 *
 *  CONCEPTO:
 *  LZ78 construye un diccionario de frases durante la búsqueda de
 *  coincidencias en el flujo de entrada. Emite pares (índice, símbolo), donde
 *  el índice apunta a una frase previa y el símbolo extiende esa frase.
 *
 *  ESTRUCTURAS DE SALIDA:
 *    - Token: { uint16_t index, uint8_t next_byte }
 *    - Diccionario dinámico de frases
 *
 *  COMPLEJIDAD:
 *    Tiempo : O(n^2) en implementación ingenua
 *    Espacio: O(n)  para el diccionario y las frases
 *
 *  USO PEDAGÓGICO:
 *    ✔ Mejor en datos con frases repetidas no consecutivas
 *    ✘ No es tan compacto como LZW en algunos casos
 * ============================================================ */

#ifndef LZ78_H
#define LZ78_H

#include <stdint.h>
#include <stdlib.h>

typedef enum { LZ78_SILENT = 0, LZ78_VERBOSE = 1 } LZ78Mode;

typedef struct {
    uint16_t index;    /* Índice de la frase previa (0 = vacío) */
    uint8_t  next_byte;/* Símbolo que extiende la frase */
} LZ78Token;

typedef struct {
    LZ78Token *tokens;
    long       count;
    long       raw_size;
} LZ78Result;

typedef struct {
    uint8_t *data;
    long     size;
} LZ78DecompResult;

LZ78Result      lz78_compress(const uint8_t *input, long in_size, LZ78Mode mode);
LZ78DecompResult lz78_decompress(const LZ78Token *tokens, long count,
                                 long original_size, LZ78Mode mode);

void lz78_print_tokens(const LZ78Token *tokens, long count, long max_show);

#endif /* LZ78_H */
