#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

/**
 * PROGRAMA 00: launcher_v2.c (SIMPLIFICADO)
 * --------------------------------------------------------------------------
 * MEJORAS: 
 * - Detecta automáticamente archivos binarios (omite .c)
 * - Soporta números sin ceros a la izquierda (ej: '1' -> '01')
 * - Interfaz más limpia y robusta.
 * --------------------------------------------------------------------------
 */

void mostrar_menu() {
    DIR *d = opendir(".");
    struct dirent *dir;
    int count = 0;

    printf("\n\033[1;36m=============================================\033[0m\n");
    printf("\033[1;33m   MASTER LAUNCHER - FILE MANAGEMENT 2026    \033[0m\n");
    printf("\033[1;36m=============================================\033[0m\n");

    while ((dir = readdir(d)) != NULL) {
        // Un binario compilado por nuestro Makefile NO tiene extensión '.'
        // y debe empezar con un número.
        if (isdigit(dir->d_name[0]) && strchr(dir->d_name, '.') == NULL) {
            printf(" \033[1;32m%-25s\033[0m", dir->d_name);
            if (++count % 2 == 0) printf("\n");
        }
    }
    closedir(d);
    printf("\n\033[1;36m---------------------------------------------\033[0m\n");
}

int main() {
    char input[50];
    char args[256];
    char cmd[512];

    while (1) {
        mostrar_menu();
        printf("\n\033[1mSelecciona un programa (ej: 1, 01, 45) o 'q' para salir: \033[0m");
        if (scanf("%s", input) == 0 || strcmp(input, "q") == 0) break;
        getchar(); // Limpieza

        // Convertir entrada a número para búsqueda flexible
        int num_buscado = atoi(input);
        
        DIR *d = opendir(".");
        struct dirent *dir;
        char programa_encontrado[128] = "";

        while ((dir = readdir(d)) != NULL) {
            // Solo miramos binarios (sin extensión)
            if (isdigit(dir->d_name[0]) && strchr(dir->d_name, '.') == NULL) {
                int num_archivo = atoi(dir->d_name);
                if (num_archivo == num_buscado) {
                    strcpy(programa_encontrado, dir->d_name);
                    break;
                }
            }
        }
        closedir(d);

        if (strlen(programa_encontrado) == 0) {
            printf("\n\033[1;31m[!] No se encontró el programa #%d\033[0m\n", num_buscado);
            continue;
        }

        // Simplificación de argumentos: preguntar en la misma línea
        printf("\033[1;34m[?] Argumentos para %s (Enter si ninguno): \033[0m", programa_encontrado);
        fgets(args, sizeof(args), stdin);
        args[strcspn(args, "\n")] = 0;

        snprintf(cmd, sizeof(cmd), "./%s %s", programa_encontrado, args);
        
        printf("\n\033[1;33m>>> EJECUTANDO: %s\033[0m\n", cmd);
        printf("---------------------------------------------\n");
        system(cmd);
        printf("\033[1;36m---------------------------------------------\033[0m\n");
        printf("Pulsa Enter para volver...");
        getchar();
    }

    printf("\nAdiós.\n");
    return 0;
}
