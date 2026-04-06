#include <stdio.h>
#include <stdlib.h>

/**
 * Programa 07: analisis_formato.c
 * Concepto: Lectura de archivos con formato usando `fscanf()`.
 */

int main() {
    FILE *archivo = fopen("alumnos.txt", "r");
    char nombre[20], apellido[20];
    int edad;
    char cabecera_nombre[20], cabecera_apellido[20], cabecera_edad[20];

    if (archivo == NULL) {
        printf("Error: Ejecuta primero '06_formato_archivo.c' para crear el archivo.\n");
        perror("Error");
        return EXIT_FAILURE;
    }
    
    // Leemos la cabecera (ignorado)
    fscanf(archivo, "%s %s %s", cabecera_nombre, cabecera_apellido, cabecera_edad);
    printf("Cabecera del archivo: %s, %s, %s\n\n", cabecera_nombre, cabecera_apellido, cabecera_edad);

    printf("Datos extraídos de 'alumnos.txt':\n");
    
    // Leemos los registros uno por uno
    while (fscanf(archivo, "%s %s %d", nombre, apellido, &edad) == 3) {
        printf("> %s %s (Edad: %d)\n", nombre, apellido, edad);
    }
    
    printf("\nLectura con formato finalizada.");
    
    fclose(archivo);
    
    return EXIT_SUCCESS;
}
