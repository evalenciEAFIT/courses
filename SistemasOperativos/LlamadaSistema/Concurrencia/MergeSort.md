# **Explicación Detallada del Merge Sort**

## **Tabla de Contenidos**
1. [¿Qué es Merge Sort?](#qué-es-merge-sort)
2. [Funcionamiento del Merge Sort](#funcionamiento-del-merge-sort)
   - [Divide y Vencerás](#divide-y-vencerás)
   - [Fases del Merge Sort](#fases-del-merge-sort)
3. [Implementación Básica del Merge Sort](#implementación-básica-del-merge-sort)
4. [Implementación Eficiente con Concurrencia](#implementación-eficiente-con-concurrencia)
5. [Comparación entre la Versión Básica y la Concurrente](#comparación-entre-la-versión-básica-y-la-concurrente)
6. [Conclusión](#conclusión)

---

## **¿Qué es Merge Sort?**

**Merge Sort** es un algoritmo de ordenación basado en el paradigma **divide y vencerás**. Divide repetidamente un arreglo en mitades hasta que cada subarreglo contenga un solo elemento, y luego combina (merge) los subarreglos ordenados para producir un arreglo completamente ordenado.

Este algoritmo tiene una complejidad temporal de **O(n log n)** en todos los casos (mejor, promedio y peor), lo que lo hace muy eficiente para grandes conjuntos de datos.

---

## **Funcionamiento del Merge Sort**

### **Divide y Vencerás**

El Merge Sort sigue el enfoque **divide y vencerás**, que consiste en:
1. **Dividir**: Dividir el problema en subproblemas más pequeños.
2. **Conquistar**: Resolver los subproblemas recursivamente.
3. **Combinar**: Combinar las soluciones de los subproblemas para obtener la solución final.

### **Fases del Merge Sort**

1. **División**:
   - El arreglo se divide en dos mitades hasta que cada subarreglo tenga un solo elemento.
2. **Ordenación**:
   - Los subarreglos se combinan de manera ordenada.
3. **Combinación**:
   - Los subarreglos ordenados se fusionan en un solo arreglo ordenado.

---

## **Implementación Básica del Merge Sort**

La implementación básica del Merge Sort utiliza recursión para dividir y combinar los subarreglos. A continuación, se muestra un ejemplo en C++:

```cpp
#include <iostream>
#include <vector>

// Función para fusionar dos subarreglos ordenados
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Crear arreglos temporales
    std::vector<int> L(n1), R(n2);

    // Copiar datos a los arreglos temporales
    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    // Fusionar los arreglos temporales de vuelta al arreglo original
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    // Copiar los elementos restantes de L[], si los hay
    while (i < n1) arr[k++] = L[i++];

    // Copiar los elementos restantes de R[], si los hay
    while (j < n2) arr[k++] = R[j++];
}

// Función principal del Merge Sort
void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Ordenar las dos mitades
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Fusionar las mitades ordenadas
        merge(arr, left, mid, right);
    }
}

int main() {
    std::vector<int> arr = {12, 11, 13, 5, 6, 7};
    std::cout << "Arreglo original: ";
    for (int num : arr) std::cout << num << " ";
    std::cout << std::endl;

    mergeSort(arr, 0, arr.size() - 1);

    std::cout << "Arreglo ordenado: ";
    for (int num : arr) std::cout << num << " ";
    std::cout << std::endl;

    return 0;
}
```

**Salida esperada**:
```
Arreglo original: 12 11 13 5 6 7 
Arreglo ordenado: 5 6 7 11 12 13
```

### **Explicación del Código**

1. **División**:
   - La función \`mergeSort\` divide el arreglo en dos mitades recursivamente hasta que cada subarreglo tenga un solo elemento.
2. **Fusión**:
   - La función \`merge\` combina dos subarreglos ordenados en un solo arreglo ordenado.

---

## **Implementación Eficiente con Concurrencia**

Para aprovechar múltiples núcleos del procesador, podemos paralelizar el Merge Sort utilizando hilos. En esta versión, cada mitad del arreglo se ordena en un hilo separado.

```cpp
#include <iostream>
#include <vector>
#include <thread>

// Función para fusionar dos subarreglos ordenados
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Crear arreglos temporales
    std::vector<int> L(n1), R(n2);

    // Copiar datos a los arreglos temporales
    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    // Fusionar los arreglos temporales de vuelta al arreglo original
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    // Copiar los elementos restantes de L[], si los hay
    while (i < n1) arr[k++] = L[i++];

    // Copiar los elementos restantes de R[], si los hay
    while (j < n2) arr[k++] = R[j++];
}

// Función principal del Merge Sort con concurrencia
void parallelMergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Crear hilos para ordenar las dos mitades
        std::thread t1(parallelMergeSort, std::ref(arr), left, mid);
        std::thread t2(parallelMergeSort, std::ref(arr), mid + 1, right);

        // Esperar a que los hilos terminen
        t1.join();
        t2.join();

        // Fusionar las mitades ordenadas
        merge(arr, left, mid, right);
    }
}

int main() {
    std::vector<int> arr = {12, 11, 13, 5, 6, 7};
    std::cout << "Arreglo original: ";
    for (int num : arr) std::cout << num << " ";
    std::cout << std::endl;

    parallelMergeSort(arr, 0, arr.size() - 1);

    std::cout << "Arreglo ordenado: ";
    for (int num : arr) std::cout << num << " ";
    std::cout << std::endl;

    return 0;
}
```

**Salida esperada**:
```
Arreglo original: 12 11 13 5 6 7 
Arreglo ordenado: 5 6 7 11 12 13
```

### **Explicación del Código**

1. **Paralelización**:
   - La función \`parallelMergeSort\` crea dos hilos para ordenar las dos mitades del arreglo simultáneamente.
   - Los hilos se sincronizan usando \`join()\` antes de fusionar los resultados.
2. **Rendimiento**:
   - Esta versión es más eficiente en sistemas multicore, ya que aprovecha múltiples núcleos para realizar el trabajo en paralelo.

---

## **Comparación entre la Versión Básica y la Concurrente**

| **Aspecto**               | **Versión Básica**                          | **Versión Concurrente**                     |
|---------------------------|---------------------------------------------|--------------------------------------------|
| **Complejidad Temporal**  | O(n log n)                                 | O(n log n)                                |
| **Uso de Hilos**          | No usa hilos                              | Usa hilos para paralelizar el trabajo     |
| **Rendimiento en Multicore** | Limitado por un solo núcleo               | Mejora significativamente en sistemas multicore |
| **Sobrecarga**            | Sin sobrecarga                            | Sobrecarga de creación y gestión de hilos |

---

## **Conclusión**

El **Merge Sort** es un algoritmo de ordenación eficiente y estable, ideal para grandes conjuntos de datos. Su naturaleza recursiva y basada en **divide y vencerás** lo hace perfecto para paralelización, lo que permite aprovechar al máximo los recursos de hardware modernos.

La implementación concurrente del Merge Sort, utilizando hilos, mejora significativamente el rendimiento en sistemas multicore. Sin embargo, es importante tener en cuenta la sobrecarga asociada con la creación y gestión de hilos, especialmente para arreglos pequeños.

Este documento proporciona una base sólida para entender y aplicar el Merge Sort, tanto en su forma básica como en su versión optimizada con concurrencia.

---
