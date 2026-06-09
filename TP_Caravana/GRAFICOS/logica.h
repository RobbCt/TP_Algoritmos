#ifndef LOGICA_H_INCLUDED
#define LOGICA_H_INCLUDED

#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include "../constantes.h"
#include "../funciones_colaDinamica.h"
#include "../funciones_ListaCircDoble.h"
#include "../funciones_listaDinamica.h"

typedef struct
{
    char icon;
    char ultimoMov;
    tNodo *posActual;
}tBandido;

typedef struct
{
    int id;
    //char nombre[30];
    char icon;
    char proteccion;
    char turno; //tiene tuno para tirar?
    unsigned vidas; ///vidas restantes? gano; perdio;
    int puntos;
    tNodo *posActual;
}tJugador;

typedef struct
{
    tNodo *destino;
    unsigned pasos;
    char direccion;
}tMovimiento;

typedef struct
{
    char icon;
    //int tempTemporal; //para algoritmo de bandidos (caliente, frio)
    unsigned TurnoActualizado;
    unsigned bandidos;
    unsigned jugador;
}tTerreno;



int cargarMapa(tListaCD *mapa, tJugador *jugador, int vidasJugador, tLista *bGlobales);

void procesarTurno(tListaCD *mapa, tJugador *jugador, tLista *bGlobales, unsigned turno);

void IABandidos(tMovimiento *mov, tBandido *bandido, unsigned turnoAc);

int calcularTemperaturaMov(tBandido *bandido, tNodo *destino, char direccion, unsigned turnoAc);

int realizarMovimientos(tJugador* j, tLista *bGlobales, tCola* colaMovimientos, tListaCD* mapa, unsigned turno);

#endif // LOGICA_H_INCLUDED
