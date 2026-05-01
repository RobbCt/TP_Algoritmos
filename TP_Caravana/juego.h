#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tipos.h"
#include "tablero.h"
#include "constantes.h"


void mostrarReglas();
int nuevaPartida(const char* nuevoJugador);
void verRanking();

#endif // JUEGO_H_INCLUDED
