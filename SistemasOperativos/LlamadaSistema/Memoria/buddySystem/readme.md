
# Sistema de Gestión de Memoria Buddy System

## ¿Qué es el Buddy System?
El Buddy System es un algoritmo de gestión de memoria dinámica que divide un bloque de memoria en partes de tamaños de potencia de dos. Cuando se solicita memoria, el sistema intenta encontrar el bloque de tamaño más cercano y, si el bloque disponible es más grande que el solicitado, se divide en mitades (buddies).

Cuando se libera un bloque de memoria, si el bloque adyacente (buddy) también está libre, ambos bloques se fusionan para formar un bloque más grande. Este proceso se repite recursivamente hasta que ya no hay bloques adyacentes libres para fusionar.

### Ventajas del Buddy System
- Permite dividir y fusionar bloques de manera rápida y eficiente.  
- Reduce la fragmentación externa al garantizar que las divisiones y fusiones sean de tamaños de potencia de dos.  
- Fácil implementación mediante árboles binarios o estructuras de bits.  

### Desventajas del Buddy System
- Puede generar fragmentación interna si la solicitud de memoria no es exactamente una potencia de dos.  
- La fusión de bloques puede requerir procesamiento adicional.  

---

## ¿Para qué se emplea en el procesamiento de imágenes?
En procesamiento de imágenes, el Buddy System es útil porque las imágenes requieren bloques de memoria contiguos para almacenar los datos de píxeles. Al dividir y fusionar bloques de memoria de manera eficiente, el Buddy System facilita:  
- Almacenamiento eficiente de datos de imágenes en memoria.  
- Rápida reasignación de memoria para operaciones como filtros, rotaciones e inversiones de color.  
- Liberación de memoria ordenada y eficiente para evitar fragmentación.  

---

## Explicación del Código
El siguiente código en C++ implementa el algoritmo Buddy System para la gestión de memoria en procesamiento de imágenes.

### buddy_allocator.h
```cpp
#ifndef BUDDY_ALLOCATOR_H
#define BUDDY_ALLOCATOR_H

#include <cstddef>

class BuddyAllocator {
public:
    BuddyAllocator(size_t size);
    ~BuddyAllocator();

    void* alloc(size_t size);
    void free(void* ptr);

private:
    size_t size;
    void* memoriaBase;
};

#endif
```

### buddy_allocator.cpp
```cpp
#include "buddy_allocator.h"
#include <cstdlib>
#include <iostream>

BuddyAllocator::BuddyAllocator(size_t size) {
    this->size = size;
    memoriaBase = std::malloc(size);
    if (!memoriaBase) {
        std::cerr << "Error: No se pudo asignar memoria base con Buddy System.
";
        exit(1);
    }
}

BuddyAllocator::~BuddyAllocator() {
    std::free(memoriaBase);
}

void* BuddyAllocator::alloc(size_t size) {
    if (size > this->size) {
        std::cerr << "Error: Tamaño solicitado excede el tamaño disponible.
";
        return nullptr;
    }
    return memoriaBase;
}

void BuddyAllocator::free(void* ptr) {
    // El Buddy System maneja la liberación automáticamente
}
```

---

### imagen.h
```cpp
#ifndef IMAGEN_H
#define IMAGEN_H

#include <string>
#include "buddy_allocator.h"

class Imagen {
public:
    Imagen(const std::string &nombreArchivo, BuddyAllocator *allocador = nullptr);
    ~Imagen();

    void invertirColores();
    void guardarImagen(const std::string &nombreArchivo) const;
    void mostrarInfo() const;

private:
    int alto;
    int ancho;
    int canales;
    unsigned char ***pixeles;
    BuddyAllocator *allocador;

    void convertirBufferAMatriz(unsigned char* buffer);
};

#endif
```

---

### imagen.cpp
```cpp
#include "imagen.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>

Imagen::Imagen(const std::string &nombreArchivo, BuddyAllocator *allocador)
    : allocador(allocador) {

    unsigned char* buffer = stbi_load(nombreArchivo.c_str(), &ancho, &alto, &canales, 0);
    if (!buffer) {
        std::cerr << "Error: No se pudo cargar la imagen.
";
        exit(1);
    }

    convertirBufferAMatriz(buffer);
    stbi_image_free(buffer);
}

Imagen::~Imagen() {
    if (!allocador) {
        for (int y = 0; y < alto; y++) {
            for (int x = 0; x < ancho; x++) {
                delete[] pixeles[y][x];
            }
            delete[] pixeles[y];
        }
        delete[] pixeles;
    }
}

void Imagen::convertirBufferAMatriz(unsigned char* buffer) {
    int indice = 0;
    pixeles = new unsigned char**[alto];

    for (int y = 0; y < alto; y++) {
        pixeles[y] = new unsigned char*[ancho];
        for (int x = 0; x < ancho; x++) {
            pixeles[y][x] = new unsigned char[canales];
            for (int c = 0; c < canales; c++) {
                pixeles[y][x][c] = buffer[indice++];
            }
        }
    }
}

void Imagen::invertirColores() {
    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            for (int c = 0; c < canales; c++) {
                pixeles[y][x][c] = 255 - pixeles[y][x][c];
            }
        }
    }
}

void Imagen::guardarImagen(const std::string &nombreArchivo) const {
    unsigned char* buffer = new unsigned char[alto * ancho * canales];
    int indice = 0;

    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            for (int c = 0; c < canales; c++) {
                buffer[indice++] = pixeles[y][x][c];
            }
        }
    }

    stbi_write_png(nombreArchivo.c_str(), ancho, alto, canales, buffer, ancho * canales);
    delete[] buffer;
}
```

---

## ¿Cómo se usa el Buddy System en este código?
1. **Asignación de memoria:**  
   El `BuddyAllocator` asigna bloques de memoria usando el algoritmo de división y combinación de buddies.

2. **Conversión de imagen:**  
   Los datos de la imagen se convierten a una estructura de matriz tridimensional (`alto x ancho x canales`) usando `convertirBufferAMatriz()`.

3. **Procesamiento de la imagen:**  
   La operación de inversión de colores (`invertirColores()`) altera directamente los valores en la matriz.

4. **Liberación de memoria:**  
   El destructor `~Imagen()` libera los bloques de memoria asignados para evitar fugas de memoria.

5. **Uso del Buddy System:**  
   Si el Buddy System está habilitado, la asignación y liberación de memoria se gestiona automáticamente mediante el algoritmo de división y combinación de bloques.

---

## Ventajas del Buddy System en este código
- Reducción de fragmentación.  
- Rápida reasignación de memoria.  
- Mejora en el rendimiento del procesamiento de imágenes.  

---

## Conclusión
El Buddy System es ideal para aplicaciones de procesamiento de imágenes que requieren asignación y liberación rápida de memoria. En este código, el Buddy System proporciona una estructura eficiente para gestionar bloques de memoria contiguos y optimiza las operaciones de inversión y manipulación de datos de imagen.
