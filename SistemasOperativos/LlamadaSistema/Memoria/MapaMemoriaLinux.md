
# Explorando el Mapa de Memoria en Linux: Comandos Esenciales

Entender cómo se organiza la memoria es fundamental para cualquier estudiante de sistemas operativos. Linux proporciona herramientas poderosas para inspeccionar el mapa de memoria de los procesos. Aquí te presentamos algunos comandos clave, explicados de forma sencilla y con ejemplos prácticos.

## ¿Qué es el Mapa de Memoria?

El mapa de memoria de un proceso muestra cómo se organizan las diferentes regiones de memoria de ese proceso. Incluye información sobre:

* **Direcciones de memoria:** Dónde comienzan y terminan las regiones.
* **Permisos:** Si una región es de lectura, escritura o ejecución.
* **Archivos mapeados:** Si una región está asociada a un archivo en el disco.

## Comandos para Explorar el Mapa de Memoria

### 1. `/proc/[pid]/maps`

* **¿Qué hace?** Muestra el mapa de memoria de un proceso específico.
* **¿Cómo usarlo?**
    * Reemplaza `[pid]` con el ID del proceso.
    * Para el proceso actual, usa `/proc/self/maps`.
    * **Ejemplo:**
        * Primero, obtén el PID de un proceso. Por ejemplo, vamos a obtener el PID del proceso bash actual. Puedes usar el comando `echo $$`
            ```bash
            echo $$
            1234 #Ejemplo: este número será el PID del proceso bash.
            ```
        * Ahora, para ver el mapa de memoria del proceso bash.
            ```bash
            cat /proc/1234/maps
            ```
        * O, para ver el mapa de memoria del proceso actual.
            ```bash
            cat /proc/self/maps
            ```
* **¿Qué muestra?**
    * Direcciones de inicio y fin.
    * Permisos (r: lectura, w: escritura, x: ejecución, p: privado, s: compartido).
    * Desplazamiento en el archivo.
    * Dispositivo.
    * Inode.
    * Ruta del archivo mapeado (si existe).
* **Ejemplo simplificado de la salida:**

    ```
    7ffc12345000-7ffc12346000 rw-p 00000000 00:00 0 [stack]
    ```

    * Esto muestra una región de la pila (stack).

### 2. `pmap`

* **¿Qué hace?** Proporciona una visión más clara y resumida del mapa de memoria.
* **¿Cómo usarlo?**
    * `pmap [pid]` (Muestra la información básica).
    * `pmap -x [pid]` (Muestra información extendida).
    * **Ejemplo:**
        * Usando el PID del ejemplo anterior.
            ```bash
            pmap 1234
            ```
        * Para informacion detallada del mismo proceso.
            ```bash
            pmap -x 1234
            ```
        * Para ver el mapa de memoria del proceso actual:
            ```bash
            pmap -x $$
            ```
* **¿Qué muestra?**
    * Dirección de memoria.
    * Tamaño.
    * Permisos.
    * Nombre del archivo que mapea a la región, en caso que lo haya.
* **¿Por qué usarlo?** Es más fácil de leer que `/proc/[pid]/maps`, especialmente para estudiantes que están comenzando.

### 3. `htop` o `top`

* **¿Qué hacen?** Monitorizan los procesos en tiempo real, incluyendo el uso de la memoria.
* **¿Cómo usarlo?**
    * `top` o `htop` (ejecútalo en la terminal).
    *Dentro de cada uno de ellos, podrás observar el uso de memoria de todos los procesos en tiempo real.
* **Ejemplo:**
    * Simplemente escribe `top` o `htop` en tu terminal y presiona Enter.
    ```bash
    top
    ```
    o
    ```bash
    htop
    ```
* **¿Qué muestran?**
    * Uso de la RAM.
    * Uso de la memoria virtual.
    * Consumo de memoria de cada proceso.
* **¿Por qué usarlos?** Para tener una visión general del uso de la memoria en tu sistema.

### 4. `vmstat`

* **¿Qué hace?** Muestra estadísticas de la memoria virtual.
* **¿Cómo usarlo?** `vmstat` (ejecútalo en la terminal).
* **Ejemplo:**
    ```bash
    vmstat
    ```
    O para datos actualizados cada 5 segundos.
    ```bash
    vmstat 5
    ```
* **¿Qué muestra?**
    * Memoria utilizada.
    * Memoria libre.
    * Actividad de intercambio (swap).
    * Actividad de paginación.
* **¿Por qué usarlo?** Para entender cómo el sistema gestiona la memoria virtual en general.

