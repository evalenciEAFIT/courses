/* ============================================================
 *  rle.h  —  Algoritmo Run-Length Encoding (RLE)
 *
 *  CONCEPTO:
 *  RLE es el algoritmo de compresión más simple. Reemplaza
 *  secuencias de caracteres repetidos por un par (contador, carácter).
 *
 *  Ejemplo:
 *    Original : AAABBBBBCCDDDDDD  (16 bytes)
 *    Codificado: 3A 5B 2C 6D       (8 bytes)  → 50% de compresión
 *
 *  ESTRUCTURA DE DATOS:
 *    - Buffer de entrada: arreglo de uint8_t
 *    - Buffer de salida : arreglo de pares { uint8_t count, uint8_t byte }
 *    - No se usan estructuras auxiliares (in-place con dos punteros)
 *
 *  COMPLEJIDAD:
 *    Tiempo : O(n)   — un pase sobre los datos
 *    Espacio: O(n)   — peor caso (sin repeticiones) dobla el tamaño
 *
 *  CUÁNDO USARLO:
 *    ✔ Imágenes con grandes áreas de color uniforme (BMP, PCX antiguo)
 *    ✔ Datos de facsímil (fax, TIFF bilevel)
 *    ✘ Texto natural (muy pocas repeticiones)
 * ============================================================ */

#ifndef RLE_H
#define RLE_H

#include <stdint.h>
#include <stdlib.h>

/* Modo de operación */
typedef enum { RLE_SILENT = 0, RLE_VERBOSE = 1 } RLEMode;

/* Resultado de una operación RLE */
typedef struct {
    uint8_t *data;       /* Buffer con el resultado (liberar con free) */
    long     size;       /* Tamaño del buffer resultado                */
} RLEResult;

/*
 * rle_compress
 * Comprime 'input' de 'in_size' bytes usando RLE.
 * Si mode == RLE_VERBOSE imprime el paso a paso.
 * Devuelve un RLEResult (data == NULL si error).
 */
RLEResult rle_compress(const uint8_t *input, long in_size, RLEMode mode);

/*
 * rle_decompress
 * Descomprime datos RLE.
 * Si mode == RLE_VERBOSE imprime el paso a paso.
 */
RLEResult rle_decompress(const uint8_t *input, long in_size, RLEMode mode);

/* Muestra la estructura interna del formato RLE codificado */
void rle_print_structure(const uint8_t *compressed, long size);

#endif /* RLE_H */
