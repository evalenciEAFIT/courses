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
#include <iostream> // Incluye la biblioteca para operaciones de entrada/salida estándar (como imprimir en la consola).
#include <vector>   // Incluye la biblioteca para usar el contenedor dinámico de arreglos 'vector'.
#include <queue>    // Incluye la biblioteca para usar la estructura de datos 'queue' (cola).

// Definición de una estructura llamada 'Proceso' para representar un proceso en el sistema.
struct Proceso {
    int id;             // Identificador único del proceso.
    int tiempo_ejecucion; // Tiempo total de ejecución requerido por el proceso.
    int tiempo_restante;  // Tiempo restante de ejecución para el proceso.

    // Constructor de la estructura 'Proceso'. Se llama cuando se crea un objeto 'Proceso'.
    Proceso(int id, int tiempo) : id(id), tiempo_ejecucion(tiempo), tiempo_restante(tiempo) {}
    // 'id(id)' inicializa el miembro 'id' con el valor pasado al constructor.
    // 'tiempo_ejecucion(tiempo)' inicializa el miembro 'tiempo_ejecucion' con el valor pasado.
    // 'tiempo_restante(tiempo)' inicializa el miembro 'tiempo_restante' con el valor pasado (al inicio es igual al tiempo total).
};

int main() {
    std::queue<Proceso> cola_listos; // Crea una cola para almacenar los procesos que están listos para ejecutarse.
                                     // Se utiliza una cola porque Round Robin procesa los procesos en orden de llegada (FIFO).
    std::vector<Proceso> procesos;    // Crea un vector para almacenar los procesos que se simularán.
    int квантум = 2;                  // Define el квантум de tiempo (time slice) para el algoritmo Round Robin.
                                     // Cada proceso se ejecutará durante este квантум antes de ser potencialmente interrumpido.

    // Crear algunos procesos de ejemplo y agregarlos al vector 'procesos'.
    procesos.emplace_back(1, 5); // Crea un proceso con ID 1 y tiempo de ejecución total de 5 unidades.
    procesos.emplace_back(2, 3); // Crea un proceso con ID 2 y tiempo de ejecución total de 3 unidades.
    procesos.emplace_back(3, 8); // Crea un proceso con ID 3 y tiempo de ejecución total de 8 unidades.
    // 'emplace_back' es una forma eficiente de añadir un nuevo elemento al final de un vector.

    // Inicializar la cola de listos con los procesos creados.
    for (auto& p : procesos) { // Itera a través de cada proceso en el vector 'procesos'.
        cola_listos.push(p);   // Añade cada proceso a la cola de listos.
    }

    std::cout << "Simulación de Round Robin ( квантум = " << квантум << "):\n"; // Imprime un mensaje indicando el inicio de la simulación y el квантум utilizado.

    int tiempo_actual = 0; // Variable para rastrear el tiempo actual de la simulación.
    // El bucle 'while' continúa mientras la cola de listos no esté vacía, es decir, mientras haya procesos pendientes de ejecución.
    while (!cola_listos.empty()) {
        Proceso proceso_actual = cola_listos.front(); // Obtiene el proceso que está al frente de la cola (el siguiente en ser ejecutado).
        cola_listos.pop();                           // Elimina el proceso del frente de la cola, ya que ahora se va a ejecutar.

        // Calcula el tiempo que el proceso actual ejecutará en este квантум.
        // Será el mínimo entre el квантум definido y el tiempo restante de ejecución del proceso.
        int tiempo_a_ejecutar = std::min( квантум, proceso_actual.tiempo_restante);
        proceso_actual.tiempo_restante -= tiempo_a_ejecutar; // Reduce el tiempo restante del proceso por la cantidad de tiempo que se ejecutó.
        tiempo_actual += tiempo_a_ejecutar;               // Incrementa el tiempo actual de la simulación.

        // Imprime un mensaje indicando qué proceso se está ejecutando y durante cuánto tiempo.
        std::cout << "Tiempo " << tiempo_actual << ": Ejecutando Proceso " << proceso_actual.id
                  << " (" << tiempo_a_ejecutar << " unidades)\n";

        // Si el proceso aún tiene tiempo restante para ejecutarse, se vuelve a añadir al final de la cola de listos.
        // Esto es la base del algoritmo Round Robin: los procesos no terminados vuelven a esperar su turno.
        if (proceso_actual.tiempo_restante > 0) {
            cola_listos.push(proceso_actual);
        } else {
            // Si el tiempo restante es 0 o menos, significa que el proceso ha terminado su ejecución.
            std::cout << "Tiempo " << tiempo_actual << ": Proceso " << proceso_actual.id << " terminado.\n";
        }
    }

    std::cout << "Simulación completada.\n"; // Imprime un mensaje indicando que la simulación ha finalizado.

    return 0; // Indica que el programa se ejecutó correctamente.
}
```
