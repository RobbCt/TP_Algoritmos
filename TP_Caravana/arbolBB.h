#ifndef ARBOLBB_H_INCLUDED
#define ARBOLBB_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "config.h"

#define CLA_DUP 0
#define SIN_INICIALIZAR 0
#define ERROR_ARCH 0
#define NO_EXISTE -1
#define ERROR_ARBOL 0

#define reservarMemNodo(X,Y,Z,W) (                                          \
                                  ((X) = (typeof(X))malloc(Y))==NULL ||     \
                                  ((Z)= malloc(W))==NULL ?                  \
                                  free(X),0:1 )
typedef struct sNodoArbol
{
    void*info;
    unsigned tamInfo;
    struct sNodoArbol *izq, *der;
}tNodoArbol;

typedef tNodoArbol* tArbolBinBusq;


void crearArbolBinBusq(tArbolBinBusq*p);
void vaciarArbolBinBusq(tArbolBinBusq* p);
int insertarEnArbol(tArbolBinBusq*p, const void*d,
                         unsigned tam, int (*cmp)(const void*, const void*));

int buscarElemArbolBinBusq(const tArbolBinBusq*p, void *d,
                           unsigned tam, int (*cmp)(const void*, const void*));
void* buscarEnArbol(const tArbolBinBusq* p, const void* claveBuscada,
                    int (*cmp)(const void*, const void*));

void recorrerPreOrdenRecArbolBinBusq(const tArbolBinBusq*p, unsigned n, void* params,
                                     void(*accion)(void*,unsigned, unsigned, void*));
#endif // ARBOLBB_H_INCLUDED
