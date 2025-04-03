# Retos del Problema de los Filósofos Comensales

El problema de los filósofos comensales es un clásico en la programación concurrente y ofrece una excelente base para plantear diversos retos que desafíen y motiven a profundizar en conceptos como concurrencia, sincronización, manejo de recursos compartidos, y resolución de problemas como deadlocks y condiciones de carrera.

## **Reto 1: Básico - Resolver Deadlock**
### **Descripción**
Modifica el programa actual para evitar que ocurra un deadlock. Implementa una solución que garantice que todos los filósofos puedan comer sin quedarse bloqueados indefinidamente.

### **Sugerencias de solución**
- **Usa un árbitro**: Solo permite que un número limitado de filósofos intenten tomar tenedores al mismo tiempo.
- **Alterna el orden de adquisición de tenedores**: Haz que uno de los filósofos tome los tenedores en orden inverso (derecho primero, luego izquierdo).
- **Usa un mutex global** para controlar el acceso a los tenedores.

---

## **Reto 2: Intermedio - Monitorear el Comportamiento de los Filósofos**
### **Descripción**
Implementa un sistema de monitoreo que registre estadísticas sobre el comportamiento de los filósofos durante la simulación. Por ejemplo:
- Cuántas veces cada filósofo ha comido.
- Cuánto tiempo en promedio pasa cada filósofo pensando y comiendo.
- ¿Qué filósofo come más rápido? ¿Cuál come menos?

### **Sugerencias de implementación**
- Usa variables globales o estructuras adicionales para almacenar las estadísticas.
- Registra los tiempos de pensamiento y comida usando `clock()` o `std::chrono`.
- Al final de la simulación, muestra un informe con las estadísticas recopiladas.

---

## **Reto 3: Intermedio - Agregar Prioridades**
### **Descripción**
Asigna prioridades a los filósofos basadas en su ID o en algún otro criterio (por ejemplo, el filósofo 0 tiene mayor prioridad que el filósofo 4). Modifica el programa para que los filósofos con mayor prioridad tengan preferencia a la hora de tomar los tenedores.

### **Sugerencias de implementación**
- Usa una cola de prioridad o un mecanismo similar para gestionar quién puede tomar los tenedores primero.
- Implementa un sistema de turnos donde los filósofos con mayor prioridad siempre se sirvan antes.

---

## **Reto 4: Avanzado - Simulación Dinámica**
### **Descripción**
Haz que el número de filósofos y tenedores sea dinámico y configurable por el usuario. Permite que el usuario ingrese el número de filósofos al iniciar el programa. Además, agrega opciones para cambiar el tiempo de pensamiento y comida durante la ejecución.

### **Sugerencias de implementación**
- Usa argumentos de línea de comandos (`argc`, `argv`) para permitir que el usuario configure el número de filósofos.
- Implementa una interfaz interactiva donde el usuario pueda cambiar los tiempos de pensamiento y comida mientras la simulación está en ejecución.

---

## **Reto 5: Avanzado - Detectar y Solucionar Condiciones de Carrera**
### **Descripción**
Amplía el programa para detectar y solucionar condiciones de carrera. Por ejemplo, si dos filósofos intentan tomar el mismo tenedor simultáneamente, el programa debe detectarlo y evitar que ocurra.

### **Sugerencias de implementación**
- Usa un contador atómico o un mutex global para verificar si dos filósofos están compitiendo por el mismo recurso.
- Implementa un mecanismo de retroceso (*backoff*) para que los filósofos esperen un tiempo aleatorio antes de intentar tomar los tenedores nuevamente.

---

Estos retos ayudarán a los desarrolladores a profundizar en los conceptos fundamentales de la concurrencia y la sincronización, mejorando sus habilidades para diseñar sistemas robustos y eficientes.
