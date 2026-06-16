#include "ranking.h"

int verRanking(FILE* archJugadores, FILE* archPartidas)
{
    unsigned cantJugadores;
    Vector vRanking;
    tPartida partida;
    tRanking ranking;

    ///ver cantidad de jugadores
    fseek(archJugadores,0,SEEK_END);
    cantJugadores= ftell(archJugadores)/sizeof(tJugadorDatos);

    if(cantJugadores==0)
        return SIN_JUGADOR;

    if(!archPartidas)
        return ERROR_ARCHIVO;
/// CREAR VECTOR ranking
    if(crearVector(&vRanking,sizeof(tRanking), cantJugadores)!=TODO_OK)
        return SIN_MEMO;

    fseek(archPartidas, 0, SEEK_SET);

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

    /// ordenar por puntaje
    //mantenemos las primitivas sin acceder a los elems del tdavector
    ordenarVector(&vRanking, compararRankingPorPuntaje);

    printf("\n=========================================");
    printf("\n  RANKING DE JUGADORES");
    printf("\n=========================================");
    printf("\n%5s %10s %15s", "ID", "PUNTOS", "MOVIMIENTOS");
    printf("\n-----------------------------------------");

    recorrerVector(&vRanking,mostrarRanking);
    printf("\n=========================================\n\n");

    destruirVector(&vRanking);
    return TODO_OK;
}

int compararRankingPorPuntaje(const void* a, const void* b)
{
    const tRanking* jugador1=a;
    const tRanking* jugador2=b;

    //si tienen ==puntaje, desempatamos con el mas eficiente¿ (menos movs)
    if (jugador1->puntajeTotal == jugador2->puntajeTotal)
        return jugador1->cantMovimientosTotales-jugador2->cantMovimientosTotales;

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


