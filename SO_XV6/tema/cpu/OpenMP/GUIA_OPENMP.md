# Guía de Paralelización Avanzada con C y OpenMP

En esta guía abordaremos el concepto de paralelismo estandarizado aplicado a procesos pesados (como álgebra de matrices) utilizando la API `OpenMP`.

---

## 1. ¿Qué es OpenMP (Open Multi-Processing)?

OpenMP es una interfaz de programación de aplicaciones (API) diseñada para la programación concurrente (o paralela) en sistemas de **memoria compartida** (como los procesadores multinúcleo de tu laptop o servidor).

En lenguajes como **C, C++ o Fortran**, OpenMP funciona por medio de **Directivas de Compilador** (los famosos `#pragma`). La gran ventaja de esta filosofía es que si compilas un programa con directivas OpenMP empleando un compilador sin soporte de este estándar, el código funcionará de manera secuencial ignorando las etiquetas en vez de generar un error de sintaxis. ¡Añades paralelismo sin romper tu programa clásico!

---

## 2. Conceptos Fundamentales

OpenMP sigue el modelo estructural de **Fork-Join** (Bifurcación-Unión):

1. **Master Thread**: De base el programa comienza teniendo 1 solo hilo (Secuencial).
2. **Fork**: Cuando el programa entra a una "región paralela" (definida por el `#pragma omp parallel`), el *Master Thread* se subdivide creando un número arbitrario de nuevos hilos trabajadores ("Slave Threads").
3. Toda la carga de trabajo dentro de esas llaves se reparte entre los hilos creados.
4. **Join**: Al finalizar el bloque de código, se realiza una barrera natural de espera; Los hilos se destruyen unificando los resultados al *Master Thread* original que avanza al resto del código hacia abajo.

---

## 3. Las Directivas Principales Utilizadas

Para indicarle al pre-procesador que vamos a incluir directivas de esta API, todas las líneas deben iniciar por:

```c
#pragma omp <directiva_a_utilizar> [clausulas_extras]
```

### A. Ejecutar un bloque en paralelo (Directiva Base)

```c
#pragma omp parallel
{
    // Código aquí adentro se ejecutará simultánemante 'T' número de veces
    printf("¡Hola desde el hilo %d!\n", omp_get_thread_num());
}
```

### B. Paralelizar sobre un iterador `for` (Work-Sharing)

La cláusula `parallel for` le avisa al procesador que tome un ciclo finito y lo parta en 'chunks' (trozos temporales). Si tienes 1000 iteraciones y tu PC tiene 4 núcleos, asignará el ciclo de la repetición 0-250 al hilo 1, de la 251-500 al hilo 2, y así...

```c
#pragma omp parallel for
for(int i = 0; i < N; i++){
    do_heavy_work(i);
}
```

Es crucial que las iteraciones en el `for` **sean totalmente independientes**. Si un ciclo depende en el resultado iterativo anterior, el código lanzaría problemas de consistencia denominados _Hilos de Competencia (Race Conditions)_.

### C. Cláusulas de Gestión de Memoria (`private`, `shared`)

En el paradigma de Memoria Compartida es vital decirle a los hilos de CPU qué variables pueden tocar todos sin pudor, y qué iteradores debe cada uno tener su copia única para no afectar la iteración de los vecinos.

* `shared(A, B, C)`: Todos los hilos leen y apuntan a la misma RAM de las Variables A, B y C.
* `private(i, j, k)`: Se crea una nueva copia efímera e independiente de $i, j, k$ en la memoria local (Pila) del hilo actual. Modificar su *'i'* no altera la variable local *'i'* en los otros hilos.

_¡Importante!: En la multiplicación de matrices, los iteradores base ($i, j, k$) deben ser estrictamente `private` para evitar colapsos cruzados, y las matrices grandes como `A` que se leen en bloque deben ser `shared` para no provocar copias abusivas de Memoria RAM de Gigabytes de ramificación._

---

## 4. Comparativa Experimental: Multiplicación de Matrices $N \times N$

El cuello de botella clásico computacional O(n³). Hemos implementado dos algoritmos:

1. `matrix_seq.c`: Lógica de matrices base para analizar cuánto tarda 1 solo núcleo de reloj.
2. `matrix_omp.c`: Mismo código, pero con el *#pragma omp parallel for* integrando la potencia de todo el _silicio_ simultáneamente. Y usando `omp_get_wtime()` para medir el tiempo real sin adulteración del kernel en concurrencia.

### Compilando el código

Debes utilizar siempre el parámetro de bandera explícito `-fopenmp` en *gcc*:

```bash
# O a través de nuestro archivo Make pregenerado:
make
```

### Ejecutar comparativa

```bash
./matrix_seq
./matrix_omp
```

Vas a presenciar como el código OpenMP puede ser **extremadamente más rápido** proporcional a la cantidad de núcleos / hilos (Lógicos o Físicos) de la máquina que ejecuta el programa C.

---

## 5. Comparativa Experimental: Multiplicación de BigNumbers (80 Dígitos)

Para visualizar los límites del paralelismo, implementamos un caso de borde donde el paralelismo OpenMP resulta **perjudicial** si no hay suficiente carga matemática para justificar la inyección de hilos.

1. `big_mult_seq.c`: Implementa el algoritmo de **multiplicación de la escuela** sobre dos cadenas (`string` / `char array`) gigantes de 80 números uno a uno simulando matemática sin el límite `long int`.
2. `big_mult_omp.c`: Mismo algoritmo pero introduciendo `#pragma omp parallel for` entre los tensores de cada dígito y `#pragma omp atomic` para que los hilos no choquen al intentar guardar el residual (el acarreo) en el mismo cajón del arreglo durante un ciclo cruzado microtemporal.

### Cómo inyectar OpenMP al cálculo de String

El punto vital del código `big_mult_omp.c` sucede al momento de acumular la suma dentro de la matriz final sin alterar las sumas lógicas:

```c
#pragma omp parallel for
for (int i = 0; i < len1; i++) {
    for (int j = 0; j < len2; j++) {
        int mul = (num1[i] - '0') * (num2[j] - '0');
      
        // Bloqueo Atómico: Pausa de microsegundos a los demás hilos
        // asegurando que esta sobre-escritura en RAM no genere variables cruzadas 
        #pragma omp atomic
        res[i + j + 1] += mul;
    }
}
```

Es imprescindible usar un **Atómico** en lugar de `private` estricto porque el índice `[i + j + 1]` choca a través de diferentes hilos. Todos los índices escriben y arrastran al mismo centro.

### Ejecutar caso de Bignumber

```bash
make run
# Observa las ejecuciones de los ejecutables 'big_mult'
```

### Conclusión y Cuello de Botella (Thread Overhead)

Con estas pruebas observarás que iterar una suma de 80 casillas con un solo hilo es de **2 a 4 veces más veloz** que poner a los otros núcleos a ayudar...

¿Por qué? Porque el sobrecoste (_Overhead_) interno que le toma al núcleo Maestro decirle al sistema operativo "Sepárame esta RAM en 12 canales virtuales, distribúyela, y luego sincronízalas con 'pausas Atómicas'" es un proceso abismalmente más largo a que ese pequeño Master hubiese simplemente resuelto la suma veloz del String completo por su propia cuenta. ¡El paralelismo debe ejecutarse exclusivamente en macro bucles gigantes algorítmicos como $O(n^3)$ y _machine_learning_ matrices!
