#define _GNU_SOURCE
#include <sched.h> // Para atar el proceso a un solo núcleo
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_PROCESOS 3 // Cuántos procesos crear
#define CICLOS 4       // Cuántas veces alternarán entre sumar y guardar
#define TICK_MS 50     // Cada cuántos milisegundos dibuja una letra en pantalla
#define TAMANO_LISTA 30000000 // 30 millones de elementos en la lista

// Códigos ANSI para colores visuales
#define COLOR_RESET "\x1b[0m"
#define COLOR_ROJO "\x1b[41;1;37m" // Para 'C' (CPU - Sumando)
#define COLOR_AZUL "\x1b[44;1;37m" // Para 'A' (I/O - Guardando)

const char *COLORES_TAREAS[] = {"\x1b[32m", "\x1b[33m",
                                "\x1b[35m"}; // Verde, Amarillo, Magenta

long tiempo_inicio_ms;
long *super_lista;            // El puntero a nuestra lista gigante
long long *suma_global_total; // Suma global compartida en RAM

// --- UTILIDAD: Obtener el tiempo actual en milisegundos ---
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

// --- LA TAREA PRINCIPAL PARA CADA PROCESO ---
// Nota en Procesos: Al usar fork(), cada hijo recibe una COPIA de 'super_lista'
void ejecutar_trabajo(int id) {
  pid_t pid = getpid();
  const char *mi_color = COLORES_TAREAS[(id - 1) % NUM_PROCESOS];
  int fila = id + 10;

  printf("\033[%d;1H", fila);
  printf("%sProceso %d (PID %d):%s ", mi_color, id, pid, COLOR_RESET);
  fflush(stdout);

  // Matemáticas de división de trabajo
  // Si la lista tiene 30 millones y hay 3 procesos, cada uno procesa 10
  // millones
  long tamano_mi_pedazo = TAMANO_LISTA / NUM_PROCESOS;
  // Dividimos esos 10 millones en 4 ciclos (2.5 millones por ciclo)
  long elementos_por_ciclo = tamano_mi_pedazo / CICLOS;

  // Inicio y fin general de este proceso
  long indice_inicio = (id - 1) * tamano_mi_pedazo;

  long long suma_total = 0;

  for (int ciclo = 0; ciclo < CICLOS; ciclo++) {

    // --- 1. RÁFAGA DE CPU ('C') - PROCESAMIENTO ---
    // Vamos a sumar matemáticamente nuestra porción
    long inicio_ciclo = indice_inicio + (ciclo * elementos_por_ciclo);
    long fin_ciclo = inicio_ciclo + elementos_por_ciclo;

    long ultimo_dibujo = obtener_ms();

    for (long i = inicio_ciclo; i < fin_ciclo; i++) {
      suma_total += super_lista[i]; // <<< AQUÍ ESTÁ EL TRABAJO REAL DE CPU >>>

      // Para que este cálculo tan rápido tarde un poco y lo podamos ver en la
      // terminal, forzamos una operación inútil adicional por cada número
      volatile long trampa = i * 2 / 3;
      (void)trampa;

      // Dibujamos una 'C' en la pantalla cada 50ms (TICK_MS)
      if (obtener_ms() - ultimo_dibujo >= TICK_MS) {
        dibujar_estado(fila, COLOR_ROJO, "C");
        ultimo_dibujo = obtener_ms();
      }
    }

    // --- 2. RÁFAGA DE I/O ('A') - ENTRADA/SALIDA ---
    // Ya sumamos esta porción. Ahora simulamos que guardamos el resultado
    // parcial en un archivo de Texto o base de datos. Esto es I/O, el proceso
    // "Duerme".
    long inicio_io = obtener_ms();
    ultimo_dibujo = obtener_ms();

    // Simulamos que guardar en disco tarda 400 milisegundos (0.4 seg)
    while (obtener_ms() - inicio_io < 400) {
      if (obtener_ms() - ultimo_dibujo >= TICK_MS) {
        dibujar_estado(fila, COLOR_AZUL, "A");
        ultimo_dibujo = obtener_ms();
      }
      usleep(2000); // Darle un respiro leve al procesador (Dormir de verdad)
    }
  }

  // Dibujar fin e imprimir la suma total real en texto blanco
  long final = obtener_ms() - tiempo_inicio_ms;
  long indice_fin = indice_inicio + tamano_mi_pedazo - 1;
  printf("\033[%d;%dH%s [FIN: Rango %ld al %ld | Suma=%lld | Tiempo=%ld ms]%s",
         fila, 25 + (int)(final / TICK_MS) + 2, mi_color, indice_inicio,
         indice_fin, suma_total, final, COLOR_RESET);
  fflush(stdout);

  __sync_fetch_and_add(
      suma_global_total,
      suma_total); // Sumar atómicamente a la variable global compartida

  exit(0);
}

// --- ENTRADA AL PROGRAMA ---
int main() {
  atar_a_un_nucleo();

  // Generar la lista gigante de números
  super_lista = malloc(TAMANO_LISTA * sizeof(long));
  for (long i = 0; i < TAMANO_LISTA; i++) {
    super_lista[i] = i; // Valores progresivos (0, 1, 2, 3...)
  }

  printf("\033[2J\033[H");
  printf("--- SUMA DE LISTA GIGANTE CON PROCESOS (1 Núcleo) ---\n");
  printf("Lista Total: 30 Millones de Elementos (Cada proceso suma 10 "
         "Millones)\n");
  printf(" 'C' (Rojo) = Sumando elementos en CPU\n");
  printf(" 'A' (Azul) = Guardando resultados parciales en Disco (I/O)\n\n");

  printf("Presiona ENTER para comenzar a procesar...\n");
  getchar();

  tiempo_inicio_ms = obtener_ms();

  // Crear variable compartida entre procesos a través del Kernel (mmap)
  suma_global_total = mmap(NULL, sizeof(long long), PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *suma_global_total = 0;

  // Crear hijos
  for (int i = 0; i < NUM_PROCESOS; i++) {
    pid_t pid = fork();
    if (pid == 0) {
      ejecutar_trabajo(i + 1);
    }
  }

  // Esperar a los hijos
  for (int i = 0; i < NUM_PROCESOS; i++) {
    wait(NULL);
  }

  free(super_lista);

  long tiempo_total = obtener_ms() - tiempo_inicio_ms;
  long long suma_esperada = (long long)TAMANO_LISTA * (TAMANO_LISTA - 1) / 2;

  printf("\033[%d;1H\n", NUM_PROCESOS + 13);
  printf("\033[1;32m=== RESULTADOS FINALES ===\033[0m\n");
  printf("Configuración: %d Procesos, %d Ciclos, Lista de %d elementos\n",
         NUM_PROCESOS, CICLOS, TAMANO_LISTA);
  printf("Suma Global Total:      %lld\n", *suma_global_total);
  printf("Fórmula de Gauss Esperada: %lld (N*(N-1)/2)\n", suma_esperada);
  printf("Tiempo Total de Ejecución: %ld ms\n", tiempo_total);
  printf("El procesamiento ha finalizado.\n");

  munmap(suma_global_total, sizeof(long long));
  return 0;
}
