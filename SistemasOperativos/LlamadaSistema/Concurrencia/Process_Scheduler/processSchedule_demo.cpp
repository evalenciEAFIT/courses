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