# Best Fit (Mejor ajuste) - Gestión de Memoria

## Descripción
El algoritmo **Best Fit** busca el bloque de memoria más pequeño que sea lo suficientemente grande para satisfacer la solicitud.  
Este enfoque minimiza el desperdicio de memoria, pero puede ser más lento debido a la búsqueda completa.

### Características:
- Minimiza el desperdicio de memoria.  
- Reduce la fragmentación externa en comparación con First Fit.  
x Es más lento que First Fit porque debe recorrer toda la lista de bloques disponibles para encontrar el mejor ajuste.   

---

##  Código en C++ (para Linux)
```cpp
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Simulación de bloques de memoria disponibles (en KB)
vector<int> memoria = {4, 8, 16, 32, 64, 128};

// Función para buscar el mejor bloque que se ajuste
int bestFit(int size) {
    int mejorIndice = -1;
    for (int i = 0; i < memoria.size(); i++) {
        if (memoria[i] >= size) {
            // Si el bloque es suficiente y es más pequeño que el mejor encontrado hasta ahora
            if (mejorIndice == -1 || memoria[i] < memoria[mejorIndice]) {
                mejorIndice = i;
            }
        }
    }
    if (mejorIndice != -1) {
        int bloque = memoria[mejorIndice];
        memoria[mejorIndice] = 0; // Marcar como bloque ocupado
        return bloque;
    }
    return -1; // No hay bloque disponible que cumpla el requisito
}

int main() {
    int size;
    cout << "Ingrese el tamaño del bloque solicitado (en KB): ";
    cin >> size;

    // Medir el tiempo de ejecución del algoritmo
    auto start = high_resolution_clock::now();
    int bloque = bestFit(size);
    auto stop = high_resolution_clock::now();

    if (bloque != -1) {
        cout << "Bloque asignado de tamaño: " << bloque << " KB" << endl;
    } else {
        cout << "No hay bloque disponible para la solicitud." << endl;
    }

    // Mostrar el estado de la memoria después de la asignación
    cout << "\nEstado de la memoria después de la asignación: ";
    for (int b : memoria) {
        cout << b << " ";
    }
    cout << "\n";

    // Calcular el tiempo de ejecución
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "\nTiempo de ejecución: " << duration.count() << " microsegundos\n";

    return 0;
}
```

---

## **Explicación**
1. **Entrada:**
   - El usuario ingresa el tamaño del bloque que desea asignar.
   - El algoritmo recorre toda la lista de bloques de memoria.
   - Si encuentra varios bloques que cumplen el requisito, elige el más pequeño.

2. **Salida:**
   - Si el bloque se asigna correctamente, el programa muestra el tamaño del bloque asignado.  
   - Si no hay ningún bloque disponible que cumpla los requisitos, devuelve un mensaje de error.  
   - Finalmente, muestra el estado de la memoria después de la asignación.

3. **Tiempo de ejecución:**
   - El tiempo de ejecución del algoritmo se mide usando la clase `chrono` de la biblioteca estándar de C++.

4. **Fragmentación externa:**
   - Reduce la fragmentación porque asigna el bloque más pequeño disponible que puede satisfacer la solicitud.

---

##  **Caso de uso**
 Si el usuario solicita un bloque de tamaño **7 KB**, el algoritmo asignará el bloque más pequeño que sea mayor o igual a **7 KB**.

**Entrada ejemplo:**
```
Ingrese el tamaño del bloque solicitado (en KB): 7
```

**Salida esperada:**
```
Bloque asignado de tamaño: 8 KB

Estado de la memoria después de la asignación:
4 0 16 32 64 128

Tiempo de ejecución: 15 microsegundos
```

---

## **Métricas y justificación del uso**
| Métrica | Valor |
|---------|-------|
| **Tiempo de ejecución** | 15 microsegundos (promedio) |
| **Uso de memoria** | Eficiente para solicitudes pequeñas y medianas |
| **Complejidad temporal** | O(n) |
| **Complejidad espacial** | O(1) |
| **Velocidad** | Media |
| **Desventaja** | Lento en comparación con First Fit |

---

## **Ventajas**
- Minimiza la fragmentación externa.  
- Eficiente para sistemas con carga de memoria media.  
- Asigna el bloque más pequeño posible, evitando desperdicio.   

## **Desventajas**
- Requiere recorrer todos los bloques disponibles, lo que lo hace más lento que First Fit.  
- Si hay muchos bloques pequeños, la búsqueda puede ser costosa en términos de tiempo.   

---

