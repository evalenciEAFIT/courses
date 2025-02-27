# Gestión de Memoria y Archivos con `system()` y `mmap` en C++

## Descripción del Código
El código implementa una combinación de llamadas al sistema y mapeo de memoria para gestionar archivos de manera eficiente en C++. Utiliza `system()` para ejecutar comandos en la terminal y `mmap()` para mapear archivos en memoria, evitando la necesidad de cargarlos completamente en RAM.

## Justificación y Aplicaciones
### 1. **Automatización de Operaciones sobre Archivos**
   - La ejecución de `system("ls -lh <archivo>")` permite obtener información rápida del archivo sin necesidad de programar una lectura personalizada.
   - Puede integrarse con herramientas externas para verificar el tamaño y permisos de archivos antes de procesarlos.

### 2. **Manejo Eficiente de Archivos Grandes**
   - `mmap()` permite mapear archivos directamente en la memoria virtual del proceso, optimizando el acceso y evitando la carga completa en RAM.
   - Ideal para procesar grandes volúmenes de datos, como registros de logs, archivos CSV masivos o bases de datos en texto plano.

### 3. **Extracción de Información de Forma Parcial**
   - Se pueden extraer los primeros `N` caracteres del archivo sin necesidad de recorrerlo en su totalidad, optimizando tiempos de acceso.
   - Útil en análisis de encabezados de archivos, identificación de formatos o previsualización rápida.

## Posibles Retos y Consideraciones
### 1. **Seguridad en `system()`**
   - `system()` ejecuta comandos directamente en la terminal, lo que puede ser peligroso si se recibe entrada no controlada del usuario.
   - Es recomendable sanitizar las entradas o evitar `system()` en entornos críticos, optando por funciones como `execvp()` en sistemas UNIX.

### 2. **Gestión de Memoria con `mmap()`**
   - Si se manipula un archivo grande, el uso ineficiente de `mmap()` podría generar problemas de paginación o swapping si la memoria virtual es insuficiente.
   - Siempre se debe liberar la memoria mapeada con `munmap()` para evitar pérdidas de recursos.

### 3. **Compatibilidad y Portabilidad**
   - `mmap()` es una función específica de sistemas POSIX, por lo que su uso en Windows requiere alternativas como `CreateFileMapping` y `MapViewOfFile`.
   - `system("ls -lh")` es dependiente de comandos UNIX y no funcionará en Windows sin adaptaciones.

## Ejemplo de Uso
### **Compilación**
```bash
g++ -o gestion_memoria gestion_memoria_system.cpp
```

### **Ejecución**
```bash
./gestion_memoria archivo.txt
```

## Conclusión
El uso de `system()` y `mmap()` en C++ permite gestionar archivos de manera eficiente, combinando automatización de operaciones en el sistema y acceso optimizado a archivos grandes. Sin embargo, se deben tomar precauciones con la seguridad en la ejecución de comandos y la gestión de memoria para garantizar un uso eficiente y seguro de los recursos del sistema.

## Código
```cpp
#include <iostream>
#include <fstream>
#include <cstdlib>
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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo>" << std::endl;
        return EXIT_FAILURE;
    }

    const char *ruta_archivo = argv[1];
    off_t tamano_archivo = obtener_tamano_archivo(ruta_archivo);

    std::cout << "[INFO] Ejecutando comando ls -lh sobre el archivo." << std::endl;
    std::string comando = "ls -lh " + std::string(ruta_archivo);
    system(comando.c_str());
    
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

    // Mostrar los primeros 100 caracteres del archivo
    std::cout << "[INFO] Primeros 100 caracteres del archivo:" << std::endl;
    for (size_t i = 0; i < 100 && i < tamano_archivo; ++i) {
        std::cout << data[i];
    }
    std::cout << std::endl;

    // Liberar la memoria mapeada
    if (munmap(data, tamano_archivo) == -1) {
        perror("Error al liberar memoria mapeada");
    }

    std::cout << "[INFO] Memoria liberada y procesamiento finalizado." << std::endl;

    return EXIT_SUCCESS;
}

```
