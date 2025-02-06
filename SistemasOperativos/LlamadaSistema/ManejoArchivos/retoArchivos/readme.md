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

### **1.2. `ManejadorArchivo.cpp` (Implementación)**
```cpp
#include "ManejadorArchivo.h"

bool ManejadorArchivo::leerArchivo(const std::string& nombreArchivo) {
    int fd = open(nombreArchivo.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return false;
    }
    datos.clear();
    uint8_t buffer[1024];
    ssize_t bytesLeidos;
    while ((bytesLeidos = read(fd, buffer, sizeof(buffer))) > 0) {
        datos.insert(datos.end(), buffer, buffer + bytesLeidos);
    }
    close(fd);
    return true;
}

void ManejadorArchivo::convertirABits() {
    bits.clear();
    for (uint8_t byte : datos) {
        bits.push_back(std::bitset<8>(byte).to_string());
    }
}

void ManejadorArchivo::mostrarBits() const {
    std::cout << "Contenido en bits:\n[";
    for (const auto& bitString : bits) {
        std::cout << bitString << "]\n[";
    }
    std::cout << "]" << std::endl;
}

void ManejadorArchivo::escribirArchivo(const std::string& nombreSalida) {
    int fd = open(nombreSalida.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        std::cerr << "Error: No se pudo abrir el archivo de salida " << nombreSalida << std::endl;
        return;
    }
    ssize_t bytesEscritos = write(fd, datos.data(), datos.size());
    close(fd);
}
```

---

### **2. Algoritmos de Compresión y Encriptación**

#### **2.1. Algoritmos de Compresión**
- **Huffman**: Codificación basada en la frecuencia de aparición de caracteres.
- **LZW**: Algoritmo de compresión sin pérdida usado en GIF y archivos ZIP.
- **RLE (Run-Length Encoding)**: Compresión simple que almacena repeticiones consecutivas.

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

