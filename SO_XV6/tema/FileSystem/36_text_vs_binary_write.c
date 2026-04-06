#include <stdio.h>
#include <stdlib.h>

/**
 * PROGRAMA 36: texto_vs_binario.c (DIFERENCIA DE ESPACIO)
 * --------------------------------------------------------------------------
 * CONCEPTO ACADÉMICO FUNDAMENTAL:
 * - Texto: Guarda caracteres ASCII (1 byte por cada dígito).
 * - Binario: Guarda los bytes reales del procesador (int = 4 bytes).
 * --------------------------------------------------------------------------
 */

int main() {
    int numero = 1234567890; // Un número grande
    
    // 1. Guardar como TEXTO (fprintf)
    // El número tiene 10 dígitos, ocupará 10 bytes de texto.
    FILE *ft = fopen("archivo_texto.txt", "w");
    fprintf(ft, "%d", numero);
    fclose(ft);

    // 2. Guardar como BINARIO (fwrite)
    // No importa cuán grande sea el número, un int SIEMPRE ocupa 4 bytes.
    FILE *fb = fopen("archivo_binario.bin", "wb");
    fwrite(&numero, sizeof(int), 1, fb);
    fclose(fb);

    printf("--- Experimento Finalizado (Texto vs Binario) ---\n");
    printf("Guardamos el número: %d\n", numero);
    
    printf("\nComprobación de ESPACIO (stat):\n");
    system("ls -l archivo_texto.txt archivo_binario.bin");

    return EXIT_SUCCESS;
}

/**
 * REFLEXIÓN ACADÉMICA (LÓGICA DEL SISTEMA): 
 * ¿Ves la diferencia? El archivo de texto usó 10 bytes para representar 
 * el número, mientras que el binario solo usó 4 bytes (los que usa el procesador).
 * ¡Por eso los juegos y bases de datos usan formatos binarios para ahorrar espacio!
 */
