#include "juego.h"
#include "GRAFICOS/logica.h"
#include "GRAFICOS/render.h"
#include "funciones_ListaCircDoble.h"
#include "funciones_listaDinamica.h"




int nuevaPartida(const char* nombreJugador, tArbolBinBusq*arbolIndices, FILE*archJugadores, FILE*archPartidas)
{
    tTablero tablero;

    tJugadorIndice datosJugador;

    ///procesamos al jug antes de iniciar la partida
    //existe?lo buscamos, no existe? lo agregamos al arbol
    if (gestionarJugador(nombreJugador, arbolIndices, archJugadores, &datosJugador) != TODO_OK)
    {
        puts("Error al procesar jugador..");
        return ERROR_PARTIDA;
    }

    ///validar q se haya creado un tablero valido
    if(generarTablero(&tablero, CARAVANA_ARCH)!=TODO_OK)
    {
        puts("No se pudo generar el tablero...");
        return ERROR_PARTIDA;
    }

    ///imprimir/crear tablero desde CARAVANA_ARCH!
    iniciarPartida(&tablero, archPartidas, datosJugador.id);


    return TODO_OK;
}

int iniciarPartida(tTablero *tablero,FILE* archPartidas,int idJugador)
{
    ///durante el juego simepre tener puntero a jugador
    tJugador jugador;
    tListaCD mapa;
    tLista bandidosGlobales;
    tPartida registroPartida;
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

    registroPartida.idPartida = obtenerUltimoIdPartida(archPartidas) + 1;
    registroPartida.idJugador = idJugador;//guardamos el id real

    registroPartida.puntaje = jugador.puntos;
    registroPartida.cantMovimientos = turno;
    registroPartida.vidasRestantes = jugador.vidas;

    //grabamos al final
    fseek(archPartidas, 0, SEEK_END);
    fwrite(&registroPartida, sizeof(tPartida), 1, archPartidas);

    //forzamo buffer da
    fflush(archPartidas);

    printf("\npartida guardada para el jugador con id %d.\n", idJugador);


    vaciarListaCD(&mapa);
    vaciarLista(&bandidosGlobales);
    //y free para todos los bandidos secuencialmente

    return jugador.vidas ? SI : NO;
}

void mostrarReglas()
{
//prox
}

