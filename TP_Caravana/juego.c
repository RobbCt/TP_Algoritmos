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
    tLista movimientos;
    tPartida registroPartida;
    unsigned turno = 0;

    crearListaCD(&mapa);

    crearLista(&bandidosGlobales);
    crearLista(&movimientos);
    ///cargo la listaCD y todos los bandidos
    cargarMapa(&mapa, &jugador, tablero->vidasInicio, &bandidosGlobales);


    while(jugador.vidas != 0 && ((tTerreno*)jugador.posActual->info)->icon != ICON_SALIDA)
    {
        renderizarPantalla(&mapa, jugador.vidas, jugador.proteccion, jugador.puntos, jugador.turno, turno);

        procesarTurno(&mapa, &jugador, &bandidosGlobales, turno, &movimientos);

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

    printf("\n=========================================");
    printf("\n MOVIMIENTOS REALIZADOS ");
    printf("\n=========================================\n");
    mostrarMovimientos(&movimientos);

    printf("\npartida guardada para el jugador con id %d.\n", idJugador);


    vaciarListaCD(&mapa);
    vaciarLista(&bandidosGlobales);
    vaciarLista(&movimientos);

    return jugador.vidas ? SI : NO;
}

void mostrarMovimientos(tLista* movimientos)
{
    tIteradorLista it;
    tMovimiento* mov;

    mov= obtenerPrimeroInfo(movimientos, &it);

    while(mov)
    {
        printf("%c%u  ", mov->direccion=='A' ? 'F' : 'B', mov->pasos);

        mov= obtenerSiguienteInfo(&it);
    }
    putchar('\n');
}

void mostrarReglas()
{
    puts("=========================================================");
    puts("               CARAVANA DEL DESIERTO");
    puts("=========================================================\n");

    puts("OBJETIVO");
    puts("La caravana debe atravesar el desierto y llegar desde");
    puts("el Campamento Inicial (I) hasta la Ciudad Refugio (S).");
    puts("La partida termina cuando llegas a la ciudad refugio");
    puts("o cuando pierdes todas tus vidas.\n");

    puts("SIMBOLOS DEL TABLERO");
    puts("  I : Campamento Inicial");
    puts("  S : Ciudad Refugio");
    puts("  J : Jugador");
    puts("  B : Bandido");
    puts("  P : Premio (+1 punto)");
    puts("  V : Vida Extra (+1 vida)");
    puts("  O : Oasis (proteccion temporal)");
    puts("  T : Tormenta de arena");
    puts("  . : Casilla vacia\n");

    puts("MOVIMIENTO");
    puts("- En cada turno se lanza un dado virtual (1 a 6).");
    puts("- Debes moverte exactamente la cantidad de casillas");
    puts("  indicada por el dado.");
    puts("- Si hay suficientes casillas entre tu posicion");
    puts("  actual y el campamento inicial, podras elegir");
    puts("  avanzar o retroceder.");
    puts("- Para ganar debes caer exactamente en la Ciudad");
    puts("  Refugio.");
    puts("- Si el dado te hace sobrepasar la salida, rebotaras");
    puts("  y retrocederas las casillas sobrantes.\n");

    puts("EVENTOS ESPECIALES");
    puts("- Premio (P): obtienes 1 punto.");
    puts("- Vida Extra (V): ganas una vida adicional.");
    puts("- Oasis (O): obtienes proteccion para el siguiente");
    puts("  turno.");
    puts("- Tormenta (T): pierdes el siguiente turno.");
    puts("  Si estas protegido por un oasis, la tormenta");
    puts("  no tiene efecto.\n");

    puts("BANDIDOS");
    puts("- Los bandidos se mueven automaticamente.");
    puts("- Pueden avanzar o retroceder por toda la ruta.");
    puts("- Para ellos el tablero es circular.");
    puts("- Si un bandido termina en tu misma casilla,");
    puts("  te intercepta.");
    puts("- Al ser interceptado:");
    puts("     * Pierdes una vida.");
    puts("     * Regresas al Campamento Inicial.");
    puts("     * El bandido es eliminado.");
    puts("- Si estabas protegido por un oasis, no pierdes");
    puts("  la vida y consumes la proteccion.\n");

    puts("CARACTERISTICAS");
    puts("- Puede haber varios bandidos en una misma posicion.");
    puts("- Los bandidos intentan rastrear al jugador utilizando");
    puts("  informacion de los movimientos recientes.");
    puts("- Cada nueva partida genera un desierto diferente.");

    puts("CONSEJOS");
    puts("- Los premios aumentan tu puntuacion final.");
    puts("- Las vidas extra pueden salvar la partida.");
    puts("- Los movimientos de todos los personajes");
    puts("  son encolados y ejecutados posteriormente.");
    puts("- Los oasis son muy utiles contra tormentas");
    puts("  y bandidos.");
    puts("- Piensa bien cada movimiento: el desierto");
    puts("  no perdona errores.\n");
    puts("=========================================================");
    puts("                 BUENA SUERTE VIAJERO");
    puts("=========================================================");

    puts("\nPresione cualquier tecla para volver al menu...");
    getch();
}

