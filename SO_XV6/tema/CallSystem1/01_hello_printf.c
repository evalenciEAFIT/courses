#include <stdio.h>

int main() {
  // La función printf pertenece a la biblioteca estándar de C (glibc)
  // Tras bambalinas, eventualmente hace una llamada al sistema (syscall)
  // para escribir en la pantalla.
  printf("Hola Mundo usando la función estándar printf!\n");

  return 0;
}
