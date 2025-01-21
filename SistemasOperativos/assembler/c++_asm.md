# Crear un Programa en C++ con Ensamblador (Assembler)

Este documento describe cómo crear un programa en C++ que integre código ensamblador utilizando las palabras clave `asm` o las instrucciones en línea soportadas por compiladores como GCC o MSVC.

---

## Requisitos Previos

### Herramientas Necesarias
1. **Compilador C++**:
   - **Linux**: GCC (`g++`).
   - **Windows**: MSVC (Microsoft Visual Studio) o MinGW.

2. **Editor de Texto**: Cualquier editor como **Visual Studio Code**, **vim**, o **Notepad++**.

3. **Conocimientos Básicos**: Familiaridad con C++ y ensamblador.

---

## Ejemplo Básico: Suma de Dos Números

Este programa combina ensamblador en línea con C++ para realizar una suma.

### Código

Guarda este código como `suma_asm.cpp`:

```cpp
#include <iostream>

int suma(int a, int b) {
    int resultado;
    asm(
        "movl %1, %%eax;" // Mover el primer operando a EAX
        "addl %2, %%eax;" // Sumar el segundo operando a EAX
        "movl %%eax, %0;" // Mover el resultado a la variable de salida
        : "=r" (resultado) // Salida
        : "r" (a), "r" (b) // Entradas
        : "%eax"           // Registros modificados
    );
    return resultado;
}

int main() {
    int num1 = 5, num2 = 7;
    int resultado = suma(num1, num2);

    std::cout << "La suma de " << num1 << " y " << num2 << " es: " << resultado << std::endl;
    return 0;
}
```

### Explicación del Código

1. **Bloque `asm`**:
   - Utiliza ensamblador en línea para realizar operaciones directamente en registros.
   - Las instrucciones ensamblador están entre comillas dobles.

2. **Sección de Salida (`: "=r"`)**:
   - Define variables de salida, indicando que el resultado se almacena en un registro general.

3. **Sección de Entrada (`: "r"`)**:
   - Especifica los operandos de entrada que se asignan a registros.

4. **Clobber List (`: "%eax"`)**:
   - Informa al compilador que el registro EAX será modificado.

---

## Ejemplo Avanzado: Multiplicación

Este programa realiza una multiplicación usando ensamblador en línea.

### Código

Guarda este código como `multiplicacion_asm.cpp`:

```cpp
#include <iostream>

int multiplicar(int a, int b) {
    int resultado;
    asm(
        "imull %1, %2;"     // Multiplicar a * b
        "movl %2, %0;"      // Mover el resultado a la variable de salida
        : "=r" (resultado)   // Salida
        : "r" (a), "r" (b)  // Entradas
        : "%eax"            // Registros modificados
    );
    return resultado;
}

int main() {
    int num1 = 3, num2 = 4;
    int resultado = multiplicar(num1, num2);

    std::cout << "El producto de " << num1 << " y " << num2 << " es: " << resultado << std::endl;
    return 0;
}
```

---

## Compilación y Ejecución

### En Linux

1. **Compilar**:
   Usa `g++` para compilar el programa:

   ```bash
   g++ suma_asm.cpp -o suma_asm
   g++ multiplicacion_asm.cpp -o multiplicacion_asm
   ```

2. **Ejecutar**:
   ```bash
   ./suma_asm
   ./multiplicacion_asm
   ```

### En Windows

1. **Compilar con MSVC**:
   Abre el **Developer Command Prompt** y compila:

   ```cmd
   cl suma_asm.cpp
   cl multiplicacion_asm.cpp
   ```

2. **Ejecutar**:
   ```cmd
   suma_asm.exe
   multiplicacion_asm.exe
   ```

---

## Notas

- **Seguridad**: Usa ensamblador en línea solo si es absolutamente necesario, ya que puede reducir la portabilidad del código.
- **Optimización**: Los compiladores modernos suelen generar código optimizado, lo que puede hacer innecesario el uso de ensamblador manual en muchos casos.
