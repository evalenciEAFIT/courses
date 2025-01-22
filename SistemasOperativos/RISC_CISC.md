# Comparación entre RISC y CISC

## ¿Qué es CISC?
CISC (Complex Instruction Set Computing) es un enfoque en la arquitectura de procesadores que utiliza un conjunto de instrucciones más grande y complejo. Estas instrucciones están diseñadas para realizar tareas complejas con una sola instrucción, reduciendo el número de líneas de código necesarias en el software, pero aumentando la complejidad del hardware.

### Ventajas de CISC
- Menor cantidad de líneas de código en los programas.
- Facilita la programación en ensamblador.
- Instrucciones específicas para tareas complejas.

### Desventajas de CISC
- Hardware más complejo y costoso.
- Ejecución de instrucciones más lenta debido a la decodificación compleja.
- Mayor consumo de energía.

## ¿Qué es RISC?
RISC (Reduced Instruction Set Computing) es una arquitectura de procesadores que utiliza un conjunto de instrucciones reducido, diseñado para ser simple y ejecutarse rápidamente. Cada instrucción tiene un tamaño fijo y generalmente se ejecuta en un solo ciclo de reloj.

### Ventajas de RISC
- Ejecución más rápida gracias a instrucciones simples.
- Hardware más sencillo y eficiente.
- Menor consumo de energía.

### Desventajas de RISC
- Los programas requieren más líneas de código para realizar tareas complejas.
- Puede ser más complejo para los programadores de bajo nivel.

## Tabla comparativa entre RISC y CISC

| Característica                | **CISC**                                      | **RISC**                                      |
|-------------------------------|-----------------------------------------------|-----------------------------------------------|
| **Definición**                | Conjunto de instrucciones complejas y numerosas. | Conjunto reducido de instrucciones simples.    |
| **Tamaño de las instrucciones** | Variable (longitud no fija).                 | Fijo (típicamente igual para todas).           |
| **Ciclos por instrucción**    | Instrucciones complejas que pueden tardar varios ciclos. | Instrucciones simples ejecutadas en un ciclo. |
| **Cantidad de instrucciones** | Muchas instrucciones (varias cientos o más).  | Pocas instrucciones (decenas a cientos).      |
| **Tipo de hardware**          | Hardware más complejo y costoso.             | Hardware más simple y menos costoso.          |
| **Memoria**                   | Menos dependiente de registros, más acceso a memoria. | Más dependiente de registros, menos acceso a memoria. |
| **Decodificación**            | Decodificación de instrucciones compleja.     | Decodificación de instrucciones sencilla.     |
| **Pipeline**                  | Dificultad para implementar el pipeline debido a instrucciones complejas. | Pipeline eficiente debido a instrucciones simples. |
| **Velocidad**                 | Puede ser más lento debido a la complejidad de las instrucciones. | Generalmente más rápido debido a la simplicidad de las instrucciones. |
| **Uso de instrucciones**      | Muchas instrucciones raramente utilizadas.   | Conjunto reducido, todas las instrucciones se usan frecuentemente. |
| **Ejemplo de procesadores**   | Intel x86, Motorola 68000.                   | ARM, MIPS, SPARC, RISC-V.                     |

## Procesadores comerciales y sistemas operativos

### Procesadores con CISC
- **Intel x86**: Utilizado en la mayoría de las computadoras personales y servidores.
- **AMD64**: Una extensión de x86 desarrollada por AMD para soportar arquitecturas de 64 bits.

### Procesadores con RISC
- **ARM**: Utilizado ampliamente en dispositivos móviles como smartphones y tablets.
- **MIPS**: Común en sistemas embebidos, routers y consolas de videojuegos.
- **SPARC**: Usado en servidores de alto rendimiento, especialmente en sistemas de misión crítica.
- **RISC-V**: Arquitectura abierta que se utiliza en investigación, dispositivos IoT y servidores emergentes.

### Sistemas operativos para CISC
- **Windows**: Compatible principalmente con la arquitectura x86/x64.
- **Linux**: Soporta múltiples arquitecturas, incluida x86.
- **MacOS**: Anteriormente diseñado para x86 antes de la transición a ARM.

### Sistemas operativos para RISC
- **Android**: Diseñado para procesadores ARM.
- **iOS**: Optimizado para la arquitectura ARM utilizada en dispositivos Apple.
- **Linux**: Disponible para ARM, MIPS y RISC-V, entre otros.
- **FreeBSD**: Ofrece soporte para arquitecturas RISC como ARM y MIPS.

### Propósitos de los sistemas operativos
- **CISC**: Orientado principalmente a aplicaciones de propósito general como productividad, entretenimiento y desarrollo.
- **RISC**: Enfocado en eficiencia energética y rendimiento, utilizado en dispositivos móviles, sistemas embebidos y servidores especializados.

## Ejemplo en ensamblador

### Ejemplo en CISC (x86):
```asm
; Multiplicación de dos números y almacenamiento del resultado
MOV AX, 5       ; Cargar el valor 5 en el registro AX
MOV BX, 10      ; Cargar el valor 10 en el registro BX
IMUL BX         ; Multiplicar AX por BX (resultado en AX)
MOV [RESULTADO], AX ; Almacenar el resultado en la memoria
```

### Ejemplo en RISC (MIPS):
```asm
# Multiplicación de dos números y almacenamiento del resultado
li $t0, 5        # Cargar el valor 5 en el registro $t0
li $t1, 10       # Cargar el valor 10 en el registro $t1
mul $t2, $t0, $t1 # Multiplicar $t0 por $t1 (resultado en $t2)
sw $t2, RESULTADO # Almacenar el resultado en la memoria
```

## Tabla de comandos comunes en CISC y RISC

| **Operación**             | **Comando CISC (x86)** | **Comando RISC (MIPS)** |
|---------------------------|------------------------|--------------------------|
| Cargar un valor           | `MOV AX, [MEM]`        | `lw $t0, MEM`            |
| Almacenar un valor        | `MOV [MEM], AX`        | `sw $t0, MEM`            |
| Suma                     | `ADD AX, BX`          | `add $t0, $t1, $t2`      |
| Resta                    | `SUB AX, BX`          | `sub $t0, $t1, $t2`      |
| Multiplicación           | `IMUL BX`             | `mul $t2, $t0, $t1`      |
| División                 | `IDIV BX`             | `div $t2, $t0, $t1`      |
| Salto condicional        | `JZ LABEL`            | `beq $t0, $zero, LABEL`  |
| Salto incondicional      | `JMP LABEL`           | `j LABEL`                |

Este documento destaca las diferencias clave entre RISC y CISC, y proporciona ejemplos y tablas para comprender mejor ambas arquitecturas.
