# **Introducción al Patrón MapReduce**

## **Tabla de Contenidos**
1. [¿Qué es MapReduce?](#qué-es-mapreduce)
2. [Funcionamiento del MapReduce](#funcionamiento-del-mapreduce)
   - [Fase Map (Mapeo)](#fase-map-mapeo)
   - [Fase Shuffle (Reorganización)](#fase-shuffle-reorganización)
   - [Fase Reduce (Reducción)](#fase-reduce-reducción)
3. [Implementación Básica en C++](#implementación-básica-en-c)
4. [Implementación Eficiente con Concurrencia](#implementación-eficiente-con-concurrencia)
5. [Comparación entre Implementaciones](#comparación-entre-implementaciones)
6. [Conclusión](#conclusión)

---

## **¿Qué es MapReduce?**

**MapReduce** es un modelo de programación diseñado para procesar grandes volúmenes de datos de manera eficiente y distribuida. Fue desarrollado por Google en 2004 y se ha convertido en una herramienta fundamental en el procesamiento de big data.

El nombre "MapReduce" proviene de las dos fases principales del proceso:
- **Map (Mapeo)**: Transforma los datos de entrada en pares clave-valor.
- **Reduce (Reducción)**: Combina los resultados intermedios para producir la salida final.

Este modelo es especialmente útil cuando se trabaja con grandes conjuntos de datos que no caben en la memoria de una sola máquina, ya que permite distribuir el trabajo entre múltiples nodos en un clúster.

---

## **Funcionamiento del MapReduce**

### **Fase Map (Mapeo)**

En esta fase, los datos de entrada se dividen en fragmentos más pequeños, y cada fragmento se procesa independientemente mediante una función **map**. La función map toma cada elemento del conjunto de datos y lo transforma en un par clave-valor.

Por ejemplo:
- Entrada: Una lista de palabras: \`["hola", "mundo", "hola"]\`.
- Salida intermedia: Pares clave-valor: \`[("hola", 1), ("mundo", 1), ("hola", 1)]\`.

### **Fase Shuffle (Reorganización)**

Después de la fase Map, los pares clave-valor intermedios se agrupan por clave. Esta fase se llama **shuffle** porque reorganiza los datos para que todos los valores asociados a una misma clave se agrupen juntos.

Por ejemplo:
- Entrada intermedia: \`[("hola", 1), ("mundo", 1), ("hola", 1)]\`.
- Salida después del shuffle: \`[("hola", [1, 1]), ("mundo", [1])]\`.

### **Fase Reduce (Reducción)**

En esta fase, una función **reduce** combina los valores asociados a cada clave para producir el resultado final. Por ejemplo, puede sumar los valores asociados a una clave.

Por ejemplo:
- Entrada después del shuffle: \`[("hola", [1, 1]), ("mundo", [1])]\`.
- Salida final: \`[("hola", 2), ("mundo", 1)]\`.

---

## **Implementación Básica en C++**

La siguiente implementación muestra cómo funciona el patrón MapReduce sin concurrencia. Es una versión sencilla y secuencial que procesa los datos paso a paso.

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <string>

// Función Map: Convierte palabras en pares (palabra, 1)
std::vector<std::pair<std::string, int>> mapFunction(const std::vector<std::string>& words) {
    std::vector<std::pair<std::string, int>> mappedData;
    for (const auto& word : words) {
        mappedData.emplace_back(word, 1);
    }
    return mappedData;
}

// Función Reduce: Combina los valores asociados a cada clave
std::map<std::string, int> reduceFunction(const std::vector<std::pair<std::string, int>>& mappedData) {
    std::map<std::string, int> reducedData;
    for (const auto& pair : mappedData) {
        reducedData[pair.first] += pair.second;
    }
    return reducedData;
}

int main() {
    // Datos de entrada
    std::vector<std::string> words = {"hola", "mundo", "hola", "mapreduce", "mundo", "hola"};

    // Fase Map
    auto mappedData = mapFunction(words);

    // Fase Reduce
    auto reducedData = reduceFunction(mappedData);

    // Mostrar resultados
    std::cout << "Resultados del conteo de palabras:" << std::endl;
    for (const auto& pair : reducedData) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}
```

**Salida esperada**:
```
Resultados del conteo de palabras:
hola: 3
mapreduce: 1
mundo: 2
```

### **Explicación del Código**

1. **Map Function**:
   - Toma una lista de palabras y genera pares \`(palabra, 1)\` para cada palabra.

2. **Shuffle**:
   - En este caso, el shuffle es implícito porque usamos un \`std::map\` para agrupar los valores por clave.

3. **Reduce Function**:
   - Combina los valores asociados a cada clave sumando los valores.

---

## **Implementación Eficiente con Concurrencia**

Para mejorar el rendimiento, podemos usar hilos para ejecutar las fases Map y Reduce en paralelo. Esto es especialmente útil cuando se trabaja con grandes volúmenes de datos.

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <thread>
#include <mutex>

std::mutex mtx;

// Función Map: Convierte palabras en pares (palabra, 1)
void mapFunction(const std::vector<std::string>& words, std::vector<std::pair<std::string, int>>& mappedData) {
    for (const auto& word : words) {
        std::lock_guard<std::mutex> lock(mtx);
        mappedData.emplace_back(word, 1);
    }
}

// Función Reduce: Combina los valores asociados a cada clave
void reduceFunction(std::map<std::string, int>& reducedData, const std::vector<std::pair<std::string, int>>& mappedData) {
    for (const auto& pair : mappedData) {
        std::lock_guard<std::mutex> lock(mtx);
        reducedData[pair.first] += pair.second;
    }
}

int main() {
    // Datos de entrada
    std::vector<std::string> words = {"hola", "mundo", "hola", "mapreduce", "mundo", "hola"};

    // Dividir los datos en partes para procesar en paralelo
    int numThreads = 2;
    int chunkSize = words.size() / numThreads;

    std::vector<std::thread> threads;
    std::vector<std::pair<std::string, int>> mappedData;
    std::map<std::string, int> reducedData;

    // Fase Map en paralelo
    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? words.size() : (start + chunkSize);
        threads.emplace_back(mapFunction, std::ref(std::vector<std::string>(words.begin() + start, words.begin() + end)), std::ref(mappedData));
    }

    for (auto& t : threads) {
        t.join();
    }

    // Fase Reduce en paralelo
    threads.clear();
    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? mappedData.size() : (start + chunkSize);
        threads.emplace_back(reduceFunction, std::ref(reducedData), std::ref(std::vector<std::pair<std::string, int>>(mappedData.begin() + start, mappedData.begin() + end)));
    }

    for (auto& t : threads) {
        t.join();
    }

    // Mostrar resultados
    std::cout << "Resultados del conteo de palabras:" << std::endl;
    for (const auto& pair : reducedData) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}
```

### **Explicación del Código**

1. **Map Function**:
   - Divide los datos en partes iguales y asigna cada parte a un hilo.
   - Usa un mutex para evitar condiciones de carrera al agregar datos al vector compartido.

2. **Shuffle**:
   - El shuffle sigue siendo implícito porque usamos un \`std::map\` para agrupar los valores por clave.

3. **Reduce Function**:
   - Divide los datos mapeados en partes iguales y asigna cada parte a un hilo.
   - Usa un mutex para evitar condiciones de carrera al actualizar el mapa compartido.

---

## **Comparación entre Implementaciones**

| Característica               | Implementación Básica                     | Implementación Eficiente con Concurrencia          |
|------------------------------|--------------------------------------------|---------------------------------------------------|
| **Rendimiento**              | Lento para grandes volúmenes de datos      | Más rápido gracias al paralelismo                  |
| **Complejidad**              | Simple y fácil de entender                 | Más compleja debido a la gestión de hilos y mutex |
| **Escalabilidad**            | No escalable                               | Escalable para grandes volúmenes de datos         |
| **Uso de Recursos**          | Bajo consumo de recursos                   | Mayor consumo debido a la creación de hilos       |

---

## **Conclusión**

El patrón **MapReduce** es una herramienta poderosa para procesar grandes volúmenes de datos de manera eficiente y escalable. La implementación básica es adecuada para problemas pequeños o de aprendizaje, mientras que la versión con concurrencia es ideal para escenarios donde el rendimiento y la escalabilidad son críticos.

Al comprender las diferencias entre estas implementaciones, puedes elegir la mejor opción según tus necesidades y aprovechar al máximo los recursos disponibles.

---
