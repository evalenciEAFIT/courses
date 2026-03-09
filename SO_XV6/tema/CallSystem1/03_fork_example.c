#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t pid;

  // Imprimir el PID del proceso original antes de llamar a fork
  // getpid() es una llamada al sistema que devuelve el Process ID actual.
  printf("Proceso original. PID: %d\n", getpid());

  // La llamada al sistema fork() le pide al sistema operativo crear un nuevo
  // proceso (hijo) duplicando el proceso actual (padre).
  // Después de fork(), AMBOS programas correrán en paralelo, de forma
  // independiente.
  pid = fork();

  if (pid < 0) {
    // Un valor negativo indica un error, el sistema no pudo crear el hijo (ej.
    // límites alcanzados)
    fprintf(stderr, "Error al ejecutar fork()!\n");
    return 1;
  } else if (pid == 0) {
    // CÓDIGO DEL PROCESO HIJO
    // En el programa duplicado, la llamada a fork() termina y devuelve 0
    // getppid() devuelve el PID del proceso «padre» (parent PID)
    printf("Soy el proceso HIJO. Mi PID es: %d, y el PID de mi padre es: %d\n",
           getpid(), getppid());
  } else {
    // CÓDIGO DEL PROCESO PADRE
    // En el programa original, la llamada a fork() devuelve el PID del hijo
    // recién nacido
    printf("Soy el proceso PADRE. Mi PID es: %d, y el PID de mi nuevo hijo es: "
           "%d\n",
           getpid(), pid);
  }

  return 0;
}
