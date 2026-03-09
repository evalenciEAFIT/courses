#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  // La System Call open()
  // Argumentos:
  // 1. Nombre del archivo
  // 2. Banderas (O_WRONLY: Solo escritura, O_CREAT: Crear si no existe,
  // O_TRUNC: Sobrescribir si ya existe)
  // 3. Permisos en caso de creación: 0644 (Lectura/Escritura para usuario,
  // Lectura para el resto)
  int fd = open("archivo_ejemplo.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

  // Si open() devuelve un número menor a 0, hubo un error (ej. sin permisos o
  // disco lleno)
  if (fd < 0) {
    perror("Error al abrir/crear el archivo");
    return 1;
  }

  // Nuestro mensaje
  const char *mensaje =
      "¡Hola! Este archivo fue creado escribiendo directamente en el disco "
      "mediante la llamada al sistema sys_write.\n";

  // Llamada al sistema write()
  // A este punto, el kernel sabe que nuestro 'fd' (File Descriptor) apunta a
  // 'archivo_ejemplo.txt'
  write(fd, mensaje, strlen(mensaje));

  // Llamada al sistema close() para liberar los recursos
  close(fd);

  printf("El archivo 'archivo_ejemplo.txt' fue escrito exitosamente en el "
         "disco!\n");
  return 0;
}
