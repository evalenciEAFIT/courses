#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void multiply_omp(const char *num1, const char *num2, char *result) {
  int len1 = strlen(num1);
  int len2 = strlen(num2);
  int *res = (int *)calloc(len1 + len2, sizeof(int));

#pragma omp parallel for
  for (int i = 0; i < len1; i++) {
    for (int j = 0; j < len2; j++) {
      int mul = (num1[i] - '0') * (num2[j] - '0');
#pragma omp atomic
      res[i + j + 1] += mul;
    }
  }

  for (int k = len1 + len2 - 1; k > 0; k--) {
    res[k - 1] += res[k] / 10;
    res[k] %= 10;
  }

  int i = 0;
  while (i < len1 + len2 && res[i] == 0)
    i++;

  if (i == len1 + len2) {
    strcpy(result, "0");
  } else {
    int idx = 0;
    while (i < len1 + len2) {
      result[idx++] = res[i++] + '0';
    }
    result[idx] = '\0';
  }
  free(res);
}

int main() {
  char num1[2000];
  char num2[2000];
  char result[4000];
  int repeticiones = 10000;

  printf("\n=======================================================\n");
  printf(" BIGNUMBERS MULTIPLICADOR - VERSION PARALELA (OpenMP)\n");
  printf("=======================================================\n");

  printf("Ingresa tu PRIMER numero grande: ");
  scanf("%1999s", num1);
  printf("Ingresa tu SEGUNDO numero grande: ");
  scanf("%1999s", num2);
  printf("Cantidad de Iteraciones (Repetirlo X veces para cargar latencia) "
         "[Ref. 10000]: ");
  if (scanf("%d", &repeticiones) != 1)
    repeticiones = 10000;

  printf("\nIniciando cálculo O(n^2) como de escuela con OPENMP %d veces...\n",
         repeticiones);

  double start = omp_get_wtime();

  for (int k = 0; k < repeticiones; k++) {
    multiply_omp(num1, num2, result);
  }

  double end = omp_get_wtime();

  printf("\nFINALIZADO CON OPENMP MULTIHILO \n");
  printf("RESULTADO FINAL: %s\n", result);
  printf("Tiempo Transcurrido (%d reps): %f segundos.\n\n", repeticiones,
         end - start);

  return 0;
}
