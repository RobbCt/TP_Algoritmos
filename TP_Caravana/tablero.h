#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "tipos.h"
#include "constantes.h"
#include "config.h"

int generarTablero( tTablero*tab, char* nArch);
int valRandom(tTablero* tab, int pos, int anterior);

#endif // TABLERO_H_INCLUDED
