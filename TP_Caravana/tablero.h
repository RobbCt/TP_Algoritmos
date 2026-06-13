#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED

#include "tipos.h"
#include "constantes.h"
#include "config.h"

int generarTablero( tTablero*tab, char* nArch);
int valRandom(tTablero* tab, int pos, int anterior);

int generarPorZonas(FILE* pf, tTablero* tab, char icons[]);
void generarPorPaquetes(FILE* pf, tTablero* tab, char icons[]);

void calcularRangoZona(int zona, int cantPos, int cantZonas, int* inicio, int* fin);
int obtenerZonaDePos(int pos, int cantPos, int cantZonas);
int selecEvento(tTablero* tab);
int selecEventoNoBandido(tTablero* tab);

#endif // TABLERO_H_INCLUDED
