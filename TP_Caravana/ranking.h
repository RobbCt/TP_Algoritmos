#ifndef RANKING_H_INCLUDED
#define RANKING_H_INCLUDED

#include "config.h"
#include "gestionDatos.h"
#include "TDAvector.h"


typedef struct
{
    int idJugador;
    int puntajeTotal;
    int cantMovimientosTotales;
}tRanking;

int abrirArchivo(FILE** pf, const char* nombreArchivo, const char* modo);
int compararRankingPorPuntaje(const void* a, const void* b);
int compararRankingPorJugador(const void* id, const void* ranking);
int verRanking(FILE* archJugadores, FILE* archPartidas);
void mostrarRanking(void* elem);

#endif // RANKING_H_INCLUDED
