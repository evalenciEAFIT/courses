/**
 * @file    ciphers.h
 * @brief   Interfaz pública del módulo de cifrado pedagógico.
 *
 * Define los prototipos de los 10 algoritmos de cifrado implementados,
 * las estructuras de estado necesarias y las constantes de configuración.
 * Todos los algoritmos operan sobre buffers de bytes (uint8_t) y soportan
 * procesamiento por aportes (streaming/chunks) para minimizar el uso de RAM.
 *
 * Algoritmos implementados:
 *   1. César       - Sustitución monoalfabética
 *   2. Vigenère    - Sustitución polialfabética
 *   3. XOR         - Flujo / Sustitución simple
 *   4. RC4         - Cifrado de flujo (stream cipher)
 *   5. TEA         - Cifrado de bloque (64-bit, Feistel)
 *   6. Atbash      - Sustitución por espejo de byte
 *   7. Transposición - Permutación de bytes en bloques
 *   8. Vernam (OTP) - Flujo pseudoaleatorio con semilla (LCG)
 *   9. AES-128     - Estándar moderno, cifrado de bloque (128-bit)
 *  10. RSA         - Cifrado asimétrico (educativo, llaves de 64-bit)
 */

#ifndef CIPHERS_H
#define CIPHERS_H

#include <stddef.h>
#include <stdint.h>

/* =========================================================
 * Constantes globales del módulo
 * ========================================================= */

/** Tamaño del aporte (chunk) en bytes para procesamiento por streaming.
 *  4096 bytes = 4 KB: tamaño óptimo para caché L1 en la mayoría de CPUs. */
#define TAMANO_APORTE 4096

/** Número de rondas AES para clave de 128 bits */
#define AES_RONDAS 10

/** Número de palabras (32-bit) en el schedule de claves AES-128: 11 rondas * 4 palabras */
#define AES_KEY_SCHEDULE_SIZE 176

/* =========================================================
 * Estructuras de datos
 * ========================================================= */

/**
 * @brief Métricas de rendimiento para un algoritmo de cifrado.
 *
 * Almacena los resultados del benchmark por aportes: tiempos de
 * cifrado/descifrado, consumo de RAM y tamaño del archivo resultante.
 */
typedef struct {
    const char *nombre;             /**< Nombre descriptivo del algoritmo       */
    const char *tipo;               /**< Categoría: "Flujo", "Bloque", etc.     */
    const char *seguridad;          /**< Nivel de seguridad cualitativo         */
    double      tiempo_cifrado_ms;  /**< Tiempo de cifrado en milisegundos      */
    double      tiempo_descifrado_ms;/**< Tiempo de descifrado en milisegundos  */
    size_t      memoria_usada;      /**< Pico de RAM en bytes (por aporte)      */
    size_t      tamano_cifrado;     /**< Tamaño del archivo cifrado en bytes    */
} metricas_cifrado_t;

/**
 * @brief Estado interno del cifrado RC4 (permutación S y contadores i, j).
 *
 * Debe inicializarse con rc4_inicializar() antes de procesar datos.
 * El estado persiste entre aportes para garantizar continuidad del keystream.
 */
typedef struct {
    uint8_t s[256]; /**< Permutación del espacio de estado (S-Box dinámica) */
    int     i;      /**< Índice de avance del keystream                     */
    int     j;      /**< Índice de intercambio del keystream                */
} estado_rc4_t;

/**
 * @brief Par de llaves pública/privada para el cifrado RSA educativo.
 *
 * Con primos p=61, q=53 se obtiene n=3233, phi=3120.
 * Llave pública: (e=17, n=3233) | Llave privada: (d=2753, n=3233)
 */
typedef struct {
    uint64_t n; /**< Módulo RSA: n = p * q                                */
    uint64_t e; /**< Exponente público (coprime con phi(n))               */
    uint64_t d; /**< Exponente privado: d = e^-1 mod phi(n)              */
} llaves_rsa_t;

/* =========================================================
 * 1. Cifrado César — Sustitución monoalfabética
 * =========================================================
 * Fórmula: C = (P + k) mod 256   |   P = (C - k + 256) mod 256
 */

/**
 * @brief Cifra o descifra un buffer de bytes con el método César.
 * @param datos        Buffer de datos a procesar (modificado in-place).
 * @param tamano       Número de bytes válidos en el buffer.
 * @param desplazamiento Valor k del desplazamiento (puede ser negativo).
 * @param descifrar    0 = cifrar, 1 = descifrar.
 * @param paso_a_paso  1 = mostrar trazas byte a byte (máx. 5 bytes).
 */
void cesar_cifrar_descifrar(uint8_t *datos, size_t tamano,
                            int desplazamiento, int descifrar, int paso_a_paso);

/* =========================================================
 * 2. Cifrado Vigenère — Sustitución polialfabética
 * =========================================================
 * Fórmula: C_i = (P_i + K[i mod m]) mod 256
 *          P_i = (C_i - K[i mod m] + 256) mod 256
 */

/**
 * @brief Cifra o descifra un buffer con el método Vigenère.
 * @param datos          Buffer de datos (modificado in-place).
 * @param tamano         Número de bytes a procesar.
 * @param clave          Array de bytes que forma la clave repetitiva.
 * @param longitud_clave Longitud m de la clave.
 * @param descifrar      0 = cifrar, 1 = descifrar.
 * @param paso_a_paso    1 = activar trazas pedagógicas.
 */
void vigenere_cifrar_descifrar(uint8_t *datos, size_t tamano,
                               const uint8_t *clave, size_t longitud_clave,
                               int descifrar, int paso_a_paso);

/* =========================================================
 * 3. Cifrado XOR — Flujo / Sustitución simple
 * =========================================================
 * Fórmula: C = P XOR K   (simétrico: aplicar dos veces = identidad)
 * El índice de clave persiste entre aportes para el keystream continuo.
 */

/**
 * @brief Aplica XOR con clave repetitiva a un aporte del stream.
 * @param datos          Buffer de datos (modificado in-place).
 * @param tamano         Bytes a procesar en este aporte.
 * @param clave          Clave de longitud variable.
 * @param longitud_clave Longitud de la clave en bytes.
 * @param indice_clave   [in/out] Posición actual en la clave (persistente).
 * @param paso_a_paso    1 = mostrar trazas.
 */
void xor_cifrar_descifrar_aporte(uint8_t *datos, size_t tamano,
                                 const uint8_t *clave, size_t longitud_clave,
                                 size_t *indice_clave, int paso_a_paso);

/* =========================================================
 * 4. Cifrado de Flujo RC4 — Stream cipher por KSA + PRGA
 * =========================================================
 * KSA: Key Scheduling Algorithm — inicializa la permutación S[256].
 * PRGA: Pseudo-Random Generation Algorithm — genera el keystream.
 */

/**
 * @brief Inicializa el estado RC4 con la clave dada (KSA).
 * @param estado        Puntero al estado a inicializar.
 * @param clave         Clave secreta de longitud variable.
 * @param longitud_clave Longitud de la clave en bytes (1-256 recomendado).
 */
void rc4_inicializar(estado_rc4_t *estado,
                     const uint8_t *clave, size_t longitud_clave);

/**
 * @brief Cifra/descifra un aporte con RC4 (PRGA). El estado persiste.
 * @param estado     Estado RC4 (modificado: avanza el keystream).
 * @param datos      Buffer a procesar (in-place).
 * @param tamano     Bytes a cifrar en este aporte.
 * @param paso_a_paso 1 = mostrar primeros 5 bytes del keystream.
 */
void rc4_cifrar_aporte(estado_rc4_t *estado, uint8_t *datos,
                       size_t tamano, int paso_a_paso);

/* =========================================================
 * 5. Cifrado TEA — Tiny Encryption Algorithm (Feistel, 64-bit)
 * =========================================================
 * Opera en bloques de 64 bits (8 bytes), clave de 128 bits (16 bytes).
 * 32 rondas de mezcla con delta = 0x9E3779B9 (cociente áureo).
 * NOTA: El último bloque debe rellenarse hasta múltiplo de 8 bytes.
 */

/** @brief Cifra un bloque de 64 bits (2 x uint32_t) con TEA. */
void tea_cifrar_bloque(uint32_t v[2], const uint32_t k[4]);

/** @brief Descifra un bloque de 64 bits con TEA (operación inversa). */
void tea_descifrar_bloque(uint32_t v[2], const uint32_t k[4]);

/**
 * @brief Cifra todos los bloques completos de 8 bytes en un aporte.
 * @param datos      Buffer con datos (tamano debe ser múltiplo de 8 antes de llamar).
 * @param tamano     Bytes a procesar (se ignoran los bytes sobrantes < 8).
 * @param clave      Clave de 16 bytes.
 * @param paso_a_paso 1 = mostrar trazas.
 */
void tea_cifrar_aporte(uint8_t *datos, size_t tamano,
                       const uint8_t clave[16], int paso_a_paso);

/** @brief Descifra todos los bloques de 8 bytes en un aporte TEA. */
void tea_descifrar_aporte(uint8_t *datos, size_t tamano,
                          const uint8_t clave[16], int paso_a_paso);

/* =========================================================
 * 6. Cifrado Atbash — Sustitución de espejo de byte
 * =========================================================
 * Fórmula: C = 255 - P  (es su propio inverso: descifrar = cifrar)
 */

/**
 * @brief Aplica transformación Atbash a cada byte (operación simétrica).
 * @param datos      Buffer de bytes (in-place).
 * @param tamano     Número de bytes.
 * @param paso_a_paso 1 = mostrar trazas.
 */
void atbash_cifrar_descifrar(uint8_t *datos, size_t tamano, int paso_a_paso);

/* =========================================================
 * 7. Transposición por Bloques — Permutación posicional
 * =========================================================
 * Cifrado: [b0,b1,b2,b3] → [b3,b0,b1,b2]  (rotación a la derecha)
 * Descifrado: [b3,b0,b1,b2] → [b0,b1,b2,b3] (rotación a la izquierda)
 */

/**
 * @brief Permuta bytes en bloques de 4 por rotación cíclica.
 * @param datos      Buffer de datos (in-place).
 * @param tamano     Bytes a procesar (bloques incompletos al final se ignoran).
 * @param descifrar  0 = cifrar (rotar derecha), 1 = descifrar (rotar izquierda).
 * @param paso_a_paso 1 = mostrar el primer bloque transformado.
 */
void transposicion_bloque_cifrar_descifrar(uint8_t *datos, size_t tamano,
                                           int descifrar, int paso_a_paso);

/* =========================================================
 * 8. Cifrado Vernam (OTP simulado) — Flujo pseudoaleatorio LCG
 * =========================================================
 * Genera una secuencia pseudoaleatoria con LCG (Linear Congruential Generator):
 *   lcg = (lcg * 1103515245 + 12345) & 0x7FFFFFFF  (parámetros ANSI C)
 * Fórmula: C_i = P_i XOR (lcg_i mod 256)
 * La misma semilla reproduce la misma secuencia para descifrar.
 */

/**
 * @brief Cifra/descifra con flujo Vernam (OTP con LCG).
 * @param datos      Buffer de datos (in-place).
 * @param tamano     Bytes a procesar.
 * @param semilla    Semilla del generador LCG (debe ser la misma para descifrar).
 * @param paso_a_paso 1 = mostrar trazas.
 *
 * @warning Para un aporte N, el LCG se inicializa desde la semilla.
 *          Si se llama en múltiples aportes del mismo archivo,
 *          se debe pasar un estado acumulado (no reimplementado aquí por simplicidad).
 */
void vernam_cifrar_descifrar(uint8_t *datos, size_t tamano,
                             uint32_t semilla, int paso_a_paso);

/* =========================================================
 * 9. AES-128 — Advanced Encryption Standard (FIPS 197)
 * =========================================================
 * Cifrado de bloque simétrico estándar mundial. Procesa bloques de 128 bits
 * (16 bytes) con clave de 128 bits mediante 10 rondas de:
 *   SubBytes → ShiftRows → MixColumns → AddRoundKey
 * La ronda final omite MixColumns.
 *
 * NOTA: Implementación pedagógica sin optimizaciones de rendimiento (tablas T).
 * Para producción usar libssl o libmbedtls.
 */

/**
 * @brief Cifra bloques de 16 bytes con AES-128 (modo ECB por aporte).
 * @param datos      Buffer (múltiplos de 16 bytes; bytes sobrantes se ignoran).
 * @param tamano     Total de bytes en el buffer.
 * @param clave      Clave AES de exactamente 16 bytes.
 * @param paso_a_paso 1 = mostrar estado del primer bloque por ronda.
 */
void aes128_cifrar_aporte(uint8_t *datos, size_t tamano,
                          const uint8_t clave[16], int paso_a_paso);

/**
 * @brief Descifra bloques de 16 bytes con AES-128 (modo ECB por aporte).
 * @param datos      Buffer de datos cifrados (múltiplos de 16 bytes).
 * @param tamano     Total de bytes.
 * @param clave      La misma clave de 16 bytes usada para cifrar.
 * @param paso_a_paso 1 = mostrar trazas inversas.
 */
void aes128_descifrar_aporte(uint8_t *datos, size_t tamano,
                             const uint8_t clave[16], int paso_a_paso);

/* =========================================================
 * 10. RSA Educativo — Cifrado asimétrico (llaves de 64 bits)
 * =========================================================
 * RSA completo: C = M^e mod n  |  M = C^d mod n
 * Parámetros pedagógicos fijos: p=61, q=53, n=3233, e=17, d=2753
 * Cada byte de entrada (0-255 < n=3233) se cifra a un uint64_t.
 * Esto implica EXPANSIÓN: el archivo cifrado mide 8× el original.
 *
 * Para producción se usa RSA con n >= 2048 bits y relleno OAEP.
 */

/**
 * @brief Genera el par de llaves RSA pedagógicas (valores fijos).
 * @param llaves  [out] Estructura con (e, d, n) listos para usar.
 */
void rsa_generar_llaves(llaves_rsa_t *llaves);

/**
 * @brief Cifra un aporte byte a byte con la llave pública RSA.
 * @param in     Array de bytes de entrada (texto plano).
 * @param out    Array de uint64_t de salida (debe tener tamano elementos).
 * @param tamano Número de bytes a cifrar.
 * @param e      Exponente público.
 * @param n      Módulo RSA.
 */
void rsa_cifrar_aporte(const uint8_t *in, uint64_t *out,
                       size_t tamano, uint64_t e, uint64_t n);

/**
 * @brief Descifra un aporte de uint64_t con la llave privada RSA.
 * @param in     Array de uint64_t cifrados (tamano elementos).
 * @param out    Array de bytes de salida (texto plano recuperado).
 * @param tamano Número de elementos a descifrar.
 * @param d      Exponente privado.
 * @param n      Módulo RSA.
 */
void rsa_descifrar_aporte(const uint64_t *in, uint8_t *out,
                          size_t tamano, uint64_t d, uint64_t n);

#endif /* CIPHERS_H */
