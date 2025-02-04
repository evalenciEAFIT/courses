# Archivos Planos y Archivos Binarios en Linux

## 1. Introducción

En **Linux**, los archivos se pueden clasificar en **archivos planos (texto)** y **archivos binarios**. Ambos tipos tienen propósitos distintos y se manejan de manera diferente con comandos y programas.

### ¿Qué es un archivo plano?

Un **archivo plano** es un archivo que contiene datos en **formato de texto legible**, donde cada línea representa una entrada de datos. Se utilizan comúnmente para **almacenar configuraciones, registros (logs), documentos y bases de datos sencillas**. Ejemplos de archivos planos incluyen `.txt`, `.csv`, `.json`, y `.xml`.

### ¿Qué es un archivo binario?

Un **archivo binario** almacena datos en **formato binario**, lo que significa que no está compuesto por caracteres legibles directamente por humanos. Se usa para **almacenar imágenes, audio, ejecutables y estructuras de datos más eficientes**. Los archivos `.bin`, `.exe`, `.png` y `.mp3` son ejemplos de archivos binarios.

## 2. Archivos Planos (Texto)

Los **archivos planos** contienen texto legible y pueden ser abiertos y editados con cualquier editor de texto. Son utilizados para almacenar datos estructurados como logs, configuraciones y documentos.

### 2.1. Crear un archivo plano desde la entrada de consola

#### Código en C++ usando llamadas al sistema

```cpp
#include <fcntl.h>   // Librería para manejar archivos (open)
#include <unistd.h>  // Librería para manejar archivos (write, close)
#include <iostream>
#include <string.h>

int main() {
    // Abre o crea un archivo de texto con permisos de escritura y lectura para el usuario
    int fd = open("archivo.txt", O_WRONLY | O_CREAT, 0644);
    if (fd == -1) { // Verifica si hubo error al abrir el archivo
        perror("Error al crear el archivo");
        return 1;
    }
    
    std::string entrada;
    std::cout << "Ingrese texto para guardar en el archivo: ";
    std::getline(std::cin, entrada);
    entrada += "\n"; // Agrega salto de línea al final
    
    // Escribe el contenido ingresado en el archivo usando write()
    write(fd, entrada.c_str(), entrada.size()); 
    close(fd); // Cierra el archivo para liberar recursos
    return 0;
}
```

### 2.2. Leer un archivo plano y mostrarlo en formato binario y hexadecimal

#### Código en C++

```cpp
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <bitset>

int main() {
    // Abre el archivo en modo solo lectura
    int fd = open("archivo.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir el archivo");
        return 1;
    }
    
    char c;
    // Lee el archivo carácter por carácter y lo muestra en binario y hexadecimal
    while (read(fd, &c, 1) > 0) {
        std::cout << "Caracter: " << c 
                  << " | Binario: " << std::bitset<8>(c) 
                  << " | Hex: " << std::hex << (int)(unsigned char)c 
                  << std::endl;
    }
    
    close(fd); // Cierra el archivo
    return 0;
}
```

## 3. Archivos Binarios

### 3.1. Crear un archivo binario desde la entrada en consola

#### Código en C++

```cpp
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main() {
    // Abre o crea un archivo binario con permisos de lectura y escritura para el usuario
    int fd = open("archivo.bin", O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("Error al crear el archivo binario");
        return 1;
    }
    
    int numero;
    std::cout << "Ingrese un número entero para guardar en el archivo binario: ";
    std::cin >> numero;
    
    // Escribe el número en formato binario en el archivo usando write()
    write(fd, &numero, sizeof(numero));
    close(fd);
    return 0;
}
```

### 3.2. Leer un archivo binario y mostrarlo en formato binario y hexadecimal

#### Código en C++

```cpp
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <bitset>

int main() {
    // Abre el archivo binario en modo lectura
    int fd = open("archivo.bin", O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir el archivo binario");
        return 1;
    }
    
    unsigned char buffer;
    // Lee el archivo byte por byte y muestra su representación en binario y hexadecimal
    while (read(fd, &buffer, 1) > 0) {
        std::cout << "Byte: " << std::bitset<8>(buffer) 
                  << " | Hex: " << std::hex << (int)buffer 
                  << std::endl;
    }
    
    close(fd);
    return 0;
}
```

## 4. Explicación de los parámetros de las llamadas al sistema

En Linux, la manipulación de archivos se realiza a través de **llamadas al sistema** como `open()`, `write()`, `read()` y `close()`, que permiten la interacción entre programas y el sistema operativo.

### `open()`
```cpp
int fd = open("archivo.txt", O_WRONLY | O_CREAT, 0644);
```
- **"archivo.txt"** → Nombre del archivo a abrir o crear.
- **O_WRONLY** → Abre el archivo en modo solo escritura.
- **O_CREAT** → Crea el archivo si no existe.
- **0644** → Permisos de archivo (propietario puede leer/escribir, otros solo leer).

### `write()`
```cpp
write(fd, entrada.c_str(), entrada.size());
```
- **fd** → Descriptor del archivo abierto.
- **entrada.c_str()** → Datos a escribir (convertido a cadena de caracteres).
- **entrada.size()** → Cantidad de bytes a escribir.

### `read()`
```cpp
read(fd, &buffer, 1);
```
- **fd** → Descriptor del archivo abierto.
- **&buffer** → Dirección donde almacenar el byte leído.
- **1** → Cantidad de bytes a leer.

### `close()`
```cpp
close(fd);
```
- **fd** → Descriptor del archivo que se desea cerrar.

## 5. Permisos de Archivos en Linux

En Linux, los permisos de archivos se representan mediante valores octales y definen los niveles de acceso para el **propietario**, el **grupo** y **otros usuarios**.

### Representación en formato octal y binario

| Octal | Binario  | Significado |
|-------|---------|-------------|
| 0     | 000     | Sin permisos |
| 1     | 001     | Solo ejecución |
| 2     | 010     | Solo escritura |
| 3     | 011     | Escritura y ejecución |
| 4     | 100     | Solo lectura |
| 5     | 101     | Lectura y ejecución |
| 6     | 110     | Lectura y escritura |
| 7     | 111     | Lectura, escritura y ejecución |

### Ejemplos de permisos en archivos

- **0644** → Propietario con lectura y escritura, grupo y otros solo lectura.
- **0755** → Propietario con todos los permisos, grupo y otros solo lectura y ejecución.
- **0777** → Todos los usuarios con todos los permisos (**riesgo de seguridad**).

Se pueden cambiar los permisos usando el comando:
```sh
chmod 644 archivo.txt  # Cambia permisos del archivo
```

## 6. Conclusiones

El uso de archivos planos y binarios en **Linux** es fundamental en la programación de sistemas. Comprender los permisos y el uso de llamadas al sistema permite una manipulación segura y eficiente de los archivos. También es importante conocer los permisos y cómo asignarlos para proteger los datos. ¡Practica estos comandos y sigue explorando!


