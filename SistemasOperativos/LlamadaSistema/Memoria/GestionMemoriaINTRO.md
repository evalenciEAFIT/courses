# Cómo el Sistema Operativo Gestiona el Crecimiento y la Reducción de la Pila (Stack) y el Montón (Heap)

La gestión de la memoria es crucial para el funcionamiento de un sistema operativo. Dos áreas clave que administra son la pila (stack) y el montón (heap). Ambas residen en la RAM, pero sirven para propósitos distintos y se gestionan de manera diferente.

![Mapa de memoria](https://upload.wikimedia.org/wikipedia/commons/thumb/2/29/Linux_Virtual_Memory_Layout_64bit.svg/800px-Linux_Virtual_Memory_Layout_64bit.svg.png)

## La Pila (Stack)

Imagina la pila como una pila de platos en una cafetería. Los platos se apilan uno encima del otro, y solo se puede acceder al plato superior. De manera similar, la pila almacena datos en un orden LIFO (último en entrar, primero en salir).

**Características:**

* **Almacenamiento:** Variables locales, parámetros de funciones, información de retorno.
* **Asignación:** Automática, gestionada por el compilador.
* **Crecimiento:** Hacia abajo en la memoria (direcciones decrecientes).
* **Tamaño:** Limitado, definido en tiempo de compilación.

**Gestión:**

* Cuando se llama a una función, se crea un nuevo marco de pila en la parte superior. Este marco contiene espacio para las variables locales y otros datos de la función.
* Al finalizar la función, el marco de pila se elimina, liberando la memoria.
* Este proceso de "empujar" (push) y "sacar" (pop) marcos de pila es muy eficiente.

## El Montón (Heap)

El montón es una región de memoria más flexible donde se asignan datos dinámicamente. Es como un almacén donde puedes tomar y devolver objetos en cualquier momento.

**Características:**

* **Almacenamiento:** Objetos, estructuras de datos dinámicas.
* **Asignación:** Manual, usando funciones como `malloc()` (en C) o `new` (en C++).
* **Crecimiento:** Hacia arriba en la memoria (direcciones crecientes).
* **Tamaño:** Flexible, limitado por la memoria disponible.

**Gestión:**

* El sistema operativo proporciona funciones para asignar y liberar memoria en el montón.
* La gestión del montón es más compleja que la de la pila, ya que el programador es responsable de liberar la memoria que ya no se necesita.
* La falta de liberación de memoria puede causar fugas de memoria, lo que agota los recursos del sistema.

## Crecimiento y Reducción

* **Pila:** Crece y se reduce automáticamente con las llamadas a funciones.
* **Montón:** Crece a medida que se asignan objetos y se reduce cuando se liberan.

## Interacción entre Pila y Montón

Aunque la pila y el montón son áreas de memoria separadas, pueden interactuar. Por ejemplo, una función puede asignar memoria en el montón y almacenar un puntero a esa memoria en la pila.

## Importancia de la Gestión Eficiente

Una gestión eficiente de la pila y el montón es esencial para el rendimiento y la estabilidad del sistema. Las fugas de memoria o los desbordamientos de pila pueden provocar fallos en las aplicaciones o incluso en todo el sistema.

## Explorando el Mapa de Memoria en Linux: Comandos Esenciales

Visita el enlace:
[Comandos para ver el mapa de memoria en Linux] (https://github.com/evalenciEAFIT/courses/blob/main/SistemasOperativos/LlamadaSistema/Memoria/MapaMemoriaLinux.md)

## Código ejemplo   
Visita el enlace:
[Ejemplo del código de memoria] (https://github.com/evalenciEAFIT/courses/blob/main/SistemasOperativos/LlamadaSistema/Memoria/codigoEjemploUsoMemoria_1.md)
