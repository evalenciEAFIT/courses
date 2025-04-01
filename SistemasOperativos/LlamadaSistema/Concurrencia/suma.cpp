#include <iostream>
#include <vector>

int main() {
    // Definir el vector de números
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Variable para almacenar la suma
    int totalSum = 0;

    // Iterar sobre el vector y sumar los elementos
    for (int num : data) {
        totalSum += num;
    }

    // Mostrar el resultado
    std::cout << "Suma total (versión básica): " << totalSum << std::endl;

    return 0;
}