#include <stdio.h>
#include <stdlib.h>

/**
 * PROYECTO 44: editor_binario.c (PARCHEO DE BYTES)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO FUNDAMENTAL:
 * Los editores hexadecimales reales permiten modificar UN BYTE 
 * en una dirección (Offset) exacta del disco. 
 * ¡Esto es como parchar un ejecutable (.exe)!
 * --------------------------------------------------------------------------
 */

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <archivo> <offset_hex> <nuevo_byte_hex>\n", argv[0]);
        printf("Ejemplo: %s datos.bin 10 a3\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *nombre = argv[1];
    // strtol convierte un string hexadecimal (base 16) a un número long
    long offset = strtol(argv[2], NULL, 16); 
    // Capturamos el nuevo byte que queremos poner
    unsigned char nuevo_byte = (unsigned char)strtol(argv[3], NULL, 16);

    // 1. Abrir para lectura/escritura binaria
    FILE *archivo = fopen(nombre, "r+b");
    if (archivo == NULL) {
        perror("Fallo al abrir archivo para editar");
        return EXIT_FAILURE;
    }

    // 2. Saltar a la dirección exacta que el usuario pidió
    if (fseek(archivo, offset, SEEK_SET) == -1) {
        perror("Error de posicionamiento (¿El offset es mayor que el archivo?)");
        fclose(archivo);
        return EXIT_FAILURE;
    }

    // 3. SOBRESCRIBIR EL BYTE
    // fwrite(puntero_a_dato, tamaño_dato, cantidad, archivo)
    if (fwrite(&nuevo_byte, 1, 1, archivo) == 1) {
        printf("¡Parche aplicado con éxito!\n");
        printf("Archivo: %s | Offset: 0x%lx | Nuevo Byte: 0x%02x\n", nombre, offset, nuevo_byte);
    } else {
        perror("Error de escritura en disco");
    }

    fclose(archivo);

    printf("\n--- Finalización del bloque Editor Binario (44) ---");

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (LÓGICA DEL SISTEMA): 
 * Modificar un solo byte no altera el tamaño del archivo en el Inodo. 
 * Simplemente cambia los bits en el sector físico del disco duro. 
 * ¡Es la forma más pura de manipulación de archivos!
 */
