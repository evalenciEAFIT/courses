#include <stdio.h>
#include <stdlib.h>

/**
 * Programa 01: hola_archivo.c
 * Concepto: Creación y cierre de un archivo básico usando streams de C.
 */

int main() {
    FILE *archivo;
    
    // Intentar abrir o crear un archivo llamado "prueba.txt" para escritura ("w")
    archivo = fopen("prueba.txt", "w");
    
    if (archivo == NULL) {
        perror("Error al crear el archivo");
        return EXIT_FAILURE;
    }
    
    printf("Archivo 'prueba.txt' creado exitosamente.\n");
    
    // Es CRUCIAL cerrar siempre los archivos para liberar recursos del SO
    fclose(archivo);
    
    return EXIT_SUCCESS;
}
