

//posiciones iniciales
@20496     //punto medio de la pantalla
D=A 
@puntoXYPantalla
M=D 

(menu)
@KBD
D=M
@keypress
D;JEQ
@menu
0;JMP
---------

(keypress)
M=D
@atenderMenu
0;JMP   

(atenderMenu)
@tecla
D=M
    
    @65   //letra A
    D=D-A 
    @pCursor
    D;JEQ
@tecla
D=M
   
    @66  //letra B
    D=D-A 
    @bCursor
    D;JEQ
@tecla
D=M
   
    @132  //right arrow
    D=D-A 
    @derCursor
    D;JEQ
@tecla
D=M
   
    @130  //left arrow
    D=D-A 
    @izqCursor
    D;JEQ
@menu
0;JMP




(pCursor)
//copiar la posicion del cursor
@puntoXYPantalla
D=M 
@puntoTMP
M=D   //tomo una copia del punto de la pantalla
@pintaCursor
0;JMP

(bCursor)
//copiar la posicion del cursor
@puntoXYPantalla
D=M 
@puntoTMP
M=D   //tomo una copia del punto de la pantalla
@borrarCursor
0;JMP

(derCursor)
@puntoXYPantalla
M=M+1
//copiar la posicion del cursor
@puntoXYPantalla
D=M 
@puntoTMP
M=D   //tomo una copia del punto de la pantalla
@pintaCursor
0;JMP

(izqCursor)
@puntoXYPantalla
M=M-1
//copiar la posicion del cursor
@puntoXYPantalla
D=M 
@puntoTMP
M=D   //tomo una copia del punto de la pantalla
@pintaCursor
0;JMP

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