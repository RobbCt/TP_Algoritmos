#include "funciones_ListaCircDoble.h"

void crearListaCD(tListaCD *p)
{
    *p = NULL;
}

int listaCDVacia(const tListaCD *p)
{
    //si el puntero del main es NULL, lista vacia
    return *p ==  NULL ? LISTA_VACIA : LISTA_DISPONIBLE;
}

int listaCDLlena(const tListaCD *p, unsigned tamBytes)
{
    return LISTA_DISPONIBLE;
}

void vaciarListaCD(tListaCD *p)
{
    ///*p apunta al primer nodo
    tNodo *pri, *elim, *sig;

    if(*p != NULL)
    {
        //lista disponible, pri marca la vuelta
        elim = pri = *p;

        do{
            //free elim, sig pivote para elim
            sig = elim->sig;

            free(elim->info);
            free(elim);

            elim = sig;
        }while(elim != pri);

        //luego de vaciar, la lista deja de existir
        *p = NULL;
    }
}

int ponerAlComienzoCD(tListaCD *p, const void *d, unsigned tamBytes)
{
    ///*p apunta al primer nodo
    tNodo *AA, *AS, *nue;

    //acondicionar y cargar nodo
    nue = (tNodo*)malloc(sizeof(tNodo));
    if(nue == NULL || (nue->info = malloc(tamBytes)) == NULL)
    {
        free(nue);
        return SIN_MEM;
    }
    memcpy(nue->info, d, tamBytes);
    nue->tamInfo = tamBytes;

    //acomodo AA y AS
    if(*p == NULL)
    {
        //nodo unico
        AA = nue;
        AS = nue;
    }
    else
    {
        //insertar antes del primer nodo
        AA = (*p)->ant;
        AS = *p;
    }

    //engancho el nodo a la lista
    nue->ant = AA;
    nue->sig = AS;

    //engancho la lista al nodo
    AA->sig = nue;
    AS->ant = nue;

    //respetando q *p apunta al primer nodo (este va al principio)
    *p = nue;

    return EXITO;
}

int ponerAlFinalCD(tListaCD *p, const void *d, unsigned tamBytes)
{
    ///*p apunta al primer nodo
    tNodo *AA, *AS, *nue;

    //acondiciono y cargo nodo
    nue = (tNodo*)malloc(sizeof(tNodo));
    if(nue == NULL || (nue->info = malloc(tamBytes)) == NULL)
    {
        free(nue);
        return SIN_MEM;
    }
    memcpy(nue->info, d, tamBytes);
    nue->tamInfo = tamBytes;

    //acomodo AA y AS
    if(*p == NULL)
    {
        //nodo unico
        AA = nue;
        AS = nue;
    }
    else
    {
        //insertar despues del ultimo nodo (anterior al primero = ultimo)
        AA = (*p)->ant;
        AS = *p;
    }

    //enganchar nodo a la lista
    nue->ant = AA;
    nue->sig = AS;

    //enganchar lista al nodo
    AA->sig = nue;
    AS->ant = nue;

    //primer nodo?
    if(*p == NULL)
        *p = nue;

    return EXITO;
}

int sacarPrimeroListaCD(tListaCD *p, void *d, unsigned tamBytes)
{
    ///*p apunta al primer nodo
    tNodo *AA, *AS, *sale;

    if(*p == NULL)
        return LISTA_VACIA;

    //sale prime nodo
    sale = *p;
    memcpy(d, sale->info, MIN(sale->tamInfo, tamBytes));

    //acomodar AA y AS
    if(sale->sig == sale)
    {
        //nodo unico
        *p = NULL;
    }
    else
    {
        //agarro extremos de 'sale'
        AA = sale->ant;
        AS = sale->sig;

        //los engancho
        AA->sig = AS;
        AS->ant = AA;

        //*p apunta al nuevo primero
        *p = AS;
    }

    //free sale
    free(sale->info);
    free(sale);

    return EXITO;
}

int verPrimeroListaCD(const tListaCD *p, void *d, unsigned tamBytes)
{
    ///*p apunta al primer nodo
    if(*p == NULL)
        return LISTA_VACIA;

    memcpy(d, (*p)->info, MIN((*p)->tamInfo, tamBytes));

    return EXITO;
}

int sacarUltimoListaCD(tListaCD *p, void *d, unsigned tamBytes)
{
    ///*p apunta al primer nodo
    tNodo *AA, *AS, *sale;

    if(*p == NULL)
        return LISTA_VACIA;

    //sale el ultimo nodo
    sale = (*p)->ant;

    memcpy(d, sale->info, MIN(sale->tamInfo, tamBytes));

    if(sale->sig == sale)
    {
        //nodo unico
        *p = NULL;
    }
    else
    {
        //agarro extremos de 'sale'
        AA = sale->ant;
        AS = *p;

        //los engancho
        AA->sig = AS;
        AS->ant = AA;
    }

    free(sale->info);
    free(sale);

    return EXITO;
}

int verUltimoListaCD(const tListaCD *p, void *d, unsigned tamBytes)
{
    ///*p apunta al primer nodo
    if(*p == NULL)
        return LISTA_VACIA;

    memcpy(d, (*p)->ant->info, MIN((*p)->ant->tamInfo, tamBytes));

    return EXITO;
}

int insertarOrdenadoXclaveCD(tListaCD *p, const void *d, unsigned tamBytes, tCmp cmp)
{
    tNodo *nue, *AA, *AS, *act = *p;

    // acondicionar y cargar nodo
    nue = (tNodo*)malloc(sizeof(tNodo));
    if(nue == NULL || (nue->info = malloc(tamBytes)) == NULL)
    {
        free(nue);
        return SIN_MEM;
    }
    memcpy(nue->info, d, tamBytes);
    nue->tamInfo = tamBytes;

    // acomodar AA y AS
    if(*p == NULL)
    {
        AA = nue;
        AS = nue;
    }
    else
    {
        //lista disponible, buscar donde insertar
        //buscar hacia adelante mientras sea mayor
        while(act->sig != *p && cmp(act->info, d) < 0)
            act = act->sig;

        //buscar hacia atrás mientras sea mayor
//        while(act->ant != *p && cmp(act->info, d) > 0)
//            act = act->ant;

        //acomodar punteros AA y AS
        if(cmp(act->info, d) < 0)
        {
            AA = act;
            AS = act->sig;
        }
        else
        {
            AA = act->ant;
            AS = act;
        }
    }

    //enganchar nodo a la lista
    nue->ant = AA;
    nue->sig = AS;

    //enganchar lista al nodo
    AA->sig = nue;
    AS->ant = nue;

    //nue es unico/primero?
    if(*p == NULL || cmp(d, (*p)->info) < 0)
        *p = nue;

    return EXITO;
}

void mostrarListaCD(const tListaCD *p, tMostrar mostrar)
{
    ///*p apunta al primer nodo
    tNodo *pri, *act;

    if(*p != NULL)
    {
        //pri marca para la vuelta
        act = pri = *p;

        do
        {
            mostrar(act->info);
            act = act->sig;

        }while(act != pri);
    }
}

void ordenarListaXclaveCD(tListaCD *p, tCmp cmp)
{
    ///*p apunta al primer nodo
    tNodo *act;
    int band = 1;

    //lista vacia/nodo unico
    if(*p == NULL || (*p)->sig == *p)
        return;

    //burbujeo optimizado, mientras haya intercambios...
    while(band)
    {
        band = 0;

        //comenzamos desde el primer nodo
        act = *p;

        do
        {
            //si act > sig = intercambio
            if(cmp(act->info, act->sig->info) > 0)
            {
                void *info = act->info;
                unsigned tamInfo = act->tamInfo;

                //intercambiar info
                act->info = act->sig->info;
                act->sig->info = info;

                //intercambio tamInfo
                act->tamInfo = act->sig->tamInfo;
                act->sig->tamInfo = tamInfo;

                band = 1;
            }

            //sig nodo
            act = act->sig;

        //iteramos hasta dar la vuelta
        }while(act->sig != *p);
    }

    //reacomodar *p al menor (primero)
    while(cmp((*p)->info, (*p)->ant->info) < 0)
        *p = (*p)->ant;
}

int eliminarXclaveCD(tListaCD *p, void *d, unsigned tamBytes, tCmp cmp)
{
    ///*p apunta al primer nodo
    tNodo *act = *p, *AA, *AS;
    int comp;

    if(*p == NULL)
        return LISTA_VACIA;

    //buscar hacia adelante mientras sea menor
    while(act->sig != *p && cmp(act->info, d) < 0)
        act = act->sig;

    //buscar hacia atras mientras sea mayor
    while(act->ant != (*p)->ant && cmp(act->info, d) > 0)
        act = act->ant;

    comp = cmp(act->info, d);

    //clave inexistente
    if(comp != 0)
        return CLA_INEX;

    memcpy(d, act->info, MIN(act->tamInfo, tamBytes));


    if(act->sig == act)
    {
        //nodo unico
        *p = NULL;
    }
    else
    {
        //agarro extremos de 'act'
        AA = act->ant;
        AS = act->sig;

        //los engancho
        AA->sig = AS;
        AS->ant = AA;

        //se elimina el primero?
        if(act == *p)
            *p = AS;
    }

    free(act->info);
    free(act);

    return EXITO;
}


