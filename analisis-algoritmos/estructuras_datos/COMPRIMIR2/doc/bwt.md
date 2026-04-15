# BWT — Burrows-Wheeler Transform

> Preprocesamiento para compresión: reordena los símbolos para agrupar repetidos.

## Idea principal

Burrows-Wheeler Transform (BWT) no comprime por sí sola. Lo que hace es tomar un bloque
completo de texto y ordenar todas sus rotaciones circulares. La salida es la última columna
de esa matriz ordenada y un índice que indica dónde quedó la rotación original.

El resultado tiene esta propiedad útil:
- símbolos iguales tienden a agruparse
- facilita etapas posteriores como RLE o Huffman

## ¿Por qué es útil?

BWT es especialmente valiosa cuando se usa en combinación con otros algoritmos.
Después de la transformada, una secuencia como `"banana"` produce una salida con
repeticiones más fáciles de comprimir.

Ejemplo simplificado:

```
Texto original: banana$
Rotaciones ordenadas:
  a$banan
  ana$ban
  banana$
  na$bana
  nana$ba
  naba$an
Última columna: nnaaba
Índice original: 2
```

La transformada resultante es `nnaaba` junto con el índice `2`.

## En este proyecto

- La implementación se encuentra en `src/bwt.c` y `src/bwt.h`.
- El algoritmo se agrega al menú principal como una opción independiente.
- También se utiliza para comparar ratios y tiempos de compresión junto con RLE,
  Huffman y LZ77.

## Características

- **Transformación:** O(n log n) por la ordenación de rotaciones
- **Espacio:** O(n)
- **Salida:** último carácter de cada rotación ordenada + índice original
- **Uso pedagógico:** demuestra el efecto de reordenar datos antes de aplicar
  codificadores de entropía o RLE

## Limitaciones

- No es una compresión final por sí misma
- Sus beneficios dependen de un codificador posterior
- El bloque debe procesarse completo, por lo que no es streaming puro

## Cómo probarlo

1. Compila el proyecto con `make`
2. Ejecuta `./bin/comprimir`
3. Carga un archivo desde `samples/fuente/`
4. Elige `5. BWT — Burrows-Wheeler Transform`

Se generarán también archivos en:

- `samples/comprimidos/out_bwt.bin`
- `samples/descomprimidos/out_bwt_decomp.bin`
