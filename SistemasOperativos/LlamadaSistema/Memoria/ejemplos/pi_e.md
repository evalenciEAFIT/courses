# **Análisis de Precisión en C++: Cálculo de π y e con Diferentes Tipos de Datos**

## **Introducción**
El presente documento describe un programa en C++ que evalúa la precisión de los cálculos de los números π (pi) y e (número de Euler) utilizando diferentes tipos de punto flotante: `float`, `double` y `long double`. Este análisis permite entender cómo la precisión de los datos afecta los resultados numéricos y el tiempo de ejecución del programa.

## **Descripción del Código**

### **1. Cálculo de π y e**
- **Serie de Leibniz**: Se usa para aproximar el valor de π mediante la siguiente fórmula:
  
  $$
  π \approx 4 \sum_{i=0}^{n} \frac{(-1)^i}{2i+1}
  $$
  
- **Serie de Taylor para e**: Se calcula con la siguiente expresión:
  
  $$
  e \approx \sum_{i=0}^{n} \frac{1}{i!}
  $$

Ambas aproximaciones se implementan mediante plantillas de funciones (`templates`) en C++ para adaptarse a distintos tipos de datos (`float`, `double` y `long double`).

### **2. Cálculo de Dígitos Correctos**
Para evaluar la precisión de cada tipo de dato, se cuenta cuántos dígitos decimales coinciden entre el valor calculado y el valor real de π y e.

1. Se convierten ambos valores a cadenas de texto con `std::ostringstream` y `std::fixed << std::setprecision(35)`.
2. Se comparan dígito por dígito hasta encontrar una discrepancia.
3. Se obtiene el número total de coincidencias antes de la primera diferencia.

### **3. Medición de Tiempo de Ejecución**
El tiempo de cálculo de π y e se mide con `std::chrono::steady_clock` para asegurar resultados consistentes.

### **4. Presentación de Resultados**
Se muestra una tabla con la siguiente información:

| Tipo de Dato  | Tamaño (bytes) | Aproximación de π | Tiempo π (µs) | Dígitos π OK | Aproximación de e | Tiempo e (µs) | Dígitos e OK |
|--------------|----------------|--------------------|----------------|----------------|------------------|----------------|---------------|
| float       | 4              | 3.14159250        | 10,235         | 6              | 2.71828198       | 10,456         | 6             |
| double      | 8              | 3.141592653589793  | 20,341         | 15             | 2.718281828459046| 20,037         | 15            |
| long double | 16             | 3.141592653589793238 | 40,673       | 18             | 2.718281828459045| 40,234         | 35            |

## **Fortalezas del Concepto**
1. **Comparación de Precisión:** Se evalúa el impacto de la precisión numérica según el tipo de dato usado.
2. **Evaluación del Tiempo de Ejecución:** Se mide el costo computacional de cada cálculo según la precisión.
3. **Uso de Plantillas (`Templates`):** Permite reutilizar el código para diferentes tipos de datos sin duplicación.
4. **Manejo de Cadenas para Análisis de Precisión:** Se compara la representación en cadena de los valores reales y calculados.

## **Retos a Desarrollar**
1. **Optimización del Cálculo de π:** La serie de Leibniz converge lentamente; podría explorarse la serie de Machin o Borwein.
2. **Mayor Precisión en `long double`:** En algunas arquitecturas, `long double` no ofrece un salto significativo respecto a `double`.
3. **Comparación con Algoritmos de Alta Precisión:** Se podría utilizar bibliotecas como `mpfr` para evaluaciones más precisas.
4. **Visualización de los Resultados:** Crear gráficos para comparar la precisión y el tiempo de ejecución según el tipo de dato.
5. **Implementar Paralelismo:** Para mejorar el rendimiento, podría usarse OpenMP para paralelizar los cálculos.

## **Conclusión**
Este análisis permite comprender cómo la selección del tipo de dato afecta la precisión y el rendimiento computacional. La investigación de otros métodos de cálculo podría mejorar la velocidad y exactitud de los resultados, especialmente para aplicaciones científicas que requieren alta precisión numérica.

