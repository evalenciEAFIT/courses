#include <stdio.h>
#include <stdlib.h>

/**
 * PROYECTO 42: visor_hex_v1.c (VISOR BINARIO)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO FUNDAMENTAL:
 * Los editores hexadecimales te muestran en realidad la MEMORIA 
 * que ocupa cada byte del archivo. 
 * Mostraremos el "OFFSET" (posición del byte desde el inicio).
 * --------------------------------------------------------------------------
 */

int main(int argc, char *argv[]) {
    if (argc != 2) return EXIT_FAILURE;

    FILE *archivo = fopen(argv[1], "rb");
    if (archivo == NULL) return EXIT_FAILURE;

    unsigned char buffer[16]; // Leemos en grupos de 16 bytes
    long offset = 0;
    size_t leidos;

    printf("\n--- VISOR HEXADECIMAL (Básico) para: %s ---\n", argv[1]);
    printf("Offset      Bytes en Hexadecimal\n");
    printf("-----------------------------------------\n");

    // Leemos 16 bytes en cada vuelta del bucle
    while ((leidos = fread(buffer, 1, 16, archivo)) > 0) {
        // 1. Mostrar la dirección actual (offset) en hexadecimal con 8 dígitos
        printf("%08lx: ", offset);

        // 2. Mostrar los bytes leídos en este bloque
        for (int i = 0; i < 16; i++) {
            if (i < leidos) {
                // %02x -> Imprime el byte en hex con 2 dígitos, añadiendo un 0 al inicio si es necesario
                printf("%02x ", buffer[i]);
            } else {
                // Espacios vacíos si el archivo termina antes de llenar los 16
                printf("   ");
            }
        }
        
        printf("\n");
        offset += 16;
    }

    fclose(archivo);
    printf("\n--- Finalización del bloque Visor Hex v1 (42) ---");

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (ESTILO SO): 
 * Usamos 'unsigned char' porque el rango (0-255) es perfecto para 
 * representar cada byte ASCII Extendido. 
 * El offset %08lx nos da las direcciones de memoria similares 
 * a las de una herramienta como gdb.
 */
