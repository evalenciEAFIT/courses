#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <thread>

// Definiciones de códigos de escape ANSI para colores de texto en la terminal.
#define RESET   "\033[0m"
#define ROJO    "\033[31m"
#define VERDE   "\033[32m"
#define AMARILLO "\033[33m"
#define AZUL    "\033[34m"
#define MAGENTA "\033[35m"
#define CIAN    "\033[36m"

// Definición de la estructura 'Proceso'.
struct Proceso {
    int id;
    int tiempo_ejecucion;
    int tiempo_restante;
    std::string estado;

    Proceso(int id, int tiempo) : id(id), tiempo_ejecucion(tiempo), tiempo_restante(tiempo), estado("CREADO") {}

    void imprimir_info() const {
        std::cout << "Proceso " << id << " (Tiempo Total: " << tiempo_ejecucion
                  << ", Restante: " << tiempo_restante << ", Estado: ";
        if (estado == "CREADO") std::cout << AZUL << estado << RESET;
        else if (estado == "LISTO") std::cout << AMARILLO << estado << RESET;
        else if (estado == "EJECUTANDO") std::cout << VERDE << estado << RESET;
        else if (estado == "TERMINADO") std::cout << MAGENTA << estado << RESET;
        else std::cout << estado;
        std::cout << ")\n";
    }
};

// Función para simular la creación de procesos.
std::vector<Proceso> crear_procesos(int num_procesos, int max_tiempo) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribucion_tiempo(1, max_tiempo);
    std::vector<Proceso> procesos_creados;
    std::cout << "\n" << AZUL << "--- Creación de Procesos ---" << RESET << "\n";
    for (int i = 1; i <= num_procesos; ++i) {
        int tiempo = distribucion_tiempo(gen);
        procesos_creados.emplace_back(i, tiempo);
        procesos_creados.back().imprimir_info();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return procesos_creados;
}

// Función para admitir los procesos a la cola de listos.
std::queue<Proceso> admitir_a_cola(std::vector<Proceso>& procesos) {
    std::queue<Proceso> cola_listos;
    std::cout << "\n" << AMARILLO << "--- Admisión a la Cola de Listos ---" << RESET << "\n";
    for (auto& p : procesos) {
        p.estado = "LISTO";
        cola_listos.push(p);
        p.imprimir_info();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    return cola_listos;
}

// Función para simular la ejecución de un Round Robin (RR) "quantum" de tiempo para un proceso.
int ejecutar_proceso(Proceso& proceso, int FraccionTiempo, int tiempo_actual) {
    int tiempo_a_ejecutar = std::min( FraccionTiempo, proceso.tiempo_restante);
    proceso.tiempo_restante -= tiempo_a_ejecutar;
    int nuevo_tiempo_actual = tiempo_actual + tiempo_a_ejecutar;
    std::cout << "Tiempo " << nuevo_tiempo_actual << ": Ejecutando Proceso " << proceso.id
              << " durante " << tiempo_a_ejecutar << " unidades.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    return nuevo_tiempo_actual;
}

// Función para gestionar la terminación o el retorno a la cola de un proceso.
void gestionar_proceso_fin_FraccionTiempo(Proceso& proceso, std::queue<Proceso>& cola_listos, int tiempo_actual) {
    if (proceso.tiempo_restante > 0) {
        proceso.estado = "LISTO";
        std::cout << "Tiempo " << tiempo_actual << ": Proceso " << proceso.id << " vuelve a la cola de listos.\n";
        cola_listos.push(proceso);
    } else {
        proceso.estado = "TERMINADO";
        std::cout << MAGENTA << "Tiempo " << tiempo_actual << ": Proceso " << proceso.id << " terminado." << RESET << "\n";
    }
    proceso.imprimir_info();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int main() {
    int num_procesos = 5;
    int max_tiempo_ejecucion = 10;
    int FraccionTiempo = 2;
    int tiempo_actual = 0;

    std::cout << CIAN << "Simulación del Planificador de Procesos con Funciones\n" << RESET;

    // Fase de Creación
    std::vector<Proceso> procesos = crear_procesos(num_procesos, max_tiempo_ejecucion);

    // Fase de Admisión
    std::queue<Proceso> cola_listos = admitir_a_cola(procesos);

    // Fase de Ejecución (Round Robin)
    std::cout << "\n" << VERDE << "--- Ejecución (Round Robin, FraccionTiempo = " << FraccionTiempo << ") ---" << RESET << "\n";
    while (!cola_listos.empty()) {
        Proceso proceso_actual = cola_listos.front();
        cola_listos.pop();
        proceso_actual.estado = "EJECUTANDO";
        proceso_actual.imprimir_info();

        tiempo_actual = ejecutar_proceso(proceso_actual, FraccionTiempo, tiempo_actual);

        gestionar_proceso_fin_FraccionTiempo(proceso_actual, cola_listos, tiempo_actual);
    }

    std::cout << "\n" << CIAN << "Simulación completada." << RESET << "\n";

    return 0;
}