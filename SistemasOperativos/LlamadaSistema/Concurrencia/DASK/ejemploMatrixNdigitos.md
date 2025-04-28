# Tutorial: Programación Concurrente con Dask en Python

## Introducción
Dask es una biblioteca de Python que permite realizar cálculos paralelos y distribuidos de manera eficiente. Es ideal para manejar grandes cantidades de datos o tareas computacionalmente intensivas, aprovechando múltiples núcleos o clústeres.

## Paso a Paso para Crear Programas Concurrentes con Dask

### 1. **Instalación de Dask**
Instala Dask y sus dependencias necesarias:
```bash
pip install dask numpy dask[distributed]
```

### 2. **Importar Dask**
Dask ofrece varias herramientas útiles:
- **`dask.delayed`**: Para paralelizar funciones personalizadas.
- **`dask.array`**: Para trabajar con matrices grandes en paralelo.
- **`dask.distributed`**: Para gestionar clústeres de cálculo.

Ejemplo de importación:
```python
from dask import delayed, compute
import dask.array as da
from dask.distributed import Client
```

### 3. **Configurar un Clúster Dask**
Inicia un clúster local para distribuir las tareas entre varios núcleos:
```python
cliente = Client(n_workers=4, threads_per_worker=2)
```

### 4. **Usar `dask.delayed` para Funciones Personalizadas**
La función `delayed` pospone la ejecución de una tarea hasta que se llame `compute`. Es útil para paralelizar operaciones específicas.

Ejemplo (multiplicación de números grandes):
```python
@delayed
def multiplicar(a, b):
    return a * b

tarea = multiplicar(a, b)
resultado = compute(tarea)[0]
```

### 5. **Usar `dask.array` para Matrices**
`dask.array` divide matrices en fragmentos ("chunks") para procesarlos en paralelo. Ajusta el tamaño de los fragmentos según tu sistema.

Ejemplo:
```python
a = da.from_array(np.random.rand(1000, 1000), chunks=(250, 250))
b = da.from_array(np.random.rand(1000, 1000), chunks=(250, 250))
resultado = da.dot(a, b).compute()
```

### 6. **Manejo de Argumentos por Línea de Comandos**
Usa `argparse` para recibir parámetros y decidir qué operaciones ejecutar:
- `--digits`: Ejecuta la multiplicación de números grandes.
- `--matrix_size`: Ejecuta la multiplicación de matrices.
- Ambos: Ejecuta ambas operaciones.

Ejemplo:
```python
parser = argparse.ArgumentParser(description="Comparación de cálculos con Dask")
parser.add_argument('--digits', type=int, default=None, help='Cantidad de dígitos')
parser.add_argument('--matrix_size', type=int, default=None, help='Tamaño de matriz')
args = parser.parse_args()
```

### 7. **Medición Precisa del Tiempo**
Usa `time.perf_counter()` para medir el tiempo con mayor precisión. Para operaciones rápidas (como la multiplicación de números grandes), el tiempo puede ser muy pequeño (por ejemplo, < 0.0001 segundos).

### 8. **Ejecutar el Programa**
Guarda el código en un archivo (por ejemplo, `program.py`) y ejecútalo con una de las siguientes opciones:
- Solo números grandes:
  ```bash
  python program.py --digits 100
  ```
- Solo matrices:
  ```bash
  python program.py --matrix_size 1000
  ```
- Ambas operaciones:
  ```bash
  python program.py --digits 100 --matrix_size 1000
  ```

### 9. **Interpretación de los Comentarios en el Código**
El código incluye comentarios detallados que explican:
- **Propósito**: Qué hace cada función o sección (por ejemplo, multiplicar números grandes o iniciar un clúster Dask).
- **Uso de Dask**: Dónde se usan `delayed`, `compute`, o `dask.array`, y cómo contribuyen al paralelismo.
- **Expectativas**: Qué resultados se esperan (por ejemplo, resultados idénticos entre secuencial y paralelo, tiempos de ejecución).
- **Entradas y salidas**: Qué parámetros recibe cada función y qué devuelve.

### 10. **Verificación de Resultados**
El código verifica que los resultados secuenciales y paralelos sean idénticos:
- **Números grandes**: Compara los resultados exactos con `==`. Si no coinciden, muestra ambos valores para depuración.
- **Matrices**: Usa `np.allclose` para permitir pequeñas diferencias numéricas debido a la aritmética de punto flotante. Si no coinciden, muestra los primeros elementos.
- Los mensajes de error son claros y muestran los valores discrepantes, facilitando la identificación de problemas.

## Comparación: Secuencial vs. Paralelo con Dask

### Caso 1: Multiplicación de Números Grandes
- **Secuencial**: Multiplica dos números grandes directamente en una sola operación.
- **Paralelo (Dask)**: Usa `dask.delayed` para realizar la misma multiplicación en paralelo. Para números de 100 dígitos, el overhead de Dask suele hacer que sea más lento.

**Nota**: El tiempo secuencial puede ser muy pequeño (< 0.0001 segundos) debido a la rapidez de la operación. El paralelismo no es eficiente para esta tarea debido al costo de coordinar tareas.

### Caso 2: Multiplicación de Matrices
- **Secuencial**: Usa `numpy.dot` para multiplicar matrices.
- **Paralelo (Dask)**: Divide las matrices en fragmentos y distribuye los cálculos entre varios núcleos. Esto es más rápido para matrices grandes.

**Nota**: Dask es eficiente para matrices grandes, aprovechando múltiples núcleos. Ajusta los fragmentos y trabajadores según tu hardware.

## Por Qué lo Secuencial Puede Ser Más Rápido
- **Costo de Dask**: Crear y coordinar tareas paralelas añade un retraso. En operaciones simples (como multiplicar números), esto supera el tiempo de cálculo.
- **Tamaño de la tarea**: Para números de 100 dígitos, la multiplicación es demasiado rápida para beneficiarse del paralelismo.
- **Solución**: Usa Dask para tareas más complejas o datos más grandes, como matrices grandes.

## Formato de Resultados
- **Números grandes**: Si el resultado tiene más de 50 dígitos, se muestran los primeros 20 y los últimos 20 dígitos, junto con la longitud total. De lo contrario, se muestra completo.
- **Matrices**: Se muestra el primer elemento de la matriz resultante para verificar la corrección.
- **Verificación**: Los resultados se comparan para confirmar que son idénticos, con mensajes detallados si hay discrepancias.

## Conclusión
- **Dask** es ideal para cálculos intensivos, como la multiplicación de matrices, donde el paralelismo reduce el tiempo significativamente.
- Para tareas simples, como multiplicar números grandes de 100 dígitos, el costo de Dask hace que lo secuencial sea más rápido.
- Usa los argumentos `--digits` y `--matrix_size` para ejecutar solo las operaciones deseadas.
- Los comentarios en el código explican cada paso, el uso de Dask, y las expectativas.
- Ajusta el número de trabajadores y tamaños de fragmentos según tu equipo para mejores resultados.
