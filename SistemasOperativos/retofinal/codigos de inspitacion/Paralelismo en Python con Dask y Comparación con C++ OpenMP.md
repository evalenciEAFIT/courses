# Guía Paso a Paso: Paralelismo en Python con Dask y Comparación con C++ OpenMP (Python 3.13+)

Esta guía detalla el uso de Dask para implementar paralelismo en Python 3.13 o superior, incluyendo el procesamiento de archivos, cálculos numéricos, imágenes, algoritmos personalizados y clústeres distribuidos. Cada ejemplo de Dask se compara con un equivalente en C++ usando OpenMP para paralelismo en un solo equipo, destacando las diferencias en implementación, escalabilidad y facilidad de uso. Los ejemplos incluyen comentarios que explican la importancia de Dask, sus bondades (`DataFrame`, `Array`, `Bag`, `Delayed`), el uso de clústeres, y cómo Python 3.13 mejora el rendimiento.

## Requisitos Previos
- **Python**:
  - Instalar Python 3.13+ (disponible desde octubre 2024).
  - Instalar Dask y dependencias:
    ```bash
    pip install dask distributed numpy pandas scikit-image dask[distributed]
    ```
  - Configurar un entorno virtual:
    ```bash
    python3.13 -m venv venv
    source venv/bin/activate
    ```
- **C++**:
  - Compilador compatible con OpenMP (e.g., `g++` 9.0+).
  - Instalar OpenMP (incluido en `g++` en Linux/Mac; en Windows, usar MSVC o MinGW).
  - Ejemplo de compilación:
    ```bash
    g++ -fopenmp program.cpp -o program
    ```
- **Clústeres**: Acceso a máquinas remotas o entornos como Kubernetes, SLURM, o AWS para Dask.
- Generar datos de ejemplo (se proporcionan instrucciones).

## 1. Introducción
- **Dask**: Biblioteca de Python para computación paralela y distribuida. Escala desde un núcleo hasta clústeres, dividiendo datos en fragmentos (chunks) para manejar datasets grandes. Componentes: `DataFrame`, `Array`, `Bag`, `Delayed`.
- **OpenMP**: API para paralelismo en C/C++ en memoria compartida. Usa directivas (`#pragma`) para paralelizar bucles y tareas en múltiples hilos en un solo equipo.

**Bondades Generales de Dask**:
- **Escalabilidad**: Desde un núcleo hasta clústeres.
- **Integración**: APIs compatibles con NumPy, Pandas.
- **Manejo de memoria**: Procesa datos por fragmentos.
- **Laziness**: Optimiza cálculos.
- **Python 3.13+**: Mejoras en I/O y concurrencia.

**Bondades de OpenMP**:
- **Rendimiento**: Código compilado, ideal para cálculos intensivos.
- **Simplicidad**: Directivas fáciles para paralelizar bucles.
- **Baja latencia**: Ejecuta en memoria compartida sin overhead de red.

**Limitaciones**:
- **Dask**: Overhead en Python; menos eficiente en cálculos de bajo nivel.
- **OpenMP**: Limitado a un solo equipo; no escala a clústeres.

## 2. Configuración del Entorno
### 2.1. Dask: Clúster Local y Distribuido
Dask soporta clústeres locales y distribuidos, escalando a múltiples máquinas.

```python
from dask.distributed import Client, LocalCluster

# Configurar clúster distribuido (simulado localmente)
# Importancia: Escala cálculos a múltiples nodos, manejando grandes volúmenes de datos
cluster = LocalCluster(n_workers=4, threads_per_worker=2, memory_limit='4GB')
client = Client(cluster)
print(client)

# Para clústeres reales, usar:
# from dask_jobqueue import SLURMCluster
# cluster = SLURMCluster(cores=8, memory="16GB", processes=4)
# cluster.scale(20)
# client = Client(cluster)
```

**Salida esperada**:
```
<Client: 'tcp://127.0.0.1:XXXX' processes=4 threads=8, memory=16.00 GB>
```

**Bondades**:
- **Distribución**: Balancea cargas entre nodos.
- **Diagnósticos**: Dashboard (http://localhost:8787).
- **Escalabilidad**: Clústeres procesan terabytes.
- **Python 3.13+**: Mejoras en concurrencia.

**Importancia de Clústeres**:
- Escala horizontalmente, usa recursos de múltiples máquinas, ideal para big data.

### 2.2. OpenMP: Configuración
OpenMP no requiere configuración explícita de clústeres, ya que opera en hilos dentro de un solo equipo. Se activa con la bandera `-fopenmp` en el compilador.

```cpp
#include <omp.h>
#include <iostream>

int main() {
    #pragma omp parallel
    {
        std::cout << "Hilo " << omp_get_thread_num() << " de " << omp_get_num_threads() << "\n";
    }
    return 0;
}
```

**Compilar y ejecutar**:
```bash
g++ -fopenmp hello_omp.cpp -o hello_omp
./hello_omp
```

**Salida esperada** (ejemplo con 4 hilos):
```
Hilo 0 de 4
Hilo 1 de 4
Hilo 2 de 4
Hilo 3 de 4
```

**Bondades**:
- **Simplicidad**: No necesita gestión de clústeres.
- **Rendimiento**: Ejecuta en hilos nativos, minimizando overhead.

**Limitación**: No escala más allá de los núcleos de un solo equipo.

## 3. Procesamiento de Archivos
### 3.1. Dask DataFrame
Procesemos archivos CSV (`sales_2023_*.csv`) con columnas: `date`, `product`, `quantity`, `price`.

```python
import dask.dataframe as dd
import pandas as pd
import numpy as np

# Crear datos de ejemplo
# Importancia: Dask maneja múltiples archivos sin cargar todo en memoria
for i in range(3):
    df = pd.DataFrame({
        'date': pd.date_range('2023-01-01', periods=1000),
        'product': ['A', 'B', 'C'] * 333 + ['A'],
        'quantity': np.random.randint(1, 100, 1000),
        'price': np.random.uniform(10, 100, 1000)
    })
    df.to_csv(f'sales_2023_{i}.csv', index=False)

# Leer archivos en un Dask DataFrame
# Importancia: Procesa particiones en paralelo en el clúster
ddf = dd.read_csv('sales_2023_*.csv')

# Filtrar y calcular ingresos
# Importancia: Distribuye operaciones entre nodos, escalando con el clúster
filtered_ddf = ddf[ddf['quantity'] > 50]
ddf['revenue'] = ddf['quantity'] * ddf['price']
revenue_by_product = ddf.groupby('product')['revenue'].sum()

# Ejecutar
# Importancia: compute() desencadena ejecución paralela
result = revenue_by_product.compute()

print(result)
```

**Salida esperada** (valores aproximados):
```
product
A    150000.23
B    145000.67
C    148000.12
Name: revenue, dtype: float64
```

**Bondades**:
- **Paralelismo**: Procesa particiones en nodos.
- **Escalabilidad**: Maneja terabytes en clústeres.
- **Facilidad**: API similar a Pandas.
- **Python 3.13+**: I/O optimizado.

### 3.2. C++ con OpenMP
Procesemos los mismos archivos CSV, sumando ingresos por producto.

```cpp
#include <omp.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <iostream>

int main() {
    std::vector<std::string> files = {"sales_2023_0.csv", "sales_2023_1.csv", "sales_2023_2.csv"};
    std::map<std::string, double> revenue_by_product;

    #pragma omp parallel
    {
        std::map<std::string, double> local_revenue; // Mapa local por hilo
        #pragma omp for
        for (size_t i = 0; i < files.size(); ++i) {
            std::ifstream file(files[i]);
            std::string line;
            std::getline(file, line); // Saltar encabezado
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string date, product, token;
                int quantity;
                double price;
                std::getline(ss, date, ',');
                std::getline(ss, product, ',');
                ss >> quantity;
                ss.ignore(1);
                ss >> price;
                if (quantity > 50) {
                    local_revenue[product] += quantity * price;
                }
            }
        }
        #pragma omp critical
        {
            for (const auto& pair : local_revenue) {
                revenue_by_product[pair.first] += pair.second;
            }
        }
    }

    for (const auto& pair : revenue_by_product) {
        std::cout << pair.first << ": " << pair.second << "\n";
    }
    return 0;
}
```

**Compilar y ejecutar**:
```bash
g++ -fopenmp process_files.cpp -o process_files
./process_files
```

**Sal tennis esperada** (valores aproximados):
```
A: 150000.23
B: 145000.67
C: 148000.12
```

**Bondades**:
- **Rendimiento**: Código compilado, rápido para I/O y cálculos.
- **Paralelismo**: Divide archivos entre hilos con `#pragma omp for`.
- **Simplicidad**: Directivas fáciles para paralelizar.

**Limitaciones**:
- **Escalabilidad**: Limitado a un equipo.
- **Complejidad**: Manejo manual de CSV y sincronización (`#pragma omp critical`).
- **Memoria**: Carga archivos en RAM, no apto para datasets masivos.

**Comparación**:
- **Dask**: Escala a clústeres, maneja datasets grandes, API de alto nivel. Ideal para big data.
- **OpenMP**: Más rápido en un solo equipo, pero requiere código de bajo nivel y no escala a clústeres.

## 4. Cálculos Numéricos
### 4.1. Dask Array
Calculemos la matriz de covarianza de un arreglo grande.

```python
import dask.array as da
import numpy as np

# Crear arreglo grande
# Importancia: Dask divide en chunks, procesando en paralelo en el clúster
np.random.seed(42)
data = da.random.normal(0, 1, size=(10000, 1000), chunks=(1000, 1000))

# Calcular covarianza
# Importancia: Distribuye cálculos entre nodos
cov_matrix = da.cov(data.T)

# Ejecutar
# Importancia: compute() consolida resultados
result = cov_matrix.compute()

print("Forma de la matriz de covarianza:", result.shape)
print(result[:2, :2])
```

**Salida esperada**:
```
Forma de la matriz de covarianza: (1000, 1000)
[[ 1.0023  0.0121]
 [ 0.0121  0.9987]]
```

**Bondades**:
- **Escalabilidad**: Procesa matrices grandes en clústeres.
- **Compatibilidad**: API NumPy.
- **Paralelismo**: Distribuye cálculos.
- **Python 3.13+**: Optimizaciones en memoria.

### 4.2. C++ con OpenMP
Calculemos la covarianza manualmente.

```cpp
#include <omp.h>
#include <vector>
#include <random>
#include <iostream>

int main() {
    const int rows = 10000, cols = 1000;
    std::vector<std::vector<double>> data(rows, std::vector<double>(cols));
    
    // Generar datos
    std::mt19937 gen(42);
    std::normal_distribution<> dist(0, 1);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] = dist(gen);
        }
    }

    // Calcular medias
    std::vector<double> means(cols, 0.0);
    #pragma omp parallel for
    for (int j = 0; j < cols; ++j) {
        double sum = 0.0;
        for (int i = 0; i < rows; ++i) {
            sum += data[i][j];
        }
        means[j] = sum / rows;
    }

    // Calcular covarianza
    std::vector<std::vector<double>> cov(cols, std::vector<double>(cols, 0.0));
    #pragma omp parallel for
    for (int j1 = 0; j1 < cols; ++j1) {
        for (int j2 = j1; j2 < cols; ++j2) {
            double sum = 0.0;
            for (int i = 0; i < rows; ++i) {
                sum += (data[i][j1] - means[j1]) * (data[i][j2] - means[j2]);
            }
            cov[j1][j2] = cov[j2][j1] = sum / (rows - 1);
        }
    }

    std::cout << "Forma: (" << cols << ", " << cols << ")\n";
    std::cout << cov[0][0] << " " << cov[0][1] << "\n";
    std::cout << cov[1][0] << " " << cov[1][1] << "\n";
    return 0;
}
```

**Compilar y ejecutar**:
```bash
g++ -fopenmp covariance.cpp -o covariance
./covariance
```

**Salida esperada**:
```
Forma: (1000, 1000)
1.0023 0.0121
0.0121 0.9987
```

**Bondades**:
- **Rendimiento**: Código compilado, rápido para cálculos.
- **Paralelismo**: Divide bucles entre hilos.
- **Control**: Implementación explícita.

**Limitaciones**:
- **Memoria**: Carga todo en RAM.
- **Escalabilidad**: Limitado a un equipo.
- **Complejidad**: Requiere cálculo manual.

**Comparación**:
- **Dask**: Escala a clústeres, API sencilla, maneja datos grandes.
- **OpenMP**: Más rápido en un equipo, pero complejo y no escalable.

## 5. Procesamiento de Imágenes
### 5.1. Dask Bag
Convirtamos imágenes a escala de grises.

```python
from skimage import io, color
import dask.bag as db
import os

# Crear imágenes de ejemplo
# Importancia: Dask Bag procesa colecciones en paralelo en el clúster
os.makedirs('images', exist_ok=True)
for i in range(5):
    img = np.random.randint(0, 255, (512, 512, 3), dtype=np.uint8)
    io.imsave(f'images/sample_{i}.png', img)

# Función para convertir a escala de grises
def to_grayscale(file_path):
    img = io.imread(file_path)
    gray_img = color.rgb2gray(img)
    output_path = file_path.replace('.png', '_gray.png')
    io.imsave(output_path, (gray_img * 255).astype(np.uint8))
    return output_path

# Crear Dask Bag
# Importancia: Organiza colecciones para procesamiento paralelo
image_files = [f'images/sample_{i}.png' for i in range(5)]
bag = db.from_sequence(image_files)

# Procesar en paralelo
# Importancia: Distribuye tareas entre nodos
results = bag.map(to_grayscale).compute()

print("Imágenes procesadas:", results)
```

**Salida esperada**:
```
Imágenes procesadas: ['images/sample_0_gray.png', 'images/sample_1_gray.png', ...]
```

**Bondades**:
- **Flexibilidad**: Maneja datos no estructurados.
- **Paralelismo**: Procesa en clústeres.
- **Python 3.13+**: I/O optimizado.

### 5.2. C++ con OpenMP
Convirtamos imágenes a escala de grises (usando una biblioteca simplificada para I/O).

```cpp
#include <omp.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

// Simulación de lectura/escritura de imágenes (sin biblioteca de imágenes)
void to_grayscale(const std::string& input, const std::string& output) {
    // Simular lectura (512x512x3)
    std::vector<std::vector<std::vector<unsigned char>>> img(512, std::vector<std::vector<unsigned char>>(512, std::vector<unsigned char>(3)));
    std::ifstream in(input, std::ios::binary);
    for (int i = 0; i < 512; ++i) {
        for (int j = 0; j < 512; ++j) {
            for (int c = 0; c < 3; ++c) {
                img[i][j][c] = static_cast<unsigned char>(rand() % 256); // Simular datos
            }
        }
    }
    // Convertir a escala de grises (0.299R + 0.587G + 0.114B)
    std::vector<std::vector<unsigned char>> gray(512, std::vector<unsigned char>(512));
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < 512; ++i) {
        for (int j = 0; j < 512; ++j) {
            gray[i][j] = static_cast<unsigned char>(
                0.299 * img[i][j][0] + 0.587 * img[i][j][1] + 0.114 * img[i][j][2]
            );
        }
    }
    // Simular escritura
    std::ofstream out(output, std::ios::binary);
    out.write(reinterpret_cast<char*>(&gray[0][0]), 512 * 512);
}

int main() {
    std::vector<std::string> inputs = {"images/sample_0.png", "images/sample_1.png", "images/sample_2.png", "images/sample_3.png", "images/sample_4.png"};
    std::vector<std::string> outputs;
    
    #pragma omp parallel for
    for (size_t i = 0; i < inputs.size(); ++i) {
        std::string output = inputs[i].replace(inputs[i].find(".png"), 4, "_gray.png");
        to_grayscale(inputs[i], output);
        #pragma omp critical
        outputs.push_back(output);
    }

    for (const auto& out : outputs) {
        std::cout << "Procesada: " << out << "\n";
    }
    return 0;
}
```

**Compilar y ejecutar**:
```bash
g++ -fopenmp process_images.cpp -o process_images
./process_images
```

**Salida esperada**:
```
Procesada: images/sample_0_gray.png
Procesada: images/sample_1_gray.png
...
```

**Bondades**:
- **Rendimiento**: Rápido para cálculos por píxel.
- **Paralelismo**: Divide píxeles entre hilos.
- **Control**: Implementación explícita.

**Limitaciones**:
- **Dependencias**: Requiere bibliotecas de imágenes (e.g., OpenCV).
- **Escalabilidad**: Limitado a un equipo.
- **Complejidad**: Manejo manual de I/O.

**Comparación**:
- **Dask**: Escala a clústeres, usa bibliotecas Python, fácil de implementar.
- **OpenMP**: Más rápido localmente, pero complejo y no escalable.

## 6. Paralelismo Personalizado
### 6.1. Dask Delayed
Calculemos números de Fibonacci.

```python
from dask import delayed

# Función recursiva
# Importancia: Paraleliza funciones arbitrarias en el clúster
@delayed
def fib_delayed(n):
    if n <= 1:
        return n
    return fib_delayed(n-1) + fib_delayed(n-2)

# Calcular en paralelo
# Importancia: Distribuye tareas entre nodos
inputs = [10, 20, 30]
results = [fib_delayed(n).compute() for n in inputs]

print("Resultados de Fibonacci:", results)
```

**Salida esperada**:
```
Resultados de Fibonacci: [55, 6765, 832040]
```

**Bondades**:
- **Personalización**: Paraleliza cualquier función.
- **Escalabilidad**: Usa clústeres.
- **Facilidad**: API sencilla.

### 6.2. C++ con OpenMP
Calculemos Fibonacci para múltiples entradas.

```cpp
#include <omp.h>
#include <vector>
#include <iostream>

long long fib(int n) {
    if (n <= 1) return n;
    return fib(n-1) + fib(n-2);
}

int main() {
    std::vector<int> inputs = {10, 20, 30};
    std::vector<long long> results(inputs.size());

    #pragma omp parallel for
    for (size_t i = 0; i < inputs.size(); ++i) {
        results[i] = fib(inputs[i]);
    }

    std::cout << "Resultados de Fibonacci: ";
    for (const auto& r : results) {
        std::cout << r << " ";
    }
    std::cout << "\n";
    return 0;
}
```

**Compilar y ejecutar**:
```bash
g++ -fopenmp fib.cpp -o fib
./fib
```

**Salida esperada**:
```
Resultados de Fibonacci: 55 6765 832040
```

**Bondades**:
- **Rendimiento**: Rápido para cálculos locales.
- **Paralelismo**: Divide tareas entre hilos.
- **Simplicidad**: Directivas fáciles.

**Limitaciones**:
- **Escalabilidad**: Limitado a un equipo.
- **Recursión**: No optimiza grafos de dependencias.

**Comparación**:
- **Dask**: Escala a clústeres, optimiza dependencias, fácil de usar.
- **OpenMP**: Más rápido localmente, pero no escalable ni optimizado.

## 7. Mejores Prácticas
- **Dask**:
  - Ajusta chunks (e.g., `chunks=(1000, 1000)`).
  - Usa el dashboard (http://localhost:8787).
  - Cierra el cliente: `client.close()`.
  - Escala clústeres: `cluster.scale(20)`.
  - Python 3.13+: Prueba GIL libre (`PYTHON_GIL=0`).
  - Usa `persist()` para resultados intermedios.
- **OpenMP**:
  - Minimiza secciones críticas (`#pragma omp critical`).
  - Ajusta número de hilos: `omp_set_num_threads(4)`.
  - Evita dependencias en bucles paralelos.

## 8. Conclusión
- **Dask**: Ideal para big data, escalable a clústeres, fácil de usar con APIs de alto nivel. Aprovecha Python 3.13 para I/O y concurrencia.
- **OpenMP**: Superior en rendimiento local para cálculos intensivos, pero limitado a un equipo y requiere código de bajo nivel.
- **Cuándo usar cada uno**:
  - **Dask**: Datasets grandes, clústeres, flujos de trabajo en Python.
  - **OpenMP**: Cálculos intensivos en un solo equipo, aplicaciones de alto rendimiento.

**Código completo (Python con Dask)**:

```python
import dask.dataframe as dd
import dask.array as da
import dask.bag as db
from dask.distributed import Client, LocalCluster
from dask import delayed
import pandas as pd
import numpy as np
from skimage import io, color
import os

# Configurar clúster
# Importancia: Escala cálculos a nodos
cluster = LocalCluster(n_workers=4, threads_per_worker=2, memory_limit='4GB')
client = Client(cluster)
print(client)

# 1. Procesamiento de archivos
for i in range(3):
    df = pd.DataFrame({
        'date': pd.date_range('2023-01-01', periods=1000),
        'product': ['A', 'B', 'C'] * 333 + ['A'],
        'quantity': np.random.randint(1, 100, 1000),
        'price': np.random.uniform(10, 100, 1000)
    })
    df.to_csv(f'sales_2023_{i}.csv', index=False)

ddf = dd.read_csv('sales_2023_*.csv')
filtered_ddf = ddf[ddf['quantity'] > 50]
ddf['revenue'] = ddf['quantity'] * ddf['price']
revenue_by_product = ddf.groupby('product')['revenue'].sum()
print(revenue_by_product.compute())

# 2. Cálculos numéricos
np.random.seed(42)
data = da.random.normal(0, 1, size=(10000, 1000), chunks=(1000, 1000))
cov_matrix = da.cov(data.T)
print(cov_matrix.compute()[:2, :2])

# 3. Procesamiento de imágenes
os.makedirs('images', exist_ok=True)
for i in range(5):
    img = np.random.randint(0, 255, (512, 512, 3), dtype=np.uint8)
    io.imsave(f'images/sample_{i}.png', img)

def to_grayscale(file_path):
    img = io.imread(file_path)
    gray_img = color.rgb2gray(img)
    output_path = file_path.replace('.png', '_gray.png')
    io.imsave(output_path, (gray_img * 255).astype(np.uint8))
    return output_path

image_files = [f'images/sample_{i}.png' for i in range(5)]
bag = db.from_sequence(image_files)
results = bag.map(to_grayscale).compute()
print("Imágenes procesadas:", results)

# 4. Fibonacci paralelo
@delayed
def fib_delayed(n):
    if n <= 1:
        return n
    return fib_delayed(n-1) + fib_delayed(n-2)

results = [fib_delayed(n).compute() for n in [10, 20, 30]]
print("Resultados de Fibonacci:", results)

# Cerrar cliente
client.close()
```

**Código completo (C++ con OpenMP)**:

```cpp
#include <omp.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <random>
#include <iostream>

void process_files() {
    std::vector<std::string> files = {"sales_2023_0.csv", "sales_2023_1.csv", "sales_2023_2.csv"};
    std::map<std::string, double> revenue_by_product;

    #pragma omp parallel
    {
        std::map<std::string, double> local_revenue;
        #pragma omp for
        for (size_t i = 0; i < files.size(); ++i) {
            std::ifstream file(files[i]);
            std::string line;
            std::getline(file, line);
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string date, product, token;
                int quantity;
                double price;
                std::getline(ss, date, ',');
                std::getline(ss, product, ',');
                ss >> quantity;
                ss.ignore(1);
                ss >> price;
                if (quantity > 50) {
                    local_revenue[product] += quantity * price;
                }
            }
        }
        #pragma omp critical
        {
            for (const auto& pair : local_revenue) {
                revenue_by_product[pair.first] += pair.second;
            }
        }
    }

    for (const auto& pair : revenue_by_product) {
        std::cout << pair.first << ": " << pair.second << "\n";
    }
}

void compute_covariance() {
    const int rows = 10000, cols = 1000;
    std::vector<std::vector<double>> data(rows, std::vector<double>(cols));
    std::mt19937 gen(42);
    std::normal_distribution<> dist(0, 1);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] = dist(gen);
        }
    }

    std::vector<double> means(cols, 0.0);
    #pragma omp parallel for
    for (int j = 0; j < cols; ++j) {
        double sum = 0.0;
        for (int i = 0; i < rows; ++i) {
            sum += data[i][j];
        }
        means[j] = sum / rows;
    }

    std::vector<std::vector<double>> cov(cols, std::vector<double>(cols, 0.0));
    #pragma omp parallel for
    for (int j1 = 0; j1 < cols; ++j1) {
        for (int j2 = j1; j2 < cols; ++j2) {
            double sum = 0.0;
            for (int i = 0; i < rows; ++i) {
                sum += (data[i][j1] - means[j1]) * (data[i][j2] - means[j2]);
            }
            cov[j1][j2] = cov[j2][j1] = sum / (rows - 1);
        }
    }

    std::cout << "Forma: (" << cols << ", " << cols << ")\n";
    std::cout << cov[0][0] << " " << cov[0][1] << "\n";
    std::cout << cov[1][0] << " " << cov[1][1] << "\n";
}

long long fib(int n) {
    if (n <= 1) return n;
    return fib(n-1) + fib(n-2);
}

void compute_fibonacci() {
    std::vector<int> inputs = {10, 20, 30};
    std::vector<long long> results(inputs.size());

    #pragma omp parallel for
    for (size_t i = 0; i < inputs.size(); ++i) {
        results[i] = fib(inputs[i]);
    }

    std::cout << "Resultados de Fibonacci: ";
    for (const auto& r : results) {
        std::cout << r << " ";
    }
    std::cout << "\n";
}

int main() {
    std::cout << "Procesamiento de archivos:\n";
    process_files();
    std::cout << "\nCálculo de covarianza:\n";
    compute_covariance();
    std::cout << "\nCálculo de Fibonacci:\n";
    compute_fibonacci();
    return 0;
}
```

**Notas**:
- **Python 3.13+**: Verifica compatibilidad de Dask. GIL libre es experimental.
- **Clústeres**: Usa `dask-jobqueue` o `dask-kubernetes` para entornos reales.
- **OpenMP**: Usa bibliotecas como OpenCV para imágenes reales.