#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * PROGRAMA 20: acceso_aleatorio.c (Random Access)
 * --------------------------------------------------------------------------
 * PROPÓSITO ACADÉMICO:
 * El acceso aleatorio es la base de las "Bases de Datos Relacionales".
 * No lees todo el disco (acceso secuencial), solo el registro que quieres.
 * 
 * En este programa simularemos leer un registro de datos fijos.
 * --------------------------------------------------------------------------
 */

struct Registro {
    int id;
    char nombre[16];
    float notaFinal;
};

int main() {
    FILE *archivo = fopen("base_datos_estudiantes.bin", "wb+");
    if (archivo == NULL) return EXIT_FAILURE;

    // 1. Preparamos 3 registros con datos de ejemplo
    struct Registro estudiantes[3] = {
        {101, "Edi", 9.5},
        {202, "Maria", 8.8},
        {303, "Juan", 7.2}
    };

    // 2. Los guardamos en disco en formato binario
    fwrite(estudiantes, sizeof(struct Registro), 3, archivo);
    printf("--- Base de datos creada con 3 estudiantes ---\n");

    // 3. QUEREMOS LEER SOLO AL SEGUNDO ESTUDIANTE (Maria) ¡SIN LEER AL PRIMERO!
    // Calculamos el salto: (Número de registro deseado - 1) * Tamaño de cada registro
    // En este caso: (2 - 1) * sizeof(struct Registro)
    long salto = (2 - 1) * sizeof(struct Registro);

    fseek(archivo, salto, SEEK_SET);

    struct Registro maria;
    fread(&maria, sizeof(struct Registro), 1, archivo);

    // 4. Mostramos el resultado
    printf("¡Se extrajo exitosamente el registro #2 de forma directa!\n");
    printf("> ID: %d, Nombre: %s, Nota: %.1f\n", maria.id, maria.nombre, maria.notaFinal);

    fclose(archivo);
    printf("\n--- Finalización del bloque 16-20 (Acceso Aleatorio) ---\n");
    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA: 
 * El acceso aleatorio funciona gracias a que los registros tienen un tamaño FIJO.
 * Si cada registro fuera de tamaño variable (como un archivo de texto con nombres largos),
 * fseek() no podría saber dónde empieza el segundo registro sin haber leído el primero.
 */
