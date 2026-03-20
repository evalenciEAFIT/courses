#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"

// Genera datos de prueba para utilizar en cada estructura midiendo inserciones realistas
void generar_datos(Persona *datos, int n) {
    // 1. Fase de construcción secuencial de cada persona y datos falsos
    for (int i = 0; i < n; i++) {
        sprintf(datos[i].nombre, "Persona_%08d", i);         // Genera "Persona_00000000", ...
        datos[i].edad = 18 + (rand() % 60);                  // Edad aleatoria de 18 hasta 77 años
        sprintf(datos[i].ciudad, "Ciudad_%d", rand() % 100); // Variedad de 100 tipos de ciudades
    }
    
    // 2. Desorden general - Algoritmo (Fisher-Yates shuffle)
    // El desorden garantiza que las estructuras como el Arbol N-ario o B 
    // tengan que hacer inserciones en distintas posiciones mitigando el sesgo en el banco de pruebas.
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Persona temp = datos[i];
        datos[i] = datos[j];
        datos[j] = temp;
    }
}
