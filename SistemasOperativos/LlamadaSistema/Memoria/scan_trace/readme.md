# 📌 Herramientas de Diagnóstico de Memoria en C++

Este documento explica el propósito y la utilidad de **Memory Trace** y **Memory Scan**, dos herramientas en C++ diseñadas para un curso de **Sistemas Operativos**. Estas herramientas permiten estudiar el comportamiento de la memoria en procesos, asignaciones dinámicas y la organización de la memoria en un sistema operativo basado en Linux.

---

## 🔍 Memory Trace: Diagnóstico de Asignaciones y Uso de Memoria

### **📖 Descripción**
El programa **Memory Trace** permite rastrear la asignación y liberación de memoria dinámica en un proceso de C++. Mide el consumo de memoria antes, durante y después de la asignación, mostrando información útil sobre su impacto en el sistema.

### **🛠️ Características**
- Mide el uso de memoria antes y después de la asignación de un bloque de datos.
- Muestra el **rango de direcciones de memoria asignadas**.
- Utiliza `std::unique_ptr` para una gestión segura de memoria.
- Permite liberar la memoria y medir su impacto en el sistema.
- Mide el tiempo de ejecución de la operación de asignación y liberación de memoria.
- Indica si la memoria asignada pertenece al **heap**, **stack** u otro segmento de memoria.

### **📌 Ubicación de la Memoria en el Programa**
- **Heap:** La memoria asignada dinámicamente con `new`, `malloc` o `std::unique_ptr` reside en el **heap**.
- **Stack:** Variables locales y direcciones de retorno de funciones se almacenan en el **stack**.

En este caso, el bloque de memoria asignado con `std::unique_ptr<int[]>` se encuentra en el **heap**, lo que significa que el espacio de memoria reservado existirá hasta que el puntero deje de estar en uso y libere los recursos asignados.

### **🚀 Ejecución**
```bash
make memory_trace
./memory_trace
```

Al ejecutar el programa, se mostrará la dirección de memoria asignada:
```
Memoria asignada en el rango: inicio:[0x561a8f2c0000] - - - [0x561a8f2cffff]
Memoria asignada y utilizada. Presione ENTER para liberar...
```
Después de presionar **ENTER**, se liberará la memoria y se mostrará el tiempo de ejecución del proceso.

---

## 🔎 Memory Scan: Exploración de Regiones de Memoria de un Proceso

### **📖 Descripción**
El programa **Memory Scan** analiza las regiones de memoria de un proceso en ejecución usando la información disponible en **`/proc/[PID]/maps`**. Este archivo proporciona un **mapa de memoria** del proceso, mostrando sus segmentos de código, datos, pila y bibliotecas cargadas.

### **🛠️ Características**
- Lee el archivo `/proc/[PID]/maps` para visualizar las regiones de memoria asignadas.
- Muestra direcciones de memoria utilizadas por código, datos y bibliotecas compartidas.
- Identifica qué secciones del proceso están en memoria y su tipo de acceso.
- Ayuda a comprender cómo Linux organiza la memoria de los procesos.

### **📌 Aplicación en Sistemas Operativos**
- Estudiar la **distribución de memoria** de un proceso en ejecución.
- Comprender la separación entre segmentos como **heap, stack y código**.
- Diagnosticar problemas de memoria, como **fugas o accesos indebidos**.

### **🚀 Ejecución**
```bash
make memory_scan
./memory_scan
```

El programa mostrará la memoria del proceso actual:
```
Proceso actual: 12345
Escaneo de memoria del proceso 12345
-----------------------------------------------------
00400000-00452000 r-xp 00000000 fd:00 123456 /usr/bin/programa
7f8a23400000-7f8a23600000 rw-p 00000000 00:00 0
...
```

### **📖 Explicación de la Salida de `memory_scan`**
Cada línea en la salida del programa representa un **segmento de memoria** asignado al proceso. La información se presenta en columnas con el siguiente formato:
```
<dirección_inicio>-<dirección_fin> <permisos> <offset> <device> <inode> <path>
```

#### **📌 Descripción de cada campo:**
- **`00400000-00452000`** → Rango de direcciones de memoria asignadas.
- **`r-xp`** → Permisos:
  - `r` → Lectura
  - `w` → Escritura
  - `x` → Ejecución
  - `p` → Privado (no compartido con otros procesos)
- **`00000000`** → Desplazamiento dentro del archivo mapeado.
- **`fd:00`** → Código del dispositivo (en este caso, archivo en disco).
- **`123456`** → Número de inodo del archivo asociado (si aplica).
- **`/usr/bin/programa`** → Archivo o segmento mapeado en la memoria del proceso (puede ser una biblioteca compartida o el binario ejecutado).

#### **📌 Interpretación de la Salida**
- **Segmentos de código:** Contienen el código ejecutable del programa y sus dependencias.
- **Heap (`rw-p`)**: Zona donde se asigna memoria dinámica (reservada con `new` o `malloc`).
- **Stack:** Espacio de memoria utilizado para manejar funciones y variables locales.
- **Bibliotecas compartidas:** Archivos `.so` cargados en la memoria del proceso.

Este análisis ayuda a comprender la estructura de memoria de un proceso en Linux y cómo el sistema operativo administra los recursos de memoria.

---

## 🎯 Conclusión
Estas herramientas son esenciales en un curso de **Sistemas Operativos**, ya que permiten a los estudiantes:
- Explorar **cómo los programas usan y liberan memoria**.
- Analizar el impacto de **diferentes estructuras de datos en el rendimiento del sistema**.
- Comprender la organización de memoria en Linux y los problemas comunes de gestión de memoria.

¡Prueba estos programas y experimenta con la memoria en **C++ y Linux**! 🚀

