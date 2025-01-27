
# Compuertas Lógicas: Diseño y Análisis

Este documento presenta una descripción completa de las principales compuertas lógicas, incluyendo su diseño con transistores, tabla de verdad, notación matemática y código HDL basado en Nand2Tetris.

---

## 1. NOT (Inversor)

### Circuito en Transistores
Un inversor se construye con un único transistor MOSFET:

![Circuito NOT](http://hyperphysics.phy-astr.gsu.edu/hbasees/Electronic/imgdig/notinv.gif)

### Tabla de Verdad
| Entrada (A) | Salida (!A) |
|-------------|-------------|
| 0           | 1           |
| 1           | 0           |

### Notación Matemática
$$
Salida = \neg A
$$

### Código en HDL
```hdl
CHIP Not {
    IN in;
    OUT out;

    PARTS:
    Nand(a=in, b=in, out=out);
}
```

---

## 2. NAND (Negación AND)

### Circuito en Transistores
La compuerta NAND utiliza cuatro transistores:

![Circuito NAND](http://hyperphysics.phy-astr.gsu.edu/hbasees/Electronic/imgdig/nand.gif)

### Tabla de Verdad
| A | B | Salida (A NAND B) |
|---|---|-------------------|
| 0 | 0 | 1                 |
| 0 | 1 | 1                 |
| 1 | 0 | 1                 |
| 1 | 1 | 0                 |

### Notación Matemática
$$
Salida = \neg (A \land B)
$$

### Código en HDL
```hdl
CHIP Nand {
    IN a, b;
    OUT out;

    PARTS:
    Nand(a=a, b=b, out=out); // Implementación directa
}
```

---

## 3. AND

### Circuito en Transistores
La compuerta AND se construye utilizando la compuerta NAND y un inversor:

### Tabla de Verdad
| A | B | Salida (A AND B) |
|---|---|------------------|
| 0 | 0 | 0                |
| 0 | 1 | 0                |
| 1 | 0 | 0                |
| 1 | 1 | 1                |

### Notación Matemática
$$
Salida = A \land B
$$

### Código en HDL
```hdl
CHIP And {
    IN a, b;
    OUT out;

    PARTS:
    Nand(a=a, b=b, out=tmp);
    Not(in=tmp, out=out);
}
```

---

## 4. OR

### Circuito en Transistores
La compuerta OR se puede implementar usando compuertas NAND:

### Tabla de Verdad
| A | B | Salida (A OR B) |
|---|---|-----------------|
| 0 | 0 | 0               |
| 0 | 1 | 1               |
| 1 | 0 | 1               |
| 1 | 1 | 1               |

### Notación Matemática
$$
Salida = A \lor B
$$

### Código en HDL
```hdl
CHIP Or {
    IN a, b;
    OUT out;

    PARTS:
    Not(in=a, out=nota);
    Not(in=b, out=notb);
    Nand(a=nota, b=notb, out=out);
}
```

---

## 5. XOR

### Circuito en Transistores
El diseño de XOR combina compuertas NAND, OR y AND.

### Tabla de Verdad
| A | B | Salida (A XOR B) |
|---|---|------------------|
| 0 | 0 | 0                |
| 0 | 1 | 1                |
| 1 | 0 | 1                |
| 1 | 1 | 0                |

### Notación Matemática
$$
Salida = (A \land \neg B) \lor (\neg A \land B)
$$

### Código en HDL
```hdl
CHIP Xor {
    IN a, b;
    OUT out;

    PARTS:
    Or(a=a, b=b, out=orab);
    And(a=a, b=notb, out=and1);
    And(a=nota, b=b, out=and2);
    Or(a=and1, b=and2, out=out);
}
```

---

Este documento proporciona una base para estudiar y entender cómo las compuertas lógicas se construyen desde transistores, cómo funcionan, y cómo implementarlas en hardware virtual. Para una explicación más detallada, se recomienda explorar el simulador de **Nand2Tetris** y las herramientas relacionadas.
