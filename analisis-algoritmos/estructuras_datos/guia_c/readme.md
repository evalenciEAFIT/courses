# Guía Básica de C en Linux/WSL

## 1. Preparación del Entorno

Si estás en **WSL** (Ubuntu) o en una distribución **Linux** (Debian/Ubuntu), primero necesitas el compilador. La herramienta estándar es **GCC** (GNU Compiler Collection).

### Instalar GCC y herramientas básicas

Abre tu terminal y ejecuta:

```bash
# Actualizar la lista de paquetes
sudo apt update

# Instalar el compilador gcc, make y gdb (debugger)
sudo apt install build-essential gdb
```

### Verificar instalación

Para asegurar que todo está listo, verifica la versión de GCC:

```bash
gcc --version
```
*Deberías ver algo como `gcc (Ubuntu ...)`.*

---

## 2. Flujo de Trabajo Básico

El proceso para crear un programa en C consta de 3 pasos:
1.  **Editar:** Escribir el código en un archivo de texto.
2.  **Compilar:** Traducir el código C a lenguaje máquina (un ejecutable).
3.  **Ejecutar:** Correr el programa.

### Paso A: Crear el archivo (Hola Mundo)

Usaremos un editor de texto simple desde la terminal llamado `nano`.

1.  Crea el archivo:
    ```bash
    nano hola.c
    ```

2.  Escribe el siguiente código dentro del editor:

    ```c
    #include <stdio.h>

    int main() {
        printf("Hola, mundo desde Linux!\n");
        return 0;
    }
    ```

3.  Guarda y sal:
    *   Presiona `Ctrl + O` (Write Out) y `Enter` para guardar.
    *   Presiona `Ctrl + X` para salir.

### Paso B: Compilar

El comando básico es `gcc nombre_archivo.c`. Sin embargo, es buena práctica ponerle un nombre a tu archivo ejecutable final usando la flag `-o`.

```bash
gcc hola.c -o hola
```

*   `hola.c`: Tu código fuente.
*   `-o hola`: Le dice al compilador "guarda el resultado en un archivo llamado 'hola'".
*   Si no pones `-o`, Linux creará un archivo por defecto llamado `a.out`.

### Paso C: Ejecutar

En Linux, para ejecutar un programa que está en tu carpeta actual, debes usar `./` antes del nombre.

```bash
./hola
```

**Salida esperada:**
```text
Hola, mundo desde Linux!
```

---

## 3. Banderas de Compilación Útiles

A medida que tus programas crezcan, querrás ver errores y advertencias. Usa estas flags:

```bash
gcc -Wall -Wextra -g mi_programa.c -o mi_programa
```

*   `-Wall`: Activa todas las advertencias principales (muy recomendado para principiantes).
*   `-Wextra`: Activa advertencias extra.
*   `-g`: **Importante para Debuggear**. Añade información de depuración al ejecutable.

---

## 4. Depuración con GDB (Debugging)

Si tu programa tiene errores lógicos o falla (segfault), necesitas un depurador. **GDB** es el estándar en Linux.

### Paso 1: Compilar con símbolos de depuración

Es obligatorio compilar con la flag `-g`:

```bash
gcc -g debug_ejemplo.c -o debug_ejemplo
```

*Supongamos que este archivo tiene un bucle infinito o una división por cero.*

### Paso 2: Iniciar GDB

```bash
gdb ./debug_ejemplo
```

Verás una terminal que empieza con `(gdb)`.

### Paso 3: Comandos básicos de GDB

Dentro de GDB, usa estos comandos para inspeccionar tu programa:

| Comando | Descripción |
| :--- | :--- |
| `run` o `r` | Inicia la ejecución del programa. |
| `break main` o `b main` | Pone un "punto de ruptura" al inicio de la función `main`. El programa se detendrá allí. |
| `next` o `n` | Ejecuta la siguiente línea de código (sin entrar a funciones internas). |
| `step` o `s` | Ejecuta la siguiente línea entrando en las funciones si las hay. |
| `print variable` o `p variable` | Muestra el valor actual de una variable. |
| `continue` o `c` | Continúa la ejecución hasta el siguiente punto de ruptura. |
| `quit` o `q` | Sale del depurador. |

### Ejemplo de sesión de depuración:

Supongamos que tienes una variable `x` y quieres ver su valor:

```text
(gdb) break main
Punto de interrupción 1 at 0x...: file debug_ejemplo.c, line 5.
(gdb) run
Starting program: .../debug_ejemplo

Breakpoint 1, main () at debug_ejemplo.c:5
5           int x = 10;
(gdb) next
6           int y = 20;
(gdb) print x
$1 = 10
(gdb) print y
$2 = 20
(gdb) quit
```

---

## 5. Resumen Rápido de Comandos de Terminal

Para gestionar tus archivos de C en Linux:

| Acción | Comando |
| :--- | :--- |
| Listar archivos en la carpeta | `ls` |
| Ver contenido de archivo | `cat archivo.c` |
| Borrar archivo compilado | `rm nombre_programa` |
| Limpiar pantalla | `clear` |
| Crear carpeta | `mkdir proyectos` |
| Entrar en carpeta | `cd proyectos` |

¡Con esto tienes todo lo necesario para escribir, compilar, ejecutar y arreglar errores en tus programas C utilizando el terminal de Linux o WSL
