#ifndef TDAVECTOR_H_INCLUDED
#define TDAVECTOR_H_INCLUDED

#include "config.h"

typedef struct
{
    void* vec;
    unsigned cantElem;
    unsigned tamElem;
    unsigned tamV;
}Vector;

int crearVector(Vector* vec, unsigned tamElem, unsigned tamInicial);
void destruirVector(Vector* vec);
int realocar(Vector* vec, unsigned ntamV);
int insertarAlFinalVector(Vector* vec, const void* elem);
void* busquedaSecuencialVector(const Vector* vec, const void* elem, int cmp(const void*, const void*));
void recorrerVector(Vector* vec, void accion (void*));
void ordenarVector(Vector* vec, int cmp(const void*, const void*));

#endif // TDAVECTOR_H_INCLUDED
