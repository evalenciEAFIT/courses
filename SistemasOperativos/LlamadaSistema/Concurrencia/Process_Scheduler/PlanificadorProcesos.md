# Flujo de Estados de un Proceso: CREADO, LISTO, EJECUTANDO, TERMINADO

En un sistema operativo multitarea, los procesos pasan por diferentes estados a lo largo de su ciclo de vida. Comprender estos estados y las transiciones entre ellos es fundamental para entender cómo el sistema operativo gestiona la ejecución de las aplicaciones. Los cuatro estados fundamentales que un proceso puede tener son: **CREADO**, **LISTO**, **EJECUTANDO** y **TERMINADO**.

## 1. CREADO (New)

* **Descripción:** Este es el estado inicial de un proceso. Un proceso entra en el estado CREADO cuando se solicita su creación, ya sea por el usuario (al ejecutar un programa), por otro proceso o por el propio sistema operativo.
* **Actividades:** Durante esta fase, el sistema operativo realiza las siguientes acciones:
    * **Asignación de Recursos:** Se asigna al proceso el espacio de memoria necesario, así como otros recursos iniciales (aunque algunos recursos podrían asignarse más tarde).
    * **Creación de la Estructura de Datos del Proceso:** Se crea el Bloque de Control del Proceso (PCB - Process Control Block), que contiene toda la información necesaria para gestionar el proceso (ID, estado, prioridad, punteros a memoria, etc.).
* **Transición:** Una vez que el sistema operativo ha completado las tareas iniciales y el proceso está listo para competir por la CPU, pasa al estado **LISTO**.

## 2. LISTO (Ready)

* **Descripción:** Un proceso en estado LISTO está preparado para ejecutarse y solo necesita que la CPU esté disponible. Puede haber múltiples procesos en estado LISTO, formando una cola de listos (ready queue).
* **Características:**
    * El proceso ha sido cargado en memoria y tiene todos los recursos necesarios para su ejecución, excepto la CPU.
    * El proceso está esperando a que el planificador de procesos (scheduler) le asigne tiempo de CPU.
* **Transiciones:**
    * **Desde CREADO:** Una vez que la inicialización del proceso se completa, pasa al estado LISTO.
    * **Desde EJECUTANDO:** Si un proceso en estado EJECUTANDO es interrumpido (por ejemplo, porque ha agotado su quantum de tiempo en un sistema Round Robin o por una interrupción de mayor prioridad), vuelve al estado LISTO para esperar su siguiente turno en la CPU.
    * **Desde BLOQUEADO (Waiting):** Si un proceso que estaba esperando un evento (E/S, semáforo, etc.) recibe la señal o el recurso esperado, pasa al estado LISTO para reanudar su ejecución cuando la CPU esté disponible.

## 3. EJECUTANDO (Running)

* **Descripción:** Un proceso en estado EJECUTANDO es aquel que actualmente está utilizando la CPU para llevar a cabo sus instrucciones. Solo un proceso puede estar en estado EJECUTANDO en una CPU mononúcleo en un instante dado. En sistemas multinúcleo, varios procesos pueden estar en estado EJECUTANDO simultáneamente.
* **Actividades:** Durante este estado, las instrucciones del programa del proceso son ejecutadas por la CPU.
* **Transiciones:**
    * **Desde LISTO:** El planificador de procesos selecciona un proceso de la cola de listos y le asigna la CPU, lo que provoca la transición al estado EJECUTANDO (esta acción se conoce como *despacho* o *dispatch*).
    * **A LISTO:** Si el proceso en ejecución es interrumpido (por ejemplo, por agotamiento del quantum de tiempo, una interrupción de mayor prioridad o una solicitud de E/S que no bloquea inmediatamente), vuelve al estado LISTO para esperar su siguiente turno en la CPU.
    * **A BLOQUEADO (Waiting):** Si el proceso en ejecución necesita esperar por un evento que no está inmediatamente disponible (como la finalización de una operación de entrada/salida, la adquisición de un bloqueo o la recepción de una señal), pasa al estado BLOQUEADO.
    * **A TERMINADO (Terminated):** Si el proceso completa su ejecución, ya sea de forma normal o debido a un error, pasa al estado TERMINADO.

## 4. TERMINADO (Terminated)

* **Descripción:** Este es el estado final de un proceso. Un proceso entra en el estado TERMINADO cuando ha completado su ejecución.
* **Actividades:**
    * **Liberación de Recursos:** El sistema operativo libera todos los recursos que fueron asignados al proceso (memoria, archivos abiertos, etc.).
    * **Eliminación del PCB:** La estructura de datos del proceso (PCB) es eliminada del sistema.
* **Transiciones:** La transición a este estado es unidireccional desde el estado EJECUTANDO. Una vez que un proceso está TERMINADO, no puede volver a ningún otro estado del ciclo de vida.

## Diagrama de Flujo de Estados

A continuación, se presenta un diagrama simple que ilustra el flujo entre estos estados:
