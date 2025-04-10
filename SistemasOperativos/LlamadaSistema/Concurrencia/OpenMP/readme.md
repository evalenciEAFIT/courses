#include <iostream>
#include <omp.h>
#include <unistd.h>

using namespace std;

class OpenMPDemo {
private:
    int num_threads;

public:
    OpenMPDemo() {
        num_threads = omp_get_max_threads();
        cout << "Número máximo de hilos disponibles: " << num_threads << endl;
    }

    // 1. Región paralela básica
    void demoParallel() {
        cout << "\n=== Demostración de parallel ===\n";
        #pragma omp parallel num_threads(4)
        {
            int id = omp_get_thread_num();
            cout << "Hola desde el hilo " << id << endl;
        }
    }

    // 2. Paralelización de bucle
    void demoFor() {
        cout << "\n=== Demostración de parallel for ===\n";
        #pragma omp parallel for num_threads(4)
        for(int i = 0; i < 8; i++) {
            cout << "Iteración " << i << " procesada por hilo " 
                 << omp_get_thread_num() << endl;
        }
    }

    // 3. Secciones paralelas
    void demoSections() {
        cout << "\n=== Demostración de sections ===\n";
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
    }

    // 4. Ejecución por un solo hilo
    void demoSingle() {
        cout << "\n=== Demostración de single ===\n";
        #pragma omp parallel num_threads(4)
        {
            #pragma omp single
            {
                cout << "Tarea single ejecutada por hilo " 
                     << omp_get_thread_num() << endl;
            }
            cout << "Hilo " << omp_get_thread_num() 
                 << " después del single" << endl;
        }
    }

    // 5. Reducción paralela
    void demoReduction() {
        cout << "\n=== Demostración de reduction ===\n";
        int suma = 0;
        #pragma omp parallel for reduction(+:suma) num_threads(4)
        for(int i = 1; i <= 10; i++) {
            suma += i;
        }
        cout << "Suma de 1 a 10 = " << suma << endl;
    }
};

int main() {
    OpenMPDemo demo;
    
    demo.demoParallel();
    demo.demoFor();
    demo.demoSections();
    demo.demoSingle();
    demo.demoReduction();

    return 0;
}
Compilación y ejecución (igual que antes):

bash

Collapse

Wrap

Copy
g++ -fopenmp openmp_demo.cpp -o openmp_demo
./openmp_demo
Explicación detallada de las directivas OpenMP
#pragma omp parallel
Propósito: Crea un equipo de hilos que ejecutan el bloque de código siguiente en paralelo.
Funcionamiento: Cada hilo ejecuta una copia idéntica del código dentro del bloque.
Cláusulas comunes:
num_threads(n): Especifica el número de hilos a usar.
En el ejemplo: Muestra un mensaje desde cada hilo creado.
#pragma omp parallel for
Propósito: Combina parallel con la distribución automática de iteraciones de un bucle for entre los hilos.
Funcionamiento: Divide las iteraciones entre los hilos disponibles, ideal para bucles independientes.
Cláusulas comunes:
schedule(type): Define cómo se distribuyen las iteraciones (static, dynamic, etc.).
En el ejemplo: Procesa 8 iteraciones distribuidas entre 4 hilos.
#pragma omp sections y #pragma omp section
Propósito: Permite ejecutar diferentes bloques de código en paralelo por distintos hilos.
Funcionamiento: Cada section es asignada a un hilo disponible, útil para tareas heterogéneas.
Requisito: Debe estar dentro de un parallel.
En el ejemplo: Ejecuta dos secciones distintas simultáneamente.
#pragma omp single
Propósito: Especifica que solo un hilo del equipo ejecutará el bloque de código.
Funcionamiento: Los otros hilos esperan en una barrera implícita hasta que el bloque single termina (a menos que se use nowait).
Cláusulas comunes:
nowait: Elimina la barrera al final.
En el ejemplo: Solo un hilo imprime el mensaje single, pero todos continúan después.
#pragma omp reduction
Propósito: Realiza operaciones de reducción (suma, producto, etc.) de manera segura en paralelo.
Funcionamiento: Cada hilo tiene una copia privada de la variable, y al final se combinan usando el operador especificado (en este caso +).
Cláusulas comunes:
reduction(operador:variable): Define la operación y variable a reducir.
En el ejemplo: Calcula la suma de 1 a 10 en paralelo.
Características adicionales del código
La clase OpenMPDemo encapsula las demostraciones.
Uso de num_threads(4) para controlar explícitamente el número de hilos (puedes ajustarlo).
Constructor que muestra el número máximo de hilos disponibles en el sistema.

#include <iostream>
#include <omp.h>
#include <unistd.h>

using namespace std;

class OpenMPDemo {
private:
    int num_threads;

public:
    OpenMPDemo() {
        num_threads = omp_get_max_threads();
        cout << "Número máximo de hilos disponibles: " << num_threads << endl;
    }

    // 1. Región paralela básica
    void demoParallel() {
        cout << "\n=== Demostración de parallel ===\n";
        #pragma omp parallel num_threads(4)
        {
            int id = omp_get_thread_num();
            cout << "Hola desde el hilo " << id << endl;
        }
    }

    // 2. Paralelización de bucle
    void demoFor() {
        cout << "\n=== Demostración de parallel for ===\n";
        #pragma omp parallel for num_threads(4)
        for(int i = 0; i < 8; i++) {
            cout << "Iteración " << i << " procesada por hilo " 
                 << omp_get_thread_num() << endl;
        }
    }

    // 3. Secciones paralelas
    void demoSections() {
        cout << "\n=== Demostración de sections ===\n";
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
    }

    // 4. Ejecución por un solo hilo
    void demoSingle() {
        cout << "\n=== Demostración de single ===\n";
        #pragma omp parallel num_threads(4)
        {
            #pragma omp single
            {
                cout << "Tarea single ejecutada por hilo " 
                     << omp_get_thread_num() << endl;
            }
            cout << "Hilo " << omp_get_thread_num() 
                 << " después del single" << endl;
        }
    }

    // 5. Reducción paralela
    void demoReduction() {
        cout << "\n=== Demostración de reduction ===\n";
        int suma = 0;
        #pragma omp parallel for reduction(+:suma) num_threads(4)
        for(int i = 1; i <= 10; i++) {
            suma += i;
        }
        cout << "Suma de 1 a 10 = " << suma << endl;
    }
};

int main() {
    OpenMPDemo demo;
    
    demo.demoParallel();
    demo.demoFor();
    demo.demoSections();
    demo.demoSingle();
    demo.demoReduction();

    return 0;
}
Compilación y ejecución (igual que antes):

bash

Collapse

Wrap

Copy
g++ -fopenmp openmp_demo.cpp -o openmp_demo
./openmp_demo
Explicación detallada de las directivas OpenMP
#pragma omp parallel
Propósito: Crea un equipo de hilos que ejecutan el bloque de código siguiente en paralelo.
Funcionamiento: Cada hilo ejecuta una copia idéntica del código dentro del bloque.
Cláusulas comunes:
num_threads(n): Especifica el número de hilos a usar.
En el ejemplo: Muestra un mensaje desde cada hilo creado.
#pragma omp parallel for
Propósito: Combina parallel con la distribución automática de iteraciones de un bucle for entre los hilos.
Funcionamiento: Divide las iteraciones entre los hilos disponibles, ideal para bucles independientes.
Cláusulas comunes:
schedule(type): Define cómo se distribuyen las iteraciones (static, dynamic, etc.).
En el ejemplo: Procesa 8 iteraciones distribuidas entre 4 hilos.
#pragma omp sections y #pragma omp section
Propósito: Permite ejecutar diferentes bloques de código en paralelo por distintos hilos.
Funcionamiento: Cada section es asignada a un hilo disponible, útil para tareas heterogéneas.
Requisito: Debe estar dentro de un parallel.
En el ejemplo: Ejecuta dos secciones distintas simultáneamente.
#pragma omp single
Propósito: Especifica que solo un hilo del equipo ejecutará el bloque de código.
Funcionamiento: Los otros hilos esperan en una barrera implícita hasta que el bloque single termina (a menos que se use nowait).
Cláusulas comunes:
nowait: Elimina la barrera al final.
En el ejemplo: Solo un hilo imprime el mensaje single, pero todos continúan después.
#pragma omp reduction
Propósito: Realiza operaciones de reducción (suma, producto, etc.) de manera segura en paralelo.
Funcionamiento: Cada hilo tiene una copia privada de la variable, y al final se combinan usando el operador especificado (en este caso +).
Cláusulas comunes:
reduction(operador:variable): Define la operación y variable a reducir.
En el ejemplo: Calcula la suma de 1 a 10 en paralelo.
Características adicionales del código
La clase OpenMPDemo encapsula las demostraciones.
Uso de num_threads(4) para controlar explícitamente el número de hilos (puedes ajustarlo).
Constructor que muestra el número máximo de hilos disponibles en el sistema.
