#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void multiply_seq(const char *num1, const char *num2, char *result) {
  int len1 = strlen(num1);
  int len2 = strlen(num2);
  int *res = (int *)calloc(len1 + len2, sizeof(int));

  for (int i = len1 - 1; i >= 0; i--) {
    for (int j = len2 - 1; j >= 0; j--) {
      int mul = (num1[i] - '0') * (num2[j] - '0');
      int sum = mul + res[i + j + 1];
      res[i + j + 1] = sum % 10;
      res[i + j] += sum / 10;
    }
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
  printf(" BIGNUMBERS MULTIPLICADOR - VERSION SECUENCIAL (Lineal)\n");
  printf("=======================================================\n");

  printf("Ingresa tu PRIMER numero grande: ");
  scanf("%1999s", num1);
  printf("Ingresa tu SEGUNDO numero grande: ");
  scanf("%1999s", num2);
  printf("Cantidad de Iteraciones (Repetirlo X veces para cargar latencia) "
         "[Ref. 10000]: ");
  if (scanf("%d", &repeticiones) != 1)
    repeticiones = 10000;

  printf("\nIniciando cálculo O(n^2) como de escuela SECUENCIAL %d veces...\n",
         repeticiones);

  clock_t start = clock();

  for (int k = 0; k < repeticiones; k++) {
    multiply_seq(num1, num2, result);
  }

  clock_t end = clock();

  printf("\n[✔️] FINALIZADO SECUENCIAL\n");
  printf("RESULTADO FINAL: %s\n", result);
  printf("Tiempo Transcurrido (%d reps): %f segundos\n", repeticiones,
         (double)(end - start) / CLOCKS_PER_SEC);

  return 0;
}
