#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * PROYECTO 45: editor_texto_minimalista.c (TUI)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO:
 * Un editor de texto es una aplicación que lee de la entrada estándar (STDIN)
 * y lo guarda de forma secuencial en un archivo. 
 * Finalizaremos el curso con esta herramienta básica pero completa.
 * --------------------------------------------------------------------------
 */

int main() {
    char nombre_archivo[100];
    char linea[1024];

    printf("--- EDI-TEXT EDITOR v1.0 ---\n");
    printf("Escribe el nombre del nuevo archivo para crear: ");
    
    // 1. Capturar el nombre del archivo del usuario
    if (scanf("%s", nombre_archivo) == 0) return EXIT_FAILURE;
    getchar(); // Consumir el salto de línea pendiente

    FILE *archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL) {
        perror("Fallo al crear archivo");
        return EXIT_FAILURE;
    }

    printf("\n--- Escribe tu texto (Escribe ':wq' en una línea sola para guardar y salir) ---\n\n");

    // 2. Bucle infinito de captura de texto
    while (1) {
        // Obtenemos una línea de texto del usuario
        if (fgets(linea, sizeof(linea), stdin) == NULL) break;

        // Comand de escape (Estilo VI)
        if (strcmp(linea, ":wq\n") == 0) {
            printf("\n--- Guardando y saliendo... ---\n");
            break;
        }

        // 3. Escribir la línea capturada en el archivo físico
        fputs(linea, archivo);
    }

    fclose(archivo);
    printf("¡Archivo guardado exitosamente: %s!\n", nombre_archivo);
    printf("--- FIN DEL CURSO DE FILE MANAGEMENT 2026 ---");

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (LÓGICA DEL SISTEMA): 
 * Un editor real (como nano o vi) usaría buffers dinámicos 
 * (malloc) para permitir editar en medio de una página. 
 * Este editor es "lineal" (solo añade al final), lo cual es 
 * la forma más simple de persistencia.
 */
