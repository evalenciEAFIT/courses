# Guía Básica de C en Linux con WSL y Code - Explicación Detallada

## Introducción

C es un lenguaje de programación de propósito general desarrollado en 1972 por Dennis Ritchie. Es conocido por su eficiencia, portabilidad y control de bajo nivel sobre el hardware. En Linux, C es especialmente importante porque el propio kernel está escrito en C.

**¿Por qué aprender C en Linux?**
- Linux está escrito en C, por lo que hay herramientas excelentes de desarrollo
- Tienes acceso directo a las llamadas al sistema
- El entorno es estable y bien documentado
- WSL permite desarrollar en C con herramientas Linux desde Windows

---

## 1. Hola Mundo - Explicación Detallada

### Análisis línea por línea

```c
// hola_mundo.c

// Directiva del preprocesador
#include <stdio.h>
/*
 * #include: Instruye al preprocesador a incluir otro archivo
 * <stdio.h>: Header file que contiene declaraciones para funciones
 * de entrada/salida estándar como printf()
 * 
 * El preprocesador:
 * 1. Elimina comentarios
 * 2. Incluye archivos .h
 * 3. Procesa macros (#define)
 * 4. Compilador propiamente dicho
 * 5. Linker une todo
 */

// Definición de función principal
int main() {
    /*
     * main(): Punto de entrada de TODO programa en C
     * El sistema operativo llama a esta función cuando ejecutas el programa
     * int: Tipo de retorno. Retorna un código de salida al SO:
     * - 0: Éxito
     * - ≠0: Error (código específico)
     */
    
    // Llamada a función printf()
    printf("¡Hola Mundo desde C en WSL!\n");
    /*
     * printf(): Función declarada en stdio.h
     * Argumento: String literal (cadena entre comillas dobles)
     * \n: Secuencia de escape para nueva línea
     * Otras secuencias comunes:
     * \t: Tabulación
     * \\: Backslash literal
     * \": Comillas dobles literal
     * \': Comilla simple literal
     */
    
    // Valor de retorno
    return 0;
    /*
     * return: Termina la función main()
     * 0: Indica al SO que el programa terminó exitosamente
     * En Linux, puedes ver el código de salida con: echo $?
     * después de ejecutar el programa
     */
}
```

### Proceso de compilación paso a paso

```bash
# 1. Preprocesamiento: Expande #includes y macros
gcc -E hola_mundo.c -o hola_mundo.i
# Salida: archivo .i con todo el código expandido

# 2. Compilación: Convierte a código ensamblador
gcc -S hola_mundo.i -o hola_mundo.s
# Salida: archivo .s en lenguaje ensamblador x86_64

# 3. Ensamblaje: Convierte a código objeto (binario)
gcc -c hola_mundo.s -o hola_mundo.o
# Salida: archivo .o (objeto) en formato ELF

# 4. Enlazado: Une con librerías
gcc hola_mundo.o -o hola_mundo
# Salida: ejecutable final

# Comando que hace todo en uno paso:
gcc -o hola_mundo hola_mundo.c
```

### Análisis del ejecutable

```bash
# Ver información del ejecutable
file hola_mundo
# Salida: hola_mundo: ELF 64-bit LSB executable, x86-64, version 1...

# Ver dependencias de librerías
ldd hola_mundo
# Salida: librerías enlazadas dinámicamente

# Desensamblar (ver código máquina)
objdump -d hola_mundo | less
```

---

## 2. Estructuras de Datos - Explicación Detallada

### Tipos de datos fundamentales

#### Tamaños y límites

```c
// tamaños_datos.c
#include <stdio.h>
#include <limits.h>   // Límites para enteros
#include <float.h>    // Límites para flotantes

int main() {
    printf("=== TAMAÑOS DE TIPOS DE DATOS ===\n");
    printf("char: %lu bytes\n", sizeof(char));
    printf("short: %lu bytes\n", sizeof(short));
    printf("int: %lu bytes\n", sizeof(int));
    printf("long: %lu bytes\n", sizeof(long));
    printf("long long: %lu bytes\n", sizeof(long long));
    printf("float: %lu bytes\n", sizeof(float));
    printf("double: %lu bytes\n", sizeof(double));
    printf("long double: %lu bytes\n\n", sizeof(long double));
    
    printf("=== LÍMITES NUMÉRICOS ===\n");
    printf("CHAR_MAX: %d\n", CHAR_MAX);
    printf("INT_MAX: %d\n", INT_MAX);
    printf("UINT_MAX: %u\n", UINT_MAX);
    printf("FLT_MAX: %e\n", FLT_MAX);
    printf("DBL_MAX: %e\n\n", DBL_MAX);
    
    // Modificadores de tipo
    signed int si = -10;     // Con signo (default)
    unsigned int ui = 10;    // Sin signo (solo positivos)
    const int ci = 100;      // Constante (no modificable)
    volatile int vi = 200;   // Volátil (el compilador no optimiza)
    
    return 0;
}
```

#### Explicación de tipos

```c
// tipos_explicados.c
#include <stdio.h>

int main() {
    /*
     * CHAR (1 byte = 8 bits)
     * Rango: -128 a 127 (signed) o 0 a 255 (unsigned)
     * En memoria: 'A' = 65 = 01000001
     */
    char c = 'A';  // ASCII: 65
    
    /*
     * ENTEROS: Representación en complemento a 2
     * Ejemplo (8 bits):
     * 5  = 00000101
     * -5 = 11111011 (complemento a 2)
     */
    int positivo = 5;
    int negativo = -5;
    
    /*
     * PUNTO FLOTANTE: IEEE 754
     * float (32 bits): 1 bit signo + 8 bits exponente + 23 bits mantisa
     * double (64 bits): 1 bit signo + 11 bits exponente + 52 bits mantisa
     */
    float f = 3.14159f;  // 'f' indica float literal
    double d = 3.141592653589793;
    
    /*
     * BOOLEANOS EN C
     * C no tiene tipo bool nativo hasta C99
     * Se usa: 0 = false, cualquier otro valor = true
     */
    int verdadero = 1;
    int falso = 0;
    
    // Desde C99 (necesita #include <stdbool.h>)
    #include <stdbool.h>
    bool b = true;  // real tipo bool
    
    return 0;
}
```

### Arrays - Representación en memoria

```c
// arrays_memoria.c
#include <stdio.h>

int main() {
    int arr[5] = {10, 20, 30, 40, 50};
    
    /*
     * REPRESENTACIÓN EN MEMORIA:
     * 
     * Dirección  | Valor | Índice
     * ---------------------------
     * 0x1000     | 10    | arr[0]
     * 0x1004     | 20    | arr[1]
     * 0x1008     | 30    | arr[2]
     * 0x100C     | 40    | arr[3]
     * 0x1010     | 50    | arr[4]
     * 
     * Cada int ocupa 4 bytes (depende de arquitectura)
     * Dirección de arr[i] = dirección_base + i * sizeof(int)
     */
    
    printf("Dirección de arr: %p\n", (void*)arr);
    printf("Dirección de &arr[0]: %p\n", (void*)&arr[0]);
    printf("Dirección de &arr[1]: %p\n", (void*)&arr[1]);
    printf("Dirección de &arr[2]: %p\n", (void*)&arr[2]);
    
    // Tamaño total del array
    printf("sizeof(arr): %lu bytes\n", sizeof(arr));
    printf("Número de elementos: %lu\n", sizeof(arr) / sizeof(arr[0]));
    
    /*
     * RELACIÓN ENTRE ARRAYS Y PUNTEROS
     * arr == &arr[0] (decae a puntero al primer elemento)
     * arr[i] == *(arr + i) (aritmética de punteros)
     */
    printf("arr[2] = %d\n", arr[2]);
    printf("*(arr + 2) = %d\n", *(arr + 2));
    
    return 0;
}
```

### Strings en C

```c
// strings.c
#include <stdio.h>
#include <string.h>

int main() {
    /*
     * STRINGS EN C
     * No existe tipo string, son arrays de chars terminados en '\0'
     * '\0' = NULL character = ASCII 0
     */
    
    // Formas de declarar strings
    char str1[] = "Hola";  // El compilador añade '\0' automático
    char str2[5] = {'H', 'o', 'l', 'a', '\0'};  // Manual
    char str3[10];  // Buffer más grande del necesario
    
    /*
     * str1 en memoria:
     * [0]: 'H' = 72
     * [1]: 'o' = 111
     * [2]: 'l' = 108
     * [3]: 'a' = 97
     * [4]: '\0' = 0  <- FIN DE STRING
     */
    
    printf("str1: %s\n", str1);
    printf("Longitud de str1: %lu\n", strlen(str1));
    printf("sizeof(str1): %lu (incluye \\0)\n\n", sizeof(str1));
    
    // Operaciones comunes
    strcpy(str3, str1);  // Copiar string
    strcat(str3, " Mundo");  // Concatenar
    printf("str3: %s\n", str3);
    
    // Comparación
    if(strcmp(str1, "Hola") == 0) {
        printf("str1 es igual a 'Hola'\n");
    }
    
    return 0;
}
```

### Estructuras - Alineación en memoria

```c
// estructuras_memoria.c
#include <stdio.h>
#include <stddef.h>  // Para offsetof()

struct Ejemplo {
    char c;     // 1 byte
    int i;      // 4 bytes
    short s;    // 2 bytes
    double d;   // 8 bytes
};

struct EjemploEmpaquetado {
    char c;
    int i;
    short s;
    double d;
} __attribute__((packed));  // Desactiva alineación

int main() {
    /*
     * ALINEACIÓN DE MEMORIA (Data Alignment)
     * Los procesadores acceden más eficientemente a datos alineados
     * Alineación típica: según el tamaño del tipo
     */
    
    struct Ejemplo e;
    
    printf("=== ESTRUCTURA NORMAL ===\n");
    printf("sizeof(struct Ejemplo): %lu\n", sizeof(struct Ejemplo));
    printf("offsetof(c): %lu\n", offsetof(struct Ejemplo, c));
    printf("offsetof(i): %lu\n", offsetof(struct Ejemplo, i));
    printf("offsetof(s): %lu\n", offsetof(struct Ejemplo, s));
    printf("offsetof(d): %lu\n\n", offsetof(struct Ejemplo, d));
    
    /*
     * LAYOUT EN MEMORIA (en arquitectura 64-bit):
     * 
     * Dirección | Campo | Tamaño | Relleno
     * ------------------------------------
     * 0x0000    | c     | 1      | 
     * 0x0001    | (pad) | 3      | ← Relleno para alinear i
     * 0x0004    | i     | 4      |
     * 0x0008    | s     | 2      |
     * 0x000A    | (pad) | 6      | ← Relleno para alinear d
     * 0x0010    | d     | 8      |
     *           | TOTAL | 24 bytes |
     */
    
    struct EjemploEmpaquetado ep;
    printf("=== ESTRUCTURA EMPAQUETADA ===\n");
    printf("sizeof(struct EjemploEmpaquetado): %lu\n", 
           sizeof(struct EjemploEmpaquetado));
    printf("Tamaño real sin padding: %lu bytes\n", 
           sizeof(char) + sizeof(int) + sizeof(short) + sizeof(double));
    
    return 0;
}
```

### Enumeraciones

```c
// enumeraciones.c
#include <stdio.h>

// Definición de enum
enum DiasSemana {
    LUNES,      // 0 por defecto
    MARTES,     // 1
    MIERCOLES,  // 2
    JUEVES,     // 3
    VIERNES,    // 4
    SABADO,     // 5
    DOMINGO     // 6
};

// Con valores específicos
enum Estado {
    OK = 0,
    ERROR = -1,
    TIMEOUT = 100,
    CONECTADO = 200
};

int main() {
    /*
     * ENUMERACIONES
     * Son constantes enteras con nombres
     * Mejoran legibilidad del código
     */
    
    enum DiasSemana hoy = MARTES;
    
    printf("Hoy es: %d\n", hoy);
    printf("Mañana sería: %d\n\n", hoy + 1);
    
    // Los enum son básicamente int
    printf("Tamaño de enum: %lu bytes (como int)\n", sizeof(enum DiasSemana));
    
    return 0;
}
```

---

## 3. Funciones - Explicación Detallada

### Stack Frame (Marco de Pila)

```c
// stack_frame.c
#include <stdio.h>

// Prototipo
int suma(int a, int b);

int main() {
    int x = 5;
    int y = 10;
    int resultado;
    
    resultado = suma(x, y);
    printf("%d + %d = %d\n", x, y, resultado);
    
    return 0;
}

int suma(int a, int b) {
    int c;  // Variable local
    
    c = a + b;
    
    /*
     * STACK FRAME DE suma():
     * 
     * Dirección alta
     * -----------------
     * |   ...         |
     * |---------------|
     * | valor retorno | ← Cuando suma() termina, control vuelve aquí
     * |---------------|
     * | dirección     | ← Return address
     * | retorno       |
     * |---------------|
     * | frame pointer | ← EBP/RBP (Base pointer anterior)
     * | anterior      |
     * |---------------|
     * | variable c    | ← Variables locales
     * |---------------|
     * | parámetro b   | ← Parámetros
     * |---------------|
     * | parámetro a   |
     * -----------------
     * Dirección baja
     * 
     * El stack crece hacia direcciones bajas de memoria
     */
    
    return c;
}
```

### Paso por valor vs referencia

```c
// paso_valor_referencia.c
#include <stdio.h>

// Paso por VALOR (copia)
void incrementar_valor(int x) {
    x = x + 1;
    printf("Dentro de incrementar_valor: x = %d\n", x);
    /*
     * Se trabaja con una COPIA del valor original
     * Cambios no afectan a la variable original
     */
}

// Paso por REFERENCIA (usando punteros)
void incrementar_referencia(int *x) {
    *x = *x + 1;
    printf("Dentro de incrementar_referencia: *x = %d\n", *x);
    /*
     * Recibe la DIRECCIÓN de la variable original
     * Cambios SI afectan a la variable original
     */
}

int main() {
    int a = 5;
    int b = 5;
    
    printf("=== PASO POR VALOR ===\n");
    printf("Antes: a = %d\n", a);
    incrementar_valor(a);
    printf("Después: a = %d\n\n", a);
    
    printf("=== PASO POR REFERENCIA ===\n");
    printf("Antes: b = %d\n", b);
    incrementar_referencia(&b);  // Pasa dirección de b
    printf("Después: b = %d\n", b);
    
    return 0;
}
```

### Variables locales vs globales

```c
// ambito_variables.c
#include <stdio.h>

// Variable GLOBAL (ámbito archivo)
int variable_global = 100;

// Variable estática (persiste entre llamadas)
int contador_llamadas() {
    static int contador = 0;  // Solo se inicializa una vez
    contador++;
    return contador;
}

void funcion1() {
    // Variable LOCAL a funcion1
    int local_func1 = 10;
    variable_global = 200;  // Puede modificar globales
    
    printf("funcion1: local_func1 = %d\n", local_func1);
    printf("funcion1: variable_global = %d\n", variable_global);
}

void funcion2() {
    // Variable LOCAL a funcion2
    int local_func2 = 20;
    
    // NO puede acceder a local_func1
    // printf("%d\n", local_func1);  // ERROR
    
    printf("funcion2: local_func2 = %d\n", local_func2);
    printf("funcion2: variable_global = %d\n", variable_global);
}

int main() {
    // Variable LOCAL a main
    int local_main = 30;
    
    funcion1();
    funcion2();
    
    printf("main: local_main = %d\n", local_main);
    printf("main: variable_global = %d\n\n", variable_global);
    
    // Variables estáticas
    printf("Contador llamadas:\n");
    for(int i = 0; i < 5; i++) {
        printf("Llamada %d: contador = %d\n", 
               i + 1, contador_llamadas());
    }
    
    return 0;
}
```

### Recursividad

```c
// recursividad.c
#include <stdio.h>

// Función recursiva clásica: factorial
unsigned long long factorial(int n) {
    /*
     * CASO BASE: condición de terminación
     * factorial(0) = 1
     * factorial(1) = 1
     */
    if(n <= 1) {
        return 1;
    }
    
    /*
     * CASO RECURSIVO: función se llama a sí misma
     * factorial(n) = n * factorial(n-1)
     * 
     * PILA DE LLAMADAS para factorial(4):
     * 
     * factorial(4) = 4 * factorial(3)
     *   factorial(3) = 3 * factorial(2)
     *     factorial(2) = 2 * factorial(1)
     *       factorial(1) = 1  ← CASO BASE
     *     factorial(2) = 2 * 1 = 2
     *   factorial(3) = 3 * 2 = 6
     * factorial(4) = 4 * 6 = 24
     */
    return n * factorial(n - 1);
}

// Recursividad de cola (Tail recursion)
int suma_recursiva_cola(int n, int acumulador) {
    if(n == 0) {
        return acumulador;
    }
    return suma_recursiva_cola(n - 1, acumulador + n);
    /*
     * Algunos compiladores optimizan tail recursion a iteración
     * Evita crecimiento del stack
     */
}

int main() {
    int numero = 5;
    
    printf("Factorial de %d:\n", numero);
    printf("Iterativo: ");
    
    // Versión iterativa para comparar
    unsigned long long fact_iterativo = 1;
    for(int i = 1; i <= numero; i++) {
        fact_iterativo *= i;
    }
    printf("%llu\n", fact_iterativo);
    
    printf("Recursivo: %llu\n\n", factorial(numero));
    
    printf("Suma de 1 a %d (recursivo cola): %d\n", 
           numero, suma_recursiva_cola(numero, 0));
    
    return 0;
}
```

### Punteros a funciones

```c
// punteros_funciones_detalle.c
#include <stdio.h>
#include <math.h>

// Definir tipo para puntero a función
typedef double (*FuncionMatematica)(double);

double cuadrado(double x) {
    return x * x;
}

double cubo(double x) {
    return x * x * x;
}

// Función que recibe otra función como parámetro
double aplicar_funcion(FuncionMatematica f, double x) {
    return f(x);
}

int main() {
    /*
     * PUNTEROS A FUNCIONES
     * Almacenan la dirección donde comienza el código de una función
     * Útiles para:
     * - Tablas de despacho
     * - Callbacks
     * - Implementar polimorfismo en C
     */
    
    // Declaración
    double (*pfunc)(double);
    
    // Asignación
    pfunc = &cuadrado;  // El & es opcional
    printf("cuadrado(5) = %.2f\n", pfunc(5.0));
    
    pfunc = cubo;
    printf("cubo(5) = %.2f\n", pfunc(5.0));
    
    // Uso con typedef
    FuncionMatematica funciones[] = {sin, cos, tan, cuadrado, cubo};
    char* nombres[] = {"seno", "coseno", "tangente", "cuadrado", "cubo"};
    
    double valor = 1.0;
    printf("\nAplicando funciones a %.2f:\n", valor);
    for(int i = 0; i < 5; i++) {
        printf("%s(%.2f) = %.4f\n", 
               nombres[i], valor, aplicar_funcion(funciones[i], valor));
    }
    
    return 0;
}
```

---

## 4. Memoria y Apuntadores - Explicación Detallada

### Segmentos de memoria en C

```c
// segmentos_memoria.c
#include <stdio.h>
#include <stdlib.h>

// Variable global (data segment)
int global_var = 100;
// Variable global no inicializada (bss segment)
int global_var_sin_inicializar;

void funcion() {
    // Variable local (stack segment)
    int local_var = 50;
    
    // Variable estática (data segment, pero ámbito local)
    static int static_local = 0;
    static_local++;
    
    printf("local_var: %d (stack)\n", local_var);
    printf("static_local: %d (data segment)\n", static_local);
    
    // Memoria dinámica (heap segment)
    int* heap_var = (int*)malloc(sizeof(int));
    *heap_var = 200;
    
    printf("heap_var: %p -> %d (heap)\n", (void*)heap_var, *heap_var);
    
    free(heap_var);  // IMPORTANTE liberar!
}

int main() {
    /*
     * SEGMENTOS DE MEMORIA DE UN PROCESO:
     * 
     * Dirección alta
     * -----------------
     * | Kernel        |
     * |---------------|
     * | Stack         | ← Variables locales, crece hacia abajo
     * | ↓             |
     * |               |
     * |               |
     * | ↑             |
     * | Heap          | ← malloc(), crece hacia arriba
     * |---------------|
     * | BSS           | ← Variables globales sin inicializar
     * |---------------|
     * | Data          | ← Variables globales inicializadas
     * |---------------|
     * | Text/Code     | ← Código ejecutable (solo lectura)
     * -----------------
     * Dirección baja
     */
    
    printf("=== DIRECCIONES DE MEMORIA ===\n");
    printf("main (función): %p (text segment)\n", (void*)main);
    printf("global_var: %p (data segment)\n", (void*)&global_var);
    printf("global_var_sin_inicializar: %p (bss segment)\n", 
           (void*)&global_var_sin_inicializar);
    
    funcion();
    funcion();  // static_local mantiene su valor
    
    return 0;
}
```

### Punteros - Conceptos fundamentales

```c
// punteros_fundamentos.c
#include <stdio.h>

int main() {
    /*
     * PUNTEROS: Variables que almacenan direcciones de memoria
     * 
     * Declaración: tipo* nombre;
     * 
     * Operadores:
     * &  : Dirección de (address-of)
     * *  : Desreferencia (dereference)
     * -> : Acceso a miembro a través de puntero
     */
    
    int x = 42;
    int* p = &x;  // p apunta a x
    
    printf("=== CONCEPTOS BÁSICOS ===\n");
    printf("Valor de x: %d\n", x);
    printf("Dirección de x: %p\n", (void*)&x);
    printf("Valor de p (dirección almacenada): %p\n", (void*)p);
    printf("Dirección de p: %p\n", (void*)&p);
    printf("Valor al que apunta p: %d\n\n", *p);
    
    /*
     * TAMAÑO DE PUNTEROS
     * Todos los punteros tienen el mismo tamaño en una arquitectura
     * porque solo almacenan direcciones de memoria
     */
    printf("=== TAMAÑOS ===\n");
    printf("sizeof(char*): %lu bytes\n", sizeof(char*));
    printf("sizeof(int*): %lu bytes\n", sizeof(int*));
    printf("sizeof(double*): %lu bytes\n", sizeof(double*));
    printf("sizeof(void*): %lu bytes\n\n", sizeof(void*));
    
    /*
     * ARITMÉTICA DE PUNTEROS
     * p + n = dirección + n * sizeof(tipo)
     */
    int arr[3] = {10, 20, 30};
    int* ptr = arr;
    
    printf("=== ARITMÉTICA ===\n");
    printf("ptr = %p -> arr[0] = %d\n", (void*)ptr, *ptr);
    printf("ptr + 1 = %p -> arr[1] = %d\n", (void*)(ptr + 1), *(ptr + 1));
    printf("ptr + 2 = %p -> arr[2] = %d\n", (void*)(ptr + 2), *(ptr + 2));
    
    // Diferencia entre punteros
    int* p1 = &arr[0];
    int* p2 = &arr[2];
    printf("p2 - p1 = %ld elementos\n", p2 - p1);
    
    return 0;
}
```

### Punteros y arrays multidimensionales

```c
// punteros_arrays_multidimensionales.c
#include <stdio.h>

int main() {
    /*
     * ARRAY BIDIMENSIONAL
     * En memoria es contiguo, no hay "filas" separadas
     */
    int matriz[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };
    
    printf("=== ARRAY BIDIMENSIONAL ===\n");
    printf("matriz[0][0]: %d\n", matriz[0][0]);
    printf("matriz[1][2]: %d\n\n", matriz[1][2]);
    
    printf("Dirección de matriz: %p\n", (void*)matriz);
    printf("Dirección de matriz[0]: %p\n", (void*)matriz[0]);
    printf("Dirección de &matriz[0][0]: %p\n", (void*)&matriz[0][0]);
    printf("Dirección de matriz[1]: %p\n", (void*)matriz[1]);
    printf("Dirección de &matriz[1][0]: %p\n\n", (void*)&matriz[1][0]);
    
    /*
     * PUNTERO A ARRAY
     * int (*p)[3] = matriz;  // Puntero a array de 3 ints
     */
    int (*p)[3] = matriz;
    
    printf("Acceso con puntero a array:\n");
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 3; j++) {
            printf("p[%d][%d] = %d\t", i, j, p[i][j]);
        }
        printf("\n");
    }
    
    /*
     * ARRAY DE PUNTEROS (no lo mismo que array bidimensional)
     */
    int a = 1, b = 2, c = 3;
    int* array_punteros[3] = {&a, &b, &c};
    
    printf("\n=== ARRAY DE PUNTEROS ===\n");
    for(int i = 0; i < 3; i++) {
        printf("array_punteros[%d] = %p -> %d\n", 
               i, (void*)array_punteros[i], *array_punteros[i]);
    }
    
    return 0;
}
```

### Gestión de memoria dinámica

```c
// memoria_dinamica_detalle.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    /*
     * FUNCIONES DE GESTIÓN DE MEMORIA DINÁMICA:
     * malloc(): Memory ALLOCation - reserva memoria sin inicializar
     * calloc(): Contiguous ALLOCation - reserva e inicializa a 0
     * realloc(): REALLOCation - cambia tamaño de bloque existente
     * free(): libera memoria
     */
    
    printf("=== MALLOC() ===\n");
    // malloc(size_t size): tamaño en bytes
    int* p1 = (int*)malloc(5 * sizeof(int));
    
    if(p1 == NULL) {
        printf("Error: no se pudo reservar memoria\n");
        return 1;  // Código de error
    }
    
    // malloc NO inicializa la memoria (contenido basura)
    printf("malloc - memoria sin inicializar:\n");
    for(int i = 0; i < 5; i++) {
        printf("p1[%d] = %d (basura)\n", i, p1[i]);
    }
    
    // Inicializar manualmente
    for(int i = 0; i < 5; i++) {
        p1[i] = i * 10;
    }
    
    printf("\n=== CALLOC() ===\n");
    // calloc(size_t num, size_t size): num elementos, tamaño de cada uno
    int* p2 = (int*)calloc(5, sizeof(int));
    
    if(p2 == NULL) {
        free(p1);  // IMPORTANTE: liberar antes de salir
        return 1;
    }
    
    // calloc SI inicializa a 0
    printf("calloc - memoria inicializada a 0:\n");
    for(int i = 0; i < 5; i++) {
        printf("p2[%d] = %d\n", i, p2[i]);
    }
    
    printf("\n=== REALLOC() ===\n");
    // Agrandar el array p1
    int* temp = (int*)realloc(p1, 10 * sizeof(int));
    
    if(temp != NULL) {
        p1 = temp;  // Usar nuevo puntero
        printf("realloc exitoso\n");
        
        // Los primeros 5 elementos se mantienen
        for(int i = 0; i < 10; i++) {
            printf("p1[%d] = %d\n", i, p1[i]);
        }
    }
    
    printf("\n=== GESTIÓN DE STRINGS DINÁMICAS ===\n");
    char* str = (char*)malloc(50 * sizeof(char));
    
    if(str != NULL) {
        strcpy(str, "Hola mundo dinámico");
        printf("String dinámico: %s\n", str);
        
        // Redimensionar string
        str = (char*)realloc(str, 100 * sizeof(char));
        strcat(str, " - ahora es más largo");
        printf("String redimensionado: %s\n", str);
        
        free(str);
    }
    
    // LIBERAR TODA LA MEMORIA
    free(p1);
    free(p2);
    
    printf("\n=== BUENAS PRÁCTICAS ===\n");
    printf("1. Verificar siempre retorno de malloc/calloc/realloc\n");
    printf("2. Inicializar memoria después de malloc()\n");
    printf("3. Usar free() por cada malloc()\n");
    printf("4. Asignar NULL después de free()\n");
    printf("5. No acceder memoria después de free()\n");
    
    return 0;
}
```

### Punteros dobles y estructuras complejas

```c
// punteros_dobles.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nombre[50];
    int edad;
} Persona;

int main() {
    /*
     * PUNTEROS DOBLES: puntero a puntero
     * Útiles para:
     * - Matrices dinámicas
     * - Modificar punteros en funciones
     * - Listas de punteros
     */
    
    printf("=== PUNTERO A PUNTERO ===\n");
    int x = 100;
    int* p = &x;
    int** pp = &p;  // pp apunta a p, que apunta a x
    
    printf("x = %d\n", x);
    printf("*p = %d\n", *p);
    printf("**pp = %d\n\n", **pp);
    
    printf("=== MATRIZ DINÁMICA 2D ===\n");
    int filas = 3, columnas = 4;
    
    // Reservar array de punteros a filas
    int** matriz = (int**)malloc(filas * sizeof(int*));
    
    // Reservar cada fila
    for(int i = 0; i < filas; i++) {
        matriz[i] = (int*)malloc(columnas * sizeof(int));
        
        // Inicializar
        for(int j = 0; j < columnas; j++) {
            matriz[i][j] = i * 10 + j;
        }
    }
    
    // Imprimir matriz
    for(int i = 0; i < filas; i++) {
        for(int j = 0; j < columnas; j++) {
            printf("%3d ", matriz[i][j]);
        }
        printf
