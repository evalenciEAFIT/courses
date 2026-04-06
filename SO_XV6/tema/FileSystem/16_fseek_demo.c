#include <stdio.h>
#include <stdlib.h>

/**
 * PROGRAMA 16: fseek_demo.c
 * --------------------------------------------------------------------------
 * CONCEPTO PROFUNDO: 
 * Imagina que un archivo es una cinta. No tienes que leerla de principio a fin siempre.
 * Con `fseek()`, puedes mover el cabezal de lectura de forma instantánea.
 * 
 * Modos de fseek():
 * - SEEK_SET: Empezar desde el byte 0 (inicio).
 * - SEEK_CUR: Moverse relativo a la posición actual del cabezal.
 * - SEEK_END: Moverse relativo al final del archivo (útil para saber tamaño).
 * --------------------------------------------------------------------------
 */

int main() {
    FILE *archivo = fopen("datos_posicion.txt", "w+"); // w+: Abrir para lectura/escritura (y borrar contenido si existe)
    
    if (archivo == NULL) {
        perror("Error al abrir");
        return EXIT_FAILURE;
    }

    // 1. Escribimos algo de prueba
    fputs("ABCDEFGHIJ", archivo); // Escribimos 10 caracteres (0 a 9)
    printf("--- Texto de prueba: 'ABCDEFGHIJ' ---\n");

    // 2. Saltar a la posición 3 (la 'D')
    // Desplazamiento de 3 bytes desde el INICIO (SEEK_SET)
    fseek(archivo, 3, SEEK_SET);
    int c = fgetc(archivo);
    printf("Salto a posición 3 (SEEK_SET): Debiera ser 'D'. Leído: %c\n", c);

    // 3. Saltar 2 posiciones HACIA ATRÁS desde donde estamos (SEEK_CUR)
    // Estamos en la posición 4 (después de leer 'D'). 
    // Retrocedemos 2 (4 - 2 = 2), caemos en la posición 2 ('C').
    fseek(archivo, -2, SEEK_CUR);
    c = fgetc(archivo);
    printf("Salto -2 relativo (SEEK_CUR): Debiera ser 'C'. Leído: %c\n", c);

    // 4. Saltar a la ÚLTIMA posición (el final) y retroceder 1
    // SEEK_END nos pone después de la 'J' (posición 10).
    // Retrocedemos 1 byte para caer en la 'J'.
    fseek(archivo, -1, SEEK_END);
    c = fgetc(archivo);
    printf("Salto -1 desde el final (SEEK_END): Debiera ser 'J'. Leído: %c\n", c);

    fclose(archivo);
    return EXIT_SUCCESS;
}
/* 
 * NOTA TÉCNICA: 
 * En archivos de texto antiguos de Windows (\r\n), fseek puede ser impredecible.
 * En Linux y archivos binarios, es exacto al byte.
 */
