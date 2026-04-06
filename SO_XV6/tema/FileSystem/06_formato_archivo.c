#include <stdio.h>
#include <stdlib.h>

/**
 * Programa 06: formato_archivo.c
 * Concepto: Escribir datos con formato estructurado usando `fprintf()`.
 */

int main() {
    FILE *archivo = fopen("alumnos.txt", "w");
    
    if (archivo == NULL) {
        perror("Error al crear alumnos.txt");
        return EXIT_FAILURE;
    }
    
    // Escribimos datos como en una tabla
    fprintf(archivo, "%-10s %-10s %-5s\n", "Nombre", "Apellido", "Edad");
    fprintf(archivo, "%-10s %-10s %-5d\n", "Edi", "Lopez", 23);
    fprintf(archivo, "%-10s %-10s %-5d\n", "Maria", "Sosa", 21);
    
    printf("Base de datos de alumnos creada exitosamente en 'alumnos.txt'.\n");
    
    fclose(archivo);
    
    return EXIT_SUCCESS;
}
