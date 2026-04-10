# Huffman Coding

> Algoritmo de compresión **óptima sin pérdida** basado en teoría de la información.
> Propuesto por **David A. Huffman** en 1952 mientras era estudiante en el MIT.
> Base de: ZIP, GZIP, JPEG, MP3, DEFLATE, PNG.

---

## ¿Qué problema resuelve?

En todo archivo, **algunos bytes aparecen mucho más que otros**.
ASCII estándar usa 8 bits fijos para todos. ¿Por qué gastar 8 bits
en una 'z' que casi no aparece, si la 'e' podría usar sólo 2 bits?

```
Texto en español — distribución real aproximada:
  'e' → 13.7%   más frecuente
  'a' → 12.5%
  ...
  'z' →  0.1%   menos frecuente

Huffman asignará:
  'e' → 101        (3 bits)   ahorro vs 8 bits
  'a' → 110        (3 bits)
  'z' → 000010101  (9 bits)   sólo aparece rarísimo → no importa
```

El **ahorro total** viene de que los bytes comunes se codifican corto.

---

## Principio de Shannon: Entropía

La longitud óptima de un código para un símbolo con probabilidad `p` es:

```
  longitud_óptima = −log₂(p)  bits

Ejemplo:
  'e' aparece 14% del tiempo → p = 0.14
  longitud_óptima = −log₂(0.14) ≈ 2.8 bits

Huffman garantiza llegar a este óptimo (o muy cerca).
```

---

## Estructuras de Datos

### 1. Tabla de Frecuencias

```c
long freq[256];   /* índice = valor del byte (0..255) */
                  /* valor  = número de apariciones   */
```

```
Ejemplo para "AAABBC":
  freq['A'] = 3
  freq['B'] = 2
  freq['C'] = 1
  (todo lo demás = 0)
```

### 2. Nodo del Árbol de Huffman

```c
typedef struct HuffNode {
    uint8_t          symbol;   /* símbolo (solo en hojas)        */
    long             freq;     /* frecuencia del subárbol         */
    struct HuffNode *left;     /* rama izquierda → bit '0'        */
    struct HuffNode *right;    /* rama derecha   → bit '1'        */
    int              is_leaf;  /* 1 = hoja, 0 = nodo interno      */
} HuffNode;
```

```
Árbol para "AAABBC" (freq: A=3, B=2, C=1):

        (6)            ← raíz (suma total)
       /   \
    (3)     (3)        ← nodos internos
    'A'    /   \
         (2)   (1)
         'B'   'C'

Códigos resultantes:
  'A' → 0       (1 bit — el más frecuente)
  'B' → 10      (2 bits)
  'C' → 11      (2 bits — el menos frecuente)
```

### 3. Cola de Prioridad Mínima (Min-Heap)

Es la **clave de eficiencia** de Huffman. Permite obtener siempre
el nodo de menor frecuencia en O(log k):

```c
typedef struct {
    HuffNode *nodes[512];  /* array de punteros a nodos */
    int       size;        /* número de nodos activos   */
} MinHeap;
```

```
Operaciones:
  heap_insert(node)   → O(log k)  — inserta y reordena
  heap_pop_min()      → O(log k)  — extrae el mínimo
  heap_sift_up(i)     → sube el nodo hasta su posición
  heap_sift_down(i)   → hunde el nodo hasta su posición

Índice de hijo izquierdo  de i: 2i + 1
Índice de hijo derecho    de i: 2i + 2
Índice del padre          de i: (i-1) / 2
```

### 4. Tabla de Códigos

```c
typedef struct {
    char code[256];  /* cadena de '0' y '1' (null-terminated) */
    int  length;     /* longitud en bits                       */
} HuffCode;

HuffCode table[256]; /* una entrada por símbolo posible (0..255) */
```

---

## El Algoritmo en 3 Fases

### Fase 1 — Contar Frecuencias: O(n)

```c
for (long i = 0; i < in_size; i++)
    freq[input[i]]++;
```

```
Resultado para "BANANA":
  freq['B'] = 1
  freq['A'] = 3
  freq['N'] = 2
```

---

### Fase 2 — Construir el Árbol con Min-Heap: O(k log k)

```
Inicializar heap con un nodo hoja por cada símbolo con freq > 0.

Estado inicial del heap (heap mínimo por freq):
  [B:1]  [N:2]  [A:3]

Iteración 1:
  Extraer los 2 mínimos: B:1 y N:2
  Crear nodo interno:    (BN):3
  Reinsertar en heap:    [(BN):3]  [A:3]

Iteración 2:
  Extraer los 2 mínimos: (BN):3 y A:3
  Crear nodo interno:    (BNA):6  ← raíz
  El heap tiene 1 nodo → FIN

Árbol resultante:
         (6)
        /   \
      (3)    A:3
     /   \
   B:1   N:2

Códigos:
  A → 1    (1 bit)  ← más frecuente
  B → 00   (2 bits)
  N → 01   (2 bits)
```

---

### Fase 3 — Generar Códigos: O(k)

Recorrido recursivo del árbol (DFS):

- Ir a la rama **izquierda** → agregar '0' al código
- Ir a la rama **derecha**  → agregar '1' al código
- Al llegar a una **hoja**  → guardar el código acumulado

```c
void build_codes(HuffNode *node, char *path, int depth, HuffCode table[]) {
    if (node->is_leaf) {
        table[node->symbol].code   = path;  /* código acumulado */
        table[node->symbol].length = depth;
        return;
    }
    path[depth] = '0';
    build_codes(node->left,  path, depth+1, table);
    path[depth] = '1';
    build_codes(node->right, path, depth+1, table);
}
```

---

### Fase 4 — Empaquetar Bits: O(n)

Los códigos son cadenas de '0'/'1'. Se empaquetan en bytes:

```
"BANANA" con códigos: A→1, B→00, N→01

  B   A   N   A   N   A
  00  1   01  1   01  1

Bits: 0 0 1 0 1 1 0 1 1
      └───byte 1──┘└──...

Byte 0:  0011 0110  → 0x36
Byte 1:  1???  ???? → padding hasta completar byte
```

---

## Descompresión

Se recorre el árbol bit a bit:

```
Para cada bit en el stream comprimido:
    bit = 0 → ir a rama izquierda
    bit = 1 → ir a rama derecha

    si llegamos a una hoja:
        emitir el símbolo de la hoja
        volver a la raíz y continuar
```

> **Requisito:** el árbol debe transmitirse junto con los datos comprimidos
> para poder descomprimir. En este proyecto se conserva en memoria (RAM).

---

## Complejidad

| Operación       | Tiempo                   | Espacio            |
| ---------------- | ------------------------ | ------------------ |
| Frecuencias      | O(n)                     | O(k)               |
| Construir árbol | O(k log k)               | O(k)               |
| Generar códigos | O(k)                     | O(k)               |
| Comprimir        | O(n)                     | O(n)               |
| Descomprimir     | O(n)                     | O(n)               |
| **Total**  | **O(n + k log k)** | **O(n + k)** |

Donde `n` = tamaño del archivo, `k` = símbolos únicos (máx 256).

---

## Propiedades Garantizadas

1. **Optimalidad:** Ningún otro código de longitud variable por símbolo
   puede comprimir mejor que Huffman (Teorema de Shannon-Fano-Elias).
2. **Libre de prefijos:** Ningún código es prefijo de otro.
   Esto permite decodificar el stream de bits sin ambigüedad.
3. **Sin pérdida:** El archivo descomprimido es **idéntico** al original.

---

## Cuándo Funciona Bien y Cuándo No

| Caso                                  | Resultado                                  |
| ------------------------------------- | ------------------------------------------ |
| Texto natural (muchas letras comunes) | ✅ 30–60% ahorro                          |
| ADN (solo A, T, C, G)                 | ✅ ~75% ahorro (4 símbolos → 2 bits c/u) |
| Datos ya comprimidos (JPEG, ZIP)      | ❌ Sin mejora (distribución uniforme)     |
| Datos aleatorios                      | ❌ Puede crecer ligeramente                |

---

## Aplicaciones Reales

| Sistema                  | Uso de Huffman                     |
| ------------------------ | ---------------------------------- |
| **ZIP / DEFLATE**  | Segunda etapa (después de LZ77)   |
| **JPEG**           | Codificación de coeficientes DCT  |
| **MP3**            | Codificación de datos de audio    |
| **PNG**            | Dentro de DEFLATE (LZ77 + Huffman) |
| **HTTP/2 (HPACK)** | Compresión de cabeceras           |

---

## Demostración en el Programa

```
1. make run
2. Opción [7] → [2] Texto natural (ideal para Huffman)
3. Opción [8] → Activar modo paso a paso
4. Opción [3] → Ejecutar Huffman

Verás paso a paso:
  ① Tabla de frecuencias con histograma
  ② Fusiones del Min-Heap (cuántas iteraciones)
  ③ Árbol dibujado en la terminal (girado 90°)
  ④ Tabla de códigos: cada símbolo con su código binario
  ⑤ Codificación byte a byte del archivo
```

---

*← [RLE](rle.md) · [Volver al README](README.md) · Siguiente: [LZ77 →](lz77.md)*
