#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/**
 * PROYECTO 43: visor_hex_v2.c (VISTA HEX + ASCII)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO:
 * Un visor hexadecimal profesional muestra siempre dos columnas:
 * 1. Los bytes en Hex (para máquinas).
 * 2. Los caracteres imprimibles en Texto (para humanos).
 * --------------------------------------------------------------------------
 */

int main(int argc, char *argv[]) {
    if (argc != 2) return EXIT_FAILURE;

    FILE *archivo = fopen(argv[1], "rb");
    if (archivo == NULL) return EXIT_FAILURE;

    unsigned char buffer[16];
    size_t leidos;
    long offset = 0;

    printf("\n--- VISOR PRO: %s ---\n", argv[1]);

    while ((leidos = fread(buffer, 1, 16, archivo)) > 0) {
        // A. Imprimir Offset
        printf("%08lx | ", offset);

        // B. Imprimir Bytes en HEX
        for (int i = 0; i < 16; i++) {
            if (i < leidos) printf("%02x ", buffer[i]);
            else printf("   ");
            if (i == 7) printf(" "); // Separador visual medio
        }

        printf(" | ");

        // C. Imprimir Representación ASCII (Si el caracter es imprimible)
        for (int i = 0; i < leidos; i++) {
            // isprint() de ctype.h nos dice si el byte es un caracter visible
            if (isprint(buffer[i])) {
                printf("%c", buffer[i]);
            } else {
                printf("."); // Si es un byte de control o basura, mostramos un punto
            }
        }
        
        printf("\n");
        offset += 16;
    }

    fclose(archivo);
    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (LÓGICA DEL SISTEMA): 
 * El "punto" (.) es el estándar en visores hexadecimales. 
 * Caracteres como saltos de línea (\n) o tabuladores no son visibles,
 * por eso mostramos el punto para mantener la alineación visual.
 */
