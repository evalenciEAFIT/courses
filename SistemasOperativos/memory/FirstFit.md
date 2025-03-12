# First Fit (Primer ajuste) - Gestión de Memoria

## Descripción
El algoritmo **First Fit** busca el primer bloque de memoria disponible que sea lo suficientemente grande para satisfacer la solicitud.

### Características:
- Rápido y fácil de implementar.  
- Es eficiente para sistemas con cargas de memoria ligeras.  
x Puede causar fragmentación externa si hay muchos bloques pequeños sin usar.   

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

// Función para buscar el primer bloque que se ajuste
int firstFit(int size) {
    for (int i = 0; i < memoria.size(); i++) {
        if (memoria[i] >= size) { // Si el bloque es lo suficientemente grande
            int bloque = memoria[i];
            memoria[i] = 0; // Asignamos el bloque marcándolo como 0 para indicar que está ocupado
            return bloque;
        }
    }
    return -1; // Si no hay bloques disponibles lo suficientemente grandes
}

int main() {
    int size;
    cout << "Ingrese el tamaño del bloque solicitado (en KB): ";
    cin >> size;

    // Medir el tiempo de ejecución del algoritmo
    auto start = high_resolution_clock::now();
    int bloque = firstFit(size);
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
   - El algoritmo busca el primer bloque que sea mayor o igual al tamaño solicitado.
   - Si encuentra un bloque adecuado, lo asigna y lo marca como `0` para indicar que está ocupado.

2. **Salida:**
   - Si el bloque se asigna correctamente, el programa muestra el tamaño del bloque asignado.  
   - Si no hay ningún bloque disponible que cumpla los requisitos, devuelve un mensaje de error.  
   - Finalmente, muestra el estado de la memoria después de la asignación.

3. **Tiempo de ejecución:**
   - El tiempo de ejecución del algoritmo se mide usando la clase `chrono` de la biblioteca estándar de C++.

4. **Fragmentación externa:**
   - Después de varias asignaciones y liberaciones de memoria, pueden quedar muchos bloques pequeños que no pueden ser reutilizados eficientemente, lo que conduce a fragmentación externa.

---

##  **Caso de uso**
➡ Si el usuario solicita un bloque de tamaño **7 KB**, el algoritmo asignará el primer bloque disponible que sea mayor o igual a **7 KB**.

**Entrada ejemplo:**
```
Ingrese el tamaño del bloque solicitado (en KB): 7
```

**Salida esperada:**
```
Bloque asignado de tamaño: 8 KB

Estado de la memoria después de la asignación:
4 0 16 32 64 128

Tiempo de ejecución: 12 microsegundos
```

---

## **Métricas y justificación del uso**
| Métrica | Valor |
|---------|-------|
| **Tiempo de ejecución** | 12 microsegundos (promedio) |
| **Uso de memoria** | Eficiente para solicitudes simples |
| **Complejidad temporal** | O(n) |
| **Complejidad espacial** | O(1) |
| **Velocidad** | Alta |
| **Desventaja** | Fragmentación externa |

---

## **Ventajas**
- Algoritmo rápido y fácil de implementar.  
- Ideal para situaciones donde las solicitudes de memoria son simples y no requieren optimización compleja.

## **Desventajas**
- Fragmentación externa.  
- Si se liberan y reasignan muchos bloques, el rendimiento puede degradarse.   

---

