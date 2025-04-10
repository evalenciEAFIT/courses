# Demostración de Directivas OpenMP en C++ (Linux)

Este documento presenta un programa en C++ que demuestra el uso de varias directivas de OpenMP para programación paralela en Linux. El código está organizado en una clase `OpenMPDemo` con funciones separadas para cada directiva, incluyendo medición de tiempos de ejecución.

## Código Fuente

```cpp
#include <iostream>
#include <omp.h>
#include <unistd.h>
#include <chrono> // Para medir tiempos

using namespace std;
using namespace std::chrono;

class OpenMPDemo {
private:
    int num_threads;

    // Función auxiliar para medir y mostrar el tiempo
    void printTime(const string& demoName, high_resolution_clock::time_point start) {
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        cout << "Tiempo de " << demoName << ": " << duration.count() << " ms" << endl;
    }

public:
    OpenMPDemo() {
        num_threads = omp_get_max_threads();
        cout << "Número máximo de hilos disponibles: " << num_threads << endl;
    }

    void demoParallel() {
        cout << "\n=== Demostración de parallel ===\n";
        auto start = high_resolution_clock::now();
        #pragma omp parallel num_threads(4)
        {
            int id = omp_get_thread_num();
            cout << "Hola desde el hilo " << id << endl;
        }
        printTime("parallel", start);
    }

    void demoFor() {
        cout << "\n=== Demostración de parallel for ===\n";
        auto start = high_resolution_clock::now();
        #pragma omp parallel for num_threads(4)
        for(int i = 0; i < 8; i++) {
            cout << "Iteración " << i << " procesada por hilo " 
                 << omp_get_thread_num() << endl;
            sleep(1); // Simula trabajo
        }
        printTime("parallel for", start);
    }

    void demoSections() {
        cout << "\n=== Demostración de sections ===\n";
        auto start = high_resolution_clock::now();
        #pragma omp parallel sections num_threads(2)
        {
            #pragma omp section
            {
                cout << "Sección 1 ejecutada por hilo " 
                     << omp_get_thread_num() << endl;
                sleep(1);
            }
            #pragma omp section
            {
                cout << "Sección 2 ejecutada por hilo " 
                     << omp_get_thread_num() << endl;
                sleep(1);
            }
        }
        printTime("sections", start);
    }

    void demoSingle() {
        cout << "\n=== Demostración de single ===\n";
        auto start = high_resolution_clock::now();
        #pragma omp parallel num_threads(4)
        {
            #pragma omp single
            {
                cout << "Tarea single ejecutada por hilo " 
                     << omp_get_thread_num() << endl;
                sleep(1);
            }
            cout << "Hilo " << omp_get_thread_num() 
                 << " después del single" << endl;
        }
        printTime("single", start);
    }

    void demoReduction() {
        cout << "\n=== Demostración de reduction ===\n";
        auto start = high_resolution_clock::now();
        int suma = 0;
        #pragma omp parallel for reduction(+:suma) num_threads(4)
        for(int i = 1; i <= 10; i++) {
            suma += i;
            sleep(1); // Simula trabajo
        }
        cout << "Suma de 1 a 10 = " << suma << endl;
        printTime("reduction", start);
    }

    void demoCritical() {
        cout << "\n=== Demostración de critical ===\n";
        auto start = high_resolution_clock::now();
        int contador = 0;
        #pragma omp parallel num_threads(4)
        {
            #pragma omp critical
            {
                contador++;
                cout << "Hilo " << omp_get_thread_num() 
                     << " incrementó contador a " << contador << endl;
                sleep(1);
            }
        }
        cout << "Valor final del contador: " << contador << endl;
        printTime("critical", start);
    }

    void demoBarrier() {
        cout << "\n=== Demostración de barrier ===\n";
        auto start = high_resolution_clock::now();
        #pragma omp parallel num_threads(4)
        {
            int id = omp_get_thread_num();
            cout << "Hilo " << id << " antes de la barrera" << endl;
            sleep(id % 2);
            #pragma omp barrier
            cout << "Hilo " << id << " después de la barrera" << endl;
        }
        printTime("barrier", start);
    }

    void demoTask() {
        cout << "\n=== Demostración de task ===\n";
        auto start = high_resolution_clock::now();
        #pragma omp parallel num_threads(4)
        {
            #pragma omp single
            {
                for(int i = 0; i < 6; i++) {
                    #pragma omp task
                    {
                        cout << "Tarea " << i << " ejecutada por hilo " 
                             << omp_get_thread_num() << endl;
                        sleep(1);
                    }
                }
            }
        }
        printTime("task", start);
    }
};

int main() {
    OpenMPDemo demo;
    
    demo.demoParallel();
    demo.demoFor();
    demo.demoSections();
    demo.demoSingle();
    demo.demoReduction();
    demo.demoCritical();
    demo.demoBarrier();
    demo.demoTask();

    return 0;
}
```

# Compilación y Ejecución de un Programa OpenMP en Linux

## Compilación y Ejecución

Para compilar y ejecutar el programa en Linux:

1. Guarda el código en un archivo, por ejemplo, `openmp_demo.cpp`.
2. Compila con soporte para OpenMP:
   ```bash
   g++ -fopenmp openmp_demo.cpp -o openmp_demo
   ```
3. Ejecutar programa:
   ```bash
   ./openmp_demo
   ```

 4. Puedes controlar el número de hilos con la variable de entorno OMP_NUM_THREADS:
   ```bash
   export OMP_NUM_THREADS=4  # Usa 4 hilos
   ```
