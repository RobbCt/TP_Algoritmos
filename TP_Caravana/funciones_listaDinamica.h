#ifndef FUNCIONES_LISTADINAMICA_H_INCLUDED
#define FUNCIONES_LISTADINAMICA_H_INCLUDED

#include <stdlib.h> //malloc
#include <string.h> //memcpy

#define L_EXITO 0
#define L_SIN_MEM 1
#define L_VACIA 2
#define L_DISPONIBLE 3
#define L_NO_ENCONTRADO 3

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

int ponerAlComienzoLista(tLista *p,const void *d, unsigned tamBytes);

int sacarPrimeroLista(tLista *p, void *d, unsigned tamBytes);

int verPrimeroLista(const tLista *p, void *d, unsigned tamBytes);

int ponerAlFinalLista(tLista *p, const void *d, unsigned tamBytes);

int sacarUltimoLista(tLista *p, void *d, unsigned tamBytes);

int verUltimoLista(const tLista *p, void *d, unsigned tamBytes);

///ITERADOR

int iniciarPrimeroItLista(tIteradorLista *it, const tLista *p);

int verActualItLista(tIteradorLista *it, void *d, unsigned tamBytes);

int avanzarItLista(tIteradorLista *it);

int modificarActualItLista(tIteradorLista *it, const void *d, unsigned tamBytes);

int eliminarActualItLista(tIteradorLista *it, tLista *p);

int esValidoItLista(const tIteradorLista *it);

void reiniciarItLista(tIteradorLista *it);

#endif // FUNCIONES_LISTADINAMICA_H_INCLUDED
