#include <iostream>
#include <vector>
#include <chrono>
#include <sys/resource.h>
#include <malloc.h>
#include <fstream>
#include <unistd.h>

using namespace std;
using namespace std::chrono;

// Definimos colores ANSI para mejorar visualización
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

// Variables globales (memoria estática)
int variableGlobal = 42;
static int variableEstatica = 99;

// Estructura para almacenar resultados de multiplicación
struct ResultadoMultiplicacion {
    long long operaciones;
    long tiempo_ms;
};

// Obtener uso de Swap desde `/proc/self/status`
long obtenerUsoSwap() {
    ifstream status("/proc/self/status");
    string linea;
    while (getline(status, linea)) {
        if (linea.find("VmSwap:") != string::npos) {
            long swap;
            sscanf(linea.c_str(), "VmSwap: %ld kB", &swap);
            return swap;
        }
    }
    return 0;
}

// Mostrar el uso de memoria
void medirUsoMemoria(const string& tipo) {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    struct mallinfo2 mi = mallinfo2();

    cout << BOLD << CYAN << "\n\n🔄 MEDICIÓN DE MEMORIA (" << tipo << ") 🔄\n" << RESET;
    cout << GREEN << " - RSS (Resident Set Size): " << RESET << usage.ru_maxrss << " KB → Memoria real usada por el proceso.\n";
    cout << YELLOW << " - Heap Arena: " << RESET << mi.arena / 1024 << " KB → Memoria reservada para el heap.\n";
    cout << YELLOW << " - Heap Usado: " << RESET << mi.uordblks / 1024 << " KB → Memoria actualmente ocupada en el heap.\n";
    cout << RED << " - Stack Usado: " << RESET << usage.ru_isrss << " KB → Memoria usada en la pila.\n";
    cout << BLUE << " - Swap Usado: " << RESET << obtenerUsoSwap() << " KB\n";

    // Mostrar direcciones de memoria
    int variableLocal = 1;
    int* variableHeap = new int(10);
    cout << BOLD << "\n⚙️  Direcciones de Memoria:\n" << RESET;
    cout << " - Global: " << &variableGlobal <<std::endl << " - Estática: " << &variableEstatica << "\n";
    cout << " - Stack: " << &variableLocal << std::endl << " - Heap: " << variableHeap << "\n";
    delete variableHeap;
}

// Mostrar barra de progreso
void mostrarBarraProgreso(long long actual, long long total) {
    int ancho_barra = 50;
    int progreso = (actual * ancho_barra) / total;

    cout << "\r[";
    for (int i = 0; i < progreso; i++) cout << GREEN << "#" << RESET;
    for (int i = progreso; i < ancho_barra; i++) cout << "-";
    cout << "] " << (actual * 100) / total << "% (" << actual << "/" << total << ")";
    cout.flush();
}

// Multiplicación de matrices con `float`
ResultadoMultiplicacion multiplicarMatrizFloat(int n, int m) {
    vector<vector<float>> A(n, vector<float>(m, 1.5f));
    vector<vector<float>> B(m, vector<float>(n, 2.5f));
    vector<vector<float>> resultado(n, vector<float>(m, 0.0f));

    long long total_ops = (long long)n * m * m, ops_realizadas = 0;
    auto inicio = high_resolution_clock::now();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < m; k++) {
                resultado[i][j] += A[i][k] * B[k][j];
                ops_realizadas++;
                if (ops_realizadas % (total_ops / 100) == 0) mostrarBarraProgreso(ops_realizadas, total_ops);
            }
        }
    }
    auto fin = high_resolution_clock::now();
    cout << endl;

    return {ops_realizadas, duration_cast<milliseconds>(fin - inicio).count()};
}

// Multiplicación de matrices con `double`
ResultadoMultiplicacion multiplicarMatrizDouble(int n, int m) {
    vector<vector<double>> A(n, vector<double>(m, 1.5));
    vector<vector<double>> B(m, vector<double>(n, 2.5));
    vector<vector<double>> resultado(n, vector<double>(m, 0.0));

    long long total_ops = (long long)n * m * m, ops_realizadas = 0;
    auto inicio = high_resolution_clock::now();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < m; k++) {
                resultado[i][j] += A[i][k] * B[k][j];
                ops_realizadas++;
                if (ops_realizadas % (total_ops / 100) == 0) mostrarBarraProgreso(ops_realizadas, total_ops);
            }
        }
    }
    auto fin = high_resolution_clock::now();
    cout << endl;

    return {ops_realizadas, duration_cast<milliseconds>(fin - inicio).count()};
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << RED << "Uso: " << argv[0] << " <filas> <columnas>\n" << RESET;
        return 1;
    }

    int n = stoi(argv[1]);
    int m = stoi(argv[2]);

    if (n <= 0 || m <= 0) {
        cerr << RED << "Las dimensiones de la matriz deben ser mayores que 0.\n" << RESET;
        return 1;
    }

    cout << BOLD << BLUE << "🔄 PROCESO DE MULTIPLICACIÓN DE MATRICES & GESTIÓN DE MEMORIA\n" << RESET;
    medirUsoMemoria("Inicio del programa");

    // Multiplicación con float
    cout << BOLD << CYAN << "\n📊 Multiplicando matrices con FLOAT (32 bits, Precisión simple):\n" << RESET;
    ResultadoMultiplicacion resultado_float = multiplicarMatrizFloat(n, m);
    medirUsoMemoria("Después de multiplicación FLOAT");

    // Multiplicación con double
    cout << BOLD << CYAN << "\n📊 Multiplicando matrices con DOUBLE (64 bits, Precisión doble):\n" << RESET;
    ResultadoMultiplicacion resultado_double = multiplicarMatrizDouble(n, m);
    medirUsoMemoria("Después de multiplicación DOUBLE");

    // Resumen final
    cout << BOLD << CYAN << "\n\n\n📊 RESUMEN FINAL 📊\n" << RESET;
    cout << GREEN << " - Tiempo multiplicación FLOAT: " << RESET << resultado_float.tiempo_ms << " ms\n";
    cout << YELLOW << "     Operaciones realizadas (FLOAT): " << RESET << resultado_float.operaciones << "\n";
    cout << GREEN << " - Tiempo multiplicación DOUBLE: " << RESET << resultado_double.tiempo_ms << " ms\n";
    cout << YELLOW << "     Operaciones realizadas (DOUBLE): " << RESET << resultado_double.operaciones << "\n";

    return 0;
}
