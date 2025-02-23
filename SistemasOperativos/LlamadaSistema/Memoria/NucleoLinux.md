# Arquitecturas de Sistemas: Monolítico, Modular y Multitarea

Los sistemas operativos y las aplicaciones pueden organizarse de diversas maneras. Aquí exploraremos tres arquitecturas fundamentales: monolítica, modular y multitarea.

## 1. Sistema Monolítico

### Descripción

* Un sistema monolítico es aquel donde todos los componentes del sistema están interconectados y residen en un único bloque.
* Esto significa que no hay separación clara entre las diferentes funciones del sistema.
* Si un componente falla, todo el sistema puede verse afectado.

![Estructura de un sistema operativo monolítico](https://somebooks.es/wp-content/uploads/2022/SOM/estructura%20monolitica.png)
### Características

* **Ventajas**:
    * Rendimiento potencialmente alto debido a la comunicación directa entre componentes.
    * Simplicidad en el desarrollo inicial.
* **Desventajas**:
    * Difícil de mantener y escalar.
    * Un fallo en un componente puede afectar a todo el sistema.
    * Dificultad de evolucionar.

### Ejemplos

* Algunas versiones antiguas de Unix.
* Algunos sistemas Embebidos.

## 2. Sistema Modular

### Descripción

* Un sistema modular divide las funciones del sistema en módulos independientes y bien definidos.
* Cada módulo realiza una tarea específica y se comunica con otros módulos a través de interfaces bien definidas.
* Proporciona una forma mucho más eficiente de desarrollo y mantenimiento de los sistemas.

![Estructura de un sistema Modular - Hibrdo](http://somebooks.es/wp-content/uploads/2022/SOM/estructura%20nucleo%20hibrido.png)

### Características

* **Ventajas**:
    * Facilidad de mantenimiento y actualización.
    * Mayor robustez: un fallo en un módulo no necesariamente afecta a todo el sistema.
    * Facilidad para escalar.
* **Desventajas**:
    * Puede haber una sobrecarga debido a la comunicación entre módulos.
    * Mayor complejidad en el diseño.

### Ejemplos

* El núcleo de Linux, que, aunque es monolítico, tiene una arquitectura modular.
* Sistemas operativos modernos que utilizan micro núcleos o arquitecturas de micro servicios.

## 3. Sistema Multitarea

### Descripción

* La multitarea permite que un sistema ejecute múltiples tareas o procesos simultáneamente.
* Esto se logra mediante la alternancia rápida entre tareas, dando la ilusión de paralelismo.
* Este método es escencial en los tiempos actuales.

### Ilustración

+---------+     +---------+     +---------+
| Tarea 1 | --> | Tarea 2 | --> | Tarea 3 | --> ...
+---------+     +---------+     +---------+
^                         |
|_________________________|


### Tipos

* **Multitarea Cooperativa**: Las tareas ceden voluntariamente el control al sistema operativo.
* **Multitarea Preemptiva**: El sistema operativo interrumpe las tareas para asignar tiempo a otras.

### Características

* **Ventajas**:
    * Mayor eficiencia en el uso de los recursos del sistema.
    * Capacidad para ejecutar múltiples aplicaciones simultáneamente.
    * Mejora la experiencia del usuario.
* **Desventajas**:
    * Mayor complejidad en la gestión de recursos y la sincronización de tareas.
    * Posibles problemas de concurrencia (condiciones de carrera, bloqueos).

### Ejemplos

* Todos los sistemas operativos modernos, como Windows, Linux y macOS.

## Conclusión

Estas arquitecturas representan diferentes enfoques para el diseño de sistemas, cada uno con sus propias ventajas y desventajas. La elección de la arquitectura adecuada depende de los requisitos específicos del sistema y las necesidades del usuario.
