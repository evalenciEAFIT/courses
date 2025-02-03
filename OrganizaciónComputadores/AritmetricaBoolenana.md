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


 --------------------------
 # Computación y Cálculo: Historia y Funcionamiento

## 1. Aritmética Booleana

La **aritmética booleana** es la base del cálculo computacional y los circuitos digitales. Fue desarrollada por **George Boole** en el siglo XIX y se fundamenta en dos estados lógicos: `0` (falso) y `1` (verdadero).

### Video Explicativo:
[![Aritmética Booleana](https://img.youtube.com/vi/mg5MmC-2DuA/0.jpg)](https://www.youtube.com/watch?v=mg5MmC-2DuA)

---

## 2. Suma, Negativo, Resta

### Suma Binaria:
La suma en binario sigue reglas similares a la aritmética decimal, pero con solo dos valores (`0` y `1`).

Ejemplo:
```
  101
+ 011
------
 1000
```

### Números Negativos y Resta:
Se usa el **Complemento a 2** para representar números negativos y realizar restas de manera eficiente.

Ejemplo de `5 - 3` en complemento a 2:
1. Representación de 5 en binario: `00000101`
2. Representación de 3 en binario: `00000011`
3. Complemento a 2 de `3`: `11111101`
4. Suma: `00000101 + 11111101 = 00000010` (resultado = `2`)

### Video Explicativo:
[![Suma y Resta en Binario](https://img.youtube.com/vi/tC3-3Xh5ZXc/0.jpg)](https://www.youtube.com/watch?v=tC3-3Xh5ZXc)

---

## 3. Calculadora por Dentro

Las **calculadoras modernas** utilizan circuitos electrónicos basados en transistores, puertas lógicas y registros para realizar operaciones matemáticas.

### Partes Claves:
- **Unidad de Control**: Maneja la entrada de datos.
- **ALU (Unidad Aritmético-Lógica)**: Ejecuta operaciones matemáticas.
- **Memoria**: Guarda valores intermedios.
- **Pantalla y Teclado**: Interfaz con el usuario.

### Video Explicativo:
[![Cómo funciona una calculadora](https://img.youtube.com/vi/HNFlPO80Zzw/0.jpg)](https://www.youtube.com/watch?v=HNFlPO80Zzw)

---

## 4. Calculadora de Canicas

La **calculadora mecánica de canicas** es un modelo físico que demuestra cómo funcionan las operaciones matemáticas con mecanismos sencillos.

### Video Demostración:
[![Calculadora de Canicas](https://img.youtube.com/vi/qK3yUdxr8lc/0.jpg)](https://www.youtube.com/watch?v=qK3yUdxr8lc)

---

## 5. Historia de la Calculadora

Desde el **ábaco** hasta las modernas calculadoras electrónicas, el desarrollo de estos dispositivos ha evolucionado enormemente.

### Hitos Importantes:
- **Siglo XVII**: Pascal inventa la Pascalina.
- **Siglo XIX**: Leibniz mejora la calculadora mecánica.
- **Siglo XX**: Aparece la calculadora electrónica.
- **1971**: Texas Instruments lanza la primera calculadora de bolsillo.

### Video Histórico:
[![Historia de la Calculadora](https://img.youtube.com/vi/VIVaJj8G2lM/0.jpg)](https://www.youtube.com/watch?v=VIVaJj8G2lM)

---

## 6. Intel 4004: Primer Microprocesador

El **Intel 4004** (1971) fue el primer microprocesador comercial de 4 bits y sentó las bases de la computación moderna.

**Características:**
- **Arquitectura de 4 bits**.
- **Frecuencia de 740 kHz**.
- **2300 transistores**.
- **Soportaba 16 instrucciones**.

### Video Explicativo:
[![Intel 4004](https://img.youtube.com/vi/ZdOcy3LUV98/0.jpg)](https://www.youtube.com/watch?v=ZdOcy3LUV98)

---

## 7. ALU (Unidad Aritmético-Lógica)

La **ALU** es el componente dentro de una CPU responsable de ejecutar operaciones matemáticas y lógicas.

**Funciones:**
- Suma, resta, multiplicación, división.
- Operaciones lógicas (AND, OR, NOT, XOR).
- Comparaciones (mayor, menor, igual).

### Video Explicativo:
[![Cómo funciona la ALU](https://img.youtube.com/vi/O0lFMZr4Q3w/0.jpg)](https://www.youtube.com/watch?v=O0lFMZr4Q3w)

---

## 8. FPU (Unidad de Punto Flotante)

La **FPU (Floating Point Unit)** se encarga de realizar operaciones matemáticas con números decimales de alta precisión.

**Ejemplo:**
- La suma `3.14 + 2.71` requiere una FPU, ya que no puede representarse fácilmente en números enteros.

### Video Explicativo:
[![Cómo funciona la FPU](https://img.youtube.com/vi/uEXxIlpVUXs/0.jpg)](https://www.youtube.com/watch?v=uEXxIlpVUXs)

---

## Conclusión

La computación y la historia de las calculadoras han evolucionado desde dispositivos mecánicos hasta poderosos procesadores modernos. Comprender estos conceptos es fundamental para explorar el funcionamiento de las computadoras actuales y futuras.



