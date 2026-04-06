#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * PROGRAMA 39: actualización_binaria.c (Modificar en el sitio)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO FUNDAMENTAL:
 * En un archivo de TEXTO, no puedes cambiar "Ana" por "Alejandro" 
 * sin mover todo lo demás en el archivo (sobrescribirías datos).
 * 
 * En archivos BINARIOS de registros fijos, como todos miden lo mismo,
 * podemos saltar a una posición exacta y cambiar solo ese registro.
 * --------------------------------------------------------------------------
 */

struct Estudiante {
    int id;
    char nombre[50];
    float promedio;
};

int main() {
    // 1. Abrimos para lectura/escritura binaria SIN borrar contenido (r+b)
    FILE *archivo = fopen("estudiantes.dat", "r+b");
    if (archivo == NULL) {
        perror("Fallo al abrir archivo binario existente");
        return EXIT_FAILURE;
    }

    // 2. Queremos actualizar la nota del primer estudiante
    // fseek(archivo_puntero, offset_bytes, SEEK_SET)
    // Nos saltamos el primero si hay más de uno. Si queremos el primero, nos saltamos 0.
    fseek(archivo, 0, SEEK_SET);

    struct Estudiante e_nuevo;
    // Leemos el registro existente para tener sus datos
    fread(&e_nuevo, sizeof(struct Estudiante), 1, archivo);

    // 3. Modificamos solo el promedio
    e_nuevo.promedio = 10.0; // ¡Le ponemos nota perfecta!

    // 4. SOBRESCRIBIR: Debemos volver a saltar al inicio del registro 
    // pues la lectura de fread() movió nuestro cabezal hacia adelante.
    fseek(archivo, 0, SEEK_SET);
    fwrite(&e_nuevo, sizeof(struct Estudiante), 1, archivo);

    printf("¡Promedio actualizado exitosamente a 10.0!\n");
    printf("> Estudiante: %s, ID: %d, NUEVO Promedio: %.2f\n", e_nuevo.nombre, e_nuevo.id, e_nuevo.promedio);

    fclose(archivo);
    printf("\n--- Finalización del bloque Actualización Binaria (39) ---");

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (ESTILO SO): 
 * Este método es la base de cómo funcionan las 
 * Bases de Datos (como SQLite). Actualizar un dato es un 
 * salto de puntero y una escritura pequeña. Muy eficiente.
 */
