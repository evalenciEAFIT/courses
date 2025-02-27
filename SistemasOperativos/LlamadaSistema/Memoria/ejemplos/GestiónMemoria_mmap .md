# Gestión de Memoria con mmap en C++

## Descripción del Código
Este programa en C++ implementa una técnica eficiente para el manejo de archivos grandes utilizando `mmap`. En lugar de cargar el archivo completamente en memoria con `fread` o `ifstream`, `mmap` permite mapear el contenido del archivo directamente en el espacio de direcciones del proceso. Esto reduce el consumo de memoria y mejora el rendimiento en operaciones de lectura intensivas.

## Justificación y Usos Potenciales
### 1. **Procesamiento de Archivos Grandes**
   - `mmap` permite procesar archivos sin cargar su totalidad en memoria RAM.
   - Ideal para sistemas con recursos limitados o archivos que superan la capacidad de RAM.

### 2. **Optimización del Acceso a Datos**
   - Evita copias innecesarias en memoria y aprovecha el sistema de paginación del SO.
   - Puede ser usado en aplicaciones que requieren acceso rápido a grandes volúmenes de datos, como bases de datos y motores de búsqueda.

### 3. **Análisis de Texto y Logs**
   - Útil para conteo de caracteres, análisis de patrones o procesamiento de logs en tiempo real.
   - Puede aplicarse en seguridad informática para detectar patrones sospechosos en archivos de registro.

### 4. **Procesamiento de Imágenes y Datos Binarios**
   - `mmap` permite acceder a imágenes o archivos binarios sin cargarlos completamente en RAM.
   - Puede ser utilizado en procesamiento de imágenes o machine learning donde se necesiten acceder a grandes conjuntos de datos binarios.

## Explicación Técnica
### 1. **Carga del Archivo con mmap**
   - Se obtiene el tamaño del archivo con `stat()`.
   - Se abre el archivo con `open()`.
   - Se mapea a memoria con `mmap()`, permitiendo su acceso como un bloque de memoria continua.

### 2. **Procesamiento Secuencial del Archivo**
   - Se recorre el archivo en memoria contando las ocurrencias de un carácter específico.
   - Se imprime la cantidad de veces que aparece el carácter buscado.

### 3. **Liberación de Recursos**
   - Se libera la memoria mapeada con `munmap()`.
   - Se cierra el descriptor de archivo después de mapearlo.

## Ejemplo de Uso
### **Compilación**
```bash
 g++ -o gestion_memoria gestion_memoria_mmap.cpp
```

### **Ejecución**
```bash
./gestion_memoria archivo.txt
```

## Conclusión
El uso de `mmap` en la gestión de archivos grandes permite mejorar el rendimiento en aplicaciones donde la lectura y manipulación eficiente de archivos es crítica. Su integración con técnicas avanzadas como procesamiento en paralelo o memoria compartida lo convierte en una solución poderosa para sistemas de alto rendimiento.


## Código
```cpp
#include <iostream>
#include <fstream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

// Función para obtener el tamaño de un archivo
off_t obtener_tamano_archivo(const char *ruta) {
    struct stat sb;
    if (stat(ruta, &sb) == -1) {
        perror("Error obteniendo tamaño del archivo");
        exit(EXIT_FAILURE);
    }
    return sb.st_size;
}

// Función que procesa el archivo de manera secuencial
size_t contar_ocurrencias(char *data, size_t tamano, char caracter) {
    size_t contador = 0;
    for (size_t i = 0; i < tamano; ++i) {
        if (data[i] == caracter) {
            contador++;
        }
    }
    return contador;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo>" << std::endl;
        return EXIT_FAILURE;
    }

    const char *ruta_archivo = argv[1];
    off_t tamano_archivo = obtener_tamano_archivo(ruta_archivo);

    std::cout << "[INFO] Cargando archivo " << ruta_archivo << " (" << tamano_archivo << " bytes) usando mmap." << std::endl;

    int fd = open(ruta_archivo, O_RDONLY);
    if (fd == -1) {
        perror("Error abriendo el archivo");
        return EXIT_FAILURE;
    }

    // Mapear archivo en memoria
    char *data = (char *)mmap(NULL, tamano_archivo, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("Error al mapear archivo en memoria");
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd); // El descriptor ya no es necesario después del mapeo
    
    std::cout << "[INFO] Archivo mapeado en memoria correctamente." << std::endl;

    // Procesar archivo de manera secuencial
    size_t total_a = contar_ocurrencias(data, tamano_archivo, 'a');
    std::cout << "[INFO] Se encontraron " << total_a << " ocurrencias de 'a' en el archivo." << std::endl;

    // Liberar la memoria mapeada
    if (munmap(data, tamano_archivo) == -1) {
        perror("Error al liberar memoria mapeada");
    }

    std::cout << "[INFO] Memoria liberada y procesamiento finalizado." << std::endl;

    return EXIT_SUCCESS;
}

```
