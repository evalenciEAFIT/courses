#ifndef DATA_H
#define DATA_H

#define MAX_CHILDREN 10

typedef struct {
    char nombre[50];
    int edad;
    char ciudad[50];
} Persona;

void generar_datos(Persona *datos, int n);

#endif
