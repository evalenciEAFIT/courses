# Deflate — LZ77 + Huffman

## Concepto

Deflate es el algoritmo híbrido que combina la búsqueda de patrones de LZ77 con la codificación de entropía de Huffman. Es la base de formatos como ZIP, gzip, PNG y HTTP compression. Logra un excelente balance entre ratio de compresión y velocidad.

## Mecánica

1. **Fase LZ77:** Busca matches en una ventana deslizante y emite tokens (distancia, longitud, literal).
2. **Fase Huffman:** Codifica los tokens resultantes usando códigos variables basados en frecuencia.

```
Flujo:
Datos → LZ77 tokens → Huffman coding → Salida comprimida
```

## Parámetros Típicos

- **Ventana LZ77:** 32KB (como en gzip).
- **Árbol Huffman:** Dinámico, adaptado a los tokens generados.
- **Compresión:** Puede ser "stored" (sin LZ), "static" (Huffman fijo) o "dynamic" (Huffman adaptado).

## Complejidad

- **Tiempo:** O(n) para LZ77 + O(n + k log k) para Huffman.
- **Espacio:** O(W) para ventana + O(k) para árbol Huffman.
- **Ratio:** 20-80% del tamaño original, dependiendo del contenido.

## Casos de Uso

- **Archivos ZIP/gzip:** Compresión general de archivos.
- **Imágenes PNG:** Después de filtros predictivos.
- **Redes HTTP:** Compresión de respuestas web.
- **No ideal para:** Datos ya comprimidos o muy pequeños.

## Ventajas sobre Algoritmos Puros

- **LZ77 solo:** Alto ratio pero salida grande (tokens sin comprimir).
- **Huffman solo:** Bueno para entropía pero no para patrones largos.
- **Deflate:** Combina lo mejor de ambos mundos.

Deflate es el "estándar de facto" para compresión general en la web y sistemas de archivos.</content>
<parameter name="filePath">/home/edi/PROYECTOS/EDA/COMPRIMIR/doc/deflate.md