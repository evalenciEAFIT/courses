# El Process Scheduler en Sistemas Operativos

## Introducción

El **process scheduler** (planificador de procesos) es un componente fundamental del sistema operativo encargado de decidir qué proceso debe ejecutarse en la CPU en un momento dado. Su objetivo principal es maximizar la utilización de la CPU, minimizar el tiempo de respuesta, garantizar la equidad entre los procesos y lograr un buen rendimiento general del sistema.

En sistemas operativos multitarea, donde múltiples procesos compiten por los recursos del sistema, el scheduler juega un papel crucial para asegurar que todos los procesos tengan la oportunidad de ejecutarse y que los recursos se utilicen de manera eficiente.

## Funciones Principales del Process Scheduler

El scheduler realiza las siguientes funciones clave:

* **Selección de Procesos:** Determina qué proceso de la cola de listos (ready queue) debe ser asignado a la CPU.
* **Conmutación de Contexto:** Cuando se decide cambiar el proceso en ejecución, el scheduler guarda el estado del proceso actual (contexto) y restaura el estado del siguiente proceso a ejecutar. Esta operación se conoce como conmutación de contexto (context switch).
* **Administración de Colas:** Mantiene diferentes colas para gestionar el estado de los procesos (ejecución, listos, bloqueados).

## Algoritmos de Planificación Comunes

Existen diversos algoritmos de planificación, cada uno con sus propias características y objetivos. Algunos de los más comunes incluyen:

* **First-Come, First-Served (FCFS):** El primer proceso que llega a la cola de listos es el primero en ejecutarse. Simple de implementar, pero puede llevar a tiempos de espera largos para procesos cortos si un proceso largo llega primero (efecto convoy).
* **Shortest Job First (SJF):** El proceso con la menor duración estimada se ejecuta primero. Optimiza el tiempo de respuesta promedio, pero requiere conocer la duración de los procesos por adelantado, lo cual no siempre es posible.
* **Priority Scheduling:** A cada proceso se le asigna una prioridad, y el proceso con la prioridad más alta se ejecuta primero. Puede llevar a inanición (starvation) si procesos de baja prioridad nunca llegan a ejecutarse.
* **Round Robin (RR):** A cada proceso se le asigna un квантум de tiempo (time slice). Si un proceso no termina dentro de su квантум, se interrumpe y se mueve al final de la cola de listos. Proporciona equidad entre los procesos, pero el tamaño del квантум es crítico para su rendimiento.
* **Multilevel Queue Scheduling:** Se utilizan múltiples colas de listos con diferentes prioridades y posiblemente diferentes algoritmos de planificación para cada cola.
* **Multilevel Feedback Queue Scheduling:** Similar al anterior, pero los procesos pueden moverse entre las colas en función de su comportamiento.

## Ejemplo en Linux Fedora: Ver el Scheduler en Uso

Linux utiliza un scheduler sofisticado llamado **Completely Fair Scheduler (CFS)** como su scheduler predeterminado para la mayoría de los procesos. CFS busca asignar una porción justa del tiempo de la CPU a cada proceso.

Puedes observar algunos parámetros relacionados con la planificación utilizando herramientas de línea de comandos en Fedora:

1.  **`top` o `htop`:** Estas herramientas muestran información en tiempo real sobre los procesos en ejecución, incluyendo el uso de la CPU y la prioridad (nice value).

    ```bash
    top
    # o
    htop
    ```

    En la salida, puedes observar la columna `%CPU` que indica el porcentaje de tiempo de CPU utilizado por cada proceso. La columna `NI` (nice value) influye en la prioridad del proceso (valores más bajos indican mayor prioridad).

2.  **`ps`:** El comando `ps` te permite listar los procesos en ejecución y ver información detallada sobre ellos, incluyendo la prioridad y el scheduler utilizado (aunque el scheduler específico no siempre se muestra directamente).

    ```bash
    ps -eo pid,ppid,ni,stat,cmd
    ```

    * `pid`: ID del proceso.
    * `ppid`: ID del proceso padre.
    * `ni`: Valor "nice" del proceso.
    * `stat`: Estado del proceso.
    * `cmd`: Comando ejecutado.

3.  **`schedtool`:** Esta herramienta (puede que necesites instalarla con `sudo dnf install schedtool`) permite consultar y modificar la política de planificación y la prioridad de un proceso específico.

    ```bash
    # Ver la política de planificación de un proceso (reemplaza <PID> con el ID del proceso)
    schedtool -p <PID>

    # Establecer la política de planificación a Round Robin (RR) con prioridad 1 para un proceso
    sudo schedtool -P rr -r 1 <PID>
    ```

    **Advertencia:** Modificar la política de planificación de procesos del sistema puede tener consecuencias inesperadas. Úsalo con precaución y solo para experimentación en un entorno de prueba.

## Ejemplo en C++: Simulación Simple de Round Robin

El siguiente ejemplo en C++ ilustra la idea básica del algoritmo de planificación Round Robin para un pequeño conjunto de procesos:

```cpp
#include <iostream>
#include <vector>
#include <queue>

struct Proceso {
    int id;
    int tiempo_ejecucion;
    int tiempo_restante;

    Proceso(int id, int tiempo) : id(id), tiempo_ejecucion(tiempo), tiempo_restante(tiempo) {}
};

int main() {
    std::queue<Proceso> cola_listos;
    std::vector<Proceso> procesos;
    int квантум = 2; // Unidad de tiempo

    // Crear algunos procesos
    procesos.emplace_back(1, 5);
    procesos.emplace_back(2, 3);
    procesos.emplace_back(3, 8);

    // Inicializar la cola de listos
    for (auto& p : procesos) {
        cola_listos.push(p);
    }

    std::cout << "Simulación de Round Robin ( квантум = " << квантум << "):\n";

    int tiempo_actual = 0;
    while (!cola_listos.empty()) {
        Proceso proceso_actual = cola_listos.front();
        cola_listos.pop();

        int tiempo_a_ejecutar = std::min( квантум, proceso_actual.tiempo_restante);
        proceso_actual.tiempo_restante -= tiempo_a_ejecutar;
        tiempo_actual += tiempo_a_ejecutar;

        std::cout << "Tiempo " << tiempo_actual << ": Ejecutando Proceso " << proceso_actual.id
                  << " (" << tiempo_a_ejecutar << " unidades)\n";

        if (proceso_actual.tiempo_restante > 0) {
            cola_listos.push(proceso_actual); // Volver a la cola si no ha terminado
        } else {
            std::cout << "Tiempo " << tiempo_actual << ": Proceso " << proceso_actual.id << " terminado.\n";
        }
    }

    std::cout << "Simulación completada.\n";

    return 0;
}
```
