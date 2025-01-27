
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
[Cargar el similador](https://nand2tetris.github.io/web-ide/chip/)   
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
La compuerta NAND utiliza dos transistores:

![Circuito NAND](http://hyperphysics.phy-astr.gsu.edu/hbasees/Electronic/ietron/nand4.gif)   

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
[Cargar el similador](https://nand2tetris.github.io/web-ide/chip/)   
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
La construcción con transitores es similar a la NAND pero se cambia el punto se salida:   
![Compuerta NAND](http://hyperphysics.phy-astr.gsu.edu/hbasees/Electronic/ietron/and4.gif)   
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
[Cargar el similador](https://nand2tetris.github.io/web-ide/chip/)   
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
Con transistores es:   
![Compuerta Or](http://hyperphysics.phy-astr.gsu.edu/hbasees/Electronic/ietron/or4.gif)
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
[Cargar el similador](https://nand2tetris.github.io/web-ide/chip/)   
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
[Cargar el similador](https://nand2tetris.github.io/web-ide/chip/)   
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


## Retos a desarrollar
![Reto de Puertas logicas](https://drive.google.com/file/d/17Rt3z7_OvpoQNlM6xtmC67Rn3blgM4W5/view)   

```
## Recursos relacionados

1. [Diseño de compuertas lógicas con transistores - HyperPhysics](http://hyperphysics.phy-astr.gsu.edu/hbasees/Electronic/trangate.html#c1)
   - Una guía detallada sobre cómo funcionan las compuertas lógicas a nivel de transistores.

2. [Nand2Tetris: Proyecto 1 - Lógica Booleana (PDF)](https://www.nand2tetris.org/_files/ugd/44046b_f2c9e41f0b204a34ab78be0ae4953128.pdf)
   - Documento oficial del proyecto Nand2Tetris para la implementación de lógica booleana.

3. [Nand2Tetris: Curso Completo (Google Drive)](https://drive.google.com/file/d/1MY1buFHo_Wx5DPrKhCNSA2cm5ltwFJzM/view)
   - Material adicional para el curso completo de Nand2Tetris.

