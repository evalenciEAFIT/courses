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

## 4. Permisos de Archivos en Linux

Explicación sobre cómo funcionan los permisos de archivo en Linux, los valores octales y binarios.

## 5. Explicación de los parámetros de las llamadas al sistema

Se detalla cómo funcionan `open()`, `write()`, `read()` y `close()`, explicando sus parámetros y su impacto en el manejo de archivos.

## 6. Conclusiones

El uso de archivos planos y binarios en **Linux** es fundamental en la programación de sistemas. Comprender los permisos y el uso de llamadas al sistema permite una manipulación segura y eficiente de los archivos. ¡Practica estos comandos y sigue explorando!

