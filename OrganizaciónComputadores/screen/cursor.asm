
@24063 //@16384  //direccion de memoria de la pantalla para el inicio
D=A
@puntoXY
M=D

@16 //altura del cursor 
D=A
@n 
M=D

(PintarCursor)
    @n
    D=M 
    @OtroCursor
    D;JEQ
    @puntoXY
    A=M
    M=-1
    @32
    D=A 
    @puntoXY
    M=M+D
    @n
    M=M-1
    @PintarCursor
    0;JMP

(OtroCursor)
@20496
D=A 
@puntoXY
M=D
@16
D=A
@n 
M=D
@PintarCursor
0;JMP


(finPintarCursor)
//    @finPintarCursor
//    0;JMP