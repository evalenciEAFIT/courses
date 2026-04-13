# RLE — Run-Length Encoding

> Algoritmo de compresión **sin pérdida** más simple que existe.
> Inventado en los años 60, base del formato de fax y BMP comprimido.

---

## ¿Qué problema resuelve?

Cuando un archivo tiene **largas rachas del mismo byte**, es un desperdicio
almacenar cada byte individualmente. RLE los reemplaza por un par compacto:

```
  Original  →  A A A A A A A B B B B C C C C C C C C
  Bytes     →  8 veces 'A'   4 veces 'B'  8 veces 'C'
  RLE       →  [8][A] [4][B] [8][C]   =  6 bytes  (antes: 20 bytes)
```

---

## Estructura de Datos

No necesita estructuras complejas. Trabaja directamente sobre dos punteros:

```
Entrada:   uint8_t input[]   → arreglo de bytes original
Salida:    uint8_t output[]  → arreglo de pares { contador, byte }

Par RLE:
  ┌──────────┬──────────┐
  │ count    │  byte    │   cada uno = 1 byte (uint8_t)
  │ (1 byte) │ (1 byte) │   → 2 bytes por "run"
  └──────────┴──────────┘

Máximo contador: 255 (cabe en uint8_t)
Si una racha supera 255, se divide en múltiples pares.
```

### Ejemplo visual con el archivo `01_rle_ideal.txt`

```
Entrada:   A A A A A A A A B B B B B C C
           └───────────┘ └─────────┘ └─┘
Run 1: 8×A   Run 2: 5×B   Run 3: 2×C

Salida RLE (bytes):
  0x08 0x41  0x05 0x42  0x02 0x43
   8    'A'   5    'B'   2    'C'
```

---

## Algoritmo de Compresión

```
Pseudocódigo:
─────────────────────────────────────────────────
i ← 0
MIENTRAS i < tamaño_entrada:
    actual ← entrada[i]
    count  ← 1

    MIENTRAS entrada[i + count] == actual
         Y  count < 255:
        count++

    salida ← (count, actual)   ← emite el par
    i ← i + count              ← salta la racha
─────────────────────────────────────────────────
```

**Un solo pase lineal → O(n) tiempo**

---

## Algoritmo de Descompresión

```
Pseudocódigo:
─────────────────────────────────────────────────
i ← 0
MIENTRAS i < tamaño_comprimido (paso de 2 en 2):
    count ← comprimido[i]
    byte  ← comprimido[i+1]

    REPETIR count veces:
        salida ← byte       ← expande el par

    i ← i + 2
─────────────────────────────────────────────────
```

Igualmente **O(n)** — trivial y extremadamente rápido.

---

## Complejidad

| Operación   | Tiempo | Espacio auxiliar |
| ------------ | ------ | ---------------- |
| Comprimir    | O(n)   | O(1)             |
| Descomprimir | O(n)   | O(1)             |

---

## Análisis: Cuándo Funciona y Cuándo No

### Mejor caso — rachas largas

```
Entrada : 1000 × 'A'    = 1000 bytes
Salida  : [255][A] [255][A] [255][A] [235][A] = 8 bytes
Ratio   : 99.2% de ahorro
```

### Caso neutral — texto con algunas repeticiones

```
"aabbccdd"  → [2][a][2][b][2][c][2][d]  = 8 bytes
Original    = 8 bytes  → sin cambio
```

### Peor caso — sin repeticiones (texto natural)

```
"Hola"  → [1][H][1][o][1][l][1][a]  = 8 bytes
Original = 4 bytes  → ¡DUPLICA el tamaño!
```

> **Regla de oro:** RLE sólo es útil cuando el archivo tiene rachas
> de al menos 3 bytes iguales consecutivos.

---

## Aplicaciones Reales

| Sistema                     | Uso de RLE                                           |
| --------------------------- | ---------------------------------------------------- |
| **Fax (T.4 / CCITT)** | Líneas de imagen con largos tramos blancos o negros |
| **BMP**               | Compresión opcional BI_RLE8 y BI_RLE4               |
| **PCX**               | Formato de imágenes antiguo de PC                   |
| **TIFF**              | Modo PackBits (variante de RLE)                      |
| **PDF**               | Filtro RunLengthDecode                               |
| **PostScript**        | Compresión de imágenes embebidas                   |

---

## Variantes Modernas

- **RLE con flag bit:** usa 1 bit para indicar si un byte es literal o inicio de run
  (evita el peor caso duplicando el tamaño)
- **RLE diferencial:** codifica la diferencia entre bytes consecutivos
  (útil en imágenes de escala de grises)
- **PackBits (Apple):** variante que mezcla literales y runs en el mismo stream

---

## Demostración en el Programa

```
1. En el menú principal ejecuta: make run
2. Opción [7] → Crear archivo de prueba → [1] (rachas repetidas)
3. Opción [8] → Activar modo paso a paso
4. Opción [2] → Ejecutar RLE

Verás:
  - Cada "run" detectado con su carácter y contador
  - La estructura interna de pares [count][byte]
  - El ratio de compresión y el tiempo en microsegundos
```

---

## Código Clave (src/rle.c)

```c
/* Núcleo de la compresión — O(n) */
while (i < in_size) {
    uint8_t current = input[i];
    long    count   = 1;

    /* Contar la racha */
    while (i + count < in_size
           && input[i + count] == current
           && count < 255) {
        count++;
    }

    /* Emitir el par (count, byte) */
    out[out_idx++] = (uint8_t)count;
    out[out_idx++] = current;

    i += count;   /* saltar toda la racha */
}
```

---

*← [Volver al README](README.md) · Siguiente: [Huffman →](huffman.md)*
