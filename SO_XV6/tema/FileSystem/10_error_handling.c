#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/**
 * Programa 10: error_handling.c
 * Concepto: Uso profundo de `errno`, `perror()` y `strerror()` para
 *           entender por qué fallan las operaciones de archivos.
 */

int main() {
    FILE *archivo = fopen("archivo_que_no_existe.txt", "r");
    
    if (archivo == NULL) {
        printf("--- Error detectado ---\n");
        
        // El número de error actual en el sistema
        printf("Número de error (errno): %d\n", errno);
        
        // Uso de strerror para obtener el mensaje asociado al número
        printf("Mensaje descriptivo (strerror): %s\n", strerror(errno));
        
        // Uso de perror para imprimir directamente al stderr
        perror("Mensaje directo con perror");
        
        printf("-----------------------\n");
        
        // Ejemplo de otro error: Permisos denegados (si intentamos abrir algo protegido)
        archivo = fopen("/root/root_file.txt", "r");
        if (archivo == NULL) {
            perror("Error al abrir archivo de root");
        }
    } else {
        fclose(archivo);
    }
    
    return EXIT_SUCCESS;
}
