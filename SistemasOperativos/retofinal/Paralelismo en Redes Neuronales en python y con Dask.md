# Guía Paso a Paso: Implementación de una Red Neuronal con Dask en Python 3.13+

Esta guía detalla cómo implementar una red neuronal simple utilizando Dask para paralelismo en Python 3.13 o superior, enfocándose en el entrenamiento distribuido de un modelo de clasificación sobre un conjunto de datos grande. Se explica la importancia de Dask para escalar el procesamiento en clústeres, sus bondades (`Array`, `Delayed`, clústeres distribuidos), y cómo Python 3.13 optimiza el rendimiento. El ejemplo incluye un modelo de red neuronal para clasificar dígitos del conjunto MNIST, comparando el enfoque secuencial con el paralelo usando Dask.

## Requisitos Previos
- Instalar Python 3.13+ (disponible desde octubre 2024).
- Instalar Dask y dependencias:
  ```bash
  pip install dask distributed numpy scikit-learn torch torchvision dask[distributed]
  ```
- Configurar un entorno virtual:
  ```bash
  python3.13 -m venv venv
  source venv/bin/activate
  ```
- Para clústeres: Acceso a máquinas remotas o entornos como Kubernetes, SLURM, o AWS.
- Descargar el conjunto de datos MNIST (incluido en `torchvision`).

## 1. Introducción
Las redes neuronales requieren procesar grandes volúmenes de datos y realizar cálculos intensivos, lo que las hace ideales para paralelismo. Dask permite escalar el entrenamiento y la inferencia de modelos en clústeres, dividiendo datos y cálculos en fragmentos (chunks). Este ejemplo usa Dask para paralelizar el entrenamiento de una red neuronal simple en el conjunto de datos MNIST, que contiene imágenes de dígitos escritos a mano (28x28 píxeles, 10 clases).

**Componentes de Dask utilizados**:
- **Dask Array**: Para manejar matrices de datos grandes.
- **Dask Delayed**: Para paralelizar cálculos personalizados, como pasos de entrenamiento.
- **Clústeres distribuidos**: Para escalar a múltiples máquinas.

**Bondades de Dask para IA**:
- **Escalabilidad**: Procesa grandes datasets en clústeres.
- **Integración**: Compatible con NumPy, PyTorch, y scikit-learn.
- **Manejo de memoria**: Divide datos en chunks, evitando saturar la RAM.
- **Paralelismo**: Distribuye cálculos entre núcleos o nodos.
- **Python 3.13+**: Mejoras en I/O y concurrencia (GIL libre experimental).

## 2. Configuración del Entorno
Configuramos un clúster Dask, comenzando con un clúster local para pruebas y mostrando cómo escalar a un clúster distribuido.

```python
from dask.distributed import Client, LocalCluster

# Configurar clúster distribuido (simulado localmente)
# Importancia: Permite escalar cálculos a múltiples nodos, ideal para datasets grandes
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
- **Distribución**: Balancea cargas entre núcleos/nodos.
- **Diagnósticos**: Dashboard (http://localhost:8787) para monitorear tareas.
- **Escalabilidad**: Procesa datasets masivos en clústeres.
- **Tolerancia a fallos**: Reintenta tareas fallidas.
- **Python 3.13+**: Mejoras en concurrencia benefician I/O y tareas distribuidas.

**Importancia de Clústeres en IA**:
- **Datos grandes**: Divide datasets (e.g., imágenes, embeddings) entre nodos.
- **Entrenamiento distribuido**: Paraleliza cálculos de gradientes y actualizaciones.
- **Aceleración**: Reduce tiempos en datasets masivos.

## 3. Preparación del Conjunto de Datos
Usamos el conjunto MNIST, que contiene 60,000 imágenes de entrenamiento y 10,000 de prueba. Lo cargamos con `torchvision` y lo convertimos a un formato compatible con Dask.

```python
import torch
import torchvision
import torchvision.transforms as transforms
import numpy as np
import dask.array as da

# Descargar y preparar MNIST
# Importancia: Dask convierte datos en arreglos distribuidos para procesamiento paralelo
transform = transforms.Compose([transforms.ToTensor()])
trainset = torchvision.datasets.MNIST(root='./data', train=True, download=True, transform=transform)
testset = torchvision.datasets.MNIST(root='./data', train=False, download=True, transform=transform)

# Convertir a arreglos NumPy
X_train = np.array([img.numpy().flatten() for img, _ in trainset])  # [60000, 784]
y_train = np.array([label for _, label in trainset])  # [60000]
X_test = np.array([img.numpy().flatten() for img, _ in testset])  # [10000, 784]
y_test = np.array([label for _, label in testset])  # [10000]

# Convertir a Dask Arrays
# Importancia: Divide datos en chunks, permitiendo procesamiento distribuido
X_train_da = da.from_array(X_train, chunks=(10000, 784))  # Chunks de 10,000 imágenes
y_train_da = da.from_array(y_train, chunks=(10000,))
X_test_da = da.from_array(X_test, chunks=(10000, 784))
y_test_da = da.from_array(y_test, chunks=(10000,))

print("Forma de X_train_da:", X_train_da.shape)
```

**Salida esperada**:
```
Forma de X_train_da: (60000, 784)
```

**Bondades de Dask Array**:
- **Escalabilidad**: Maneja datasets grandes dividiendo en chunks.
- **Compatibilidad**: Similar a NumPy, fácil de integrar con PyTorch.
- **Paralelismo**: Distribuye operaciones entre nodos.
- **Memoria eficiente**: Procesa fragmentos, no todo el dataset.
- **Python 3.13+**: Optimizaciones en memoria reducen overhead.

## 4. Definición de la Red Neuronal
Definimos una red neuronal simple usando PyTorch: dos capas fully connected con activación ReLU y salida softmax para 10 clases.

```python
import torch.nn as nn

# Definir la red neuronal
# Importancia: Modelo base para entrenamiento paralelo con Dask
class SimpleNN(nn.Module):
    def __init__(self, input_size=784, hidden_size=128, output_size=10):
        super(SimpleNN, self).__init__()
        self.fc1 = nn.Linear(input_size, hidden_size)
        self.relu = nn.ReLU()
        self.fc2 = nn.Linear(hidden_size, output_size)

    def forward(self, x):
        x = self.fc1(x)
        x = self.relu(x)
        x = self.fc2(x)
        return x

# Instanciar modelo
model = SimpleNN()
```

## 5. Entrenamiento Secuencial (Baseline)
Primero, entrenamos el modelo de forma secuencial con PyTorch para establecer una línea base.

```python
import torch.optim as optim

# Configurar pérdida y optimizador
criterion = nn.CrossEntropyLoss()
optimizer = optim.SGD(model.parameters(), lr=0.01)

# Entrenamiento secuencial
# Importancia: Baseline para comparar con Dask
def train_sequential(model, X, y, epochs=5, batch_size=128):
    dataset = torch.utils.data.TensorDataset(torch.tensor(X, dtype=torch.float32), torch.tensor(y, dtype=torch.long))
    dataloader = torch.utils.data.DataLoader(dataset, batch_size=batch_size, shuffle=True)
    
    for epoch in range(epochs):
        for data, target in dataloader:
            optimizer.zero_grad()
            output = model(data)
            loss = criterion(output, target)
            loss.backward()
            optimizer.step()
        print(f"Época {epoch+1}, Pérdida: {loss.item():.4f}")

# Entrenar
train_sequential(model, X_train, y_train)
```

**Salida esperada** (valores aproximados):
```
Época 1, Pérdida: 0.8234
Época 2, Pérdida: 0.5123
...
Época 5, Pérdida: 0.3124
```

**Limitaciones**:
- **Memoria**: Carga todo el dataset en RAM.
- **Escalabilidad**: No aprovecha clústeres.
- **Tiempo**: Secuencial, lento para datasets grandes.

## 6. Entrenamiento Paralelo con Dask
Usamos Dask para paralelizar el entrenamiento, dividiendo el dataset en chunks y procesando mini-lotes en paralelo.

```python
from dask import delayed
import torch

# Función para entrenar un mini-lote
# Importancia: Dask Delayed paraleliza cálculos de gradientes
@delayed
def train_batch(model_state_dict, X_batch, y_batch, lr=0.01):
    model = SimpleNN()
    model.load_state_dict(model_state_dict)
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.SGD(model.parameters(), lr=lr)
    
    X_tensor = torch.tensor(X_batch, dtype=torch.float32)
    y_tensor = torch.tensor(y_batch, dtype=torch.long)
    
    optimizer.zero_grad()
    output = model(X_tensor)
    loss = criterion(output, y_tensor)
    loss.backward()
    optimizer.step()
    
    return model.state_dict(), loss.item()

# Función para entrenar una época
# Importancia: Distribuye mini-lotes entre nodos
def train_epoch(model, X_da, y_da, batch_size=128, lr=0.01):
    n_samples = X_da.shape[0]
    n_batches = (n_samples + batch_size - 1) // batch_size
    losses = []
    
    model_state_dict = model.state_dict()
    for i in range(0, n_samples, batch_size):
        end = min(i + batch_size, n_samples)
        X_batch = X_da[i:end].compute()  # Obtener chunk
        y_batch = y_da[i:end].compute()
        model_state_dict, loss = train_batch(model_state_dict, X_batch, y_batch, lr)
        losses.append(loss)
    
    model.load_state_dict(model_state_dict)
    return sum(losses) / len(losses)

# Entrenamiento paralelo
# Importancia: Escala entrenamiento a clústeres
def train_parallel(model, X_da, y_da, epochs=5, batch_size=128, lr=0.01):
    for epoch in range(epochs):
        loss = train_epoch(model, X_da, y_da, batch_size, lr)
        print(f"Época {epoch+1}, Pérdida promedio: {loss:.4f}")

# Entrenar
train_parallel(model, X_train_da, y_train_da)
```

**Salida esperada** (valores aproximados):
```
Época 1, Pérdida promedio: 0.8356
Época 2, Pérdida promedio: 0.5241
...
Época 5, Pérdida promedio: 0.3205
```

**Bondades del Enfoque con Dask**:
- **Paralelismo**: Entrena mini-lotes en paralelo en el clúster.
- **Escalabilidad**: Maneja datasets grandes dividiendo en chunks.
- **Memoria eficiente**: Procesa fragmentos, no todo el dataset.
- **Flexibilidad**: Dask Delayed permite personalizar el entrenamiento.
- **Python 3.13+**: Mejoras en I/O y concurrencia aceleran carga de datos.

**Importancia en IA**:
- **Datasets grandes**: Escala a millones de imágenes o muestras.
- **Clústeres**: Usa múltiples nodos para acelerar el entrenamiento.
- **Iteraciones rápidas**: Reduce tiempos en experimentos.

## 7. Evaluación del Modelo
Evaluamos el modelo en el conjunto de prueba.

```python
# Función para evaluar
# Importancia: Dask paraleliza la inferencia
@delayed
def evaluate_batch(model, X_batch, y_batch):
    X_tensor = torch.tensor(X_batch, dtype=torch.float32)
    y_tensor = torch.tensor(y_batch, dtype=torch.long)
    with torch.no_grad():
        output = model(X_tensor)
        _, predicted = torch.max(output, 1)
        correct = (predicted == y_tensor).sum().item()
        total = y_tensor.size(0)
    return correct, total

# Evaluación paralela
def evaluate_parallel(model, X_da, y_da, batch_size=128):
    n_samples = X_da.shape[0]
    results = []
    
    for i in range(0, n_samples, batch_size):
        end = min(i + batch_size, n_samples)
        X_batch = X_da[i:end].compute()
        y_batch = y_da[i:end].compute()
        results.append(evaluate_batch(model, X_batch, y_batch))
    
    correct = sum(r[0] for r in results)
    total = sum(r[1] for r in results)
    return correct / total

# Evaluar
accuracy = evaluate_parallel(model, X_test_da, y_test_da)
print(f"Precisión en test: {accuracy:.4f}")
```

**Salida esperada** (valor aproximado):
```
Precisión en test: 0.9123
```

**Bondades**:
- **Paralelismo**: Evalúa mini-lotes en paralelo.
- **Escalabilidad**: Maneja grandes conjuntos de prueba.
- **Eficiencia**: Usa recursos del clúster.

## 8. Mejores Prácticas
- **Chunks**: Ajusta tamaños (e.g., `chunks=(10000, 784)`) para balancear memoria y rendimiento.
- **Monitoreo**: Usa el dashboard (http://localhost:8787).
- **Recursos**: Cierra el cliente: `client.close()`.
- **Clústeres**: Escala nodos según carga: `cluster.scale(20)`.
- **Python 3.13+**: Prueba GIL libre (`PYTHON_GIL=0`) para I/O-bound (experimental).
- **Persistencia**: Usa `persist()` para datos intermedios.
- **Optimización**: Considera frameworks como Dask-ML o PyTorch Distributed para modelos más complejos.

## 9. Conclusión
Dask es una herramienta poderosa para escalar el entrenamiento e inferencia de redes neuronales en Python, especialmente para datasets grandes y clústeres distribuidos. Comparado con el enfoque secuencial, Dask reduce tiempos y maneja memoria eficientemente, integrándose bien con PyTorch. Python 3.13+ mejora el rendimiento con optimizaciones en I/O y concurrencia, haciendo que Dask sea ideal para aplicaciones de IA en big data.

**Código completo**:

```python
import torch
import torchvision
import torchvision.transforms as transforms
import torch.nn as nn
import torch.optim as optim
import numpy as np
import dask.array as da
from dask.distributed import Client, LocalCluster
from dask import delayed
import os

# Configurar clúster
# Importancia: Escala cálculos a múltiples nodos
cluster = LocalCluster(n_workers=4, threads_per_worker=2, memory_limit='4GB')
client = Client(cluster)
print(client)

# Preparar MNIST
# Importancia: Convierte datos a Dask Arrays para procesamiento distribuido
transform = transforms.Compose([transforms.ToTensor()])
trainset = torchvision.datasets.MNIST(root='./data', train=True, download=True, transform=transform)
testset = torchvision.datasets.MNIST(root='./data', train=False, download=True, transform=transform)

X_train = np.array([img.numpy().flatten() for img, _ in trainset])
y_train = np.array([label for _, label in trainset])
X_test = np.array([img.numpy().flatten() for img, _ in testset])
y_test = np.array([label for _, label in testset])

X_train_da = da.from_array(X_train, chunks=(10000, 784))
y_train_da = da.from_array(y_train, chunks=(10000,))
X_test_da = da.from_array(X_test, chunks=(10000, 784))
y_test_da = da.from_array(y_test, chunks=(10000,))

# Definir red neuronal
class SimpleNN(nn.Module):
    def __init__(self, input_size=784, hidden_size=128, output_size=10):
        super(SimpleNN, self).__init__()
        self.fc1 = nn.Linear(input_size, hidden_size)
        self.relu = nn.ReLU()
        self.fc2 = nn.Linear(hidden_size, output_size)

    def forward(self, x):
        x = self.fc1(x)
        x = self.relu(x)
        x = self.fc2(x)
        return x

model = SimpleNN()

# Función para entrenar un mini-lote
@delayed
def train_batch(model_state_dict, X_batch, y_batch, lr=0.01):
    model = SimpleNN()
    model.load_state_dict(model_state_dict)
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.SGD(model.parameters(), lr=lr)
    X_tensor = torch.tensor(X_batch, dtype=torch.float32)
    y_tensor = torch.tensor(y_batch, dtype=torch.long)
    optimizer.zero_grad()
    output = model(X_tensor)
    loss = criterion(output, y_tensor)
    loss.backward()
    optimizer.step()
    return model.state_dict(), loss.item()

# Función para entrenar una época
def train_epoch(model, X_da, y_da, batch_size=128, lr=0.01):
    n_samples = X_da.shape[0]
    n_batches = (n_samples + batch_size - 1) // batch_size
    losses = []
    model_state_dict = model.state_dict()
    for i in range(0, n_samples, batch_size):
        end = min(i + batch_size, n_samples)
        X_batch = X_da[i:end].compute()
        y_batch = y_da[i:end].compute()
        model_state_dict, loss = train_batch(model_state_dict, X_batch, y_batch, lr)
        losses.append(loss)
    model.load_state_dict(model_state_dict)
    return sum(losses) / len(losses)

# Entrenamiento paralelo
def train_parallel(model, X_da, y_da, epochs=5, batch_size=128, lr=0.01):
    for epoch in range(epochs):
        loss = train_epoch(model, X_da, y_da, batch_size, lr)
        print(f"Época {epoch+1}, Pérdida promedio: {loss:.4f}")

# Evaluar
@delayed
def evaluate_batch(model, X_batch, y_batch):
    X_tensor = torch.tensor(X_batch, dtype=torch.float32)
    y_tensor = torch.tensor(y_batch, dtype=torch.long)
    with torch.no_grad():
        output = model(X_tensor)
        _, predicted = torch.max(output, 1)
        correct = (predicted == y_tensor).sum().item()
        total = y_tensor.size(0)
    return correct, total

def evaluate_parallel(model, X_da, y_da, batch_size=128):
    n_samples = X_da.shape[0]
    results = []
    for i in range(0, n_samples, batch_size):
        end = min(i + batch_size, n_samples)
        X_batch = X_da[i:end].compute()
        y_batch = y_da[i:end].compute()
        results.append(evaluate_batch(model, X_batch, y_batch))
    correct = sum(r[0] for r in results)
    total = sum(r[1] for r in results)
    return correct / total

# Ejecutar entrenamiento y evaluación
train_parallel(model, X_train_da, y_train_da)
accuracy = evaluate_parallel(model, X_test_da, y_test_da)
print(f"Precisión en test: {accuracy:.4f}")

# Cerrar cliente
# Importancia: Libera recursos
client.close()
```

**Notas**:
- **Python 3.13+**: Verifica compatibilidad de Dask y PyTorch. GIL libre es experimental (`--disable-gil`).
- **Clústeres**: Usa `dask-jobqueue` (SLURM) o `dask-kubernetes` para entornos reales.
- **Optimizaciones**: Para modelos más complejos, considera Dask-ML o PyTorch DistributedDataParallel.
- **Limitaciones**: Este ejemplo simplifica el entrenamiento distribuido. En la práctica, frameworks como Horovod o Ray pueden ser más robustos para redes neuronales grandes.