# Aritmética Booleana y su Implementación con Compuertas Lógicas

Documento inspirado en el curso *nand2tetris* y recursos audiovisuales sobre el diseño de calculadoras y computadoras.

---

## Índice
1. [Introducción a la Lógica Booleana](#introducción)
2. [Compuertas Lógicas Básicas](#compuertas-básicas)
3. [Operaciones Aritméticas Booleanas](#operaciones-aritméticas)
   - [Suma Binaria](#suma-binaria)
   - [Negación y Complemento a Dos](#negación)
   - [Incremento](#incremento)
   - [Resta Binaria](#resta-binaria)
4. [Diseño de una Unidad Aritmético-Lógica (ALU)](#alu)
5. [Contexto Histórico y Ejemplos Prácticos](#contexto-histórico)
6. [Conclusión](#conclusión)

---

## 1. Introducción a la Lógica Booleana <a name="introducción"></a>
La aritmética booleana es la base de las operaciones en computadoras modernas. Utiliza **bits** (0 y 1) y operaciones lógicas para realizar cálculos. Inspirados en videos como *"El ingenioso diseño de las calculadoras"* y *"Construyo una computadora de CANICAS y madera"*, exploraremos cómo se implementan estas operaciones con compuertas lógicas.

---

## 2. Compuertas Lógicas Básicas <a name="compuertas-básicas"></a>
Todas las operaciones se construyen a partir de compuertas fundamentales:
- **NOT (Negación)**: Invierte el bit de entrada.  
  Símbolo: `▷○`  
  Tabla de verdad:  
  | A | NOT A |
  |---|---|
  | 0 | 1 |
  | 1 | 0 |

- **AND (Conjunción)**: Resultado 1 solo si ambas entradas son 1.  
  Símbolo: `▷&`  
  Tabla de verdad:  
  | A | B | A AND B |
  |---|---|---|
  | 0 | 0 | 0 |
  | 0 | 1 | 0 |
  | 1 | 0 | 0 |
  | 1 | 1 | 1 |

- **OR (Disyunción)**: Resultado 1 si al menos una entrada es 1.  
  Símbolo: `▷≥1`  
  Tabla de verdad:  
  | A | B | A OR B |
  |---|---|---|
  | 0 | 0 | 0 |
  | 0 | 1 | 1 |
  | 1 | 0 | 1 |
  | 1 | 1 | 1 |

- **XOR (OR Exclusivo)**: Resultado 1 si las entradas son diferentes.  
  Símbolo: `▷=1`  
  Tabla de verdad:  
  | A | B | A XOR B |
  |---|---|---|
  | 0 | 0 | 0 |
  | 0 | 1 | 1 |
  | 1 | 0 | 1 |
  | 1 | 1 | 0 |

---

## 3. Operaciones Aritméticas Booleanas <a name="operaciones-aritméticas"></a>

### Suma Binaria <a name="suma-binaria"></a>
La suma de dos bits se implementa con un **semisumador (Half-Adder)** y un **sumador completo (Full-Adder)**.

#### Semisumador (Half-Adder)
- **Entradas**: A, B  
- **Salidas**: Suma (S), Acarreo (C)  
- Circuito:  
