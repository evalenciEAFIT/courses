# Guía de Uso de `argc` y `argv` en C++ para Linux

## Introducción
En C++, los programas pueden recibir argumentos desde la línea de comandos mediante los parámetros `argc` y `argv` en la función `main`. Esta guía explica cómo utilizarlos correctamente en sistemas Linux, incluyendo argumentos obligatorios, opciones y los flags `--version` y `--help`.

---

## 1. ¿Qué son `argc` y `argv`?
### 📌 `argc` (Argument Count)
Es un entero que indica la cantidad de argumentos pasados al programa desde la línea de comandos. Incluye el nombre del programa como primer argumento.

### 📌 `argv` (Argument Vector)
Es un array de cadenas (`char*`) que contiene los argumentos pasados al programa. 
- `argv[0]` es el nombre del ejecutable.
- `argv[1]`, `argv[2]`, etc., son los argumentos proporcionados por el usuario.

---

## 2. Ejemplo Básico de Uso
```cpp
#include <iostream>

int main(int argc, char* argv[]) {
    // Imprime el número total de argumentos
    std::cout << "Número de argumentos: " << argc << "\n";
    
    // Itera sobre todos los argumentos e imprime su contenido
    for (int i = 0; i < argc; i++) {
        std::cout << "Argumento " << i << ": " << argv[i] << "\n";
    }
    return 0;
}
```
### 🔹 Ejecución en terminal
```bash
./mi_programa arg1 arg2 arg3
```
### 🔹 Salida esperada
```
Número de argumentos: 4
Argumento 0: ./mi_programa
Argumento 1: arg1
Argumento 2: arg2
Argumento 3: arg3
```

---

## 3. Argumentos Obligatorios y Opcionales
```cpp
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // Verifica si el usuario proporcionó al menos un argumento
    if (argc < 2) {
        std::cerr << "Error: Se requiere al menos un argumento.\n";
        return 1;
    }
    
    // Imprime el primer argumento obligatorio
    std::cout << "Argumento obligatorio: " << argv[1] << "\n";
    return 0;
}
```
### 🔹 Ejecución
```bash
./mi_programa nombre
```
### 🔹 Salida esperada
```
Argumento obligatorio: nombre
```
Si no se proporciona un argumento, el programa mostrará un mensaje de error.

---

## 4. Opciones (`--version`, `--help`, `-o`)
```cpp
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // Verifica si se ingresó al menos un argumento
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " [opciones]" << std::endl;
        return 1;
    }
    
    // Almacena la opción ingresada por el usuario
    std::string opcion = argv[1];
    
    // Manejo de la opción --help o -h
    if (opcion == "--help" || opcion == "-h") {
        std::cout << "Uso: " << argv[0] << " [opciones]" << std::endl;
        std::cout << "  --help, -h       Muestra esta ayuda" << std::endl;
        std::cout << "  --version, -v    Muestra la versión" << std::endl;
        return 0;
    }
    // Manejo de la opción --version o -v
    else if (opcion == "--version" || opcion == "-v") {
        std::cout << "Versión 1.0" << std::endl;
        return 0;
    }
    // Si la opción no es válida
    else {
        std::cout << "Opción desconocida: " << opcion << std::endl;
        return 1;
    }
}
```
### 🔹 Ejecución y salida esperada
```bash
./mi_programa --help
```
```
Uso: ./mi_programa [opciones]
  --help, -h       Muestra esta ayuda
  --version, -v    Muestra la versión
```
```bash
./mi_programa --version
```
```
Versión 1.0
```
```bash
./mi_programa --otro
```
```
Opción desconocida: --otro
```

---

## 5. Conclusión
- `argc` nos dice cuántos argumentos se pasaron al programa.
- `argv` contiene los valores de los argumentos.
- Se pueden definir argumentos obligatorios y opciones como `--help` y `--version` para mejorar la experiencia del usuario.

Esta guía proporciona una base para el manejo de argumentos en C++ en Linux. 

