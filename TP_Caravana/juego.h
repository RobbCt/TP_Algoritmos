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
#include "funciones_listaDinamica.h"
#include "GRAFICOS/logica.h"
#include "GRAFICOS/render.h"
#include "funciones_ListaCircDoble.h"


void mostrarReglas();
int nuevaPartida(const char* nombreJugador, tArbolBinBusq *arbolIndices, FILE *archJugadores,FILE *archPartidas);

#endif // JUEGO_H_INCLUDED
