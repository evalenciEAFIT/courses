#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#define BUFFER_SIZE 4096

// Función para copiar un archivo usando System Calls
void copy_file(const char *src, const char *dest) {
    int fd_src, fd_dest;
    ssize_t bytes_read, bytes_written;
    char buffer[BUFFER_SIZE];
    struct stat st;

    // Obtener los permisos del archivo original
    if (stat(src, &st) == -1) {
        perror("Error al obtener los stats del archivo de origen");
        return;
    }

    // System calls: open (lectura)
    fd_src = open(src, O_RDONLY);
    if (fd_src < 0) {
        perror("Error al abrir el archivo de origen");
        return;
    }

    // System calls: open (escritura, creación, truncado) manteniendo los permisos
    fd_dest = open(dest, O_WRONLY | O_CREAT | O_TRUNC, st.st_mode);
    if (fd_dest < 0) {
        perror("Error al crear/abrir el archivo de destino");
        close(fd_src);
        return;
    }

    // System calls: read y write en un ciclo
    while ((bytes_read = read(fd_src, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(fd_dest, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Error al escribir en el archivo de destino");
            break;
        }
    }

    if (bytes_read < 0) {
        perror("Error al leer el archivo de origen");
    } else {
        printf("[OK] Archivo respaldado: %s -> %s\n", src, dest);
    }

    // System calls: close
    close(fd_src);
    close(fd_dest);
}

// Función para copiar un directorio de forma recursiva
void copy_directory(const char *src, const char *dest) {
    struct stat st;
    
    // System calls: stat para obtener información
    if (stat(src, &st) == -1) {
        perror("Error al obtener stats del directorio de origen");
        return;
    }

    // System calls: mkdir para crear el directorio de backup
    if (mkdir(dest, st.st_mode) == -1) {
        if (errno != EEXIST) {
            perror("Error al crear el directorio de destino");
            return;
        }
    } else {
        printf("[OK] Directorio creado: %s\n", dest);
    }

    // System calls: opendir para listar el contenido (internamente usa open/getdents)
    DIR *dir = opendir(src);
    if (!dir) {
        perror("Error al abrir el directorio de origen");
        return;
    }

    struct dirent *entry;
    char next_src[1024];
    char next_dest[1024];

    // Recorrer el directorio
    while ((entry = readdir(dir)) != NULL) {
        // Ignorar "." y ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construir rutas completas
        snprintf(next_src, sizeof(next_src), "%s/%s", src, entry->d_name);
        snprintf(next_dest, sizeof(next_dest), "%s/%s", dest, entry->d_name);

        struct stat next_st;
        // System calls: lstat para detectar el tipo de archivo y evitar enlaces simbólicos que formen ciclos
        if (lstat(next_src, &next_st) == -1) {
            perror("Error al obtener los stats de un elemento");
            continue;
        }

        if (S_ISDIR(next_st.st_mode)) {
            // Es un directorio, procedemos recursivamente
            copy_directory(next_src, next_dest);
        } else if (S_ISREG(next_st.st_mode)) {
            // Es un archivo regular
            copy_file(next_src, next_dest);
        } else {
            // Otros como links simbólicos, character devices, etc. se ignoran en este script simple
            printf("[INFO] Elemento ignorado (especial o link): %s\n", next_src);
        }
    }

    closedir(dir);
}

void print_help(const char *prog_name) {
    printf("==========================================\n");
    printf("      SISTEMA DE BACKUP C - SysCalls      \n");
    printf("==========================================\n");
    printf("Uso: %s [OPCIÓN] [ORIGEN] [DESTINO]\n", prog_name);
    printf("\nPrueba de concepto de un sistema de copias de seguridad usando las\n");
    printf("llamadas al sistema de POSIX/Linux (open, read, write, close, mkdir, etc.).\n\n");
    printf("Opciones:\n");
    printf("  -h, --help    Muestra esta ayuda.\n");
    printf("  -b, --backup  Realizar respaldo un archivo o directorio recursivamente.\n");
    printf("\nEjemplos:\n");
    printf("  %s -b archivo.txt backup_archivo.txt\n", prog_name);
    printf("  %s -b /home/user/documentos /tmp/backup_documentos\n\n", prog_name);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help(argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_help(argv[0]);
        return EXIT_SUCCESS;
    }

    if (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "--backup") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Error: Faltan argumentos. Forma correcta: %s -b origen destino\n", argv[0]);
            return EXIT_FAILURE;
        }

        const char *src = argv[2];
        const char *dest = argv[3];

        struct stat st;
        // Revisar si el origen existe antes de intentar copiar nada
        if (stat(src, &st) == -1) {
            perror("Error comprobando el directorio/archivo de origen");
            return EXIT_FAILURE;
        }

        if (S_ISDIR(st.st_mode)) {
            printf("--- Iniciando respaldo del directorio '%s' en '%s' ---\n", src, dest);
            copy_directory(src, dest);
            printf("--- Respaldo completado ---\n");
        } else if (S_ISREG(st.st_mode)) {
            printf("--- Iniciando respaldo del archivo '%s' en '%s' ---\n", src, dest);
            copy_file(src, dest);
            printf("--- Respaldo completado ---\n");
        } else {
            fprintf(stderr, "Error: El origen no es válido. Debe ser carpeta o archivo.\n");
            return EXIT_FAILURE;
        }
    } else {
        fprintf(stderr, "Error: Opción no reconocida '%s'.\n", argv[1]);
        print_help(argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
