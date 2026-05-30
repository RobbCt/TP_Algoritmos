#ifndef FUNCIONES_COLADINAMICA_H_INCLUDED
#define FUNCIONES_COLADINAMICA_H_INCLUDED

#include <stdlib.h>
#include <string.h>

typedef struct sNodoC
{
    void *info;
    unsigned tamInfo;
    struct sNodoC *sig;
}tNodoC;

typedef struct
{
    tNodoC *pri;
    tNodoC *ult;
}tCola;

void crearCola(tCola *p);
int colaVacia(const tCola *p);
int colaLlena(const tCola *p, unsigned tamInfo);
int ponerEnCola(tCola *p, const void *d, unsigned tamInfo);
int sacarDeCola(tCola *p, void *d, unsigned tamInfo);
int verPrimeroCola(const tCola *p, void *d, unsigned tamInfo);
void vaciarCola(tCola *p);


#endif // FUNCIONES_COLADINAMICA_H_INCLUDED
