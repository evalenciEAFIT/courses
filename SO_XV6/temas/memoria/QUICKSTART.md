# 🚀 Guía Rápida - Memoria Virtual

## Comandos Esenciales

```bash
# Compilar y ejecutar
make
make run

# Limpiar
make clean

# Ejecutar con valgrind
make valgrind

# Ver información del sistema
make info

# Ver mapa de memoria del proceso
make memmap
```

## Estructura del Proyecto

```
memoria/
├── README.md              # Guía completa del proyecto
├── NOTAS.md              # Notas de clase y teoría
├── QUICKSTART.md         # Esta guía rápida
├── Makefile              # Automatización de compilación
├── mem1.c                # Programa principal con demostraciones
├── ejercicios/           # Ejercicios prácticos
│   ├── README.md
│   └── ejercicio1_allocator.c
└── mem1                  # Ejecutable (generado)
```

## Demostraciones Incluidas

### 1️⃣ Mapeo de Memoria del Proceso
Visualiza los segmentos: TEXT, DATA, HEAP, STACK

### 2️⃣ mmap - Mapeo de Memoria
Aprende a usar mmap() para control fino de memoria

### 3️⃣ Separación de Espacios
Entiende Copy-on-Write y aislamiento entre procesos

### 4️⃣ Protección de Páginas
Experimenta con permisos R/W/X y segmentation faults

## Conceptos Clave

| Concepto | Descripción |
|----------|-------------|
| **Dirección Virtual** | Dirección que usa el programa (traducida por MMU) |
| **Página** | Bloque de memoria (típicamente 4KB) |
| **mmap()** | Syscall para mapear memoria |
| **fork()** | Crea proceso hijo (usa COW) |
| **COW** | Copy-on-Write: optimización en fork() |
| **Page Fault** | Excepción cuando se accede a página no presente |
| **ASLR** | Randomización de direcciones (seguridad) |

## Experimentos Rápidos

### Ver direcciones cambiantes (ASLR)
```bash
./mem1 | grep "TEXT"
./mem1 | grep "TEXT"
./mem1 | grep "TEXT"
# Observa que las direcciones cambian
```

### Ver mapa de memoria en tiempo real
```bash
./mem1 &
cat /proc/$!/maps
kill %1
```

### Medir page faults
```bash
/usr/bin/time -v ./mem1 2>&1 | grep fault
```

## Recursos de Aprendizaje

📖 **README.md** - Guía completa con teoría y ejemplos  
📝 **NOTAS.md** - Conceptos teóricos detallados  
🎯 **ejercicios/** - Práctica hands-on  

## Próximos Pasos

1. ✅ Ejecuta `make run` para ver las demostraciones
2. ✅ Lee `README.md` para entender la teoría
3. ✅ Revisa `NOTAS.md` para profundizar
4. ✅ Intenta los ejercicios en `ejercicios/`
5. ✅ Experimenta modificando `mem1.c`

## Ayuda

```bash
make help          # Ver todos los comandos disponibles
man mmap          # Manual de mmap()
man fork          # Manual de fork()
man proc          # Manual de /proc filesystem
```

## Preguntas Frecuentes

**P: ¿Por qué las direcciones cambian cada vez?**  
R: ASLR (Address Space Layout Randomization) para seguridad

**P: ¿Qué es un page fault?**  
R: Excepción cuando se accede a página no presente en RAM

**P: ¿Cuál es la diferencia entre malloc() y mmap()?**  
R: malloc() es función de biblioteca, mmap() es syscall con más control

**P: ¿Qué es Copy-on-Write?**  
R: Optimización donde fork() comparte páginas hasta que se modifican

---

**¡Feliz aprendizaje! 🎓**
