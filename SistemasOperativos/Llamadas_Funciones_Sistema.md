
# Llamadas a funciones del sistema en la programación

Las llamadas a funciones del sistema ("system calls") son interfaces fundamentales que permiten a las aplicaciones de usuario interactuar con el sistema operativo. A través de estas, los programas pueden acceder a recursos del sistema, como archivos, dispositivos, memoria y procesos.

## Concepto

Una llamada al sistema es una solicitud que un programa en modo usuario hace al núcleo del sistema operativo para realizar una tarea específica. Estas tareas suelen involucrar operaciones que requieren permisos especiales o acceso directo al hardware, como leer o escribir archivos, crear procesos, o establecer conexiones de red.

En términos generales, las llamadas al sistema representan la barrera entre el espacio de usuario (user space) y el espacio del núcleo (kernel space). Cuando un programa ejecuta una llamada al sistema, el control se transfiere del programa al núcleo mediante un mecanismo especial, como una interrupción o una instrucción específica del procesador.

### Características principales:
1. **Privilegios:** El núcleo tiene privilegios que no están disponibles en el espacio de usuario.
2. **Interfaz estándar:** Las llamadas al sistema son expuestas mediante APIs estándar, como POSIX en sistemas tipo Unix.
3. **Bloqueo:** Algunas llamadas pueden ser bloqueantes, lo que significa que el programa espera hasta que se complete la operación.

---

## Clasificación y descripción de las llamadas al sistema en Linux

En los sistemas operativos Linux, las llamadas al sistema están clasificadas según su propósito. A continuación, se presenta una lista de llamadas al sistema comunes, agrupadas y explicadas según su uso:

### 1. Gestión de archivos

#### `open`
- **Propósito:** Abrir un archivo para lectura, escritura o ambos.
- **Uso:**
  ```c
  int fd = open("archivo.txt", O_RDONLY);
  ```
- **Descripción:** Devuelve un descriptor de archivo que puede usarse para realizar operaciones como lectura (`read`) o escritura (`write`).

#### `read`
- **Propósito:** Leer datos de un archivo.
- **Uso:**
  ```c
  ssize_t bytes_leidos = read(fd, buffer, tamaño);
  ```
- **Descripción:** Lee hasta `tamaño` bytes del archivo referido por el descriptor `fd` y los almacena en `buffer`.

#### `write`
- **Propósito:** Escribir datos en un archivo.
- **Uso:**
  ```c
  ssize_t bytes_escritos = write(fd, buffer, tamaño);
  ```
- **Descripción:** Escribe `tamaño` bytes desde `buffer` en el archivo referido por el descriptor `fd`.

#### `close`
- **Propósito:** Cerrar un archivo.
- **Uso:**
  ```c
  int resultado = close(fd);
  ```
- **Descripción:** Libera el descriptor de archivo para que pueda ser reutilizado.

### 2. Gestión de procesos

#### `fork`
- **Propósito:** Crear un nuevo proceso.
- **Uso:**
  ```c
  pid_t pid = fork();
  ```
- **Descripción:** El proceso hijo es una copia del proceso padre, con un PID único.

#### `execve`
- **Propósito:** Ejecutar un nuevo programa en el proceso actual.
- **Uso:**
  ```c
  execve("/bin/ls", argv, envp);
  ```
- **Descripción:** Reemplaza el código del proceso actual por el nuevo programa especificado.

#### `wait`
- **Propósito:** Esperar a que un proceso hijo termine.
- **Uso:**
  ```c
  int status;
  pid_t pid = wait(&status);
  ```
- **Descripción:** Permite al proceso padre sincronizarse con la finalización de sus hijos.

### 3. Gestión de memoria

#### `mmap`
- **Propósito:** Mapear un archivo o dispositivo en memoria.
- **Uso:**
  ```c
  void* addr = mmap(NULL, tamaño, PROT_READ, MAP_PRIVATE, fd, offset);
  ```
- **Descripción:** Asigna un área de memoria que corresponde al contenido de un archivo o dispositivo.

#### `munmap`
- **Propósito:** Desmapear una región de memoria.
- **Uso:**
  ```c
  int resultado = munmap(addr, tamaño);
  ```
- **Descripción:** Libera la memoria mapeada previamente con `mmap`.

### 4. Comunicaciones y redes

#### `socket`
- **Propósito:** Crear un punto final de comunicación.
- **Uso:**
  ```c
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  ```
- **Descripción:** Devuelve un descriptor de socket que puede usarse para enviar y recibir datos a través de redes.

#### `bind`
- **Propósito:** Asociar un socket a una dirección local.
- **Uso:**
  ```c
  int resultado = bind(sockfd, (struct sockaddr*)&direccion, sizeof(direccion));
  ```
- **Descripción:** Establece la dirección IP y el puerto que el socket usará para la comunicación.

#### `listen`
- **Propósito:** Marcar un socket como pasivo, para aceptar conexiones.
- **Uso:**
  ```c
  int resultado = listen(sockfd, 5);
  ```
- **Descripción:** Configura el socket para aceptar conexiones entrantes.

#### `accept`
- **Propósito:** Aceptar una conexión entrante en un socket.
- **Uso:**
  ```c
  int cliente_fd = accept(sockfd, (struct sockaddr*)&cliente, &tam_cliente);
  ```
- **Descripción:** Devuelve un descriptor de socket para interactuar con el cliente conectado.

### 5. Gestión del sistema

#### `uname`
- **Propósito:** Obtener información sobre el sistema operativo.
- **Uso:**
  ```c
  struct utsname buffer;
  int resultado = uname(&buffer);
  ```
- **Descripción:** Llena la estructura `utsname` con detalles del sistema, como el nombre del kernel y la versión.

#### `getpid`
- **Propósito:** Obtener el identificador del proceso actual.
- **Uso:**
  ```c
  pid_t pid = getpid();
  ```
- **Descripción:** Devuelve el PID del proceso que lo llama.

#### `gettimeofday`
- **Propósito:** Obtener la hora actual.
- **Uso:**
  ```c
  struct timeval tiempo;
  int resultado = gettimeofday(&tiempo, NULL);
  ```
- **Descripción:** Llena la estructura `timeval` con la hora actual en segundos y microsegundos desde el Epoch.

---

## Ejemplo en C++

En C++, las llamadas al sistema pueden realizarse utilizando bibliotecas estándar como `<unistd.h>` en sistemas Unix/Linux. A continuación, un ejemplo básico que utiliza la llamada al sistema `write` para escribir en la salida estándar:

```cpp
#include <unistd.h> // Para las llamadas al sistema write y read
#include <cstring>  // Para manejar cadenas de caracteres

int main() {
    const char* mensaje = "Hola desde una llamada al sistema!
";
    size_t longitud = strlen(mensaje);

    // Llamada al sistema write
    ssize_t bytes_escritos = write(STDOUT_FILENO, mensaje, longitud);

    if (bytes_escritos == -1) {
        // Manejo de errores
        perror("Error al escribir");
        return 1;
    }

    return 0;
}
```

### Explicación del código:
- `write` es una llamada al sistema que escribe datos en un archivo o descriptor de archivo. En este caso, `STDOUT_FILENO` (la salida estándar) se utiliza como destino.
- La función devuelve el número de bytes escritos o `-1` si ocurre un error.

---

## Ejemplo en Rust

Rust proporciona una forma segura y eficiente de realizar llamadas al sistema mediante la biblioteca estándar y crates externos como `nix`. Aquí hay un ejemplo similar que escribe en la salida estándar:

```rust
use std::io::{self, Write};
use std::os::unix::io::AsRawFd;

fn main() {
    let mensaje = b"Hola desde una llamada al sistema en Rust!
";
    let stdout = io::stdout();
    let mut handle = stdout.lock();

    // Obtener el descriptor de archivo de stdout
    let fd = handle.as_raw_fd();

    // Llamada al sistema write usando libc
    let bytes_escritos = unsafe {
        libc::write(fd, mensaje.as_ptr() as *const _, mensaje.len())
    };

    if bytes_escritos == -1 {
        eprintln!("Error al escribir: {}", std::io::Error::last_os_error());
    }
}
```

### Explicación del código:
- `libc::write` es una función no segura que se usa para interactuar directamente con el sistema operativo.
- Se utiliza `as_raw_fd` para obtener el descriptor de archivo del estándar de salida.
- El uso de `unsafe` es necesario porque interactuar directamente con funciones del sistema operativo puede tener implicaciones no controladas por Rust.

---

## Importancia de las llamadas al sistema

1. **Control de hardware:** Permiten a las aplicaciones interactuar con dispositivos físicos sin gestionar directamente sus detalles internos.
2. **Abstracción:** Ofrecen una capa de abstracción sobre operaciones complejas, como la gestión de memoria o las comunicaciones en red.
3. **Estandarización:** APIs como POSIX garantizan que las aplicaciones puedan ejecutarse en múltiples sistemas compatibles.

Las llamadas al sistema son un puente esencial entre el software y el hardware. Comprender cómo funcionan y cuándo utilizarlas es fundamental para diseñar aplicaciones eficientes y seguras.

