# Llamadas al Sistema Operativo 
(Linux)

## 1. Introducción a las llamadas al sistema

Las **llamadas al sistema** (*system calls*) son mecanismos proporcionados por el **núcleo del sistema operativo** para que los programas de usuario puedan interactuar con el hardware y los recursos del sistema, como archivos, memoria, procesos y comunicación.

En **Linux**, las llamadas al sistema permiten a un programa realizar operaciones como:
- Leer y escribir archivos
- Crear y administrar procesos
- Asignar memoria
- Comunicación entre procesos (IPC)
- Administrar permisos y recursos del sistema

Las llamadas al sistema se ejecutan a través de la **interrupción del sistema** que cambia el contexto de ejecución del modo usuario al modo núcleo.

## 2. Uso de llamadas al sistema en C++ en Linux

En **C++ en Linux**, las llamadas al sistema se pueden invocar mediante la **API POSIX**, a través de las funciones de la biblioteca estándar de C (`unistd.h`, `fcntl.h`, `sys/types.h`, etc.).

Algunas llamadas al sistema comunes en Linux incluyen:
- `open()`, `read()`, `write()`, `close()` → Manejo de archivos
- `fork()`, `exec()`, `wait()` → Creación y control de procesos
- `mmap()`, `munmap()` → Gestión de memoria
- `socket()`, `bind()`, `listen()`, `accept()` → Comunicación en red

### 2.1. Ejemplo de uso de `write()`

```cpp
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

int main() {
    const char *mensaje = "Hola, Linux!\n";
    write(STDOUT_FILENO, mensaje, 13); // Escribe en la salida estándar (consola)
    return 0;
}
```

Aquí, `write()` es una **llamada al sistema** que envía datos a la salida estándar.

### 2.2. Ejemplo de manejo de archivos con `open()`, `write()`, `close()`

```cpp
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main() {
    int archivo = open("ejemplo.txt", O_WRONLY | O_CREAT, 0644);
    if (archivo == -1) {
        perror("Error al abrir el archivo");
        return 1;
    }
    
    const char *texto = "Este es un ejemplo de llamada al sistema en Linux.\n";
    write(archivo, texto, 46);
    close(archivo);
    
    return 0;
}
```

Aquí:
1. `open()` abre un archivo con permisos de escritura (`O_WRONLY`) y lo crea si no existe (`O_CREAT`).
2. `write()` escribe datos en el archivo.
3. `close()` cierra el archivo.

### 2.3. Ejemplo de creación de procesos con `fork()`

```cpp
#include <unistd.h>
#include <iostream>

int main() {
    pid_t pid = fork();
    
    if (pid == 0) {
        std::cout << "Proceso hijo ejecutándose.\n";
    } else {
        std::cout << "Proceso padre ejecutándose.\n";
    }
    return 0;
}
```

Aquí, `fork()` crea un nuevo proceso **hijo** duplicando el proceso actual.

## 3. Conclusión

Las llamadas al sistema son esenciales para interactuar con el núcleo de **Linux** desde **C++**. Permiten gestionar archivos, memoria y procesos de manera eficiente. Su uso adecuado garantiza una **buena administración de recursos** y un **desempeño óptimo** de los programas.
