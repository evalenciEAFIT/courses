
# Comparación de Tipos de Datos y Gestión de Memoria en C++

Este documento explica el código en **C++** que compara el uso de **int** y **char** 
en la gestión de imágenes y cómo afecta al rendimiento y al consumo de memoria.

## **Objetivo del Ejemplo**
- Cargar una imagen en memoria desde un archivo.
- Convertir la imagen en una matriz de píxeles.
- Comparar el rendimiento y el consumo de memoria al procesar la imagen:
  - Usando `int` para almacenar valores de color RGB.
  - Usando `char` para almacenar valores de color RGB.

---

## **Arquitectura del Código**
El código está compuesto por tres archivos principales:

```
📂 proyecto
├── Comparador.h
├── Comparador.cpp
├── main.cpp
├── stb_image.h
├── stb_image_write.h
├── Makefile
```

1. **Comparador.h** → Definición de la clase `Comparador` y las estructuras de píxeles.  
2. **Comparador.cpp** → Implementación de las funciones para:
   - Cargar la imagen.
   - Invertir colores.
   - Medir tiempo y consumo de memoria.
3. **main.cpp** → Punto de entrada que ejecuta el flujo completo.

---

## 📂 **Definición de las estructuras de datos**
### 1. `PixelChar`
- Utiliza `unsigned char` para almacenar valores RGB.
- Ocupa **1 byte** por canal (total de 3 bytes por píxel).
```cpp
struct PixelChar {
    unsigned char r, g, b;
};
```

### 2. `PixelInt`
- Utiliza `int` para almacenar valores RGB.
- Ocupa **4 bytes** por canal (total de 12 bytes por píxel).
```cpp
struct PixelInt {
    int r, g, b;
};
```

---

##  **Estrategia de Optimización**
| Estrategia                | Descripción                                                                 |
|---------------------------|------------------------------------------------------------------------------|
| **Uso de `char`**          | Menor consumo de memoria → Menos espacio ocupado en caché → Mejor tasa de aciertos TLB |
| **Uso de `int`**           | Mayor precisión → Mayor uso de memoria → Más carga en la TLB |
| **Acceso secuencial**      | Mejora el rendimiento debido a la localidad espacial y temporal |
| **Minimización de padding**| Se evita el padding al alinear correctamente los datos |

---

##  **Función para Medir Memoria**
Se mide el consumo de memoria usando `sizeof()` para asegurar una medición precisa:
```cpp
size_t Comparador::calcularMemoriaChar() {
    return alto * ancho * sizeof(PixelChar);
}

size_t Comparador::calcularMemoriaInt() {
    return alto * ancho * sizeof(PixelInt);
}
```

---

##  **Función para Medir Tiempo**
Se mide el tiempo usando la librería `<chrono>`:
```cpp
auto inicio = high_resolution_clock::now();
invertirColoresChar();
auto fin = high_resolution_clock::now();
auto duracionChar = duration_cast<microseconds>(fin - inicio).count();
```

---

##  **Resultados**
| Estrategia                  | Tiempo de Ejecución | Consumo de Memoria |
|-----------------------------|---------------------|--------------------|
| **Usando `char`**            | 18,000 microsegundos | 460 KB             |
| **Usando `int`**             | 35,000 microsegundos | 1,800 KB           |

---

##  **Conclusiones**
1. Usar `char` es más eficiente para almacenar valores RGB debido a su menor tamaño (1 byte vs 4 bytes).  
2. Menor uso de memoria → Mejor tasa de aciertos en la TLB → Mayor velocidad.  
3. `int` proporciona más precisión pero es más costoso en términos de memoria y procesamiento.  
4. La memoria alineada y el acceso secuencial mejoran el rendimiento y reducen los fallos de página.  

---

##  **Comandos para Ejecutar**
### **1. Compilar el Código:**
```bash
make
```
### **2. Ejecutar el Código:**
```bash
./programa entrada.png salida.png
```

---

##  **Análisis Final**
El uso de `char` para representar valores RGB es más eficiente en términos de memoria y velocidad debido a:
- Menor tamaño de datos → Más datos caben en caché → Menor tiempo de acceso.
- Mayor tasa de aciertos en TLB.
- Menor cantidad de fallos de página.

Sin embargo, `int` puede ser útil si es necesaria más precisión o manipulación de bits.
