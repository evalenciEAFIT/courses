# Guía de System Calls (Llamadas al Sistema) en Linux y C

¡Hola! Esta pequeña guía tiene como objetivo explicar qué son las **llamadas al sistema (System Calls)** en el contexto de Linux y el lenguaje C. Además, documenta los tres ejemplos que hemos creado en este proyecto.

## ¿Qué es una Llamada al Sistema (System Call)?

En un sistema operativo como Linux, existe una división muy marcada:
1.  **Espacio de Usuario (User Space):** Donde se ejecutan tus programas, aplicaciones y nuestro código.
2.  **Modo Kernel (Kernel Space):** Donde reside el núcleo del sistema operativo. Él tiene control absoluto sobre el hardware (memoria, discos, pantalla, redes, etc.).

Para que un programa en el Espacio de Usuario pueda interactuar con el hardware (por ejemplo, imprimir un mensaje en la pantalla, leer un archivo o crear un proceso), **debe pedirle permiso al Kernel**.

La forma de hacer esta petición es mediante una **Llamada al Sistema (System Call)**.

### Ejemplos Comunes
-   `write`: Escribir datos a un archivo o la pantalla.
-   `read`: Leer datos de un archivo, teclado, red.
-   `open`: Abrir un archivo.
-   `fork`: Crear un nuevo proceso.
-   `exit`: Terminar un proceso.

---

## Diferencia entre la Biblioteca Estándar (glibc) y Syscalls Directas

Normalmente en C usamos funciones de la biblioteca estándar, como `printf()`. Detrás de escenas, `printf()` prepara el texto, los números y el formato, y al final **hace una llamada al sistema `write`** para que el Kernel efectivamente muestre los caracteres en la pantalla.

Usar las *Syscalls* directamente (`write()`) suele ser más rápido en ciertos contextos muy específicos (programación de bajo nivel), pero es más difícil porque en vez de usar cadenas formateadas (`%d`, `%s`), debes manejar los bytes crudos y su longitud exacta.

---

## 📁 Explorando los Ejemplos del Proyecto

### 1. Nuestro conocido: `01_hello_printf.c`
Este código hace un *"Hola Mundo"* utilizando la función de biblioteca `printf()`. Como mencionamos, `printf()` internamente usa buffer y formato antes de llamar al sistema operativo.

### 2. Directo al Kernel: `02_hello_syscall.c`
En este ejemplo utilizamos **directamente** la system call `write()`.
La firma de la función es:
`ssize_t write(int fd, const void *buf, size_t count);`

-   **`fd` (File Descriptor):** Usamos el valor `1` que representa la **salida estándar** (`stdout` / la consola).
-   **`buf`:** Es nuestro mensaje (el arreglo de caracteres).
-   **`count`:** La cantidad exacta de bytes que necesitamos enviar al Kernel. Por eso calculamos la longitud con `strlen()`.

### 3. Creación de Procesos: `03_fork_example.c`
La llamada al sistema `fork()` es fascinante. Le dice al Kernel: *"Clona el proceso actual exactamente como está"*.

-   Cuando `fork()` se ejecuta, el Kernel hace una copia del programa. De un momento a otro ¡tienes **dos** programas corriendo al mismo tiempo!
-   **¿Cómo sabemos cuál es cuál?** `fork()` devuelve valores diferentes a cada uno:
    -   Al **Hijo** recién creado le devuelve el valor `0`.
    -   Al **Padre** (el programa original) le devuelve el número de identificación del hijo (Process ID o **PID**).
    -   Si devuelve un número menor a 0, hubo un error y no se pudo crear el clon.

Por esta razón, típicamente usamos bloques `if / else` inmediatamente después de `fork()` para decirle a cada programa qué debe hacer desde ese punto en adelante.

### 4. Escribiendo Archivos al Disco: `04_write_file.c`
Este documento abre un archivo o lo crea si no existe usando la llamada `open()` de nivel bajo (del sistema POSIX, no la tradicional `fopen` de la biblioteca C).
- Usamos banderas (Flags) enviadas al Kernel por bits `O_WRONLY | O_CREAT | O_TRUNC` que le dicen al SO respectivamente: "Solo para escribir", "Créalo si no existe", y "Limpia cualquier cosa que tuviera antes".
- El núcleo devuelve un **descriptor de archivo** (File Descriptor). Esta es una credencial numérica de acceso para probar que la aplicación tiene autorización del Kernel para operar. El SO lo verifica siempre que disparamos el `write` final.

### 5. Leyendo Datos por Búferes: `05_read_file.c`
Leyendo el archivo que originamos en el paso anterior usando otro File Descriptor (esta vez con el flag inverso `O_RDONLY`).
- No es seguro pedirle al núcleo "dame el archivo entero", podría pesar 100 GB y nuestra RAM colapsaría.
- Así que le pedimos a la syscall `read` en un Bucle iterativo que nos entregue la información de a `128` bytes (nuestro buffer reservado), y repetimos el ciclo hasta que detectemos el fin (EOF).

## 🧠 El Nivel Más Bajo: Programando en Ensamblador (Assembly)

Para entender cómo funciona el CPU y las llamadas por debajo, escribimos los equivalentes exactos de nuestros programas en C utilizando lenguaje Ensamblador puro (sintaxis AT&T para Linux x86_64).

### 1. Ensamblador + glibc: `01_hello_printf.s`
Emula a `01_hello_printf.c`. Carga un string en el registro `%rdi` e invoca la función `printf` utilizando la instrucción `call`. Aún dependemos de la biblioteca de C (`glibc`), por lo que no es una `syscall` pura.

### 2. Ensamblador Puro (sys_write): `02_hello_syscall.s`
Emula a `02_hello_syscall.c`. No usa ninguna librería. Nos comunicamos pura y duramente con el Kernel utilizando interrupciones de software `syscall`:
- `sys_write`: Cargamos **1** en el registro `%rax`. Seteamos `%rdi` (Descriptor), `%rsi` (Dirección memoria) y `%rdx` (Longitud). Y ejecutamos `syscall`.
- `sys_exit`: Cargamos **60** en `%rax` para terminar el programa y devolver el control a Linux.

### 3. Modificando el Sistema (sys_fork y demás): `03_fork_example.s`
La obra maestra técnica. Emula `03_fork_example.c`. Llama a múltiples funciones del Kernel en crudo sin depender en absoluto de C:
- **`sys_fork`** (`%rax = 57`): Crea el proceso hijo.
- **`sys_getpid`** (`%rax = 39`): Obtiene nuestro PID.
- **`sys_getppid`** (`%rax = 110`): Obtiene el PID de nuestro padre.
- ¡Incluye además un bloque de código manual `print_num` (ITOA) que convierte los Process IDs devueltos por Linux (que son enteros binarios gigantes) en caracteres de texto (ASCII), usando divisiones puras sobre registros de la CPU, guardándolos en la pila de hilos locales y luego expeliendo esos caracteres por la pantalla!

### 4 y 5. Archivos con Ensamblador Puro: `sys_open`, `sys_read`
Creados en `04_write_file.s` y `05_read_file.s`. Accedemos a los drivers del Sistema de Archivos Ext4 en el disco de manera completamente nativa y rápida, usando puros registros de la CPU para configurar permisos octales (`rw-r--r--` convertido como el int `420`) y las operaciones bit a bit nativas para truncar el documento usando `sys_open` (syscall `2`), pidiendo memoria RAM reservada (bss data segement) de tamaño estricto dictado por la arquitectura y realizando escrituras cíclicas con Syscall `0` (Read).

---

## 🛠️ Cómo Compilar y Usar Archivos en Ensamblador

A diferencia de C (donde `gcc` hace de todo con un solo comando), los binarios puros en Ensamblador tienen dos pasos manuales (excepto `01_hello_printf.s` que, al depender de `glibc`, también exige `gcc`).

1. **Ensamblador que usa Librerías de C:**
   ```bash
   gcc 01_hello_printf.s -o 01_hello_printf_asm
   ```

2. **Ensamblador Puro `as` (Assembler) y `ld` (Linker):** 
   Traducimos a código objeto (`.o`) y luego los enlazamos a un ejecutable.
   ```bash
   as 02_hello_syscall.s -o 02_hello_syscall.o  && ld 02_hello_syscall.o -o 02_hello_syscall_asm
   as 03_fork_example.s -o 03_fork_example.o    && ld 03_fork_example.o -o 03_fork_example_asm
   as 04_write_file.s -o 04_write_file.o        && ld 04_write_file.o -o 04_write_file_asm
   as 05_read_file.s -o 05_read_file.o          && ld 05_read_file.o -o 05_read_file_asm
   ```

3. **La Ejecución:**
   ```bash
   ./04_write_file_asm
   ./05_read_file_asm
   ```

---

## 🚀 Cómo Compilar y Ejecutar C

Abre el terminal en esta carpeta y usa el compilador `gcc` de esta manera para nuestros primeros tres ejemplos C:

```bash
# Para compilar los programas en C:
gcc 01_hello_printf.c -o 01_hello_printf
gcc 02_hello_syscall.c -o 02_hello_syscall
gcc 03_fork_example.c -o 03_fork_example
gcc 04_write_file.c -o 04_write_file
gcc 05_read_file.c -o 05_read_file

# Para ejecutarlos (El orden 04 -> 05 importa, debes crear el archivo para leerlo!):
./04_write_file
./05_read_file
```
