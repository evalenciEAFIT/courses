# Llamadas al Sistema en C++ y Comparación con Funciones del Sistema Operativo

## Introducción

Las llamadas al sistema (system calls) son interfaces proporcionadas por el sistema operativo para permitir que los programas de usuario interactúen con los recursos del sistema. A diferencia de las funciones de biblioteca estándar, las llamadas al sistema involucran una transición del modo usuario al modo kernel, lo que implica un mayor costo computacional.

## Clasificación de las Llamadas al Sistema

Las llamadas al sistema pueden clasificarse en diferentes categorías:

1. **Manejo de Archivos**: `open()`, `read()`, `write()`, `close()`
2. **Manejo de Procesos**: `fork()`, `exec()`, `wait()`, `exit()`
3. **Manejo de Memoria**: `mmap()`, `brk()`
4. **Manejo de Dispositivos**: `ioctl()`, `select()`
5. **Manejo de Redes**: `socket()`, `connect()`, `bind()`, `listen()`, `accept()`

## Explicación de los Parámetros de `open()` y `fopen()`

Las funciones `open()` y `fopen()` permiten la manipulación de archivos en C/C++ con diferentes configuraciones.

### Parámetros de `open()`

- `O_RDONLY`: Solo lectura.
- `O_WRONLY`: Solo escritura.
- `O_RDWR`: Lectura y escritura.
- `O_CREAT`: Crea el archivo si no existe.
- `O_TRUNC`: Trunca el archivo si ya existe.
- `O_APPEND`: Escribe datos al final del archivo.

### Parámetros de `fopen()`

- `"r"`: Modo solo lectura.
- `"w"`: Modo escritura (sobrescribe si existe).
- `"a"`: Modo anexar.
- `"r+"`: Modo lectura y escritura (el archivo debe existir).
- `"w+"`: Modo lectura y escritura (sobrescribe si existe).
- `"a+"`: Modo lectura y escritura, escribiendo al final.

Cada modo puede ser combinado con `"b"` (modo binario) para leer/escribir archivos en formato binario, por ejemplo `"rb"`, `"wb"`, `"ab"`.

## Ejemplos de Código

### 1. Uso de llamadas al sistema

#### 1.1 Archivos Planos

##### 1.1.1 Escribir

```cpp
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string>

int main() {
    // Abrir el archivo en modo escritura, creándolo si no existe y truncándolo si ya existe.
    int fd = open("archivo.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) { // Verificar si hubo un error al abrir el archivo.
        perror("Error creando el archivo");
        return 1;
    }
    
    // Solicitar al usuario que ingrese un texto para guardar en el archivo.
    std::string entrada;
    std::cout << "Ingrese el texto a guardar: ";
    std::getline(std::cin, entrada);
    
    // Escribir el texto ingresado en el archivo.
    write(fd, entrada.c_str(), entrada.size());
    
    // Cerrar el archivo.
    close(fd);
    return 0;
}
```
**Explicación:** Este código abre un archivo de texto en modo escritura, solicita una entrada del usuario y la escribe en el archivo usando `write()`. Luego, cierra el archivo para liberar los recursos.

##### 1.1.2 Leer

```cpp
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main() {
    // Abrir el archivo en modo solo lectura.
    int fd = open("archivo.txt", O_RDONLY);
    if (fd == -1) { // Verificar si hubo un error al abrir el archivo.
        perror("Error abriendo el archivo");
        return 1;
    }
    
    // Declarar un buffer para almacenar los datos leídos del archivo.
    char buffer[128];
    ssize_t bytes_leidos = read(fd, buffer, sizeof(buffer));
    
    // Escribir los datos leídos directamente en la salida estándar.
    write(STDOUT_FILENO, buffer, bytes_leidos);
    
    // Cerrar el archivo.
    close(fd);
    return 0;
}
```
**Explicación:** Este código abre un archivo en modo lectura y lee su contenido utilizando `read()`. Luego, imprime los datos leídos en la salida estándar con `write()`.

#### 1.2 Archivos Binarios

##### 1.2.1 Escribir

```cpp
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <vector>

int main() {
    // Abrir el archivo binario en modo escritura, creándolo si no existe y truncándolo si ya existe.
    int fd = open("archivo.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) { // Verificar si hubo un error al abrir el archivo.
        perror("Error creando el archivo binario");
        return 1;
    }
    
    // Datos binarios a escribir en el archivo.
    std::vector<uint8_t> datos = {0x41, 0x42, 0x43, 0x44};
    write(fd, datos.data(), datos.size());
    
    // Cerrar el archivo.
    close(fd);
    return 0;
}
```
**Explicación:** Este código abre un archivo binario en modo escritura y escribe una serie de bytes en él. Los datos se guardan como bytes puros, ideales para formatos binarios.

##### 1.2.2 Leer

```cpp
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <vector>

int main() {
    // Abrir el archivo binario en modo solo lectura.
    int fd = open("archivo.bin", O_RDONLY);
    if (fd == -1) { // Verificar si hubo un error al abrir el archivo.
        perror("Error abriendo el archivo binario");
        return 1;
    }
    
    // Buffer para leer los datos binarios.
    std::vector<uint8_t> buffer(4);
    read(fd, buffer.data(), buffer.size());
    
    // Imprimir los datos en formato hexadecimal y ASCII.
    for (auto byte : buffer) {
        printf("(0x%X::%08b) %c ", byte, byte, byte);
    }
    printf("\n");
    
    // Cerrar el archivo.
    close(fd);
    return 0;
}
```
**Explicación:** Este código lee datos binarios de un archivo y los imprime en la consola en formato hexadecimal, binario y ASCII. Es útil para analizar el contenido de archivos binarios.

### 2. Uso de Librerías del Sistema Operativo

#### 2.1 Archivos Planos

##### 2.1.1 Escribir

```cpp
#include <cstdio>
#include <iostream>
#include <string>

int main() {
    // Abrir el archivo en modo escritura (crea o sobrescribe el archivo).
    FILE *archivo = fopen("archivo.txt", "w");
    if (!archivo) { // Verificar si hubo un error al abrir el archivo.
        perror("Error creando el archivo");
        return 1;
    }
    
    // Solicitar al usuario que ingrese un texto.
    std::string entrada;
    std::cout << "Ingrese el texto a guardar: ";
    std::getline(std::cin, entrada);
    
    // Escribir el texto ingresado en el archivo.
    fwrite(entrada.c_str(), 1, entrada.size(), archivo);
    
    // Cerrar el archivo.
    fclose(archivo);
    return 0;
}
```
**Explicación:** Este código utiliza `fopen()` para abrir un archivo en modo escritura, escribe una cadena proporcionada por el usuario y luego cierra el archivo para asegurar la integridad de los datos.

##### 2.1.2 Leer

```cpp
#include <cstdio>

int main() {
    // Abrir el archivo en modo lectura.
    FILE *archivo = fopen("archivo.txt", "r");
    if (!archivo) { // Verificar si hubo un error al abrir el archivo.
        perror("Error abriendo el archivo");
        return 1;
    }
    
    // Leer los datos del archivo y almacenarlos en un buffer.
    char buffer[128];
    fread(buffer, 1, sizeof(buffer), archivo);
    
    // Imprimir los datos leídos.
    printf("%s", buffer);
    
    // Cerrar el archivo.
    fclose(archivo);
    return 0;
}
```
**Explicación:** Este código utiliza `fopen()` para abrir un archivo en modo lectura, lee su contenido en un buffer y lo imprime en la consola. Luego cierra el archivo para liberar recursos.

## Conclusión

La diferencia principal entre llamadas al sistema y funciones de biblioteca radica en el nivel de abstracción y optimización. Las llamadas al sistema permiten un control más detallado, mientras que las funciones de biblioteca optimizan el rendimiento utilizando buffers internos.


