#include <iostream>
#include "Comparador.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << " <archivo_entrada> <archivo_salida>\n";
        return 1;
    }

    Comparador img;
    if (!img.cargarDesdeArchivo(argv[1])) return 1;

    img.medirTiempoYMemoria();

    img.guardarEnArchivo(argv[2]);

    return 0;
}
