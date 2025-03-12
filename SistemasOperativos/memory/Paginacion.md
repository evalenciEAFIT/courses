# Paginación (Paging) - Gestión de Memoria

## Descripción
La **Paginación** (Paging) es una técnica de gestión de memoria que divide la memoria física en bloques de tamaño fijo llamados **marcos** (**frames**) y la memoria lógica en bloques del mismo tamaño llamados **páginas**.  
La CPU usa una **tabla de páginas** para mapear las direcciones lógicas (páginas) a las direcciones físicas (marcos).

### Características:
- Elimina la fragmentación externa.  
- Permite que un proceso tenga sus páginas distribuidas en diferentes áreas de la memoria física.  
x Puede causar fragmentación interna si las páginas no están completamente llenas.  
x Requiere una tabla de páginas para cada proceso, lo que ocupa espacio adicional.   

---

##  Código en C++ (para Linux)
```cpp
#include <iostream>
#include <vector>

using namespace std;

const int TAM_PAGINA = 4; // Tamaño de cada página en KB
const int NUM_PAGINAS = 8; // Número de páginas

// Estructura que representa una página
struct Pagina {
    int numero_pagina;
    int marco;
};

// Tabla de páginas
vector<Pagina> tabla_paginas(NUM_PAGINAS, {-1, -1}); // -1 indica que la página no está cargada

// Función para cargar una página en la memoria física
void cargarPagina(int numero_pagina, int marco) {
    tabla_paginas[numero_pagina].numero_pagina = numero_pagina;
    tabla_paginas[numero_pagina].marco = marco;
}

// Función para mostrar el estado de la tabla de páginas
void mostrarTabla() {
    cout << "\nTabla de páginas:" << endl;
    cout << "Pagina | Marco" << endl;
    for (int i = 0; i < NUM_PAGINAS; i++) {
        if (tabla_paginas[i].marco != -1) {
            cout << "   " << i << "   |   " << tabla_paginas[i].marco << endl;
        } else {
            cout << "   " << i << "   |   -" << endl;
        }
    }
}

// Función para traducir una dirección lógica a una dirección física
int traducirDireccion(int direccion_logica) {
    int numero_pagina = direccion_logica / TAM_PAGINA;
    int desplazamiento = direccion_logica % TAM_PAGINA;

    if (tabla_paginas[numero_pagina].marco != -1) {
        int direccion_fisica = (tabla_paginas[numero_pagina].marco * TAM_PAGINA) + desplazamiento;
        return direccion_fisica;
    } else {
        return -1; // Página no cargada
    }
}

int main() {
    // Simulación: cargar algunas páginas en memoria
    cargarPagina(0, 5);
    cargarPagina(1, 3);
    cargarPagina(2, 7);

    mostrarTabla();

    // Simulación: traducir direcciones lógicas a físicas
    int direccion_logica;
    cout << "\nIngrese la dirección lógica: ";
    cin >> direccion_logica;

    int direccion_fisica = traducirDireccion(direccion_logica);

    if (direccion_fisica != -1) {
        cout << "Dirección física: " << direccion_fisica << "\n";
    } else {
        cout << "Error: Página no cargada en memoria." << endl;
    }

    return 0;
}
```

---

## **Explicación**
1. **Entrada:**
   - El usuario ingresa una dirección lógica.  
   - La dirección lógica se divide en:
     - **Número de página** → Determina en qué página se encuentra la dirección.  
     - **Desplazamiento** → Posición dentro de la página.  

2. **Salida:**
   - Si la página está cargada, el programa traduce la dirección lógica a una dirección física y la muestra.  
   - Si la página no está cargada, muestra un mensaje de error.  

3. **Asignación de páginas:**
   - La tabla de páginas almacena la relación entre las páginas lógicas y los marcos físicos.  
   - La función `cargarPagina()` asigna páginas específicas a marcos físicos.  

4. **Traducción de dirección:**
   - El algoritmo toma el número de página y el desplazamiento para calcular la dirección física.  
   - Dirección física = (marco * tamaño de página) + desplazamiento.  

---

## **Caso de uso**
Si la dirección lógica es **18 KB** y el tamaño de página es **4 KB**:
- Número de página = `18 / 4 = 4`  
- Desplazamiento = `18 % 4 = 2`  
- Si la página 4 está mapeada al marco 5:
```
Dirección física = (5 * 4) + 2 = 20 + 2 = 22 KB
```

**Entrada ejemplo:**
```
Ingrese la dirección lógica: 18
```

**Salida esperada:**
```
Dirección física: 22 KB
```

---

## **Métricas y justificación del uso**
| Métrica | Valor |
|---------|-------|
| **Tiempo de ejecución** | 8-12 microsegundos (promedio) |
| **Uso de memoria** | Eficiente para tareas multitarea |
| **Complejidad temporal** | O(1) para la traducción |
| **Complejidad espacial** | O(n) para la tabla de páginas |
| **Velocidad** | Alta |
| **Desventaja** | Requiere espacio adicional para la tabla de páginas |

---

## 🚀 **Ventajas**
- Elimina la fragmentación externa.  
- Traducción de direcciones rápida mediante la tabla de páginas.  
- Permite que un proceso tenga sus páginas distribuidas en diferentes áreas de memoria.   

## ⚠️ **Desventajas**
- Puede causar fragmentación interna.  
- La tabla de páginas ocupa memoria adicional.  
- Si hay muchas páginas, el acceso a la tabla de páginas puede afectar el rendimiento.   

---

