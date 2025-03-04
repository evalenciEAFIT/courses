#  Reto: Optimización de Gestión de Memoria en Procesamiento de Imágenes

##  Contexto
En sistemas operativos, el **manejo eficiente de la memoria** es crucial cuando se trabaja con grandes volúmenes de datos, como imágenes. El código base actual permite cargar imágenes y extraer sus canales de color, pero aún presenta oportunidades de **optimización en la asignación y liberación de memoria**.

##  Objetivo
Optimizar el código base para mejorar la eficiencia en la gestión de memoria al procesar imágenes. Esto implica reducir el consumo de RAM, evitar accesos innecesarios a memoria y mejorar la velocidad de manipulación de matrices de píxeles.

---

##  Actividades del Reto
### 1️ Medir el consumo de memoria actual
- Usar herramientas como `time`, `valgrind`, `htop` o `/proc/meminfo` para analizar el consumo de memoria al ejecutar el programa.
- Registrar los resultados antes de realizar cualquier optimización.

### 2️ Optimizar el almacenamiento de los canales de píxeles
- El código actual almacena cada canal como una **matriz de enteros** (`std::vector<std::vector<int>>`).
- **Reto:** Convertir la estructura a un **buffer lineal** (`std::vector<uint8_t>`) para mejorar la gestión de memoria.

### 3️ Liberación temprana de memoria
- Actualmente, las matrices de los canales RGB **siguen en memoria incluso después de ser utilizadas**.
- **Reto:** Implementar un mecanismo para liberar la memoria después de guardar los archivos CSV y visualizar los datos.

### 4️ Uso de asignación dinámica y punteros inteligentes
- **Reto:** Implementar `std::unique_ptr` o `std::shared_ptr` para manejar la memoria de los canales de píxeles de manera eficiente.

### 5️ Comparación del rendimiento antes y después de la optimización
- **Medir nuevamente el consumo de memoria** y el tiempo de ejecución para validar las mejoras.
- Presentar una tabla con la comparación de tiempos y uso de memoria antes y después de la optimización.

---

##  Evaluación del Reto
- **Funcionalidad**: ¿El programa sigue funcionando correctamente después de las optimizaciones?
- **Uso de memoria**: ¿Se redujo el consumo de RAM al procesar imágenes grandes?
- **Eficiencia en el acceso**: ¿El procesamiento es más rápido con las nuevas estructuras de datos?
- **Claridad del código**: ¿La implementación optimizada sigue siendo comprensible y bien documentada?

---

## 💡 Ejemplo de Cambio en la Estructura de Memoria

**Antes (uso de `std::vector<std::vector<int>>`):**
```cpp
std::vector<std::vector<int>> canalRojo, canalVerde, canalAzul;
```

**Después (uso de `std::vector<uint8_t>` como buffer lineal):**
```cpp
std::vector<uint8_t> canalRojo, canalVerde, canalAzul;
```
- Ahora, en lugar de hacer accesos indirectos con `vector<vector<int>>`, los datos están almacenados en **una sola estructura contigua en memoria**, lo que mejora la eficiencia del acceso.

---

