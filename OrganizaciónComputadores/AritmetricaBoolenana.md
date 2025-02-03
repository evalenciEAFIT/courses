# Aritmética Booleana

La aritmética booleana es una rama de las matemáticas que se centra en operaciones lógicas sobre valores binarios: **1** (verdadero) y **0** (falso). Es fundamental en el diseño de circuitos digitales y sistemas informáticos.

## Operaciones Básicas

1. **AND (Conjunción)**: La operación AND resulta en **1** solo si **ambos operandos** son **1**; de lo contrario, es **0**.

   | A | B | A AND B |
   |---|---|---------|
   | 0 | 0 |    0    |
   | 0 | 1 |    0    |
   | 1 | 0 |    0    |
   | 1 | 1 |    1    |

2. **OR (Disyunción)**: La operación OR resulta en **1** si **al menos uno** de los operandos es **1**; si ambos son **0**, el resultado es **0**.

   | A | B | A OR B |
   |---|---|--------|
   | 0 | 0 |   0    |
   | 0 | 1 |   1    |
   | 1 | 0 |   1    |
   | 1 | 1 |   1    |

3. **NOT (Negación)**: La operación NOT invierte el valor del operando; convierte **1** en **0** y viceversa.

   | A | NOT A |
   |---|-------|
   | 0 |   1   |
   | 1 |   0   |

Estas operaciones son la base para construir funciones lógicas más complejas y circuitos digitales.

## Aplicaciones

La aritmética booleana es esencial en el diseño de:

- **Circuitos Lógicos**: Implementación de puertas lógicas en hardware.
- **Sistemas Digitales**: Desarrollo de componentes como sumadores, multiplexores y unidades aritmético-lógicas (ALU).
- **Programación**: Control de flujo y toma de decisiones en algoritmos.

## Recursos Adicionales

Para profundizar en el funcionamiento interno de las calculadoras y la historia de la computación, se recomiendan los siguientes videos:

- **El ingenioso diseño de las calculadoras | Así funcionan por dentro**: Este video explora el funcionamiento interno de las calculadoras modernas.

  [![El ingenioso diseño de las calculadoras](https://img.youtube.com/vi/-ZwAFmaR_sA/0.jpg)](https://www.youtube.com/watch?v=-ZwAFmaR_sA)

- **Construyo una computadora de CANICAS y madera | ASÍ funciona**: Demostración de una calculadora mecánica construida con canicas y madera, basada en principios de aritmética booleana.

  [![Calculadora de canicas](https://img.youtube.com/vi/NQX6irk11qA/0.jpg)](https://www.youtube.com/watch?v=NQX6irk11qA)

- **LA HISTORIA DE LA CALCULADORA: Evolución de la Calculadora**: Un recorrido por la evolución de las calculadoras desde sus inicios hasta la era digital.

  [![Historia de la calculadora](https://img.youtube.com/vi/8Oz9eFAZ-tY/0.jpg)](https://www.youtube.com/watch?v=8Oz9eFAZ-tY)

- **Intel 4004, el primer chip**: Análisis del primer microprocesador comercial, el Intel 4004, y su impacto en la computación.

  [![Intel 4004](https://img.youtube.com/vi/EGE_VUmhAQg/0.jpg)](https://www.youtube.com/watch?v=EGE_VUmhAQg)

Estos recursos ofrecen una comprensión más profunda de la aritmética booleana y su aplicación en la tecnología actual.


# Aritmética Booleana

La aritmética booleana es una rama de las matemáticas que se centra en operaciones lógicas sobre valores binarios: **1** (verdadero) y **0** (falso). Es fundamental en el diseño de circuitos digitales y sistemas informáticos.

## Operaciones Básicas

1. **AND (Conjunción)**: La operación AND resulta en **1** solo si **ambos operandos** son **1**; de lo contrario, es **0**.

   | A | B | A AND B |
   |---|---|---------|
   | 0 | 0 |    0    |
   | 0 | 1 |    0    |
   | 1 | 0 |    0    |
   | 1 | 1 |    1    |

2. **OR (Disyunción)**: La operación OR resulta en **1** si **al menos uno** de los operandos es **1**; si ambos son **0**, el resultado es **0**.

   | A | B | A OR B |
   |---|---|--------|
   | 0 | 0 |   0    |
   | 0 | 1 |   1    |
   | 1 | 0 |   1    |
   | 1 | 1 |   1    |

3. **NOT (Negación)**: La operación NOT invierte el valor del operando; convierte **1** en **0** y viceversa.

   | A | NOT A |
   |---|-------|
   | 0 |   1   |
   | 1 |   0   |

Estas operaciones son la base para construir funciones lógicas más complejas y circuitos digitales.

## Suma en Aritmética Booleana

En aritmética booleana, la **suma** se realiza utilizando la operación lógica OR. Sin embargo, al trabajar con números binarios en sistemas digitales, la suma se lleva a cabo mediante circuitos denominados **sumadores**.

Un **medio sumador** es un circuito que suma dos bits y produce una **suma** y un **acarreo** como resultados. Para sumar números de más de un bit, se emplean **sumadores completos**, que consideran el acarreo de la posición anterior.

La tabla de verdad para un medio sumador es la siguiente:

   | A | B | Suma | Acarreo |
   |---|---|------|---------|
   | 0 | 0 |  0   |    0    |
   | 0 | 1 |  1   |    0    |
   | 1 | 0 |  1   |    0    |
   | 1 | 1 |  0   |    1    |

Donde:

- **Suma** = A XOR B
- **Acarreo** = A AND B

Para sumar números binarios de múltiples bits, se conectan varios sumadores completos en cascada, donde el acarreo de una etapa se convierte en una entrada para la siguiente.

## Resta y Números Negativos en Aritmética Booleana

La **resta** en sistemas digitales se realiza comúnmente utilizando la técnica del **complemento a dos**. Esta técnica permite restar un número sumando su complemento, simplificando el diseño del hardware.

**Pasos para obtener el complemento a dos de un número binario:**

1. **Invertir** todos los bits del número (obtener el complemento a uno).
2. **Sumar** 1 al resultado obtenido.

**Ejemplo:**

Para obtener el complemento a dos del número binario `0101` (que representa el decimal 5):

1. **Invertir los bits**: `0101` → `1010`
2. **Sumar 1**: `1010` + `1` = `1011`

El resultado `1011` representa el valor -5 en complemento a dos.

Al utilizar el complemento a dos, la resta `A - B` se convierte en la suma de `A` y el complemento a dos de `B`:

`A - B` ≡ `A + (complemento a dos de B)`

Esto simplifica las operaciones aritméticas en sistemas digitales, ya que la misma unidad de hardware puede manejar tanto sumas como restas.

## Unidad Aritmético-Lógica (ALU)

La **Unidad Aritmético-Lógica (ALU)** es un componente fundamental en los procesadores. Se encarga de realizar operaciones aritméticas (como suma y resta) y lógicas (como AND, OR, NOT) sobre los datos.

Las funciones principales de una ALU incluyen:

- **Operaciones aritméticas**: suma, resta, multiplicación y, en algunos casos, división.
- **Operaciones lógicas**: AND, OR, NOT, XOR.
- **Desplazamientos de bits**: operaciones que mueven bits a la izquierda o derecha, utilizadas en multiplicaciones o divisiones rápidas por potencias de dos.

La ALU recibe instrucciones de la **Unidad de Control** del procesador, que le indica qué operación realizar y sobre qué datos operar.

## Unidad de Punto Flotante (FPU)

La **Unidad de Punto Flotante (FPU)** es un componente especializado del procesador diseñado para manejar operaciones aritméticas con números en formato de punto flotante, que son números con
::contentReference[oaicite:1]{index=1}
 

