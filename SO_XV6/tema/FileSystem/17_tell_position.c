#include <stdio.h>
#include <stdlib.h>

/**
 * PROGRAMA 17: saber_posicion.c (ftell)
 * --------------------------------------------------------------------------
 * PROPÓSITO ACADÉMICO:
 * Entender cómo el Sistema Operativo rastrea el "indicador de posición".
 * Cada vez que lees o escribes, el SO actualiza este número automáticamente.
 * 
 * ¿Para qué sirve ftell()?
 * - Para guardar un punto de retorno (checkpoint).
 * - Para calcular el tamaño de un archivo en bytes.
 * --------------------------------------------------------------------------
 */

int main() {
    FILE *archivo = fopen("ejemplo_tell.txt", "w+");
    if (archivo == NULL) return EXIT_FAILURE;

    // 1. Escribimos un texto corto
    fputs("Sistemas Operativos 2026", archivo);
    
    // 2. ¿En qué posición estamos después de escribir?
    // ftell() nos devolverá el número de bytes desde el inicio.
    long posicionActual = ftell(archivo);
    printf("Posición actual después de escribir: %ld bytes.\n", posicionActual);

    // 3. RETO COMPLEJO: Calcular el tamaño del archivo usando saltos.
    // Paso A: Saltar al final
    fseek(archivo, 0, SEEK_END);
    
    // Paso B: Preguntar dónde está el cabezal (el byte del final es el tamaño total)
    long tamanoArchivo = ftell(archivo);
    printf("¡Tamaño total calculado con saltos!: %ld bytes.\n", tamanoArchivo);

    fclose(archivo);
    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA:
 * El SO no guarda el "tamaño" como una variable que se actualiza siempre.
 * Internamente, el sistema de archivos (ext4, NTFS) tiene metadatos,
 * pero usar fseek al final y ftell es la forma estándar e infalible de saberlo en C.
 */
