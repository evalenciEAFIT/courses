#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  int N;
  printf("\n==============================================\n");
  printf(" MATRICES - VERSION PARALELA (OpenMP Threaded)\n");
  printf("==============================================\n");
  printf("Ingrese el tamaño de la matriz cuadrada (N): ");
  if (scanf("%d", &N) != 1 || N <= 0) {
    printf("Tamano invalido.\n");
    return 1;
  }

  double *A = (double *)malloc(N * N * sizeof(double));
  double *B = (double *)malloc(N * N * sizeof(double));
  double *C = (double *)calloc(N * N, sizeof(double));

  if (!A || !B || !C) {
    printf("Error asignando memoria (Posible N demasiado grande)\n");
    return 1;
  }

  int i, j, k;

// Inicialización Multihilo
#pragma omp parallel for private(i) shared(A, B)
  for (i = 0; i < N * N; i++) {
    A[i] = (double)rand() / RAND_MAX;
    B[i] = (double)rand() / RAND_MAX;
  }

  printf("Iniciando multiplicación paralela (OpenMP) de matrices de %dx%d...\n",
         N, N);

  double inicio = omp_get_wtime();

// Trabajo pesado en OpenMP
#pragma omp parallel for private(i, j, k) shared(A, B, C)
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      for (k = 0; k < N; k++) {
        C[i * N + j] += A[i * N + k] * B[k * N + j];
      }
    }
  }

  double fin = omp_get_wtime();
  double tiempo_transcurrido = fin - inicio;

  printf("Paralelización completada en %f segundos usando %d hilos "
         "lógicos.\n\n",
         tiempo_transcurrido, omp_get_max_threads());

  free(A);
  free(B);
  free(C);
  return 0;
}
