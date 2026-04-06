#include <stdio.h>
#include <sys/stat.h>
#include <pwd.h>   // Para obtener el nombre de usuario (getpwuid)
#include <grp.h>   // Para obtener el nombre de grupo (getgrgid)
#include <stdlib.h>

/**
 * PROGRAMA 24: dueño_archivo.c (IDENTIFICACIÓN de DUEÑO)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO:
 * El Inodo no guarda nombres ("edi"), guarda números de identificación.
 * st_uid (User ID) y st_gid (Group ID).
 * 
 * En este programa traducimos esos IDs a nombres reales leyendo la
 * base de datos de usuarios del sistema (/etc/passwd y /etc/group).
 * --------------------------------------------------------------------------
 */

int main(int argc, char *argv[]) {
    if (argc != 2) return EXIT_FAILURE;

    struct stat st;
    if (stat(argv[1], &st) == -1) return EXIT_FAILURE;

    // Obtener información del dueño (UID)
    struct passwd *user_info = getpwuid(st.st_uid);
    // Obtener información del grupo (GID)
    struct group *group_info = getgrgid(st.st_gid);

    printf("Analizando dueño de: %s\n", argv[1]);

    if (user_info != NULL) {
        printf("> Usuario (UID %d): %s\n", st.st_uid, user_info->pw_name);
    } else {
        printf("> UID %d desconocido en el sistema.\n", st.st_uid);
    }

    if (group_info != NULL) {
        printf("> Grupo (GID %d): %s\n", st.st_gid, group_info->gr_name);
    } else {
        printf("> GID %d desconocido en el sistema.\n", st.st_gid);
    }

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (DETRÁS DE ESCENA): 
 * Si mueves un disco de Linux a otra computadora, el ID 1000 podría llamarse 
 * "edi" en una y "maria" en otra. ¡El nombre depende de la configuración 
 * del sistema operativo, el Inodo solo guarda el número!
 */
