***

# Guía Completa de Estructuras de Datos en C

Esta guía proporciona una explicación detallada de las estructuras de datos más utilizadas. Para cada estructura, se incluye la implementación en C con comentarios extensos que explican qué sucede en la memoria durante cada operación.

---

## Tabla de Contenidos
1. [Arreglos Dinámicos](#1-arreglos-dinámicos)
2. [Listas Simplemente Enlazadas](#2-listas-simplemente-enlazadas)
3. [Listas Doblemente Enlazadas](#3-listas-doblemente-enlazadas)
4. [Listas Circulares](#4-listas-circulares)
5. [Pilas (Stacks)](#5-pilas-stacks)
6. [Colas (Queues)](#6-colas-queues)
7. [Tablas Hash (Hash Tables)](#7-tablas-hash-hash-tables)
8. [Árboles Binarios de Búsqueda (BST)](#8-árboles-binarios-de-búsqueda-bst)

---

## 1. Arreglos Dinámicos

### Concepto
Simulamos un vector que puede crecer usando `realloc` para redimensionar la memoria cuando se llena. Los datos están en memoria contigua.

### Código en C Comentado

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* elementos; // Puntero al bloque de memoria de los datos
    int tamaño;     // Capacidad total actual del arreglo
    int cantidad;   // Cantidad de elementos usados actualmente
} ArregloDinamico;

// Inicializar el arreglo reservando memoria inicial
ArregloDinamico* crearArreglo() {
    ArregloDinamico* a = (ArregloDinamico*)malloc(sizeof(ArregloDinamico));
    // Empezamos con capacidad para 2 enteros
    a->elementos = (int*)malloc(2 * sizeof(int)); 
    a->tamaño = 2;
    a->cantidad = 0;
    return a;
}

// ADICIONAR
void adicionar(ArregloDinamico* a, int valor) {
    // Paso 1: Verificar si hay espacio libre
    if (a->cantidad == a->tamaño) {
        // Paso 2: Si no hay espacio, duplicamos el tamaño
        a->tamaño *= 2;
        // Paso 3: realloc busca un nuevo bloque de memoria más grande y copia los datos viejos
        a->elementos = (int*)realloc(a->elementos, a->tamaño * sizeof(int));
    }
    // Paso 4: Guardar el valor en la siguiente posición libre
    a->elementos[a->cantidad] = valor;
    // Paso 5: Incrementar el contador de elementos
    a->cantidad++;
    printf("Adicionado: %d\n", valor);
}

// RETIRAR (por índice)
void retirar(ArregloDinamico* a, int indice) {
    if (indice < 0 || indice >= a->cantidad) return; // Verificar límites
    
    printf("Retirado: %d\n", a->elementos[indice]);
    
    // Paso 1: Mover todos los elementos una posición hacia la izquierda
    // Esto sobrescribe el elemento que queremos borrar
    for (int i = indice; i < a->cantidad - 1; i++) {
        a->elementos[i] = a->elementos[i + 1];
    }
    // Paso 2: Reducir el contador de elementos (el espacio físico queda reservado pero no usado)
    a->cantidad--;
}

// BUSCAR
int buscar(ArregloDinamico* a, int valor) {
    // Recorremos secuencialmente hasta encontrar el valor
    for (int i = 0; i < a->cantidad; i++) {
        if (a->elementos[i] == valor) return i; // Retorna el índice si lo encuentra
    }
    return -1; // -1 indica no encontrado
}

// RECORRER
void recorrer(ArregloDinamico* a) {
    printf("Arreglo: ");
    // Itera desde 0 hasta la cantidad actual de elementos
    for (int i = 0; i < a->cantidad; i++) {
        printf("%d ", a->elementos[i]);
    }
    printf("\n");
}

int main() {
    ArregloDinamico* arr = crearArreglo();
    adicionar(arr, 10);
    adicionar(arr, 20);
    adicionar(arr, 30);
    recorrer(arr);
    
    retirar(arr, 1); // Quita el 20
    recorrer(arr);
    return 0;
}
```

---

## 2. Listas Simplemente Enlazadas

### Concepto
Nodos dispersos en memoria. Cada nodo tiene un dato y un puntero `siguiente`.

### Código en C Comentado

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct Nodo {
    int dato;
    struct Nodo* siguiente; // Puntero al siguiente nodo
} Nodo;

Nodo* cabeza = NULL; // Puntero global al inicio de la lista

// ADICIONAR al final
void adicionar(int valor) {
    // Paso 1: Crear el nuevo nodo en memoria
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    nuevo->dato = valor;
    nuevo->siguiente = NULL; // Como será el último, apunta a NULL

    // Paso 2: Si la lista está vacía, el nuevo nodo es la cabeza
    if (cabeza == NULL) {
        cabeza = nuevo;
    } else {
        // Paso 3: Si no está vacía, recorremos hasta encontrar el último nodo
        Nodo* temp = cabeza;
        while (temp->siguiente != NULL) {
            temp = temp->siguiente;
        }
        // Paso 4: Enlazamos el último nodo actual con el nuevo
        temp->siguiente = nuevo;
    }
    printf("Adicionado: %d\n", valor);
}

// RETIRAR por valor
void retirar(int valor) {
    if (cabeza == NULL) return; // Lista vacía

    // Caso especial: Borrar la cabeza
    if (cabeza->dato == valor) {
        Nodo* temp = cabeza;          // Guardamos la cabeza para borrarla luego
        cabeza = cabeza->siguiente;   // La cabeza pasa a ser el siguiente
        free(temp);                   // Liberamos memoria
        printf("Retirado: %d\n", valor);
        return;
    }

    // Caso general: Buscar el nodo ANTERIOR al que queremos borrar
    Nodo* actual = cabeza;
    // Avanzamos mientras el siguiente exista y no sea el valor buscado
    while (actual->siguiente != NULL && actual->siguiente->dato != valor) {
        actual = actual->siguiente;
    }

    // Si encontramos el siguiente nodo, significa que este es el que precede al que queremos borrar
    if (actual->siguiente != NULL) {
        Nodo* temp = actual->siguiente; // Nodo a borrar
        // Saltamos el nodo: actual -> siguiente.siguiente
        actual->siguiente = temp->siguiente;
        free(temp);
        printf("Retirado: %d\n", valor);
    }
}

// BUSCAR
int buscar(int valor) {
    Nodo* temp = cabeza;
    int indice = 0;
    while (temp != NULL) {
        if (temp->dato == valor) return indice; // Encontrado
        temp = temp->siguiente; // Avanzar puntero
        indice++;
    }
    return -1;
}

// RECORRER
void recorrer() {
    Nodo* temp = cabeza;
    printf("Lista: ");
    while (temp != NULL) { // Mientras no lleguemos al final (NULL)
        printf("%d -> ", temp->dato);
        temp = temp->siguiente; // Ir al siguiente
    }
    printf("NULL\n");
}
```

---

## 3. Listas Doblemente Enlazadas

### Concepto
Cada nodo tiene un puntero `siguiente` y otro `anterior`. Permite navegar hacia adelante y atrás.

### Código en C Comentado

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct NodoDoble {
    int dato;
    struct NodoDoble* siguiente;
    struct NodoDoble* anterior; // Puntero extra hacia atrás
} NodoDoble;

NodoDoble* cabeza = NULL;

// ADICIONAR
void adicionar(int valor) {
    NodoDoble* nuevo = (NodoDoble*)malloc(sizeof(NodoDoble));
    nuevo->dato = valor;
    nuevo->siguiente = NULL;
    nuevo->anterior = NULL;

    if (cabeza == NULL) {
        cabeza = nuevo;
    } else {
        // Buscar el último nodo
        NodoDoble* temp = cabeza;
        while (temp->siguiente != NULL) temp = temp->siguiente;
        
        // Enlace doble:
        temp->siguiente = nuevo; // El antiguo último apunta al nuevo
        nuevo->anterior = temp;  // El nuevo apunta hacia atrás al antiguo último
    }
}

// RETIRAR
void retirar(int valor) {
    NodoDoble* temp = cabeza;
    // Buscar el nodo por su valor
    while (temp != NULL && temp->dato != valor) {
        temp = temp->siguiente;
    }
    if (temp == NULL) return; // No encontrado

    // Si hay un nodo anterior, le decimos que apunte al siguiente del que borramos
    if (temp->anterior != NULL) 
        temp->anterior->siguiente = temp->siguiente;
    else 
        cabeza = temp->siguiente; // Si no hay anterior, borramos la cabeza

    // Si hay un nodo siguiente, le decimos que apunte al anterior del que borramos
    if (temp->siguiente != NULL) 
        temp->siguiente->anterior = temp->anterior;
    
    free(temp);
}

// RECORRER
void recorrer() {
    NodoDoble* temp = cabeza;
    printf("Lista Doble: ");
    while (temp != NULL) {
        printf("<-|%d|-> ", temp->dato);
        temp = temp->siguiente;
    }
    printf("\n");
}
```

---

## 4. Listas Circulares

### Concepto
El último nodo apunta al primero (`cabeza`). No hay fin. Para recorrerla, debemos asegurarnos de no entrar en bucle infinito.

### Código en C Comentado

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct NodoC {
    int dato;
    struct NodoC* siguiente;
} NodoC;

// Apuntamos al ÚLTIMO nodo porque desde ahí podemos acceder al primero fácilmente (ultimo->siguiente)
NodoC* ultimo = NULL; 

// ADICIONAR
void adicionar(int valor) {
    NodoC* nuevo = (NodoC*)malloc(sizeof(NodoC));
    nuevo->dato = valor;
    
    if (ultimo == NULL) {
        // Si está vacía, el nuevo apunta a sí mismo
        ultimo = nuevo;
        nuevo->siguiente = nuevo; 
    } else {
        // El nuevo apunta a la cabeza (que es ultimo->siguiente)
        nuevo->siguiente = ultimo->siguiente;
        // El antiguo último apunta al nuevo
        ultimo->siguiente = nuevo;
        // Actualizamos quién es el último
        ultimo = nuevo;
    }
}

// RETIRAR
void retirar(int valor) {
    if (ultimo == NULL) return;
    
    NodoC* actual = ultimo->siguiente; // Empezamos en la "cabeza"
    NodoC* previo = ultimo;           // El anterior es el último (por ser circular)
    
    do {
        if (actual->dato == valor) {
            // Caso: Solo hay un nodo
            if (actual == ultimo && actual->siguiente == actual) {
                ultimo = NULL;
            } else {
                // Enlazamos el previo con el siguiente, saltando el actual
                previo->siguiente = actual->siguiente;
                // Si borramos el nodo que era el último, actualizamos la referencia global
                if (actual == ultimo) ultimo = previo;
            }
            free(actual);
            return;
        }
        previo = actual;
        actual = actual->siguiente;
    } while(actual != ultimo->siguiente); // Repetir hasta dar la vuelta completa
}

// RECORRER
void recorrer() {
    if (ultimo == NULL) return;
    NodoC* temp = ultimo->siguiente; // Empezar en la cabeza
    printf("Lista Circular: ");
    do {
        printf("%d ", temp->dato);
        temp = temp->siguiente;
    } while (temp != ultimo->siguiente); // Parar cuando volvamos al inicio
    printf("(Ciclo)\n");
}
```

---

## 5. Pilas (Stacks) - LIFO

### Concepto
Último en entrar, primero en salir. Solo operamos sobre el "tope" de la pila.

### Código en C Comentado

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct NodoP {
    int dato;
    struct NodoP* siguiente;
} NodoP;

NodoP* tope = NULL; // Puntero que representa la parte superior de la pila

// ADICIONAR (Push)
void adicionar(int valor) {
    // Paso 1: Crear nodo
    NodoP* nuevo = (NodoP*)malloc(sizeof(NodoP));
    nuevo->dato = valor;
    
    // Paso 2: El nuevo nodo apunta al que era el tope anterior
    nuevo->siguiente = tope;
    
    // Paso 3: El tope ahora es el nuevo nodo
    tope = nuevo;
    printf("Push: %d\n", valor);
}

// RETIRAR (Pop)
void retirar() {
    if (tope == NULL) {
        printf("Pila vacía\n");
        return;
    }
    // Paso 1: Guardamos referencia al tope actual para poder borrarlo
    NodoP* temp = tope;
    
    // Paso 2: El tope baja al siguiente nodo
    tope = tope->siguiente;
    
    // Paso 3: Liberar memoria
    printf("Pop: %d\n", temp->dato);
    free(temp);
}

// BUSCAR (Nota: En pilas, buscar no es lo habitual, pero es posible)
int buscar(int valor) {
    NodoP* temp = tope;
    int profundidad = 0;
    while (temp != NULL) {
        if (temp->dato == valor) return profundidad;
        temp = temp->siguiente; // Bajar en la pila
        profundidad++;
    }
    return -1;
}

// RECORRER
void recorrer() {
    NodoP* temp = tope;
    printf("Pila (Tope -> Base): ");
    while (temp != NULL) {
        printf("%d ", temp->dato);
        temp = temp->siguiente;
    }
    printf("\n");
}
```

---

## 6. Colas (Queues) - FIFO

### Concepto
Primero en entrar, primero en salir. Tenemos un puntero `frente` (para sacar) y un puntero `final` (para meter).

### Código en C Comentado

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct NodoCola {
    int dato;
    struct NodoCola* siguiente;
} NodoCola;

NodoCola* frente = NULL;
NodoCola* final = NULL;

// ADICIONAR (Enqueue)
void adicionar(int valor) {
    NodoCola* nuevo = (NodoCola*)malloc(sizeof(NodoCola));
    nuevo->dato = valor;
    nuevo->siguiente = NULL;
    
    // Si la cola está vacía, el nuevo nodo es tanto el frente como el final
    if (final == NULL) {
        frente = final = nuevo;
    } else {
        // Enlazamos el actual final con el nuevo
        final->siguiente = nuevo;
        // El nuevo nodo se convierte en el nuevo final
        final = nuevo;
    }
}

// RETIRAR (Dequeue)
void retirar() {
    if (frente == NULL) return;
    
    // Guardamos el nodo a borrar
    NodoCola* temp = frente;
    
    // El frente avanza al siguiente nodo
    frente = frente->siguiente;
    
    // Si al avanzar el frente quedamos vacíos, el final también debe ser NULL
    if (frente == NULL) final = NULL;
    
    free(temp);
}

// BUSCAR
int buscar(int valor) {
    NodoCola* temp = frente;
    int pos = 0;
    // Recorremos desde el frente hacia atrás
    while(temp != NULL) {
        if(temp->dato == valor) return pos;
        temp = temp->siguiente;
        pos++;
    }
    return -1;
}

// RECORRER
void recorrer() {
    NodoCola* temp = frente;
    printf("Cola (Frente -> Final): ");
    while(temp != NULL) {
        printf("%d ", temp->dato);
        temp = temp->siguiente;
    }
    printf("\n");
}
```

---

## 7. Tablas Hash (Hash Tables)

### Concepto
Usamos una función matemática (hash) para convertir una clave en un índice de arreglo. Si dos claves caen en el mismo índice (colisión), usamos listas enlazadas en ese índice.

### Código en C Comentado

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANO 10 // Tamaño fijo del arreglo principal

typedef struct ItemHash {
    char clave[20]; // Clave (ej: "edad")
    int valor;      // Valor (ej: 25)
    struct ItemHash* siguiente; // Puntero para manejar colisiones (Chaining)
} ItemHash;

// Arreglo de punteros (inicializado en NULL)
ItemHash* tablaHash[TAMANO] = {NULL};

// FUNCIÓN HASH: Convierte una string en un número de índice
int hash(char* clave) {
    unsigned long hash = 0;
    // Sumar los valores ASCII de los caracteres
    for (int i = 0; clave[i]; i++) hash += clave[i];
    // Módulo para asegurar que el índice esté dentro del rango (0 a 9)
    return hash % TAMANO;
}

// ADICIONAR
void adicionar(char* clave, int valor) {
    // Paso 1: Obtener el índice donde debería ir este dato
    unsigned int indice = hash(clave);
    
    // Paso 2: Crear el nuevo item
    ItemHash* nuevo = (ItemHash*)malloc(sizeof(ItemHash));
    strcpy(nuevo->clave, clave);
    nuevo->valor = valor;
    
    // Paso 3: Insertar al INICIO de la lista enlazada en ese índice
    // Esto maneja las colisiones simplemente encadenando los nodos
    nuevo->siguiente = tablaHash[indice];
    tablaHash[indice] = nuevo;
    printf("Hash '%s' -> Indice %d\n", clave, indice);
}

// RETIRAR
void retirar(char* clave) {
    unsigned int indice = hash(clave);
    ItemHash* actual = tablaHash[indice];
    ItemHash* prev = NULL;

    // Recorrer la lista enlazada solo en ese índice
    while (actual != NULL) {
        if (strcmp(actual->clave, clave) == 0) {
            // Encontrado: desconectar nodo
            if (prev == NULL) {
                // Es el primero de la lista en ese índice
                tablaHash[indice] = actual->siguiente;
            } else {
                // Está en medio o al final de la lista
                prev->siguiente = actual->siguiente;
            }
            free(actual);
            return;
        }
        prev = actual;
        actual = actual->siguiente;
    }
}

// BUSCAR
int buscar(char* clave) {
    unsigned int indice = hash(clave);
    ItemHash* actual = tablaHash[indice];
    // Solo buscamos en la "cuba" (bucket) correspondiente al hash
    while (actual != NULL) {
        if (strcmp(actual->clave, clave) == 0) return actual->valor;
        actual = actual->siguiente;
    }
    return -1;
}

// RECORRER
void recorrer() {
    printf("--- Tabla Hash ---\n");
    for (int i = 0; i < TAMANO; i++) {
        ItemHash* temp = tablaHash[i];
        if (temp != NULL) {
            printf("[%d] -> ", i); // Imprimir índice
            // Imprimir la cadena de colisiones si existen
            while (temp != NULL) {
                printf("(%s:%d) -> ", temp->clave, temp->valor);
                temp = temp->siguiente;
            }
            printf("NULL\n");
        }
    }
}
```

---

## 8. Árboles Binarios de Búsqueda (BST)

### Concepto
Árbol jerárquico donde el hijo izquierdo es menor que el padre, y el derecho es mayor. La eliminación es la operación más compleja.

### Código en C Comentado

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct NodoArbol {
    int dato;
    struct NodoArbol* izq;
    struct NodoArbol* der;
} NodoArbol;

NodoArbol* raiz = NULL;

// Crear nodo auxiliar
NodoArbol* crearNodo(int valor) {
    NodoArbol* n = (NodoArbol*)malloc(sizeof(NodoArbol));
    n->dato = valor;
    n->izq = n->der = NULL;
    return n;
}

// ADICIONAR (Recursivo)
NodoArbol* adicionar(NodoArbol* nodo, int valor) {
    // Paso 1: Si llegamos a un punto NULL, ahí va el nuevo nodo
    if (nodo == NULL) return crearNodo(valor);

    // Paso 2: Comparar con el nodo actual
    if (valor < nodo->dato) {
        // Si es menor, vamos a la izquierda recursivamente
        nodo->izq = adicionar(nodo->izq, valor);
    } else if (valor > nodo->dato) {
        // Si es mayor, vamos a la derecha recursivamente
        nodo->der = adicionar(nodo->der, valor);
    }
    // Si es igual, no hacemos nada (suponemos valores únicos)
    return nodo;
}

// BUSCAR (Recursivo)
NodoArbol* buscar(NodoArbol* nodo, int valor) {
    // Si el nodo es NULL o encontramos el valor, lo retornamos
    if (nodo == NULL || nodo->dato == valor) return nodo;

    // Si el valor es menor que el actual, buscar a la izquierda
    if (valor < nodo->dato) return buscar(nodo->izq, valor);
    
    // Si es mayor, buscar a la derecha
    return buscar(nodo->der, valor);
}

// Función auxiliar para encontrar el valor MÁS PEQUEÑO de un subárbol
// Se usa para encontrar el reemplazante cuando borramos un nodo con 2 hijos
NodoArbol* minValorNodo(NodoArbol* nodo) {
    NodoArbol* actual = nodo;
    // El valor más pequeño siempre está lo más a la izquierda posible
    while (actual && actual->izq != NULL) actual = actual->izq;
    return actual;
}

// RETIRAR
NodoArbol* retirar(NodoArbol* raiz, int valor) {
    // Paso 1: Buscar el nodo a borrar
    if (raiz == NULL) return raiz;

    if (valor < raiz->dato)
        raiz->izq = retirar(raiz->izq, valor);
    else if (valor > raiz->dato)
        raiz->der = retirar(raiz->der, valor);
    else {
        // ¡Encontrado el nodo a borrar!
        
        // Caso A: Solo tiene un hijo (o ninguno)
        if (raiz->izq == NULL) {
            NodoArbol* temp = raiz->der;
            free(raiz);
            return temp; // Retorna el hijo derecho para conectarlo con el abuelo
        } else if (raiz->der == NULL) {
            NodoArbol* temp = raiz->izq;
            free(raiz);
            return temp;
        }

        // Caso B: Tiene dos hijos
        // Estrategia: Reemplazar con el "Sucesor" (menor valor del subárbol derecho)
        NodoArbol* temp = minValorNodo(raiz->der);
        
        // Copiar el dato del sucesor al nodo actual
        raiz->dato = temp->dato;
        
        // Borrar el sucesor duplicado del subárbol derecho (llamada recursiva)
        raiz->der = retirar(raiz->der, temp->dato);
    }
    return raiz;
}

// RECORRER (In-Order: Izquierda - Raíz - Derecha)
// Este recorrido imprime los datos ordenados de menor a mayor
void recorrer(NodoArbol* nodo) {
    if (nodo != NULL) {
        recorrer(nodo->izq);  // 1. Ir a la izquierda
        printf("%d ", nodo->dato); // 2. Imprimir
        recorrer(nodo->der);  // 3. Ir a la derecha
    }
}
```
