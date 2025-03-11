#include <iostream>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <unistd.h>

using namespace std;
using namespace std::chrono;

// Tamaño de página típico en sistemas Linux: 4 KB
const int PAGE_SIZE = 4096;  // 4 KB en bytes
const int NUM_PAGES = 1024;  // Número de páginas para recorrer

// Función para medir el tiempo de acceso a la memoria
void measure_access_time() {
    // Reserva suficiente memoria para múltiples páginas
    vector<int> buffer(NUM_PAGES * PAGE_SIZE / sizeof(int), 0);

    // **Acceso secuencial dentro de una misma página** -> TLB hit
    auto start_hit = high_resolution_clock::now();
    for (int i = 0; i < NUM_PAGES * PAGE_SIZE / sizeof(int); i++) {
        buffer[i]++;
    }
    auto end_hit = high_resolution_clock::now();
    auto duration_hit = duration_cast<nanoseconds>(end_hit - start_hit).count();

    cout << "Tiempo de acceso (TLB Hit): " << duration_hit / NUM_PAGES << " ns por acceso\n";

    // **Acceso cruzando páginas** -> TLB miss
    auto start_miss = high_resolution_clock::now();
    for (int i = 0; i < NUM_PAGES; i++) {
        buffer[(i * PAGE_SIZE) / sizeof(int)]++;
    }
    auto end_miss = high_resolution_clock::now();
    auto duration_miss = duration_cast<nanoseconds>(end_miss - start_miss).count();

    cout << "Tiempo de acceso (TLB Miss): " << duration_miss / NUM_PAGES << " ns por acceso\n";
}

int main() {
    cout << "Demostración del efecto de TLB en el rendimiento de memoria\n";
    measure_access_time();
    return 0;
}
