# LZW — Lempel-Ziv-Welch

> Compresor que transmite códigos en lugar de pares (índice, símbolo).

## Idea principal

LZW inicia con un diccionario de todos los símbolos de un byte y luego
aprende frases más largas a medida que procesa la entrada.

En lugar de enviar un par `(índice, símbolo)` como LZ78, LZW sólo emite un
`código` que representa una frase entera.

## Flujo básico

1. Comienza con el diccionario de 256 entradas para los bytes 0..255
2. Encuentra la frase más larga que coincida con el texto actual
3. Emite el código de esa frase
4. Agrega al diccionario la frase anterior extendida con el siguiente símbolo

## Ventaja pedagógica

- Muestra cómo un diccionario puede crecer sin transmitir el símbolo separado
- Revela el famoso caso especial de LZW cuando el código entrante es el siguiente
  código aún no agregado al diccionario

## Implementación en este proyecto

- `src/lzw.h` / `src/lzw.c`
- Token fijo de 2 bytes: `uint16_t code`
- Descompresión reconstruye las frases del diccionario y maneja el caso especial

## Características

- **Mejor caso:** datos con repeticiones de frases largas
- **Base de:** GIF, compresores clásicos y enfoques de preprocesamiento
- **Complejidad:** O(n^2) en esta implementación didáctica
