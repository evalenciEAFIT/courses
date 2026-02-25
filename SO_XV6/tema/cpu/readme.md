# Proyecto de Concurrencia: Monitoreo de Procesos vs Hilos

Este proyecto, desarrollado en C usando entornos Linux, está diseñado para entender **de forma visual y simple** la diferencia entre usar **Procesos** (vía `fork()`) e **Hilos** (vía `pthreads`). Además, explica y demuestra los cambios de estado en el programador (Process Scheduler) cuando un proceso/hilo realiza operaciones de CPU o se bloquea por I/O.

## ¿Qué hace cada programa?

1. **procesos.c**: Crea tres subprocesos independientes.
2. **hilos.c**: Crea tres hilos (Threads / LWP) que coexisten dentro del mismo espacio de direcciones y memoria que su programa base.

Ambos programas resuelven un problema colaborativo (Sumar un vector de 30 Millones de elementos):

* Ráfaga de CPU (Suma Matemática): Aparecerá una **C** roja en la gráfica visual (Equivale al estado **R** o Running).
* Ráfaga de I/O (Guardar en Disco/Dormir): Aparecerá una **A** azul en la gráfica visual (Equivale al estado **S** o Sleep).

## Instrucciones

### 1. Compilar el código

Ejecuta el comando en tu terminal:

```bash
make
```

### 2. Hacer ejecutable el script de monitoreo

```bash
chmod +x monitorear.sh
```

### 3. ¡Hora del Show! (Necesitarás 2 terminales)

Para entender verdaderamente esto, debes ejecutar el programa en una terminal y vigilarlo (monitorearlo) en otra.

#### Experimento 1: PROCESOS

1. **Terminal 1:** Ejecuta el programa de procesos.

   ```bash
   ./procesos
   ```

   *Anota el `PID Principal` que aparecerá en pantalla al iniciar.*
2. **Terminal 2:** Usa nuestro script para vigilar los procesos hijos (-p).

   ```bash
   ./monitorear.sh -p <EL_PID_PRINCIPAL>
   ```

   **(Observar detenidamente la columna STAT en la otra consola y cómo cambia de 'R' (Instrucción C) a 'S' (Instrucción A))**.
   *Presiona Ctrl+C en esta terminal para salir cuando terminen.*

#### Experimento 2: HILOS

1. **Terminal 1:** Ejecuta el programa de hilos.

   ```bash
   ./hilos
   ```

   *Anota el `PID Principal`.*
2. **Terminal 2:** Usa nuestro script para vigilar los hilos internos (threads/LWPs).

   ```bash
   ./monitorear.sh -t <EL_PID_PRINCIPAL>
   ```

   **(Notarás que todos tienen el mismo PID global pero distinto TID (Thread ID))**.

---

## Resumen Pedagógico:

* **PPID (Parent Process ID):** Quién creó este proceso. Los hijos tienen el PID del `Proceso Principal`. Los hilos simplemente pertenecen todos al mismo proceso (por eso todos tienen el mismo PID pero diferente TID).
* **Instrucción C (R) vs Instrucción A (S):** Muestra físicamente el cambio de estado en el planificador del Sistema Operativo. El sistema sabe en qué momento el programa usa el procesador (*C*) y en qué momento espera al disco (*A*), por lo que le arrebata el núcleo y se lo da a alguien más. No desperdiciamos procesador en sleep().
* **Aislamiento (PROCESOS) vs Compartido (HILOS):** Los Procesos requirieron crear "Memoria Compartida" especial (`mmap`) para poder acumular el resultado. Por el contrario, los Hilos del Experimento 2 comparten la misma RAM de fábrica, uniendo sus matemáticas (`suma_global_total`) de forma nativa.

---

## ¿Cómo se codifican Procesos e Hilos en C?

### 1. Creación de Procesos (Fork)

El enfoque principal para la creación de multiprocesos en Linux/Unix se basa en la llamada al sistema `fork()`.

* Requiere incluir las librerías `<unistd.h>` (para la gestión normal de POSIX) y `<sys/wait.h>` (para esperar al proceso).
* Al hacer `fork()`, el hilo de ejecución original *"se divide en dos"*.

```c
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
    pid_t pid = fork(); // ¡Aquí nace el clon!

    if (pid == 0) {
        // --- Espacio del HIJO ---
        // Este bloque solo lo ejecuta el nuevo proceso clonado.
        printf("Soy el Proceso Hijo.\n");
        // exit(0); // Es buena práctica matar aquí al hijo para que no siga leyendo el código viejo
    } else if (pid > 0) {
        // --- Espacio del PADRE ---
        // 'pid' guarda aquí el número de identificador del Hijo creado
        printf("Soy el Proceso Padre. Creé a mi hijo.\n");
        wait(NULL);  // Detiene al padre hasta que el hijo muera (evitando procesos 'Zombies')
    } else {
        printf("Ocurrió un error al hacer fork().\n");
    }
  
    return 0;
}
```

### 2. Creación de Hilos (Pthreads)

Para sistemas POSIX, la librería oficial es `pthread` (POSIX Threads).

* Requiere incluir `<pthread.h>`.
* Requiere una **función** separada donde vivirá ese hilo, que debe obligatoriamente retornar un puntero nulo (`void *`) y recibir un puntero nulo para argumentos (`void *arg`).
* Al compilar, **siempre debes usar la bandera `-pthread`** (ej. `gcc mi_codigo.c -pthread`).

```c
#include <pthread.h>
#include <stdio.h>

// El "Trabajo" que ejecutará el Hilo paralelamente.
void *mi_tarea_paralela(void *argumento) {
    printf("¡Hola desde el Hilo paralelo!\n");
    return NULL; // El hilo termina cuando retorna
}

int main() {
    pthread_t mi_hilo; // Variable que guarda el identificador lógico del hilo

    // Se inicializa el hilo; desde este instante, compite con el main() por procesador.
    if (pthread_create(&mi_hilo, NULL, mi_tarea_paralela, NULL) != 0) {
        printf("Error creando el hilo.\n");
        return 1;
    }

    printf("Acabo de iniciar el hilo desde el Main principal.\n");

    // "Join" es el equivalente a "wait()". Obliga al Main a aguardar hasta que el hilo paralelo termine.
    pthread_join(mi_hilo, NULL);

    return 0;
}
```
