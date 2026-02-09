#!/bin/bash

################################################################################
# Script de Sincronización: Proyecto Memoria Virtual → GitHub
################################################################################
#
# Propósito:
#   Sincronizar automáticamente los cambios del proyecto local de memoria
#   virtual al repositorio de GitHub (evalenciEAFIT/courses).
#
# Uso:
#   ./sync-to-github.sh [mensaje-de-commit]
#
# Ejemplos:
#   ./sync-to-github.sh "Agregar ejercicio 6"
#   ./sync-to-github.sh "Corregir bug en ejercicio 2"
#   ./sync-to-github.sh  # Usa mensaje por defecto
#
################################################################################

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Configuración
PROJECT_DIR="/home/edi/PROYECTOS/SO/memoria"
GITHUB_REPO="/home/edi/PROYECTOS/courses"
TARGET_DIR="SO_XV6/temas/memoria"

################################################################################
# Funciones auxiliares
################################################################################

print_header() {
    echo -e "${CYAN}╔════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${CYAN}║  Sincronización: Memoria Virtual → GitHub                     ║${NC}"
    echo -e "${CYAN}╚════════════════════════════════════════════════════════════════╝${NC}"
    echo ""
}

print_step() {
    echo -e "${BLUE}▶${NC} $1"
}

print_success() {
    echo -e "${GREEN}✓${NC} $1"
}

print_error() {
    echo -e "${RED}✗${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}⚠${NC} $1"
}

print_info() {
    echo -e "${MAGENTA}ℹ${NC} $1"
}

################################################################################
# Verificaciones previas
################################################################################

check_directories() {
    print_step "Verificando directorios..."
    
    if [ ! -d "$PROJECT_DIR" ]; then
        print_error "Directorio del proyecto no encontrado: $PROJECT_DIR"
        exit 1
    fi
    
    if [ ! -d "$GITHUB_REPO" ]; then
        print_error "Repositorio de GitHub no encontrado: $GITHUB_REPO"
        print_info "Ejecuta: cd /home/edi/PROYECTOS && git clone https://github.com/evalenciEAFIT/courses.git"
        exit 1
    fi
    
    print_success "Directorios verificados"
}

################################################################################
# Sincronización de archivos
################################################################################

sync_files() {
    print_step "Sincronizando archivos..."
    
    # Crear directorio de destino si no existe
    mkdir -p "$GITHUB_REPO/$TARGET_DIR"
    
    # Copiar archivos (excluyendo .git y archivos temporales)
    rsync -av --delete \
        --exclude='.git' \
        --exclude='*.o' \
        --exclude='*.swp' \
        --exclude='*~' \
        --exclude='.DS_Store' \
        "$PROJECT_DIR/" "$GITHUB_REPO/$TARGET_DIR/"
    
    if [ $? -eq 0 ]; then
        print_success "Archivos sincronizados"
    else
        print_error "Error al sincronizar archivos"
        exit 1
    fi
}

################################################################################
# Git operations
################################################################################

git_operations() {
    local commit_message="$1"
    
    cd "$GITHUB_REPO" || exit 1
    
    # Verificar estado
    print_step "Verificando estado de Git..."
    git status --short
    echo ""
    
    # Agregar cambios
    print_step "Agregando cambios al staging area..."
    git add "$TARGET_DIR/"
    
    if [ $? -ne 0 ]; then
        print_error "Error al agregar archivos"
        exit 1
    fi
    print_success "Cambios agregados"
    
    # Verificar si hay cambios para commitear
    if git diff --cached --quiet; then
        print_warning "No hay cambios para commitear"
        return 0
    fi
    
    # Crear commit
    print_step "Creando commit..."
    git commit -m "$commit_message"
    
    if [ $? -ne 0 ]; then
        print_error "Error al crear commit"
        exit 1
    fi
    print_success "Commit creado"
    
    # Mostrar resumen del commit
    echo ""
    print_info "Resumen del commit:"
    git log -1 --stat --color=always
    echo ""
    
    # Push a GitHub
    print_step "Subiendo cambios a GitHub..."
    git push origin main
    
    if [ $? -eq 0 ]; then
        print_success "Push exitoso"
        echo ""
        print_info "Ver cambios en: https://github.com/evalenciEAFIT/courses/tree/main/$TARGET_DIR"
    else
        print_error "Error al hacer push"
        print_warning "Verifica tu conexión a internet y credenciales de Git"
        exit 1
    fi
}

################################################################################
# Estadísticas
################################################################################

show_stats() {
    print_step "Estadísticas del proyecto..."
    
    cd "$PROJECT_DIR" || exit 1
    
    echo ""
    echo -e "${CYAN}Archivos C:${NC}"
    find . -name "*.c" -type f | wc -l
    
    echo -e "${CYAN}Líneas de código C:${NC}"
    find . -name "*.c" -type f -exec wc -l {} + | tail -1 | awk '{print $1}'
    
    echo -e "${CYAN}Archivos de documentación:${NC}"
    find . -name "*.md" -type f | wc -l
    
    echo -e "${CYAN}Tamaño total:${NC}"
    du -sh . | awk '{print $1}'
    
    echo ""
}

################################################################################
# MAIN
################################################################################

main() {
    print_header
    
    # Obtener mensaje de commit
    if [ -z "$1" ]; then
        COMMIT_MSG="Actualización automática del proyecto de Memoria Virtual - $(date '+%Y-%m-%d %H:%M:%S')"
        print_info "Usando mensaje de commit por defecto"
    else
        COMMIT_MSG="$1"
    fi
    
    echo -e "${YELLOW}Mensaje de commit:${NC} \"$COMMIT_MSG\""
    echo ""
    
    # Ejecutar pasos
    check_directories
    sync_files
    git_operations "$COMMIT_MSG"
    show_stats
    
    # Mensaje final
    echo ""
    echo -e "${GREEN}╔════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║  ✓ Sincronización completada exitosamente                     ║${NC}"
    echo -e "${GREEN}╚════════════════════════════════════════════════════════════════╝${NC}"
    echo ""
}

# Ejecutar script
main "$@"
