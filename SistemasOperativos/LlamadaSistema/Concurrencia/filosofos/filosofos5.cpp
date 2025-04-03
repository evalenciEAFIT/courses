#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <fstream>
#include <signal.h>
#include <atomic>

#define NUM_FILOSOFOS 5
#define IZQUIERDA (id_filosofo + NUM_FILOSOFOS - 1) % NUM_FILOSOFOS
#define DERECHA (id_filosofo + 1) % NUM_FILOSOFOS

using namespace std;

// Colores ANSI para la terminal
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

// Colores de fondo ANSI
#define BG_RED "\033[48;5;196m"    // Rojo intenso (para "Ocupado")
#define BG_GREEN "\033[48;5;46m"  // Verde intenso (para "Libre")
#define BG_YELLOW "\033[48;5;226m" // Amarillo intenso
#define BG_BLUE "\033[48;5;21m"   // Azul intenso
#define BG_MAGENTA "\033[48;5;201m" // Magenta intenso

// Mutexes para los tenedores (representan los recursos compartidos)
pthread_mutex_t tenedores[NUM_FILOSOFOS];

// Estado de los tenedores (true = ocupado, false = libre)
bool tenedor_en_uso[NUM_FILOSOFOS] = {false};

// Colores únicos para cada filósofo
const char* colores[NUM_FILOSOFOS] = {RED, GREEN, YELLOW, BLUE, MAGENTA};
const char* bg_colores[NUM_FILOSOFOS] = {BG_RED, BG_GREEN, BG_YELLOW, BG_BLUE, BG_MAGENTA};

// Variables globales para controlar la simulación
std::atomic<bool> pausa(false); // Controla si la simulación está en pausa
std::atomic<bool> detener(false); // Controla si la simulación debe detenerse
std::atomic<bool> reiniciar(false); // Controla si se debe reiniciar la simulación

// Archivo de registro para registrar eventos importantes
ofstream log_file;

// Función para inicializar el archivo de registro
void inicializar_log() {
    log_file.open("simulacion.log"); // Abre el archivo de registro
    log_file << "Inicio de la simulación\n"; // Registra el inicio de la simulación
}

// Función para registrar eventos en el archivo de registro
void registrar_evento(const string& evento) {
    log_file << evento << endl; // Escribe el evento en el archivo de registro
}

// Función para cerrar el archivo de registro
void cerrar_log() {
    log_file << "Fin de la simulación\n"; // Registra el fin de la simulación
    log_file.close(); // Cierra el archivo de registro
}

// Función para mostrar el estado de los tenedores con colores de fondo
void mostrar_estado_tenedores() {
    cout << RESET << "Estado de los tenedores: ";
    for (int i = 0; i < NUM_FILOSOFOS; ++i) {
        if (tenedor_en_uso[i]) {
            // Muestra "Ocupado" en rojo de fondo si el tenedor está en uso
            cout << BG_RED << "[T" << i << ":Ocupado] " << RESET;
        } else {
            // Muestra "Libre" en verde de fondo si el tenedor está disponible
            cout << BG_GREEN << "[T" << i << ":Libre] " << RESET;
        }
    }
    cout << RESET << endl; // Restablece el color al final
}

// Función para simular que un filósofo está pensando
void pensar(int id_filosofo) {
    usleep(500000 + rand() % 1000000); // Simula tiempo de pensamiento (entre 0.5 y 1.5 segundos)
    cout << bg_colores[id_filosofo] << colores[id_filosofo]
         << " Filósofo " << id_filosofo << " está PENSANDO " << RESET << endl;
    registrar_evento("Filósofo " + to_string(id_filosofo) + " está pensando."); // Registra el evento
}

// Función para simular que un filósofo está comiendo
void comer(int id_filosofo) {
    usleep(200000 + rand() % 500000); // Simula tiempo de comida (entre 0.2 y 0.7 segundos)
    cout << colores[id_filosofo] << "Filósofo " << id_filosofo << " está comiendo." << RESET << endl;
    registrar_evento("Filósofo " + to_string(id_filosofo) + " está comiendo."); // Registra el evento
}

// Función que ejecuta cada filósofo (lógicas concurrentes)
void* filosofo(void* arg) {
    int id_filosofo = *((int*)arg); // Obtiene el ID del filósofo

    while (!detener) { // Bucle infinito mientras la simulación no esté detenida
        if (pausa || reiniciar) {
            usleep(100000); // Espera si la simulación está en pausa o reiniciando
            continue; // Salta al siguiente ciclo
        }

        pensar(id_filosofo); // El filósofo piensa antes de intentar comer

        // Intentar tomar los tenedores
        cout << RESET << "Filósofo " << id_filosofo << " intentando tomar tenedores..." << RESET << endl;

        pthread_mutex_lock(&tenedores[IZQUIERDA]); // Bloquea el tenedor izquierdo
        tenedor_en_uso[IZQUIERDA] = true; // Marca el tenedor izquierdo como ocupado
        pthread_mutex_lock(&tenedores[DERECHA]); // Bloquea el tenedor derecho
        tenedor_en_uso[DERECHA] = true; // Marca el tenedor derecho como ocupado

        mostrar_estado_tenedores(); // Muestra el estado actual de los tenedores

        comer(id_filosofo); // El filósofo come después de tomar ambos tenedores

        // Liberar los tenedores
        pthread_mutex_unlock(&tenedores[IZQUIERDA]); // Libera el tenedor izquierdo
        tenedor_en_uso[IZQUIERDA] = false; // Marca el tenedor izquierdo como libre
        pthread_mutex_unlock(&tenedores[DERECHA]); // Libera el tenedor derecho
        tenedor_en_uso[DERECHA] = false; // Marca el tenedor derecho como libre

        mostrar_estado_tenedores(); // Muestra el estado actual de los tenedores
    }

    return nullptr; // Finaliza el hilo
}

// Hilo para manejar la interacción del usuario (control de pausa/detención/reinicio)
void* manejar_entrada_usuario(void*) {
    cout << RESET << "Presiona Ctrl+C para detener la simulación." << RESET << endl;
    cout << RESET << "Envía SIGUSR1 (kill -SIGUSR1 <PID>) para pausar/reanudar." << RESET << endl;
    cout << RESET << "Presiona 'x' para que todos los filósofos suelten los tenedores y reinicien." << RESET << endl;

    while (!detener) {
        char input = getchar(); // Lee la entrada del usuario

        if (input == 'x') {
            reiniciar = true; // Activa el modo de reinicio
            cout << RED << "Reiniciando simulación: Todos los filósofos sueltan los tenedores..." << RESET << endl;

            // Forzar la liberación de todos los tenedores
            for (int i = 0; i < NUM_FILOSOFOS; ++i) {
                pthread_mutex_unlock(&tenedores[i]); // Libera el mutex
                tenedor_en_uso[i] = false; // Marca el tenedor como libre
            }
            mostrar_estado_tenedores(); // Muestra el estado actual de los tenedores

            sleep(2); // Espera 2 segundos antes de reiniciar
            reiniciar = false; // Desactiva el modo de reinicio
            cout << GREEN << "Simulación reiniciada." << RESET << endl;
        } else if (input == '\n') {
            pausa = !pausa; // Alterna entre pausa y reanudación
            cout << RESET << "Simulación " << (pausa ? "PAUSADA" : "REANUDADA") << RESET << endl;
        }
    }

    return nullptr; // Finaliza el hilo
}

// Función principal
int main() {
    inicializar_log(); // Inicializa el archivo de registro

    pthread_t filosofos[NUM_FILOSOFOS]; // Arreglo de hilos para los filósofos
    int ids[NUM_FILOSOFOS]; // IDs de los filósofos

    // Inicializa los mutexes para los tenedores
    for (int i = 0; i < NUM_FILOSOFOS; ++i) {
        pthread_mutex_init(&tenedores[i], nullptr); // Inicializa el mutex para cada tenedor
    }

    // Crea los hilos para los filósofos
    for (int i = 0; i < NUM_FILOSOFOS; ++i) {
        ids[i] = i; // Asigna el ID del filósofo
        pthread_create(&filosofos[i], nullptr, filosofo, &ids[i]); // Crea el hilo para el filósofo
    }

    // Crea un hilo para manejar la entrada del usuario
    pthread_t hilo_entrada;
    pthread_create(&hilo_entrada, nullptr, manejar_entrada_usuario, nullptr);

    // Espera a que termine la simulación
    while (!detener) {
        sleep(1); // Espera 1 segundo antes de verificar nuevamente
    }

    // Destruye los mutexes
    for (int i = 0; i < NUM_FILOSOFOS; ++i) {
        pthread_mutex_destroy(&tenedores[i]); // Libera los recursos de los mutexes
    }

    cerrar_log(); // Cierra el archivo de registro

    return 0; // Finaliza el programa
}