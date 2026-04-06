#include <stdio.h>
#include <dirent.h>  // LIBRERÍA DE DIRECTORIOS
#include <stdlib.h>

/**
 * PROGRAMA 34: listar_simple.c (Listado de archivos)
 * --------------------------------------------------------------------------
 * PROPÓSITO ACADÉMICO:
 * Entender cómo las aplicaciones "ven" el contenido de una carpeta.
 * 
 * Concepto de Flujo de Directorio (DIR):
 * Similar a FILE*, DIR es un puntero a una lista de registros (dirent).
 * Cada llamada a readdir() nos devuelve el REGISTRO del siguiente archivo.
 * --------------------------------------------------------------------------
 */

int main(int argc, char *argv[]) {
    // Si no pasamos nada, listamos el directorio actual "."
    const char *ruta = (argc > 1) ? argv[1] : ".";
    
    // 1. Abrir el flujo de directorio
    DIR *directorio = opendir(ruta);
    
    if (directorio == NULL) {
        perror("Error al abrir directorio");
        return EXIT_FAILURE;
    }

    // 2. Definir una estructura para el contenido
    struct dirent *entrada;

    printf("--- Listado simple para: %s ---\n", ruta);

    // 3. Bucle de lectura de archivos
    // readdir() devuelve NULL cuando ya no quedan más archivos que leer.
    while ((entrada = readdir(directorio)) != NULL) {
        // d_name: Nombre del archivo/carpeta
        // d_type: Tipo básico del archivo (DT_REG = regular, DT_DIR = directorio)
        printf("%-20s (Tipo FD: %d)\n", entrada->d_name, entrada->d_type);
    }
    
    // 4. Siempre debemos cerrar el flujo
    closedir(directorio);

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (LÓGICA DEL SISTEMA): 
 * Notarás que siempre aparecen '.' (el actual) y '..' (el padre).
 * Para un listado limpio, debes filtrar esos nombres con un IF (strcmp).
 */
