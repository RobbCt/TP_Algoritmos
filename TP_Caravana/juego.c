#include "juego.h"
#include "GRAFICOS/logica.h"
#include "GRAFICOS/render.h"
#include "funciones_ListaCircDoble.h"


int reservarJugador(tJugador** j);


int nuevaPartida(const char* nombreJugador)
{
    tTablero tablero;

    ///validar q se haya creado un tablero valido
//    do
//    {
//        generarTablero(&tablero, CARAVANA_ARCH);
//    }while(!juegoValido(CARAVANA_ARCH ));

    if(generarTablero(&tablero, CARAVANA_ARCH) != TODO_OK)
    {
        puts("No se pudo generar el tablero...");
        return ERROR_PARTIDA;
    }

    ///imprimir/crear tablero desde CARAVANA_ARCH!
    iniciarPartida(&tablero);


    // buscarJugador(nombreJugador)
    // si existe-> actualizar
    // si no existe-> crear archivo/registro
    return TODO_OK;
}

int iniciarPartida(tTablero *tablero)
{

    tListaCD mapa;
    tJugador *jugador;
    tCola colaMovimientos;

    ///durante el juego simepre tener puntero a jugador
    reservarJugador(&jugador);

    crearListaCD(&mapa);

    //Acondicionar Cola Movimientos
    crearCola(&colaMovimientos);

    ///cree la lista dinamica y sus nodos tienen toda la info
    cargarMapa(&mapa, jugador, tablero->vidasInicio);

    while(jugador->vidas != 0)
    {
        renderizarPantalla(&mapa, jugador->vidas, jugador->proteccion, jugador->puntos, jugador->turno);

        procesarTurno(&mapa, jugador, &colaMovimientos);
    }

    vaciarListaCD(&mapa);
    free(jugador);
    //y free para todos los bandidos secuencialmente

    return TODO_OK;
}

int reservarJugador(tJugador** j)
{
    *j = malloc(sizeof(tJugador));
    if (*j == NULL)
        return SIN_MEMO;

    return EXITO;
}





void mostrarReglas()
{
//prox
}

void verRanking()
{
//prox
}

