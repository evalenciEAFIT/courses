# Algoritmo Clásico de Compresión en C++

## 1. Introducción
La compresión de datos es una técnica fundamental en la informática para reducir el tamaño de los archivos y optimizar el almacenamiento y transmisión de información. Uno de los algoritmos clásicos de compresión es **Run-Length Encoding (RLE)**, que se basa en la eliminación de redundancias en los datos.

El **Run-Length Encoding (RLE)** es un método de compresión sin pérdida que reemplaza secuencias repetitivas de caracteres por una representación más corta indicando el carácter y el número de veces que se repite consecutivamente. Es particularmente útil en datos con patrones repetitivos, como imágenes en formato BMP y datos de sensores.

---

## 2. Pseudocódigo del Algoritmo RLE
```plaintext
1. Inicializar una cadena vacía para almacenar la salida comprimida.
2. Recorrer el texto de entrada carácter por carácter.
3. Contar cuántas veces se repite el carácter consecutivamente.
4. Almacenar el carácter y el número de repeticiones en la salida comprimida.
5. Continuar hasta el final del texto.
6. Retornar la cadena comprimida.
```

---

## 3. Ejemplo Paso a Paso
### **Ejemplo:**
Texto de entrada: **"AAABBBCCCC"**

1. **Contar las repeticiones:**
   ```
   A: 3
   B: 3
   C: 4
   ```
2. **Generar la cadena comprimida:**
   ```
   A3B3C4
   ```
3. **Resultado final:**
   ```
   Entrada:  "AAABBBCCCC"
   Salida comprimida:  "A3B3C4"
   ```
   
Este método reduce el tamaño del texto cuando hay secuencias largas de caracteres repetidos.

---

## 4. Implementación en C++
```cpp
#include <iostream>
#include <string>

// Función para comprimir usando Run-Length Encoding (RLE)
std::string comprimirRLE(const std::string& texto) {
    std::string comprimido;
    int longitud = texto.length();

    for (int i = 0; i < longitud; i++) {
        int contador = 1;
        while (i < longitud - 1 && texto[i] == texto[i + 1]) {
            contador++;
            i++;
        }
        comprimido += texto[i] + std::to_string(contador);
    }

    return comprimido;
}

int main() {
    std::string texto = "AAABBBCCCC";
    std::string comprimido = comprimirRLE(texto);

    std::cout << "Texto original: " << texto << "\n";
    std::cout << "Texto comprimido: " << comprimido << "\n";

    return 0;
}
```

---

## 5. Explicación Paso a Paso del Algoritmo
1. **Recorrer el texto:** Se itera sobre cada carácter del texto.
2. **Contar repeticiones:** Se cuenta cuántas veces aparece consecutivamente el mismo carácter.
3. **Crear la salida comprimida:** Se almacena el carácter junto con la cantidad de repeticiones.
4. **Retornar el resultado:** Se devuelve la cadena comprimida reduciendo el espacio ocupado.

---

## 6. Fuente de Consulta
Este documento se basa en los libros:
- **Pro Cryptography and Cryptanalysis with C++23**  
  [Enlace](https://link-springer-com.ezproxy.eafit.edu.co/chapter/10.1007/978-1-4842-9450-5_2)

- **Fundamentals of Cryptography**  
  [Enlace](https://link-springer-com.ezproxy.eafit.edu.co/book/10.1007/978-3-030-73492-3)

Estas fuentes ofrecen una explicación detallada sobre los métodos de compresión y encriptación de datos, junto con su implementación en sistemas computacionales.

---

## 7. Conclusión
El algoritmo **Run-Length Encoding (RLE)** es una técnica simple pero efectiva para la compresión de datos sin pérdida. Funciona mejor en datos con muchas repeticiones consecutivas y es ampliamente utilizado en imágenes y archivos de texto estructurados. Su implementación en C++ es sencilla y eficiente para optimizar almacenamiento y transmisión de datos.

