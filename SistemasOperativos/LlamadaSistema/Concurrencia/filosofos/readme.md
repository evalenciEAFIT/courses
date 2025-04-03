# Problema de los Cinco Filósofos y Concurrencia

El problema de los cinco filósofos es un clásico problema de concurrencia que modela la necesidad de sincronización y control de recursos compartidos en sistemas operativos y multihilos.

## Descripción del Problema
Cinco filósofos están sentados alrededor de una mesa, cada uno con un plato de comida. Entre cada par de filósofos hay un tenedor. Para poder comer, cada filósofo necesita tomar los dos tenedores a su izquierda y derecha. Sin embargo, solo hay cinco tenedores disponibles, lo que introduce la posibilidad de interbloqueo si todos intentan tomar los tenedores simultáneamente.

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

## Conclusión
El problema de los cinco filósofos ilustra los retos de la sincronización de hilos y acceso a recursos compartidos en concurrencia. Mediante mutexes y mecanismos de detección de interbloqueo y condiciones de carrera, el código ofrece una representación práctica de estos conceptos fundamentales en sistemas operativos y programación concurrente.

