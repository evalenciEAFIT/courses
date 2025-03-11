// Programa: Pantalla negra/blanca en Hack ASM de Nand2Tetris
// Descripcion: 
//   - Si se presiona una tecla → pantalla negra
//   - Si no hay tecla presionada → pantalla blanca

// =========================
// Inicializacion
// =========================
@SCREEN
D=A
@posicionPantalla
M=D              // Inicializar el cursor en el inicio de la pantalla

@8192            // 256 filas * 32 palabras de 16 bits
D=A
@tamanoPantalla
M=D              // Guardar el tamaño total de la pantalla

// =========================
// Bucle Principal
// =========================
(BUCLE)
    @KBD
    D=M
    @LIMPIAR_PANTALLA   // Si no hay tecla presionada, ir a LIMPIAR_PANTALLA
    D;JEQ
    
    @PINTAR_NEGRO       // Si hay una tecla presionada, ir a PINTAR_NEGRO
    0;JMP

// =========================
// Pintar Pantalla Blanca
// =========================
(LIMPIAR_PANTALLA)
    @colorActual
    M=0                // Color blanco (todos los bits en 0)
    @PINTAR
    0;JMP

// =========================
// Pintar Pantalla Negra
// =========================
(PINTAR_NEGRO)
    @colorActual
    M=-1               // Color negro (todos los bits en 1)
    @PINTAR
    0;JMP

// =========================
// Escribir en Pantalla
// =========================
(PINTAR)
    @tamanoPantalla
    D=M
    @contador
    M=D                // Inicializar contador

(PINTAR_LOOP)
    @posicionPantalla
    D=M
    @24575              // Limite superior de la pantalla
    D=D-A
    @REINICIAR_CURSOR
    D;JGT               // Si supera el limite, reiniciar cursor
    
    @colorActual
    D=M
    @posicionPantalla
    A=M
    M=D                // Escribir color en la posicion actual de la pantalla
    
    @posicionPantalla
    M=M+1              // Moverse a la siguiente posicion de la pantalla
    
    @contador
    M=M-1              // Decrementar el contador
    D=M
    @PINTAR_LOOP
    D;JGT              // Si quedan posiciones, continuar

    @BUCLE
    0;JMP

// =========================
// Reiniciar Cursor
// =========================
(REINICIAR_CURSOR)
    @SCREEN
    D=A
    @posicionPantalla
    M=D                // Restablecer el cursor al inicio de la pantalla
    @BUCLE
    0;JMP
