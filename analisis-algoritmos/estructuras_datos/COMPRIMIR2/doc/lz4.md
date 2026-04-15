# LZ4 — Compresión Rápida

## Concepto

LZ4 es un algoritmo de compresión sin pérdida diseñado para ser extremadamente rápido, sacrificando algo de ratio de compresión por velocidad. No utiliza codificación de entropía (como Huffman), sino que se basa únicamente en referencias a secuencias repetidas.

## Mecánica

- **Búsqueda de matches:** Busca secuencias de al menos 4 bytes que se repitan dentro de una ventana deslizante.
- **Emisión de tokens:** Cuando encuentra un match, emite un marcador especial seguido de la longitud del match y la distancia al match anterior.
- **Literales:** Los bytes que no coinciden se emiten directamente.

```
Ejemplo simplificado:
Entrada: "ABCDABCDABCD"
Salida:  "ABCD" + token(match len=4, dist=4) + token(match len=4, dist=8)
```

## Complejidad

- **Tiempo:** O(n) — muy rápido, optimizado para CPU modernas.
- **Espacio:** O(1) adicional — no requiere estructuras grandes.
- **Ratio:** Típicamente 40-60% del tamaño original, dependiendo del contenido.

## Casos de Uso

- **Compresión en tiempo real:** Logs, bases de datos, redes.
- **Almacenamiento temporal:** Archivos que se comprimen/descomprimen frecuentemente.
- **No ideal para:** Archivos ya comprimidos o datos aleatorios.

## Comparación con Otros LZ

| Algoritmo | Velocidad | Ratio | Complejidad |
|-----------|-----------|-------|-------------|
| LZ77      | Media     | Alto  | O(n × W)    |
| LZ78      | Media     | Alto  | O(n)        |
| LZW       | Media     | Alto  | O(n)        |
| **LZ4**   | **Alta**  | **Medio**| **O(n)**    |

LZ4 es perfecto cuando la velocidad es más importante que el máximo ahorro de espacio.</content>
<parameter name="filePath">/home/edi/PROYECTOS/EDA/COMPRIMIR/doc/lz4.md