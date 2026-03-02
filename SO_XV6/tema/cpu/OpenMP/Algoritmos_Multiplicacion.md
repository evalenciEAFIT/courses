# Algoritmos de Multiplicación: Matrices y Números Grandes

Este documento explica de forma detallada dos algoritmos fundamentales en ciencias de la computación: la multiplicación de matrices y la multiplicación de números excepcionalmente grandes (BigInt).

---

## 1. Multiplicación de Matrices

La multiplicación de matrices es una operación lineal que combina dos matrices para producir una tercera.

### Algoritmo Básico (Clásico)

Dadas dos matrices:
- **Matriz A** de dimensiones `M x N` (M filas y N columnas)
- **Matriz B** de dimensiones `N x P` (N filas y P columnas)

Para que se puedan multiplicar, el número de columnas de **A** debe ser igual al número de filas de **B** (ambos son `N`). El resultado será una **Matriz C** de dimensiones `M x P`.

**Fórmula Matemática:**
Cada elemento de la matriz resultante `C` en la posición `(i, j)` se calcula mediante la suma del producto de los elementos de la fila `i` de la matriz `A` por los elementos de la columna `j` de la matriz `B`:

$$C_{i,j} = \sum_{k=0}^{N-1} (A_{i,k} \times B_{k,j})$$

### Implementación (Pseudocódigo en C)

El algoritmo requiere de tres bucles anidados (`for`):

```c
// Inicializar la matriz C en 0
for (int i = 0; i < M; i++) {           // Recorre las filas de A
    for (int j = 0; j < P; j++) {       // Recorre las columnas de B
        C[i][j] = 0;
        for (int k = 0; k < N; k++) {   // Recorre los elementos para multiplicar y sumar
            C[i][j] += A[i][k] * B[k][j];
        }
    }
}
```

### Características Importantes
- **Complejidad Temporal:** $O(M \times P \times N)$. Si son matrices cuadradas de tamaño $N$, la complejidad es de **$O(N^3)$**.
- **Acceso a Memoria:** El bucle más interno accede a la Matriz B usando la memoria saltando por filas enteras (debido al manejo del índice `[k][j]`), lo cual carece de localidad espacial causando constantes fallos de caché ("cache misses"). En implementaciones de alto rendimiento se transpone la matriz B primero o se usan técnicas de bloqueo de caché (Cache Tiling).

---

## 2. Multiplicación de Números Gigantes (Más de 100 dígitos)

Los tipos de datos numéricos predeterminados en un lenguaje como C (`int`, `unsigned long long`) manejan datos hasta la capacidad del hardware, el cual suele ser de 64-bits. Este tamaño solo puede almacenar valores que tienen hasta unos **20 dígitos decimales**. 

Para representar y procesar números de más de 100 dígitos, es necesario usar una estructura tipo **BigInt**: donde cada dígito del número se guarda de manera independiente dentro de un **vector (array)** o como cadena de texto.

### Algoritmo de Multiplicación Tradicional (Grado Escolar)

Este algoritmo se basa en emular en código la técnica clásica de la "multiplicación a mano" en papel.

1. **Representación invertida:** El número se almacena con el dígito de menor peso (unidades) en el índice `0`. Así se facilita manejar números de distintos tamaños sin complicar el índice de memoria al hacer la suma.
2. **Multiplicación y almacenamiento de acarreos:** Se multiplica cada dígito de `A` iterando contra los dígitos de `B`.
3. **Índice coincidente:** El producto del `i`-ésimo dígito del número 1 y el `j`-ésimo dígito del número 2 recae (y se suma) exactamente en la posición de índice `(i + j)` del arreglo respuesta.

### Implementación (Pseudocódigo en C)
Considerando que `num1` y `num2` son arreglos que guardan los dígitos, y que sus longitudes son `len1` y `len2`. El resultado a lo mucho contendrá el tamaño de `len1 + len2`.

```c
// Array para almacenar el resultado, pre-llenado en ceros
int resultado[len1 + len2] = {0};

for (int i = 0; i < len1; i++) {
    int acarreo = 0;
    
    for (int j = 0; j < len2; j++) {
        // Multiplicar dígitos + acarreos + el valor previo asimilado en esta posición
        int suma = resultado[i + j] + (num1[i] * num2[j]) + acarreo;
        
        resultado[i + j] = suma % 10;     // Quedarse con un solo dígito decimal en la posición actual
        acarreo = suma / 10;              // Separar la decena obtenida como "acarreo" (carry)
    }
    
    // Si terminamos de multiplicar por todos los iteradores posicionales (j) de num2, 
    // y sigue sobrando acarreo, este se deposita en la siguiente posición correspondiente:
    if (acarreo > 0) {
        resultado[i + len2] += acarreo;
    }
}
```

### Características Importantes
- **Complejidad Temporal:** Como se requieren dos iteraciones completas sobre los tamaños de los números, la complejidad radica en $O(N \times M)$ (donde `N` y `M` son el número total de cifras).
- **Complejidad Espacial:** Ligeramente por encima de $O(N + M)$ para definir la memoria donde residirá el resultado.
- **Enfoques avanzados:** Cuando el número excede miles o millones de cifras, $O(N^2)$ resulta excesivamente lento; ahí se opta por métodos como el **Algoritmo de Karatsuba** $O(N^{1.58})$ o transformaciones matemáticas como la **FFT** de Schönhage–Strassen con ritmos de $O(N \log N \log \log N)$.

---

## 3. Algoritmo de Karatsuba (Para Grandes Números)

El algoritmo de Karatsuba es un método eficiente de multiplicación rápida (*Fast Multiplication algorithm*) introducido en 1960. 
Utiliza el paradigma de **Divide y Vencerás** (*Divide and Conquer*), siendo mucho más rápido que el método tradicional $O(N^2)$ al aplicarse en números enormes (miles de dígitos). 

### Concepto y Lógica Matemática
Si tenemos dos números muy grandes, $X$ e $Y$, con $n$ dígitos, Karatsuba los divide por la mitad (por ejemplo, en base $B = 10$ o una base binaria):
$$X = x_1 \cdot B^m + x_0$$
$$Y = y_1 \cdot B^m + y_0$$
donde $m \approx n/2$. Los valores $x_1$ e $y_1$ son las mitades más significativas y $x_0, y_0$ las mitades menos significativas.

El producto algebraico normal requiere de $4$ multiplicaciones:
$$X \times Y = (x_1 y_1) B^{2m} + (x_1 y_0 + x_0 y_1) B^m + (x_0 y_0)$$

Karatsuba optimiza esto calculando solo **3 multiplicaciones**:
1. $z_2 = x_1 \times y_1$
2. $z_0 = x_0 \times y_0$
3. $z_1 = (x_1 + x_0) \times (y_1 + y_0)$

El término cruzado intermedio se obtiene restando en lugar de multiplicar de más:
$$(x_1 y_0 + x_0 y_1) = z_1 - z_2 - z_0$$

Y la ecuación final se ensambla con desplazamientos (añadiendo ceros al final, o "shifteando"):
$$X \times Y = z_2 B^{2m} + (z_1 - z_2 - z_0) B^m + z_0$$

### Implementación (Pseudocódigo Recursivo)
A un alto nivel, imaginando que usamos una estructura como `BigInt`:

```c
BigInt karatsuba(BigInt num1, BigInt num2) {
    // 1. Caso Base: Si los números son suficientemente pequeños, usamos el tradicional
    if (num1.length <= UMBRAL || num2.length <= UMBRAL) {
        return multiplicacion_tradicional(num1, num2);
    }

    // 2. Extraer el tamaño 'm' (mitad de la longitud máxima)
    int m = max(num1.length, num2.length) / 2;

    // 3. Dividir num1 y num2 en mitades superior e inferior
    BigInt x1 = dividir_sup(num1, m); 
    BigInt x0 = dividir_inf(num1, m);
    BigInt y1 = dividir_sup(num2, m); 
    BigInt y0 = dividir_inf(num2, m);

    // 4. Calcular los 3 productos recursivos
    BigInt z2 = karatsuba(x1, y1);
    BigInt z0 = karatsuba(x0, y0);
    
    // z1 = karatsuba( (x1+x0), (y1+y0) )
    BigInt sumaX = sumar_bigint(x1, x0);
    BigInt sumaY = sumar_bigint(y1, y0);
    BigInt z1 = karatsuba(sumaX, sumaY);

    // 5. Calcular término cruzado = z1 - z2 - z0
    BigInt pasoRestar1 = restar_bigint(z1, z2);
    BigInt termino_cruzado = restar_bigint(pasoRestar1, z0);

    // 6. Ensamblar desplazando las posiciones (multiplicando por B^m y B^2m)
    BigInt termino_z2 = shift_left(z2, 2 * m);         // Z2 * B^2m
    BigInt termino_z1 = shift_left(termino_cruzado, m); // crossterm * B^m
    
    // Resultado = (Z2 * B^2m) + (crossterm * B^m) + Z0
    BigInt tempSuma = sumar_bigint(termino_z2, termino_z1);
    return sumar_bigint(tempSuma, z0);
}
```

### Características Importantes
- **Complejidad Temporal:** Dado que el algoritmo convierte 4 llamadas sub-recursivas en solo 3, su relación de recurrencia es $T(N) = 3T(N/2) + O(N)$. Según el Teorema Maestro, la complejidad temporal se reduce de $O(N^2)$ a **$O(N^{\log_2 3}) \approx O(N^{1.585})$**.
- **Comportamiento en la Práctica:** Para números con pocos dígitos (generalmente por debajo de un `UMBRAL` o *Threshold* entre 50 a 100 dígitos), se sigue utilizando el algoritmo en $O(N^2)$. Esto se debe a que la creación de memoria adicional temporal que se requiere para las recursiones y las operaciones de suma/resta tienen un alto costo indirecto ("overhead"). Se debe hallar ese umbral de optimización.
- **Complejidad Espacial:** Dado que se asignan variables extra de memoria a cada nivel recursivo, su consumo de espacio varía entre $O(N \log N)$ a $O(N)$ (dependiendo de si se pueden reusar variables/arreglos localmente).
