#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  int N;
  printf("\n==============================================\n");
  printf(" MATRICES - VERSION SECUENCIAL (1 Núcleo)\n");
  printf("==============================================\n");
  printf("Ingrese el tamaño de la matriz cuadrada (N): ");
  if (scanf("%d", &N) != 1 || N <= 0) {
    printf("Tamano invalido.\n");
    return 1;
  }

  // Ubicacion de memoria Dinámica 1D para arrays N x N (Mejor cache tracking)
  double *A = (double *)malloc(N * N * sizeof(double));
  double *B = (double *)malloc(N * N * sizeof(double));
  double *C = (double *)calloc(N * N, sizeof(double));

  if (!A || !B || !C) {
    printf("Error asignando en el bloque RAM (Posible N demasiado grande)\n");
    return 1;
  }

  int i, j, k;

  // Configurar semilla aleatoria
  srand(time(NULL));

  // Inicialización aleatoria
  for (i = 0; i < N * N; i++) {
    A[i] = (double)rand() / RAND_MAX;
    B[i] = (double)rand() / RAND_MAX;
  }

  printf("Iniciando multiplicación secuencial de matrices de %dx%d...\n", N, N);

  clock_t inicio = clock();

  // Multiplicación de matrices clásica O(n^3) - Achatada en arreglos 1D C[i][j]
  // -> C[i * N + j]
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      for (k = 0; k < N; k++) {
        C[i * N + j] += A[i * N + k] * B[k * N + j];
      }
    }
  }

  clock_t fin = clock();
  double tiempo_transcurrido = (double)(fin - inicio) / CLOCKS_PER_SEC;

  printf("[✔️] Multiplicación secuencial completada en %f segundos.\n\n",
         tiempo_transcurrido);

  free(A);
  free(B);
  free(C);
  return 0;
}
