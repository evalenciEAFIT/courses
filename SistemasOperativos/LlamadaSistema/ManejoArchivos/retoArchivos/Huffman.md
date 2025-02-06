# Algoritmo de Compresión de Huffman en C++

## 1. Introducción
El algoritmo de **Huffman** es un método de compresión sin pérdida que asigna códigos binarios de longitud variable a los caracteres en función de su frecuencia en el archivo de entrada. Los caracteres más frecuentes reciben códigos más cortos, mientras que los menos frecuentes tienen códigos más largos. Este enfoque permite reducir significativamente el tamaño de los datos.

El algoritmo se basa en la construcción de un **árbol binario de Huffman**, en el cual cada nodo representa un carácter y su frecuencia de aparición. Una vez construido el árbol, se asignan códigos binarios a cada carácter, reemplazando así los caracteres originales por sus respectivas representaciones en bits.

---

## 2. Pseudocódigo del Algoritmo de Huffman

```plaintext
1. Contar la frecuencia de cada carácter en la entrada y almacenarlo en un diccionario.
2. Insertar cada carácter en una cola de prioridad como un nodo hoja.
3. Mientras haya más de un nodo en la cola de prioridad:
   a. Extraer los dos nodos con menor frecuencia.
   b. Crear un nuevo nodo padre con la suma de sus frecuencias.
   c. Insertar el nuevo nodo en la cola de prioridad.
4. Generar los códigos de Huffman recorriendo el árbol desde la raíz.
5. Construir una tabla de asignación de códigos binarios a los caracteres.
6. Sustituir cada carácter en el texto original por su código de Huffman.
7. Guardar los códigos y el texto comprimido en un archivo binario.
```

---

## 3. Ejemplo Paso a Paso
### **Ejemplo:**
Supongamos que tenemos el siguiente texto: **"BEEEP"**.

1. **Cálculo de frecuencias:**
   ```
   B: 1
   E: 3
   P: 1
   ```
2. **Construcción del Árbol de Huffman:**
   - Se crean nodos hoja para cada carácter.
   - Se combinan los nodos de menor frecuencia.
   - Se genera el árbol con la raíz que suma todas las frecuencias.

3. **Generación de Códigos:**
   - `E` → `0`
   - `B` → `10`
   - `P` → `11`

4. **Texto comprimido:**
   ```
   BEEEP → 10 0 0 0 11
   ```
   
Este código comprimido ocupa menos espacio que la representación original.

---

## 4. Implementación en C++
```cpp
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

// Definición de la estructura del nodo del árbol de Huffman
struct Nodo {
    char caracter;
    int frecuencia;
    Nodo* izquierda;
    Nodo* derecha;

    // Constructor del nodo
    Nodo(char c, int f, Nodo* izq = nullptr, Nodo* der = nullptr) 
        : caracter(c), frecuencia(f), izquierda(izq), derecha(der) {}
};

// Comparador para la cola de prioridad, ordena por frecuencia ascendente
struct Comparar {
    bool operator()(Nodo* a, Nodo* b) {
        return a->frecuencia > b->frecuencia; // Menor frecuencia tiene mayor prioridad
    }
};

// Construcción del árbol de Huffman basado en las frecuencias
Nodo* construirArbolHuffman(const std::unordered_map<char, int>& frecuencias) {
    std::priority_queue<Nodo*, std::vector<Nodo*>, Comparar> cola;

    // Insertamos cada carácter en la cola de prioridad
    for (auto& par : frecuencias)
        cola.push(new Nodo(par.first, par.second));

    // Construcción del árbol de Huffman
    while (cola.size() > 1) {
        Nodo* izquierda = cola.top(); cola.pop(); // Extrae el nodo con menor frecuencia
        Nodo* derecha = cola.top(); cola.pop();   // Extrae el segundo nodo con menor frecuencia

        // Se crea un nuevo nodo padre con la suma de las frecuencias
        Nodo* padre = new Nodo('\0', izquierda->frecuencia + derecha->frecuencia, izquierda, derecha);
        cola.push(padre);
    }

    return cola.top(); // Raíz del árbol de Huffman
}

// Generación de los códigos de Huffman recorriendo el árbol
void generarCodigos(Nodo* raiz, std::string codigo, std::unordered_map<char, std::string>& codigos) {
    if (!raiz) return;

    // Si el nodo no es nulo y es una hoja, almacenar el código generado
    if (raiz->caracter != '\0')
        codigos[raiz->caracter] = codigo;
    
    // Recorrer la izquierda con un "0"
    generarCodigos(raiz->izquierda, codigo + "0", codigos);
    // Recorrer la derecha con un "1"
    generarCodigos(raiz->derecha, codigo + "1", codigos);
}
```

---

## 5. Fuente de Consulta
Este documento se basa en el libro:
- **A Concise Introduction to Data Compression**
  [Enlace](https://link-springer-com.ezproxy.eafit.edu.co/book/10.1007/978-1-84800-072-8)

Esta fuente ofrece una explicación detallada sobre los métodos de compresión de datos y su implementación en sistemas computacionales.

---

## 6. Conclusión
El algoritmo de Huffman es una técnica eficiente para la compresión de datos sin pérdida. Se usa ampliamente en formatos de compresión como **ZIP, PNG y MP3** debido a su capacidad de reducir el tamaño de los archivos sin perder información. Además, su implementación en C++ permite su integración en sistemas de almacenamiento y transmisión de datos eficientes.

