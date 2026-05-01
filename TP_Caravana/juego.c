#include "juego.h"

int nuevaPartida(const char* nombreJugador)
{
    tTablero tablero;

    if(generarTablero(&tablero, CARAVANA_ARCH)!=TODO_OK)
    {
        puts("No se pudo generar el tablero...");
        return ERROR_PARTIDA;
    }

    // buscarJugador(nombreJugador)
    // si existe-> actualizar
    // si no existe-> crear archivo/registro
    return TODO_OK;
}


void mostrarReglas()
{
//prox
}

void verRanking()
{
//prox
}

