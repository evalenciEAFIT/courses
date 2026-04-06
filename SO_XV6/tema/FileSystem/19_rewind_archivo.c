#include <stdio.h>
#include <stdlib.h>

/**
 * PROGRAMA 19: rebobinar_rewind.c
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO:
 * rewind() es una función de conveniencia sobre fseek().
 * 
 * Es equivalente a: fseek(puntero, 0L, SEEK_SET);
 * 
 * ¿Por qué usar rewind() en lugar de fseek?
 * - Porque aclara que quieres reiniciar el archivo completamente.
 * - Porque limpia el "error indicator" del archivo (si hubo problemas anteriores).
 * --------------------------------------------------------------------------
 */

int main() {
    FILE *archivo = fopen("historia.txt", "w+");
    if (archivo == NULL) return EXIT_FAILURE;

    // 1. Escribimos algo largo
    fputs("Érase una vez... hace mucho tiempo en un SO muy lejano...", archivo);
    
    // 2. Necesitamos leerlo ahora para verificarlo
    // Si intentamos fgetc() ahora, no leeremos nada porque estamos AL FINAL.
    printf("Posición actual antes de rebobinar: %ld (al final)\n", ftell(archivo));

    // 3. Volvemos al inicio (Rebobinar)
    rewind(archivo);
    
    // 4. Verificamos leyendo los primeros 10 caracteres
    char buffer[11] = {0};
    fgets(buffer, 11, archivo);
    printf("Primeros 10 caracteres tras rebobinar: [%s]\n", buffer);

    printf("Posición actual después de leer los 10 primeros: %ld\n", ftell(archivo));

    fclose(archivo);
    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA: 
 * Al leer de un archivo, el buffer interno del SO adelanta datos. 
 * rewind() hace que el sistema descarte esos datos del buffer y 
 * empiece de cero, asegurando una lectura limpia.
 */
