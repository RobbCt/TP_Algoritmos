#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#include "tipos.h"
#include "tablero.h"
#include "constantes.h"
#include "ranking.h"
#include "arbolBB.h"
#include "gestionDatos.h"

void mostrarReglas();
int nuevaPartida(const char* nombreJugador, tArbolBinBusq*arbolIndices,
                 FILE*archJugadores,FILE*archPartidas);
int iniciarPartida(tTablero *tablero, FILE*,int);

#endif // JUEGO_H_INCLUDED
