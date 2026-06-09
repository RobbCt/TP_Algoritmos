#include "juego.h"
#include "GRAFICOS/logica.h"
#include "GRAFICOS/render.h"
#include "funciones_ListaCircDoble.h"
#include "funciones_listaDinamica.h"


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

char iniciarPartida(tTablero *tablero)
{
    ///durante el juego simepre tener puntero a jugador
    tJugador jugador;
    tListaCD mapa;
    tLista bandidosGlobales;
    unsigned turno = 0;

    crearListaCD(&mapa);

    crearLista(&bandidosGlobales);

    ///cargo la listaCD y todos los bandidos
    cargarMapa(&mapa, &jugador, tablero->vidasInicio, &bandidosGlobales);

    while(jugador.vidas != 0 && ((tTerreno*)jugador.posActual->info)->icon != ICON_SALIDA)
    {
        renderizarPantalla(&mapa, jugador.vidas, jugador.proteccion, jugador.puntos, jugador.turno, turno);

        procesarTurno(&mapa, &jugador, &bandidosGlobales, turno);

        turno++;
    }

    vaciarListaCD(&mapa);
    vaciarLista(&bandidosGlobales);
    //y free para todos los bandidos secuencialmente

    return jugador.vidas ? SI : NO;
}

void mostrarReglas()
{
//prox
}

void verRanking()
{
//prox
}

