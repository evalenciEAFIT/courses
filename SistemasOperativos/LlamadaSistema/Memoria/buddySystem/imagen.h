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
    void mostrarInfo() const;  // ✅ Declaración como const

private:
    int alto;
    int ancho;
    int canales;
    unsigned char ***pixeles;
    BuddyAllocator *allocador;

    void convertirBufferAMatriz(unsigned char* buffer); // ✅ Declaración privada
};

#endif
