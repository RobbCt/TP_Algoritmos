#ifndef FUNCIONES_LISTADINAMICA_H_INCLUDED
#define FUNCIONES_LISTADINAMICA_H_INCLUDED

#include <stdlib.h> //malloc
#include <string.h> //memcpy

#define EXITO 0
#define SIN_MEM 1
#define LISTA_VACIA 2
#define LISTA_DISPONIBLE 3
#define NO_ENCONTRADO 3

#define MIN(X, Y) ((X) <= (Y) ? (X) : (Y))

typedef struct sNodoL
{
    void *info;
    unsigned tamInfo;
    struct sNodoL *sig;
}tNodoL;

typedef tNodoL* tLista;

typedef struct
{
    tNodoL *actual;
    unsigned posicion;
} tIteradorLista; //onda topicos

///PRIMITIVAS

void crearLista(tLista *p);

int listaVacia(const tLista *p);

int listaLlena(const tLista *p, unsigned cantBytes);

void vaciarLista(tLista *p);

int ponerAlComienzo(tLista *p,const void *d, unsigned tamBytes);

int sacarPrimeroLista(tLista *p, void *d, unsigned tamBytes);

int verPrimeroLista(const tLista *p, void *d, unsigned tamBytes);

int ponerAlFinal(tLista *p, const void *d, unsigned tamBytes);

int sacarUltimoLista(tLista *p, void *d, unsigned tamBytes);

int verUltimoLista(const tLista *p, void *d, unsigned tamBytes);

void* obtenerPrimeroInfo(tLista *p, tIteradorLista *it);

void* obtenerSiguienteInfo(tIteradorLista *it);

tNodoL* obtenerPrimerNodo(tLista *p, tIteradorLista *it);

tNodoL* obtenerSiguienteNodo(tIteradorLista *it);

int elimDirDeLista(tLista *p, const void *d);

void* buscarPorPosicionLista(tLista* p, int posicion);

#endif // FUNCIONES_LISTADINAMICA_H_INCLUDED
