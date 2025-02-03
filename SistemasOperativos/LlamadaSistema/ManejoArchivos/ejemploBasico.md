# Uso de llamadas al sistema en Linux para manejo de archivos

Este documento describe el uso de llamadas al sistema en Linux para la manipulación de archivos binarios y de texto. Se incluyen cuatro programas en C++ que utilizan exclusivamente llamadas al sistema para la lectura y escritura de archivos, tanto en formato binario como en texto plano.

## Introducción

Las llamadas al sistema permiten interactuar directamente con el núcleo de Linux para realizar operaciones de bajo nivel en archivos. A diferencia de las funciones estándar de C++, estas llamadas ofrecen mayor control y eficiencia al evitar la sobrecarga del sistema de entrada/salida de alto nivel.

Las funciones principales utilizadas en estos programas son:

- `open()`: Abre un archivo con opciones específicas.
- `read()`: Lee datos desde un archivo.
- `write()`: Escribe datos en un archivo.
- `close()`: Cierra un archivo.

A continuación, se presentan los programas con su respectiva explicación y comentarios detallados en las líneas de código.

---

## 1. Escritura en Archivo Binario (`BinarioWrite.cpp`)

Este programa permite escribir datos ingresados por el usuario en un archivo binario utilizando llamadas al sistema.

### Código:

```cpp
#include <fcntl.h>    // Biblioteca para manejo de archivos (open, permisos)
#include <unistd.h>   // Biblioteca para llamadas al sistema (write, close)
#include <iostream>
#include <string.h>

int main() {
    const char* nombre_archivo = "salida.bin";
    
    // Abrir el archivo en modo escritura binaria.
    // O_WRONLY: Solo escritura.
    // O_CREAT: Crear el archivo si no existe.
    // O_TRUNC: Si el archivo existe, se sobrescribe.
    // 0644: Permisos de lectura y escritura para el usuario, y solo lectura para el grupo y otros.
    int fd = open(nombre_archivo, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error al abrir el archivo");
        return 1;
    }
    
    char buffer[256];
    std::cout << "Ingrese un texto para guardar en binario: ";
    std::cin.getline(buffer, sizeof(buffer)); // Leer entrada del usuario
    
    // Escribir los datos ingresados en el archivo binario.
    write(fd, buffer, strlen(buffer));
    
    // Cerrar el archivo para liberar recursos.
    close(fd);
    return 0;
}
```

---

## 2. Lectura de Archivo Binario (`BinarioLeer.cpp`)

Este programa lee un archivo binario y muestra cada byte en ASCII, binario y hexadecimal.

### Código:

```cpp
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

int main() {
    const char* nombre_archivo = "salida.bin";
    
    // Abrir el archivo en modo solo lectura
    int fd = open(nombre_archivo, O_RDONLY);
    if (fd == -1) {
        write(STDERR_FILENO, "Error al abrir el archivo\n", 26);
        return 1;
    }
    
    unsigned char byte;
    ssize_t bytes_leidos;
    const char* encabezado = "Caracter :: Binario      :: Hexadecimal\n";
    write(STDOUT_FILENO, encabezado, strlen(encabezado));
    
    while ((bytes_leidos = read(fd, &byte, 1)) > 0) {
        char buffer_salida[50];
        char binario[9];
        
        // Convertir manualmente cada byte a binario
        for (int i = 7; i >= 0; --i)
            binario[7 - i] = ((byte >> i) & 1) ? '1' : '0';
        binario[8] = '\0';
        
        // Formatear la salida con información ASCII, binaria y hexadecimal
        int len = sprintf(buffer_salida, "  %c      :: %s :: 0x%02X\n", byte >= 32 ? byte : '.', binario, byte);
        write(STDOUT_FILENO, buffer_salida, len);
    }
    
    // Cerrar el archivo
    close(fd);
    return 0;
}
```

---

## 3. Escritura en Archivo de Texto (`PlanoWrite.cpp`)

Este programa escribe texto ingresado por el usuario en un archivo de texto plano.

### Código:

```cpp
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string>

int main() {
    // Abrir el archivo en modo escritura con permisos adecuados
    int fd = open("archivo.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error creando el archivo");
        return 1;
    }
    
    std::string entrada;
    std::cout << "Ingrese el texto a guardar: ";
    std::getline(std::cin, entrada);
    
    // Escribir la entrada en el archivo
    write(fd, entrada.c_str(), entrada.size());
    
    // Cerrar el archivo
    close(fd);
    return 0;
}
```

---

## 4. Lectura de Archivo de Texto (`PlanoRead.cpp`)

Este programa lee un archivo de texto y muestra su contenido en la terminal.

### Código:

```cpp
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main() {
    // Abrir archivo en modo lectura
    int fd = open("archivo.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error abriendo el archivo");
        return 1;
    }
    
    char buffer[128];
    ssize_t bytes_leidos = read(fd, buffer, sizeof(buffer));
    
    // Mostrar el contenido en pantalla
    write(STDOUT_FILENO, buffer, bytes_leidos);
    
    // Cerrar archivo
    close(fd);
    return 0;
}
```

---

## Conclusión

Estos programas demuestran cómo utilizar llamadas al sistema en Linux para manipular archivos de manera eficiente y sin la sobrecarga de funciones estándar de C++. Usar `open()`, `read()`, `write()` y `close()` permite un mayor control sobre los archivos y su acceso.

Para ejecutar estos programas, compílalos con `g++` y pruébalos en un entorno Linux.

