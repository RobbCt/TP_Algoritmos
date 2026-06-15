#include "funciones_colaDinamica.h"

void crearCola(tCola *p)
{
    p->pri = NULL;
    p->ult = NULL;
}

int colaVacia(const tCola *p)
{
    //literal del lado izq
    return p->pri == NULL ? C_VACIA : C_DISPONIBLE;
}

int colaLlena(const tCola *p, unsigned tamInfo)
{
    //ver si se puede hacer espacio para un nuevo nodo en memoria
    tNodoC *nue = malloc(sizeof(tNodoC));
    void *info = malloc(tamInfo);

    free(nue);
    free(info);

    return (nue == NULL || info == NULL) ? C_SIN_MEM : C_DISPONIBLE;
}

int ponerEnCola(tCola *p, const void *d, unsigned tamInfo)
{
    tNodoC *nue;

    nue = malloc(sizeof(tNodoC));

                      //si nue es NULL, no puedo acceder a ningun campo
    if(nue == NULL || (nue->info = malloc(tamInfo)) == NULL)
    {
        free(nue);
        return C_SIN_MEM;
    }//si el if es false, nue y info tienen dir validas

    memcpy(nue->info, d, tamInfo);
    nue->tamInfo = tamInfo;
    nue->sig = NULL; //no le sigue nadie (aun) al nuevo nodo (sirve para termianr de recorrer)

    if(p->ult != NULL)
        p->ult->sig = nue; //si hay ultimo, a ese ultimo le decimos q le sigue nue
    else
        p->pri = nue; //si no hay ultimo, nue es el utimo

    p->ult = nue; //siempre q agregemos un nodo, ese sera tmb el ultimo

    return C_EXITO;
}

int sacarDeCola(tCola *p, void *d, unsigned tamInfo)
{
    //se saca el primero
    tNodoC *sale = p->pri;

    if(sale == NULL) //ver si hay nodos q sacar
        return C_VACIA;

    memcpy(d, sale->info, MIN(tamInfo, sale->tamInfo)); //desde sale copiamos el primero
    p->pri = sale->sig; //el siguiente al nodo copiado es primero

    free(sale->info);
    free(sale);

    if(p->pri == NULL)//if opcional
        p->ult = NULL;//si el primero es nadie (se acabo la cola) tampoco hay ultimo

    return C_EXITO;
}

int verPrimeroCola(const tCola *p, void *d, unsigned tamInfo)
{
    if(p->pri == NULL)
        return C_VACIA;

    memcpy(d, p->pri->info, MIN(tamInfo,p->pri->tamInfo));

    return C_EXITO;
}

void vaciarCola(tCola *p)
{
    while (p->pri) //mientras el primero no sea NULL...
    {
        tNodoC *elim = p->pri; //guardo la dir del primero en elim

        p->pri = elim->sig; //el primero avanze al siguiente

        free(elim->info); //libero elim
        free(elim);
    }

    p->pri = NULL; //aca el primero y el ultimo son NULL
}
