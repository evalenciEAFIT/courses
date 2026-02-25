#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h> // Para atar el hilo a un solo núcleo
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>

#define NUM_HILOS 3           // Cuántos hilos crear
#define CICLOS 4              // Cuántas veces alternarán entre sumar y guardar
#define TICK_MS 50            // Cada cuántos milisegundos dibuja una letra
#define TAMANO_LISTA 300 // 30 millones de elementos en la lista

// Códigos ANSI para colores en la terminal (Fondo y Texto)
#define COLOR_RESET "\x1b[0m"
#define COLOR_ROJO "\x1b[41;1;37m" // Para 'C' (CPU - Sumando)
#define COLOR_AZUL "\x1b[44;1;37m" // Para 'A' (I/O - Guardando)

const char *COLORES_TAREAS[] = {"\x1b[36m", "\x1b[33m",
                                "\x1b[35m"}; // Cyan, Amarillo, Magenta

long tiempo_inicio_ms;
long *super_lista; // El puntero a nuestra lista gigante COMPARTIDA

long long suma_global_total = 0;
pthread_mutex_t mutex_suma = PTHREAD_MUTEX_INITIALIZER;

// --- UTILIDAD: Obtener el tiempo real transcurrido ---
long obtener_ms() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

// --- UTILIDAD: Forzar concurrencia en 1 Núcleo ---
void atar_a_un_nucleo() {
  cpu_set_t mascara;
  CPU_ZERO(&mascara);
  CPU_SET(0, &mascara);
  sched_setaffinity(0, sizeof(mascara), &mascara);
}

// --- UTILIDAD: Dibuja las letras en la línea de tiempo ---
void dibujar_estado(int fila, const char *color_fondo, const char *letra) {
  long transcurrido = obtener_ms() - tiempo_inicio_ms;
  int columna = 25 + (transcurrido / TICK_MS);

  printf("\033[%d;%dH%s%s%s", fila, columna, color_fondo, letra, COLOR_RESET);
  fflush(stdout);
}

// --- LA TAREA PRINCIPAL PARA CADA HILO ---
// Nota en Hilos: Todos comparten la misma 'super_lista' al estar en el mismo
// Programa
void *ejecutar_trabajo(void *arg) {
  int id = *((int *)arg);
  free(arg);

  pid_t tid = syscall(SYS_gettid);
  const char *mi_color = COLORES_TAREAS[(id - 1) % NUM_HILOS];
  int fila = id + 10;

  printf("\033[%d;1H", fila);
  printf("%sHilo %d (TID %d):%s ", mi_color, id, tid, COLOR_RESET);
  fflush(stdout);

  // Matemáticas de división de trabajo (30 millones / 3 hilos = 10 millones
  // cada uno)
  long tamano_mi_pedazo = TAMANO_LISTA / NUM_HILOS;
  // 10 millones divididos en 4 ciclos = 2.5 millones por ciclo
  long elementos_por_ciclo = tamano_mi_pedazo / CICLOS;

  // Inicio y fin general de este hilo dentro del Vector global
  long indice_inicio = (id - 1) * tamano_mi_pedazo;

  long long suma_total = 0;

  for (int ciclo = 0; ciclo < CICLOS; ciclo++) {

    // --- 1. RÁFAGA DE CPU ('C') - PROCESAMIENTO ---
    long inicio_ciclo = indice_inicio + (ciclo * elementos_por_ciclo);
    long fin_ciclo = inicio_ciclo + elementos_por_ciclo;

    long ultimo_dibujo = obtener_ms();

    for (long i = inicio_ciclo; i < fin_ciclo; i++) {
      suma_total += super_lista[i]; // <<< TRABAJO REAL DE CPU EN MEMORIA >>>

      // Retraso artificial matemático para poder ver el efecto temporal en la
      // consola
      volatile long trampa = i * 2 / 3;
      (void)trampa;

      // Dibuja una 'C' cada 50ms (TICK_MS)
      if (obtener_ms() - ultimo_dibujo >= TICK_MS) {
        dibujar_estado(fila, COLOR_ROJO, "C");
        ultimo_dibujo = obtener_ms();
      }
    }

    // --- 2. RÁFAGA DE I/O ('A') - ENTRADA/SALIDA ---
    // Ya sumamos esta sub-porción. "Enviamos" o "Guardamos" el pedazo de
    // resultado.
    long inicio_io = obtener_ms();
    ultimo_dibujo = obtener_ms();

    // Guardar por internet o en disco toma aprox 0.4 seg de espera de hardware
    // (I/O)
    while (obtener_ms() - inicio_io < 400) {
      if (obtener_ms() - ultimo_dibujo >= TICK_MS) {
        dibujar_estado(fila, COLOR_AZUL, "A");
        ultimo_dibujo = obtener_ms();
      }
      usleep(2000); // Dormir/Ceder CPU de verdad
    }
  }

  long final = obtener_ms() - tiempo_inicio_ms;
  long indice_fin = indice_inicio + tamano_mi_pedazo - 1;
  printf("\033[%d;%dH%s [FIN: Rango %ld al %ld | Suma=%lld | Tiempo=%ld ms]%s",
         fila, 25 + (int)(final / TICK_MS) + 2, mi_color, indice_inicio,
         indice_fin, suma_total, final, COLOR_RESET);
  fflush(stdout);

  // Bloqueamos la variable compartida para evitar condiciones de carrera (Race
  // Conditions)
  pthread_mutex_lock(&mutex_suma);
  suma_global_total += suma_total;
  pthread_mutex_unlock(&mutex_suma);

  return NULL;
}

// --- ENTRADA AL PROGRAMA ---
int main() {
  atar_a_un_nucleo();

  // Memoria Compartida en R.A.M Central (El Heap)
  super_lista = malloc(TAMANO_LISTA * sizeof(long));
  for (long i = 0; i < TAMANO_LISTA; i++) {
    super_lista[i] = i; // Valores progresivos (0, 1, 2, 3...)
  }

  printf("\033[2J\033[H");
  printf("--- SUMA DE LISTA GIGANTE CON HILOS - PTHREADS (1 Núcleo) ---\n");
  printf("Lista Total: 30 Millones de Elementos (Cada hilo suma 10 Millones "
         "concurrente)\n");
  printf(" 'C' (Rojo) = Sumando matemática en CPU\n");
  printf(
      " 'A' (Azul) = I/O, Esperando al Disco Duro para guardar resultados\n\n");

  printf("Presiona ENTER para comenzar...\n");
  getchar();

  tiempo_inicio_ms = obtener_ms();

  pthread_t hilos[NUM_HILOS];

  // Arrancar Hilos Multitarea
  for (int i = 0; i < NUM_HILOS; i++) {
    int *id = malloc(sizeof(int));
    *id = i + 1;
    if (pthread_create(&hilos[i], NULL, ejecutar_trabajo, id) != 0) {
      perror("Error al crear hilo");
      return 1;
    }
  }

  // Esperar
  for (int i = 0; i < NUM_HILOS; i++) {
    pthread_join(hilos[i], NULL);
  }

  free(super_lista);

  long tiempo_total = obtener_ms() - tiempo_inicio_ms;
  long long suma_esperada = (long long)TAMANO_LISTA * (TAMANO_LISTA - 1) / 2;

  printf("\033[%d;1H\n", NUM_HILOS + 13);
  printf("\033[1;32m=== RESULTADOS FINALES ===\033[0m\n");
  printf("Configuración: %d Hilos, %d Ciclos, Lista de %d elementos\n",
         NUM_HILOS, CICLOS, TAMANO_LISTA);
  printf("Suma Global Total:         %lld\n", suma_global_total);
  printf("Fórmula de Gauss Esperada: %lld (N*(N-1)/2)\n", suma_esperada);
  printf("Tiempo Total de Ejecución: %ld ms\n", tiempo_total);
  printf("Todos los hilos han terminado de procesar su parte del Arreglo.\n");

  return 0;
}
