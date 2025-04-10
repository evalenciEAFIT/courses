# Código C++ con Directivas OpenMP: Procesamiento Paralelo de Tareas Mixtas

## Descripción del Problema
Queremos procesar un conjunto de tareas que incluyen: 
1. Calcular la suma de un arreglo grande (reducción).
2. Ejecutar secciones independientes como inicialización y escritura de resultados.
3. Sincronizar hilos en puntos específicos.
4. Gestionar tareas dinámicas con dependencias (e.g., generar datos y procesarlos).
5. Proteger una sección crítica para actualizar un contador global.

El enfoque secuencial sería lento, ya que cada tarea se ejecutaría una tras otra. Además, sin sincronización adecuada, los resultados podrían ser inconsistentes en un entorno paralelo.

## Solución Propuesta
Utilizamos OpenMP para paralelizar las tareas:
- `#pragma omp parallel` para crear un equipo de hilos.
- `#pragma omp parallel for` con `reduction` para la suma del arreglo.
- `#pragma omp sections` para tareas independientes.
- `#pragma omp single` para inicialización única.
- `#pragma omp critical` para accesos seguros.
- `#pragma omp barrier` para sincronización.
- `#pragma omp task` con `depend` para tareas dinámicas.

El objetivo es minimizar el tiempo de ejecución y garantizar resultados correctos mediante la sincronización adecuada.

---

## Código en C++

```cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <omp.h>
using namespace std;

// Función para simular trabajo
void simulate_work(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}

int main() {
    const int SIZE = 1000; // Tamaño del arreglo
    vector<int> data(SIZE, 1); // Arreglo inicializado con 1s
    int sum = 0; // Variable para la suma
    int critical_counter = 0; // Contador protegido
    vector<int> task_results(4, 0); // Resultados de tareas dinámicas

    // Medir tiempo de ejecución
    auto start = chrono::high_resolution_clock::now();

    // Crear un equipo de hilos para todo el procesamiento
    #pragma omp parallel num_threads(4)
    {
        int thread_id = omp_get_thread_num(); // Identificador del hilo

        // 1. Inicialización única con #pragma omp single
        #pragma omp single
        {
            cout << "Hilo " << thread_id << " inicializando datos...\n";
            simulate_work(500); // Simula inicialización
        }

        // 2. Paralelizar un bucle con #pragma omp parallel for y reduction
        #pragma omp for reduction(+:sum)
        for (int i = 0; i < SIZE; i++) {
            simulate_work(1); // Trabajo por elemento
            sum += data[i]; // Suma paralela segura
        }

        // 3. Ejecutar secciones independientes con #pragma omp sections
        #pragma omp sections
        {
            #pragma omp section
            {
                cout << "Hilo " << thread_id << " en sección 1: procesando datos\n";
                simulate_work(1000); // Simula procesamiento
            }
            #pragma omp section
            {
                cout << "Hilo " << thread_id << " en sección 2: escribiendo resultados\n";
                simulate_work(1000); // Simula escritura
            }
        }

        // 4. Sincronizar hilos con #pragma omp barrier
        #pragma omp barrier
        cout << "Hilo " << thread_id << " pasó la barrera\n";

        // 5. Proteger una sección crítica con #pragma omp critical
        #pragma omp critical (counter_update)
        {
            simulate_work(200); // Simula actualización
            critical_counter++;
            cout << "Hilo " << thread_id << " incrementó contador a " << critical_counter << "\n";
        }

        // 6. Generar tareas dinámicas con #pragma omp task y dependencias
        #pragma omp single
        {
            for (int i = 0; i < 4; i++) {
                #pragma omp task depend(out:task_results[i])
                {
                    simulate_work(500); // Simula generación de datos
                    task_results[i] = i * 10;
                    cout << "Tarea " << i << " generada por hilo " << thread_id << ": " << task_results[i] << "\n";
                }
            }
        }

        // 7. Procesar resultados de tareas con dependencia
        #pragma omp task depend(in:task_results[0], task_results[1], task_results[2], task_results[3])
        {
            int task_sum = 0;
            for (int i = 0; i < 4; i++) {
                task_sum += task_results[i];
            }
            cout << "Suma de tareas calculada por hilo " << thread_id << ": " << task_sum << "\n";
        }
    } // Fin del bloque paralelo

    // Calcular y mostrar tiempo de ejecución
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "\nResultados finales:\n";
    cout << "Suma del arreglo: " << sum << "\n";
    cout << "Contador crítico: " << critical_counter << "\n";
    cout << "Tiempo total: " << duration.count() << " ms\n";

    return 0;
}
```

## **Explicación del Diseño e Implementación**

### **Directivas Utilizadas**

1. **\#pragma omp parallel**: Define el bloque paralelo principal con 4 hilos. Todo el procesamiento ocurre dentro de este equipo.  
2. **\#pragma omp single**: Garantiza que solo un hilo realice la inicialización y la generación de tareas dinámicas, evitando duplicación.  
3. **\#pragma omp for con reduction**: Paraleliza el bucle que suma el arreglo data. La cláusula reduction(+:sum) asegura que cada hilo mantenga una copia privada de sum, combinándolas al final.  
4. **\#pragma omp sections y \#pragma omp section**: Divide el trabajo en dos secciones independientes (procesamiento y escritura) que se ejecutan en paralelo.  
5. **\#pragma omp barrier**: Sincroniza todos los hilos tras las secciones, asegurando que todos terminen antes de continuar.  
6. **\#pragma omp critical**: Protege el acceso al contador critical\_counter, permitiendo incrementos seguros uno a la vez.  
7. **\#pragma omp task con depend**: Crea tareas dinámicas para generar datos (task\_results) y una tarea dependiente que suma esos resultados, respetando las dependencias.

### **Diseño**

* **Estructura**: El código está encapsulado en un solo bloque parallel para maximizar la reutilización de hilos y minimizar el *overhead* de creación.  
* **Simulación**: simulate\_work() usa sleep para emular trabajo computacional, permitiendo medir el impacto de la paralelización.  
* **Sincronización**: Se combinan barrier, critical, y depend para coordinar las tareas y evitar condiciones de carrera.  
* **Medición**: \<chrono\> mide el tiempo total, mostrando la eficiencia del enfoque paralelo.

### **Implementación**

* **Arreglo data**: Representa un conjunto grande de datos para la reducción.  
* **Contador crítico**: Simula una variable compartida que requiere protección.  
* **Tareas dinámicas**: Usa un arreglo task\_results para demostrar dependencias entre generación y procesamiento.  
* **Salidas**: Cada hilo imprime mensajes para rastrear su progreso, facilitando la depuración y verificación.

---

## **Compilación y Ejecución**

* Compilar: g++ \-fopenmp codigo.cpp \-o codigo  
* Ejecutar: ./codigo

## **Resultados Esperados**

* **Suma del arreglo**: 1000 (SIZE × 1).  
* **Contador crítico**: 4 (un incremento por hilo).  
* **Tiempo**: Depende del hardware, pero será menor que una versión secuencial equivalente (\~5000 ms secuencial vs. \~2000 ms paralelo con 4 hilos).



### Notas Finales
- **Directivas Incluidas**: Se usan `parallel`, `for`, `sections`, `section`, `single`, `reduction`, `critical`, `barrier`, y `task` con `depend`. Otras como `schedule` o `nowait` podrían añadirse, pero se omitieron para mantener la simplicidad.
- **Flexibilidad**: Puedes ajustar `SIZE`, el número de hilos (`num_threads`), o los tiempos en `simulate_work()` para experimentar con diferentes cargas de trabajo.
