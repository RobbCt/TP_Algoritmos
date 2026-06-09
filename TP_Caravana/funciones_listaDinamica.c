#include "funciones_listaDinamica.h"
#include <stdio.h>

void crearLista(tLista *p)
{
    ///el puntero del main es quien apunta al nodo

    //(*p) es el puntero del main
    //(p) es el puntero al puntero del main
    *p = NULL;
}

int listaVacia(const tLista *p)
{
    //si el puntero del main apunta a nulo, esta vacio
    return *p == NULL ? LISTA_VACIA : LISTA_DISPONIBLE;
}

int listaLlena(const tLista *p, unsigned cantBytes)
{
    return LISTA_DISPONIBLE;
}

void vaciarLista(tLista *p)
{
    while(*p)
    {
        tNodoL *elim = *p;
        *p = elim->sig;

        free(elim->info);
        free(elim);
    }
}

int ponerAlComienzo(tLista *p,const void *d, unsigned tamBytes)
{
     tNodoL *nue;

     nue = (tNodoL *)malloc(sizeof(tNodoL));

     if(nue == NULL || (nue->info = malloc(tamBytes)) == NULL)
     {
         free(nue);
         return SIN_MEM;
     } //reservo memoria para todo el nodo

    memcpy(nue->info, d, tamBytes);
    nue->tamInfo = tamBytes;
    nue->sig = *p; //el nuevo nodo tiene como siguiente el primero nodo
    *p = nue; //ahora el neuvo nodo es el primero

    return EXITO;
}

int sacarPrimeroLista(tLista *p, void *d, unsigned tamBytes)
{
    tNodoL *sale = *p;

    if(*p == NULL)
        return LISTA_VACIA;

    *p = sale->sig;
    memcpy(d, sale->info, MIN(sale->tamInfo, tamBytes));

    free(sale->info);
    free(sale);

    return EXITO;
}

int verPrimeroLista(const tLista *p, void *d, unsigned tamBytes)
{
    if(*p == NULL)
        return LISTA_VACIA;

    memcpy(d, (*p)->info, MIN((*p)->tamInfo, tamBytes));

    return EXITO;
}

int ponerAlFinal(tLista *p, const void *d, unsigned tamBytes)
{
    tNodoL *nue;

    nue = (tNodoL *)malloc(sizeof(tNodoL));
    if(nue == NULL || (nue->info = malloc(tamBytes)) == NULL)
    {
        free(nue);
        return SIN_MEM;
    }

    memcpy(nue->info, d, tamBytes);
    nue->tamInfo = tamBytes;
    nue->sig = NULL;



    while(*p) //mietras *p apunte a un nodo, itero
        p = &(*p)->sig; //[(*p)->sig] es la direccion al siguiente nodo
    //[&(*p)->sig] es la direccion del puntero q apunta al siguiente nodo
    //con cada iteracion hago que, p apunte al puntero sig de cada nodo

    *p = nue; //[*p] es el puntero del campo sig del ultimo nodo (era NULL), siguiente al ultimo = nue

    return EXITO;
}

int sacarUltimoLista(tLista *p, void *d, unsigned tamBytes)
{
    if(*p == NULL)
        return LISTA_VACIA;

    while((*p)->sig) //mientras el nodo [*p] apunte a, otro nodo con un sig
        p = &(*p)->sig;
    //cuando corta el while, [*p] es un puntero al ultimo nodo

    memcpy(d, (*p)->info, MIN(tamBytes, (*p)->tamInfo));
    free((*p)->info);
    free(*p);

    *p = NULL;

    return EXITO;
}

int verUltimoLista(const tLista *p, void *d, unsigned tamBytes)
{
    if(*p == NULL)
        return LISTA_VACIA;

    while((*p)->sig)
        p = &(*p)->sig;

    memcpy(d, (*p)->info, MIN(tamBytes, (*p)->tamInfo));

    return EXITO;
}

void* obtenerPrimeroInfo(tLista *p, tIteradorLista *it)
{
    if(*p == NULL)
        return NULL;

    it->actual = *p;
    it->posicion = 0;

    return it->actual->info;
}

void* obtenerSiguienteInfo(tIteradorLista *it)
{
    if(it->actual == NULL || it->actual->sig == NULL)
        return NULL;

    it->actual = it->actual->sig;
    it->posicion++;

    return it->actual->info;
}

tNodoL* obtenerPrimerNodo(tLista *p, tIteradorLista *it)
{
    if(*p == NULL)
        return NULL;

    it->actual = *p;
    it->posicion = 0;

    return it->actual;
}

tNodoL* obtenerSiguienteNodo(tIteradorLista *it)
{
    if(it->actual == NULL || it->actual->sig == NULL)
        return NULL;

    it->actual = it->actual->sig;
    it->posicion++;

    return it->actual;
}

int elimDirDeLista(tLista *p, const void *d)
{
    tNodoL *anterior = NULL, *aEliminar = NULL;

    //si la lista esta vacia
    if(!*p)
        return LISTA_VACIA;

    //itero hasta llegar a fin de lista o encontrar la direccion
    while(*p && (tNodoL*)d != *p)
    {
        anterior = *p;
        p = &(*p)->sig;
    }

    //si llegue a fin de lista no lo encontre
    if(!*p)
    {
        puts("\nbandido HGHFH");
        system("pause");
        return NO_ENCONTRADO;

    }

    //antes de modificar *p
    aEliminar = *p;

    //si lo encontre, lo engancho
    if(anterior)
        anterior->sig = aEliminar->sig;
    else
        *p = aEliminar->sig;

    //elimino el nodo
    free(aEliminar->info);
    free(aEliminar);

    return EXITO;
}
