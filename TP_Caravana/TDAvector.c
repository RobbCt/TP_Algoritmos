#include "TDAvector.h"

int crearVector(Vector* vec, unsigned tamElem, unsigned tamInicial)
{
    vec->vec=malloc(tamElem*tamInicial);
    if(!vec->vec)
        return SIN_MEMO;
    vec->tamElem=tamElem;
    vec->cantElem=0;
    vec->tamV=tamInicial;
    return TODO_OK;
}
void destruirVector(Vector* vec)
{
    free(vec->vec);
}
int realocar(Vector* vec, unsigned ntamV) ///REZISE
{
    void*nvec=realloc(vec->vec,ntamV* vec->tamElem);
    if(!nvec)
        return SIN_MEMO;
    vec->vec=nvec;
    vec->tamV=ntamV;
    return TODO_OK;
}
int insertarAlFinalVector(Vector* vec, const void* elem)
{
    void* ult;
    if(vec->cantElem==vec->tamV)
    {
        if(realocar(vec, 2*vec->tamV)!=TODO_OK)
            return SIN_MEMO;
    }
    ult= vec->vec+vec->cantElem*vec->tamElem;
    memcpy(ult, elem, 1*vec->tamElem);
    vec->cantElem++;
    return TODO_OK;
}

void* busquedaSecuencialVector(const Vector* vec, const void* elem, int cmp(const void*, const void*))
{
    void* pvec;
    void* ult;
    if(vec->cantElem == 0)
        return NULL;

    pvec=vec->vec;
    ult=pvec+(vec->cantElem-1)*vec->tamElem;
    while(pvec<=ult)
    {
        if(cmp(elem, pvec)==0)
            return pvec;
        pvec+=vec->tamElem;
    }
    return NULL;
}
void recorrerVector(Vector* vec, void accion (void*))
{
    int i=0;
    void* pvec=vec->vec;
    for(i=0; i<vec->cantElem; i++)
    {
        accion(pvec);
        pvec+=vec->tamElem;
    }
}
void ordenarVector(Vector* vec, int cmp(const void*, const void*))
{
    qsort(vec->vec, vec->cantElem, vec->tamElem, cmp);
}

