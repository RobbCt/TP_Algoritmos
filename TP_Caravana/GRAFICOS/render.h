#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include <stdio.h>
#include "../funciones_ListaCircDoble.h"

void renderizarPantalla(tListaCD *mapa, int vidas, char proteccion, int puntos, char turno);
void imprimirBorde(int cant);

#endif // RENDER_H_INCLUDED
