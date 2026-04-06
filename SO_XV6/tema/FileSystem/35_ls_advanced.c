#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

/**
 * PROGRAMA 35: listar_avanzado.c (Un clon de ls -l)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO FUNDAMENTAL:
 * Combinar readdir() con stat() es como las aplicaciones de 
 * gestión de archivos (Nautilus, Explorer) obtienen todos los datos.
 * --------------------------------------------------------------------------
 */

int main(int argc, char *argv[]) {
    const char *ruta = (argc > 1) ? argv[1] : ".";
    DIR *dir = opendir(ruta);
    struct dirent *entrada;
    struct stat info;
    char ruta_archivo[512];

    if (dir == NULL) {
        perror("Fallo al abrir directorio");
        return EXIT_FAILURE;
    }

    printf("--- Listado Detallado para: %s ---\n", ruta);
    printf("%-20s %-12s %-15s\n", "Nombre", "Tamaño (B)", "Últ. Modif.");
    
    // 1. Empezamos el bucle por todos los archivos
    while ((entrada = readdir(dir)) != NULL) {
        // Ignoramos '.' y '..' para un listado más limpio
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) continue;

        // 2. IMPORTANTE: Necesitamos la ruta COMPLETA del archivo para llamar a stat().
        // readdir() solo nos da el nombre relativo ("archivo.txt"). 
        snprintf(ruta_archivo, sizeof(ruta_archivo), "%s/%s", ruta, entrada->d_name);

        // 3. Obtener metadatos con stat()
        if (stat(ruta_archivo, &info) == -1) continue;

        // 4. Traducir la fecha (ctime devuelve un string con salto \n al final, lo limpiaremos un poco)
        char *fecha = ctime(&info.st_mtime);
        fecha[strlen(fecha) - 1] = '\0'; // Quitar el \n del final

        // 5. Imprimir toda la información en formato de tabla
        printf("%-20s %-12ld %-15s\n", entrada->d_name, (long)info.st_size, fecha);
    }

    closedir(dir);
    printf("\n--- Finalización bloque Directorios (31-35) ---");

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (LÓGICA DEL SISTEMA): 
 * Este programa es pesado. Llamar a stat() miles de veces puede ralentizar una red.
 * Por eso cuando una carpeta tiene miles de archivos, los exploradores 
 * de archivos pueden tardar un poco en mostrar la información.
 */
