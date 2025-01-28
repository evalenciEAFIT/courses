#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>
#include <ctime>
#include <iomanip>

int main() {
    // Obtener el tiempo actual usando una llamada al sistema
    time_t tiempoActual;
    syscall(SYS_time, &tiempoActual);

    // Ajustar el tiempo a UTC-5
    int timezone_offset = -5 * 3600; // UTC-5 en segundos
    tiempoActual += timezone_offset;

    // Convertir el tiempo ajustado a estructura de tiempo local
    std::tm* horaLocal = std::gmtime(&tiempoActual);

    // Mostrar la hora en formato HH:MM:SS
    if (horaLocal != nullptr) {
        std::cout << "Hora actual: "
                  << std::setw(2) << std::setfill('0') << horaLocal->tm_hour << ":"
                  << std::setw(2) << std::setfill('0') << horaLocal->tm_min << ":"
                  << std::setw(2) << std::setfill('0') << horaLocal->tm_sec
                  << std::endl;
    } else {
        std::cerr << "Error al obtener la hora local." << std::endl;
        return 1;
    }

    return 0;
}
