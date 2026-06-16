#ifndef LOGICA_H_INCLUDED
#define LOGICA_H_INCLUDED

#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include "render.h"
#include "../constantes.h"
#include "../funciones_colaDinamica.h"
#include "../funciones_ListaCircDoble.h"
#include "../funciones_listaDinamica.h"

typedef struct
{
    char icon;
    char ultimoMov;         //dir de movimiento I, D
    tNodoCD *posActual;
}tBandido;

typedef struct
{
    char icon;
    char proteccion;          //tiene proteccion? SI, NO
    char turno;               //puede jugar?      SI, NO
    unsigned vidas;           //vidas restantes?  gano, perdio
    int puntos;
    tNodoCD *posActual;
}tJugador;

typedef struct
{
    unsigned pasos;         //casillas a moverse
    char direccion;         //dir del movimiento I, D
    tNodoCD *destino;
}tMovimiento;

typedef struct
{
    char icon;
    unsigned turnoActualizado;  //en q turno un B paso por aqui?
    unsigned bandidos;          //contador de B en casilla
    unsigned jugador;           //esta el J aqui? 1, 0
}tTerreno;




int cargarMapa(tListaCD *mapa, tJugador *jugador, tLista *bGlobales);

void procesarTurno(tListaCD *mapa, tJugador *jugador, tLista *bGlobales, unsigned turno, tLista* listaMovimientos);




#endif // LOGICA_H_INCLUDED
