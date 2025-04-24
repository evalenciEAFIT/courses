# **Guía Práctica para la Computación Paralela con Dask**

## **Sección 1: Introducción a Dask: Escalando sus Cargas de Trabajo en Python**

### **1.1 ¿Qué es Dask? Más Allá del Python de un Solo Núcleo**

Dask es una biblioteca de código abierto y flexible para Python, diseñada específicamente para la computación paralela y distribuida.1 Su propósito fundamental es permitir a los desarrolladores y científicos de datos escalar las herramientas familiares del ecosistema Python, como NumPy, Pandas y Scikit-learn, así como código Python personalizado, para manejar conjuntos de datos que exceden la memoria RAM disponible ("larger-than-memory") y para acelerar tareas computacionalmente intensivas.1 A medida que los conjuntos de datos crecen en volumen y las computaciones se vuelven más complejas, las herramientas estándar de Python que operan en un solo núcleo de procesador alcanzan sus límites. Dask surge como una solución para superar estas limitaciones, permitiendo el procesamiento de datos que no caben en la memoria al extenderlos al disco si es necesario y utilizando eficientemente múltiples núcleos de CPU en una sola máquina o distribuyendo la carga de trabajo a través de múltiples máquinas en un clúster.1

### **1.2 Conceptos Centrales: Cómo Funciona Dask**

Dask se compone internamente de dos partes principales que trabajan conjuntamente para habilitar la computación paralela 2:

* **Programación Dinámica de Tareas (Task Scheduling):** En su núcleo, Dask posee un programador de tareas dinámico optimizado para cargas de trabajo computacionales, especialmente aquellas de naturaleza interactiva, comunes en el análisis de datos.8 Este programador toma algoritmos complejos y los descompone en muchas tareas pequeñas. Luego, gestiona la ejecución de estas tareas, manejando sus dependencias y asignándolas a los recursos computacionales disponibles (hilos, procesos o trabajadores en un clúster) de manera eficiente.6 Aunque comparte similitudes conceptuales con herramientas de flujo de trabajo como Airflow o Luigi, el programador de Dask está específicamente afinado para la baja latencia y alta eficiencia requeridas en cálculos numéricos y análisis interactivos.8  
* **Colecciones Paralelas (Parallel Collections):** Sobre el programador de tareas se construyen las colecciones de Dask, que son la interfaz principal para la mayoría de los usuarios.2 Estas colecciones, como Dask Array, Dask DataFrame y Dask Bag, imitan las interfaces de programación de aplicaciones (API) de sus contrapartes en el ecosistema PyData (NumPy, Pandas, iteradores de Python), pero operan de forma paralela y distribuida.2 Esto significa que los usuarios pueden trabajar con estructuras de datos familiares pero aplicadas a conjuntos de datos mucho más grandes o para acelerar cálculos.  
* **Evaluación Perezosa y Grafos de Tareas (Lazy Evaluation & Task Graphs):** Una característica fundamental de Dask es su evaluación perezosa ("lazy evaluation").12 Cuando se aplican operaciones sobre colecciones Dask (por ejemplo, sumar dos Dask Arrays o agrupar un Dask DataFrame), la computación no se realiza inmediatamente. En su lugar, Dask construye un grafo de tareas que representa la computación completa, incluyendo todas las operaciones y sus dependencias.6 Este grafo es una estructura de datos que define qué funciones deben ejecutarse y sobre qué datos, y cómo los resultados fluyen entre las tareas. La ejecución real de estas tareas solo se dispara cuando se solicita explícitamente el resultado, típicamente mediante una llamada al método .compute().13 Esta estrategia de evaluación perezosa es crucial porque permite a Dask analizar el grafo completo de tareas *antes* de la ejecución, posibilitando optimizaciones significativas como la fusión de tareas o la gestión inteligente de la memoria, lo que conduce a una ejecución más eficiente.14  
* **Fragmentación/Particionado (Chunking/Partitioning):** Para manejar grandes conjuntos de datos, Dask los divide automáticamente en piezas más pequeñas y manejables. En Dask Array, estas piezas se llaman "chunks" (fragmentos), que son típicamente arrays de NumPy más pequeños.2 En Dask DataFrame, se llaman "partitions" (particiones), que son DataFrames de Pandas individuales.2 Dask orquesta las operaciones sobre estos chunks o particiones en paralelo, permitiendo procesar datos que no cabrían en la memoria de una sola vez y utilizando múltiples núcleos o máquinas.2

### **1.3 Por Qué y Cuándo Usar Dask (y Cuándo No)**

Dask es particularmente útil en varios escenarios:

* **Procesamiento de Datos Más Grandes que la Memoria:** Cuando los conjuntos de datos exceden la RAM disponible, Dask permite realizar cálculos "out-of-core", utilizando el disco como extensión de la memoria si es necesario y procesando los datos en fragmentos.1  
* **Aceleración de Computaciones:** Para tareas computacionalmente intensivas que tardan demasiado en un solo núcleo, Dask puede paralelizar el trabajo utilizando todos los núcleos de una máquina o distribuyéndolo en un clúster, reduciendo significativamente el tiempo de ejecución.1  
* **Paralelización de Algoritmos Personalizados:** Mediante interfaces de bajo nivel como dask.delayed, Dask ofrece la flexibilidad para paralelizar algoritmos complejos y personalizados que no se ajustan fácilmente a los patrones de las colecciones de alto nivel.6

Las **ventajas** clave de Dask incluyen:

* **API Familiar:** Para los usuarios de Pandas y NumPy, la transición a Dask DataFrame y Dask Array es relativamente sencilla debido a la similitud de las APIs.2 Esta familiaridad es una elección estratégica fundamental de Dask. Reduce drásticamente la curva de aprendizaje y el "costo humano" 18 asociado con la adopción de nuevas herramientas de computación paralela, haciendo que el escalado sea accesible para la vasta comunidad de usuarios de Python sin requerir un cambio completo de paradigma, a diferencia de otros frameworks distribuidos.9  
* **Flexibilidad:** Dask proporciona tanto colecciones de alto nivel para tareas comunes como APIs de bajo nivel (dask.delayed, dask.futures) para un control detallado sobre la paralelización.6  
* **Escalabilidad:** Dask está diseñado para escalar desde una laptop (aprovechando múltiples núcleos) hasta clústeres de miles de nodos en entornos de alto rendimiento (HPC) o en la nube.1  
* **Integración Nativa con Python:** Al ser una biblioteca pura de Python, se integra perfectamente con el ecosistema existente.8

Sin embargo, Dask **no siempre es la solución necesaria o la más eficiente**. Antes de recurrir a Dask, es recomendable considerar alternativas 19:

* **Optimizar Algoritmos/Estructuras de Datos:** A menudo, NumPy, Pandas o Scikit-learn pueden tener funciones más eficientes o enfoques algorítmicos alternativos para la tarea en cuestión.  
* **Usar Formatos de Archivo Eficientes:** Formatos binarios como Parquet, ORC, Zarr o HDF5 permiten un acceso eficiente a los datos y pueden ayudar a manejar grandes conjuntos de datos sin necesidad de paralelización compleja.19  
* **Compilar Código:** Usar Numba o Cython para compilar partes críticas del código Python puede proporcionar aceleraciones significativas, haciendo innecesaria la paralelización con Dask.19  
* **Muestreo (Sampling):** Si el objetivo es obtener insights, un muestreo inteligente del conjunto de datos puede ser suficiente y mucho más manejable.19  
* **Profiling:** Identificar los cuellos de botella reales en el código mediante profiling es esencial antes de decidir si la paralelización es la solución adecuada.19

En general, Dask introduce cierta sobrecarga. Por lo tanto, **no se recomienda para conjuntos de datos pequeños** que caben cómodamente en memoria o para **computaciones que ya son muy rápidas** (subsegundo), ya que la sobrecarga de Dask podría ralentizar el proceso en lugar de acelerarlo.6

## **Sección 2: Primeros Pasos: Instalación de Dask**

### **2.1 Métodos de Instalación: conda y pip**

Instalar Dask es generalmente sencillo y se puede realizar utilizando los gestores de paquetes comunes de Python: conda o pip.20

* **Usando conda:** conda es a menudo recomendado, especialmente si ya se utiliza la distribución Anaconda o Miniconda, ya que maneja bien las dependencias complejas, incluyendo las no escritas en Python. Dask suele venir incluido en Anaconda.21 Para instalar o actualizar:
```Bash  
  conda install dask
```
  Este comando generalmente instala Dask junto con dependencias comunes como Pandas y NumPy.20 Dask está disponible tanto en el canal defaults como en conda-forge. Para especificar conda-forge (a menudo preferido por tener versiones más actualizadas):  
```Bash  
  conda install dask \-c conda-forge
```   
  Para una instalación mínima que solo incluye el núcleo de Dask y sus dependencias esenciales:  
```Bash    
  conda install dask-core \-c conda-forge
```
  
* **Usando pip:** Dask también se puede instalar usando pip, el instalador de paquetes de Python. Para una instalación completa que incluye Dask, el programador distribuido y todas las dependencias opcionales comunes (recomendado para empezar):  
```Bash    
  python -m pip install "dask[complete]"
```
  
  Para una instalación mínima que solo incluye el núcleo de Dask:  
```Bash  
  python -m pip install dask
```

### **2.2 Entendiendo las Opciones de Instalación y Dependencias**

Es importante comprender que Dask es modular. La instalación mínima (pip install dask o conda install dask-core) solo proporciona el programador de tareas central y la funcionalidad básica.20 Para usar las colecciones de alto nivel, se necesitan dependencias adicionales:

* dask.array requiere NumPy.  
* dask.dataframe requiere Pandas (y NumPy).  
* dask.bag requiere cloudpickle y toolz.  
* El programador distribuido (dask.distributed) requiere tornado y distributed.  
* El panel de diagnóstico (dashboard) requiere bokeh.

pip permite instalar estas dependencias opcionales usando "extras" entre corchetes.20 Por ejemplo:

```Bash  

# Instalar dependencias para Dask DataFrame  
python -m pip install "dask[dataframe]"

# Instalar dependencias para Dask Array  
python -m pip install "dask[array]"

# Instalar dependencias para el dashboard  
python -m pip install "dask[diagnostics]"

# Instalar dependencias para el programador distribuido  
python -m pip install "dask[distributed]"
```

La razón de esta modularidad es evitar que los usuarios que solo necesitan el programador ligero tengan que descargar e instalar bibliotecas grandes como NumPy o Pandas si no las van a usar.20

**Importante:** Si se instala solo el núcleo de Dask (pip install dask) e intentar importar, por ejemplo, dask.dataframe, se producirá un ImportError porque la dependencia (Pandas) no está instalada.23 Por ello, para el uso interactivo general y para seguir los ejemplos de este tutorial, se recomienda instalar el paquete completo (dask\[complete\]) o, como mínimo, las extras específicas que se vayan a utilizar (p.ej., dask\[dataframe\] y dask\[array\]).20

Aunque dask\[complete\] o conda install dask suelen incluir dask.distributed, este también se puede instalar explícitamente si es necesario.20 Para despliegues en clústeres específicos, pueden ser necesarios paquetes adicionales como dask-jobqueue para sistemas HPC (SLURM, PBS) 3 o dask-kubernetes, dask-gateway 27 para otros entornos, pero estos están fuera del alcance de esta introducción básica.

### **2.3 Verificando la Instalación (Opcional)**

Una forma sencilla de verificar que Dask está instalado correctamente es abrir un intérprete de Python o un notebook y ejecutar:

```Python

import dask  
import dask.array as da  
import dask.dataframe as dd

print(dask.__version__)
```
Si estos comandos se ejecutan sin errores (asumiendo que se instalaron las dependencias para array y dataframe), la instalación básica es funcional. Para una verificación más exhaustiva, especialmente para desarrolladores, se pueden ejecutar los tests internos de Dask usando pytest.20

## **Sección 3: Los Bloques de Construcción de Dask: Colecciones Centrales**

### **3.1 Vista General: Colecciones de Alto Nivel vs. Bajo Nivel**

Las colecciones Dask son las interfaces principales a través de las cuales los usuarios interactúan con la biblioteca para definir y ejecutar cálculos paralelos.6 Se pueden clasificar en dos categorías principales:

* **Colecciones de Alto Nivel:** Estas incluyen Dask Array, Dask DataFrame y Dask Bag. Están diseñadas para trabajar con estructuras de datos comunes y proporcionan APIs que imitan de cerca a sus equivalentes en NumPy, Pandas y listas/iteradores de Python.2 Su objetivo es facilitar el escalado de flujos de trabajo existentes con cambios mínimos en el código.  
* **Colecciones de Bajo Nivel:** Estas son principalmente Dask Delayed y Dask Futures. Ofrecen un nivel de control mucho más fino sobre la paralelización, permitiendo a los usuarios construir grafos de tareas personalizados y manejar lógicas de paralelización complejas que no encajan directamente en los patrones de las colecciones de alto nivel.2

Esta estructura dual representa un espectro de control. Dask actúa tanto como una solución "llave en mano" para patrones de escalado comunes a través de sus colecciones de alto nivel, como una biblioteca fundamental para construir sistemas paralelos a medida utilizando herramientas de bajo nivel como Delayed. Esto permite a Dask satisfacer las necesidades tanto de usuarios que buscan una forma sencilla de escalar sus análisis existentes como de aquellos que requieren una flexibilidad total para diseñar algoritmos paralelos complejos.

### **3.2 Dask Delayed: Paralelizando Código Python Personalizado**

dask.delayed es una herramienta potente y flexible para paralelizar código Python general, especialmente útil cuando las colecciones de alto nivel no se ajustan al problema.6 Funciona envolviendo llamadas a funciones Python de forma "perezosa" (lazy). En lugar de ejecutar la función inmediatamente, dask.delayed devuelve un objeto Delayed que representa la llamada a la función y sus argumentos, registrándola como una tarea en un grafo.8 Esto se puede hacer usando el decorador @delayed o envolviendo la llamada directamente con delayed(func)(\*args).

Al encadenar llamadas a funciones envueltas con delayed, donde la salida de una función se convierte en la entrada de otra, Dask construye automáticamente un grafo de tareas que captura las dependencias entre ellas.8 Este grafo representa el flujo completo de la computación planificada.

La ejecución real de todas las tareas en el grafo solo ocurre cuando se llama al método .compute() sobre el objeto Delayed final que representa el resultado deseado.8 Dask entonces utiliza su programador para ejecutar las tareas del grafo en paralelo, respetando las dependencias.

**Caso de Uso: Problemas Embarazosamente Paralelos**

Un caso de uso ideal para dask.delayed son los problemas "embarazosamente paralelos", que consisten en muchas tareas completamente independientes que no requieren comunicación entre sí \[Query Point 4\].

* **Ejemplo 1: Aplicación Paralela de Funciones:** Consideremos aplicar una función simple, como incrementar un número, a una lista de valores.  
```Python  
  import dask  
  import time

  # Funciones simples de ejemplo  
  def inc(x):  
      time.sleep(0.1) # Simular trabajo  
      return x + 1

  def add(x, y):  
      time.sleep(0.1) # Simular trabajo  
      return x + y

  # Enfoque secuencial  
  start_time = time.time()  
  results_seq =  
  for i in range(10):  
      y = inc(i)  
      results_seq.append(y)  
  total_seq = sum(results_seq)  
  print(f"Secuencial: Resultado={total_seq}, Tiempo={time.time() - start_time:.2f}s")

  # Enfoque paralelo con dask.delayed  
  delayed = dask.delayed # Alias para conveniencia

  start_time = time.time()  
  results_delayed =  
  for i in range(10):  
      y = delayed(inc)(i) # Envuelve la llamada a inc  
      results_delayed.append(y)  
  total_delayed = delayed(sum)(results_delayed) # Envuelve la llamada a sum

  # Visualizar el grafo de tareas (opcional, requiere graphviz)  
  # total_delayed.visualize(filename='graph_inc.png')

  # Ejecutar la computación  
  result_parallel = total_delayed.compute()  
  print(f"Paralelo (Delayed): Resultado={result_parallel}, Tiempo={time.time() - start_time:.2f}s")
```

  En este ejemplo, delayed convierte las llamadas a inc y sum en tareas perezosas. El grafo resultante se ejecuta en paralelo al llamar a .compute(), lo que lleva a una ejecución más rápida que el bucle secuencial (asumiendo múltiples núcleos disponibles).  
* **Ejemplo 2: Procesamiento Paralelo de Archivos:** Simulemos la lectura y procesamiento simple de varios archivos.  
  ```Python  
  import os  
  # Crear archivos de ejemplo (simulado)  
  if not os.path.exists('temp_data'): os.makedirs('temp_data')  
  for i in range(5):  
      with open(f'temp_data file_{i}.txt', 'w') as f:  
          f.write(f"Data in file {i} \n" * (i + 1))

  filenames = [f'temp_data/file_{i}.txt' for i in range(5)]

  def process_file(filename):  
      # Simular lectura y procesamiento (contar líneas)  
      time.sleep(0.2)  
      with open(filename, 'r') as f:  
          return len(f.readlines())

  # Enfoque paralelo con dask.delayed  
  start_time = time.time()  
  results =  
  for fn in filenames:  
      count = delayed(process_file)(fn)  
      results.append(count)

  total_lines = delayed(sum)(results)  
  # total_lines.visualize(filename='graph_files.png')

  final_result = total_lines.compute()  
  print(f"Procesamiento Archivos (Delayed): Líneas Totales={final_result}, Tiempo={time.time() - start_time:.2f}s")

  # Limpiar archivos de ejemplo  
  # import shutil; shutil.rmtree('temp_data')
```
  Aquí, cada llamada a process\_file se convierte en una tarea independiente, permitiendo que la lectura y el procesamiento de los archivos ocurran en paralelo.

dask.delayed es la herramienta fundamental cuando se necesita integrar lógica Python personalizada en un flujo de trabajo Dask o cuando la estructura del problema no se mapea directamente a las operaciones de Array o DataFrame.8

### **3.3 Dask Arrays: Escalando Flujos de Trabajo NumPy**

Dask Array proporciona una interfaz paralela y distribuida para trabajar con grandes arrays N-dimensionales, imitando la API de NumPy.2 Un Dask Array está compuesto internamente por muchos arrays de NumPy más pequeños, llamados "chunks" (fragmentos), organizados en una cuadrícula.2 Las operaciones sobre Dask Arrays se implementan utilizando algoritmos de bloques ("blocked algorithms") que operan sobre estos chunks en paralelo, permitiendo cálculos en arrays que son demasiado grandes para caber en la memoria.2

La compatibilidad con la API de NumPy es un objetivo clave, cubriendo una gran parte de las funcionalidades comunes como slicing, operaciones aritméticas, reducciones (sum, mean, std, etc.), funciones matemáticas (sin, cos, exp, etc.) y más.2 Esto facilita a los usuarios de NumPy la adaptación a Dask Array para escalar sus análisis numéricos.2

La **fragmentación (chunking)** es un concepto central. Al crear un Dask Array (por ejemplo, desde un array NumPy existente con da.from\_array, o usando funciones generadoras como da.random.random o da.ones), se especifica cómo debe dividirse el array en chunks mediante el argumento chunks.3 El tamaño de los chunks es un parámetro crítico que afecta el rendimiento:

* Chunks demasiado pequeños: Pueden generar una gran cantidad de tareas, aumentando la sobrecarga del programador.19  
* Chunks demasiado grandes: Pueden no caber en la memoria de un trabajador individual o reducir el grado de paralelismo posible.19 Una regla general es apuntar a tamaños de chunk entre 100 MB y 1 GB, pero el tamaño óptimo depende de la memoria disponible, el número de núcleos y la naturaleza de los cálculos.19

**Caso de Uso: Operaciones en Grandes Conjuntos de Datos Numéricos**

Simulemos operaciones en un array demasiado grande para la memoria RAM \[Query Point 5\].

```Python
import dask.array as da  
import numpy as np

\# Crear un Dask Array grande (simulando datos \> memoria)  
\# Forma: (20000, 50000), Chunks: (1000, 1000\) \-\> 20\*50 \= 1000 chunks  
shape \= (20000, 50000)  
chunks \= (1000, 1000)  
x \= da.random.random(shape, chunks=chunks)  
print(x)  
print(f"Tamaño en GB (aprox): {x.nbytes / 1e9:.2f} GB")

\# Operaciones tipo NumPy (son perezosas, devuelven Dask Arrays/Scalars)  
y \= x \+ x.T  \# Operación elemento a elemento  
z \= x.mean(axis=0) \# Reducción a lo largo de un eje  
w \= y\[:1000, :1000\] \# Slicing

print("\\nResultados perezosos:")  
print(y)  
print(z)  
print(w)

\# Para obtener resultados concretos (NumPy arrays), usar.compute()  
\# ¡Advertencia\! Esto puede requerir mucha memoria si el resultado es grande.  
start\_time \= time.time()  
mean\_val \= x.mean().compute() \# Calcular la media global  
print(f"\\nMedia global calculada: {mean\_val:.4f} (Tiempo: {time.time() \- start\_time:.2f}s)")

\# Calcular la forma del resultado de z sin calcular todos los valores  
start\_time \= time.time()  
z\_shape \= da.compute(z.shape) \# Solo calcula la forma  
print(f"Forma de z (mean(axis=0)): {z\_shape} (Tiempo: {time.time() \- start\_time:.2f}s)")

\# Calcular un resultado más pequeño (el slice w)  
start\_time \= time.time()  
w\_computed \= w.compute()  
print(f"Slice w calculado, forma: {w\_computed.shape} (Tiempo: {time.time() \- start\_time:.2f}s)")
```
Este ejemplo muestra cómo crear un Dask Array grande y aplicar operaciones familiares de NumPy. Las operaciones son perezosas, construyendo un grafo de tareas. .compute() desencadena la ejecución paralela sobre los chunks. Es importante notar que llamar a .compute() en un resultado que sigue siendo muy grande puede agotar la memoria del cliente; a menudo se calculan resultados agregados más pequeños o se utiliza .persist() (ver Sección 4).

Dask Array se integra estrechamente con otras bibliotecas científicas como Xarray, que lo utiliza como backend para manejar arrays N-dimensionales etiquetados, comunes en ciencias de la tierra y clima.3

### **3.4 Dask DataFrames: Escalando Flujos de Trabajo Pandas**

Dask DataFrame paraleliza muchas de las operaciones de la popular biblioteca Pandas, permitiendo trabajar con conjuntos de datos tabulares que exceden la memoria RAM o acelerar análisis en datos grandes.3 Un Dask DataFrame está compuesto por múltiples DataFrames de Pandas más pequeños, llamados "particiones".2 Estas particiones suelen estar divididas a lo largo del índice del DataFrame, lo que significa que cada partición contiene un rango de filas.2

Al igual que Dask Array busca la compatibilidad con NumPy, Dask DataFrame se esfuerza por replicar una gran parte de la API de Pandas.2 Operaciones como la lectura de archivos (CSV, Parquet), selección de filas y columnas, filtrado, creación de nuevas columnas, y muchas agregaciones groupby funcionan de manera similar a Pandas, pero se ejecutan en paralelo sobre las particiones.7 Sin embargo, es importante saber que no todas las funcionalidades de Pandas están implementadas o pueden ser paralelizadas eficientemente en Dask. Operaciones que requieren una reordenación completa de los datos (shuffling), como set\_index sobre una columna no ordenada o ciertas agregaciones complejas, pueden ser costosas en Dask porque requieren mover grandes cantidades de datos entre las particiones (y potencialmente entre máquinas en un clúster).

**Caso de Uso: Procesamiento de Grandes Conjuntos de Datos Tabulares**

Veamos cómo usar Dask DataFrame para procesar datos que simulan ser más grandes que la memoria, por ejemplo, leyendo múltiples archivos CSV \[Query Point 6\].

```Python
import dask.dataframe as dd  
import pandas as pd  
import os  
import glob

\# Crear archivos CSV de ejemplo (simulado)  
if not os.path.exists('temp\_csv'): os.makedirs('temp\_csv')  
num\_files \= 10  
rows\_per\_file \= 10000  
for i in range(num\_files):  
    df\_temp \= pd.DataFrame({  
        'id': range(i \* rows\_per\_file, (i \+ 1) \* rows\_per\_file),  
        'value': np.random.rand(rows\_per\_file) \* 100,  
        'category': np.random.choice(, size=rows\_per\_file)  
    })  
    df\_temp.to\_csv(f'temp\_csv/data\_{i:02d}.csv', index=False)

\# Leer múltiples archivos CSV en un Dask DataFrame  
\# El comodín '\*' permite leer todos los archivos que coincidan  
ddf \= dd.read\_csv('temp\_csv/data\_\*.csv')

print(f"Dask DataFrame creado con {ddf.npartitions} particiones.")  
\# Inspeccionar las primeras filas (lee solo la primera partición)  
print("\\nPrimeras filas (head):")  
print(ddf.head())

\# Operaciones tipo Pandas (perezosas)  
\# Filtrar filas  
filtered\_ddf \= ddf\[ddf.value \> 90\]

\# Seleccionar columnas  
selected\_cols\_ddf \= ddf\[\['id', 'category'\]\]

\# Crear una nueva columna  
ddf\['value\_squared'\] \= ddf.value \*\* 2

\# Agregación groupby  
mean\_by\_category \= ddf.groupby('category').value.mean()

print("\\nResultados perezosos:")  
print(mean\_by\_category) \# Muestra la estructura, no el resultado

\# Calcular resultados concretos con.compute()  
start\_time \= time.time()  
mean\_by\_category\_computed \= mean\_by\_category.compute()  
print(f"\\nMedia por categoría calculada:\\n{mean\_by\_category\_computed}")  
print(f"(Tiempo: {time.time() \- start\_time:.2f}s)")

start\_time \= time.time()  
count\_filtered \= len(filtered\_ddf) \# len() también dispara.compute() implícitamente  
print(f"\\nNúmero de filas con valor \> 90: {count\_filtered}")  
print(f"(Tiempo: {time.time() \- start\_time:.2f}s)")

\# Limpiar archivos de ejemplo  
\# import shutil; shutil.rmtree('temp\_csv')
```
Este ejemplo demuestra la lectura de múltiples archivos, seguida de operaciones comunes de Pandas como filtrado, selección, creación de columnas y groupby. Todas estas operaciones son perezosas hasta que se llama a .compute(). Para conjuntos de datos realmente grandes, dd.read\_parquet() suele ser más eficiente que dd.read\_csv().

Dask DataFrame se utiliza a menudo en pipelines de ETL (Extract, Transform, Load) y como paso previo para el entrenamiento de modelos de Machine Learning con bibliotecas que soportan Dask, como XGBoost 11 o a través de la biblioteca dask-ml que paraleliza partes de Scikit-learn.2

### **3.5 Dask Bags: Paralelizando Operaciones sobre Iterables**

Dask Bag proporciona una estructura de datos paralela para colecciones de objetos Python genéricos, especialmente útil para datos semi-estructurados o no estructurados como archivos de texto, logs JSON o listas de objetos Python arbitrarios.2 Se puede pensar en un Dask Bag como una colección similar a una lista o un iterador, pero que opera en paralelo y no garantiza un orden específico entre sus elementos.2

La API de Dask Bag incluye operaciones funcionales comunes como map, filter, fold (una forma de reducción), y groupby.2 Funciona procesando los datos de forma secuencial (streaming) dentro de cada partición, lo que ayuda a minimizar el uso de memoria.3 A menudo se utiliza para tareas de preprocesamiento, como la limpieza de texto o la extracción de información de archivos JSON, antes de cargar los datos en una estructura más organizada como un Dask DataFrame.3 Su funcionalidad es comparable a los RDDs de Spark o a trabajar con iteradores en Python puro.3

**Breve Ejemplo de Caso de Uso:**

```Python
import dask.bag as db  
import json

\# Crear un Dask Bag desde una secuencia Python  
b \= db.from\_sequence(, npartitions=2)

\# O leer líneas de archivos de texto (simulado)  
\# b\_text \= db.read\_text('temp\_logs/\*.log').map(json.loads)

\# Operaciones tipo map/filter/groupby (perezosas)  
names \= b.map(lambda record: record\['name'\])  
alice\_records \= b.filter(lambda record: record\['name'\] \== 'Alice')  
value\_sum \= b.pluck('value').sum() \# pluck extrae un campo, luego suma  
name\_frequencies \= names.frequencies()

print("\\nResultados perezosos (Bag):")  
print(value\_sum)  
print(name\_frequencies)

\# Calcular resultados con.compute()  
start\_time \= time.time()  
total\_value \= value\_sum.compute()  
print(f"\\nSuma total de valores: {total\_value}")  
print(f"(Tiempo: {time.time() \- start\_time:.2f}s)")

start\_time \= time.time()  
freq \= name\_frequencies.compute()  
print(f"\\nFrecuencia de nombres: {freq}")  
print(f"(Tiempo: {time.time() \- start\_time:.2f}s)")
```

Este ejemplo muestra cómo crear un Bag y aplicar operaciones funcionales para extraer, filtrar y agregar información de los registros.

### **3.6 Eligiendo la Colección Correcta (Tabla Resumen)**

La elección de la colección Dask adecuada depende del tipo de datos y de la naturaleza de la computación que se desea paralelizar. La siguiente tabla resume las características clave:

| Colección | Paraleliza | Tipo de Dato | Características Clave | Casos de Uso Comunes |
| :---- | :---- | :---- | :---- | :---- |
| Dask Delayed | Funciones Python custom | Objetos Python arbitrarios | Perezoso, flexible, construcción de grafos, bajo nivel | Tareas embarazosamente paralelas, flujos de trabajo custom, envolver código no-Dask |
| Dask Array | Arrays NumPy | Numérico (homogéneo) | Fragmentado (Chunked), API NumPy, algoritmos de bloques | Grandes arrays N-D, computación científica, procesamiento de imágenes |
| Dask DataFrame | DataFrames Pandas | Tabular (heterogéneo) | Particionado, API Pandas, alineado por índice | Grandes archivos CSV/Parquet, operaciones tipo BBDD, análisis de datos estructurados |
| Dask Bag | Iterables/listas Python | Semi/No estructurado | No ordenado, map/filter/fold, streaming | Archivos de log, datos JSON, procesamiento de texto, operaciones paralelas en listas |

Esta tabla sirve como una guía rápida para seleccionar la herramienta Dask más apropiada para una tarea específica, basándose en la estructura de los datos y las operaciones requeridas.

## **Sección 4: Ejecutando su Código: Ejecución y Programación**

### **4.1 Disparando Computaciones: .compute() y .persist()**

Como se mencionó anteriormente, las operaciones en las colecciones Dask son perezosas por defecto, construyendo un grafo de tareas en lugar de ejecutar inmediatamente.12 Para obtener un resultado concreto, se utilizan principalmente dos métodos:

* **.compute():** Este es el método más común para ejecutar el grafo de tareas completo asociado a un objeto Dask.7 Realiza todos los cálculos necesarios y devuelve el resultado final como un objeto estándar de Python (por ejemplo, un array de NumPy, un DataFrame de Pandas, un número, una lista). Es crucial tener en cuenta que el resultado completo **debe caber en la memoria de la máquina cliente** donde se llama a .compute(). Si el resultado final sigue siendo muy grande, llamar a .compute() puede provocar errores de memoria.  
* **.persist():** Este método también ejecuta el grafo de tareas, pero en lugar de traer el resultado final de vuelta al cliente, **deja los resultados distribuidos en la memoria de los trabajadores** del clúster (o en los procesos/hilos locales si se usa un programador local) como nuevas colecciones Dask.14 Devuelve inmediatamente un nuevo objeto Dask que apunta a estos resultados persistidos. .persist() es particularmente útil para **resultados intermedios** que se van a utilizar varias veces en cálculos posteriores. Al persistir un resultado intermedio, se evita tener que recalcularlo desde el principio cada vez que se necesite. Para que .persist() sea efectivo en un entorno distribuido, se necesita tener un clúster Dask activo (ver Sección 4.2).

La elección entre .compute() y .persist() depende de si se necesita el resultado final en el cliente y si cabe en memoria (.compute()), o si se trata de un resultado intermedio grande que se reutilizará (.persist()). La llamada a .compute() es el punto donde el grafo de tareas, potencialmente optimizado por Dask 14, es entregado al programador para su ejecución real.

### **4.2 Entendiendo los Programadores (Schedulers) de Dask**

Los programadores son el motor de Dask, responsables de tomar el grafo de tareas y ejecutarlo en los recursos computacionales disponibles.6 Dask ofrece varios programadores, que se pueden agrupar en dos categorías principales: locales (para una sola máquina) y distribuidos (para clústeres).

* **Programadores Locales (Single Machine):** Estos programadores están diseñados para ejecutar tareas en paralelo utilizando los recursos de una única máquina.5 Son los predeterminados y no requieren configuración adicional.  
  * **Programador Basado en Hilos (Threaded Scheduler):** Utiliza un pool de hilos (ThreadPoolExecutor) dentro del proceso Python principal. Es el predeterminado para Dask Array y Dask DataFrame. Tiene baja sobrecarga y es muy eficiente para código numérico (NumPy, Pandas, Scikit-learn) que libera el Global Interpreter Lock (GIL) de Python, permitiendo paralelismo real.19  
  * **Programador Basado en Procesos (Multiprocessing Scheduler):** Utiliza un pool de procesos (ProcessPoolExecutor). Cada proceso tiene su propio intérprete de Python y memoria, lo que le permite sortear el GIL. Es más adecuado para tareas que están limitadas por el GIL (código Python puro, manipulación de strings o diccionarios).5 Sin embargo, tiene una sobrecarga mayor debido a la necesidad de serializar datos para la comunicación entre procesos.  
  * **Programador de Hilo Único (Single-Threaded Scheduler):** Ejecuta todas las tareas secuencialmente en el hilo principal. No ofrece paralelismo real, pero es extremadamente útil para **depuración**, ya que los errores son más fáciles de rastrear y permite entender el flujo de ejecución del grafo.5

Dask generalmente elige un programador local predeterminado razonable basado en la colección utilizada, pero se puede especificar explícitamente usando el argumento scheduler en la llamada a .compute() (p.ej., result.compute(scheduler='threads')).

* **Programador Distribuido (Distributed Scheduler):** Este es un programador más avanzado y potente, diseñado para coordinar la ejecución de tareas a través de **múltiples máquinas** en una red (un clúster).1 También se puede usar en una sola máquina para obtener diagnósticos más detallados y un control más fino sobre la ejecución. Consta de tres componentes principales 6:  
  * Un **dask-scheduler central:** Orquesta el grafo de tareas, monitoriza el estado de los trabajadores y asigna tareas.  
  * Uno o más **dask-worker:** Procesos que se ejecutan en las máquinas del clúster (o localmente). Realizan los cálculos reales y almacenan los resultados. Cada trabajador suele gestionar múltiples hilos o procesos.  
  * Un objeto **Client:** Se crea en la sesión interactiva del usuario (por ejemplo, en un notebook Jupyter) y actúa como punto de entrada para interactuar con el clúster. Envía los grafos de tareas al scheduler y recibe los resultados.

  Para usar el programador distribuido, primero se debe configurar un clúster. Esto se puede hacer manualmente iniciando los procesos dask-scheduler y dask-worker en las máquinas deseadas, o de forma más sencilla usando herramientas como:

  * dask.distributed.LocalCluster: Crea un scheduler y trabajadores localmente en la misma máquina, útil para pruebas y desarrollo.3  
  * dask-jobqueue: Para desplegar Dask en clústeres HPC con gestores de colas como SLURM, PBS, SGE, LSF.3  
  * Integraciones con Kubernetes, YARN, o proveedores de nube (AWS, GCP, Azure) a través de bibliotecas como dask-kubernetes, dask-yarn, dask-cloudprovider, o servicios comerciales como Coiled.3  
* **Eligiendo un Programador:** La elección del programador tiene implicaciones significativas en el rendimiento y la complejidad.10  
  * **Empiece localmente:** Para la mayoría de los casos en una sola máquina, los programadores locales predeterminados (basados en hilos para código numérico, basados en procesos para código limitado por GIL) suelen ser suficientes.19 Use el programador de hilo único para depurar.  
  * **Considere el GIL:** Si su código pasa la mayor parte del tiempo en operaciones NumPy/Pandas/Scikit-learn, el programador de hilos es probablemente el mejor. Si es código Python puro intensivo, el de procesos puede ser necesario para superar el GIL.19  
  * **Vaya a distribuido con cautela:** Solo mueva al programador distribuido (incluso con LocalCluster) si realmente necesita escalar más allá de los recursos de un solo proceso (por ejemplo, para superar limitaciones de memoria o para usar múltiples máquinas) o si necesita las herramientas de diagnóstico avanzadas que proporciona.10 Configurar y gestionar un clúster distribuido introduce complejidad adicional y sobrecarga de red. Un error común es pasar al modo distribuido demasiado pronto para problemas que podrían resolverse eficientemente localmente, lo que puede resultar en un rendimiento *peor* debido a la sobrecarga.10

El programador elegido dicta cómo se ejecutan las tareas, dónde residen los datos intermedios y cómo se manejan los recursos, impactando directamente en la eficiencia y escalabilidad de las computaciones Dask.

## **Sección 5: Monitorizando su Trabajo: El Panel de Control (Dashboard) de Dask**

### **5.1 Introducción al Dashboard**

Una de las características más valiosas de Dask, especialmente cuando se utiliza el programador distribuido (incluso con un LocalCluster en una sola máquina), es su panel de control (dashboard) basado en web.6 Este dashboard proporciona diagnósticos interactivos y visualizaciones en tiempo real del estado del clúster y de las computaciones en curso.6

Normalmente, al crear un objeto dask.distributed.Client, se imprime un enlace al dashboard en la salida. Por defecto, suele estar accesible en http://localhost:8787 si se ejecuta localmente.

```Python
from dask.distributed import Client, LocalCluster

\# Iniciar un clúster local (scheduler \+ workers en la misma máquina)  
\# Esto imprimirá la dirección del dashboard (p.ej., http://127.0.0.1:8787/status)  
cluster \= LocalCluster()  
client \= Client(cluster)  
print(client) \# Muestra información del cliente y enlace al dashboard
```

### **5.2 Cómo Ayuda a Visualizar y Diagnosticar**

El dashboard ofrece varias páginas y herramientas que proporcionan información crucial para entender, depurar y optimizar las cargas de trabajo de Dask 6:

* **Task Stream (Flujo de Tareas):** Muestra una visualización en tiempo real de las tareas que se están ejecutando en cada trabajador (hilo o proceso) a lo largo del tiempo. Las tareas suelen estar codificadas por colores según la función que ejecutan (por ejemplo, read\_csv, sum, inc). Esto permite ver el grado de paralelismo, identificar qué tareas tardan más, detectar desequilibrios en la carga de trabajo entre trabajadores o identificar períodos de inactividad.  
* **Progress (Progreso):** Muestra barras de progreso para las diferentes operaciones de alto nivel que componen la computación (por ejemplo, cuántas particiones de read\_csv se han completado, el progreso de un groupby, etc.). También indica cuántas tareas están en memoria, cuántas están listas para ejecutarse, cuántas están en proceso y cuántas han fallado.  
* **Workers (Trabajadores):** Proporciona una tabla con información detallada sobre cada trabajador en el clúster, incluyendo el uso de memoria, la carga de CPU, el número de tareas completadas, el número de hilos/procesos, y la comunicación de red. Es fundamental para diagnosticar problemas de memoria (trabajadores que superan su límite), identificar trabajadores sobrecargados o inactivos.  
* **Graph (Grafo):** Visualiza el grafo de tareas que Dask está ejecutando actualmente. Aunque puede ser complejo para cálculos grandes, puede ayudar a entender las dependencias entre tareas.  
* **Profile (Perfil):** Permite realizar un perfilado estadístico del tiempo de ejecución del código en los trabajadores para identificar cuellos de botella a nivel de función.  
* **System (Sistema):** Muestra información general del sistema, como el uso de CPU y memoria en todo el clúster.

Utilizar el dashboard es fundamental para ir más allá del uso básico de Dask. Permite:

* **Entender el Paralelismo:** Ver cuántas tareas se ejecutan simultáneamente.  
* **Identificar Cuellos de Botella:** Detectar tareas o funciones que tardan inesperadamente mucho tiempo.  
* **Diagnosticar Errores de Memoria:** Observar si los trabajadores se están quedando sin memoria.  
* **Verificar la Utilización de Recursos:** Comprobar si se están utilizando eficientemente la CPU y la memoria.  
* **Optimizar el Rendimiento:** Obtener pistas sobre cómo ajustar el tamaño de los chunks/particiones, la elección del programador o la lógica del código para mejorar la eficiencia.

En resumen, el dashboard transforma la computación paralela de una "caja negra" a un proceso transparente, proporcionando la información necesaria para usar Dask de manera efectiva y eficiente.6 Se considera una herramienta esencial para cualquier usuario serio de Dask.

## **Sección 6: Conclusión y Próximos Pasos**

### **6.1 Resumen del Poder de Dask**

Dask se presenta como una biblioteca excepcionalmente potente y flexible dentro del ecosistema Python para abordar los desafíos de la computación paralela y el manejo de grandes conjuntos de datos. Sus fortalezas clave residen en:

* **Escalado del Ecosistema Python:** Permite a los usuarios aplicar las herramientas que ya conocen y aman (NumPy, Pandas, Scikit-learn) a problemas de mayor escala.  
* **APIs Familiares:** Reduce significativamente la barrera de entrada a la computación paralela para los desarrolladores de Python.  
* **Flexibilidad:** Ofrece un espectro de herramientas, desde colecciones de alto nivel fáciles de usar hasta la interfaz dask.delayed de bajo nivel para una personalización completa.  
* **Escalabilidad:** Funciona eficientemente tanto en una sola máquina como en grandes clústeres distribuidos.  
* **Diagnósticos:** Proporciona herramientas como el dashboard para entender, depurar y optimizar el rendimiento.

Dask llena un vacío importante, permitiendo a los usuarios de Python escalar sus análisis más allá de las limitaciones de un solo núcleo o de la memoria RAM, sin necesidad de abandonar el ecosistema Python ni de aprender paradigmas de programación completamente nuevos.

### **6.2 Próximos Pasos y Recursos**

Después de esta introducción, el siguiente paso es experimentar. Se anima a los usuarios a:

* Probar los ejemplos con sus propios datos o problemas.  
* Experimentar con diferentes colecciones (Array, DataFrame, Delayed) para ver cuál se adapta mejor.  
* Utilizar LocalCluster y el dashboard para observar cómo se ejecutan sus cálculos.  
* Ajustar parámetros como el tamaño de los chunks o particiones y observar su impacto.

Para profundizar en el conocimiento de Dask, los siguientes recursos son altamente recomendados:

* **Documentación Oficial de Dask:** ([https://docs.dask.org/](https://docs.dask.org/)) La fuente más completa y actualizada de información.3  
* **Ejemplos de Dask:** ([https://examples.dask.org/](https://examples.dask.org/)) Una colección de notebooks con ejemplos prácticos para diversas aplicaciones.16  
* **Tutorial de Dask:** ([https://tutorial.dask.org/](https://tutorial.dask.org/)) Un tutorial interactivo basado en notebooks que cubre los fundamentos en detalle.11  
* **Blog de Dask:** Publica artículos sobre nuevas características, casos de uso y mejores prácticas.  
* **Ecosistema Dask:** Explorar bibliotecas que se integran o extienden Dask, como Xarray (para datos multidimensionales etiquetados), Dask-ML (para machine learning escalable), Prefect/Airflow (para orquestación de flujos de trabajo que incluyen Dask), entre otras.11

### **6.3 Pensamiento Final**

Dask es una herramienta poderosa que democratiza la computación paralela y distribuida para la comunidad Python. Al proporcionar interfaces familiares y una arquitectura flexible, permite a los científicos de datos, ingenieros y investigadores abordar problemas más grandes y complejos de manera más eficiente, impulsando la innovación y el descubrimiento en una amplia variedad de dominios. Dominar Dask abre la puerta a trabajar con datos y realizar cálculos a una escala que antes era inalcanzable dentro del entorno Python estándar.

#### **Fuentes citadas**

1. hamedalemo.github.io, acceso: abril 23, 2025, [https://hamedalemo.github.io/advanced-geo-python/lectures/dask\_intro.html\#:\~:text=Dask%20is%20a%20Python%20library,cores%20and%20even%20distributed%20clusters.](https://hamedalemo.github.io/advanced-geo-python/lectures/dask_intro.html#:~:text=Dask%20is%20a%20Python%20library,cores%20and%20even%20distributed%20clusters.)  
2. Dask (software) \- Wikipedia, acceso: abril 23, 2025, [https://en.wikipedia.org/wiki/Dask\_(software)](https://en.wikipedia.org/wiki/Dask_\(software\))  
3. Dask — Dask documentation, acceso: abril 23, 2025, [https://docs.dask.org/](https://docs.dask.org/)  
4. Parallel Computing with Dash and Dask | Dash for Python Documentation | Plotly, acceso: abril 23, 2025, [https://dash.plotly.com/dask-dash](https://dash.plotly.com/dask-dash)  
5. Dask in Python | GeeksforGeeks, acceso: abril 23, 2025, [https://www.geeksforgeeks.org/introduction-to-dask-in-python/](https://www.geeksforgeeks.org/introduction-to-dask-in-python/)  
6. Dask Overview — Dask Cookbook \- Project Pythia, acceso: abril 23, 2025, [https://projectpythia.org/dask-cookbook/notebooks/00-dask-overview.html](https://projectpythia.org/dask-cookbook/notebooks/00-dask-overview.html)  
7. An Introduction to Dask: The Python Data Scientist's Power Tool \- KDnuggets, acceso: abril 23, 2025, [https://www.kdnuggets.com/introduction-dask-python-data-scientist-power-tool](https://www.kdnuggets.com/introduction-dask-python-data-scientist-power-tool)  
8. Dask — dask 0.16.1 documentation, acceso: abril 23, 2025, [https://dask-local.readthedocs.io/](https://dask-local.readthedocs.io/)  
9. Python Dask | Domino Data Lab, acceso: abril 23, 2025, [https://domino.ai/data-science-dictionary/dask](https://domino.ai/data-science-dictionary/dask)  
10. 10\. Introduction to Dask — Advanced Geospatial Analytics with Python \- Hamed Alemohammad, acceso: abril 23, 2025, [https://hamedalemo.github.io/advanced-geo-python/lectures/dask\_intro.html](https://hamedalemo.github.io/advanced-geo-python/lectures/dask_intro.html)  
11. Welcome to the Dask Tutorial — Dask Tutorial documentation, acceso: abril 23, 2025, [https://tutorial.dask.org/00\_overview.html](https://tutorial.dask.org/00_overview.html)  
12. Dask Collections: Arrays — easy.gems documentation, acceso: abril 23, 2025, [https://easy.gems.dkrz.de/Processing/dask/collections.html](https://easy.gems.dkrz.de/Processing/dask/collections.html)  
13. 10 Minutes to Dask, acceso: abril 23, 2025, [https://docs.dask.org/en/stable/10-minutes-to-dask.html](https://docs.dask.org/en/stable/10-minutes-to-dask.html)  
14. Custom Collections \- Dask documentation, acceso: abril 23, 2025, [https://docs.dask.org/en/stable/custom-collections.html](https://docs.dask.org/en/stable/custom-collections.html)  
15. Custom Collections — dask 0.16.1 documentation, acceso: abril 23, 2025, [https://dask-local.readthedocs.io/en/latest/custom-collections.html](https://dask-local.readthedocs.io/en/latest/custom-collections.html)  
16. Dask DataFrame \- Dask documentation, acceso: abril 23, 2025, [https://dask.pydata.org/en/latest/dataframe.html](https://dask.pydata.org/en/latest/dataframe.html)  
17. Dask DataFrame — Dask Tutorial, acceso: abril 23, 2025, [https://ncar.github.io/dask-tutorial/notebooks/02-dask-dataframe.html](https://ncar.github.io/dask-tutorial/notebooks/02-dask-dataframe.html)  
18. Dask | Scale the Python tools you love, acceso: abril 23, 2025, [https://www.dask.org/](https://www.dask.org/)  
19. Dask Best Practices \- Dask documentation, acceso: abril 23, 2025, [https://docs.dask.org/en/stable/best-practices.html](https://docs.dask.org/en/stable/best-practices.html)  
20. Install Dask, acceso: abril 23, 2025, [https://docs.dask.org/en/stable/install.html](https://docs.dask.org/en/stable/install.html)  
21. Install Dask — dask 0.16.1 documentation, acceso: abril 23, 2025, [https://dask-local.readthedocs.io/en/latest/install.html](https://dask-local.readthedocs.io/en/latest/install.html)  
22. Get Started \- Dask, acceso: abril 23, 2025, [https://www.dask.org/get-started](https://www.dask.org/get-started)  
23. Default pip installation of Dask gives "ImportError: No module named toolz" \- Stack Overflow, acceso: abril 23, 2025, [https://stackoverflow.com/questions/41453473/default-pip-installation-of-dask-gives-importerror-no-module-named-toolz](https://stackoverflow.com/questions/41453473/default-pip-installation-of-dask-gives-importerror-no-module-named-toolz)  
24. Dask install via pip does not install all dependencies (toolz, cloudpicke) \#3899 \- GitHub, acceso: abril 23, 2025, [https://github.com/dask/dask/issues/3899](https://github.com/dask/dask/issues/3899)  
25. Install Dask.Distributed, acceso: abril 23, 2025, [https://distributed.dask.org/en/stable/install.html](https://distributed.dask.org/en/stable/install.html)  
26. Installing — Dask-jobqueue 0.9.0+8.g6dc463d documentation, acceso: abril 23, 2025, [https://jobqueue.dask.org/en/latest/install.html](https://jobqueue.dask.org/en/latest/install.html)  
27. Dask Gateway \- EODC Knowledge Base, acceso: abril 23, 2025, [https://docs.eodc.eu/services/dask.html](https://docs.eodc.eu/services/dask.html)  
28. Working with Collections \- Dask documentation, acceso: abril 23, 2025, [https://docs.dask.org/en/stable/delayed-collections.html](https://docs.dask.org/en/stable/delayed-collections.html)
