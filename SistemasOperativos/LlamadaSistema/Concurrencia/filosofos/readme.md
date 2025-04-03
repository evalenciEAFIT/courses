# Problema de los 5 Filósofos y Concurrencia

El problema de los cinco filósofos es un clásico problema de concurrencia que modela la necesidad de sincronización y control de recursos compartidos en sistemas operativos y multihilos.

## Descripción del Problema
5 filósofos están sentados alrededor de una mesa, cada uno con un plato de comida. Entre cada par de filósofos hay un tenedor. Para poder comer, cada filósofo necesita tomar los dos tenedores a su izquierda y derecha. Sin embargo, solo hay cinco tenedores disponibles, lo que introduce la posibilidad de interbloqueo si todos intentan tomar los tenedores simultáneamente.
   
[![Miniatura del video](https://img.youtube.com/vi/8KFvrgbuWxY/0.jpg)](https://youtu.be/8KFvrgbuWxY?si=vWBKP32X5hjuuG-z)

## Conceptos Claves de Concurrencia
A través del código provisto, se pueden observar y estudiar tres problemas principales en concurrencia:

### 1. Interbloqueo (Deadlock)
El interbloqueo ocurre cuando todos los filósofos toman un tenedor y esperan por el otro, quedando indefinidamente bloqueados. En el código, el interbloqueo se detecta con la función:

```cpp
bool hay_deadlock() {
    for (int i = 0; i < NUM_FILOSOFOS; ++i) {
        if (!tenedor_en_uso[i]) return false;
    }
    return true;
}
```
Si todos los tenedores están en uso, el programa detecta interbloqueo y finaliza la ejecución.

### 2. Condición de Carrera (Race Condition)
Una condición de carrera sucede cuando dos o más filósofos intentan acceder al mismo recurso simultáneamente, causando comportamientos inesperados. En el código, se verifica de la siguiente manera:

```cpp
bool hay_race_condition(int id_filosofo) {
    if (tenedor_en_uso[IZQUIERDA] && tenedor_en_uso[DERECHA]) {
        return true;
    }
    return false;
}
```
Si dos filósofos intentan tomar los mismos tenedores a la vez, se registra un mensaje indicando la condición de carrera.

### 3. Sincronización y Control de Concurrencia
Para evitar estos problemas, se usan mecanismos como:
- **Mutexes (`pthread_mutex_t`)**: Se utilizan para bloquear y desbloquear tenedores, evitando acceso simultáneo.
- **Variables atómicas (`std::atomic<bool>`)**: Permiten controlar la pausa y detención de la simulación sin interferencias de hilos.

Ejemplo del uso de mutex para evitar acceso concurrente:

```cpp
pthread_mutex_lock(&tenedores[IZQUIERDA]);
tenedor_en_uso[IZQUIERDA] = true;
pthread_mutex_lock(&tenedores[DERECHA]);
tenedor_en_uso[DERECHA] = true;
```
Esto asegura que un filósofo solo puede tomar los tenedores si ambos están disponibles.

## Controles en la Simulación
La simulación incluye controles de usuario para pausar y reanudar la ejecución:

```cpp
std::atomic<bool> pausa(false);
std::atomic<bool> detener(false);
```
Estos valores se alternan con una entrada del usuario:

```cpp
getchar(); // Espera entrada del usuario
pausa = !pausa; // Alterna entre pausa y reanudación
```

## Código Completo
El siguiente código implementa la simulación del problema de los cinco filósofos:

```cpp
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

pthread_mutex_t tenedores[NUM_FILOSOFOS];
bool tenedor_en_uso[NUM_FILOSOFOS] = {false};
std::atomic<bool> pausa(false);
std::atomic<bool> detener(false);

void pensar(int id_filosofo) {
    usleep(500000 + rand() % 1000000);
    cout << "Filósofo " << id_filosofo << " está pensando." << endl;
}

void comer(int id_filosofo) {
    usleep(200000 + rand() % 500000);
    cout << "Filósofo " << id_filosofo << " está comiendo." << endl;
}

bool hay_deadlock() {
    for (int i = 0; i < NUM_FILOSOFOS; ++i) {
        if (!tenedor_en_uso[i]) return false;
    }
    return true;
}

void* filosofo(void* arg) {
    int id_filosofo = *((int*)arg);
    while (!detener) {
        if (pausa) {
            usleep(100000);
            continue;
        }

        pensar(id_filosofo);
        pthread_mutex_lock(&tenedores[IZQUIERDA]);
        tenedor_en_uso[IZQUIERDA] = true;
        pthread_mutex_lock(&tenedores[DERECHA]);
        tenedor_en_uso[DERECHA] = true;
        if (hay_deadlock()) {
            cout << "DEADLOCK DETECTADO" << endl;
            exit(1);
        }
        comer(id_filosofo);
        pthread_mutex_unlock(&tenedores[IZQUIERDA]);
        tenedor_en_uso[IZQUIERDA] = false;
        pthread_mutex_unlock(&tenedores[DERECHA]);
        tenedor_en_uso[DERECHA] = false;
    }
    return nullptr;
}

int main() {
    pthread_t filosofos[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];
    for (int i = 0; i < NUM_FILOSOFOS; ++i) {
        pthread_mutex_init(&tenedores[i], nullptr);
    }
    for (int i = 0; i < NUM_FILOSOFOS; ++i) {
        ids[i] = i;
        pthread_create(&filosofos[i], nullptr, filosofo, &ids[i]);
    }
    sleep(10);
    for (int i = 0; i < NUM_FILOSOFOS; ++i) {
        pthread_mutex_destroy(&tenedores[i]);
    }
    return 0;
}
```

## Ejemplo de Uso y Compilación
Para compilar y ejecutar el código, usa los siguientes comandos en una terminal Linux:

```sh
g++ -o filosofos filosofos.cpp -lpthread
./filosofos
```

### Salida Esperada
```
Filósofo 0 está pensando.
Filósofo 1 está pensando.
Filósofo 2 está pensando.
Filósofo 3 está pensando.
Filósofo 4 está pensando.
Filósofo 0 está comiendo.
Filósofo 1 está comiendo.
...
```



[Accede al reto inspirado en el problema de los filósofos aquí](https://github.com/evalenciEAFIT/courses/blob/main/SistemasOperativos/LlamadaSistema/Concurrencia/filosofos/reto_inspirado_filosofos.md)


Esto muestra a los filósofos alternando entre pensar y comer, mientras el programa gestiona los recursos compartidos de manera concurrente.

