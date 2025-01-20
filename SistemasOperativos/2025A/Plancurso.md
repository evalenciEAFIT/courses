# Plan Completo del Curso: Sistemas Operativos (2025)

## Objetivo del Curso
El curso tiene como objetivo proporcionar a los estudiantes una comprensión sólida de los conceptos fundamentales y prácticos de los sistemas operativos. Los estudiantes aprenderán a instalar, configurar y operar distribuciones de Linux, así como a gestionar archivos, memoria y procesos concurrentes. Se desarrollarán habilidades para aplicar estos conceptos en proyectos prácticos que simulen escenarios del mundo real.

## Resultados Esperados
Al finalizar el curso, los estudiantes serán capaces de:

- Instalar y configurar sistemas operativos Linux en entornos locales.
- Aplicar comandos básicos y avanzados de Linux para administrar recursos del sistema.
- Implementar soluciones para problemas de gestión de archivos, memoria y procesos.
- Diseñar y desarrollar proyectos prácticos relacionados con sistemas operativos.
- Defender y presentar soluciones técnicas con un enfoque crítico y analítico.

## Unidades y Subobjetivos

### Unidad 1: Introducción a los Sistemas Operativos
- **Subobjetivo 1.1**: Comprender las funciones y tipos de sistemas operativos.
- **Subobjetivo 1.2**: Familiarizarse con la instalación y configuración de entornos Linux.
- **Actividades**: Instalación de una distribución de Linux en máquinas virtuales y configuración básica.

### Unidad 2: Manejo de Archivos y Sistemas de Archivos
- **Subobjetivo 2.1**: Aprender a gestionar archivos y permisos en Linux.
- **Subobjetivo 2.2**: Comprender la estructura de sistemas de archivos y realizar particionamiento.
- **Actividades**: Uso de comandos como `chmod`, `cp`, `mv` y configuración de particiones.

### Unidad 3: Gestión de Memoria
- **Subobjetivo 3.1**: Explorar los conceptos de memoria virtual, paginación y segmentación.
- **Subobjetivo 3.2**: Configurar y optimizar el uso de memoria swap.
- **Actividades**: Configuración de memoria y análisis de herramientas como `free` y `vmstat`.

### Unidad 4: Concurrencia y Planificación de Procesos
- **Subobjetivo 4.1**: Implementar sincronización de procesos mediante semáforos y mutex.
- **Subobjetivo 4.2**: Simular algoritmos de planificación como FIFO y Round Robin.
- **Actividades**: Desarrollo de programas en C++ para resolver problemas clásicos como productores-consumidores.

### Unidad 5: Proyecto Final
- **Subobjetivo 5.1**: Aplicar todos los conceptos aprendidos en un proyecto integrador.
- **Subobjetivo 5.2**: Documentar y defender el proyecto final.

## Evaluación del Curso
La evaluación del curso se distribuye de la siguiente manera:

- **Primer Parcial (25%)**: 15% examen teórico y 10% práctica sustentada.
- **Segundo Parcial (25%)**: 15% examen teórico y 10% práctica sustentada.
- **Tercer Parcial (25%)**: 15% examen teórico y 10% práctica sustentada.
- **Proyecto Final (25%)**: Evaluado en tres criterios: funcionalidad (40%), documentación (30%), y presentación (30%).

Cada parcial incluirá una parte teórica para evaluar los conocimientos conceptuales del estudiante y una práctica sustentada que permitirá demostrar habilidades técnicas en la resolución de problemas relacionados con los temas del curso.

# Plan Detallado Semana a Semana

| Sm | Fecha               | Actividad                | Tema / Actividad                               | Detalles                                    | Práctica o Evaluación | Lenguaje          |
|----|---------------------|--------------------------|-----------------------------------------------|---------------------------------------------|-------------------------|------------------|
| 1  | 21/01/2025 (Martes) | Clase                   | Introducción al curso                        | Presentación del programa y objetivos      | N/A                     | N/A              |
|    | 23/01/2025 (Jueves) | Clase                   | Introducción a sistemas operativos           | Conceptos básicos y funciones             | N/A                     | N/A              |
| 2  | 28/01/2025 (Martes) | Clase + Práctica       | Instalación de máquinas virtuales           | Configuración de VirtualBox y VM         | Configurar VM con Debian 12 | N/A              |
|    | 30/01/2025 (Jueves) | Clase + Práctica       | Instalación de Linux                         | Instalación de Debian y navegación      | Practicar comandos en terminal Linux | Bash             |
| 3  | 04/02/2025 (Martes) | Clase + Práctica       | Comandos básicos de Linux                   | Introducción a terminal y estructura     | Crear y eliminar archivos/directorios | Bash             |
|    | 06/02/2025 (Jueves) | Clase + Práctica       | WSL en Windows                                | Configuración inicial de WSL             | Configurar y usar WSL en Windows     | Bash             |
| 4  | 11/02/2025 (Martes) | Clase + Práctica       | Manejo de archivos en Linux                  | Copiar, mover, permisos                    | Administrar archivos y permisos      | Bash             |
|    | 13/02/2025 (Jueves) | Evaluación            | Primer parcial                                | Examen teórico y práctico               | Configuración inicial de Linux       | Bash             |
| 5  | 18/02/2025 (Martes) | Clase                   | Introducción a sistemas de archivos          | FAT, NTFS, EXT                            | N/A                     | N/A              |
|    | 20/02/2025 (Jueves) | Clase                   | Introducción a sistemas de archivos          | FAT, NTFS, EXT                            | N/A                     | N/A              |
| 6  | 25/02/2025 (Martes) | Clase + Práctica       | Particiones y montaje                        | Creación y gestión de particiones        | Crear particiones y sistemas de archivos. | Bash             |
|    | 27/02/2025 (Jueves) | Clase + Práctica       | Almacenamiento y caché                      | Introducción a jerarquías y tipos        | Analizar almacenamiento con comandos | Bash             |
| 7  | 04/03/2025 (Martes) | Clase + Práctica       | RAID básico y distribuidos                  | Configuración de RAID                    | Crear y administrar RAID              | Bash             |
|    | 06/03/2025 (Jueves) | Clase + Práctica       | RAID básico y distribuidos                  | Configuración de RAID                    | Crear y administrar RAID              | Bash             |
| 8  | 11/03/2025 (Martes) | Evaluación            | Segundo parcial                              | Examen teórico y práctico               | Montaje y sistemas de archivos       | Bash             |
|    | 13/03/2025 (Jueves) | Clase                   | Gestión de memoria: conceptos básicos       | Introducción a paginación y segmentación | Analizar uso de memoria con herramientas | C, C++, Rust     |
| 9  | 18/03/2025 (Martes) | Clase + Práctica       | Práctica de memoria virtual                 | Configuración de memoria swap            | Configurar y verificar uso de swap    | C, C++, Rust     |
|    | 20/03/2025 (Jueves) | Clase + Práctica       | Gestión avanzada de memoria                 | Soluciones a fragmentación               | Implementar y analizar soluciones     | C, C++, Rust     |
| 10 | 25/03/2025 (Martes) | Clase + Práctica       | Gestión avanzada de memoria                 | Soluciones a fragmentación               | Implementar y analizar soluciones     | C, C++, Rust     |
|    | 27/03/2025 (Jueves) | Clase + Práctica       | Introducción a concurrencia                 | Conceptos básicos de procesos            | Crear programa con hilos simples      | C, C++, Rust     |
| 11 | 01/04/2025 (Martes) | Clase + Práctica       | Sincronización de procesos                  | Problemas de sincronización              | Implementar semáforos en C++           | C, C++, Rust     |
|    | 03/04/2025 (Jueves) | Clase + Práctica       | Planificación de procesos                   | Algoritmos: FIFO, Round Robin             | Simular planificador Round Robin      | C, C++, Rust     |
| 12 | 08/04/2025 (Martes) | Clase + Práctica       | Planificación de procesos                   | Algoritmos: FIFO, Round Robin             | Simular planificador Round Robin      | C, C++, Rust     |
|    | 10/04/2025 (Jueves) | Evaluación            | Tercer parcial                               | Examen teórico y práctico               | Implementar algoritmos de concurrencia | C, C++, Rust     |
|    | 15/04/2025 (Martes) | Clase                   | Semana Santa                                 | No hay clase                              | N/A                     | N/A              |
|    | 17/04/2025 (Jueves) | Clase                   | Semana Santa                                 | No hay clase                              | N/A                     | N/A              |
| 13 | 22/04/2025 (Martes) | Planificación          | Planificación del proyecto final             | Asignación de roles y diseño inicial    | Crear documento de planificación       | N/A              |
|    | 24/04/2025 (Jueves) | Clase                   | Desarrollo inicial                           | Implementar módulos básicos             | N/A                     | N/A              |
| 14 | 29/04/2025 (Martes) | Clase                   | Desarrollo avanzado                          | Integración de gestión de memoria y procesos | N/A                     | N/A              |
|    | 01/05/2025 (Jueves) | Clase                   | Festivo (Día del Trabajo)                   | No hay clase                              | N/A                     | N/A              |
| 15 | 06/05/2025 (Martes) | Trabajo Final           | Ajustes finales                              | Integración de gestión de memoria y procesos | N/A                     | N/A              |
|    | 08/05/2025 (Jueves) | Trabajo Final           | Ajustes finales                              | Integración de gestión de memoria y procesos | N/A                     | Libre            |
| 17 | 13/05/2025 (Martes) | Defensa Final           | Preparación y defensa del proyecto          | Documentación y defensa del proyecto      | N/A                     | Libre            |
|    | 15/05/2025 (Jueves) | Defensa Final           | Exposición final                            | Exposición formal y entrega final         | N/A                     | Libre            |
| 18 | 20/05/2025 (Martes) | Cierre                  | Retroalimentación                           | Feedback del curso y cierre formal         | N/A                     | N/A              |
|    | 22/05/2025 (Jueves) | Cierre                  | Resultados finales                           | Revisión y entrega de calificaciones      | N/A                     | N/A              |



## Detalle del Proyecto Final

**Nombre del Proyecto:** Simulador de Sistema Operativo

**Objetivo:** Crear un simulador que gestione procesos, memoria y archivos, replicando las funciones básicas de un sistema operativo.

### Fases del Proyecto

1. **Planificación:**
   - Asignación de roles y definición del alcance.
   - Diseño del flujo de trabajo y especificación de módulos.
   - **Entregable:** Documento de diseño inicial.

2. **Desarrollo:**
   - Implementación de módulos:
     - Gestor de Memoria: Simulación de memoria virtual y paginación.
     - Planificador de Procesos: Aplicación de algoritmos de planificación.
     - Sistema de Archivos: Operaciones básicas como creación, lectura y eliminación.
   - **Entregable:** Código funcional de los módulos.

3. **Pruebas y Ajustes:**
   - Verificación de la integración de módulos.
   - Resolución de errores y optimizaciones finales.
   - **Entregable:** Versión final del simulador.

4. **Presentación:**
   - Exposición formal con demostración del simulador.
   - Defensa técnica y preguntas del evaluador.
   - **Entregable:** Presentación en formato PDF y repositorio del proyecto.

### Evaluación del Proyecto
- 40% Funcionalidad: Correcta implementación de los módulos.
- 30% Documentación: Claridad y calidad de los comentarios y explicaciones.
- 30% Presentación: Claridad y profundidad de la exposición y defensa.

### Entrega Final
- Repositorio en GitHub o similar con el código fuente y documentación.
- Documento de explicación técnica del proyecto.

## Normas de Clase
Para garantizar un ambiente de aprendizaje sano y productivo, se establecen las siguientes normas de comportamiento dentro del aula:

1. **Prohibición de Distracciones Electrónicas:** No está permitido el uso de celulares o cualquier dispositivo que genere distracciones durante las clases. En caso de incumplimiento, el estudiante será retirado del aula para evitar interrumpir el aprendizaje de sus compañeros.
2. **Respeto Mutuo:** Se espera que todos los estudiantes mantengan un ambiente de respeto hacia sus compañeros y el docente. Comentarios inapropiados o conductas disruptivas no serán tolerados.
3. **Participación Activa:** Los estudiantes deben estar atentos y participar en las actividades propuestas. La falta de atención constante puede impactar su desempeño y calificación final.

Estas normas buscan fomentar un entorno de concentración, colaboración y respeto, esenciales para alcanzar los objetivos del curso.

