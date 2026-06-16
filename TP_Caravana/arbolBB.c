#include "arbolBB.h"


void crearArbolBinBusq(tArbolBinBusq*p)
{
    *p=NULL;
}
int insertarEnArbol(tArbolBinBusq*p, const void*d,
                         unsigned tam, int (*cmp)(const void*, const void*))
{
    tNodoArbol*nue;
    int rc;

    while(*p)
    {
        if((rc=cmp(d, (*p)->info))<0)
            p=&(*p)->izq;
        else if(rc>0)
            p=&(*p)->der;
        else
            return CLA_DUP;
    }
    if(!reservarMemNodo(nue, sizeof(tNodoArbol),nue->info, tam))
        return SIN_MEMO;


    nue->tamInfo= tam;
    memcpy(nue->info, d, tam);
    nue->der = nue->izq=NULL;
    *p=nue;

    return TODO_OK;
}

void recorrerPreOrdenRecArbolBinBusq(const tArbolBinBusq*p, unsigned n, void* params,
                                     void(*accion)(void*,unsigned, unsigned, void*))
{
    if(!*p)
        return ;
    accion((*p)->info,(*p)->tamInfo, n, params);
    recorrerPreOrdenRecArbolBinBusq(&(*p)->izq, n+1, params, accion);
    recorrerPreOrdenRecArbolBinBusq(&(*p)->der, n+1, params, accion);
}
void recorrerInOrdenRecArbolBinBusq(const tArbolBinBusq*p, unsigned n, void* params,
                                     void(*accion)(void*,unsigned, unsigned, void*))
{
    if(!*p)
        return ;
    recorrerInOrdenRecArbolBinBusq(&(*p)->izq, n+1, params, accion);
    accion((*p)->info,(*p)->tamInfo, n, params);
    recorrerInOrdenRecArbolBinBusq(&(*p)->der, n+1, params, accion);
}

//  invoca a accion solo si cmp es verdadero
void recorrerArbolFiltradoInOrden(const tArbolBinBusq *p, void *info,
                           int (*cmp)(const void *, const void *),
                           void (*accion)(const void *, void *),void *param)
{
    if (!*p)
        return;
    recorrerArbolFiltradoInOrden(&(*p)->izq, info, cmp, accion, param);

    if (cmp((*p)->info, info)==0)
        accion((*p)->info, param);

    recorrerArbolFiltradoInOrden(&(*p)->der, info, cmp, accion, param);
}
void* buscarEnArbol(const tArbolBinBusq* p, const void* claveBuscada,
                    int (*cmp)(const void*, const void*))
{
    int rc;

    while (*p)
    {
        rc = cmp(claveBuscada, (*p)->info);

        if (rc < 0)
            p = &(*p)->izq;
        else if (rc > 0)
            p = &(*p)->der;
        else
            return (*p)->info;
    }

    return NULL;
}

void vaciarArbolBinBusq(tArbolBinBusq*p)
{
    if (!*p)
        return;

    vaciarArbolBinBusq(&(*p)->izq);
    vaciarArbolBinBusq(&(*p)->der);

    free((*p)->info);
    free(*p);

    *p= NULL;
}
