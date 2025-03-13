#ifndef BUDDY_ALLOCATOR_H
#define BUDDY_ALLOCATOR_H

#include <cstddef>

class BuddyAllocator {
public:
    // Constructor: asigna un bloque de memoria de tamaño especificado.
    BuddyAllocator(size_t size);

    // Destructor: libera el bloque de memoria.
    ~BuddyAllocator();

    // Asigna un bloque de memoria del tamaño solicitado.
    void* alloc(size_t size);

    // Libera el bloque de memoria (sin efecto en esta implementación).
    void free(void* ptr);

private:
    size_t size;         // Tamaño total de la memoria gestionada
    void* memoriaBase;   // Puntero al bloque de memoria base
};

#endif
