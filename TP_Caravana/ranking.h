#ifndef RANKING_H_INCLUDED
#define RANKING_H_INCLUDED

#include "config.h"


typedef struct
{
    int id;
    char nombre[30];
} tJugadorIndice;

typedef struct
{
    int idPartida;
    int idJugador;
    int puntaje;
    int cantMovimientos;
    //tFecha fecha;
    int vidasRestantes;
} tPartida;

typedef struct
{
    void* vec;
    unsigned cantElem;
    unsigned tamElem;
    unsigned tamV;
}Vector;

typedef struct
{
    int idJugador;
    int puntajeTotal;
    int cantMovimientosTotales;
}tRanking;

int abrirArchivo(FILE** pf, const char* nombreArchivo, const char* modo);
int crearVector(Vector* vec, unsigned tamElem, unsigned tamInicial);
void destruirVector(Vector* vec);
int realocar(Vector* vec, unsigned ntamV);
int insertarAlFinalVector(Vector* vec, const void* elem);
void* busquedaSecuencialVector(const Vector* vec, const void* elem, int cmp(const void*, const void*));
void recorrerVector(Vector* vec, void accion (void*));
int compararRankingPorPuntaje(const void* a, const void* b);
int compararRankingPorJugador(const void* id, const void* ranking);
int verRanking();
void mostrarRanking(void* elem);

#endif // RANKING_H_INCLUDED
