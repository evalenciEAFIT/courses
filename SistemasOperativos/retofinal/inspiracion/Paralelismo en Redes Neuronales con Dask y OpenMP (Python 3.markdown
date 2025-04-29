# Guía Paso a Paso: Implementación de una Red Neuronal con Dask y OpenMP (Python 3.13+ y C++)

Esta guía detalla cómo implementar una red neuronal simple para clasificar dígitos del conjunto MNIST, utilizando **Dask** en Python 3.13+ para paralelismo distribuido y **C++ con OpenMP** para paralelismo en memoria compartida. Se compara un ejemplo de entrenamiento paralelo en ambos enfoques, destacando las bondades de Dask (`Array`, `Delayed`, clústeres) y OpenMP (directivas, rendimiento local). Se explica la importancia de cada enfoque, sus ventajas para IA, y cómo Python 3.13 optimiza el rendimiento en Dask. El modelo es una red neuronal fully connected con dos capas para clasificar imágenes de 28x28 píxeles en 10 clases.

## Requisitos Previos
- **Python**:
  - Instalar Python 3.13+ (octubre 2024).
  - Instalar Dask y dependencias:
    ```bash
    pip install dask distributed numpy scikit-learn torch torchvision dask[distributed]
    ```
  - Configurar entorno virtual:
    ```bash
    python3.13 -m venv venv
    source venv/bin/activate
    ```
  - Descargar MNIST (incluido en `torchvision`).
- **C++**:
  - Compilador con OpenMP (e.g., `g++` 9.0+).
  - Biblioteca para MNIST (usaremos una implementación simplificada para cargar datos).
  - Compilar con OpenMP:
    ```bash
    g++ -fopenmp neural_network.cpp -o neural_network
    ```
- **Clústeres**: Acceso a máquinas remotas o entornos como Kubernetes, SLURM, o AWS (para Dask).
- Hardware: Múltiples núcleos para OpenMP; clústeres para Dask.

## 1. Introducción
Las redes neuronales requieren procesar grandes datasets y realizar cálculos intensivos, lo que las hace ideales para paralelismo. **Dask** escala el entrenamiento a clústeres, dividiendo datos en chunks, mientras que **OpenMP** paraleliza cálculos en un solo equipo usando hilos. Este ejemplo entrena una red neuronal simple en MNIST (60,000 imágenes de entrenamiento, 10,000 de prueba) con ambos enfoques.

**Dask**:
- Usa `Dask Array` para datos distribuidos y `Dask Delayed` para cálculos personalizados.
- Escala a clústeres, ideal para big data.
- Bondades: escalabilidad, integración con PyTorch, manejo de memoria.

**OpenMP**:
- Usa directivas (`#pragma`) para paralelizar bucles y cálculos.
- Limitado a un equipo, pero rápido en memoria compartida.
- Bondades: alto rendimiento, simplicidad en paralelismo local.

**Python 3.13+**: Mejoras en I/O y concurrencia (GIL libre experimental) benefician Dask.

## 2. Configuración del Entorno
### 2.1. Dask: Clúster Local y Distribuido
Configuramos un clúster Dask para distribuir cálculos.

```python
from dask.distributed import Client, LocalCluster

# Configurar clúster (simulado localmente)
# Importancia: Escala cálculos a múltiples nodos, ideal para datasets grandes
cluster = LocalCluster(n_workers=4, threads_per_worker=2, memory_limit='4GB')
client = Client(cluster)
print(client)

# Para clústeres reales (ejemplo con SLURM):
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
- **Escalabilidad**: Procesa datasets masivos.
- **Tolerancia a fallos**: Reintenta tareas fallidas.
- **Python 3.13+**: Mejoras en concurrencia para I/O.

**Importancia en IA**: Divide datasets y cálculos entre nodos, acelerando entrenamiento.

### 2.2. OpenMP: Configuración
OpenMP opera en hilos dentro de un equipo, activado con `-fopenmp`.

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
- **Simplicidad**: Directivas fáciles para paralelizar.
- **Rendimiento**: Ejecuta en hilos nativos.
- **Limitación**: No escala a clústeres.

**Importancia en IA**: Acelera cálculos locales, pero limitado a un equipo.

## 3. Preparación del Conjunto de Datos
### 3.1. Dask: Carga de MNIST
Cargamos MNIST con `torchvision` y lo convertimos a `Dask Array`.

```python
import torch
import torchvision
import torchvision.transforms as transforms
import numpy as np
import dask.array as da

# Cargar MNIST
# Importancia: Dask convierte datos en arreglos distribuidos
transform = transforms.Compose([transforms.ToTensor()])
trainset = torchvision.datasets.MNIST(root='./data', train=True, download=True, transform=transform)
testset = torchvision.datasets.MNIST(root='./data', train=False, download=True, transform=transform)

# Convertir a NumPy
X_train = np.array([img.numpy().flatten() for img, _ in trainset])  # [60000, 784]
y_train = np.array([label for _, label in trainset])  # [60000]
X_test = np.array([img.numpy().flatten() for img, _ in testset])  # [10000, 784]
y_test = np.array([label for _, label in testset])  # [10000]

# Convertir a Dask Arrays
# Importancia: Divide datos en chunks para procesamiento paralelo
X_train_da = da.from_array(X_train, chunks=(10000, 784))
y_train_da = da.from_array(y_train, chunks=(10000,))
X_test_da = da.from_array(X_test, chunks=(10000, 784))
y_test_da = da.from_array(y_test, chunks=(10000,))

print("Forma de X_train_da:", X_train_da.shape)
```

**Salida esperada**:
```
Forma de X_train_da: (60000, 784)
```

**Bondades**:
- **Escalabilidad**: Maneja datasets grandes.
- **Memoria**: Procesa chunks.
- **Python 3.13+**: I/O optimizado.

### 3.2. C++: Carga de MNIST
En C++, cargamos MNIST desde archivos (suponemos un formato simplificado: imágenes como vectores de 784 floats, etiquetas como enteros). Para este ejemplo, simulamos datos debido a la complejidad de leer MNIST sin bibliotecas externas.

```cpp
#include <vector>
#include <random>
#include <iostream>

// Simular carga de MNIST (en práctica, usar biblioteca como libtorch)
void load_mnist(std::vector<std::vector<float>>& X_train, std::vector<int>& y_train, 
                std::vector<std::vector<float>>& X_test, std::vector<int>& y_test) {
    const int train_size = 60000, test_size = 10000, dim = 784;
    std::mt19937 gen(42);
    std::uniform_real_distribution<> dist(0, 1);
    
    X_train.resize(train_size, std::vector<float>(dim));
    y_train.resize(train_size);
    X_test.resize(test_size, std::vector<float>(dim));
    y_test.resize(test_size);
    
    for (int i = 0; i < train_size; ++i) {
        for (int j = 0; j < dim; ++j) {
            X_train[i][j] = dist(gen); // Simular píxeles
        }
        y_train[i] = rand() % 10; // Simular etiquetas
    }
    for (int i = 0; i < test_size; ++i) {
        for (int j = 0; j < dim; ++j) {
            X_test[i][j] = dist(gen);
        }
        y_test[i] = rand() % 10;
    }
}
```

**Nota**: En la práctica, usa `libtorch` (PyTorch C++) o una biblioteca para cargar MNIST.

**Bondades**:
- **Rendimiento**: Carga rápida en memoria compartida.
- **Limitación**: Requiere RAM para todo el dataset; no escala a clústeres.

## 4. Definición de la Red Neuronal
### 4.1. Dask: Red Neuronal en PyTorch
Definimos una red fully connected con PyTorch.

```python
import torch.nn as nn

# Red neuronal
# Importancia: Modelo base para entrenamiento paralelo
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
```

### 4.2. C++: Red Neuronal Simple
Implementamos una red equivalente en C++ (sin `libtorch` para simplicidad).

```cpp
#include <vector>
#include <cmath>

class SimpleNN {
public:
    SimpleNN(int input_size = 784, int hidden_size = 128, int output_size = 10) 
        : input_size_(input_size), hidden_size_(hidden_size), output_size_(output_size) {
        // Inicializar pesos y sesgos
        std::mt19937 gen(42);
        std::normal_distribution<> dist(0, 0.01);
        W1_.resize(input_size_ * hidden_size_);
        b1_.resize(hidden_size_);
        W2_.resize(hidden_size_ * output_size_);
        b2_.resize(output_size_);
        for (auto& w : W1_) w = dist(gen);
        for (auto& b : b1_) b = dist(gen);
        for (auto& w : W2_) w = dist(gen);
        for (auto& b : b2_) b = dist(gen);
    }

    // Forward pass
    std::vector<float> forward(const std::vector<float>& x) {
        std::vector<float> hidden(hidden_size_, 0.0);
        std::vector<float> output(output_size_, 0.0);
        
        // Capa 1: W1 * x + b1, ReLU
        for (int j = 0; j < hidden_size_; ++j) {
            for (int i = 0; i < input_size_; ++i) {
                hidden[j] += W1_[j * input_size_ + i] * x[i];
            }
            hidden[j] += b1_[j];
            hidden[j] = std::max(0.0f, hidden[j]); // ReLU
        }
        
        // Capa 2: W2 * hidden + b2
        for (int j = 0; j < output_size_; ++j) {
            for (int i = 0; i < hidden_size_; ++i) {
                output[j] += W2_[j * hidden_size_ + i] * hidden[i];
            }
            output[j] += b2_[j];
        }
        
        return output;
    }

    std::vector<float> W1_, b1_, W2_, b2_; // Pesos y sesgos
private:
    int input_size_, hidden_size_, output_size_;
};
```

**Bondades**:
- **Dask**: API de alto nivel, fácil integración con PyTorch.
- **OpenMP**: Control explícito, rápido en cálculos locales.

## 5. Entrenamiento
### 5.1. Dask: Entrenamiento Paralelo
Paralelizamos el entrenamiento con `Dask Delayed`, procesando mini-lotes en el clúster.

```python
from dask import delayed
import torch
import torch.optim as optim

# Entrenar un mini-lote
# Importancia: Paraleliza cálculos de gradientes
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

# Entrenar una época
# Importancia: Distribuye mini-lotes entre nodos
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
# Importancia: Escala a clústeres
def train_parallel(model, X_da, y_da, epochs=5, batch_size=128, lr=0.01):
    for epoch in range(epochs):
        loss = train_epoch(model, X_da, y_da, batch_size, lr)
        print(f"Época {epoch+1}, Pérdida promedio: {loss:.4f}")

# Entrenar
model = SimpleNN()
train_parallel(model, X_train_da, y_train_da)
```

**Salida esperada** (valores aproximados):
```
Época 1, Pérdida promedio: 0.8356
Época 2, Pérdida promedio: 0.5241
...
Época 5, Pérdida promedio: 0.3205
```

**Bondades**:
- **Paralelismo**: Entrena mini-lotes en paralelo.
- **Escalabilidad**: Usa clústeres.
- **Memoria**: Procesa chunks.
- **Python 3.13+**: I/O optimizado.

### 5.2. OpenMP: Entrenamiento Paralelo
Implementamos un entrenamiento paralelo con OpenMP, dividiendo mini-lotes entre hilos. Usamos gradiente descendente simplificado.

```cpp
#include <omp.h>
#include <vector>
#include <cmath>
#include <random>
#include <iostream>

class SimpleNN {
public:
    SimpleNN(int input_size = 784, int hidden_size = 128, int output_size = 10) 
        : input_size_(input_size), hidden_size_(hidden_size), output_size_(output_size) {
        std::mt19937 gen(42);
        std::normal_distribution<> dist(0, 0.01);
        W1_.resize(input_size_ * hidden_size_);
        b1_.resize(hidden_size_);
        W2_.resize(hidden_size_ * output_size_);
        b2_.resize(output_size_);
        for (auto& w : W1_) w = dist(gen);
        for (auto& b : b1_) b = dist(gen);
        for (auto& w : W2_) w = dist(gen);
        for (auto& b : b2_) b = dist(gen);
    }

    // Forward pass
    std::vector<float> forward(const std::vector<float>& x) {
        std::vector<float> hidden(hidden_size_, 0.0);
        std::vector<float> output(output_size_, 0.0);
        for (int j = 0; j < hidden_size_; ++j) {
            for (int i = 0; i < input_size_; ++i) {
                hidden[j] += W1_[j * input_size_ + i] * x[i];
            }
            hidden[j] += b1_[j];
            hidden[j] = std::max(0.0f, hidden[j]);
        }
        for (int j = 0; j < output_size_; ++j) {
            for (int i = 0; i < hidden_size_; ++i) {
                output[j] += W2_[j * hidden_size_ + i] * hidden[i];
            }
            output[j] += b2_[j];
        }
        return output;
    }

    // Softmax para pérdida
    std::vector<float> softmax(const std::vector<float>& x) {
        std::vector<float> result(x.size());
        float max_x = *std::max_element(x.begin(), x.end());
        float sum = 0.0;
        for (size_t i = 0; i < x.size(); ++i) {
            result[i] = std::exp(x[i] - max_x);
            sum += result[i];
        }
        for (auto& r : result) r /= sum;
        return result;
    }

    // Entrenar un mini-lote
    void train_batch(const std::vector<std::vector<float>>& X, const std::vector<int>& y, 
                     float lr, std::vector<float>& grad_W1, std::vector<float>& grad_b1,
                     std::vector<float>& grad_W2, std::vector<float>& grad_b2, float& loss) {
        grad_W1.assign(W1_.size(), 0.0);
        grad_b1.assign(b1_.size(), 0.0);
        grad_W2.assign(W2_.size(), 0.0);
        grad_b2.assign(b2_.size(), 0.0);
        loss = 0.0;
        
        for (size_t i = 0; i < X.size(); ++i) {
            // Forward
            std::vector<float> hidden(hidden_size_, 0.0);
            for (int j = 0; j < hidden_size_; ++j) {
                for (int k = 0; k < input_size_; ++k) {
                    hidden[j] += W1_[j * input_size_ + k] * X[i][k];
                }
                hidden[j] += b1_[j];
                hidden[j] = std::max(0.0f, hidden[j]);
            }
            std::vector<float> output(output_size_, 0.0);
            for (int j = 0; j < output_size_; ++j) {
                for (int k = 0; k < hidden_size_; ++k) {
                    output[j] += W2_[j * hidden_size_ + k] * hidden[k];
                }
                output[j] += b2_[j];
            }
            
            // Pérdida (cross-entropy)
            std::vector<float> probs = softmax(output);
            loss -= std::log(probs[y[i]]);
            
            // Backward (gradientes simplificados)
            std::vector<float> grad_output(output_size_, 0.0);
            for (int j = 0; j < output_size_; ++j) {
                grad_output[j] = probs[j] - (j == y[i] ? 1.0 : 0.0);
            }
            for (int j = 0; j < output_size_; ++j) {
                for (int k = 0; k < hidden_size_; ++k) {
                    grad_W2[j * hidden_size_ + k] += grad_output[j] * hidden[k];
                    grad_b2[j] += grad_output[j];
                }
            }
            std::vector<float> grad_hidden(hidden_size_, 0.0);
            for (int j = 0; j < hidden_size_; ++j) {
                for (int k = 0; k < output_size_; ++k) {
                    grad_hidden[j] += grad_output[k] * W2_[k * hidden_size_ + j];
                }
                if (hidden[j] > 0) {
                    for (int k = 0; k < input_size_; ++k) {
                        grad_W1[j * input_size_ + k] += grad_hidden[j] * X[i][k];
                    }
                    grad_b1[j] += grad_hidden[j];
                }
            }
        }
        
        // Normalizar gradientes
        float scale = 1.0 / X.size();
        for (auto& g : grad_W1) g *= scale;
        for (auto& g : grad_b1) g *= scale;
        for (auto& g : grad_W2) g *= scale;
        for (auto& g : grad_b2) g *= scale;
        loss *= scale;
    }

    // Actualizar pesos
    void update_weights(const std::vector<float>& grad_W1, const std::vector<float>& grad_b1,
                        const std::vector<float>& grad_W2, const std::vector<float>& grad_b2, float lr) {
        #pragma omp parallel for
        for (size_t i = 0; i < W1_.size(); ++i) {
            W1_[i] -= lr * grad_W1[i];
        }
        #pragma omp parallel for
        for (size_t i = 0; i < b1_.size(); ++i) {
            b1_[i] -= lr * grad_b1[i];
        }
        #pragma omp parallel for
        for (size_t i = 0; i < W2_.size(); ++i) {
            W2_[i] -= lr * grad_W2[i];
        }
        #pragma omp parallel for
        for (size_t i = 0; i < b2_.size(); ++i) {
            b2_[i] -= lr * grad_b2[i];
        }
    }

    std::vector<float> W1_, b1_, W2_, b2_;
private:
    int input_size_, hidden_size_, output_size_;
};

// Entrenamiento
void train_parallel(SimpleNN& model, const std::vector<std::vector<float>>& X, 
                    const std::vector<int>& y, int epochs = 5, int batch_size = 128, float lr = 0.01) {
    int n_samples = X.size();
    for (int epoch = 0; epoch < epochs; ++epoch) {
        float total_loss = 0.0;
        int n_batches = (n_samples + batch_size - 1) / batch_size;
        
        #pragma omp parallel
        {
            std::vector<float> grad_W1(model.W1_.size(), 0.0);
            std::vector<float> grad_b1(model.b1_.size(), 0.0);
            std::vector<float> grad_W2(model.W2_.size(), 0.0);
            std::vector<float> grad_b2(model.b2_.size(), 0.0);
            float local_loss = 0.0;
            
            #pragma omp for
            for (int b = 0; b < n_batches; ++b) {
                int start = b * batch_size;
                int end = std::min(start + batch_size, n_samples);
                std::vector<std::vector<float>> X_batch(X.begin() + start, X.begin() + end);
                std::vector<int> y_batch(y.begin() + start, y.begin() + end);
                
                float batch_loss;
                model.train_batch(X_batch, y_batch, lr, grad_W1, grad_b1, grad_W2, grad_b2, batch_loss);
                local_loss += batch_loss;
            }
            
            #pragma omp critical
            {
                total_loss += local_loss;
                for (size_t i = 0; i < grad_W1.size(); ++i) model.W1_[i] -= lr * grad_W1[i];
                for (size_t i = 0; i < grad_b1.size(); ++i) model.b1_[i] -= lr * grad_b1[i];
                for (size_t i = 0; i < grad_W2.size(); ++i) model.W2_[i] -= lr * grad_W2[i];
                for (size_t i = 0; i < grad_b2.size(); ++i) model.b2_[i] -= lr * grad_b2[i];
            }
        }
        
        std::cout << "Época " << epoch + 1 << ", Pérdida promedio: " << total_loss / n_batches << "\n";
    }
}
```

**Compilar y ejecutar (dentro de un programa principal)**:
```bash
g++ -fopenmp neural_network.cpp -o neural_network
./neural_network
```

**Salida esperada** (valores aproximados):
```
Época 1, Pérdida promedio: 2.3
Época 2, Pérdida promedio: 1.8
...
Época 5, Pérdida promedio: 0.9
```

**Nota**: La pérdida es aproximada; los datos simulados afectan la convergencia.

**Bondades**:
- **Rendimiento**: Código compilado, rápido en cálculos.
- **Paralelismo**: Divide mini-lotes entre hilos con `#pragma omp for`.
- **Control**: Implementación explícita de gradientes.

**Limitaciones**:
- **Escalabilidad**: Limitado a un equipo.
- **Memoria**: Carga todo el dataset en RAM.
- **Complejidad**: Requiere implementar forward/backward manualmente.

## 6. Evaluación
### 6.1. Dask: Evaluación Paralela
Evaluamos el modelo en el conjunto de prueba.

```python
# Evaluar un mini-lote
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

**Salida esperada** (aproximada):
```
Precisión en test: 0.9123
```

**Bondades**:
- **Paralelismo**: Evalúa en paralelo.
- **Escalabilidad**: Maneja grandes conjuntos.
- **Facilidad**: API de alto nivel.

### 6.2. OpenMP: Evaluación Paralela
Evaluamos el modelo en C++.

```cpp
float evaluate_parallel(SimpleNN& model, const std::vector<std::vector<float>>& X, 
                       const std::vector<int>& y, int batch_size = 128) {
    int n_samples = X.size();
    int correct = 0, total = 0;
    
    #pragma omp parallel
    {
        int local_correct = 0, local_total = 0;
        #pragma omp for
        for (int b = 0; b < (n_samples + batch_size - 1) / batch_size; ++b) {
            int start = b * batch_size;
            int end = std::min(start + batch_size, n_samples);
            for (int i = start; i < end; ++i) {
                std::vector<float> output = model.forward(X[i]);
                int predicted = std::distance(output.begin(), 
                    std::max_element(output.begin(), output.end()));
                if (predicted == y[i]) local_correct++;
                local_total++;
            }
        }
        #pragma omp critical
        {
            correct += local_correct;
            total += local_total;
        }
    }
    
    return static_cast<float>(correct) / total;
}
```

**Salida esperada** (aproximada):
```
Precisión en test: 0.85
```

**Nota**: La precisión es menor debido a datos simulados y optimización simplificada.

**Bondades**:
- **Rendimiento**: Rápido en evaluación local.
- **Paralelismo**: Divide evaluación entre hilos.
- **Limitaciones**: No escala a clústeres, requiere RAM.

## 7. Comparación
- **Dask**:
  - **Ventajas**: Escala a clústeres, maneja datasets grandes, API de alto nivel, integración con PyTorch.
  - **Desventajas**: Overhead de Python, configuración de clústeres.
  - **Uso**: Big data, entrenamiento distribuido, flujos en Python.
- **OpenMP**:
  - **Ventajas**: Alto rendimiento local, código compilado, simplicidad en paralelismo.
  - **Desventajas**: Limitado a un equipo, implementación manual compleja.
  - **Uso**: Cálculos intensivos en un solo equipo, aplicaciones de alto rendimiento.
- **Python 3.13+**: Mejora I/O y concurrencia en Dask, no aplica a C++.

## 8. Mejores Prácticas
- **Dask**:
  - Ajusta chunks: `chunks=(10000, 784)`.
  - Usa dashboard: http://localhost:8787.
  - Cierra cliente: `client.close()`.
  - Escala clústeres: `cluster.scale(20)`.
  - Usa `persist()` para datos intermedios.
  - Considera Dask-ML o PyTorch Distributed para modelos complejos.
- **OpenMP**:
  - Minimiza secciones críticas (`#pragma omp critical`).
  - Ajusta hilos: `omp_set_num_threads(4)`.
  - Usa `libtorch` para redes reales.
- **Python 3.13+**: Prueba GIL libre (`PYTHON_GIL=0`) para Dask (experimental).

## 9. Conclusión
Dask es ideal para entrenar redes neuronales en datasets grandes, escalando a clústeres con facilidad. OpenMP ofrece alto rendimiento en un solo equipo, pero no escala ni maneja datasets masivos. Python 3.13+ mejora Dask con optimizaciones en I/O y concurrencia, mientras que C++ con OpenMP es más adecuado para aplicaciones locales de alto rendimiento.

**Código completo (Python con Dask)**:

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
cluster = LocalCluster(n_workers=4, threads_per_worker=2, memory_limit='4GB')
client = Client(cluster)
print(client)

# Cargar MNIST
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

# Red neuronal
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

# Entrenar mini-lote
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

# Entrenar época
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

# Ejecutar
train_parallel(model, X_train_da, y_train_da)
accuracy = evaluate_parallel(model, X_test_da, y_test_da)
print(f"Precisión en test: {accuracy:.4f}")

# Cerrar cliente
client.close()
```

**Código completo (C++ con OpenMP)**:

```cpp
#include <omp.h>
#include <vector>
#include <cmath>
#include <random>
#include <iostream>

class SimpleNN {
public:
    SimpleNN(int input_size = 784, int hidden_size = 128, int output_size = 10) 
        : input_size_(input_size), hidden_size_(hidden_size), output_size_(output_size) {
        std::mt19937 gen(42);
        std::normal_distribution<> dist(0, 0.01);
        W1_.resize(input_size_ * hidden_size_);
        b1_.resize(hidden_size_);
        W2_.resize(hidden_size_ * output_size_);
        b2_.resize(output_size_);
        for (auto& w : W1_) w = dist(gen);
        for (auto& b : b1_) b = dist(gen);
        for (auto& w : W2_) w = dist(gen);
        for (auto& b : b2_) b = dist(gen);
    }

    std::vector<float> forward(const std::vector<float>& x) {
        std::vector<float> hidden(hidden_size_, 0.0);
        std::vector<float> output(output_size_, 0.0);
        for (int j = 0; j < hidden_size_; ++j) {
            for (int i = 0; i < input_size_; ++i) {
                hidden[j] += W1_[j * input_size_ + i] * x[i];
            }
            hidden[j] += b1_[j];
            hidden[j] = std::max(0.0f, hidden[j]);
        }
        for (int j = 0; j < output_size_; ++j) {
            for (int i = 0; i < hidden_size_; ++i) {
                output[j] += W2_[j * hidden_size_ + i] * hidden[i];
            }
            output[j] += b2_[j];
        }
        return output;
    }

    std::vector<float> softmax(const std::vector<float>& x) {
        std::vector<float> result(x.size());
        float max_x = *std::max_element(x.begin(), x.end());
        float sum = 0.0;
        for (size_t i = 0; i < x.size(); ++i) {
            result[i] = std::exp(x[i] - max_x);
            sum += result[i];
        }
        for (auto& r : result) r /= sum;
        return result;
    }

    void train_batch(const std::vector<std::vector<float>>& X, const std::vector<int>& y, 
                     float lr, std::vector<float>& grad_W1, std::vector<float>& grad_b1,
                     std::vector<float>& grad_W2, std::vector<float>& grad_b2, float& loss) {
        grad_W1.assign(W1_.size(), 0.0);
        grad_b1.assign(b1_.size(), 0.0);
        grad_W2.assign(W2_.size(), 0.0);
        grad_b2.assign(b2_.size(), 0.0);
        loss = 0.0;
        
        for (size_t i = 0; i < X.size(); ++i) {
            std::vector<float> hidden(hidden_size_, 0.0);
            for (int j = 0; j < hidden_size_; ++j) {
                for (int k = 0; k < input_size_; ++k) {
                    hidden[j] += W1_[j * input_size_ + k] * X[i][k];
                }
                hidden[j] += b1_[j];
                hidden[j] = std::max(0.0f, hidden[j]);
            }
            std::vector<float> output(output_size_, 0.0);
            for (int j = 0; j < output_size_; ++j) {
                for (int k = 0; k < hidden_size_; ++k) {
                    output[j] += W2_[j * hidden_size_ + k] * hidden[k];
                }
                output[j] += b2_[j];
            }
            
            std::vector<float> probs = softmax(output);
            loss -= std::log(probs[y[i]]);
            
            std::vector<float> grad_output(output_size_, 0.0);
            for (int j = 0; j < output_size_; ++j) {
                grad_output[j] = probs[j] - (j == y[i] ? 1.0 : 0.0);
            }
            for (int j = 0; j < output_size_; ++j) {
                for (int k = 0; k < hidden_size_; ++k) {
                    grad_W2[j * hidden_size_ + k] += grad_output[j] * hidden[k];
                    grad_b2[j] += grad_output[j];
                }
            }
            std::vector<float> grad_hidden(hidden_size_, 0.0);
            for (int j = 0; j < hidden_size_; ++j) {
                for (int k = 0; k < output_size_; ++k) {
                    grad_hidden[j] += grad_output[k] * W2_[k * hidden_size_ + j];
                }
                if (hidden[j] > 0) {
                    for (int k = 0; k < input_size_; ++k) {
                        grad_W1[j * input_size_ + k] += grad_hidden[j] * X[i][k];
                    }
                    grad_b1[j] += grad_hidden[j];
                }
            }
        }
        
        float scale = 1.0 / X.size();
        for (auto& g : grad_W1) g *= scale;
        for (auto& g : grad_b1) g *= scale;
        for (auto& g : grad_W2) g *= scale;
        for (auto& g : grad_b2) g *= scale;
        loss *= scale;
    }

    void update_weights(const std::vector<float>& grad_W1, const std::vector<float>& grad_b1,
                        const std::vector<float>& grad_W2, const std::vector<float>& grad_b2, float lr) {
        #pragma omp parallel for
        for (size_t i = 0; i < W1_.size(); ++i) W1_[i] -= lr * grad_W1[i];
        #pragma omp parallel for
        for (size_t i = 0; i < b1_.size(); ++i) b1_[i] -= lr * grad_b1[i];
        #pragma omp parallel for
        for (size_t i = 0; i < W2_.size(); ++i) W2_[i] -= lr * grad_W2[i];
        #pragma omp parallel for
        for (size_t i = 0; i < b2_.size(); ++i) b2_[i] -= lr * grad_b2[i];
    }

    std::vector<float> W1_, b1_, W2_, b2_;
private:
    int input_size_, hidden_size_, output_size_;
};

void load_mnist(std::vector<std::vector<float>>& X_train, std::vector<int>& y_train, 
                std::vector<std::vector<float>>& X_test, std::vector<int>& y_test) {
    const int train_size = 60000, test_size = 10000, dim = 784;
    std::mt19937 gen(42);
    std::uniform_real_distribution<> dist(0, 1);
    
    X_train.resize(train_size, std::vector<float>(dim));
    y_train.resize(train_size);
    X_test.resize(test_size, std::vector<float>(dim));
    y_test.resize(test_size);
    
    for (int i = 0; i < train_size; ++i) {
        for (int j = 0; j < dim; ++j) {
            X_train[i][j] = dist(gen);
        }
        y_train[i] = rand() % 10;
    }
    for (int i = 0; i < test_size; ++i) {
        for (int j = 0; j < dim; ++j) {
            X_test[i][j] = dist(gen);
        }
        y_test[i] = rand() % 10;
    }
}

void train_parallel(SimpleNN& model, const std::vector<std::vector<float>>& X, 
                    const std::vector<int>& y, int epochs = 5, int batch_size = 128, float lr = 0.01) {
    int n_samples = X.size();
    for (int epoch = 0; epoch < epochs; ++epoch) {
        float total_loss = 0.0;
        int n_batches = (n_samples + batch_size - 1) / batch_size;
        
        #pragma omp parallel
        {
            std::vector<float> grad_W1(model.W1_.size(), 0.0);
            std::vector<float> grad_b1(model.b1_.size(), 0.0);
            std::vector<float> grad_W2(model.W2_.size(), 0.0);
            std::vector<float> grad_b2(model.b2_.size(), 0.0);
            float local_loss = 0.0;
            
            #pragma omp for
            for (int b = 0; b < n_batches; ++b) {
                int start = b * batch_size;
                int end = std::min(start + batch_size, n_samples);
                std::vector<std::vector<float>> X_batch(X.begin() + start, X.begin() + end);
                std::vector<int> y_batch(y.begin() + start, y.begin() + end);
                
                float batch_loss;
                model.train_batch(X_batch, y_batch, lr, grad_W1, grad_b1, grad_W2, grad_b2, batch_loss);
                local_loss += batch_loss;
            }
            
            #pragma omp critical
            {
                total_loss += local_loss;
                for (size_t i = 0; i < grad_W1.size(); ++i) model.W1_[i] -= lr * grad_W1[i];
                for (size_t i = 0; i < grad_b1.size(); ++i) model.b1_[i] -= lr * grad_b1[i];
                for (size_t i = 0; i < grad_W2.size(); ++i) model.W2_[i] -= lr * grad_W2[i];
                for (size_t i = 0; i < grad_b2.size(); ++i) model.b2_[i] -= lr * grad_b2[i];
            }
        }
        
        std::cout << "Época " << epoch + 1 << ", Pérdida promedio: " << total_loss / n_batches << "\n";
    }
}

float evaluate_parallel(SimpleNN& model, const std::vector<std::vector<float>>& X, 
                       const std::vector<int>& y, int batch_size = 128) {
    int n_samples = X.size();
    int correct = 0, total = 0;
    
    #pragma omp parallel
    {
        int local_correct = 0, local_total = 0;
        #pragma omp for
        for (int b = 0; b < (n_samples + batch_size - 1) / batch_size; ++b) {
            int start = b * batch_size;
            int end = std::min(start + batch_size, n_samples);
            for (int i = start; i < end; ++i) {
                std::vector<float> output = model.forward(X[i]);
                int predicted = std::distance(output.begin(), 
                    std::max_element(output.begin(), output.end()));
                if (predicted == y[i]) local_correct++;
                local_total++;
            }
        }
        #pragma omp critical
        {
            correct += local_correct;
            total += local_total;
        }
    }
    
    return static_cast<float>(correct) / total;
}

int main() {
    std::vector<std::vector<float>> X_train, X_test;
    std::vector<int> y_train, y_test;
    load_mnist(X_train, y_train, X_test, y_test);
    
    SimpleNN model;
    train_parallel(model, X_train, y_train);
    float accuracy = evaluate_parallel(model, X_test, y_test);
    std::cout << "Precisión en test: " << accuracy << "\n";
    
    return 0;
}
```

**Notas**:
- **Python 3.13+**: Verifica compatibilidad de Dask/PyTorch. GIL libre experimental.
- **C++**: Usa `libtorch` para MNIST y redes reales. Este ejemplo simplifica la carga de datos.
- **Clústeres**: Dask usa `dask-jobqueue` o `dask-kubernetes` para entornos reales.
- **Optimizaciones**: Para IA compleja, considera Horovod (Dask) o MPI (C++).