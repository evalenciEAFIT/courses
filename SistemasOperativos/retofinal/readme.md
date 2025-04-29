# Proyecto Final: Sistema de Backup Seguro con Algoritmos de Compresión Clásicos, Dask y OpenMP

## Enunciado

Desarrollar un sistema de respaldo seguro que permita a los usuarios seleccionar múltiples carpetas de un disco duro, respaldar todos los archivos de dichas carpetas (incluyendo sus subcarpetas), comprimirlos en un único archivo de backup utilizando algoritmos de compresión clásicos (ZIP, GZIP, o BZIP2), y opcionalmente encriptarlo. El archivo de backup generado podrá almacenarse en un disco duro externo, en un servicio de almacenamiento en la nube, o dividirse en fragmentos para guardarse en dispositivos USB. El sistema debe implementar técnicas de paralelismo utilizando **Dask** si se desarrolla en Python o **OpenMP** si se desarrolla en C++, para optimizar el rendimiento en la compresión, encriptación (si aplica) y transferencia de datos. Se permite el uso de bibliotecas externas para compresión, encriptación y otras funcionalidades.

### Requisitos Funcionales
1. **Selección de Carpetas**: El sistema debe permitir al usuario seleccionar múltiples carpetas. Todos los archivos dentro de las carpetas seleccionadas, incluidas sus subcarpetas, serán respaldados.
2. **Compresión**: Los archivos seleccionados deben comprimirse en un único archivo de backup utilizando uno de los siguientes algoritmos de compresión clásicos:
   - **ZIP**: Basado en el algoritmo DEFLATE.
   - **GZIP**: También basado en DEFLATE, optimizado para compresión de archivos individuales.
   - **BZIP2**: Basado en el algoritmo Burrows-Wheeler, con mejor compresión pero mayor uso de recursos.
3. **Encriptación Opcional**: Si el usuario lo desea, el archivo de backup debe encriptarse utilizando un algoritmo seguro (por ejemplo, AES-256). La encriptación debe proteger el contenido del archivo y requerir una clave para su restauración.
4. **Opciones de Almacenamiento**:
   - **Disco Duro Externo**: El archivo de backup debe poder copiarse a un disco duro externo conectado.
   - **Nube**: El sistema debe integrarse con al menos un servicio de almacenamiento en la nube (por ejemplo, Google Drive, Dropbox, o un servidor propio vía API).
   - **Fragmentación en USB**: El archivo de backup debe poder dividirse en fragmentos de tamaño configurable para almacenarse en múltiples dispositivos USB.
5. **Restauración**: El sistema debe permitir restaurar los archivos desde el backup, descomprimiendo y, si aplica, desencriptando el contenido.
6. **Interfaz de Usuario**: Puede ser una interfaz gráfica (GUI) o de línea de comandos (CLI), pero debe ser intuitiva y permitir configurar todas las opciones mencionadas.

### Requisitos Técnicos
1. **Paralelismo**:
   - **Python con Dask**: Si se elige Python, el sistema debe usar **Dask** para implementar paralelismo. Ejemplos incluyen:
     - Procesar múltiples archivos o carpetas simultáneamente durante la compresión usando `dask.bag` o `dask.array`.
     - Dividir tareas de encriptación en tareas paralelas gestionadas por Dask.
     - Gestionar transferencias paralelas a la nube o dispositivos USB con `dask.delayed`.
   - **C++ con OpenMP**: Si se elige C++, el sistema debe usar **OpenMP** para implementar paralelismo. Ejemplos incluyen:
     - Paralelizar bucles de compresión o encriptación con directivas `#pragma omp parallel for`.
     - Gestionar múltiples hilos para leer/escribir archivos o transferir datos a dispositivos con `#pragma omp sections`.
   - El paralelismo debe aplicarse en al menos dos etapas del proceso (por ejemplo, compresión y transferencia).
2. **Lenguaje de Programación**: Python (usando Dask) o C++ (usando OpenMP). La elección debe justificarse en el informe.
3. **Algoritmos de Compresión**: El sistema debe implementar uno de los algoritmos clásicos (ZIP, GZIP, o BZIP2). Se permite usar bibliotecas externas, como:
   - Python: `zipfile`, `gzip`, `bz2`, o `pyzipper` (para ZIP con encriptación).
   - C++: `zlib` (para GZIP/ZIP), `bzip2`, o bibliotecas como `libarchive`.
4. **Encriptación**: Se permite usar bibliotecas para encriptación, como:
   - Python: `cryptography` o `pycryptodome` para AES-256.
   - C++: `OpenSSL` o `Crypto++` para AES-256.
5. **Gestión de Errores**: El sistema debe manejar errores como fallos de conexión a la nube, dispositivos USB llenos, o claves de encriptación incorrectas, informando al usuario de manera clara.
6. **Documentación**: Incluir un informe técnico que describa:
   - La arquitectura del sistema.
   - La implementación de paralelismo con Dask o OpenMP (incluyendo ejemplos de código relevantes).
   - El algoritmo de compresión clásico elegido y la biblioteca utilizada.
   - La implementación de encriptación (si aplica) y la biblioteca utilizada.
   - Decisiones de diseño y justificación del lenguaje elegido.

### Entregables
1. Código fuente del sistema, organizado y comentado.
2. Ejecutable o instrucciones claras para compilar/ejecutar el programa (incluyendo instalación de Dask, OpenMP, y bibliotecas externas).
3. Informe técnico (máximo 10 páginas) que incluya:
   - Descripción de la arquitectura.
   - Explicación detallada del uso de Dask o OpenMP para paralelismo.
   - Descripción del algoritmo de compresión clásico y la biblioteca utilizada.
   - Justificación del lenguaje, bibliotecas, y herramientas seleccionadas.
   - Instrucciones de uso.
4. Demostración funcional en clase o video grabado (máximo 5 minutos).

### Restricciones
- El sistema debe ser compatible con al menos un sistema operativo (Windows, Linux, o macOS).
- El uso de bibliotecas externas para compresión (por ejemplo, `zlib`, `zipfile`) y encriptación (por ejemplo, `cryptography`, `OpenSSL`) está explícitamente permitido, pero debe documentarse.
- El tiempo de ejecución debe optimizarse mediante Dask o OpenMP, especialmente para grandes volúmenes de datos.
- Para Python, Dask debe usarse explícitamente para tareas de paralelismo; no se aceptarán soluciones basadas únicamente en `multiprocessing` o `threading`.
- Para C++, OpenMP debe usarse para gestionar el paralelismo; no se aceptarán soluciones basadas únicamente en hilos nativos (`std::thread`).
- El sistema debe usar uno de los algoritmos de compresión clásicos especificados (ZIP, GZIP, o BZIP2).

### Rúbrica de Evaluación

| **Criterio**                              | **Puntos** | **Descripción**                                                                 |
|-------------------------------------------|------------|--------------------------------------------------------------------------------|
| **Funcionalidad Completa**                | 30         | Implementa correctamente la selección de carpetas, compresión con ZIP/GZIP/BZIP2, encriptación opcional, y todas las opciones de almacenamiento (disco, nube, USB). |
| **Uso de Paralelismo (Dask/OpenMP)**      | 25         | Aplica Dask (Python) o OpenMP (C++) de manera efectiva en al menos dos etapas (por ejemplo, compresión y transferencia). Se explica claramente en el informe con ejemplos de código. |
| **Uso de Algoritmos Clásicos**            | 15         | Implementa correctamente uno de los algoritmos de compresión clásicos (ZIP, GZIP, o BZIP2) usando bibliotecas adecuadas, con resultados funcionales. |
| **Gestión de Errores**                    | 10         | Maneja errores de forma robusta (fallos de red, dispositivos llenos, claves incorrectas) y proporciona mensajes claros al usuario. |
| **Interfaz de Usuario**                   | 10         | Interfaz (GUI o CLI) intuitiva, fácil de usar, y con opciones bien organizadas. |
| **Documentación y Código**                | 10         | Código limpio, comentado, y organizado. Informe técnico claro, conciso, y completo, con énfasis en el uso de Dask/OpenMP, algoritmo de compresión, y bibliotecas. |

### Notas Adicionales
- La evaluación premiará la implementación eficiente de Dask o OpenMP y la correcta integración de algoritmos de compresión clásicos, demostrada mediante pruebas con al menos 1 GB de datos.
- Los proyectos que no usen Dask (para Python), OpenMP (para C++), o que no implementen un algoritmo de compresión clásico (ZIP, GZIP, o BZIP2) recibirán una penalización significativa.
- Se recomienda incluir métricas de rendimiento (por ejemplo, tiempo de ejecución con y sin paralelismo, tasa de compresión) en el informe.
- Fecha de entrega: [24 de mayo 2025].
