#include "funciones_ListaCircDoble.h"

void crearListaCD(tListaCD *p)
{
    *p = NULL;
}

int listaCDVacia(const tListaCD *p)
{
    //si el puntero del main es NULL, lista vacia
    return *p ==  NULL ? LCD_VACIA : LCD_DISPONIBLE;
}

int listaCDLlena(const tListaCD *p, unsigned tamBytes)
{
    return LCD_DISPONIBLE;
}

void vaciarListaCD(tListaCD *p)
{
    ///*p apunta al primer nodo
    tNodoCD *pri, *elim, *sig;

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
    tNodoCD *AA, *AS, *nue;

    //acondicionar y cargar nodo
    nue = (tNodoCD*)malloc(sizeof(tNodoCD));
    if(nue == NULL || (nue->info = malloc(tamBytes)) == NULL)
    {
        free(nue);
        return LCD_SIN_MEM;
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

    return LCD_EXITO;
}

int ponerAlFinalCD(tListaCD *p, const void *d, unsigned tamBytes)
{
    ///*p apunta al primer nodo
    tNodoCD *AA, *AS, *nue;

    //acondiciono y cargo nodo
    nue = (tNodoCD*)malloc(sizeof(tNodoCD));
    if(nue == NULL || (nue->info = malloc(tamBytes)) == NULL)
    {
        free(nue);
        return LCD_SIN_MEM;
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

    return LCD_EXITO;
}

int sacarPrimeroListaCD(tListaCD *p, void *d, unsigned tamBytes)
{
    ///*p apunta al primer nodo
    tNodoCD *AA, *AS, *sale;

    if(*p == NULL)
        return LCD_VACIA;

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

    return LCD_EXITO;
}

int verPrimeroListaCD(const tListaCD *p, void *d, unsigned tamBytes)
{
    ///*p apunta al primer nodo
    if(*p == NULL)
        return LCD_VACIA;

    memcpy(d, (*p)->info, MIN((*p)->tamInfo, tamBytes));

    return LCD_EXITO;
}

int sacarUltimoListaCD(tListaCD *p, void *d, unsigned tamBytes)
{
    ///*p apunta al primer nodo
    tNodoCD *AA, *AS, *sale;

    if(*p == NULL)
        return LCD_VACIA;

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

    return LCD_EXITO;
}

int verUltimoListaCD(const tListaCD *p, void *d, unsigned tamBytes)
{
    ///*p apunta al primer nodo
    if(*p == NULL)
        return LCD_VACIA;

    memcpy(d, (*p)->ant->info, MIN((*p)->ant->tamInfo, tamBytes));

    return LCD_EXITO;
}

void mostrarListaCD(const tListaCD *p, tMostrar mostrar)
{
    ///*p apunta al primer nodo
    tNodoCD *pri, *act;

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

void mostrarLista2CD(const tListaCD *p, tAccion accion, void *contexto)
{
    tNodoCD *pri, *act;

    if(*p != NULL)
    {
        act = pri = *p;

        do
        {
            accion(act->info, contexto);
            act = act->sig;

        }while(act != pri);
    }
}

tNodoCD* obtenerPrimerNodoListaCD(const tListaCD *p)//cargarTerrenoInicioDesdeArch
{
    //*p apunta al primer nodo
    if(*p == NULL)
        return NULL;

    return *p;
}

tNodoCD* obtenerUltimoNodoListaCD(const tListaCD *p)//cargarTerrenosSiguientesDesdeArch
{
    //*p apunta al primer nodo, el anterior es el ultimo
    if(!*p)
        return NULL;

    return (*p)->ant;
}

int posEnListaCD(const tListaCD *p, const tNodoCD *nodoBuscado)//ingresoDireccionDeMov
{
    //cuento cuantos nodos hasta el inicio hay
    const tNodoCD *act = *p;
    int pos = 0;

    if(act == NULL)
        return -1;

    while(act != nodoBuscado)
    {
        act = act->sig;
        pos++;

        //no econtrado (vuelta)
        if(act == *p)
            return -1;
    }

    return pos;
}


///ITERADOR///

int iniciarPrimeroItCD(tIteradorCD *it, const tListaCD *p)
{
    if(!it || !*p)
        return LCD_VACIA;

    it->actual = *p;
    it->posicion = 0;

    return LCD_EXITO;
}

int iniciarNodoItCD(tIteradorCD *it, tNodoCD *nodo)
{
    if(!it || !nodo)
        return LCD_VACIA;

    it->actual = nodo;
    it->posicion = 0;

    return LCD_EXITO;
}

int verActualItCD(tIteradorCD *it, void *d, unsigned tamBytes)
{
    if(!it || !it->actual)
        return LCD_VACIA;

    memcpy(d, it->actual->info, MIN(tamBytes, it->actual->tamInfo));

    return LCD_EXITO;
}

tNodoCD* verNodoActualItCD(const tIteradorCD *it)
{
    if(!it)
        return NULL;

    return it->actual;
}

int avanzarItCD(tIteradorCD *it)
{
    if(!it || !it->actual)
        return LCD_VACIA;

    it->actual = it->actual->sig;
    it->posicion++;

    return LCD_EXITO;
}

int retrocederItCD(tIteradorCD *it)
{
    if(!it || !it->actual)
        return LCD_VACIA;

    it->actual = it->actual->ant;
    it->posicion--;

    return LCD_EXITO;
}

int modificarActualItCD(tIteradorCD *it, const void *d, unsigned tamBytes)
{
    if(!it || !it->actual || !d)
        return LCD_CLA_INEX;

    memcpy(it->actual->info,
           d,
           MIN(tamBytes, it->actual->tamInfo));

    return LCD_EXITO;
}

void reiniciarItCD(tIteradorCD *it)
{
    if(!it)
        return;

    it->actual = NULL;
    it->posicion = 0;
}




