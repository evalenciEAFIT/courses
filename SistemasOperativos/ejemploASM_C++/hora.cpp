#include <iostream>
#include <ctime>
#include <iomanip>

int main() {
    // Obtener el tiempo actual en segundos desde el epoch
    std::time_t tiempoActual = std::time(nullptr);

    // Ajustar el tiempo a UTC-11
    int timezone_offset = -5 * 3600; // UTC-11 en segundos
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
