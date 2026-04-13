#!/bin/bash
# rama.sh — Sincronización automática con el repositorio educacional

SOURCE="/home/edi/PROYECTOS/EDA/COMPRIMIR"
DEST="/home/edi/PROYECTOS/courses/analisis-algoritmos/estructuras_datos/COMPRIMIR2"
REPO="/home/edi/PROYECTOS/courses"

echo "Sincronizando archivos..."
rsync -av --delete \
    --exclude='bin/' --exclude='app/' --exclude='*.o' \
    --exclude='samples/comprimidos/' --exclude='samples/descomprimidos/' \
    "$SOURCE/" "$DEST/"

echo "Actualizando repositorio local y subiendo cambios..."
cd "$REPO"
git pull origin main --rebase      # <--- AÑADIDO: Evita conflictos de 'rejected'
git add .
git commit -m "COMPRIMIR: Actualización EDA $(date '+%Y-%m-%d %H:%M')"
git push origin main
