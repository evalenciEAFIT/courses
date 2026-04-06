#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * PROGRAMA 38: deserialización_estructuras.c (Recuperar Bloques)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO FUNDAMENTAL:
 * Es el proceso INVERSO a la serialización. 
 * Leemos los bytes del disco directamente sobre el bloque de memoria 
 * que ocupa nuestra estructura en C.
 * --------------------------------------------------------------------------
 */

struct Estudiante {
    int id;
    char nombre[50];
    float promedio;
};

int main() {
    FILE *archivo = fopen("estudiantes.dat", "rb"); // rb: READ BINARY
    if (archivo == NULL) return EXIT_FAILURE;

    // 1. Declaramos una estructura VACÍA
    struct Estudiante e_recuperado;

    // 2. Cargamos el BLOQUE de disco a la MEMORIA
    // fread(destino_en_memoria, tamaño_del_bloque, cuántos, origen_archivo)
    size_t leidos = fread(&e_recuperado, sizeof(struct Estudiante), 1, archivo);

    if (leidos == 1) {
        printf("--- DE-Serializando Estudiante desde estudiantes.dat ---\n");
        printf("> Nombre recuperado: %s\n", e_recuperado.nombre);
        printf("> ID recuperado:     %d\n", e_recuperado.id);
        printf("> Promedio recuperado: %.2f\n", e_recuperado.promedio);
    } else {
        perror("Error al leer bloque");
    }

    fclose(archivo);
    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (LÓGICA DEL SISTEMA): 
 * Es vital que la definición de la estructura (struct Estudiante) 
 * sea exactamente igual tanto al guardar como al leer. 
 * Si añades un campo más, ¡los bytes leídos ya no coincidirán 
 * con la posición de las variables en la memoria!
 */
