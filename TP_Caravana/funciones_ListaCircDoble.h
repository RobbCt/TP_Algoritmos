#ifndef FUNCIONES_LISTACIRCDOBLE_H_INCLUDED
#define FUNCIONES_LISTACIRCDOBLE_H_INCLUDED

#include <stdlib.h> //malloc
#include <string.h> //memcpy

#define EXITO 0
#define SIN_MEM 1
#define LISTA_VACIA 2
#define LISTA_DISPONIBLE 3
#define CLA_INEX 4

#define MIN(X, Y) ((X) <= (Y) ? (X) : (Y))

typedef struct sNodo
{
    void *info;
    unsigned tamInfo;
    struct sNodo *sig,
                 *ant;
}tNodo;

typedef tNodo* tListaCD;

typedef int (*tCmp)(const void*, const void*);

typedef void (*tMostrar)(const void*);

///PRIMITIVAS

void crearListaCD(tListaCD *p);

int listaCDVacia(const tListaCD *p);

int listaCDLlena(const tListaCD *p, unsigned cantBytes);

void vaciarListaCD(tListaCD *p);

int ponerAlComienzoCD(tListaCD *p, const void *d, unsigned tamBytes);

int ponerAlFinalCD(tListaCD *p, const void *d, unsigned tamBytes);

int sacarPrimeroListaCD(tListaCD *p, void *d, unsigned tamBytes);

int verPrimeroListaCD(const tListaCD *p, void *d, unsigned tamBytes);

int sacarUltimoListaCD(tListaCD *p, void *d, unsigned tamBytes);

int verUltimoListaCD(const tListaCD *p, void *d, unsigned tamBytes);

int insertarOrdenadoXclaveCD(tListaCD *p, const void *d, unsigned tamBytes, tCmp cmp);

void mostrarListaCD(const tListaCD *p, tMostrar mostrar);

void ordenarListaXclaveCD(tListaCD *p, tCmp cmp);

int eliminarXclaveCD(tListaCD *p, void *d, unsigned tamBytes, tCmp cmp);

#endif // FUNCIONES_LISTACIRCDOBLE_H_INCLUDED
