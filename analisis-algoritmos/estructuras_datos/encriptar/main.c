#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "ciphers.h"

#define DIF_TIEMPO_MS(inicio, fin) (((double)(fin - inicio)) / CLOCKS_PER_SEC * 1000.0)

const uint8_t CLAVE_BENCHMARK_XOR[16] = "ClaveSimple123";
const uint8_t CLAVE_BENCHMARK_RC4[16] = "ClaveFlujo6543";
const uint8_t CLAVE_BENCHMARK_TEA[16] = {0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF,0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF};
const uint8_t CLAVE_BENCHMARK_VIG[16] = "ClaveVigenere01";
const uint8_t CLAVE_BENCHMARK_AES[16] = {0x43,0x6c,0x61,0x76,0x65,0x41,0x45,0x53,0x31,0x32,0x38,0x5f,0x32,0x30,0x32,0x34};

void imprimir_separador(char c, int longitud) {
    for (int i = 0; i < longitud; i++) putchar(c);
    putchar('\n');
}

void imprimir_celda_alineada(const char *texto, int ancho_visual) {
    int ancho_real = 0, i = 0;
    while (texto[i] != '\0') {
        if ((texto[i] & 0xC0) != 0x80) ancho_real++;
        i++;
    }
    printf("%s", texto);
    for (int s = ancho_real; s < ancho_visual; s++) putchar(' ');
}

int generar_archivo_prueba(const char *nombre_archivo, size_t tamano_kb) {
    FILE *f = fopen(nombre_archivo, "wb");
    if (!f) return 0;
    uint8_t buffer[1024];
    for (int i = 0; i < 1024; i++) buffer[i] = (uint8_t)(rand() % 256);
    for (size_t i = 0; i < tamano_kb; i++) {
        if (fwrite(buffer, 1, 1024, f) != 1024) { fclose(f); return 0; }
    }
    fclose(f);
    return 1;
}

void ejecutar_benchmark_aportes(const char *nombre_archivo) {
    FILE *f = fopen(nombre_archivo, "rb");
    if (!f) { printf("\x1b[31m[!] Error al abrir el archivo '%s'.\x1b[0m\n", nombre_archivo); return; }
    fseek(f, 0, SEEK_END);
    long tamano_archivo = ftell(f);
    fseek(f, 0, SEEK_SET);

    metricas_cifrado_t metricas[10];
    clock_t inicio, fin;
    uint8_t buffer_aporte[TAMANO_APORTE];
    size_t leidos;

    // 1. César
    metricas[0].nombre = "César"; metricas[0].tipo = "Monoalfabética"; metricas[0].seguridad = "Muy Bajo (1/5)";
    metricas[0].memoria_usada = TAMANO_APORTE; metricas[0].tamano_cifrado = tamano_archivo;
    inicio = clock(); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) cesar_cifrar_descifrar(buffer_aporte, leidos, 3, 0, 0);
    fin = clock(); metricas[0].tiempo_cifrado_ms = DIF_TIEMPO_MS(inicio, fin);
    inicio = clock(); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) cesar_cifrar_descifrar(buffer_aporte, leidos, 3, 1, 0);
    fin = clock(); metricas[0].tiempo_descifrado_ms = DIF_TIEMPO_MS(inicio, fin);

    // 2. Vigenère
    metricas[1].nombre = "Vigenère"; metricas[1].tipo = "Polialfabética"; metricas[1].seguridad = "Bajo (2/5)";
    metricas[1].memoria_usada = TAMANO_APORTE; metricas[1].tamano_cifrado = tamano_archivo;
    inicio = clock(); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) vigenere_cifrar_descifrar(buffer_aporte, leidos, CLAVE_BENCHMARK_VIG, 16, 0, 0);
    fin = clock(); metricas[1].tiempo_cifrado_ms = DIF_TIEMPO_MS(inicio, fin);
    inicio = clock(); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) vigenere_cifrar_descifrar(buffer_aporte, leidos, CLAVE_BENCHMARK_VIG, 16, 1, 0);
    fin = clock(); metricas[1].tiempo_descifrado_ms = DIF_TIEMPO_MS(inicio, fin);

    // 3. XOR
    metricas[2].nombre = "XOR"; metricas[2].tipo = "Flujo / Sust."; metricas[2].seguridad = "Muy Bajo (1/5)";
    metricas[2].memoria_usada = TAMANO_APORTE; metricas[2].tamano_cifrado = tamano_archivo;
    size_t idx = 0; inicio = clock(); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) xor_cifrar_descifrar_aporte(buffer_aporte, leidos, CLAVE_BENCHMARK_XOR, 14, &idx, 0);
    fin = clock(); metricas[2].tiempo_cifrado_ms = DIF_TIEMPO_MS(inicio, fin);
    idx = 0; inicio = clock(); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) xor_cifrar_descifrar_aporte(buffer_aporte, leidos, CLAVE_BENCHMARK_XOR, 14, &idx, 0);
    fin = clock(); metricas[2].tiempo_descifrado_ms = DIF_TIEMPO_MS(inicio, fin);

    // 4. RC4
    metricas[3].nombre = "RC4"; metricas[3].tipo = "Flujo (Stream)"; metricas[3].seguridad = "Medio (3.5/5)";
    metricas[3].memoria_usada = TAMANO_APORTE + sizeof(estado_rc4_t); metricas[3].tamano_cifrado = tamano_archivo;
    estado_rc4_t est; inicio = clock(); rc4_inicializar(&est, CLAVE_BENCHMARK_RC4, 14); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) rc4_cifrar_aporte(&est, buffer_aporte, leidos, 0);
    fin = clock(); metricas[3].tiempo_cifrado_ms = DIF_TIEMPO_MS(inicio, fin);
    inicio = clock(); rc4_inicializar(&est, CLAVE_BENCHMARK_RC4, 14); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) rc4_cifrar_aporte(&est, buffer_aporte, leidos, 0);
    fin = clock(); metricas[3].tiempo_descifrado_ms = DIF_TIEMPO_MS(inicio, fin);

    // 5. TEA
    metricas[4].nombre = "TEA"; metricas[4].tipo = "Bloque (64-bit)"; metricas[4].seguridad = "Alto (4.5/5)";
    metricas[4].memoria_usada = TAMANO_APORTE; metricas[4].tamano_cifrado = (tamano_archivo + 7) & ~7;
    inicio = clock(); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) tea_cifrar_aporte(buffer_aporte, leidos, CLAVE_BENCHMARK_TEA, 0);
    fin = clock(); metricas[4].tiempo_cifrado_ms = DIF_TIEMPO_MS(inicio, fin);
    inicio = clock(); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) tea_descifrar_aporte(buffer_aporte, leidos, CLAVE_BENCHMARK_TEA, 0);
    fin = clock(); metricas[4].tiempo_descifrado_ms = DIF_TIEMPO_MS(inicio, fin);

    // 6. Atbash
    metricas[5].nombre = "Atbash"; metricas[5].tipo = "Sustitución"; metricas[5].seguridad = "Bajo (1/5)";
    metricas[5].memoria_usada = TAMANO_APORTE; metricas[5].tamano_cifrado = tamano_archivo;
    inicio = clock(); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) atbash_cifrar_descifrar(buffer_aporte, leidos, 0);
    fin = clock(); metricas[5].tiempo_cifrado_ms = DIF_TIEMPO_MS(inicio, fin);
    inicio = clock(); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) atbash_cifrar_descifrar(buffer_aporte, leidos, 0);
    fin = clock(); metricas[5].tiempo_descifrado_ms = DIF_TIEMPO_MS(inicio, fin);

    // 7. Transposición
    metricas[6].nombre = "Transposición"; metricas[6].tipo = "Permutación"; metricas[6].seguridad = "Bajo (1.5/5)";
    metricas[6].memoria_usada = TAMANO_APORTE; metricas[6].tamano_cifrado = tamano_archivo;
    inicio = clock(); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) transposicion_bloque_cifrar_descifrar(buffer_aporte, leidos, 0, 0);
    fin = clock(); metricas[6].tiempo_cifrado_ms = DIF_TIEMPO_MS(inicio, fin);
    inicio = clock(); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) transposicion_bloque_cifrar_descifrar(buffer_aporte, leidos, 1, 0);
    fin = clock(); metricas[6].tiempo_descifrado_ms = DIF_TIEMPO_MS(inicio, fin);

    // 8. Vernam
    metricas[7].nombre = "Vernam (OTP)"; metricas[7].tipo = "Flujo (Seed)"; metricas[7].seguridad = "Alto (4/5)";
    metricas[7].memoria_usada = TAMANO_APORTE; metricas[7].tamano_cifrado = tamano_archivo;
    inicio = clock(); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) vernam_cifrar_descifrar(buffer_aporte, leidos, 12345, 0);
    fin = clock(); metricas[7].tiempo_cifrado_ms = DIF_TIEMPO_MS(inicio, fin);
    inicio = clock(); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) vernam_cifrar_descifrar(buffer_aporte, leidos, 12345, 0);
    fin = clock(); metricas[7].tiempo_descifrado_ms = DIF_TIEMPO_MS(inicio, fin);

    // 9. AES-128
    metricas[8].nombre = "AES-128"; metricas[8].tipo = "Bloque (128-bit)"; metricas[8].seguridad = "Muy Alto (5/5)";
    metricas[8].memoria_usada = TAMANO_APORTE; metricas[8].tamano_cifrado = (tamano_archivo + 15) & ~15;
    inicio = clock(); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) aes128_cifrar_aporte(buffer_aporte, leidos, CLAVE_BENCHMARK_AES, 0);
    fin = clock(); metricas[8].tiempo_cifrado_ms = DIF_TIEMPO_MS(inicio, fin);
    inicio = clock(); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) aes128_descifrar_aporte(buffer_aporte, leidos, CLAVE_BENCHMARK_AES, 0);
    fin = clock(); metricas[8].tiempo_descifrado_ms = DIF_TIEMPO_MS(inicio, fin);

    // 10. RSA
    metricas[9].nombre = "RSA (Educativo)"; metricas[9].tipo = "Asimétrico"; metricas[9].seguridad = "Alto (Demo)";
    metricas[9].memoria_usada = TAMANO_APORTE * 8; metricas[9].tamano_cifrado = tamano_archivo * 8;
    llaves_rsa_t ll; rsa_generar_llaves(&ll); uint64_t *br = malloc(TAMANO_APORTE * 8);
    inicio = clock(); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) rsa_cifrar_aporte(buffer_aporte, br, leidos, ll.e, ll.n);
    fin = clock(); metricas[9].tiempo_cifrado_ms = DIF_TIEMPO_MS(inicio, fin);
    inicio = clock(); fseek(f, 0, SEEK_SET);
    while ((leidos = fread(buffer_aporte, 1, TAMANO_APORTE, f)) > 0) rsa_descifrar_aporte(br, buffer_aporte, leidos, ll.d, ll.n);
    fin = clock(); metricas[9].tiempo_descifrado_ms = DIF_TIEMPO_MS(inicio, fin);
    free(br); fclose(f);

    printf("\n\x1b[1;36m=== COMPARATIVA DE RENDIMIENTO (10 ALGORITMOS) ===\x1b[0m\n");
    imprimir_separador('-', 99);
    printf("| %-22s | %-20s | %-19s | %-11s | %-11s |\n", "Algoritmo", "Tipo", "Seguridad", "T. Enc", "T. Dec");
    imprimir_separador('-', 99);
    for (int i = 0; i < 10; i++) {
        printf("| "); imprimir_celda_alineada(metricas[i].nombre, 22); printf(" | ");
        imprimir_celda_alineada(metricas[i].tipo, 20); printf(" | ");
        imprimir_celda_alineada(metricas[i].seguridad, 19);
        printf(" | %8.3f ms | %8.3f ms |\n", metricas[i].tiempo_cifrado_ms, metricas[i].tiempo_descifrado_ms);
    }
    imprimir_separador('-', 99);
}

int cifrar_archivo_por_aportes(const char *origen, const char *destino, int alg, int p_a_p, const uint8_t *clave, size_t longitud_clave, int desplazamiento) {
    FILE *f_in = fopen(origen, "rb"), *f_out = fopen(destino, "wb");
    if (!f_in || !f_out) { if (f_in) fclose(f_in); if (f_out) fclose(f_out); return 0; }
    uint8_t buffer[TAMANO_APORTE]; size_t leidos;
    if (alg == 1) while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { cesar_cifrar_descifrar(buffer, leidos, desplazamiento, 0, p_a_p); fwrite(buffer, 1, leidos, f_out); }
    else if (alg == 2) while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { vigenere_cifrar_descifrar(buffer, leidos, clave, longitud_clave, 0, p_a_p); fwrite(buffer, 1, leidos, f_out); }
    else if (alg == 3) { size_t idx = 0; while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { xor_cifrar_descifrar_aporte(buffer, leidos, clave, longitud_clave, &idx, p_a_p); fwrite(buffer, 1, leidos, f_out); } }
    else if (alg == 4) { estado_rc4_t es; rc4_inicializar(&es, clave, longitud_clave); while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { rc4_cifrar_aporte(&es, buffer, leidos, p_a_p); fwrite(buffer, 1, leidos, f_out); } }
    else if (alg == 5) while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { if (leidos % 8 != 0) leidos = (leidos + 7) & ~7; tea_cifrar_aporte(buffer, leidos, clave, p_a_p); fwrite(buffer, 1, leidos, f_out); }
    else if (alg == 6) while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { atbash_cifrar_descifrar(buffer, leidos, p_a_p); fwrite(buffer, 1, leidos, f_out); }
    else if (alg == 7) while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { transposicion_bloque_cifrar_descifrar(buffer, leidos, 0, p_a_p); fwrite(buffer, 1, leidos, f_out); }
    else if (alg == 8) while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { vernam_cifrar_descifrar(buffer, leidos, 12345, p_a_p); fwrite(buffer, 1, leidos, f_out); }
    else if (alg == 9) while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { if (leidos % 16 != 0) leidos = (leidos + 15) & ~15; aes128_cifrar_aporte(buffer, leidos, clave, p_a_p); fwrite(buffer, 1, leidos, f_out); }
    else if (alg == 10) { llaves_rsa_t ll; rsa_generar_llaves(&ll); uint64_t *br = malloc(TAMANO_APORTE * 8); while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { rsa_cifrar_aporte(buffer, br, leidos, ll.e, ll.n); fwrite(br, 8, leidos, f_out); } free(br); }
    fclose(f_in); fclose(f_out); return 1;
}

int descifrar_archivo_por_aportes(const char *origen, const char *destino, int alg, int p_a_p, const uint8_t *clave, size_t longitud_clave, int desplazamiento) {
    FILE *f_in = fopen(origen, "rb"), *f_out = fopen(destino, "wb");
    if (!f_in || !f_out) { if (f_in) fclose(f_in); if (f_out) fclose(f_out); return 0; }
    uint8_t buffer[TAMANO_APORTE]; size_t leidos;
    if (alg == 1) while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { cesar_cifrar_descifrar(buffer, leidos, desplazamiento, 1, p_a_p); fwrite(buffer, 1, leidos, f_out); }
    else if (alg == 2) while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { vigenere_cifrar_descifrar(buffer, leidos, clave, longitud_clave, 1, p_a_p); fwrite(buffer, 1, leidos, f_out); }
    else if (alg == 3) { size_t idx = 0; while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { xor_cifrar_descifrar_aporte(buffer, leidos, clave, longitud_clave, &idx, p_a_p); fwrite(buffer, 1, leidos, f_out); } }
    else if (alg == 4) { estado_rc4_t es; rc4_inicializar(&es, clave, longitud_clave); while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { rc4_cifrar_aporte(&es, buffer, leidos, p_a_p); fwrite(buffer, 1, leidos, f_out); } }
    else if (alg == 5) while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { tea_descifrar_aporte(buffer, leidos, clave, p_a_p); fwrite(buffer, 1, leidos, f_out); }
    else if (alg == 6) while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { atbash_cifrar_descifrar(buffer, leidos, p_a_p); fwrite(buffer, 1, leidos, f_out); }
    else if (alg == 7) while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { transposicion_bloque_cifrar_descifrar(buffer, leidos, 1, p_a_p); fwrite(buffer, 1, leidos, f_out); }
    else if (alg == 8) while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { vernam_cifrar_descifrar(buffer, leidos, 12345, p_a_p); fwrite(buffer, 1, leidos, f_out); }
    else if (alg == 9) while ((leidos = fread(buffer, 1, TAMANO_APORTE, f_in)) > 0) { aes128_descifrar_aporte(buffer, leidos, clave, p_a_p); fwrite(buffer, 1, leidos, f_out); }
    else if (alg == 10) { llaves_rsa_t ll; rsa_generar_llaves(&ll); uint64_t *br = malloc(TAMANO_APORTE * 8); while ((leidos = fread(br, 8, TAMANO_APORTE, f_in)) > 0) { rsa_descifrar_aporte(br, buffer, leidos, ll.d, ll.n); fwrite(buffer, 1, leidos, f_out); } free(br); }
    fclose(f_in); fclose(f_out); return 1;
}

void mostrar_menu() {
    printf("\n\x1b[1;34m============================================================\x1b[0m\n");
    printf("\x1b[1;34m|   SISTEMA DE ENCRIPTACIÓN PEDAGÓGICO (10 ALGORITMOS)    |\x1b[0m\n");
    printf("\x1b[1;34m============================================================\x1b[0m\n");
    printf(" 1. Generar archivo de prueba\n 2. Ejecutar comparativa (Benchmark)\n 3. Cifrar archivo\n 4. Descifrar archivo\n 5. Prueba paso a paso (Texto)\n 6. Salir\n Selecciona: ");
}

int main() {
    srand(time(NULL)); int op; char n1[256], n2[256];
    while (1) {
        mostrar_menu(); if (scanf("%d", &op) != 1) { while (getchar() != '\n'); continue; }
        if (op == 1) { printf("Nombre: "); scanf("%s", n1); printf("KB: "); size_t kb; scanf("%lu", &kb); generar_archivo_prueba(n1, kb); }
        else if (op == 2) { printf("Nombre: "); scanf("%s", n1); ejecutar_benchmark_aportes(n1); }
        else if (op == 3 || op == 4) {
            printf("Origen: "); scanf("%s", n1); printf("Destino: "); scanf("%s", n2);
            printf("Algoritmo (1-10):\n 1. César, 2. Vigenère, 3. XOR, 4. RC4, 5. TEA, 6. Atbash, 7. Transp, 8. Vernam, 9. AES-128, 10. RSA\n Opción: ");
            int a; scanf("%d", &a);
            uint8_t k[256]; size_t lk = 0; int d = 3;
            if (a == 1) { printf("Desp: "); scanf("%d", &d); }
            else if ((a >= 2 && a <= 5) || a == 9) { printf("Clave: "); char ks[256]; scanf("%s", ks); lk = strlen(ks); memcpy(k, ks, lk); if(a==9)lk=16; }
            if (op == 3) cifrar_archivo_por_aportes(n1, n2, a, 0, k, lk, d);
            else descifrar_archivo_por_aportes(n1, n2, a, 0, k, lk, d);
        }
        else if (op == 5) { /* Simplificado */ printf("Funcionalidad demo paso a paso.\n"); }
        else if (op == 6) break;
    }
    return 0;
}
