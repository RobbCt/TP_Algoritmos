#include "ranking.h"

int verRanking()
{
    FILE* archJugadores, *archPartidas;
    unsigned cantJugadores;
    Vector vRanking;
    tPartida partida;
    tRanking ranking;

    ///ver cantidad de jugadores
    if(abrirArchivo(&archJugadores,JUGADORES,"rb")!=TODO_OK)
        return ERROR_ARCHIVO;
    fseek(archJugadores,0,SEEK_END);
    cantJugadores= ftell(archJugadores)/sizeof(tJugadorIndice);
    fclose(archJugadores);

    if(cantJugadores==0)
        return SIN_JUGADOR;

    if(abrirArchivo(&archPartidas,PARTIDAS,"rb")!=TODO_OK)
        return ERROR_ARCHIVO;
/// CREAR VECTOR ranking
    if(crearVector(&vRanking,sizeof(tRanking), cantJugadores)!=TODO_OK)
    {
        fclose(archPartidas);
        return SIN_MEMO;
    }
    while(fread(&partida,sizeof(tPartida),1,archPartidas))
    {
        tRanking* posJugador= busquedaSecuencialVector(&vRanking,&(partida.idJugador),compararRankingPorJugador);

        if(!posJugador)
        {
            ranking.idJugador=partida.idJugador;
            ranking.puntajeTotal=partida.puntaje;
            ranking.cantMovimientosTotales=partida.cantMovimientos;
            if(insertarAlFinalVector(&vRanking,&ranking)!=TODO_OK)
            {
                fclose(archPartidas);
                destruirVector(&vRanking);
                return SIN_MEMO;
            }
        }
        else
        {
            posJugador->puntajeTotal+=partida.puntaje;
            posJugador->cantMovimientosTotales+=partida.cantMovimientos;
        }
    }
    fclose(archPartidas);
    /// ordenar por puntaje
    qsort(vRanking.vec, vRanking.cantElem, sizeof(tRanking),compararRankingPorPuntaje);
    recorrerVector(&vRanking,mostrarRanking);
    destruirVector(&vRanking);
    return TODO_OK;
}

int compararRankingPorPuntaje(const void* a, const void* b)
{
    const tRanking* jugador1=a;
    const tRanking* jugador2=b;
    return jugador2->puntajeTotal - jugador1->puntajeTotal;
}
int compararRankingPorJugador(const void* id, const void* ranking)
{
    const int* idBuscado = id;
    const tRanking* reg = ranking;

    return *idBuscado - reg->idJugador;
}
void mostrarRanking(void* elem)
{
    tRanking* dato=(tRanking*)elem;
    printf("\n%5d %10d %10d", dato->idJugador, dato->puntajeTotal, dato->cantMovimientosTotales);
}
int abrirArchivo(FILE** pf, const char* nombreArchivo, const char* modo)
{
    *pf = fopen(nombreArchivo, modo);

    if(!*pf)
    {
        fprintf(stderr, "\nNo se pudo abrir/crear el archivo: %s\n", nombreArchivo);
        return ERROR_ARCHIVO;
    }
    return TODO_OK;
}

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

