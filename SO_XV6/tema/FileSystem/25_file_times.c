#include <stdio.h>
#include <sys/stat.h>
#include <time.h>    // Para ctime (traducción de fechas)
#include <stdlib.h>

/**
 * PROGRAMA 25: fechas_archivos.c (TIMESTAMPS)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO:
 * El Inodo rastrea 3 fechas importantes para cada archivo.
 * - atime (Access Time): Cuándo fue "leído" la última vez.
 * - mtime (Modification Time): Cuándo se cambió su "contenido".
 * - ctime (Status Change Time): Cuándo se cambiaron sus "metadatos" 
 *   (permisos, nombre, dueño, etc).
 * --------------------------------------------------------------------------
 */

int main(int argc, char *argv[]) {
    if (argc != 2) return EXIT_FAILURE;

    struct stat st;
    if (stat(argv[1], &st) == -1) return EXIT_FAILURE;

    printf("Analizando fechas del archivo: %s\n", argv[1]);

    // Las fechas están en segundos desde el 1 de Enero de 1970 (Unix Epoch).
    // Usamos ctime() para traducirlos a texto legible.
    printf("> Último ACCESO:       %s", ctime(&st.st_atime));
    printf("> Última MODIFICACIÓN: %s", ctime(&st.st_mtime));
    printf("> Último CAMBIO STATUS: %s", ctime(&st.st_ctime));

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (ESTILO DE APRENDIZAJE): 
 * Muchos sistemas de archivos (como ext4) se pueden montar con 'noatime'.
 * Eso desactiva la fecha de "Último ACCESO" para ganar velocidad,
 * ya que si no, cada lectura de un archivo implica una escritura extra 
 * en el Inodo ¡para decir cuándo se leyó!
 */
