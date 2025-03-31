# **Estrategias para Mejorar la Eficiencia de Iteraciones usando Hilos y Multiproceso en C++**

## **Tabla de Contenidos**
1. [Introducción](#introducción)
2. [Estrategia para Mejorar la Eficiencia](#estrategia-para-mejorar-la-eficiencia)
   - [División de Tareas](#división-de-tareas)
   - [Uso de Hilos](#uso-de-hilos)
   - [Uso de Multiprocesamiento](#uso-de-multiprocesamiento)
3. [Algoritmos Clásicos de Paralelismo](#algoritmos-clásicos-de-paralelismo)
   - [MapReduce](#mapreduce)
   - [Merge Sort Paralelo](#merge-sort-paralelo)
   - [Suma Paralela](#suma-paralela)
4. [Ejemplos de Código](#ejemplos-de-código)
5. [Conclusión](#conclusión)

---

## **Introducción**

En aplicaciones donde se realizan iteraciones extensas (por ejemplo, cálculos matemáticos, procesamiento de datos o simulaciones), el uso de hilos y multiprocesamiento puede mejorar significativamente los tiempos de ejecución. Dividir el trabajo entre múltiples núcleos del procesador permite aprovechar al máximo el hardware disponible.

Este documento presenta una estrategia para optimizar iteraciones mediante hilos y multiprocesamiento, junto con ejemplos prácticos de algoritmos clásicos de paralelismo.

---

## **Estrategia para Mejorar la Eficiencia**

### **División de Tareas**

La clave para mejorar la eficiencia es dividir el problema en subproblemas más pequeños que puedan ejecutarse en paralelo. Por ejemplo:
- Si tienes un bucle que itera sobre un rango de números, puedes dividir ese rango en partes iguales y asignar cada parte a un hilo o proceso diferente.
- Asegúrate de que las tareas sean independientes para evitar condiciones de carrera.

### **Uso de Hilos**

Los hilos son unidades ligeras de ejecución dentro de un proceso. En C++, puedes usar la biblioteca \`<thread>\` para crear hilos y distribuir el trabajo.

### **Uso de Multiprocesamiento**

El multiprocesamiento implica la creación de procesos independientes que pueden ejecutarse en paralelo. Es útil cuando necesitas mayor aislamiento entre las tareas o cuando trabajas con grandes volúmenes de datos. En Linux, puedes usar \`fork()\` o memoria compartida para implementarlo.

---

## **Algoritmos Clásicos de Paralelismo**

### **MapReduce**

El patrón MapReduce divide un problema en dos fases:
1. **Map**: Aplica una función a cada elemento de un conjunto de datos.
2. **Reduce**: Combina los resultados parciales para producir el resultado final.

**Ejemplo de Uso**: Procesamiento de grandes conjuntos de datos, como conteo de palabras.

### **Merge Sort Paralelo**

El Merge Sort es un algoritmo de ordenación que se presta bien al paralelismo. La idea es dividir la lista en mitades, ordenarlas en paralelo y luego combinar los resultados.

### **Suma Paralela**

La suma paralela divide un conjunto de números en subconjuntos, calcula sumas parciales en paralelo y luego combina los resultados.

---

## **Ejemplos de Código**

### **MapReduce en C++**

```cpp
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

std::mutex mtx;

void mapFunction(const std::vector<int>& data, int start, int end, std::vector<int>& results) {
    int partialSum = 0;
    for (int i = start; i < end; ++i) {
        partialSum += data[i];
    }
    std::lock_guard<std::mutex> lock(mtx);
    results.push_back(partialSum);
}

int reduceFunction(const std::vector<int>& results) {
    int total = 0;
    for (int value : results) {
        total += value;
    }
    return total;
}

int main() {
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> results;
    std::vector<std::thread> threads;

    int numThreads = 4;
    int chunkSize = data.size() / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? data.size() : (start + chunkSize);
        threads.emplace_back(mapFunction, std::ref(data), start, end, std::ref(results));
    }

    for (auto& t : threads) {
        t.join();
    }

    int totalSum = reduceFunction(results);
    std::cout << "Suma total: " << totalSum << std::endl;

    return 0;
}
```

### **Merge Sort Paralelo**

```cpp
#include <iostream>
#include <vector>
#include <thread>

void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void parallelMergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        std::thread t1(parallelMergeSort, std::ref(arr), left, mid);
        std::thread t2(parallelMergeSort, std::ref(arr), mid + 1, right);

        t1.join();
        t2.join();

        merge(arr, left, mid, right);
    }
}

int main() {
    std::vector<int> arr = {12, 11, 13, 5, 6, 7};
    parallelMergeSort(arr, 0, arr.size() - 1);

    std::cout << "Arreglo ordenado: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

### **Suma Paralela**

```cpp
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

std::mutex mtx;

void calculatePartialSum(const std::vector<int>& data, int start, int end, int& partialSum) {
    int sum = 0;
    for (int i = start; i < end; ++i) {
        sum += data[i];
    }
    std::lock_guard<std::mutex> lock(mtx);
    partialSum += sum;
}

int main() {
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int numThreads = 4;
    int chunkSize = data.size() / numThreads;

    std::vector<std::thread> threads;
    int totalSum = 0;

    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? data.size() : (start + chunkSize);
        threads.emplace_back(calculatePartialSum, std::ref(data), start, end, std::ref(totalSum));
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Suma total: " << totalSum << std::endl;

    return 0;
}
```

---

## **Conclusión**

El uso de hilos y multiprocesamiento en C++ permite mejorar significativamente la eficiencia de iteraciones intensivas. Al dividir el trabajo en tareas más pequeñas y ejecutarlas en paralelo, puedes aprovechar al máximo los recursos de hardware disponibles. Los algoritmos clásicos de paralelismo, como MapReduce, Merge Sort Paralelo y Suma Paralela, son excelentes ejemplos de cómo aplicar estas técnicas en problemas reales.

---
