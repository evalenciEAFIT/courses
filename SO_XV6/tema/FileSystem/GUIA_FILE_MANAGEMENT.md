# 📁 Guía Maestra de File Management en C (Linux)

Bienvenidos al curso práctico de **Sistemas de Archivos y Persistencia**. Este proyecto contiene 45 programas diseñados para llevarte desde la creación de un archivo simple hasta la construcción de un **Editor Hexadecimal** y un **Editor de Texto**.

---

## 🛠️ Herramientas de Compilación y Ejecución
Para facilitar el aprendizaje, usa el `Makefile` incluido:

```bash
# Ver lista de programas
make list

# Ejecutar un programa específico (Ejemplo: el número 21)
make run-21_file_stat

# Limpiar el directorio de binarios y archivos de prueba
make clean
```

---

## 🗺️ Mapa de Ruta de Aprendizaje (45 Programas)

### Fase 1: Stream I/O Básico (Funciones de Librería)
*Concepto: Archivos como flujos de datos (streams) gestionados por la Librería Estándar de C.*
- **01-05**: Creación, lectura/escritura de caracteres y líneas (`fopen`, `fgetc`, `fgets`).
- **06-10**: Formateo de datos (`fprintf`, `fscanf`) y manejo de errores con `errno`.

### Fase 2: System Calls de Linux (Bajo Nivel)
*Concepto: Hablar directamente con el Kernel usando descriptores de archivo (int fd).*
- **11-13**: Uso de `open()`, `read()`, `write()`, `close()`.
- **14-15**: Manipulación de descriptores (`dup()`, `dup2()`) y redirección de salida.

### Fase 3: Navegación y Punteros
*Concepto: El acceso aleatorio (Random Access) y el movimiento del cabezal de lectura.*
- **16-19**: Movimientos con `fseek()`, `ftell()`, `lseek()`, `rewind()`.
- **20**: **Acceso Aleatorio**: Lectura directa del registro N sin leer los anteriores.

### Fase 4: Metadatos e Inodos
*Concepto: Analizar "la etiqueta" del archivo (Inodo) sin leer su contenido.*
- **21-25**: Uso de `stat()`. Tamaño físico vs lógico, tipos de archivos, permisos y fechas.
- **26-27**: Cambio de identidad (`chmod`, `chown`).
- **28-30**: **Enlaces (Links)**: Diferencia entre Enlaces Físicos (Hard) y Simbólicos (Soft).

### Fase 5: Estructura de Directorios (FileSystem)
*Concepto: Las carpetas son archivos especiales que listan otros archivos.*
- **31-33**: Creación y borrado de directorios (`mkdir`, `rmdir`).
- **34-35**: **Implementación de `ls`**: Navegar por flujos de directorio (`opendir`, `readdir`).

### Especial: Benchmark de Rendimiento
- **32**: Comparativa profunda entre **Syscalls vs Librerías**. Se explica el costo del *Context Switch* (Cambio de Modo Usuario a Modo Kernel).

### Fase 6: Texto Plano vs Binario (Persistencia)
*Concepto: Cómo los programas profesionales guardan datos eficientemente.*
- **36**: Comparativa de espacio: Guardar un número como texto (10 bytes) vs binario (4 bytes).
- **37-39**: **Serialización**: Guardar y cargar Estructuras (`struct`) completas de memoria a disco.
- **40**: **Endianness**: Cómo el procesador ordena los bytes en el disco.

### Fase 7: Proyectos Finales (Herramientas de Sistema)
- **41**: Visor de texto con numeración de líneas (Clon de `cat -n`).
- **42-43**: **Visor Hexadecimal Profesional**: Offset, Bytes Hex y representación ASCII.
- **44**: **Parcheador de Bytes**: Modificar un byte específico en cualquier dirección del archivo.
- **45**: **Editor de Texto**: Herramienta TUI para crear y escribir archivos linealmente.

---

## 🎓 Tips Académicos de Sistemas Operativos
1. **Todo es un archivo**: En Linux, tu teclado, tu monitor, tu disco y tus procesos son representados como archivos en la carpeta `/dev`.
2. **Buffering**: Las librerías de C son más rápidas que las syscalls directas porque acumulan datos en el "Espacio de Usuario" antes de pedírselo al disco.
3. **Inodos**: El nombre del archivo es solo una etiqueta. El "alma" del archivo es su número de Inodo, que guarda los permisos y la ubicación física en el disco.

---
*Documentación generada para Edi - PROYECTOS/SO/FileSystem 2026*
