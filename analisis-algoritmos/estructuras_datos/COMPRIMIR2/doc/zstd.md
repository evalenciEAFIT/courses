# Zstandard (Zstd) — BWT + Huffman

## Concepto

Zstandard es un algoritmo moderno que combina la transformación Burrows-Wheeler (BWT) con codificación Huffman. BWT reordena los datos para agrupar símbolos similares, facilitando una compresión posterior más efectiva. Es el sucesor espiritual de LZMA/xz, con mejor velocidad.

## Mecánica

1. **Fase BWT:** Crea rotaciones del texto, las ordena, y toma la última columna.
2. **Fase Huffman:** Codifica la salida de BWT usando códigos variables.

```
Flujo pedagógico:
Texto: "banana"
Rotaciones ordenadas:
  ananab
  banana ← original (índice 1)
  nabana
  anaban
  banana
  abanan
Última columna: "nnbaaa" → Agrupa 'n's y 'a's
Luego Huffman codifica "nnbaaa"
```

## Complejidad

- **Tiempo:** O(n log n) para BWT + O(n) para Huffman.
- **Espacio:** O(n) para rotaciones implícitas.
- **Ratio:** Superior a Deflate, especialmente en texto repetitivo.

## Casos de Uso

- **Compresión moderna:** Reemplaza gzip en muchos sistemas (Facebook, Linux kernel).
- **Archivos grandes:** Mejor que LZ4 en ratio, más rápido que xz.
- **Streaming:** Soporta compresión en bloques.
- **No ideal para:** Datos muy pequeños o ya comprimidos.

## Comparación con Otros

| Algoritmo | Ratio | Velocidad | Uso Principal |
|-----------|-------|-----------|---------------|
| LZ4       | Medio | Alta      | Tiempo real   |
| Deflate   | Alto  | Media     | Archivos ZIP  |
| **Zstd**  | **Muy Alto**| **Alta** | **Moderno**   |
| xz/LZMA   | Máximo| Baja      | Máx. compresión|

Zstd ofrece el mejor compromiso: ratios altos con velocidad práctica, gracias a BWT.</content>
<parameter name="filePath">/home/edi/PROYECTOS/EDA/COMPRIMIR/doc/zstd.md