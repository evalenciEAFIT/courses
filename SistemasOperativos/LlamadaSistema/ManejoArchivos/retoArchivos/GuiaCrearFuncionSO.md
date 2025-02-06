# Guía: Crear un Programa en C++ como una Función del Sistema Operativo

## Introducción
En esta guía, aprenderemos a desarrollar un programa en **C++** que pueda ejecutarse como una función del sistema operativo Linux. Esto implica que el programa podrá ser llamado desde la terminal como un comando personalizado, similar a herramientas como `ls`, `grep`, o `cat`.

---

## 1. Requisitos Previos
Antes de comenzar, asegúrate de tener:
- Un sistema operativo **Linux**
- Un compilador de C++ (Ejemplo: `g++`)
- Permisos de administrador (para instalar el programa en `/usr/local/bin`)

---

## 2. Escribir el Código Fuente en C++

```cpp
#include <iostream>  // Para entrada y salida estándar
#include <fstream>   // Para manejo de archivos

int main(int argc, char* argv[]) {
    // Validar el número de argumentos de entrada
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo>" << std::endl;
        return 1; // Retorna error si no se proporciona el argumento requerido
    }
    
    // Abrir el archivo en modo lectura
    std::ifstream archivo(argv[1]);
    if (!archivo) { // Verificar si el archivo pudo abrirse correctamente
        std::cerr << "Error: No se pudo abrir el archivo " << argv[1] << std::endl;
        return 1; // Retorna error si el archivo no se encuentra o no es accesible
    }
    
    std::string linea;
    // Leer el archivo línea por línea y mostrarlo en pantalla
    while (std::getline(archivo, linea)) {
        std::cout << linea << std::endl;
    }
    
    return 0; // Retorna 0 para indicar que el programa finalizó correctamente
}
```

### Explicación del Código
1. **Inclusión de Librerías**
   - `<iostream>`: Permite el uso de `std::cout` y `std::cerr` para imprimir mensajes en la consola.
   - `<fstream>`: Se utiliza para leer archivos mediante `std::ifstream`.
2. **Manejo de Argumentos (`argc` y `argv`)**
   - `argc` cuenta el número de argumentos proporcionados.
   - `argv[1]` almacena el nombre del archivo pasado como argumento.
3. **Apertura y Lectura del Archivo**
   - `std::ifstream archivo(argv[1]);` intenta abrir el archivo en modo lectura.
   - `std::getline(archivo, linea)`: Lee el contenido línea por línea y lo muestra en consola.
4. **Control de Errores**
   - Si el número de argumentos no es correcto, el programa muestra un mensaje de error.
   - Si el archivo no se puede abrir, el programa notifica al usuario y retorna un código de error.

---

## 3. Compilar el Programa
Para compilar el programa, usa el siguiente comando:

```bash
g++ -o mycommand mycommand.cpp
```

Esto generará un ejecutable llamado `mycommand`.

---

## 4. Instalar el Programa en el Sistema
Para poder ejecutar el programa desde cualquier ubicación, debes moverlo a un directorio accesible globalmente como `/usr/local/bin`:

```bash
sudo mv mycommand /usr/local/bin/
```

Luego, asegúrate de que tenga permisos de ejecución:

```bash
sudo chmod +x /usr/local/bin/mycommand
```

---

## 5. Ejecutar el Programa desde Cualquier Lugar
Ahora puedes ejecutar el programa desde cualquier directorio en la terminal:

```bash
mycommand archivo.txt
```

Si todo está configurado correctamente, verás el contenido del archivo `archivo.txt` en la terminal.

---

## 6. Conclusión
Siguiendo estos pasos, hemos convertido un programa en C++ en un comando del sistema operativo. Esto es útil para crear herramientas personalizadas que faciliten el manejo de archivos y otros procesos en Linux.

---

## 7. Recursos Adicionales
- [Documentación oficial de GNU/Linux](https://www.gnu.org/software/libc/manual/)

