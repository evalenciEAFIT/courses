#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <cstdlib> // Para system("color ..."); en Windows
#include <iomanip> // Para std::setw

// Definiciones de colores ANSI para la terminal
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_GRAY    "\033[90m"

// Definición de los estados del proceso
enum class EstadoProceso {
    NUEVO,
    LISTO,
    EJECUCION,
    BLOQUEADO,
    TERMINADO
};

// Función para convertir el estado del proceso a una cadena con color
std::string estadoAStringConColor(EstadoProceso estado) {
    switch (estado) {
        case EstadoProceso::NUEVO:
            return COLOR_CYAN "NUEVO" COLOR_RESET;
        case EstadoProceso::LISTO:
            return COLOR_GREEN "LISTO" COLOR_RESET;
        case EstadoProceso::EJECUCION:
            return COLOR_BLUE "EJECUCIÓN" COLOR_RESET;
        case EstadoProceso::BLOQUEADO:
            return COLOR_YELLOW "BLOQUEADO" COLOR_RESET;
        case EstadoProceso::TERMINADO:
            return COLOR_MAGENTA "TERMINADO" COLOR_RESET;
        default:
            return "DESCONOCIDO";
    }
}

// Clase que representa un proceso
class Proceso {
public:
    int id;
    std::string nombreArchivo;
    std::string contenido;
    EstadoProceso estado;
    size_t progreso;
    std::string colorProceso; // Color único para la salida del proceso

    Proceso(int id, const std::string& nombreArchivo, const std::string& color) :
        id(id), nombreArchivo(nombreArchivo), estado(EstadoProceso::NUEVO), progreso(0), colorProceso(color) {}

    bool cargarContenido() {
        std::ifstream archivo(nombreArchivo);
        if (archivo.is_open()) {
            std::stringstream buffer;
            buffer << archivo.rdbuf();
            contenido = buffer.str();
            archivo.close();
            estado = EstadoProceso::LISTO;
            return true;
        } else {
            std::cerr << COLOR_RED "Error al abrir el archivo: " << nombreArchivo << COLOR_RESET << std::endl;
            estado = EstadoProceso::TERMINADO;
            return false;
        }
    }

    bool ejecutarCaracter() {
        if (estado == EstadoProceso::EJECUCION && progreso < contenido.length()) {
            std::cout << colorProceso << contenido[progreso] << COLOR_RESET;
            progreso++;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (progreso == contenido.length()) {
                estado = EstadoProceso::TERMINADO;
                std::cout << COLOR_MAGENTA " (Proceso " << id << " Terminado)" COLOR_RESET << std::endl;
            }
            return true;
        }
        return false;
    }

    void bloquear() {
        if (estado == EstadoProceso::EJECUCION) {
            estado = EstadoProceso::BLOQUEADO;
            std::cout << COLOR_YELLOW "Proceso " << id << " bloqueado." COLOR_RESET << std::endl;
        }
    }

    void desbloquear() {
        if (estado == EstadoProceso::BLOQUEADO) {
            estado = EstadoProceso::LISTO;
            std::cout << COLOR_GREEN "Proceso " << id << " desbloqueado." COLOR_RESET << std::endl;
        }
    }

    void mostrarInfo() const {
        std::cout << COLOR_BLUE "ID: " << std::setw(2) << id << COLOR_RESET
                  << " | Archivo: " << std::left << std::setw(15) << nombreArchivo << std::right
                  << " | Estado: " << estadoAStringConColor(estado)
                  << " | Progreso: " << progreso << "/" << contenido.length() << std::endl;
    }
};

// Clase que simula el planificador de procesos
class Planificador {
public:
    std::vector<Proceso> procesos;
    int siguienteId = 1;
    std::vector<std::string> coloresProceso = {COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE, COLOR_MAGENTA};
    int siguienteColor = 0;

    void agregarProceso(const std::string& nombreArchivo) {
        std::string color = coloresProceso[siguienteColor % coloresProceso.size()];
        Proceso nuevoProceso(siguienteId++, nombreArchivo, color);
        siguienteColor++;
        if (nuevoProceso.cargarContenido()) {
            procesos.push_back(nuevoProceso);
            std::cout << COLOR_GREEN "Proceso " << nuevoProceso.id << " cargado desde " << nombreArchivo << COLOR_RESET << std::endl;
        }
    }

    void simularCiclo() {
        if (procesos.empty()) {
            std::cout << "No hay procesos para ejecutar." << std::endl;
            return;
        }

        // Fase de transición a EJECUCION
        for (auto& proceso : procesos) {
            if (proceso.estado == EstadoProceso::LISTO && proceso.estado != EstadoProceso::EJECUCION) {
                proceso.estado = EstadoProceso::EJECUCION;
            }
        }

        std::cout << "\n--- Ejecución de Ráfaga ---" << std::endl;
        for (auto& proceso : procesos) {
            if (proceso.estado == EstadoProceso::EJECUCION) {
                // Simular un evento aleatorio de bloqueo (15% de probabilidad)
                if (rand() % 7 == 0) {
                    proceso.bloquear();
                } else {
                    proceso.ejecutarCaracter();
                }
            }
        }
        std::cout << "\n--- Fin de Ráfaga ---" << std::endl;

        // Fase de desbloqueo aleatorio (10% de probabilidad para cada proceso bloqueado)
        for (auto& proceso : procesos) {
            if (proceso.estado == EstadoProceso::BLOQUEADO && rand() % 10 == 0) {
                proceso.desbloquear();
            }
        }
    }

    void mostrarEstadoProcesos() const {
        std::cout << "\n--- Estado de los Procesos ---" << std::endl;
        std::cout << COLOR_BLUE "ID | Archivo           | Estado     | Progreso" COLOR_RESET << std::endl;
        std::cout << "---------------------------------------------" << std::endl;
        for (const auto& proceso : procesos) {
            proceso.mostrarInfo();
        }
        std::cout << "---------------------------------------------" << std::endl;
    }

    bool todosTerminados() const {
        for (const auto& proceso : procesos) {
            if (proceso.estado != EstadoProceso::TERMINADO) {
                return false;
            }
        }
        return true;
    }
};

int main() {
#ifdef _WIN32
    system("color");
#endif

    Planificador planificador;
    srand(time(nullptr));

    planificador.agregarProceso("proceso1.txt");
    planificador.agregarProceso("proceso2.txt");
    planificador.agregarProceso("proceso3.txt");
    planificador.agregarProceso("proceso4.txt"); // Agregamos un cuarto proceso

    std::cout << "\n--- Simulación del Planificador ---" << std::endl;

    while (!planificador.todosTerminados()) {
        planificador.simularCiclo();
        planificador.mostrarEstadoProcesos();
        std::this_thread::sleep_for(std::chrono::milliseconds(800)); // Aumentamos la pausa para ver mejor
    }

    std::cout << "\n--- Todos los procesos han terminado. ---" << std::endl;

    return 0;
}

// Contenido de ejemplo para los archivos:
// proceso1.txt: Texto del primer proceso.
// proceso2.txt: Contenido del segundo trabajo.
// proceso3.txt: Un tercer proceso en marcha.
// proceso4.txt: Edi.