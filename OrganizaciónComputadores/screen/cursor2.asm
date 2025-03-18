
@20000  //direccion de memoria de la pantalla para el inicio
D=A
@puntoXY
M=D

@16 //altura del cursor 
D=A
@n 
M=D

(menu)
    @20000  //reubicar la pos
    D=A 
    @puntoXY
    M=D  //cursor de inicio
    @16 
    D=A 
    @n 
    M=D //pos n en 16

    @KBD
    D=M
    // 0=48  1=49 A=65 B=66  
    @65  //boton A
    D=D-A
    @PintarCursor
    D;JEQ
    
    @KBD
    D=M
    @66  //boton B
    D=D-A
    @BorrarCursor
    D;JEQ
    @menu
    0;JMP

(PintarCursor)
    @n
    D=M 
    @menu
    D;JEQ
    @puntoXY
    A=M
    M=-1   //pinta negro
    @32
    D=A 
    @puntoXY
    M=M+D
    @n
    M=M-1
    @PintarCursor
    0;JMP

(BorrarCursor)
    @n
    D=M 
    @menu
    D;JEQ
    @puntoXY
    A=M
    M=0  //pinta blanco
    @32
    D=A 
    @puntoXY
    M=M+D
    @n
    M=M-1
    @BorrarCursor
    0;JMP


//(finPintarCursor)
//    @finPintarCursor
//    0;JMP