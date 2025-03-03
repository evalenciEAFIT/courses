# Uso de `if`,`while`  y `for` en ensamblador de Nand2Tetris

## Introducción
Este documento explica cómo implementar estructuras de control condicional (`if`), repetitiva (`for`) y (`while`) en el lenguaje ensamblador de la arquitectura Hack, usada en Nand2Tetris.

---

## Código `if`

### Explicación
- **Condición:** `if (x > 0)`, donde `x` está almacenado en `RAM[16]`.
- Se usa `RAM[17]` como variable `resultado`.
- Si `x > 0`, se suma 1 a `resultado`.
- Si `x <= 0`, se resta 1 a `resultado`.

### Código ensamblador Hack
```assembly
@16      // Cargar x en D
D=M
@NEGATIVE
D;JLE   // Si D <= 0, saltar a NEGATIVE

// x > 0: incrementar resultado en RAM[17]
@17
D=M
@17
M=D+1
@END_IF
0;JMP   // Saltar al final

(NEGATIVE)
// x <= 0: restar 1 a resultado en RAM[17]
@17
D=M
@17
M=D-1

(END_IF)
@END_IF
0;JMP   // Bucle infinito para detener el programa
```
---

## Código `while`

### Explicación
- **Bucle `while(x < 10)`**.
- Se incrementa `x` hasta que llegue a 10.

### Código ensamblador Hack
```assembly
@16
D=M    // Cargar x en D

(WHILE)
@10
D=D-A  // Comparar x con 10
@END_WHILE
D;JGE  // Si x >= 10, salir del bucle

@16
D=M
@16
M=D+1  // Incrementar x (x = x + 1)

@WHILE
0;JMP  // Volver a WHILE

(END_WHILE)
@END_WHILE
0;JMP  // Bucle infinito para detener el programa
```
---
## Código `for`

### Explicación
- **Bucle `for (i = 0; i < 5; i++)`**.
- Se usa `RAM[17]` como contador.
- Se ejecuta 5 veces, incrementando `x` en cada iteración.

### Código ensamblador Hack
```assembly
@16
D=M        // Cargar x en D
@END_IF
D;JLE      // Si D <= 0, saltar a END_IF

@17
M=0        // Inicializar contador i = 0

(LOOP)
@17
D=M
@END_FOR
D-5;JGE    // Si i >= 5, salir del bucle

@16
D=M
@16
M=D+1      // Incrementar x (x = x + 1)

@17
D=M
@17
M=D+1      // Incrementar i (i = i + 1)

@LOOP
0;JMP      // Volver a LOOP

(END_FOR)
(END_IF)
@END_IF
0;JMP      // Bucle infinito para detener el programa
```
---

## Reto Adicional

Para fortalecer la comprensión del ensamblador Hack, intenta implementar las siguientes operaciones:

### 1. Multiplicación (`x * y`)
- Almacenar `x` en `RAM[16]` y `y` en `RAM[18]`.
- Implementar la multiplicación mediante sumas sucesivas.
- Guardar el resultado en `RAM[19]`.

### 2. División (`x / y`)
- Almacenar `x` en `RAM[16]` y `y` en `RAM[18]`.
- Implementar la división mediante restas sucesivas.
- Guardar el cociente en `RAM[19]` y el residuo en `RAM[20]`.

### 3. Cálculo de la serie de Fibonacci
- Implementa un código que calcule el termino `n` de la serie de Fibonacci.
- Usa `RAM[16]` para almacenar `n` (el número de términos a generar).
- Usa `RAM[20]` para almacenar el resultado.


## Ejemplo de ejecución

### Entrada inicial en RAM
```
RAM[16] = 5  // Número de términos de Fibonacci
```

### Salida esperada en RAM después de ejecución
```
RAM[20] = 5  // Quinto término de la serie de Fibonacci
```
---
