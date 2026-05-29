#ifndef LOGICA_H_INCLUDED
#define LOGICA_H_INCLUDED

#include <stdio.h>
#include "../tdaCola.h"
#include "../funciones_ListaCircDoble.h"

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
    tNodo *posActual;
    unsigned pasos;
    char direccion;
    int id;

}tMovimiento;

typedef struct
{
    char icon;
    int temperatura; //para algoritmo de bandidos (caliente, frio)
    tBandido *bandido;
    tJugador *jugador;
}tTerreno;



int cargarMapa(tListaCD *mapa, tJugador *jugador, int vidasJugador);
void procesarTurno(tListaCD *mapa, tJugador *jugador, tCola* colaMovimientos);
int RealizarMovimiento(tJugador* jugador, tCola* colaMovimientos, tListaCD* mapa);
int tirarDado();




#endif // LOGICA_H_INCLUDED
