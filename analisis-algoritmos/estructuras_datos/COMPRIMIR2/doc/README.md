# COMPRIMIR — Laboratorio de Algoritmos de Compresión

> **Proyecto pedagógico EDA** | Algoritmos: RLE · Huffman · LZ77 · LZ78 · LZW · LZ4 · Deflate · Zstd · BWT
> Lenguaje: C11 · Linux · Terminal interactiva con colores ANSI

---

## Estructura del Proyecto

```
COMPRIMIR/
├── Makefile                     ← Compilación inteligente
├── src/                         ← Código fuente
│   ├── main.c                   ← Menú TUI principal
│   ├── utils.h / utils.c        ← Timer, colores, tabla comparativa
│   ├── rle.h   / rle.c          ← Algoritmo RLE
│   ├── huffman.h / huffman.c    ← Algoritmo Huffman (Min-Heap + árbol)
│   ├── lz.h    / lz.c           ← Algoritmo LZ77 (ventana deslizante)
│   ├── lz78.h  / lz78.c         ← Algoritmo LZ78 (diccionario de frases)
│   ├── lzw.h   / lzw.c          ← Algoritmo LZW (códigos de diccionario)
│   └── bwt.h   / bwt.c          ← Burrows-Wheeler Transform (preprocesamiento)
├── bin/                         ← Objetos .o + ejecutable compilado
│   └── comprimir                ← Binario final
├── app/
│   └── comprimir                ← Script lanzador (wrapper bash)
├── samples/
│   ├── fuente/                  ← Archivos de entrada para pruebas
│   │   ├── 01_rle_ideal.txt     ← Grandes rachas repetidas → ideal RLE
│   │   ├── 02_adn.txt           ← Secuencia ADN (A,T,C,G)
│   │   ├── 03_quijote.txt       ← Texto literario → ideal Huffman
│   │   ├── 04_codigo_c.txt      ← Código fuente → ideal LZ77
│   │   ├── 05_fibonacci.txt     ← Patrones numéricos
│   │   └── 06_aleatorio.txt     ← Peor caso (sin compresibilidad)
│   ├── comprimidos/             ← Salidas binarias de compresión
│   └── descomprimidos/          ← Salidas restauradas (verificación)
└── doc/                         ← Documentación de los algoritmos
    ├── README.md                ← Este archivo
    ├── rle.md                   ← Teoría y ejemplos RLE
    ├── huffman.md               ← Teoría y ejemplos Huffman
    ├── lz77.md                  ← Teoría y ejemplos LZ77
    ├── lz78.md                  ← Teoría y ejemplos LZ78
    ├── lzw.md                   ← Teoría y ejemplos LZW    ├── lz4.md                   ← Teoría y ejemplos LZ4
    ├── deflate.md               ← Teoría y ejemplos Deflate
    ├── zstd.md                  ← Teoría y ejemplos Zstandard    └── bwt.md                   ← Teoría y ejemplos BWT
```

---

## Compilar y Ejecutar

```bash
# Compilar (genera bin/comprimir y app/comprimir)
make

# Compilar y ejecutar directamente
make run

# Compilar con AddressSanitizer (detecta fugas de memoria)
make debug

# Analizar memoria con Valgrind
make valgrind

# Limpiar binarios y salidas
make clean

# Ver todos los comandos disponibles
make help
```

---

## Cómo Usar la Aplicación

Al ejecutar `./bin/comprimir` se muestra el **menú principal TUI** organizado en secciones pedagógicas:

```
  ┌─ GESTIÓN DE DATOS ──────────────────────────────────────┐
  │  [1] Seleccionar / cambiar archivo                      │
  │  [14] Crear archivo de prueba                           │
  └─────────────────────────────────────────────────────────┘

  ┌─ ALGORITMOS INDIVIDUALES ──────────────────────────────┐
  │  [2] RLE — Run-Length Encoding (rachas repetidas)      │
  │  [3] Huffman — Codificación de entropía                │
  │  [4] LZ77 — Ventana deslizante                          │
  │  [5] LZ78 — Diccionario de frases                       │
  │  [6] LZW — Diccionario dinámico                         │
  │  [7] LZ4 — Compresión rápida                            │
  │  [8] Deflate — LZ77 + Huffman (como ZIP)               │
  │  [9] Zstd — BWT + Huffman (como Zstandard)             │
  │  [10] BWT — Burrows-Wheeler Transform                   │
  └─────────────────────────────────────────────────────────┘

  ┌─ COMPARACIONES ────────────────────────────────────────┐
  │  [11] Comparar todos los algoritmos                    │
  │  [12] Comparar algoritmos LZ                           │
  └─────────────────────────────────────────────────────────┘

  ┌─ HERRAMIENTAS PEDAGÓGICAS ─────────────────────────────┐
  │  [13] Ver estructuras de datos                         │
  │  [15] Activar/Desactivar modo paso a paso              │
  └─────────────────────────────────────────────────────────┘

  [0] Salir
```

### Flujo de trabajo típico

```
1 → Seleccionar un archivo desde samples/fuente/
    (o usar [14] para generar uno de prueba específico por algoritmo)

15 → Activar modo paso a paso (opcional)
    Cada algoritmo explica cada decisión en tiempo real

2-10 → Ejecutar algoritmos individuales
        Se muestra: ratio de compresión, tiempo, verificación de integridad
        y explicaciones pedagógicas del funcionamiento

11 → Comparar todos los algoritmos simultáneamente
    Tabla con métricas, ganadores en ratio y velocidad

12 → Comparar solo algoritmos LZ
    Enfoque en variantes de diccionario y ventana
```

---

## Archivos de Muestra Incluidos

| Archivo              | Descripción                        | Mejor Algoritmo     |
| -------------------- | ----------------------------------- | ------------------- |
| `01_rle_ideal.txt` | Grandes rachas del mismo carácter  | **RLE**       |
| `02_adn.txt`       | Secuencia de ADN (A, T, C, G)       | RLE / Huffman       |
| `03_quijote.txt`   | Fragmento del Quijote               | **Huffman**   |
| `04_codigo_c.txt`  | Código fuente C                    | **LZ77**      |
| `05_fibonacci.txt` | Secuencia de Fibonacci con patrones | **LZ78**     |
| `06_aleatorio.txt` | Datos sin estructura                | Ninguno (peor caso) |
| `07_lz78.txt`      | Frases crecientes y repetidas       | **LZ78**     |
| `08_lzw.txt`       | Repeticiones con extensión de frases| **LZW**      |
| `09_bwt.txt`       | Texto rotacional con patrones       | **BWT**      |

---

## Los Algoritmos

### RLE — Run-Length Encoding

**Idea:** Reemplaza rachas de bytes iguales por un par `[cantidad][byte]`.

```
Original :  AAABBBBBCCDDDDDD   (16 bytes)
Codificado: 3A 5B 2C 6D        (8 bytes)  → 50% ahorro
```

- **Mejor caso:** imágenes con grandes áreas de color uniforme
- **Peor caso:** texto sin repeticiones (¡dobla el tamaño!)
- **Complejidad:** O(n) tiempo, O(n) espacio

---

### Huffman Coding

**Idea:** Asigna códigos binarios cortos a los símbolos más frecuentes.

```
Ejemplo (texto en español):
  'e' → 101        (3 bits, muy frecuente)
  'a' → 110        (3 bits)
  'z' → 00001010   (8 bits, poco frecuente)
```

Fases:

1. Contar frecuencias de cada byte
2. Construir árbol binario con Min-Heap (fusionar los 2 nodos menores)
3. Asignar código 0/1 según rama izquierda/derecha del árbol

- **Mejor caso:** texto con distribución desigual de caracteres
- **Complejidad:** O(n + k log k), k = símbolos únicos

---

### LZ77 — Lempel-Ziv 1977

**Idea:** Ventana deslizante — si una cadena ya apareció en el historial,
emite una referencia `(distancia, longitud)` en lugar de repetir los bytes.

```
Historial (ventana)    Lookahead
[...abcdefgh abcde]    [fgh...]
                            ↑ match de longitud 3 a distancia 9
Token: (9, 3, nextbyte)
```

- **Base de:** gzip, zlib, PNG, ZIP, DEFLATE
- **Mejor caso:** texto o código con frases y palabras repetidas
- **Complejidad:** O(n × W), W = tamaño de ventana

---

### LZ78 — Lempel-Ziv 1978

**Idea:** Construye un diccionario de frases únicas. Cuando encuentra una frase nueva,
la añade al diccionario y emite el índice de la frase previa más su extensión.

```
Diccionario inicial: vacío
Entrada: "ABAAB"
Procesamiento:
  "A" → nueva, emitir (0, 'A'), añadir "A" como frase 1
  "B" → nueva, emitir (0, 'B'), añadir "B" como frase 2
  "A" → existe (frase 1), continuar
  "AB" → nueva, emitir (1, 'B'), añadir "AB" como frase 3
  "A" → existe (frase 1), continuar
  "AB" → existe (frase 3), fin
```

- **Base de:** GIF, TIFF, algunos compresores antiguos
- **Mejor caso:** texto con frases crecientes y repetidas
- **Complejidad:** O(n) — diccionario crece linealmente

---

### LZW — Lempel-Ziv-Welch

**Idea:** Diccionario inicial con 256 símbolos (bytes). A medida que procesa,
combina símbolos en frases más largas y emite códigos de diccionario.

```
Diccionario inicial: 0-255 = bytes individuales
Entrada: "TOBEORNOTTOBE"
Procesamiento:
  "T" → emitir 84, añadir "TO" como 256
  "O" → emitir 79, añadir "OB" como 257
  "B" → emitir 66, añadir "BE" como 258
  "E" → emitir 69, añadir "EO" como 259
  "O" → emitir 79, añadir "OR" como 260
  "R" → emitir 82, añadir "RN" como 261
  "N" → emitir 78, añadir "NO" como 262
  "O" → emitir 79, añadir "OT" como 263
  "T" → emitir 84, añadir "TT" como 264
  "T" → existe "TT" (264), continuar
  "TO" → nueva, emitir 264, añadir "TOB" como 265
  ...y así sucesivamente
```

- **Base de:** GIF, TIFF, Unix compress, PDF
- **Mejor caso:** texto con frases repetidas y extensiones
- **Complejidad:** O(n) — diccionario adaptativo

---

### LZ4 — Compresión Rápida

**Idea:** Algoritmo LZ simplificado enfocado en velocidad. Busca matches de al menos 4 bytes
y emite referencias cortas sin codificación de entropía.

```
Entrada: "ABCDABCDABCD"
Compresión:
  Emitir literal "ABCD"
  Encontrar match "ABCD" a distancia 4, longitud 4
  Emitir token: marcador + len=4 + dist=4
  Repetir para el resto
```

- **Base de:** Compresión en tiempo real, bases de datos
- **Mejor caso:** datos con repeticiones locales
- **Complejidad:** O(n) — muy rápido

---

### Deflate — LZ77 + Huffman

**Idea:** Combina LZ77 para encontrar patrones con Huffman para codificar los tokens resultantes.
Es el algoritmo detrás de ZIP, gzip, PNG.

```
Fases:
1. LZ77: convertir datos en tokens (distancia, longitud, literal)
2. Huffman: codificar tokens con códigos variables por frecuencia

Ejemplo:
  Datos: "ABCDABCD"
  LZ77: literal "ABCD" + match(dist=4, len=4)
  Huffman: codificar "ABCD" y el match con códigos óptimos
```

- **Base de:** ZIP, gzip, zlib, HTTP compression
- **Mejor caso:** archivos generales con patrones y entropía desigual
- **Complejidad:** O(n) + O(k log k)

---

### Zstd — BWT + Huffman

**Idea:** Usa Burrows-Wheeler Transform para reordenar datos agrupando símbolos similares,
luego aplica Huffman. Moderno y eficiente.

```
Ejemplo simplificado:
Texto: "banana"
BWT: ordenar rotaciones → última columna "nnbaaa"
Huffman: codificar "nnbaaa" (muchos 'n' y 'a' juntos)
```

- **Base de:** Compresión moderna (Facebook, Linux kernel)
- **Mejor caso:** texto repetitivo que se beneficia de reordenamiento
- **Complejidad:** O(n log n) + O(n)

---

### BWT — Burrows-Wheeler Transform

**Idea:** Reordena rotaciones circulares del bloque y extrae la última columna.
Esto agrupa símbolos similares para que los codificadores posteriores compriman mejor.

Ejemplo breve:

```
Texto: banana$
Salida BWT: nnaaba
Índice original: 2
```

- **Uso:** preprocesamiento antes de RLE / Huffman
- **Mejor caso:** datos con estructuras repetidas que se benefician de reordenamiento
- **Complejidad:** O(n log n) por la ordenación de rotaciones

---

## Salidas Generadas

Cada vez que se comprime un archivo se generan:

```
samples/comprimidos/out_rle.bin          ← datos RLE comprimidos
samples/comprimidos/out_huffman.bin      ← datos Huffman comprimidos
samples/comprimidos/out_lz77.bin         ← tokens LZ77 empaquetados
samples/comprimidos/out_lz78.bin         ← tokens LZ78 empaquetados
samples/comprimidos/out_lzw.bin          ← tokens LZW empaquetados
samples/comprimidos/out_lz4.bin          ← datos LZ4 comprimidos
samples/comprimidos/out_deflate.bin      ← datos Deflate comprimidos
samples/comprimidos/out_zstd.bin         ← datos Zstd comprimidos
samples/comprimidos/out_bwt.bin          ← transformación BWT

samples/descomprimidos/out_rle_decomp.bin       ← verificación RLE
samples/descomprimidos/out_huffman_decomp.bin   ← verificación Huffman
samples/descomprimidos/out_lz77_decomp.bin      ← verificación LZ77
samples/descomprimidos/out_lz78_decomp.bin      ← verificación LZ78
samples/descomprimidos/out_lzw_decomp.bin       ← verificación LZW
samples/descomprimidos/out_lz4_decomp.bin       ← verificación LZ4
samples/descomprimidos/out_deflate_decomp.bin   ← verificación Deflate
samples/descomprimidos/out_zstd_decomp.bin      ← verificación Zstd
samples/descomprimidos/out_bwt_decomp.bin       ← verificación BWT
```

> La aplicación verifica automáticamente que el archivo descomprimido
> es **idéntico byte a byte** al original. Se muestra `✔ OK` o `✘ FALLO`.

---

## Ejemplo de Sesión Completa

```bash
make run

# En el menú:
# [14] → Crear archivo de prueba → [2] Texto natural (Huffman)
# [15] → Activar paso a paso
# [3] → Ejecutar Huffman
#   → Ver tabla de frecuencias
#   → Ver construcción del árbol
#   → Ver códigos generados
# [11] → Comparar todos los algoritmos
#   → Ver tabla comparativa con métricas
#   → Identificar ganadores en ratio y velocidad
```
#   → Ver tabla de códigos
#   → Ver codificación byte a byte
# [5] → Comparar los 3 algoritmos
#   → Tabla con ratios, tiempos y ganadores
```

---

## Requisitos

| Herramienta | Versión mínima   |
| ----------- | ------------------ |
| `gcc`     | 9.0+               |
| `make`    | 4.0+               |
| Terminal    | Soporte ANSI/UTF-8 |
| SO          | Linux / macOS      |

---

*Proyecto para la asignatura Estructuras de Datos y Algoritmos (EDA)*
