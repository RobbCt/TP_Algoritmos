#include "juego.h"


#include "GRAFICOS/logica.h"
#include "GRAFICOS/render.h"
///cambiar por LCDE
#include "funciones_listaDinamica.h"

int nuevaPartida(const char* nombreJugador)
{
    tTablero tablero;

    ///validar q se haya creado un tablero valido
//    do
//    {
//        generarTablero(&tablero, CARAVANA_ARCH);
//    }while(!juegoValido(CARAVANA_ARCH ));

    if(generarTablero(&tablero, CARAVANA_ARCH)!=TODO_OK)
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
    tLista mapa;
    tJugador *jugador;

    ///durante el juego simepre tener puntero a jugador
    jugador = malloc(sizeof(tJugador));
    if(jugador == NULL)
        return SIN_MEMO;

    crearLista(&mapa);

    ///cree la lista dinamica y sus nodos tienen toda la info
    cargarMapa(&mapa, jugador, tablero->vidasInicio);

    while(jugador->vidas != 0)
    {
        renderizarMapa(&mapa);

        procesarTurno(&mapa, jugador);
    }

    vaciarLista(&mapa);
    free(jugador);
    //y free para todos los bandidos secuencialmente

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

