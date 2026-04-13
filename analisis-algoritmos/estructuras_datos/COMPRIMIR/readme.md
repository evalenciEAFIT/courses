# COMPRIMIR — Laboratorio de Algoritmos de Compresión

> **Proyecto pedagógico EDA** | Algoritmos: RLE · Huffman · LZ77
> Lenguaje: C11 · Linux 

---

## Estructura del Proyecto

```
COMPRIMIR/
├── README.md                    ← Este archivo
├── Makefile                     ← Compilación inteligente
├── src/                         ← Código fuente
│   ├── main.c                   ← Menú TUI principal
│   ├── utils.h / utils.c        ← Timer, colores, tabla comparativa
│   ├── rle.h   / rle.c          ← Algoritmo RLE
│   ├── huffman.h / huffman.c    ← Algoritmo Huffman (Min-Heap + árbol)
│   └── lz.h    / lz.c           ← Algoritmo LZ77 (ventana deslizante)
├── samples/
│   ├── fuente/                  ← Archivos de entrada para pruebas
│   │   ├── 01_rle_ideal.txt     ← Grandes rachas repetidas → ideal RLE
│   │   ├── 02_adn.txt           ← Secuencia ADN (A,T,C,G)
│   ├── comprimidos/             ← Salidas binarias de compresión
│   └── descomprimidos/          ← Salidas restauradas (verificación)
└── doc/                         ← Documentación de los algoritmos
    ├── rle.md                   ← Teoría y ejemplos RLE
    ├── huffman.md               ← Teoría y ejemplos Huffman
    └── lz77.md                  ← Teoría y ejemplos LZ77
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

Al ejecutar `./bin/comprimir` se muestra el **menú principal TUI**:

```
  [1] Seleccionar / cambiar archivo
  [2] RLE — Run-Length Encoding
  [3] Huffman Coding
  [4] LZ77 (ventana deslizante)
  [5] Comparar los 3 algoritmos
  [6] Ver estructura de datos
  [7] Crear archivo de prueba
  [8] Activar/Desactivar paso a paso
  [0] Salir
```

### Flujo de trabajo típico

```
1 → Cargar un archivo desde samples/fuente/
    (o usar [7] para generar uno de prueba)

8 → Activar modo paso a paso (opcional)
    El algoritmo explicará cada decisión en tiempo real

2/3/4 → Ejecutar un algoritmo individual
        Se muestra: ratio de compresión, tiempo, verificación de integridad

5 → Comparar los 3 algoritmos simultáneamente
    Se genera una tabla con ganadores en ratio y velocidad
```

---

## Archivos de Muestra Incluidos

| Archivo              | Descripción                        | Mejor Algoritmo     |
| -------------------- | ----------------------------------- | ------------------- |
| `01_rle_ideal.txt` | Grandes rachas del mismo carácter  | **RLE**       |
| `02_adn.txt`       | Secuencia de ADN (A, T, C, G)       | RLE / Huffman       |
| `03_quijote.txt`   | Fragmento del Quijote               | **Huffman**   |
| `04_codigo_c.txt`  | Código fuente C                    | **LZ77**      |
| `05_fibonacci.txt` | Secuencia de Fibonacci con patrones | LZ77                |
| `06_aleatorio.txt` | Datos sin estructura                | Ninguno (peor caso) |

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

## Salidas Generadas

Cada vez que se comprime un archivo se generan:

```
samples/comprimidos/out_rle.bin          ← datos RLE comprimidos
samples/comprimidos/out_huffman.bin      ← datos Huffman comprimidos
samples/comprimidos/out_lz77.bin         ← tokens LZ77 empaquetados

samples/descomprimidos/out_rle_decomp.bin       ← verificación RLE
samples/descomprimidos/out_huffman_decomp.bin   ← verificación Huffman
samples/descomprimidos/out_lz77_decomp.bin      ← verificación LZ77
```

> La aplicación verifica automáticamente que el archivo descomprimido
> es **idéntico byte a byte** al original. Se muestra `✔ OK` o `✘ FALLO`.

---

## Ejemplo de Sesión Completa

```bash
make run

# En el menú:
# [7] → Crear archivo de prueba → [2] Texto natural (Huffman)
# [8] → Activar paso a paso
# [3] → Ejecutar Huffman
#   → Ver tabla de frecuencias
#   → Ver construcción del árbol
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


