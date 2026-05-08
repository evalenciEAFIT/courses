# Explicación Detallada de los Métodos de Encriptación Implementados

Este documento describe la teoría, la mecánica criptográfica y el código en C utilizado en el proyecto para procesar archivos mediante aportes (chunks/streaming).

---

## 1. Cifrado César

### Teoría
El cifrado César es uno de los métodos de cifrado más antiguos y sencillos. Es un tipo de **sustitución monoalfabética** en el que cada carácter del texto original se desplaza un número fijo de posiciones en el alfabeto o en el rango de bytes (0-255).

*   **Fórmula de Cifrado**: $C = (P + k) \pmod{256}$
*   **Fórmula de Descifrado**: $P = (C - k) \pmod{256}$

Donde $P$ es el byte original, $C$ es el byte cifrado, y $k$ es el desplazamiento.

### Código Empleado
```c
void cesar_cifrar_descifrar(uint8_t *datos, size_t tamano, int desplazamiento, int descifrar, int paso_a_paso) {
    int desp = (desplazamiento % 256 + 256) % 256;
    if (descifrar) {
        desp = 256 - desp;
    }

    for (size_t i = 0; i < tamano; i++) {
        uint8_t original = datos[i];
        datos[i] = (uint8_t)((datos[i] + desp) % 256);
    }
}
```

---

## 2. Cifrado Vigenère

### Teoría
El cifrado Vigenère es una **sustitución polialfabética** que utiliza una serie de diferentes cifrados César basados en las letras de una palabra clave. Al usar una clave que cambia el desplazamiento en cada posición, se rompe el análisis de frecuencias simple característico del cifrado César.

*   **Fórmula de Cifrado**: $C_i = (P_i + K_{i \bmod m}) \pmod{256}$
*   **Fórmula de Descifrado**: $P_i = (C_i - K_{i \bmod m} + 256) \pmod{256}$

Donde $m$ es la longitud de la clave.

### Código Empleado
```c
void vigenere_cifrar_descifrar(uint8_t *datos, size_t tamano, const uint8_t *clave, size_t longitud_clave, int descifrar, int paso_a_paso) {
    if (longitud_clave == 0) return;

    for (size_t i = 0; i < tamano; i++) {
        uint8_t original = datos[i];
        uint8_t c = clave[i % longitud_clave];
        if (descifrar) {
            datos[i] = (uint8_t)((datos[i] - c + 256) % 256);
        } else {
            datos[i] = (uint8_t)((datos[i] + c) % 256);
        }
    }
}
```

---

## 3. Cifrado XOR

### Teoría
El cifrado XOR aplica la operación lógica booleana **O Exclusivo** bit a bit entre los bytes del archivo original y una clave repetitiva. Es simétrico: aplicar la operación dos veces con la misma clave devuelve el texto original.

*   **Fórmula**: $C = P \oplus K$
*   **Descifrado**: $P = C \oplus K$

### Código Empleado
```c
void xor_cifrar_descifrar_aporte(uint8_t *datos, size_t tamano, const uint8_t *clave, size_t longitud_clave, size_t *indice_clave, int paso_a_paso) {
    if (longitud_clave == 0) return;

    for (size_t i = 0; i < tamano; i++) {
        uint8_t original = datos[i];
        uint8_t k = clave[*indice_clave];
        datos[i] ^= k;
        *indice_clave = (*indice_clave + 1) % longitud_clave;
    }
}
```

---

## 4. Cifrado de Flujo RC4

### Teoría
RC4 es un cifrado de flujo diseñado por Ron Rivest. Genera una secuencia de bits pseudoaleatorios (keystream) a partir de un vector de estado $S$ de 256 bytes inicializado con la clave original. Los bytes de la secuencia se combinan con el texto en claro usando XOR.

### Código Empleado
```c
void rc4_cifrar_aporte(estado_rc4_t *estado, uint8_t *datos, size_t tamano, int paso_a_paso) {
    int i = estado->i;
    int j = estado->j;

    for (size_t k = 0; k < tamano; k++) {
        i = (i + 1) % 256;
        j = (j + estado->s[i]) % 256;
        
        uint8_t temp = estado->s[i];
        estado->s[i] = estado->s[j];
        estado->s[j] = temp;
        
        uint8_t K = estado->s[(estado->s[i] + estado->s[j]) % 256];
        datos[k] ^= K;
    }
    estado->i = i;
    estado->j = j;
}
```

---

## 5. Cifrado por Bloques TEA (Tiny Encryption Algorithm)

### Teoría
TEA es un cifrado por bloques notable por su simplicidad de diseño y alta seguridad. Opera en bloques de 64 bits (8 bytes) usando una clave de 128 bits (16 bytes). Utiliza una red de Feistel con 32 rondas completas de mezcla, basándose en la adición, el desplazamiento y operaciones XOR.

### Código Empleado
```c
void tea_cifrar_bloque(uint32_t v[2], const uint32_t k[4]) {
    uint32_t v0 = v[0], v1 = v[1], suma = 0, i;
    uint32_t delta = 0x9e3779b9;
    for (i = 0; i < 32; i++) {
        suma += delta;
        v0 += ((v1 << 4) + k[0]) ^ (v1 + suma) ^ ((v1 >> 5) + k[1]);
        v1 += ((v0 << 4) + k[2]) ^ (v0 + suma) ^ ((v0 >> 5) + k[3]);
    }
    v[0] = v0; v[1] = v1;
}
```

---

## 6. Cifrado Atbash

### Teoría
El cifrado Atbash es un método de **sustitución simétrica** en el que se invierte el alfabeto. Para archivos binarios con rango de bytes (0 a 255), el valor se transforma en su complemento exacto de 255.

*   **Fórmula**: $C = 255 - P$
*   **Fórmula**: $P = 255 - C$

### Código Empleado
```c
void atbash_cifrar_descifrar(uint8_t *datos, size_t tamano, int paso_a_paso) {
    for (size_t i = 0; i < tamano; i++) {
        uint8_t original = datos[i];
        datos[i] = 255 - original;
    }
}
```

---

## 7. Cifrado de Transposición por Bloques

### Teoría
Altera la posición de los bytes en el archivo original sin cambiar sus valores. Trabaja por bloques de 4 bytes donde se realiza una rotación o permutación a la derecha o izquierda.

*   **Fórmula (Cifrado)**: $[b_0, b_1, b_2, b_3] \longrightarrow [b_3, b_0, b_1, b_2]$
*   **Fórmula (Descifrado)**: $[b_3, b_0, b_1, b_2] \longrightarrow [b_0, b_1, b_2, b_3]$

### Código Empleado
```c
void transposicion_bloque_cifrar_descifrar(uint8_t *datos, size_t tamano, int descifrar, int paso_a_paso) {
    for (size_t i = 0; i + 4 <= tamano; i += 4) {
        uint8_t b0 = datos[i];
        uint8_t b1 = datos[i+1];
        uint8_t b2 = datos[i+2];
        uint8_t b3 = datos[i+3];

        if (descifrar) {
            datos[i]   = b1;
            datos[i+1] = b2;
            datos[i+2] = b3;
            datos[i+3] = b0;
        } else {
            datos[i]   = b3;
            datos[i+1] = b0;
            datos[i+2] = b1;
            datos[i+3] = b2;
        }
    }
}
```

---

## 8. Cifrado Vernam (OTP)

### Teoría
Es un cifrado de flujo de **secreto perfecto** (One-Time Pad). Cada byte del texto en claro se combina mediante XOR con un byte de una secuencia de clave aleatoria única que es tan larga como el mensaje. En este proyecto se simula la secuencia aleatoria mediante un LCG (Linear Congruential Generator) a partir de una semilla numérica secreta.

*   **Fórmula**: $C_i = P_i \oplus \text{Mascara}_i$

### Código Empleado
```c
void vernam_cifrar_descifrar(uint8_t *datos, size_t tamano, uint32_t semilla, int paso_a_paso) {
    uint32_t lcg = semilla;
    for (size_t i = 0; i < tamano; i++) {
        lcg = (lcg * 1103515245 + 12345) & 0x7fffffff;
        uint8_t mascara = (uint8_t)(lcg % 256);
        datos[i] ^= mascara;
    }
}
```

---

## 9. Cifrado AES-128 (Advanced Encryption Standard)

### Teoría
AES es el estándar de cifrado moderno más utilizado a nivel mundial. Es un cifrado de bloques simétrico que procesa datos en bloques de 128 bits (16 bytes) mediante 10 rondas de transformación (para la versión de 128 bits). Cada ronda consiste en cuatro pasos:
1.  **SubBytes**: Sustitución no lineal usando una tabla (S-Box).
2.  **ShiftRows**: Permutación cíclica de las filas del estado.
3.  **MixColumns**: Mezcla de las columnas mediante multiplicación en campos de Galois (GF(2^8)).
4.  **AddRoundKey**: Operación XOR con una subclave derivada de la clave principal.

### Código Empleado (Simplificado)
```c
void aes128_cifrar_aporte(uint8_t *datos, size_t tamano, const uint8_t clave[16], int paso_a_paso) {
    uint8_t w[176]; aes_key_expansion(clave, w);
    for (size_t i = 0; i + 16 <= tamano; i += 16) {
        uint8_t *st = &datos[i];
        for (int j = 0; j < 16; j++) st[j] ^= w[j]; // AddRoundKey inicial
        for (int r = 1; r <= 10; r++) {
            aes_sub_bytes(st);
            aes_shift_rows(st);
            if (r < 10) aes_mix_columns(st);
            for (int j = 0; j < 16; j++) st[j] ^= w[r * 16 + j]; // AddRoundKey
        }
    }
}
```

---

## 10. Cifrado RSA (Rivest-Shamir-Adleman)

### Teoría
RSA es el primer y más conocido algoritmo de **cifrado asimétrico**. Se basa en la dificultad matemática de factorizar el producto de dos números primos grandes. A diferencia de los métodos anteriores, utiliza un par de llaves:
*   **Llave Pública ($e, n$):** Se usa para cifrar.
*   **Llave Privada ($d, n$):** Se usa para descifrar.

**Funcionamiento:**
1.  Se eligen dos primos $p$ y $q$.
2.  Se calcula $n = p \cdot q$ y $\phi(n) = (p-1)(q-1)$.
3.  Se elige $e$ tal que sea coprimo con $\phi(n)$.
4.  Se calcula $d$ como el inverso modular de $e \pmod{\phi(n)}$.

*   **Cifrado**: $C = M^e \pmod{n}$
*   **Descifrado**: $M = C^d \pmod{n}$

### Código Empleado (Modular Exponentiation)
```c
static uint64_t power_mod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1; base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (__uint128_t)res * base % mod;
        base = (__uint128_t)base * base % mod;
        exp /= 2;
    }
    return res;
}
```
