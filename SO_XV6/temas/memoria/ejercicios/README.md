# 🎯 Ejercicios Prácticos - Memoria Virtual

Este directorio contiene ejercicios para profundizar en los conceptos de memoria virtual.

## 📋 Lista de Ejercicios

### Ejercicio 1: Implementar un Allocator Simple ⭐
**Dificultad:** Básica  
**Archivo:** `ejercicio1_allocator.c`  
**Objetivo:** Crear tu propio `my_malloc()` y `my_free()` usando mmap()

### Ejercicio 2: Memoria Compartida ⭐⭐
**Dificultad:** Intermedia  
**Archivo:** `ejercicio2_shared_memory.c`  
**Objetivo:** Comunicar dos procesos usando memoria compartida con mmap()

### Ejercicio 3: Memory-Mapped File ⭐⭐
**Dificultad:** Intermedia  
**Archivo:** `ejercicio3_mmap_file.c`  
**Objetivo:** Mapear un archivo en memoria y procesarlo eficientemente

### Ejercicio 4: Profiling de Page Faults ⭐⭐⭐
**Dificultad:** Avanzada  
**Archivo:** `ejercicio4_page_faults.c`  
**Objetivo:** Medir y analizar page faults en diferentes escenarios

### Ejercicio 5: Implementar COW Manual ⭐⭐⭐
**Dificultad:** Avanzada  
**Archivo:** `ejercicio5_cow.c`  
**Objetivo:** Simular Copy-on-Write usando mprotect() y signal handlers

---

## 🚀 Cómo Empezar

1. Lee el enunciado del ejercicio en el archivo correspondiente
2. Implementa la solución
3. Compila con: `gcc -Wall -Wextra -o ejercicioN ejercicioN_*.c`
4. Ejecuta y verifica los resultados
5. Compara con la solución propuesta (si está disponible)

---

## 📝 Plantilla Base

Cada ejercicio sigue esta estructura:

```c
/*
 * EJERCICIO N: [Título]
 * 
 * Objetivo:
 *   [Descripción del objetivo]
 * 
 * Conceptos:
 *   - [Concepto 1]
 *   - [Concepto 2]
 * 
 * Tareas:
 *   1. [Tarea 1]
 *   2. [Tarea 2]
 * 
 * Bonus:
 *   - [Desafío adicional]
 */

// Tu código aquí
```

---

## ✅ Criterios de Evaluación

- **Funcionalidad:** ¿El código hace lo que se pide?
- **Corrección:** ¿Maneja errores apropiadamente?
- **Eficiencia:** ¿Usa los recursos de forma óptima?
- **Claridad:** ¿El código es legible y está comentado?
- **Completitud:** ¿Implementa todas las tareas y bonus?

---

**Nota:** Los ejercicios están diseñados para ser progresivamente más desafiantes. ¡No te desanimes si los primeros te parecen difíciles!
