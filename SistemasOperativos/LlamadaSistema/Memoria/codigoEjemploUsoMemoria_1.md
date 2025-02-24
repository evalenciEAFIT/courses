# Guía para Compilar y Ejecutar el Programa en Linux

Este documento explica el código proporcionado, el procedimiento para compilarlo y ejecutarlo en Linux, así como el uso de la memoria en el programa.

---

## 1. Descripción del Código

El programa en C++ analiza y muestra la ubicación en memoria de diferentes tipos de variables: globales, locales, en el heap y estáticas. Además, verifica si una dirección de memoria pertenece al heap o a la pila (stack).

### **Componentes Principales:**
- **Uso de `sbrk` y `/proc/self/maps`** para determinar el inicio y fin del heap.
- **Códigos de escape ANSI** para mejorar la salida en la terminal con colores.
- **Función `isAddressInHeap`** para determinar si una dirección de memoria está en el heap.
- **Función `isAddressInStack`** para verificar si una dirección pertenece a la pila.
- **Uso de variables globales, locales, dinámicas (heap) y estáticas** para demostrar sus ubicaciones en memoria.
- **Uso de `std::setw` y `std::boolalpha`** para mejorar el formato de salida.

---

## 2. Compilación en Linux

Para compilar el programa en Linux, asegúrese de tener `g++` instalado. Ejecute el siguiente comando en la terminal:

```sh
 g++ -o memoria memoria.cpp -std=c++11
```

**Explicación del comando:**
- `g++`: Compilador de C++.
- `-o memoria`: Genera un ejecutable llamado `memoria`.
- `memoria.cpp`: Archivo fuente del programa.
- `-std=c++11`: Especifica el estándar de C++11 para asegurar compatibilidad.

---

## 3. Ejecución del Programa

Una vez compilado, ejecute el programa con:

```sh
 ./memoria
```

Para mostrar la ayuda, ejecute:

```sh
 ./memoria -h
```

Para modificar el valor de la variable global:

```sh
 ./memoria 100
```

**Ejemplo de Salida:**
```
DIRECCION MEMORIA [0x555555756004]   STATIC :         33 variable global
DIRECCION MEMORIA [0x7fffffffd53c] STACK  :          0 variable local
DIRECCION MEMORIA [0x5555557772a0]   HEAP  :          0 variable en el heap
DIRECCION MEMORIA [0x555555756008]   STATIC :          0 variable statica

-----------------------------------------------------------------------------

VERIFICACIONES

globalVariable está en el heap: false
localVariable está en el heap: false
heapVariable está en el heap: true
staticLocalVariable está en el heap: false

globalVariable está en la pila: false
localVariable está en la pila: true
heapVariable está en la pila: false
staticLocalVariable está en la pila: false
```

---

## 4. Explicación del Uso de Memoria

El programa analiza diferentes secciones de la memoria:

### **1. Segmento de Datos Estáticos**
- **Variables globales y estáticas** se almacenan en esta sección de la memoria.

### **2. Pila (Stack)**
- **Variables locales** se almacenan en la pila y se eliminan automáticamente cuando la función termina.

### **3. Heap**
- **Memoria dinámica** (asignada con `new`) se almacena en el heap y debe liberarse con `delete`.

El programa verifica si las variables pertenecen al heap o a la pila y muestra la información en pantalla.

---

## 5. Consideraciones Finales

- Se recomienda liberar la memoria asignada en el heap con `delete` para evitar fugas de memoria.
- El programa hace uso de `/proc/self/maps` para obtener información precisa sobre la memoria asignada.
- El uso de colores mejora la visualización en terminales compatibles con ANSI.

---

**Autor:**
Este documento fue generado para documentar el programa de administración y verificación de memoria en Linux.

