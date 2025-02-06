# Manejo de Archivos en C++ con Llamadas al Sistema

## Introducción
Este documento describe el manejo de archivos en C++ utilizando **llamadas al sistema** en Linux. Se presentan las estructuras de código necesarias para leer y escribir archivos binarios y de texto, así como su representación en bits.

Además, se incluyen **algoritmos clásicos de compresión y encriptación**, junto con referencias bibliográficas para un estudio más profundo.

---

## 1. Archivos de Código
### **1.1. `ManejadorArchivo.h` (Encabezado)**
```cpp
#ifndef MANEJADOR_ARCHIVO_H
#define MANEJADOR_ARCHIVO_H

// =====================================
//  LIBRERÍAS UTILIZADAS Y SU PROPÓSITO
// =====================================
#include <iostream>    // Para entrada/salida estándar (std::cout, std::cerr)
#include <vector>      // Para manejar estructuras de datos dinámicas
#include <string>      // Para manejar cadenas de texto
#include <fcntl.h>     // Para usar `open()`, `O_RDONLY`, `O_WRONLY`, `O_CREAT`
#include <unistd.h>    // Para usar `read()`, `write()`, `close()`
#include <bitset>      // Para representar cada byte en su forma binaria (8 bits)
#include <cstdint>     // Para usar tipos de datos de tamaño fijo como uint8_t

// =====================================
//  CLASE MANEJADORARCHIVO
// =====================================
// Clase que permite leer archivos en modo binario o texto, convertirlos en bits
// y escribirlos en otro archivo.
class ManejadorArchivo {
private:
    std::vector<uint8_t> datos;  // Vector para almacenar el contenido del archivo en bytes
    std::vector<std::string> bits; // Vector para almacenar la representación en bits de cada byte

public:
    bool leerArchivo(const std::string& nombreArchivo);  // Lee el archivo y almacena los bytes
    void convertirABits();   // Convierte los bytes almacenados en bits
    void escribirArchivo(const std::string& nombreSalida);  // Escribe los datos en otro archivo
    void mostrarBits() const;  // Muestra en pantalla los bits de cada byte
    std::vector<std::vector<int>> matrizBits();  // Devuelve la matriz de bits (8 bits por fila)
};

#endif
```

---

### **1.2. `ManejadorArchivo.cpp` (Cuerpo: implementación de las funciones)**
```cpp
#include "ManejadorArchivo.h"

// =====================================
//  FUNCIÓN: leerArchivo()
// =====================================
// Propósito: Leer un archivo en modo binario y almacenar su contenido en un vector.
// Parámetro: `nombreArchivo` → Nombre del archivo a leer.
// Retorna: `true` si el archivo fue leído exitosamente, `false` en caso de error.
bool ManejadorArchivo::leerArchivo(const std::string& nombreArchivo) {
    // `open()` abre el archivo en modo solo lectura
    // `O_RDONLY` → Solo lectura
    int fd = open(nombreArchivo.c_str(), O_RDONLY);
    
    if (fd == -1) {  // Si `open()` falla, devuelve `-1`
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return false;
    }

    datos.clear();  // Limpiamos el vector antes de leer nuevos datos
    uint8_t buffer[1024];  // Buffer temporal de 1024 bytes
    ssize_t bytesLeidos;

    // `read(fd, buffer, sizeof(buffer))` lee el archivo en bloques de 1024 bytes
    while ((bytesLeidos = read(fd, buffer, sizeof(buffer))) > 0) {
        datos.insert(datos.end(), buffer, buffer + bytesLeidos);  // Almacena los bytes en `datos`
    }

    // `close(fd)` cierra el archivo para liberar recursos
    close(fd);
    return true;
}

// =====================================
// FUNCIÓN: convertirABits()
// =====================================
// Propósito: Convierte cada byte leído en su representación de 8 bits.
void ManejadorArchivo::convertirABits() {
    bits.clear();  // Limpiar el vector antes de convertir nuevos datos
    
    for (uint8_t byte : datos) {
        bits.push_back(std::bitset<8>(byte).to_string());  // Convierte cada byte a 8 bits
    }
}

// =====================================
// FUNCIÓN: mostrarBits()
// =====================================
// Propósito: Muestra en pantalla los bits de cada byte leído.
void ManejadorArchivo::mostrarBits() const {
    std::cout << "Contenido en bits:\n[";

    for (const auto& bitString : bits) {
        std::cout << bitString << "]\n[";
    }
    std::cout << "]";
    std::cout << std::endl;
}

// =====================================
//  FUNCIÓN: escribirArchivo()
// =====================================
//  Propósito: Escribir los datos en un archivo de salida.
//  Parámetro: `nombreSalida` → Nombre del archivo de salida.

void ManejadorArchivo::escribirArchivo(const std::string& nombreSalida) {
    // `open()` abre el archivo para escritura (creándolo si no existe)
    // `O_WRONLY` → Solo escritura
    // `O_CREAT` → Crea el archivo si no existe
    // `O_TRUNC` → Si el archivo existe, lo sobreescribe
    // `0644` → Permisos de lectura/escritura para el dueño, lectura para otros
    int fd = open(nombreSalida.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    if (fd == -1) {
        std::cerr << "Error: No se pudo abrir el archivo de salida " << nombreSalida << std::endl;
        return;
    }

    // `write(fd, datos.data(), datos.size())` escribe el contenido en el archivo
    ssize_t bytesEscritos = write(fd, datos.data(), datos.size());
    
    if (bytesEscritos == -1) {
        std::cerr << "Error: No se pudo escribir en el archivo " << nombreSalida << std::endl;
    } else {
        std::cout << "Archivo guardado exitosamente: " << nombreSalida << std::endl;
    }

    close(fd);  // Se cierra el archivo para liberar recursos
}

// =====================================
//  FUNCIÓN: matrizBits()
// =====================================
//  Propósito: Convierte los bytes en una matriz de bits (8 bits por fila).
//  Retorna: `std::vector<std::vector<int>>`  Matriz donde cada fila representa 8 bits.
std::vector<std::vector<int>> ManejadorArchivo::matrizBits() {
    std::vector<std::vector<int>> matriz;

    for (uint8_t byte : datos) {
        std::vector<int> filaBits(8);
        for (int i = 7; i >= 0; --i) {
            filaBits[i] = (byte >> i) & 1; // Extrae cada bit del byte
        }
        matriz.push_back(filaBits);
    }
    
    return matriz;
}
```

---
### **1.3. `main.cpp` (programa principal)**
```cpp
#include "ManejadorArchivo.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo_entrada> [archivo_salida]" << std::endl;
        return 1;
    }

    std::string archivoEntrada = argv[1];
    std::string archivoSalida = (argc > 2) ? argv[2] : "salida.bin";

    ManejadorArchivo manejador;

    if (manejador.leerArchivo(archivoEntrada)) {
        manejador.convertirABits();
        manejador.mostrarBits();
        manejador.escribirArchivo(archivoSalida);

        std::vector<std::vector<int>> matriz = manejador.matrizBits();
        std::cout << "\nMatriz de bits del archivo:\n";
        int tamano = matriz.size();
        int indice = 0;
        std::cout << "  Tamaño lista:" << tamano << std::endl;
        for (const auto& letra : matriz) {
            std::cout << "[" << indice << "/" << tamano-1 << "] ";
            for (int bit : letra) 
                std::cout<< bit;
            indice++;
            
            std::cout << std::endl;
        }
    }

    return 0;
}

```

---
### **1.4. `Makefile` (Script para compilar)**
```make
CC = g++
CFLAGS = -Wall -Wextra -std=c++17

all: UsoBits

UsoBits: main.o ManejadorArchivo.o
	$(CC) $(CFLAGS) -o UsoBits main.o ManejadorArchivo.o

main.o: main.cpp ManejadorArchivo.h
	$(CC) $(CFLAGS) -c main.cpp

ManejadorArchivo.o: ManejadorArchivo.cpp ManejadorArchivo.h
	$(CC) $(CFLAGS) -c ManejadorArchivo.cpp

clean:
	rm -f UsoBits *.o

```

---


### **2. Algoritmos de Compresión y Encriptación**

#### **2.1. Algoritmos de Compresión**
- **Huffman**: Codificación basada en la frecuencia de aparición de caracteres.  
  - Ejemplo: [Huffman.md](https://github.com/evalenciEAFIT/courses/blob/main/SistemasOperativos/LlamadaSistema/ManejoArchivos/retoArchivos/Huffman.md)
- **LZW**: Algoritmo de compresión sin pérdida usado en GIF y archivos ZIP.
- **RLE (Run-Length Encoding)**: Compresión simple que almacena repeticiones consecutivas.  
  - Ejemplo: [REL.md](https://github.com/evalenciEAFIT/courses/blob/main/SistemasOperativos/LlamadaSistema/ManejoArchivos/retoArchivos/REL.md)

##### **Pseudocódigo de Huffman:**
```plaintext
1. Calcular la frecuencia de cada símbolo en el archivo.
2. Construir un árbol binario con una cola de prioridad.
3. Asignar códigos binarios más cortos a los símbolos más frecuentes.
4. Comprimir el archivo reemplazando los símbolos por sus códigos binarios.
```

#### **2.2. Algoritmos de Encriptación**
- **AES (Advanced Encryption Standard)**: Cifrado simétrico de 128, 192 o 256 bits.
- **RSA**: Cifrado asimétrico basado en la factorización de números primos.
- **ChaCha20**: Algoritmo de flujo rápido y seguro.

##### **Pseudocódigo de AES:**
```plaintext
1. Expandir la clave utilizando la programación de claves de AES.
2. Dividir el texto en bloques de 128 bits.
3. Para cada bloque:
   a. Aplicar sustitución de bytes.
   b. Permutación de filas.
   c. Mezcla de columnas.
   d. Agregar la clave de ronda.
4. Repetir los pasos del 3 al 10 veces (según el tamaño de la clave).
5. Obtener el texto cifrado final.
```

---

## 3. Instrucciones para Compilar y Ejecutar
```bash
make
./UsoBits archivo.txt salida.bin
```

## 4. Instrucciones para Limpiar el Proyecto
```bash
make clean
```

