## Guía de OpenMP: Directivas, Ejemplos y Análisis de Rendimiento

OpenMP (Open Multi-Processing) es una API que facilita la programación paralela en sistemas de memoria compartida. Utiliza directivas de preprocesador (`#pragma`) para indicar al compilador cómo paralelizar el código. A continuación, se explican las directivas principales con ejemplos claros en C++ y se comparan soluciones secuenciales frente a paralelas.

---

## Requisitos para los Ejemplos
- Compilador compatible con OpenMP (como `g++`).
- Habilitar OpenMP al compilar: `g++ -fopenmp archivo.cpp -o archivo`.
- Biblioteca `<chrono>` para medir tiempos y `<thread>` para simular trabajo con `sleep`.

Incluye estas cabeceras en todos los ejemplos:
```cpp
#include <iostream>
#include <chrono>
#include <thread>
#include <omp.h>
using namespace std;
```
Demostración de Directivas OpenMP en C++ (Linux)

Este documento presenta un programa en C++ que demuestra el uso de varias directivas de OpenMP para programación paralela en Linux. El código está organizado en una clase `OpenMPDemo` con funciones separadas para cada directiva, incluyendo medición de tiempos de ejecución.

## **1\. \#pragma omp parallel**

### **Propósito**

Crea un equipo de hilos que ejecutan el mismo bloque de código en paralelo.

### **Funcionamiento**

Cada hilo ejecuta una copia idéntica del código dentro del bloque.

### **Cláusulas comunes**

* num\_threads(n): Especifica el número de hilos.

### **Ejemplo**

#### **Secuencial**

```cpp
void sequential_hello() { 
    cout << "Hola desde el hilo principal\n";  
}

int main() {  
    auto start = chrono::high_resolution_clock::now();  
    sequential_hello();  
    auto end = chrono::high_resolution_clock::now();  
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);  
    cout << "Tiempo secuencial: " << duration.count() << " ms\n";  
    return 0;  
}
```

#### **Paralelo**
```cpp 
void parallel_hello() {  
    #pragma omp parallel num_threads(4)  
    {  
        int id = omp_get_thread_num();  
        cout << "Hola desde el hilo " << id << "\n";  
    }  
}

int main() {  
    auto start = chrono::high_resolution_clock::now(); 
    parallel_hello();  
    auto end = chrono::high_resolution_clock::now();  
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);  
    cout << "Tiempo paralelo: " << duration.count() << " ms\n"; 
    return 0;  
}

```

### **Resultado**

* **Secuencial**: 1 saludo, tiempo mínimo (\~0 ms).  
* **Paralelo**: 4 saludos (con 4 hilos), tiempo mínimo (\~0 ms).

---

## **2\. \#pragma omp parallel for**

### **Propósito**

Divide las iteraciones de un bucle entre los hilos disponibles.

### **Funcionamiento**

Ideal para bucles con iteraciones independientes.

### **Cláusulas comunes**

* schedule(type): Define cómo se distribuyen las iteraciones (e.g., static, dynamic).

### **Ejemplo**

#### **Secuencial**

```cpp 
void sequential_loop() {  
    for (int i = 0; i < 8; i++) {  
        this_thread::sleep_for(chrono::milliseconds(250)); // Simula trabajo  
        cout << "Iteración " << i << " completada\n";  
    }  
}

int main() {  
    auto start = chrono::high_resolution_clock::now();  
    sequential_loop();  
    auto end = chrono::high_resolution_clock::now();  
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);  
    cout << "Tiempo secuencial: " << duration.count() << " ms\n";  
    return 0;  
}
```

#### **Paralelo**

```cpp 
void parallel_loop() {  
    #pragma omp parallel for num_threads(4)  
    for (int i = 0; i < 8; i++) {  
        this_thread::sleep_for(chrono::milliseconds(250));`  
        cout << "Iteración " << i << " completada por hilo " << omp_get_thread_num() << "\n";  
    }  
}

int main() {  
    auto start = chrono::high_resolution_clock::now();  
    parallel_loop();  
    auto end = chrono::high_resolution_clock::now();  
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);  
    cout << "Tiempo paralelo: " << duration.count() << " ms\n";  
    return 0;  
}
```

### **Resultado**

* **Secuencial**: \~2000 ms (8 × 250 ms).  
* **Paralelo**: \~500 ms (8 iteraciones ÷ 4 hilos).

---

## **3\. \#pragma omp sections y \#pragma omp section**

### **Propósito**

Ejecuta diferentes bloques de código en paralelo.

### **Funcionamiento**

Cada section se asigna a un hilo diferente. Requiere estar dentro de un parallel.

### **Ejemplo**

#### **Secuencial**

```cpp  
void sequential_sections() {  
    this_thread::sleep_for(chrono::seconds(1)); // Sección 1  
    cout << "Sección 1 completada\n";  
    this_thread::sleep_for(chrono::seconds(1)); // Sección 2  
    cout << "Sección 2 completada\n";  
}

int main() {  
    auto start = chrono::high_resolution_clock::now();  
    sequential_sections();  
    auto end = chrono::high_resolution_clock::now();  
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);  
    cout << "Tiempo secuencial: " << duration.count() << " ms\n";  
    return 0;  
}
```

#### **Paralelo**
```cpp
void parallel_sections() {  
    #pragma omp parallel sections num_threads(2)  
    {  
        #pragma omp section  
        {  
            this_thread::sleep_for(chrono::seconds(1));  
            cout << "Sección 1 completada por hilo " << omp_get_thread_num() << "\n";  
        }  
        #pragma omp section  
        {  
            this_thread::sleep_for(chrono::seconds(1));  
            cout << "Sección 2 completada por hilo " << omp_get_thread_num() << "\n";  
        }  
    }  
}

int main() {  
    auto start = chrono::high_resolution_clock::now();  
    parallel_sections();  
    auto end = chrono::high_resolution_clock::now();  
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);  
    cout << "Tiempo paralelo: " << duration.count() << " ms\n";  
    return 0;  
}
```

### **Resultado**

* **Secuencial**: \~2000 ms (1 \+ 1 s).  
* **Paralelo**: \~1000 ms (2 secciones en paralelo).

---

## **4\. \#pragma omp single**

### **Propósito**

Solo un hilo ejecuta el bloque, mientras otros esperan.

### **Funcionamiento**

Incluye una barrera implícita, a menos que se use nowait.

### **Cláusulas comunes**

* nowait: Elimina la barrera.

### **Ejemplo**

#### **Secuencial**
```cpp 
void sequential_single() {  
    this_thread::sleep_for(chrono::seconds(1));  
    cout << "Tarea única completada\n";  
}

int main() {  
    auto start = chrono::high_resolution_clock::now();  
    sequential_single();  
    auto end = chrono::high_resolution_clock::now();  
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);  
    cout << "Tiempo secuencial: " << duration.count() << " ms\n";  
    return 0;  
}
```

#### **Paralelo**
```cpp
void parallel_single() {  
    #pragma omp parallel num_threads(4)  
    {  
        #pragma omp single nowait  
        {  
            this_thread::sleep_for(chrono::seconds(1));  
            cout << "Tarea única completada por hilo " << omp_get_thread_num() << "\n";  
        }  
        cout << "Hilo " << omp_get_thread_num() << " continúa\n";  
    }  
}

int main() {  
    auto start = chrono::high_resolution_clock::now();  
    parallel_single();  
    auto end = chrono::high_resolution_clock::now();  
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);  
    cout << "Tiempo paralelo: " << duration.count() << " ms\n";  
    return 0;  
}
```
### **Resultado**

* **Secuencial**: \~1000 ms.  
* **Paralelo**: \~1000 ms \+ overhead (sin nowait sería igual, pero con espera).

---

## **5\. \#pragma omp reduction**

### **Propósito**

Realiza operaciones de reducción seguras en paralelo (e.g., suma, producto).

### **Funcionamiento**

Cada hilo tiene una copia privada de la variable, que se combina al final.

### **Cláusulas comunes**

* reduction(operador:variable): Define la operación.

### **Ejemplo**

#### **Secuencial**
```cpp  
void sequential_reduction() {  
    int sum = 0;  
    for (int i = 1; i <= 10; i++) {  
        this_thread::sleep_for(chrono::milliseconds(300));  
        sum += i;  
    }  
    cout << "Suma secuencial: " << sum << "\n";  
}

int main() {  
    auto start = chrono::high_resolution_clock::now();  
    sequential_reduction();  
    auto end = chrono::high_resolution_clock::now();  
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);  
    cout << "Tiempo secuencial: " << duration.count() << " ms\n";  
    return 0;  
}
```

#### **Paralelo**
```cpp
void parallel_reduction() {  
    int sum = 0;  
    #pragma omp parallel for reduction(+:sum) num_threads(4)  
    for (int i = 1; i <= 10; i++) {  
        this_thread::sleep_for(chrono::milliseconds(300));  
        sum += i;  
    }  
    cout << "Suma paralela: " << sum << "\n";  
}

int main() {  
    auto start = chrono::high_resolution_clock::now();  
    parallel_reduction();  
    auto end = chrono::high_resolution_clock::now();  
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);  
    cout << "Tiempo paralelo: " << duration.count() << " ms\n";  
    return 0;  
}
```
### **Resultado**

* **Secuencial**: \~3000 ms (10 × 300 ms).  
* **Paralelo**: \~750 ms (10 iteraciones ÷ 4 hilos).

---

## **6\. \#pragma omp critical**

### **Propósito**

Protege una sección crítica para acceso exclusivo.

### **Funcionamiento**

Solo un hilo a la vez puede entrar.

### **Cláusulas comunes**

* (nombre): Identifica secciones críticas específicas.

### **Ejemplo**

#### **Secuencial**

```cpp  
void sequential_critical() {  
    int counter = 0;  
    for (int i = 0; i < 4; i++) {  
        this_thread::sleep_for(chrono::seconds(1));  
        counter++;  
    }  
    cout << "Contador secuencial: " << counter << "\n";  
}

int main() {  
    auto start = chrono::high_resolution_clock::now();  
    sequential_critical();  
    auto end = chrono::high_resolution_clock::now();  
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);  
    cout << "Tiempo secuencial: " << duration.count() << " ms\n";  
    return 0;  
}
```

#### **Paralelo**
```cpp  
void parallel_critical() {  
    int counter = 0;`  
    #pragma omp parallel for num_threads(4)`  
    for (int i = 0; i < 4; i++) {  
        this_thread::sleep_for(chrono::seconds(1));  
        #pragma omp critical  
        counter++;  
    }  
    cout << "Contador paralelo: " << counter << "\n";  
}

int main() {  
    auto start = chrono::high_resolution_clock::now();  
    parallel_critical();  
    auto end = chrono::high_resolution_clock::now();  
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);  
    cout << "Tiempo paralelo: " << duration.count() << " ms\n";  
    return 0;  
}
```

### **Resultado**

* **Secuencial**: \~4000 ms (4 × 1000 ms).  
* **Paralelo**: \~4000 ms (acceso secuencial forzado por critical).

---

## **7\. \#pragma omp barrier**

### **Propósito**

Sincroniza todos los hilos en un punto específico.

### **Funcionamiento**

Ningún hilo avanza hasta que todos llegan.

### **Ejemplo**

#### **Secuencial**

```cpp  
void sequential_barrier() {  
    this_thread::sleep_for(chrono::seconds(1));  
    cout << "Fase 1 completada\n";  
    this_thread::sleep_for(chrono::seconds(1));  
    cout << "Fase 2 completada\n";  
}

int main() {  
    auto start = chrono::high_resolution_clock::now();  
    sequential_barrier();  
    auto end = chrono::high_resolution_clock::now();  
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);  
    cout << "Tiempo secuencial: " << duration.count() << " ms\n";  
    return 0;  
}
```

#### **Paralelo**
```cpp  
void parallel_barrier() {  
    #pragma omp parallel num_threads(4)  
    {  
        int id = omp_get_thread_num();  
        this_thread::sleep_for(chrono::milliseconds(500 + id * 100)); // Diferentes tiempos  
        cout << "Fase 1 completada por hilo " << id << "\n";  
        #pragma omp barrier  
        cout << "Fase 2 completada por hilo " << id << "\n";  
    }  
}

int main() {  
    auto start = chrono::high_resolution_clock::now();  
    parallel_barrier();  
    auto end = chrono::high_resolution_clock::now();  
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);  
    cout << "Tiempo paralelo: " << duration.count() << " ms\n";  
    return 0;  
}
```

### **Resultado**

* **Secuencial**: \~2000 ms.  
* **Paralelo**: \~800 ms (limitado por el hilo más lento: 500 \+ 300 ms).

---

## **8\. \#pragma omp task**

### **Propósito**

Crea tareas que se distribuyen entre los hilos.

### **Funcionamiento**

Ideal para trabajo irregular o dinámico.

### **Cláusulas comunes**

* depend(tipo:vars): Define dependencias entre tareas.

### **Ejemplo**

#### **Secuencial**

```cpp
void sequential_tasks() {  
    for (int i = 0; i < 6; i++) {  
        this_thread::sleep_for(chrono::milliseconds(500));  
        cout << "Tarea " << i << " completada\n";  
    }  
}

int main() {  
    auto start = chrono::high_resolution_clock::now();  
    sequential_tasks();  
    auto end = chrono::high_resolution_clock::now();  
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);  
    cout << "Tiempo secuencial: " << duration.count() << " ms\n";  
    return 0;  
}
```

#### **Paralelo**
```cpp
void parallel_tasks() {  
    #pragma omp parallel num_threads(4)  
    {  
        #pragma omp single  
        for (int i = 0; i < 6; i++) {  
            #pragma omp task  
            {  
                this_thread::sleep_for(chrono::milliseconds(500));  
                cout << "Tarea " << i << " completada por hilo " << omp_get_thread_num() << "\n";  
            }  
        }  
    }  
}

int main() {  
    auto start = chrono::high_resolution_clock::now();  
    parallel_tasks();  
    auto end = chrono::high_resolution_clock::now();  
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);  
    cout << "Tiempo paralelo: " << duration.count() << " ms\n";  
    return 0;  
}
```

### **Resultado**

* **Secuencial**: \~3000 ms (6 × 500 ms).  
* **Paralelo**: \~1500 ms (6 tareas ÷ 4 hilos).

---

## **Análisis de Rendimiento**

### **Metodología**

* **Medición**: Se usa \<chrono\> para medir tiempos en milisegundos.  
* **Simulación**: Tiempos simulados con sleep para representar trabajo computacional.

### **Resultados Comparativos**

| Directiva | Tiempo Secuencial | Tiempo Paralelo | Observación |
| ----- | ----- | ----- | ----- |
| parallel | \~0 ms | \~0 ms | Tiempo mínimo |
| parallel for | \~2000 ms | \~500 ms | 8 iteraciones ÷ 4 hilos |
| sections | \~2000 ms | \~1000 ms | 2 secciones en paralelo |
| single | \~1000 ms | \~1000 ms | \+ overhead en paralelo |
| reduction | \~3000 ms | \~750 ms | 10 iteraciones ÷ 4 hilos |
| critical | \~4000 ms | \~4000 ms | Secuencial por acceso exclusivo |
| barrier | \~2000 ms | \~800 ms | Limitado por el más lento |
| task | \~3000 ms | \~1500 ms | 6 tareas ÷ 4 hilos |

### **Conclusión**

* **Ganancias**: parallel for, sections, reduction, y task muestran mejoras significativas al distribuir el trabajo.  
* **Limitaciones**: critical no mejora debido a la exclusión mutua; barrier depende del hilo más lento.  
* **Dependencia**: Los tiempos reales varían según hardware, sistema operativo y carga del sistema.

---
