#include <string.h>
#include <unistd.h>

int main() {
  // Definimos el mensaje que queremos imprimir en pantalla
  const char *mensaje = "Hola Mundo usando system call (write) directamente!\n";

  // Llamada al sistema 'write' para escribir en un descriptor de archivo (file
  // descriptor). Esta función le pide al kernel (al sistema operativo) de forma
  // directa que imprima nuestro texto, sin pasar por los envoltorios habituales
  // de glibc como printf.
  //
  // Argumentos de write():
  // 1: El descriptor de archivo. '1' significa la salida estándar (stdout o
  // pantalla). 2: Puntero a la cadena de texto con los datos a escribir. 3:
  // Cantidad de bytes (longitud del texto) a escribir.
  write(1, mensaje, strlen(mensaje));

  return 0;
}
