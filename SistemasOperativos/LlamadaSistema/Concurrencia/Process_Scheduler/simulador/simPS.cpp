#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <cstdlib> // Para system("color ..."); en Windows

// Definiciones de colores ANSI para la terminal (pueden no funcionar en todas las terminales)
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"

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
    size_t progreso; // Para rastrear el carácter actual que se está "ejecutando"

    Proceso(int id, const std::string& nombreArchivo) :
        id(id), nombreArchivo(nombreArchivo), estado(EstadoProceso::NUEVO), progreso(0) {}

    // Función para cargar el contenido del archivo del proceso
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
            estado = EstadoProceso::TERMINADO; // Considerar como terminado si no se puede cargar
            return false;
        }
    }

    // Simula la ejecución de un carácter del proceso
    bool ejecutarCaracter() {
        if (estado == EstadoProceso::EJECUCION && progreso < contenido.length()) {
            std::cout << COLOR_GREEN "Proceso " << id << COLOR_RESET << ": " << contenido[progreso];
            progreso++;
            // Simular un breve tiempo de ejecución
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (progreso == contenido.length()) {
                estado = EstadoProceso::TERMINADO;
                std::cout << COLOR_MAGENTA " (Terminado)" COLOR_RESET << std::endl;
            }
            return true;
        }
        return false;
    }

    // Simula el bloqueo del proceso
    void bloquear() {
        if (estado == EstadoProceso::EJECUCION) {
            estado = EstadoProceso::BLOQUEADO;
            std::cout << COLOR_YELLOW "Proceso " << id << " bloqueado." COLOR_RESET << std::endl;
        }
    }

    // Simula el desbloqueo del proceso
    void desbloquear() {
        if (estado == EstadoProceso::BLOQUEADO) {
            estado = EstadoProceso::LISTO;
            std::cout << COLOR_GREEN "Proceso " << id << " desbloqueado." COLOR_RESET << std::endl;
        }
    }

    // Muestra la información del proceso con color
    void mostrarInfo() const {
        std::cout << COLOR_BLUE "Proceso ID: " << id << COLOR_RESET
                  << ", Archivo: " << nombreArchivo
                  << ", Estado: " << estadoAStringConColor(estado)
                  << ", Progreso: " << progreso << "/" << contenido.length() << std::endl;
    }
};

// Clase que simula el planificador de procesos
class Planificador {
public:
    std::vector<Proceso> procesos;
    int siguienteId = 1;

    // Función para agregar un nuevo proceso desde un archivo
    void agregarProceso(const std::string& nombreArchivo) {
        Proceso nuevoProceso(siguienteId++, nombreArchivo);
        if (nuevoProceso.cargarContenido()) {
            procesos.push_back(nuevoProceso);
            std::cout << COLOR_GREEN "Proceso " << nuevoProceso.id << " cargado desde " << nombreArchivo << COLOR_RESET << std::endl;
        }
    }

    // Simula un ciclo del planificador (ejecución Round Robin simple)
    void simularCiclo() {
        if (procesos.empty()) {
            std::cout << "No hay procesos para ejecutar." << std::endl;
            return;
        }

        for (auto& proceso : procesos) {
            if (proceso.estado == EstadoProceso::LISTO) {
                proceso.estado = EstadoProceso::EJECUCION;
            }
        }

        for (auto& proceso : procesos) {
            if (proceso.estado == EstadoProceso::EJECUCION) {
                // Simular un evento aleatorio de bloqueo (10% de probabilidad)
                if (rand() % 10 == 0) {
                    proceso.bloquear();
                } else {
                    proceso.ejecutarCaracter();
                }
            }
        }

        // Simular un evento aleatorio de desbloqueo (5% de probabilidad)
        for (auto& proceso : procesos) {
            if (proceso.estado == EstadoProceso::BLOQUEADO && rand() % 20 == 0) {
                proceso.desbloquear();
            }
        }
    }

    // Muestra el estado de todos los procesos
    void mostrarEstadoProcesos() const {
        std::cout << "\n--- Estado de los Procesos ---" << std::endl;
        for (const auto& proceso : procesos) {
            proceso.mostrarInfo();
        }
        std::cout << "-----------------------------" << std::endl;
    }

    // Verifica si todos los procesos han terminado
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
    // Habilitar colores en la terminal de Windows (si es necesario)
#ifdef _WIN32
    system("color");
#endif

    Planificador planificador;
    srand(time(nullptr)); // Inicializar la semilla para números aleatorios

    // Cargar algunos procesos de archivos (debes crear estos archivos)
    planificador.agregarProceso("proceso1.txt");
    planificador.agregarProceso("proceso2.txt");
    planificador.agregarProceso("proceso3.txt");

    std::cout << "\n--- Simulación del Planificador ---" << std::endl;

    while (!planificador.todosTerminados()) {
        planificador.simularCiclo();
        planificador.mostrarEstadoProcesos();
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Pausa para visualizar
    }

    std::cout << "\n--- Todos los procesos han terminado. ---" << std::endl;

    return 0;
}

// Contenido de ejemplo para los archivos:
// proceso1.txt: Hola desde el proceso 1!
// proceso2.txt: Este es el proceso numero dos.
// proceso3.txt: Un tercer proceso en ejecucion.