#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * PROGRAMA 37: serialización_estructuras.c (Guardar Bloques)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO FUNDAMENTAL:
 * No tienes que guardar dato por dato (nombre, luego edad, luego nota).
 * Puedes guardar la ESTRUCTURA COMPLETA del tirón desde la memoria al disco.
 * --------------------------------------------------------------------------
 */

struct Estudiante {
    int id;
    char nombre[50];
    float promedio;
};

int main() {
    FILE *archivo = fopen("estudiantes.dat", "wb");
    if (archivo == NULL) return EXIT_FAILURE;

    // 1. Crear una estructura con datos
    struct Estudiante e1 = {2026001, "Edi Lopez", 9.85};

    printf("--- Serializando Estudiante a estudiantes.dat ---\n");
    printf("> Estudiante: %s, ID: %d, Promedio: %.2f\n", e1.nombre, e1.id, e1.promedio);

    // 2. Guardar el BLOQUE COMPLETO DE MEMORIA
    // fwrite(puntero_a_memoria, tamaño_del_bloque, cuántos_bloques, archivo)
    // fwrite(puntero, sizeof(struct Estudiante), 1, archivo)
    size_t escritos = fwrite(&e1, sizeof(struct Estudiante), 1, archivo);

    if (escritos == 1) {
        printf("¡Guardado exitoso como bloque de bytes!\n");
    }

    fclose(archivo);
    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (LÓGICA DEL SISTEMA): 
 * Este método no guarda texto. Guarda los bits tal como existen en la memoria (RAM).
 * Si abres el archivo .dat con un block de notas (gedit/notepad), verás "basura" 
 * y tal vez el nombre legible. ¡Esto es lo que hace un archivo .exe o .png!
 */
