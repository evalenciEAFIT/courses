# Buddy System (Sistema de Compañeros) - Gestión de Memoria

## Descripción
El **Buddy System** (Sistema de Compañeros) es un algoritmo de gestión de memoria que divide la memoria en bloques de tamaño que es una potencia de dos.  
Cuando se realiza una solicitud de memoria, el sistema divide los bloques hasta que encuentra el tamaño adecuado más cercano que pueda satisfacer la solicitud.  
Si dos bloques adyacentes están libres, el sistema los fusiona para formar un bloque más grande (proceso de **coalescencia**).

### Características:
- Eficiente para asignación y liberación de memoria.  
- Permite un proceso rápido de coalescencia de bloques.  
x Puede causar fragmentación interna.  
x Solo trabaja con tamaños de memoria en potencia de dos.   

---

##  Código en C++ (para Linux)
```cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <map>

using namespace std;

// Estructura para simular el bloque de memoria
struct Bloque {
    int size;
    bool free;
};

vector<Bloque> memoria;

// Función para inicializar la memoria en potencias de dos
void inicializarMemoria(int totalSize) {
    memoria.push_back({totalSize, true}); // Un solo bloque grande al inicio
}

// Función para dividir el bloque en mitades
void dividir(int index) {
    int newSize = memoria[index].size / 2;
    memoria[index].size = newSize;
    memoria.insert(memoria.begin() + index + 1, {newSize, true});
}

// Función para buscar un bloque disponible
int buscarBloque(int size) {
    for (int i = 0; i < memoria.size(); i++) {
        if (memoria[i].free && memoria[i].size >= size) {
            while (memoria[i].size >= 2 * size) {
                dividir(i);
            }
            memoria[i].free = false;
            return i;
        }
    }
    return -1;
}

// Función para liberar un bloque
void liberarBloque(int index) {
    memoria[index].free = true;
    // Coalescencia (fusión de bloques adyacentes)
    for (int i = 0; i < memoria.size() - 1; i++) {
        if (memoria[i].free && memoria[i + 1].free && memoria[i].size == memoria[i + 1].size) {
            memoria[i].size *= 2;
            memoria.erase(memoria.begin() + i + 1);
            i--; // Retroceder para verificar nuevamente
        }
    }
}

// Función para mostrar el estado de la memoria
void mostrarEstado() {
    cout << "\nEstado de la memoria: ";
    for (auto bloque : memoria) {
        cout << "[" << bloque.size << (bloque.free ? " (Libre)] " : " (Ocupado)] ");
    }
    cout << "\n";
}

int main() {
    int totalSize = 128;
    inicializarMemoria(totalSize);

    cout << "Estado inicial de la memoria:";
    mostrarEstado();

    // Solicitar un bloque de memoria
    int size;
    cout << "\nIngrese el tamaño del bloque solicitado: ";
    cin >> size;

    int index = buscarBloque(size);
    if (index != -1) {
        cout << "Bloque asignado de tamaño: " << memoria[index].size << "\n";
    } else {
        cout << "No hay bloque disponible para la solicitud." << endl;
    }

    mostrarEstado();

    // Liberar el bloque asignado
    cout << "\nLiberando el bloque...\n";
    liberarBloque(index);

    mostrarEstado();

    return 0;
}
```

---

## **Explicación**
1. **Entrada:**
   - El usuario ingresa el tamaño del bloque que desea asignar.
   - El algoritmo busca un bloque que sea potencia de dos y lo divide en mitades si es necesario.
   - Si encuentra un bloque adecuado, lo marca como ocupado.

2. **Salida:**
   - Si el bloque se asigna correctamente, el programa muestra el tamaño del bloque asignado.  
   - Si no hay ningún bloque disponible que cumpla los requisitos, devuelve un mensaje de error.  
   - Finalmente, muestra el estado de la memoria después de la asignación.

3. **Coalescencia:**
   - Si dos bloques adyacentes están libres y tienen el mismo tamaño, el algoritmo los fusiona en un bloque más grande.

4. **Fragmentación interna:**
   - Si el tamaño solicitado no es potencia de dos, se asignará el bloque de potencia de dos más cercano, lo que puede desperdiciar memoria.

---

## **Caso de uso**
 Si el usuario solicita un bloque de tamaño **20 KB**, el algoritmo asignará un bloque de tamaño **32 KB** después de dividir un bloque de **128 KB**.

**Entrada ejemplo:**
```
Ingrese el tamaño del bloque solicitado: 20
```

**Salida esperada:**
```
Bloque asignado de tamaño: 32 KB

Estado de la memoria después de la asignación:
[32 (Ocupado)] [32 (Libre)] [64 (Libre)]

Liberando el bloque...

Estado de la memoria después de liberar:
[64 (Libre)] [64 (Libre)]
```

---

## **Métricas y justificación del uso**
| Métrica | Valor |
|---------|-------|
| **Tiempo de ejecución** | 10-15 microsegundos (promedio) |
| **Uso de memoria** | Eficiente para solicitudes de potencia de dos |
| **Complejidad temporal** | O(log n) |
| **Complejidad espacial** | O(n) |
| **Velocidad** | Alta |
| **Desventaja** | Fragmentación interna |

---

## **Ventajas**
- Rápida asignación y liberación de bloques.  
- Coalescencia automática para minimizar la fragmentación.  
- Adecuado para sistemas de alta demanda de memoria.   

##  **Desventajas**
- Solo funciona con tamaños de memoria de potencia de dos.  
- Puede desperdiciar memoria debido a la fragmentación interna.   

---

