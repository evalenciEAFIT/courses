# Reto 1 - Shell EAFITos

## Descripción General
**Valor:** 25% de la nota final de la materia Sistemas Operativos  
**Fecha de entrega:** Semana del 23 de febrero de 2025 (23/02/2025)  
**Modalidad:** Individual o en equipos de 2 a 5 personas  

## Objetivo del Reto
Desarrollar una shell educativa llamada **EAFITos** en lenguaje C que implemente comandos básicos y avanzados para operaciones del sistema, siguiendo buenas prácticas de programación en C y trabajo colaborativo.

## Modalidades de Trabajo

### Opción 1: Individual
- **Requisitos mínimos:** 6 comandos básicos
- **Recomendado para:** Estudiantes que prefieren trabajo independiente

### Opción 2: En equipo (2-5 personas)
- **Requisitos mínimos:** 
  - 2 personas: 8 comandos (6 básicos + 2 avanzados)
  - 3 personas: 10 comandos (6 básicos + 4 avanzados)
  - 4 personas: 12 comandos (6 básicos + 6 avanzados)
  - 5 personas: 15 comandos (6 básicos + 9 avanzados)
- **Roles sugeridos:** Líder, Desarrolladores, Tester, Documentador

## Comandos por Categoría (Total: 20 comandos disponibles)

### Comandos Básicos (6 obligatorios para todos)
1. **`listar`** - Muestra contenido del directorio actual
2. **`leer <archivo>`** - Muestra contenido de un archivo de texto
3. **`tiempo`** - Muestra fecha y hora actual
4. **`calc <num1> <operador> <num2>`** - Calculadora básica
5. **`ayuda`** - Muestra lista de comandos
6. **`salir`** - Termina la shell

### Comandos de Archivos (7 disponibles)
7. **`crear <archivo>`** - Crea archivo vacío
8. **`eliminar <archivo>`** - Elimina archivo con confirmación
9. **`renombrar <viejo> <nuevo>`** - Renombra archivo
10. **`copiar <origen> <destino>`** - Copia archivo
11. **`mover <origen> <destino>`** - Mueve archivo
12. **`buscar <texto> <archivo>`** - Busca texto en archivo
13. **`estadisticas <archivo>`** - Estadísticas del archivo

### Comandos del Sistema (4 disponibles)
14. **`historial`** - Muestra últimos 10 comandos
15. **`limpiar`** - Limpia la pantalla
16. **`usuario`** - Muestra información del usuario
17. **`directorio`** - Muestra directorio actual

### Comandos Avanzados (3 disponibles)
18. **`grep <patron> <archivo>`** - Búsqueda con patrones
19. **`wc <archivo>`** - Contador de líneas/palabras/caracteres
20. **`comprimir <archivo>`** - Compresión básica (RLE)

## Requisitos Mínimos por Tamaño de Equipo

| Tamaño | Comandos Totales | Básicos | Avanzados | Features Extra |
|--------|------------------|---------|-----------|----------------|
| Individual | 6 | 6 | 0 | REPL básico |
| 2 personas | 8 | 6 | 2 | Historial + 1 extra |
| 3 personas | 10 | 6 | 4 | Makefile + pruebas |
| 4 personas | 12 | 6 | 6 | Modularidad + documentación |
| 5 personas | 15 | 6 | 9 | Interfaz mejorada + errores detallados |

## Estructura del Proyecto para Equipos

### Arquitectura Recomendada para 3+ Personas
```
eafitos/
├── src/
│   ├── core/                    # Núcleo del shell
│   │   ├── main.c              # Punto de entrada
│   │   ├── shell_loop.c        # Bucle REPL
│   │   └── parser.c            # Parseo de comandos
│   ├── commands/               # Comandos por categoría
│   │   ├── basic_commands.c    # 6 comandos básicos
│   │   ├── file_commands.c     # Comandos de archivos
│   │   ├── system_commands.c   # Comandos del sistema
│   │   └── advanced_commands.c # Comandos avanzados
│   └── utils/                  # Utilidades
│       ├── memory_manager.c    # Gestión de memoria
│       ├── error_handler.c     # Manejo de errores
│       └── helpers.c           # Funciones auxiliares
├── include/                    # Headers
│   ├── shell.h
│   ├── commands.h
│   ├── parser.h
│   └── utils.h
├── tests/                      # Pruebas
│   ├── unit_tests.c
│   ├── integration_tests.c
│   └── test_runner.sh
├── docs/                       # Documentación
│   ├── API.md
│   ├── DESIGN.md
│   └── USER_GUIDE.md
├── scripts/                    # Scripts auxiliares
│   ├── build.sh
│   └── format.sh
├── Makefile                    # Build system
├── README.md                   # Documentación principal
├── CONTRIBUTORS.md            # Colaboradores y roles
├── CHANGELOG.md               # Historial de cambios
└── LICENSE                    # Licencia del proyecto
```

### Roles Sugeridos para Equipos Grandes
| Rol | Responsabilidades | Personas |
|-----|-------------------|----------|
| **Líder del Proyecto** | Coordinación, integración, entrega | 1 |
| **Arquitecto** | Diseño, estructura, headers | 1 |
| **Desarrolladores** | Implementación comandos | 2-3 |
| **Tester** | Pruebas, calidad, depuración | 1 |
| **Documentador** | Documentación, README, ejemplos | 1 |

## Rúbrica de Evaluación

### Total: 100 puntos (25% de la nota final)

#### **Funcionalidad (40 puntos)**
| Criterio | Individual | 2 pers | 3 pers | 4 pers | 5 pers |
|----------|------------|--------|--------|--------|--------|
| Comandos mínimos | 24/24 | 24/24 | 24/24 | 24/24 | 24/24 |
| Comandos extra | 0/8 | 4/8 | 8/8 | 12/12 | 18/18 |
| REPL funcional | 8/8 | 8/8 | 8/8 | 8/8 | 8/8 |
| **Subtotal** | **32/40** | **36/40** | **40/40** | **44/40** | **50/40** |

#### **Calidad del Código (30 puntos)**
| Criterio | Individual | Equipo 2-3 | Equipo 4-5 | Descripción |
|----------|------------|------------|------------|-------------|
| Modularidad | 6 | 8 | 10 | Separación en módulos |
| Documentación | 6 | 8 | 10 | Comentarios + docs |
| Estilo | 4 | 5 | 5 | Convenciones |
| Memoria | 4 | 4 | 5 | Sin leaks |
| Makefile | 4 | 5 | 5 | Build system |
| **Subtotal** | **24/30** | **30/30** | **35/30** | |

#### **Colaboración (20 puntos - solo equipos)**
| Criterio | 2 pers | 3 pers | 4 pers | 5 pers | Descripción |
|----------|--------|--------|--------|--------|-------------|
| Git usage | 5 | 5 | 5 | 5 | Commits, branches |
| Distribución | 5 | 5 | 5 | 5 | Trabajo equilibrado |
| Integración | 5 | 5 | 5 | 5 | Código integrado |
| Roles | 5 | 5 | 5 | 5 | Roles definidos |
| **Subtotal** | **20/20** | **20/20** | **20/20** | **20/20** | |

#### **Features Avanzadas (10 puntos)**
| Criterio | Puntos | Descripción |
|----------|--------|-------------|
| Pruebas automáticas | 3 | Unit tests |
| Sistema de ayuda | 2 | Ayuda por comando |
| Manejo de señales | 2 | Ctrl+C, Ctrl+Z |
| Interfaz mejorada | 3 | Colores, prompt personalizable |

### Esquema de Calificación Final
```
Para Individual:
Nota = (Funcionalidad + Calidad + Features) × 1.0

Para Equipos:
Nota = (Funcionalidad + Calidad + Colaboración + Features) × Factor

Factor de Equipo:
- 2 personas: × 1.0
- 3 personas: × 0.95
- 4 personas: × 0.90
- 5 personas: × 0.85

Nota máxima: 100 puntos
```

### Penalizaciones Comunes
- **Entrega tardía:** -10 puntos/día
- **No compila:** -20 puntos
- **Uso de system():** -15 puntos
- **Memory leaks:** -10 puntos
- **Sin documentación de roles:** -10 puntos (equipos)
- **Contribución desigual:** Ajuste individual según evaluación

## Cronograma para Equipos Grandes

### Fase 1: Planificación (Hasta 02/02)
- Formación de equipos
- Asignación de roles
- Diseño de arquitectura
- División de tareas

### Fase 2: Desarrollo (02/02 - 16/02)
- **Semana 1:** Núcleo del shell + comandos básicos
- **Semana 2:** Comandos avanzados + integración
- **Checkpoint (16/02):** Revisión de avances (opcional)

### Fase 3: Integración y Pruebas (16/02 - 21/02)
- Integración de módulos
- Pruebas unitarias e integración
- Depuración y optimización

### Fase 4: Entrega (22/02 - 23/02)
- Documentación final
- Preparación de entrega
- **Entrega final: 23/02**

## Instrucciones de Entrega para Equipos

### Estructura de Archivos
```
NOMBRE_PROYECTO/
├── src/                      # Código fuente organizado
├── docs/
│   ├── ROLES.md             # Descripción de roles y contribuciones
│   └── DESIGN_DECISIONS.md  # Decisiones de diseño
├── entregables/
│   ├── informe.pdf          # Informe técnico (opcional)
│   └── presentacion.pptx    # Presentación (si aplica)
└── README.md                # Debe incluir:
    - Integrantes y porcentajes
    - Diagrama de arquitectura
    - Instrucciones de compilación
    - Guía de uso completa
```

### Repositorio Git Requerido para Equipos 3+
- Debe mostrar actividad de todos los miembros
- Commits significativos con mensajes claros
- Uso de branches para features
- Merge requests/review de código

### Informe de Contribución
Cada equipo debe entregar un archivo `CONTRIBUTION.md` con:
```markdown
# Contribución de Miembros

|  Nombre      | Código | Rol     | Tareas Asignadas           | % Contribución |
|--------------|--------|---------|----------------------------|----------------|
| Ana Pérez    | 1001   | Líder   | Coordinación, shell loop   | 20%            |
| Luis Gómez   | 1002   | Dev     | Comandos archivos, parser  | 25%            |
| Carlos Ruiz  | 1003   | Dev     | Comandos sistema, utils    | 25%            |
| María López  | 1004   | Tester  | Pruebas, depuración        | 20%            |
| Pedro Díaz   | 1005   | Doc     | Documentación, ejemplos    | 10%            |

Total: 100%
```

## Evaluación de Colaboración

### Evidencia Requerida
1. **Historial de Git:** Commits de cada miembro
2. **Revisiones de código:** Comentarios en pull requests
3. **Distribución de archivos:** Quién trabajó en qué
4. **Consistencia:** Estilo de código uniforme

### Entrevistas (si aplica)
- Preguntas individuales sobre cualquier parte del código
- Cada miembro debe poder explicar la arquitectura completa
- Evaluación de comprensión, no solo implementación

## Recursos para Equipos Grandes

### Herramientas Recomendadas
1. **Control de versiones:** Git + GitHub/GitLab
2. **Comunicación:** Discord/Teams para coordinación
3. **Gestión de tareas:** Trello, GitHub Projects, o similar
4. **Revisión de código:** GitHub Pull Requests

### Patrones de Diseño Sugeridos
```c
// Patrón: Tabla de comandos (Command Pattern)
typedef struct {
    char *nombre;
    void (*funcion)(char **);
    char *descripcion;
    char *uso;
} Comando;

Comando comandos[] = {
    {"listar", cmd_listar, "Lista archivos", "listar [opciones]"},
    {"leer", cmd_leer, "Lee archivo", "leer <archivo>"},
    // ...
};

// Patrón: Módulos independientes
// Cada comando en archivo separado, compilado por separado
```

### Convenciones de Equipo
1. **Estilo de código:** Acordar convenciones al inicio
2. **Comentarios:** Formato uniforme (Doxygen o similar)
3. **Estructura de commits:** Conventional commits
4. **Reuniones:** Standups semanales para sincronización

## Puntos Extra para Equipos Grandes

### Características Avanzadas (+5 puntos cada una)
1. **Sistema de plugins:** Añadir comandos dinámicamente
2. **Autocompletado:** Con tabulador
3. **Scripting:** Ejecutar archivos .eafitos
4. **Variables de entorno:** Personalizables
5. **Logging:** Sistema de registro de actividades
6. **Internacionalización:** Soporte múltiples idiomas
7. **Temas:** Interfaz personalizable

### Optimizaciones Técnicas (+3 puntos cada una)
1. **Compilación condicional:** Features opcionales
2. **Benchmarking:** Comparación de performance
3. **Análisis estático:** Integración con herramientas
4. **Continuous Integration:** GitHub Actions/GitLab CI

## Consideraciones Especiales

### Para Equipos de 5 Personas
- Se espera calidad profesional
- Documentación exhaustiva
- Pruebas completas
- Código bien estructurado
- Presentación organizada

### Evaluación de Contribución Individual
- **Auto-evaluación:** Cada miembro evalúa su contribución
- **Evaluación por pares:** Miembros evalúan a sus compañeros
- **Evidencia tangible:** Commits, issues resueltos, documentación

### Conflictos en Equipo
- Reportar al profesor antes del 16/02 si hay problemas
- Posibilidad de redistribución o cambio de modalidad
- Después del 16/02, el equipo se mantiene

---

**Nota:** Trabajar en equipo grande (4-5 personas) simula entornos profesionales reales donde la coordinación, comunicación y gestión de proyectos son tan importantes como las habilidades técnicas. La evaluación considerará tanto el producto final como el proceso de desarrollo colaborativo.
