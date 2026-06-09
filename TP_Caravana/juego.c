#include "juego.h"


#include "GRAFICOS/logica.h"
#include "GRAFICOS/render.h"
#include "funciones_ListaCircDoble.h"



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
    tListaCD mapa;
    tJugador *jugador;

    ///durante el juego simepre tener puntero a jugador
    jugador = malloc(sizeof(tJugador));
    if(jugador == NULL)
        return SIN_MEMO;

    crearListaCD(&mapa);

    ///cree la lista dinamica y sus nodos tienen toda la info
    cargarMapa(&mapa, jugador, tablero->vidasInicio);

    int turno=0;
    //while(jugador->vidas != 0)
    while(turno!=10) //pa testeos
    {
        renderizarMapa(&mapa);

        procesarTurno(&mapa, jugador);
        turno++;
    }

    tPartida registroPartida;

    registroPartida.idPartida = obtenerUltimoIdPartida(archPartidas) + 1;
    registroPartida.idJugador = idJugador;//guardamos el id real

    registroPartida.puntaje = jugador->puntos;
    registroPartida.cantMovimientos = jugador->turno;
    registroPartida.vidasRestantes = jugador->vidas;

    //grabamos al final
    fseek(archPartidas, 0, SEEK_END);
    fwrite(&registroPartida, sizeof(tPartida), 1, archPartidas);

    //forzamo buffer da
    fflush(archPartidas);

    printf("\npartida guardada para el jugador con id %d.\n", idJugador);


    vaciarListaCD(&mapa);
    free(jugador);
    //y free para todos los bandidos secuencialmente

    return TODO_OK;
}


void mostrarReglas()
{
//prox
}

