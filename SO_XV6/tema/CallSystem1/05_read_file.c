#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  // Abrimos el archivo en modo "Sólo Lectura" (O_RDONLY -> equivalente a 0 para
  // el Kernel)
  int fd = open("archivo_ejemplo.txt", O_RDONLY);

  // Si no lo encuentra, da error
  if (fd < 0) {
    perror(
        "Error al leer, asegúrate de correr primero el programa de escritura");
    return 1;
  }

  // Buffer es un espacio de memoria temporal reservado en la RAM para ir
  // guardando "pedacitos" de archivo a medida que los leemos. Aquí pedimos 128
  // bytes.
  char buffer[128];
  ssize_t bytes_leidos;

  printf("¡Leyendo el contenido del archivo con sys_read!:\n");
  printf("----------------------------------------------\n");

  // sys_read lee hasta 128 bytes y los pone en 'buffer'. Devuelve la cantidad
  // de bytes que realmente leyó. Mientras haya bytes, continuaremos un ciclo
  // "Bucle While"
  while ((bytes_leidos = read(fd, buffer, sizeof(buffer))) > 0) {
    // En vez de usar printf, podemos decirle a Linux que escriba directo en la
    // pantalla (stdout = 1) los bytes exactos que acabamos de leer
    write(1, buffer, bytes_leidos);
  }

  printf("----------------------------------------------\n");

  // Y debemos cerrar el descriptor
  close(fd);

  return 0;
}
