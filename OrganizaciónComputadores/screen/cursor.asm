

//posiciones iniciales
@20496     //punto medio de la pantalla
D=A 
@puntoXYPantalla
M=D 

(menu)
@KBD
D=M
@65   //letra A
D=D-A 
@pCursor
D;JEQ
@KBD
D=M
@66  //letra B
D=D-A 
@bCursor
D;JEQ
@menu
0;JMP


(pCursor)
//copiar la posicion del cursor
@puntoXYPantalla
D=M 
@puntoTMP
M=D   //tomo una copia del punto de la pantalla
(pintaCursor)
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
D=M 
A=D
M=-1  //pita negro
@32   //salta de linea
D=A 
@puntoTMP
M=D+M
@alturaActual
M=M-1   //reduce la altura
@cicloPintarCursor
0;JMP

(bCursor)
//copiar la posicion del cursor
@puntoXYPantalla
D=M 
@puntoTMP
M=D   //tomo una copia del punto de la pantalla

(borrarCursor)
@alturaActual
D=M 
@finCicloPintarCursor
D;JEQ
@puntoTMP
D=M 
A=D
M=0  //pita blanco
@32   //salta de linea
D=A 
@puntoTMP
M=D+M
@alturaActual
M=M-1   //reduce la altura
@borrarCursor
0;JMP

(finCicloPintarCursor)
@16
D=A 
@alturaActual
M=D 
@menu
0;JMP