#include <stdio.h>
#include <stdlib.h>

/**
 * PROGRAMA 40: endianness.c (Endianness)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO FUNDAMENTAL:
 * ¿Cómo se guardan los bytes en la memoria?
 * - Little Endian (Intel): Primero el byte menos significativo.
 * - Big Endian (Network): Primero el byte más significativo.
 * 
 * Este programa te mostrará el orden real de los bytes en tu sistema.
 * --------------------------------------------------------------------------
 */

int main() {
    // 1. Un entero de 4 bytes con valor hexadecimal fácil de seguir
    // 0x12 (mayor peso), 0x34, 0x56, 0x78 (menor peso)
    unsigned int n = 0x12345678;

    // 2. Apuntamos un puntero a char (1 byte) al inicio del entero (4 bytes)
    // Esto nos permite "ver" byte a byte lo que hay en memoria.
    unsigned char *p = (unsigned char *)&n;

    printf("--- Experimento de Endianness (Orden de Bytes en tu procesador) ---\n");
    printf("> Entero en decimal: %u\n", n);
    printf("> Entero en hexadecimal: 0x%08x\n", n);

    printf("\n>>> Los bytes guardados en la memoria son:\n");
    for (int i = 0; i < sizeof(int); i++) {
        printf("Posición [%d]: 0x%02x\n", i, p[i]);
    }

    if (p[0] == 0x78) {
        printf("\n¡¡TU SISTEMA ES LITTLE ENDIAN!! (Como la mayoría de PCs de hoy)\n");
        printf("Porque el byte menos significativo (0x78) se guardó PRIMERO.\n");
    } else {
        printf("\n¡¡TU SISTEMA ES BIG ENDIAN!! (Como los servidores de red / PowerPC antiguos)\n");
        printf("Porque el byte más significativo (0x12) se guardó PRIMERO.\n");
    }

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (LÓGICA DEL SISTEMA): 
 * Si guardas un archivo binario en tu PC (Little Endian) 
 * y lo mandas a un mainframe (Big Endian), los números 
 * se leerán completamente mal. ¡Esto se llama incompatibilidad de endianness!
 */
