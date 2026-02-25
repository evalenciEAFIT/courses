#!/bin/bash

# Script de Monitoreo para procesos, hilos y MEMORIA
# Actualiza cada 0.5 segundos

mostrar_ayuda() {
    echo "Uso: ./monitorear.sh [ -p | -t ] <PID_PRINCIPAL>"
    echo " -p : Monitorea procesos e Hijos (PROCESOS)"
    echo " -t : Monitorea hilos asociados a un programa (THREADS)"
    echo ""
    echo "Ejemplo: ./monitorear.sh -p 12345"
    exit 1
}

if [ $# -ne 2 ]; then
    mostrar_ayuda
fi

TIPO=$1
PID=$2

# Verificamos si el proceso existe
if ! kill -0 "$PID" 2>/dev/null; then
    echo "Error: el proceso con PID $PID no se encuentra en ejecución."
    exit 1
fi

echo "===================================================================================="
echo " ESTADOS IMPORTANTES (STAT):"
echo "  R = Instrucción C (Running - Usando activamente la CPU)"
echo "  S = Instrucción A (Sleep - Bloqueado esperando I/O)"
echo " "
echo " MEMORIA:"
echo "  %MEM = Porcentaje de RAM Física Usada."
echo "  VSZ  = Virtual Size en KB (Memoria lógica solicitada por el programa)."
echo "  RSS  = Resident Set Size en KB (Memoria real materializada en la RAM)."
echo "===================================================================================="
sleep 3

if [ "$TIPO" == "-p" ]; then
    # Monitoreo de procesos (+ info de memoria)
    watch -n 0.5 -t "echo '>>> PID PADRE: $PID <<<'; ps --headers -p $PID -o pid,ppid,psr,stat,%mem,vsz,rss,comm; echo ''; echo '>>> PROCESOS HIJOS <<<'; ps --headers --ppid $PID -o pid,ppid,psr,stat,%mem,vsz,rss,comm"
elif [ "$TIPO" == "-t" ]; then
    # Monitoreo de hilos (+ info de memoria)
    watch -n 0.5 -t "echo '>>> HILOS (TID/LWP) del PROCESO $PID <<<'; ps --headers -T -p $PID -o pid,tid,psr,stat,%mem,vsz,rss,comm"
else
    mostrar_ayuda
fi
