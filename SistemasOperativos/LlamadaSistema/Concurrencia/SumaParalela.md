# **Explicación y Implementación de la Suma de un Vector**

## **Tabla de Contenidos**
1. [Introducción](#introducción)
2. [Suma Básica de un Vector](#suma-básica-de-un-vector)
   - [Explicación del Algoritmo](#explicación-del-algoritmo)
   - [Implementación en C++](#implementación-en-c)
3. [Suma Paralela de un Vector](#suma-paralela-de-un-vector)
   - [Explicación del Enfoque Paralelo](#explicación-del-enfoque-paralelo)
   - [Implementación en C++](#implementación-en-c-1)
4. [Comparación entre la Versión Básica y la Paralela](#comparación-entre-la-versión-básica-y-la-paralela)
5. [Conclusión](#conclusión)

---

## **Introducción**

La suma de los elementos de un vector es una operación común en programación, especialmente en aplicaciones de procesamiento de datos y cálculos matemáticos. Aunque la implementación básica es sencilla, su rendimiento puede mejorarse significativamente utilizando técnicas de paralelización, como hilos, para aprovechar múltiples núcleos del procesador.

En este documento, se presenta una implementación básica de la suma de un vector y una versión optimizada con concurrencia en C++.

---

## **Suma Básica de un Vector**

### **Explicación del Algoritmo**

La suma básica de un vector consiste en iterar sobre todos los elementos del vector y acumular sus valores en una variable. Este enfoque es secuencial y tiene una complejidad temporal de **O(n)**, donde **n** es el número de elementos en el vector.

### **Implementación en C++**

```cpp
#include <iostream>
#include <vector>

int main() {
    // Definir el vector de números
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Variable para almacenar la suma
    int totalSum = 0;

    // Iterar sobre el vector y sumar los elementos
    for (int num : data) {
        totalSum += num;
    }

    // Mostrar el resultado
    std::cout << "Suma total (versión básica): " << totalSum << std::endl;

    return 0;
}
```

**Salida esperada**:
```
Suma total (versión básica): 55
```

### **Explicación del Código**

1. **Vector de Datos**:
   - Se define un vector de enteros que contiene los números a sumar.
2. **Iteración Secuencial**:
   - Se utiliza un bucle \`for\` para recorrer cada elemento del vector y acumular su valor en la variable \`totalSum\`.
3. **Resultado**:
   - Finalmente, se imprime la suma total.

---

## **Suma Paralela de un Vector**

### **Explicación del Enfoque Paralelo**

La suma paralela divide el vector en subconjuntos más pequeños, asigna cada subconjunto a un hilo diferente y calcula las sumas parciales en paralelo. Luego, las sumas parciales se combinan para obtener el resultado final. Este enfoque aprovecha múltiples núcleos del procesador para mejorar el rendimiento.

### **Implementación en C++**

```cpp
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

std::mutex mtx; // Mutex para sincronización

// Función para calcular la suma parcial de un rango del vector
void calculatePartialSum(const std::vector<int>& data, int start, int end, int& partialSum) {
    int sum = 0;
    for (int i = start; i < end; ++i) {
        sum += data[i];
    }
    std::lock_guard<std::mutex> lock(mtx); // Bloquear el mutex
    partialSum += sum;
}

int main() {
    // Definir el vector de números
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int numThreads = 4; // Número de hilos
    int chunkSize = data.size() / numThreads; // Tamaño de cada subconjunto
    std::vector<std::thread> threads; // Vector para almacenar los hilos
    int totalSum = 0; // Variable para almacenar la suma total

    // Crear hilos para calcular las sumas parciales
    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? data.size() : (start + chunkSize);
        threads.emplace_back(calculatePartialSum, std::ref(data), start, end, std::ref(totalSum));
    }

    // Esperar a que todos los hilos terminen
    for (auto& t : threads) {
        t.join();
    }

    // Mostrar el resultado
    std::cout << "Suma total (versión paralela): " << totalSum << std::endl;

    return 0;
}
```

**Salida esperada**:
```
Suma total (versión paralela): 55
```

### **Explicación del Código**

1. **División del Trabajo**:
   - El vector se divide en partes iguales, y cada parte se asigna a un hilo diferente.
2. **Cálculo de Sumas Parciales**:
   - Cada hilo calcula la suma de su subconjunto de elementos y actualiza la variable compartida \`totalSum\`.
3. **Sincronización**:
   - Se utiliza un \`std::mutex\` para evitar condiciones de carrera al actualizar la variable compartida.
4. **Combinación de Resultados**:
   - Las sumas parciales calculadas por los hilos se combinan automáticamente en la variable \`totalSum\`.

---

## **Comparación entre la Versión Básica y la Paralela**

| **Aspecto**               | **Versión Básica**                          | **Versión Paralela**                     |
|---------------------------|---------------------------------------------|--------------------------------------------|
| **Complejidad Temporal**  | O(n)                                       | O(n)                                    |
| **Uso de Hilos**          | No usa hilos                              | Usa hilos para paralelizar el trabajo     |
| **Rendimiento en Multicore** | Limitado por un solo núcleo               | Mejora significativamente en sistemas multicore |
| **Sobrecarga**            | Sin sobrecarga                            | Sobrecarga de creación y gestión de hilos |

---

## **Conclusión**

La suma de los elementos de un vector es una operación simple pero fundamental en muchos algoritmos. La implementación básica es adecuada para vectores pequeños o cuando no es necesario optimizar el rendimiento. Sin embargo, para grandes conjuntos de datos o en sistemas multicore, la versión paralela ofrece una mejora significativa en el rendimiento.

Este documento proporciona una base sólida para entender y aplicar tanto la suma básica como la suma paralela de un vector en C++. Con estos ejemplos, puedes explorar y adaptar estas técnicas a tus propios proyectos.

---
