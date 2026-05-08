# Comparativa Pedagógica de Algoritmos de Encriptación en C

Este proyecto en C permite comparar diferentes algoritmos de encriptación de archivos para comprender su funcionamiento, así como sus ventajas y desventajas en términos de **eficiencia**, **consumo de memoria**, **recursos**, y **nivel de seguridad**.

El proyecto soporta el **procesamiento por aportes (streaming/chunks)**, lo que permite que el consumo de memoria RAM sea constante y extremadamente bajo (4 KB) independientemente del tamaño del archivo original.

## Algoritmos Implementados

| Algoritmo | Tipo de Cifrado | Complejidad | Nivel de Seguridad | Características |
| :--- | :--- | :--- | :--- | :--- |
| **César** | Sustitución monoalfabética | Muy Baja | Muy Bajo (1/5) | Desplazamiento fijo de bytes. |
| **Vigenère** | Sustitución polialfabética | Baja | Bajo (2/5) | Palabra clave periódica. |
| **XOR** | Sustitución simple | Muy Baja | Muy Bajo (1/5) | Operación bit a bit simple. |
| **RC4** | Flujo (Stream) | Media | Medio (3.5/5) | Secuencia pseudoaleatoria fluida. |
| **TEA** | Bloque (64-bit) | Alta | Alto (4.5/5) | Red de Feistel de 32 rondas. |
| **Atbash** | Sustitución espejo | Muy Baja | Bajo (1/5) | Complemento de byte (255-P). |
| **Transposición**| Permutación | Baja | Bajo (1.5/5) | Cambia el orden en bloques de 4. |
| **Vernam (OTP)**| Flujo con Semilla | Media | Alto (4/5) | XOR con flujo LCG (Semilla). |
| **AES-128** | Bloque (128-bit) | Muy Alta | Muy Alto (5/5) | Estándar global (10 rondas). |
| **RSA** | Asimétrico | Muy Alta | Alto (Educativo) | Basado en primos (Demo 64-bit). |

## Estructura del Proyecto

```
ENCRIPTAR/
├── Makefile                # Reglas de compilación automatizadas
├── ciphers.h               # Definición de estructuras y prototipos (10 métodos)
├── ciphers.c               # Implementación de los 10 algoritmos
├── main.c                  # CLI interactivo, benchmark y gestión de archivos
├── explicacion_metodos.md  # Documento técnico detallado con matemáticas y código
└── README.md               # Documentación de uso y análisis teórico
```

## Requisitos y Compilación

Para compilar el proyecto es necesario tener `gcc` y `make` instalados.

Compilar el ejecutable:
```bash
make
```

Limpiar archivos temporales y binarios:
```bash
make clean
```

Ejecutar el programa:
```bash
make run
```

## Uso de la Herramienta

Una vez compilado, puedes ejecutar el binario interactivamente:

```bash
./encriptar
```

### Opciones del Menú Interactivo:

1. **Generar archivo de prueba**: Crea un archivo de datos aleatorios con el tamaño deseado (en KB).
2. **Ejecutar comparativa (Benchmark)**: Mide el tiempo de encriptación y desencriptación de los **10 algoritmos**, el consumo de memoria y los tamaños de salida.
3. **Cifrar un archivo**: Permite elegir un archivo local y el algoritmo deseado para generar un archivo cifrado. 
    * *Nota*: Para RSA, el archivo resultante será 8 veces más grande que el original debido a la expansión de datos de 8 bits a 64 bits.
4. **Descifrar un archivo**: Permite restaurar el archivo original a partir de uno previamente cifrado.
5. **Ver cifrado paso a paso**: Permite ingresar una frase rápida y visualizar la transformación byte a byte (César, Vigenère, XOR).

## Notas Pedagógicas
Este proyecto destaca la diferencia entre cifrados de **flujo** (XOR, RC4, Vernam) y cifrados de **bloque** (TEA, AES), así como la distinción fundamental entre cifrado **simétrico** y **asimétrico** (RSA).
