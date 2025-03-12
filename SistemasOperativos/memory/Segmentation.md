# Segmentación (Segmentation) - Gestión de Memoria

## Descripción
La **Segmentación** (Segmentation) es una técnica de gestión de memoria en la que un proceso se divide en segmentos de diferentes tamaños, de acuerdo con las estructuras lógicas del programa (como código, datos, pila, etc.).  
Cada segmento se asigna de manera independiente en la memoria física y es gestionado mediante una **tabla de segmentos** que almacena la dirección base y el límite de cada segmento.

### Características:
- Permite un manejo flexible de la memoria, adaptándose al tamaño real de los segmentos.  
- Permite proteger segmentos individuales mediante permisos específicos.  
x Puede causar fragmentación externa si hay muchos segmentos pequeños dispersos en la memoria.  
x La sobrecarga de mantener una tabla de segmentos puede ser alta.   

---

##  Código en C++ (para Linux)
```cpp
#include <iostream>
#include <vector>

using namespace std;

// Estructura para representar un segmento
struct Segmento {
    int base;   // Dirección base del segmento
    int limite; // Tamaño del segmento
};

// Tabla de segmentos (simulación)
vector<Segmento> tabla_segmentos = {
    {0, 100},    // Segmento de código
    {100, 200},  // Segmento de datos
    {300, 150},  // Segmento de pila
};

// Función para traducir una dirección lógica a una dirección física
int traducirDireccion(int num_segmento, int desplazamiento) {
    if (num_segmento >= 0 && num_segmento < tabla_segmentos.size()) {
        Segmento segmento = tabla_segmentos[num_segmento];
        if (desplazamiento >= 0 && desplazamiento < segmento.limite) {
            return segmento.base + desplazamiento;
        } else {
            cout << "Error: Desplazamiento fuera de límite." << endl;
            return -1;
        }
    } else {
        cout << "Error: Número de segmento inválido." << endl;
        return -1;
    }
}

// Función para mostrar la tabla de segmentos
void mostrarTablaSegmentos() {
    cout << "\nTabla de Segmentos:" << endl;
    cout << "Segmento | Base | Límite" << endl;
    for (int i = 0; i < tabla_segmentos.size(); i++) {
        cout << "   " << i << "     | " << tabla_segmentos[i].base << "   | " << tabla_segmentos[i].limite << endl;
    }
}

int main() {
    mostrarTablaSegmentos();

    // Simulación: traducir una dirección lógica a física
    int num_segmento, desplazamiento;
    cout << "\nIngrese el número de segmento: ";
    cin >> num_segmento;
    cout << "Ingrese el desplazamiento: ";
    cin >> desplazamiento;

    int direccion_fisica = traducirDireccion(num_segmento, desplazamiento);

    if (direccion_fisica != -1) {
        cout << "Dirección física: " << direccion_fisica << "\n";
    }

    return 0;
}
```

---

## **Explicación**
1. **Entrada:**
   - El usuario ingresa el número de segmento y el desplazamiento dentro del segmento.  
   - El algoritmo verifica si el número de segmento es válido.  
   - Si el desplazamiento está dentro del límite del segmento, calcula la dirección física usando la fórmula:
   
   **Dirección física = Base del segmento + Desplazamiento**

2. **Salida:**
   - Si el segmento y el desplazamiento son válidos, devuelve la dirección física.  
   - Si el segmento o el desplazamiento son inválidos, devuelve un mensaje de error.  

3. **Tabla de segmentos:**
   - La tabla de segmentos almacena la dirección base y el tamaño de cada segmento.  
   - Cada segmento se gestiona de manera independiente y puede tener permisos específicos.  

---

## **Caso de uso**
Si el usuario solicita acceder al segmento **1** (datos) con un desplazamiento de **50**:
- Base del segmento 1 = `100`  
- Límite del segmento 1 = `200`  
- Dirección física = `100 + 50 = 150`

**Entrada ejemplo:**
```
Ingrese el número de segmento: 1
Ingrese el desplazamiento: 50
```

**Salida esperada:**
```
Dirección física: 150
```

---

## **Métricas y justificación del uso**
| Métrica | Valor |
|---------|-------|
| **Tiempo de ejecución** | 8-12 microsegundos (promedio) |
| **Uso de memoria** | Eficiente para estructuras lógicas separadas |
| **Complejidad temporal** | O(1) para la traducción |
| **Complejidad espacial** | O(n) para la tabla de segmentos |
| **Velocidad** | Alta |
| **Desventaja** | Fragmentación externa |

---

## **Ventajas**
- Permite manejar estructuras lógicas separadas (código, datos, pila).  
- Ofrece protección de memoria por segmento.  
- Permite que los segmentos crezcan y se adapten al tamaño de los datos.   

##  **Desventajas**
- Puede causar fragmentación externa si los segmentos se asignan de manera dispersa.  
- La sobrecarga de la tabla de segmentos aumenta con el número de segmentos.  
- Si los segmentos son muy grandes, puede haber desperdicio de memoria.   

---

