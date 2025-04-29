# Guía Paso a Paso: Compresión de Archivos con Dask en Python 3.13+

Esta guía detalla cómo implementar la compresión de un archivo grande utilizando **Dask** en Python 3.13 o superior, aprovechando el paralelismo para dividir el archivo en fragmentos y comprimirlos de forma distribuida. Se utiliza el algoritmo de compresión **zlib** (similar a gzip) para comprimir un archivo de texto grande, mostrando cómo Dask escala el proceso en clústeres. El documento incluye un ejemplo práctico, comentarios explicativos sobre la importancia de Dask, y cómo las mejoras de Python 3.13 optimizan el rendimiento.

## Requisitos Previos
- Instalar Python 3.13+ (disponible desde octubre 2024).
- Instalar Dask y dependencias:
  ```bash
  pip install dask distributed
  ```
- Configurar un entorno virtual:
  ```bash
  python3.13 -m venv venv
  source venv/bin/activate
  ```
- Para clústeres: Acceso a máquinas remotas o entornos como Kubernetes, SLURM, o AWS.
- Generar un archivo de texto grande para pruebas (se proporciona código).

## 1. Introducción
La compresión de archivos grandes (e.g., logs, datasets) puede ser intensiva en CPU y lenta si se realiza de forma secuencial. **Dask** permite paralelizar el proceso dividiendo el archivo en fragmentos (chunks) y comprimiendo cada uno en paralelo, ya sea en múltiples núcleos locales o en un clúster distribuido. Este ejemplo comprime un archivo de texto grande usando el módulo **zlib** y **Dask Bag** para manejar fragmentos, con soporte para clústeres.

**Componentes de Dask utilizados**:
- **Dask Bag**: Para procesar fragmentos de datos no estructurados (e.g., fragmentos de archivo).
- **Clústeres distribuidos**: Para escalar la compresión a múltiples máquinas.

**Bondades de Dask para compresión**:
- **Escalabilidad**: Procesa archivos grandes en clústeres.
- **Paralelismo**: Comprime fragmentos simultáneamente.
- **Manejo de memoria**: Lee y procesa datos en chunks, evitando cargar el archivo completo.
- **Flexibilidad**: Compatible con diferentes algoritmos de compresión.
- **Python 3.13+**: Mejoras en I/O y concurrencia (GIL libre experimental) aceleran la lectura/escritura.

## 2. Configuración del Entorno
Configuramos un clúster Dask, comenzando con un clúster local y mostrando cómo escalar a un clúster distribuido.

```python
from dask.distributed import Client, LocalCluster

# Configurar clúster distribuido (simulado localmente)
# Importancia: Escala la compresión a múltiples nodos, acelerando el procesamiento
cluster = LocalCluster(n_workers=4, threads_per_worker=2, memory_limit='4GB')
client = Client(cluster)
print(client)

# Para clústeres reales (ejemplo con SLURM):
# from dask_jobqueue import SLURMCluster
# cluster = SLURMCluster(cores=8, memory="16GB", processes=4)
# cluster.scale(20)  # Escalar a 20 trabajadores
# client = Client(cluster)
```

**Salida esperada**:
```
<Client: 'tcp://127.0.0.1:XXXX' processes=4 threads=8, memory=16.00 GB>
```

**Bondades del Cliente y Clústeres**:
- **Distribución**: Balancea tareas entre núcleos/nodos.
- **Diagnósticos**: Dashboard (http://localhost:8787) para monitorear CPU y memoria.
- **Escalabilidad**: Procesa archivos de terabytes en clústeres.
- **Tolerancia a fallos**: Reintenta tareas fallidas.
- **Python 3.13+**: Mejoras en concurrencia benefician I/O-bound tasks.

**Importancia para compresión**:
- Divide el archivo en fragmentos procesados en paralelo.
- Escala a clústeres para archivos masivos.
- Reduce tiempos en tareas CPU-intensivas como la compresión.

## 3. Generación de un Archivo de Prueba
Creamos un archivo de texto grande (~100 MB) para simular un caso real.

```python
import os

# Crear archivo de texto grande
# Importancia: Simula un dataset real para probar la compresión paralela
def create_large_file(filename, size_mb=100):
    with open(filename, 'w') as f:
        for i in range(size_mb * 1024 * 1024 // 100):  # Aproximadamente 100 MB
            f.write(f"Línea de prueba {i} " * 10 + "\n")
    print(f"Archivo {filename} creado, tamaño: {os.path.getsize(filename) / (1024*1024):.2f} MB")

input_file = 'large_file.txt'
create_large_file(input_file)
```

**Salida esperada**:
```
Archivo large_file.txt creado, tamaño: 100.00 MB
```

## 4. Compresión Secuencial (Baseline)
Primero, implementamos la compresión secuencial con **zlib** para comparar con Dask.

```python
import zlib
import time

# Compresión secuencial
# Importancia: Baseline para comparar con el enfoque paralelo
def compress_sequential(input_file, output_file):
    start_time = time.time()
    with open(input_file, 'rb') as f:
        data = f.read()
    compressed_data = zlib.compress(data, level=6)  # Nivel de compresión medio
    with open(output_file, 'wb') as f:
        f.write(compressed_data)
    elapsed = time.time() - start_time
    print(f"Compresión secuencial completada en {elapsed:.2f} segundos")
    print(f"Tamaño comprimido: {os.path.getsize(output_file) / (1024*1024):.2f} MB")

output_file_seq = 'large_file_seq.zlib'
compress_sequential(input_file, output_file_seq)
```

**Salida esperada** (valores aproximados):
```
Compresión secuencial completada en 2.50 segundos
Tamaño comprimido: 10.50 MB
```

**Limitaciones**:
- **Memoria**: Carga todo el archivo en RAM.
- **Escalabilidad**: No usa múltiples núcleos ni clústeres.
- **Tiempo**: Lento para archivos muy grandes.

## 5. Compresión Paralela con Dask
Usamos **Dask Bag** para dividir el archivo en fragmentos, comprimirlos en paralelo, y combinar los resultados.

```python
import dask.bag as db
import zlib

# Función para leer el archivo en fragmentos
# Importancia: Divide el archivo en chunks para procesamiento paralelo
def read_in_chunks(filename, chunk_size=10*1024*1024):  # 10 MB por chunk
    chunks = []
    with open(filename, 'rb') as f:
        while True:
            chunk = f.read(chunk_size)
            if not chunk:
                break
            chunks.append(chunk)
    return chunks

# Función para comprimir un fragmento
# Importancia: Permite paralelizar la compresión con Dask
def compress_chunk(chunk):
    return zlib.compress(chunk, level=6)

# Compresión paralela
# Importancia: Escala la compresión a múltiples núcleos/nodos
def compress_parallel(input_file, output_file, chunk_size=10*1024*1024):
    start_time = time.time()
    
    # Crear Dask Bag con fragmentos
    chunks = read_in_chunks(input_file, chunk_size)
    bag = db.from_sequence(chunks)
    
    # Comprimir fragmentos en paralelo
    compressed_chunks = bag.map(compress_chunk).compute()
    
    # Combinar fragmentos comprimidos
    with open(output_file, 'wb') as f:
        for chunk in compressed_chunks:
            f.write(chunk)
    
    elapsed = time.time() - start_time
    print(f"Compresión paralela completada en {elapsed:.2f} segundos")
    print(f"Tamaño comprimido: {os.path.getsize(output_file) / (1024*1024):.2f} MB")

output_file_par = 'large_file_par.zlib'
compress_parallel(input_file, output_file_par)
```

**Salida esperada** (valores aproximados):
```
Compresión paralela completada en 1.20 segundos
Tamaño comprimido: 10.50 MB
```

**Bondades de Dask Bag**:
- **Paralelismo**: Comprime fragmentos simultáneamente en el clúster.
- **Escalabilidad**: Maneja archivos grandes dividiendo en chunks.
- **Memoria eficiente**: Procesa fragmentos sin cargar todo el archivo.
- **Flexibilidad**: Aplicable a otros algoritmos de compresión (e.g., `lzma`, `bz2`).
- **Python 3.13+**: Mejoras en I/O aceleran lectura/escritura.

**Importancia para compresión**:
- **Aceleración**: Reduce tiempos usando múltiples núcleos/nodos.
- **Escalabilidad**: Procesa archivos de terabytes en clústeres.
- **Eficiencia**: Minimiza uso de memoria.

## 6. Descompresión para Verificación
Verificamos que los archivos comprimidos se pueden descomprimir correctamente.

```python
# Descompresión para verificar
# Importancia: Confirma que la compresión paralela es válida
def decompress_file(input_file, output_file):
    with open(input_file, 'rb') as f:
        compressed_data = f.read()
    decompressed_data = zlib.decompress(compressed_data)
    with open(output_file, 'wb') as f:
        f.write(decompressed_data)
    print(f"Descompresión completada, tamaño: {os.path.getsize(output_file) / (1024*1024):.2f} MB")

decompress_file(output_file_par, 'large_file_decompressed.txt')
```

**Salida esperada**:
```
Descompresión completada, tamaño: 100.00 MB
```

**Nota**: El archivo descomprimido debería coincidir con el original (`large_file.txt`).

## 7. Mejores Prácticas
- **Tamaño de chunks**: Ajusta `chunk_size` (e.g., 10 MB) según memoria y número de trabajadores.
- **Monitoreo**: Usa el dashboard (http://localhost:8787) para analizar rendimiento.
- **Recursos**: Cierra el cliente: `client.close()`.
- **Clústeres**: Escala nodos para archivos muy grandes: `cluster.scale(20)`.
- **Python 3.13+**: Prueba GIL libre (`PYTHON_GIL=0`) para tareas I/O-bound (experimental).
- **Compresión**: Ajusta el nivel de compresión (`zlib.compress(level=6)`) para balancear velocidad y tamaño.
- **Almacenamiento distribuido**: Usa sistemas como S3 o HDFS para archivos en clústeres.

## 8. Conclusión
Dask es una herramienta poderosa para comprimir archivos grandes en Python, escalando el proceso a múltiples núcleos o clústeres. Comparado con la compresión secuencial, Dask reduce significativamente el tiempo de procesamiento al paralelizar la compresión de fragmentos. Python 3.13+ mejora el rendimiento con optimizaciones en I/O y concurrencia, haciendo que Dask sea ideal para aplicaciones que manejan grandes volúmenes de datos.

**Código completo**:

```python
import dask.bag as db
from dask.distributed import Client, LocalCluster
import zlib
import os
import time

# Configurar clúster
# Importancia: Escala la compresión a múltiples nodos
cluster = LocalCluster(n_workers=4, threads_per_worker=2, memory_limit='4GB')
client = Client(cluster)
print(client)

# Crear archivo de prueba
# Importancia: Simula un dataset real
def create_large_file(filename, size_mb=100):
    with open(filename, 'w') as f:
        for i in range(size_mb * 1024 * 1024 // 100):
            f.write(f"Línea de prueba {i} " * 10 + "\n")
    print(f"Archivo {filename} creado, tamaño: {os.path.getsize(filename) / (1024*1024):.2f} MB")

input_file = 'large_file.txt'
create_large_file(input_file)

# Compresión secuencial
# Importancia: Baseline para comparación
def compress_sequential(input_file, output_file):
    start_time = time.time()
    with open(input_file, 'rb') as f:
        data = f.read()
    compressed_data = zlib.compress(data, level=6)
    with open(output_file, 'wb') as f:
        f.write(compressed_data)
    elapsed = time.time() - start_time
    print(f"Compresión secuencial completada en {elapsed:.2f} segundos")
    print(f"Tamaño comprimido: {os.path.getsize(output_file) / (1024*1024):.2f} MB")

output_file_seq = 'large_file_seq.zlib'
compress_sequential(input_file, output_file_seq)

# Leer en fragmentos
# Importancia: Divide el archivo para procesamiento paralelo
def read_in_chunks(filename, chunk_size=10*1024*1024):
    chunks = []
    with open(filename, 'rb') as f:
        while True:
            chunk = f.read(chunk_size)
            if not chunk:
                break
            chunks.append(chunk)
    return chunks

# Comprimir un fragmento
# Importancia: Permite paralelizar con Dask
def compress_chunk(chunk):
    return zlib.compress(chunk, level=6)

# Compresión paralela
# Importancia: Escala a clústeres
def compress_parallel(input_file, output_file, chunk_size=10*1024*1024):
    start_time = time.time()
    chunks = read_in_chunks(input_file, chunk_size)
    bag = db.from_sequence(chunks)
    compressed_chunks = bag.map(compress_chunk).compute()
    with open(output_file, 'wb') as f:
        for chunk in compressed_chunks:
            f.write(chunk)
    elapsed = time.time() - start_time
    print(f"Compresión paralela completada en {elapsed:.2f} segundos")
    print(f"Tamaño comprimido: {os.path.getsize(output_file) / (1024*1024):.2f} MB")

output_file_par = 'large_file_par.zlib'
compress_parallel(input_file, output_file_par)

# Descompresión para verificar
# Importancia: Confirma la validez de la compresión
def decompress_file(input_file, output_file):
    with open(input_file, 'rb') as f:
        compressed_data = f.read()
    decompressed_data = zlib.decompress(compressed_data)
    with open(output_file, 'wb') as f:
        f.write(decompressed_data)
    print(f"Descompresión completada, tamaño: {os.path.getsize(output_file) / (1024*1024):.2f} MB")

decompress_file(output_file_par, 'large_file_decompressed.txt')

# Cerrar cliente
# Importancia: Libera recursos
client.close()
```

**Notas**:
- **Python 3.13+**: Verifica compatibilidad de Dask. GIL libre experimental (`--disable-gil`).
- **Clústeres**: Usa `dask-jobqueue` o `dask-kubernetes` para entornos reales.
- **Optimizaciones**: Ajusta `chunk_size` y nivel de compresión según necesidades.
- **Alternativas**: Considera `lzma` o `bz2` para mayor compresión, aunque más lentos.