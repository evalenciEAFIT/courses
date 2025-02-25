# Multiplicación de Matrices y Gestión de Memoria en C++

Este documento explica en detalle el funcionamiento del programa de **multiplicación de matrices** en C++, destacando el uso de **memoria** en Linux y la diferencia entre **float** y **double** en cálculos de punto flotante.

## Descripción del Programa

El programa realiza las siguientes acciones:
1. **Medición del uso de memoria**: Mide el uso de **RAM**, **heap**, **stack** y **swap**.
2. **Multiplicación de matrices con float y double**:
   - `float` (32 bits, precisión simple)
   - `double` (64 bits, precisión doble)
3. **Monitoreo de operaciones realizadas**.
4. **Visualización de memoria antes y después de la ejecución**.
5. **Uso de barra de progreso** que desaparece al finalizar la ejecución.

---
## Definición de Conceptos de Memoria

| Término         | Descripción |
|-----------------|-------------|
| **RSS (Resident Set Size)** | Memoria física (RAM) utilizada por el proceso en KB. |
| **Heap Arena** | Memoria reservada por el proceso para el heap, en KB. |
| **Heap Usado** | Memoria actualmente utilizada en el heap, en KB. |
| **Stack Usado** | Memoria utilizada en la pila (stack), en KB. |
| **Swap Usado** | Memoria en disco usada cuando la RAM es insuficiente, en KB. |

---
## Precisión de Punto Flotante

El programa compara el rendimiento y la precisión de `float` y `double`.

| Tipo   | Tamaño (bits) | Precisión (dígitos significativos) | Costo Computacional |
|--------|--------------|---------------------------------|------------------|
| **float**  | 32 bits      | ~7 dígitos                     | Bajo             |
| **double** | 64 bits      | ~15 dígitos                    | Alto             |

---
## Explicación del Código

### 1. Medición del Uso de Memoria

```cpp
void medirUsoMemoria(const string& tipo) {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    struct mallinfo2 mi = mallinfo2();

    cout << "\n\n\U0001F504 MEDICIÓN DE MEMORIA (" << tipo << ") \U0001F504\n";
    cout << " - RSS: " << usage.ru_maxrss << " KB" << endl;
    cout << " - Heap Arena: " << mi.arena / 1024 << " KB" << endl;
    cout << " - Heap Usado: " << mi.uordblks / 1024 << " KB" << endl;
    cout << " - Stack Usado: " << usage.ru_isrss << " KB" << endl;
    cout << " - Swap Usado: " << obtenerUsoSwap() << " KB" << endl;
}
```

---
### 2. Multiplicación de Matrices con float y double

Cada operación de multiplicación se realiza en **tres bucles anidados** y se mide el tiempo de ejecución.

```cpp
ResultadoMultiplicacion multiplicarMatrizFloat(int n, int m) {
    vector<vector<float>> A(n, vector<float>(m, 1.5f));
    vector<vector<float>> B(m, vector<float>(n, 2.5f));
    vector<vector<float>> resultado(n, vector<float>(m, 0.0f));

    auto inicio = high_resolution_clock::now();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < m; k++) {
                resultado[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    auto fin = high_resolution_clock::now();

    return {n * m * m, duration_cast<milliseconds>(fin - inicio).count()};
}
```

Lo mismo se aplica para `double`, solo cambiando los tipos de datos.

---
## 3. Barra de Progreso Dinámica

Para una mejor experiencia de usuario, se muestra una **barra de progreso** que desaparece al finalizar la ejecución.

```cpp
void mostrarBarraProgreso(long long actual, long long total, bool borrar = false) {
    int ancho_barra = 50;
    int progreso = (actual * ancho_barra) / total;

    cout << "\r[";
    for (int i = 0; i < progreso; i++) cout << "#";
    for (int i = progreso; i < ancho_barra; i++) cout << "-";
    cout << "] " << (actual * 100) / total << "%";
    cout.flush();

    if (borrar && actual == total) {
        usleep(200000);
        cout << "\033[F\033[K\033[F\033[K";
    }
}
```

---
## **Conclusión**

- El programa ilustra el uso de memoria en **Linux**.
- Se compara el rendimiento entre `float` y `double`.
- Se observa la diferencia en **costo computacional y precisión**.
- Se implementa una **barra de progreso dinámica** para una mejor visualización.

Este código es una **excelente herramienta educativa** para entender la **gestión de memoria y la eficiencia de cálculo en C++**.

---

