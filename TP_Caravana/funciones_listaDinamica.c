#include "funciones_listaDinamica.h"
#include <stdio.h>

void crearLista(tLista *p)
{
    ///el puntero del main es quien apunta al nodo
    //*p es el puntero del main, p es el puntero al puntero del main
    *p = NULL;
}

int listaVacia(const tLista *p)
{
    //si el puntero del main apunta a nulo, esta vacio
    return *p == NULL ? L_VACIA : L_DISPONIBLE;
}

int listaLlena(const tLista *p, unsigned cantBytes)
{
    return L_DISPONIBLE;
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

int ponerAlComienzoLista(tLista *p,const void *d, unsigned tamBytes)
{
     tNodoL *nue;

     nue = (tNodoL *)malloc(sizeof(tNodoL));

     if(nue == NULL || (nue->info = malloc(tamBytes)) == NULL)
     {
         free(nue);
         return L_SIN_MEM;
     } //reservo memoria para todo el nodo

    memcpy(nue->info, d, tamBytes);
    nue->tamInfo = tamBytes;
    nue->sig = *p; //el nuevo nodo tiene como siguiente el primero nodo
    *p = nue; //ahora el neuvo nodo es el primero

    return L_EXITO;
}

int sacarPrimeroLista(tLista *p, void *d, unsigned tamBytes)
{
    tNodoL *sale = *p;

    if(*p == NULL)
        return L_VACIA;

    *p = sale->sig;
    memcpy(d, sale->info, MIN(sale->tamInfo, tamBytes));

    free(sale->info);
    free(sale);

    return L_EXITO;
}

int verPrimeroLista(const tLista *p, void *d, unsigned tamBytes)
{
    if(*p == NULL)
        return L_VACIA;

    memcpy(d, (*p)->info, MIN((*p)->tamInfo, tamBytes));

    return L_EXITO;
}

int ponerAlFinalLista(tLista *p, const void *d, unsigned tamBytes)
{
    tNodoL *nue;

    nue = (tNodoL *)malloc(sizeof(tNodoL));
    if(nue == NULL || (nue->info = malloc(tamBytes)) == NULL)
    {
        free(nue);
        return L_SIN_MEM;
    }

    memcpy(nue->info, d, tamBytes);
    nue->tamInfo = tamBytes;
    nue->sig = NULL;



    while(*p) //mietras *p apunte a un nodo, itero
        p = &(*p)->sig; //[(*p)->sig] es la direccion al siguiente nodo
    //[&(*p)->sig] es la direccion del puntero q apunta al siguiente nodo
    //con cada iteracion hago que, p apunte al puntero sig de cada nodo

    *p = nue; //[*p] es el puntero del campo sig del ultimo nodo (era NULL), siguiente al ultimo = nue

    return L_EXITO;
}

int sacarUltimoLista(tLista *p, void *d, unsigned tamBytes)
{
    if(*p == NULL)
        return L_VACIA;

    while((*p)->sig) //mientras el nodo [*p] apunte a, otro nodo con un sig
        p = &(*p)->sig;
    //cuando corta el while, [*p] es un puntero al ultimo nodo

    memcpy(d, (*p)->info, MIN(tamBytes, (*p)->tamInfo));
    free((*p)->info);
    free(*p);

    *p = NULL;

    return L_EXITO;
}

int verUltimoLista(const tLista *p, void *d, unsigned tamBytes)
{
    if(*p == NULL)
        return L_VACIA;

    while((*p)->sig)
        p = &(*p)->sig;

    memcpy(d, (*p)->info, MIN(tamBytes, (*p)->tamInfo));

    return L_EXITO;
}

int verInfoNodoLista(const tNodoL *nodo, void *d, unsigned tamBytes)//rm
{
    //con la dir del nodo, copio la info
    if(!nodo || !nodo->info)
        return L_VACIA;

    memcpy(d, nodo->info, MIN(tamBytes, nodo->tamInfo));

    return L_EXITO;
}

int modificarInfoNodoLista(tNodoL *nodo, const void *d, unsigned tamBytes)//rm
{
    if(!nodo)
        return L_VACIA;

    memcpy(nodo->info, d, MIN(tamBytes, nodo->tamInfo));

    return L_EXITO;
}

///ITERADOR///

int iniciarPrimeroItLista(tIteradorLista *it, const tLista *p)
{
    if(!it || !*p)
        return L_VACIA;

    it->actual = *p;
    it->posicion = 0;

    return L_EXITO;
}

int verActualItLista(tIteradorLista *it, void *d, unsigned tamBytes)
{
    if(!it || !it->actual)
        return L_NO_ENCONTRADO;

    memcpy(d, it->actual->info, MIN(tamBytes, it->actual->tamInfo));

    return L_EXITO;
}

int avanzarItLista(tIteradorLista *it)
{
    if(!it || !it->actual)
        return L_VACIA;

    it->actual = it->actual->sig;
    it->posicion++;

    return L_EXITO;
}

int modificarActualItLista(tIteradorLista *it, const void *d, unsigned tamBytes)
{
    if(!it || !it->actual || !d)
        return L_NO_ENCONTRADO;

    memcpy(it->actual->info, d, MIN(tamBytes, it->actual->tamInfo));

    return L_EXITO;
}

int eliminarActualItLista(tIteradorLista *it, tLista *p)
{
    tNodoL *ant = NULL, *act = *p, *aEliminar = it->actual;

    if(!*p || !it || !it->actual)
        return L_NO_ENCONTRADO;

    while(act && act != aEliminar)
    {
        ant = act;
        act = act->sig;
    }

    if(!act)
        return L_NO_ENCONTRADO;

    if(!ant)
        *p = act->sig;
    else
        ant->sig = act->sig;

    it->actual = act->sig;

    free(act->info);
    free(act);

    return L_EXITO;
}


void* buscarPorPosicionLista(tLista* p, int posicion)
{
    int i= 1;
    while (*p && i < posicion)
    {
        p = &(*p)->sig;
        i++;
    }
    return *p? (*p)->info: NULL;
}

int esValidoItLista(const tIteradorLista *it)
{
    return it && it->actual != NULL;
}

void reiniciarItLista(tIteradorLista *it)
{
    if(!it)
        return;

    it->actual = NULL;
    it->posicion = 0;
}
