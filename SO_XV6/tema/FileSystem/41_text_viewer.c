#include <stdio.h>
#include <stdlib.h>

/**
 * PROYECTO 41: visor_texto.c (Clon de 'cat -n')
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO:
 * Combinar lectura de archivos por línea con formateo de salida.
 * Útil para tareas de depuración en archivos de código fuente.
 * --------------------------------------------------------------------------
 */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <archivo_a_visualizar>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *archivo = fopen(argv[1], "r");
    if (archivo == NULL) {
        perror("Fallo crítico al abrir el archivo de texto");
        return EXIT_FAILURE;
    }

    char buffer[1024];
    int numero_linea = 1;

    printf("--- Visualizando: %s ---\n", argv[1]);

    // Bucle de lectura eficiente linea a linea con fgets()
    while (fgets(buffer, sizeof(buffer), archivo)) {
        // Imprimimos el número de línea (justificado a 4 espacios para estética)
        // y el contenido de la línea
        printf("%4d | %s", numero_linea++, buffer);
    }

    printf("\n--- Fin del archivo ---\n");

    fclose(archivo);
    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (LÓGICA DEL SISTEMA): 
 * Si el archivo no termina en salto de línea (\n), printf("%s", buffer) 
 * no lo añadirá. Esto es fiel a cómo el comando cat original 
 * muestra los archivos en Linux.
 */
