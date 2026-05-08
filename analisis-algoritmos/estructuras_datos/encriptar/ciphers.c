#include "ciphers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------
// 1. Cifrado César (Sustitución Monoalfabética por desplazamiento)
// ---------------------------------------------------------
void cesar_cifrar_descifrar(uint8_t *datos, size_t tamano, int desplazamiento, int descifrar, int paso_a_paso) {
    int desp = (desplazamiento % 256 + 256) % 256;
    if (descifrar) desp = 256 - desp;

    if (paso_a_paso && tamano > 0) printf("\n\x1b[33m[Traza Paso a Paso: Cifrado César (Desp: %d)]\x1b[0m\n", desplazamiento);

    for (size_t i = 0; i < tamano; i++) {
        uint8_t original = datos[i];
        datos[i] = (uint8_t)((datos[i] + desp) % 256);
        if (paso_a_paso && i < 5) {
            printf("  Byte [%02lu] '0x%02X' ('%c') --> '0x%02X' ('%c')\n",
                   (unsigned long)i, original, (original >= 32 && original < 127) ? original : '.',
                   datos[i], (datos[i] >= 32 && datos[i] < 127) ? datos[i] : '.');
        }
    }
}

// ---------------------------------------------------------
// 2. Cifrado Vigenère (Sustitución Polialfabética)
// ---------------------------------------------------------
void vigenere_cifrar_descifrar(uint8_t *datos, size_t tamano, const uint8_t *clave, size_t longitud_clave, int descifrar, int paso_a_paso) {
    if (longitud_clave == 0) return;
    if (paso_a_paso && tamano > 0) printf("\n\x1b[33m[Traza Paso a Paso: Cifrado Vigenère]\x1b[0m\n");

    for (size_t i = 0; i < tamano; i++) {
        uint8_t original = datos[i];
        uint8_t c = clave[i % longitud_clave];
        if (descifrar) datos[i] = (uint8_t)((datos[i] - c + 256) % 256);
        else datos[i] = (uint8_t)((datos[i] + c) % 256);

        if (paso_a_paso && i < 5) printf("  Byte [%02lu] '0x%02X' + Clave['%c'] --> '0x%02X'\n", (unsigned long)i, original, (c >= 32 && c < 127) ? c : '.', datos[i]);
    }
}

// ---------------------------------------------------------
// 3. Cifrado XOR por Aportes
// ---------------------------------------------------------
void xor_cifrar_descifrar_aporte(uint8_t *datos, size_t tamano, const uint8_t *clave, size_t longitud_clave, size_t *indice_clave, int paso_a_paso) {
    if (longitud_clave == 0) return;
    if (paso_a_paso && tamano > 0) printf("\n\x1b[33m[Traza Paso a Paso: Cifrado XOR]\x1b[0m\n");

    for (size_t i = 0; i < tamano; i++) {
        uint8_t original = datos[i];
        uint8_t k = clave[*indice_clave];
        datos[i] ^= k;
        if (paso_a_paso && i < 5) printf("  Byte [%02lu] '0x%02X' ^ Clave['%c'] --> '0x%02X'\n", (unsigned long)i, original, (k >= 32 && k < 127) ? k : '.', datos[i]);
        *indice_clave = (*indice_clave + 1) % longitud_clave;
    }
}

// ---------------------------------------------------------
// 4. Cifrado RC4 por Aportes
// ---------------------------------------------------------
void rc4_inicializar(estado_rc4_t *estado, const uint8_t *clave, size_t longitud_clave) {
    for (int i = 0; i < 256; i++) estado->s[i] = i;
    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + estado->s[i] + clave[i % longitud_clave]) % 256;
        uint8_t temp = estado->s[i];
        estado->s[i] = estado->s[j];
        estado->s[j] = temp;
    }
    estado->i = estado->j = 0;
}

void rc4_cifrar_aporte(estado_rc4_t *estado, uint8_t *datos, size_t tamano, int paso_a_paso) {
    int i = estado->i, j = estado->j;
    if (paso_a_paso && tamano > 0) printf("\n\x1b[33m[Traza Paso a Paso: Cifrado RC4]\x1b[0m\n");

    for (size_t k = 0; k < tamano; k++) {
        i = (i + 1) % 256;
        j = (j + estado->s[i]) % 256;
        uint8_t temp = estado->s[i];
        estado->s[i] = estado->s[j];
        estado->s[j] = temp;
        uint8_t K = estado->s[(estado->s[i] + estado->s[j]) % 256];
        uint8_t original = datos[k];
        datos[k] ^= K;
        if (paso_a_paso && k < 5) printf("  Byte [%02lu] '0x%02X' ^ Keystream('0x%02X') --> '0x%02X'\n", (unsigned long)k, original, K, datos[k]);
    }
    estado->i = i; estado->j = j;
}

// ---------------------------------------------------------
// 5. Cifrado TEA por Aportes
// ---------------------------------------------------------
void tea_cifrar_bloque(uint32_t v[2], const uint32_t k[4]) {
    uint32_t v0 = v[0], v1 = v[1], suma = 0, delta = 0x9e3779b9;
    for (int i = 0; i < 32; i++) {
        suma += delta;
        v0 += ((v1 << 4) + k[0]) ^ (v1 + suma) ^ ((v1 >> 5) + k[1]);
        v1 += ((v0 << 4) + k[2]) ^ (v0 + suma) ^ ((v0 >> 5) + k[3]);
    }
    v[0] = v0; v[1] = v1;
}

void tea_descifrar_bloque(uint32_t v[2], const uint32_t k[4]) {
    uint32_t v0 = v[0], v1 = v[1], suma = 0xC6EF3720, delta = 0x9e3779b9;
    for (int i = 0; i < 32; i++) {
        v1 -= ((v0 << 4) + k[2]) ^ (v0 + suma) ^ ((v0 >> 5) + k[3]);
        v0 -= ((v1 << 4) + k[0]) ^ (v1 + suma) ^ ((v1 >> 5) + k[1]);
        suma -= delta;
    }
    v[0] = v0; v[1] = v1;
}

void tea_cifrar_aporte(uint8_t *datos, size_t tamano, const uint8_t clave[16], int paso_a_paso) {
    const uint32_t *k = (const uint32_t*)clave;
    if (paso_a_paso && tamano > 0) printf("\n\x1b[33m[Traza Paso a Paso: Cifrado TEA]\x1b[0m\n");
    for (size_t i = 0; i + 8 <= tamano; i += 8) {
        uint32_t *v = (uint32_t*)&datos[i];
        tea_cifrar_bloque(v, k);
    }
}

void tea_descifrar_aporte(uint8_t *datos, size_t tamano, const uint8_t clave[16], int paso_a_paso) {
    const uint32_t *k = (const uint32_t*)clave;
    if (paso_a_paso && tamano > 0) printf("\n\x1b[33m[Traza Paso a Paso: Descifrado TEA]\x1b[0m\n");
    for (size_t i = 0; i + 8 <= tamano; i += 8) {
        uint32_t *v = (uint32_t*)&datos[i];
        tea_descifrar_bloque(v, k);
    }
}

// ---------------------------------------------------------
// 6. Cifrado Atbash
// ---------------------------------------------------------
void atbash_cifrar_descifrar(uint8_t *datos, size_t tamano, int paso_a_paso) {
    if (paso_a_paso && tamano > 0) printf("\n\x1b[33m[Traza Paso a Paso: Cifrado Atbash]\x1b[0m\n");
    for (size_t i = 0; i < tamano; i++) datos[i] = 255 - datos[i];
}

// ---------------------------------------------------------
// 7. Transposición por Bloques
// ---------------------------------------------------------
void transposicion_bloque_cifrar_descifrar(uint8_t *datos, size_t tamano, int descifrar, int paso_a_paso) {
    if (paso_a_paso && tamano > 0) printf("\n\x1b[33m[Traza Paso a Paso: Transposición]\x1b[0m\n");
    for (size_t i = 0; i + 4 <= tamano; i += 4) {
        uint8_t b0 = datos[i], b1 = datos[i+1], b2 = datos[i+2], b3 = datos[i+3];
        if (descifrar) { datos[i]=b1; datos[i+1]=b2; datos[i+2]=b3; datos[i+3]=b0; }
        else { datos[i]=b3; datos[i+1]=b0; datos[i+2]=b1; datos[i+3]=b2; }
    }
}

// ---------------------------------------------------------
// 8. Cifrado Vernam (OTP)
// ---------------------------------------------------------
void vernam_cifrar_descifrar(uint8_t *datos, size_t tamano, uint32_t semilla, int paso_a_paso) {
    if (paso_a_paso && tamano > 0) printf("\n\x1b[33m[Traza Paso a Paso: Cifrado Vernam]\x1b[0m\n");
    uint32_t lcg = semilla;
    for (size_t i = 0; i < tamano; i++) {
        lcg = (lcg * 1103515245 + 12345) & 0x7fffffff;
        datos[i] ^= (uint8_t)(lcg % 256);
    }
}

// ---------------------------------------------------------
// 9. Cifrado AES-128 (Implementación Pedagógica)
// ---------------------------------------------------------

static const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static const uint8_t rsbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

static const uint8_t rcon[11] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

static void aes_sub_bytes(uint8_t state[16]) { for (int i = 0; i < 16; i++) state[i] = sbox[state[i]]; }
static void aes_inv_sub_bytes(uint8_t state[16]) { for (int i = 0; i < 16; i++) state[i] = rsbox[state[i]]; }
static void aes_shift_rows(uint8_t state[16]) {
    uint8_t tmp[16]; memcpy(tmp, state, 16);
    state[1] = tmp[5]; state[5] = tmp[9]; state[9] = tmp[13]; state[13] = tmp[1];
    state[2] = tmp[10]; state[6] = tmp[14]; state[10] = tmp[2]; state[14] = tmp[6];
    state[3] = tmp[15]; state[7] = tmp[3]; state[11] = tmp[7]; state[15] = tmp[11];
}
static void aes_inv_shift_rows(uint8_t state[16]) {
    uint8_t tmp[16]; memcpy(tmp, state, 16);
    state[1] = tmp[13]; state[5] = tmp[1]; state[9] = tmp[5]; state[13] = tmp[9];
    state[2] = tmp[10]; state[6] = tmp[14]; state[10] = tmp[2]; state[14] = tmp[6];
    state[3] = tmp[7]; state[7] = tmp[11]; state[11] = tmp[15]; state[15] = tmp[3];
}
static uint8_t aes_mul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) p ^= a;
        uint8_t hi = a & 0x80; a <<= 1;
        if (hi) a ^= 0x1b;
        b >>= 1;
    }
    return p;
}
static void aes_mix_columns(uint8_t state[16]) {
    for (int i = 0; i < 4; i++) {
        uint8_t *c = &state[i * 4], a[4]; memcpy(a, c, 4);
        c[0] = aes_mul(2, a[0]) ^ aes_mul(3, a[1]) ^ a[2] ^ a[3];
        c[1] = a[0] ^ aes_mul(2, a[1]) ^ aes_mul(3, a[2]) ^ a[3];
        c[2] = a[0] ^ a[1] ^ aes_mul(2, a[2]) ^ aes_mul(3, a[3]);
        c[3] = aes_mul(3, a[0]) ^ a[1] ^ a[2] ^ aes_mul(2, a[3]);
    }
}
static void aes_inv_mix_columns(uint8_t state[16]) {
    for (int i = 0; i < 4; i++) {
        uint8_t *c = &state[i * 4], a[4]; memcpy(a, c, 4);
        c[0] = aes_mul(14, a[0]) ^ aes_mul(11, a[1]) ^ aes_mul(13, a[2]) ^ aes_mul(9, a[3]);
        c[1] = aes_mul(9, a[0]) ^ aes_mul(14, a[1]) ^ aes_mul(11, a[2]) ^ aes_mul(13, a[3]);
        c[2] = aes_mul(13, a[0]) ^ aes_mul(9, a[1]) ^ aes_mul(14, a[2]) ^ aes_mul(11, a[3]);
        c[3] = aes_mul(11, a[0]) ^ aes_mul(13, a[1]) ^ aes_mul(9, a[2]) ^ aes_mul(14, a[3]);
    }
}
static void aes_key_expansion(const uint8_t key[16], uint8_t w[176]) {
    memcpy(w, key, 16);
    for (int i = 4; i < 44; i++) {
        uint8_t tmp[4]; memcpy(tmp, &w[(i - 1) * 4], 4);
        if (i % 4 == 0) {
            uint8_t t = tmp[0];
            tmp[0] = sbox[tmp[1]] ^ rcon[i / 4];
            tmp[1] = sbox[tmp[2]]; tmp[2] = sbox[tmp[3]]; tmp[3] = sbox[t];
        }
        for (int j = 0; j < 4; j++) w[i * 4 + j] = w[(i - 4) * 4 + j] ^ tmp[j];
    }
}
void aes128_cifrar_aporte(uint8_t *datos, size_t tamano, const uint8_t clave[16], int paso_a_paso) {
    uint8_t w[176]; aes_key_expansion(clave, w);
    for (size_t i = 0; i + 16 <= tamano; i += 16) {
        uint8_t *st = &datos[i];
        for (int j = 0; j < 16; j++) st[j] ^= w[j];
        for (int r = 1; r <= 10; r++) {
            aes_sub_bytes(st); aes_shift_rows(st);
            if (r < 10) aes_mix_columns(st);
            for (int j = 0; j < 16; j++) st[j] ^= w[r * 16 + j];
        }
    }
}
void aes128_descifrar_aporte(uint8_t *datos, size_t tamano, const uint8_t clave[16], int paso_a_paso) {
    uint8_t w[176]; aes_key_expansion(clave, w);
    for (size_t i = 0; i + 16 <= tamano; i += 16) {
        uint8_t *st = &datos[i];
        for (int j = 0; j < 16; j++) st[j] ^= w[160 + j];
        for (int r = 9; r >= 0; r--) {
            aes_inv_shift_rows(st); aes_inv_sub_bytes(st);
            for (int j = 0; j < 16; j++) st[j] ^= w[r * 16 + j];
            if (r > 0) aes_inv_mix_columns(st);
        }
        if (paso_a_paso && i == 0) printf("\n[AES-128] Bloque 0 descifrado.\n");
    }
}

// ---------------------------------------------------------
// 10. RSA Educativo
// ---------------------------------------------------------
static uint64_t power_mod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1; base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (__uint128_t)res * base % mod;
        base = (__uint128_t)base * base % mod;
        exp /= 2;
    }
    return res;
}
void rsa_generar_llaves(llaves_rsa_t *llaves) {
    uint64_t p = 61, q = 53;
    llaves->n = p * q;
    llaves->e = 17; llaves->d = 2753;
}
void rsa_cifrar_aporte(const uint8_t *in, uint64_t *out, size_t tamano, uint64_t e, uint64_t n) {
    for (size_t i = 0; i < tamano; i++) out[i] = power_mod(in[i], e, n);
}
void rsa_descifrar_aporte(const uint64_t *in, uint8_t *out, size_t tamano, uint64_t d, uint64_t n) {
    for (size_t i = 0; i < tamano; i++) out[i] = (uint8_t)power_mod(in[i], d, n);
}
