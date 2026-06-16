#ifndef FUNCIONES_LISTACIRCDOBLE_H_INCLUDED
#define FUNCIONES_LISTACIRCDOBLE_H_INCLUDED

#include <stdlib.h> //malloc
#include <string.h> //memcpy

#define LCD_EXITO 0
#define LCD_SIN_MEM 1
#define LCD_VACIA 2
#define LCD_DISPONIBLE 3
#define LCD_CLA_INEX 4

#define MIN(X, Y) ((X) <= (Y) ? (X) : (Y))

typedef struct sNodoCD
{
    void *info;
    unsigned tamInfo;
    struct sNodoCD *sig,
                 *ant;
}tNodoCD;

typedef tNodoCD* tListaCD;

typedef struct
{
    tNodoCD *actual;
    int posicion;
} tIteradorCD; //onda topicos

typedef int (*tCmp)(const void*, const void*);

typedef void (*tMostrar)(const void*);

typedef void (*tAccion)(void *info, void *contexto);

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

void mostrarListaCD(const tListaCD *p, tMostrar mostrar);

void mostrarLista2CD(const tListaCD *p, tAccion accion, void *contexto);

tNodoCD* obtenerPrimerNodoListaCD(const tListaCD *lista);

tNodoCD* obtenerUltimoNodoListaCD(const tListaCD *lista);

int posEnListaCD(const tListaCD *lista, const tNodoCD *nodoBuscado);


///ITERADOR

int iniciarPrimeroItCD(tIteradorCD *it, const tListaCD *p);

int iniciarNodoItCD(tIteradorCD *it, tNodoCD *nodo);

int verActualItCD(tIteradorCD *it, void *d, unsigned tamBytes);

tNodoCD* verNodoActualItCD(const tIteradorCD *it);

int avanzarItCD(tIteradorCD *it);

int retrocederItCD(tIteradorCD *it);

int modificarActualItCD(tIteradorCD *it, const void *d, unsigned tamBytes);

void reiniciarItCD(tIteradorCD *it);













#endif // FUNCIONES_LISTACIRCDOBLE_H_INCLUDED
