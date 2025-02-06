# Guía: Uso de `argc` y `argv` en C++ para Linux

## Introducción
En C++, `argc` (argument count) y `argv` (argument vector) se utilizan para manejar argumentos de línea de comandos cuando se ejecuta un programa en un entorno de terminal en Linux.

- `argc` indica la cantidad de argumentos pasados al programa (incluyendo el nombre del programa).
- `argv` es un array de cadenas de caracteres que almacena los argumentos como texto.

---

## Sintaxis de `argc` y `argv`

```cpp
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "Número de argumentos recibidos: " << argc << "\n";
    for (int i = 0; i < argc; i++) {
        std::cout << "Argumento " << i << ": " << argv[i] << "\n";
    }
    return 0;
}
```

---

## Ejemplo de Uso
### Compilación y Ejecución
```bash
g++ programa.cpp -o programa
./programa arg1 arg2 arg3
```

### Salida Esperada
```plaintext
Número de argumentos recibidos: 4
Argumento 0: ./programa
Argumento 1: arg1
Argumento 2: arg2
Argumento 3: arg3
```

---

## Explicación Paso a Paso
1. **Compilación**: Se compila el archivo fuente `programa.cpp` usando `g++`.
2. **Ejecución**: Se ejecuta el programa con argumentos adicionales en la terminal.
3. **Salida**:
   - `argc` cuenta los argumentos pasados (incluyendo el nombre del ejecutable).
   - `argv[i]` almacena cada argumento en orden.

---

## Validación de Argumentos
Si queremos asegurarnos de que el usuario proporciona los argumentos esperados, podemos agregar validaciones:

```cpp
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <argumento1> <argumento2>" << std::endl;
        return 1;
    }
    
    std::cout << "Primer argumento: " << argv[1] << "\n";
    std::cout << "Segundo argumento: " << argv[2] << "\n";
    return 0;
}
```

### Ejemplo de Ejecución con Validación
```bash
./programa valor1 valor2
```
**Salida:**
```plaintext
Primer argumento: valor1
Segundo argumento: valor2
```

Si el usuario no proporciona suficientes argumentos, el programa imprimirá un mensaje de error.

---

## Conclusión
El uso de `argc` y `argv` en C++ es fundamental para interactuar con la terminal en sistemas Linux, permitiendo procesar argumentos de entrada para personalizar la ejecución de un programa.

---

## Recursos Adicionales
- [Documentación oficial de C++](https://en.cppreference.com/w/cpp/language/main)

