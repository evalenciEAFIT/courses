
´´´ASM
//posiciones iniciales
@20496     //punto medio de la pantalla
D=M
@puntoXYPantalla
M=D 


(pintaCursor)
//copiar la posicion del cursor
@puntoXYPantalla
D=M 
@puntoTMP
M=D   //tomo una copia del punto de la pantalla
//--------------------------------------
@16   //altura del cursor
D=A
@alturaActual
M=D 
//--------------------------------------
(cicloPintarCursor)
@alturaActual
D=M 
@finCicloPintarCursor
D;JEQ
@puntoTMP
M=-1  //pita negro
@32   //salta de linea
D=A 
@puntoTMP
M=M+D
@alturaActual
M=M-1   //reduce la altura
@cicloPintarCursor
0;JMP

(borrarCursor)
@alturaActual
D=M 
@finCicloPintarCursor
D;JEQ
@puntoTMP
M=0  //pita blanco
@32   //salta de linea
D=A 
@puntoTMP
M=M+D
@alturaActual
M=M-1   //reduce la altura
@cicloPintarCursor
0;JMP

(finCicloPintarCursor)
@16
D=A 
@alturaActual
M=D 
´´´
