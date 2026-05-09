#ifndef LOGICA_H_INCLUDED
#define LOGICA_H_INCLUDED

#include <stdio.h>

///cambiar por LCDE
#include "../funciones_listaDinamica.h"

typedef struct
{
    char icon;
    int ultimoMov;
    tNodo *posActual;
}tBandido;

typedef struct
{
    char icon;
    int proteccion;
    int turno; //tiene tuno para tirar?
    int vidas;
    int puntos;
    tNodo *posActual;
}tJugador;

typedef struct
{
    char icon;
    int temperatura; //para algoritmo de bandidos (caliente, frio)
    tBandido *bandido;
    tJugador *jugador;
}tTerreno;



int cargarMapa(tLista *mapa, tJugador *jugador, int vidasJugador);
void procesarTurno(tLista *mapa, tJugador *jugador);




#endif // LOGICA_H_INCLUDED
