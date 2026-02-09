# 🚀 Script de Sincronización a GitHub

Script automatizado para sincronizar el proyecto de Memoria Virtual con el repositorio de GitHub.

## 📋 Descripción

Este script (`sync-to-github.sh`) automatiza el proceso de:
1. Copiar archivos del proyecto local al repositorio clonado
2. Crear un commit con los cambios
3. Hacer push a GitHub
4. Mostrar estadísticas del proyecto

## 🔧 Requisitos Previos

- Repositorio clonado en `/home/edi/PROYECTOS/courses/`
- Credenciales de Git configuradas
- `rsync` instalado (normalmente viene preinstalado en Linux)

## 📖 Uso

### Sintaxis Básica

```bash
./sync-to-github.sh [mensaje-de-commit]
```

### Ejemplos

**1. Con mensaje personalizado:**
```bash
./sync-to-github.sh "Agregar ejercicio 6 sobre paginación"
```

**2. Con mensaje por defecto (incluye fecha/hora):**
```bash
./sync-to-github.sh
```
Genera: `"Actualización automática del proyecto de Memoria Virtual - 2026-02-08 20:50:31"`

**3. Mensajes descriptivos:**
```bash
./sync-to-github.sh "Corregir bug en ejercicio 2"
./sync-to-github.sh "Actualizar documentación del README"
./sync-to-github.sh "Agregar tests para ejercicio 4"
```

## ✨ Características

### 🎨 Output Colorizado
- **Verde (✓):** Operaciones exitosas
- **Azul (▶):** Pasos en progreso
- **Amarillo (⚠):** Advertencias
- **Rojo (✗):** Errores
- **Magenta (ℹ):** Información adicional

### 🔍 Verificaciones Automáticas
- Verifica que existan los directorios necesarios
- Detecta si hay cambios para commitear
- Valida operaciones de Git

### 📊 Estadísticas
Al finalizar, muestra:
- Número de archivos `.c`
- Líneas de código C
- Archivos de documentación
- Tamaño total del proyecto

### 🚫 Archivos Excluidos
El script automáticamente excluye:
- `.git/` (directorio de Git)
- `*.o` (archivos objeto)
- `*.swp` (archivos temporales de vim)
- `*~` (backups)
- `.DS_Store` (archivos de macOS)

## 📂 Estructura de Directorios

```
/home/edi/PROYECTOS/
├── SO/memoria/                    # Proyecto local (origen)
│   ├── ejercicios/
│   ├── README.md
│   └── sync-to-github.sh         # ← Este script
│
└── courses/                       # Repositorio GitHub (destino)
    └── SO_XV6/temas/memoria/     # Aquí se copian los archivos
```

## 🔄 Flujo de Trabajo Típico

1. **Trabajar en el proyecto local:**
   ```bash
   cd /home/edi/PROYECTOS/SO/memoria
   # Editar archivos, compilar, probar...
   ```

2. **Sincronizar con GitHub:**
   ```bash
   ./sync-to-github.sh "Descripción de los cambios"
   ```

3. **Verificar en GitHub:**
   El script te dará el enlace directo:
   ```
   https://github.com/evalenciEAFIT/courses/tree/main/SO_XV6/temas/memoria
   ```

## 🛠️ Solución de Problemas

### Error: "Repositorio de GitHub no encontrado"

**Solución:**
```bash
cd /home/edi/PROYECTOS
git clone https://github.com/evalenciEAFIT/courses.git
```

### Error: "Permission denied (publickey)"

**Solución:** Las credenciales ya están configuradas con token. Si hay problemas:
```bash
git config --global credential.helper store
```

### Error: "rsync: command not found"

**Solución:**
```bash
sudo dnf install rsync  # Fedora/RHEL
# o
sudo apt install rsync  # Ubuntu/Debian
```

### Ver logs detallados

Si necesitas más información sobre qué está pasando:
```bash
bash -x ./sync-to-github.sh "mensaje"
```

## 📝 Notas Importantes

1. **Backup automático:** El script usa `rsync --delete`, lo que significa que elimina archivos en el destino que no existen en el origen. Esto mantiene ambos directorios sincronizados.

2. **Conflictos:** Si alguien más modificó el repositorio, el script te avisará. Necesitarás hacer `git pull` manualmente en `/home/edi/PROYECTOS/courses/`.

3. **Credenciales:** El token de GitHub ya está configurado y guardado. No necesitas volver a ingresarlo.

## 🎯 Casos de Uso Comunes

### Después de completar un ejercicio nuevo
```bash
./sync-to-github.sh "Completar ejercicio 6: Memoria virtual avanzada"
```

### Después de corregir bugs
```bash
./sync-to-github.sh "Fix: Corregir memory leak en ejercicio 2"
```

### Después de actualizar documentación
```bash
./sync-to-github.sh "Docs: Actualizar README con nuevas instrucciones"
```

### Sincronización rápida sin mensaje específico
```bash
./sync-to-github.sh
```

## 🔗 Enlaces Útiles

- **Repositorio:** https://github.com/evalenciEAFIT/courses
- **Proyecto en GitHub:** https://github.com/evalenciEAFIT/courses/tree/main/SO_XV6/temas/memoria
- **Documentación de Git:** https://git-scm.com/doc

## 📞 Soporte

Si encuentras problemas:
1. Verifica que los directorios existan
2. Asegúrate de tener conexión a internet
3. Revisa que las credenciales de Git estén configuradas
4. Ejecuta con `-x` para ver logs detallados

---

**Última actualización:** 8 de Febrero, 2026  
**Versión del script:** 1.0
