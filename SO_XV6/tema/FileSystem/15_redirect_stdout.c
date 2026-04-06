#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/**
 * PROGRAMA 15: redireccionar_stdout.c
 * --------------------------------------------------------------------------
 * CONCEPTO PROFUNDO: 
 * En Linux, TODO es un archivo. La pantalla (tu terminal) es en realidad
 * el File Descriptor 1 (STDOUT_FILENO).
 * 
 * La llamada `dup2(nuevo_fd, viejo_fd)` es mágica. Permite que el 'viejo_fd'
 * ahora sea una copia exacta del 'nuevo_fd'.
 * Si reemplazamos el descriptor 1 (la pantalla) por el descriptor de un 
 * archivo físico, todos los `printf()` se irán automáticamente al archivo 
 * ¡sin cambiar ni una sola línea de código de impresión!
 * --------------------------------------------------------------------------
 */

int main() {
    // 1. Abrimos un archivo para guardar todo lo que imprimiría el programa.
    const char *nombre_archivo = "bitacora.txt";
    int fd_archivo = open(nombre_archivo, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    if (fd_archivo == -1) {
        perror("Error al abrir bitacora.txt");
        return EXIT_FAILURE;
    }

    printf("--- Iniciando redirección ---\n");
    printf("Este mensaje debería verse en la pantalla (todavía).\n");
    
    // 2. Usamos dup2 para "redirigir"
    // Reemplazamos STDOUT_FILENO (fd número 1) por nuestro fd_archivo.
    // A partir de aquí, printf() usará el fd_archivo internamente.
    if (dup2(fd_archivo, STDOUT_FILENO) == -1) {
        perror("Error en la redirección dup2");
        close(fd_archivo);
        return EXIT_FAILURE;
    }

    // El descriptor de archivo ya no es necesario tenerlo "extra" (como número 3)
    // porque ahora el número 1 ya apunta al archivo. Lo cerramos.
    close(fd_archivo);

    // 3. Probamos varios métodos de salida estándar.
    // Todos se irán al archivo ahora.
    printf("¡Hola! Este mensaje NO aparece en pantalla.\n");
    printf("Se ha guardado directamente dentro de 'bitacora.txt'.\n");
    puts("Incluso puts() se redirige automáticamente.");
    
    // 4. Escribir algo importante que confirme el éxito
    const char *exito = "La redirección de STDOUT funcionó perfectamente.\n";
    write(1, exito, strlen(exito)); // Usamos descriptor 1 directamente

    return EXIT_SUCCESS;
}
/*
 * PREGUNTA PARA REFLEXIONAR:
 * ¿Qué pasaría si usasemos dup2 con el File Descriptor 0 (STDIN_FILENO)?
 * Respuesta: Estarías leyendo de un archivo en lugar de recibir teclas del usuario.
 */
