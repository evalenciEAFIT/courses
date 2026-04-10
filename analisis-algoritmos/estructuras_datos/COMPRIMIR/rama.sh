#!/bin/bash

# ==============================================================================
#  rama.sh — Script de sincronización del Proyecto COMPRIMIR
#  Propósito: Copiar el estado actual al repositorio 'courses' y subir a GitHub
# ==============================================================================

# Colores para una salida premium
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Configuración de rutas
SOURCE_DIR="/home/edi/PROYECTOS/EDA/COMPRIMIR"
TARGET_DIR="/home/edi/PROYECTOS/courses/analisis-algoritmos/estructuras_datos/COMPRIMIR"
REPO_DIR="/home/edi/PROYECTOS/courses"

echo -e "${BLUE}▶ Iniciando actualización del proyecto COMPRIMIR...${NC}"

# 1. Verificar directorios
if [ ! -d "$SOURCE_DIR" ]; then
    echo -e "${RED}✘ Error: Directorio origen no encontrado.${NC}"
    exit 1
fi

if [ ! -d "$REPO_DIR" ]; then
    echo -e "${RED}✘ Error: Repositorio 'courses' no encontrado en $REPO_DIR.${NC}"
    exit 1
fi

mkdir -p "$TARGET_DIR"

# 2. Sincronizar archivos (excluyendo binarios y temporales)
echo -e "${BLUE}▶ Copiando archivos a la estructura del curso...${NC}"
rsync -av --delete \
    --exclude='bin/' \
    --exclude='app/comprimir' \
    --exclude='samples/comprimidos/' \
    --exclude='samples/descomprimidos/' \
    --exclude='.git/' \
    --exclude='*.o' \
    "$SOURCE_DIR/" "$TARGET_DIR/"

# 3. Operaciones de Git
echo -e "${BLUE}▶ Preparando commit en el repositorio local...${NC}"
cd "$REPO_DIR" || exit

git add .

# Verificar si hay cambios antes de hacer commit
if git diff --cached --quiet; then
    echo -e "${YELLOW}ℹ No hay cambios nuevos detectados.${NC}"
else
    COMMIT_MSG="COMPRIMIR: Actualización automática $(date '+%Y-%m-%d %H:%M:%S')"
    git commit -m "$COMMIT_MSG"
    echo -e "${GREEN}✓ Archivos comprometidos localmente.${NC}"
fi

# 4. Push a GitHub
echo -e "${BLUE}▶ Subiendo cambios a GitHub (evalenciEAFIT/courses)...${NC}"
git push origin main

if [ $? -eq 0 ]; then
    echo -e "${GREEN}╔════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║  ✓ ACTUALIZACIÓN COMPLETADA EXITOSAMENTE                       ║${NC}"
    echo -e "${GREEN}╚════════════════════════════════════════════════════════════════╝${NC}"
else
    echo -e "${RED}✘ Error al subir a GitHub. Verifica tu conexión o token (PAT).${NC}"
    exit 1
fi
