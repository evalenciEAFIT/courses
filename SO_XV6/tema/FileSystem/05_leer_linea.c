#include <stdio.h>
#include <stdlib.h>

/**
 * Programa 05: leer_linea.c
 * Concepto: Leer archivos por líneas completas usando `fgets()`.
 *           Seguridad contra desbordamiento de búfer.
 */

int main() {
    FILE *archivo = fopen("notas.txt", "r");
    char buffer[100]; // Buffer para guardar cada línea temporalmente
    
    if (archivo == NULL) {
        printf("Error: Ejecuta primero '04_escribir_linea.c' para crear el archivo.\n");
        perror("Error de apertura");
        return EXIT_FAILURE;
    }
    
    printf("Contenido del archivo 'notas.txt' línea por línea:\n");

    /* fgets(buffer, tamaño_maximo, archivo)
       Lee hasta que encuentra un \n, llega a 99 caracteres, o llega al final */
    while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
        printf("> %s", buffer);
    }
    
    printf("\nFin de la lectura de líneas.");
    
    fclose(archivo);
    
    return EXIT_SUCCESS;
}
