# Worst Fit (Peor ajuste) - Gestión de Memoria

## Descripción
El algoritmo **Worst Fit** selecciona el bloque de memoria más grande disponible que pueda satisfacer la solicitud.  
Este enfoque deja los bloques más pequeños disponibles para futuras asignaciones, pero puede aumentar la fragmentación externa.

### Características:
- Puede dejar espacio para bloques pequeños.  
- Útil para situaciones donde las solicitudes son grandes o variables.  
x Puede aumentar la fragmentación externa.  
x Puede desaprovechar bloques grandes.   

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

// Función para buscar el bloque más grande que se ajuste
int worstFit(int size) {
    int peorIndice = -1;
    for (int i = 0; i < memoria.size(); i++) {
        if (memoria[i] >= size) {
            // Si el bloque es suficiente y es más grande que el peor encontrado hasta ahora
            if (peorIndice == -1 || memoria[i] > memoria[peorIndice]) {
                peorIndice = i;
            }
        }
    }
    if (peorIndice != -1) {
        int bloque = memoria[peorIndice];
        memoria[peorIndice] = 0; // Marcar como bloque ocupado
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
    int bloque = worstFit(size);
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
   - Si encuentra varios bloques que cumplen el requisito, elige el más grande.

2. **Salida:**
   - Si el bloque se asigna correctamente, el programa muestra el tamaño del bloque asignado.  
   - Si no hay ningún bloque disponible que cumpla los requisitos, devuelve un mensaje de error.  
   - Finalmente, muestra el estado de la memoria después de la asignación.

3. **Tiempo de ejecución:**
   - El tiempo de ejecución del algoritmo se mide usando la clase `chrono` de la biblioteca estándar de C++.

4. **Fragmentación externa:**
   - Puede causar fragmentación externa porque los bloques más pequeños se desperdician o quedan inutilizados.

---

## **Caso de uso**
 Si el usuario solicita un bloque de tamaño **7 KB**, el algoritmo asignará el bloque más grande que sea mayor o igual a **7 KB**.

**Entrada ejemplo:**
```
Ingrese el tamaño del bloque solicitado (en KB): 7
```

**Salida esperada:**
```
Bloque asignado de tamaño: 128 KB

Estado de la memoria después de la asignación:
4 8 16 32 64 0

Tiempo de ejecución: 14 microsegundos
```

---

## **Métricas y justificación del uso**
| Métrica | Valor |
|---------|-------|
| **Tiempo de ejecución** | 14 microsegundos (promedio) |
| **Uso de memoria** | No eficiente (puede dejar huecos) |
| **Complejidad temporal** | O(n) |
| **Complejidad espacial** | O(1) |
| **Velocidad** | Media |
| **Desventaja** | Alta fragmentación externa |

---

## **Ventajas**
- Deja espacio para bloques pequeños futuros.  
- Útil para solicitudes grandes.  
- Evita el desperdicio de bloques pequeños.   

##  **Desventajas**
- La fragmentación externa puede aumentar rápidamente.  
- Las solicitudes pequeñas pueden desperdiciar memoria.   

---

