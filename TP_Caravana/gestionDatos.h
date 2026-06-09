#ifndef GESTIONDATOS_H_INCLUDED
#define GESTIONDATOS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arbolBB.h"
#include "constantes.h"

typedef struct
{
    int id;
    char nombre[30];
} tJugadorIndice; //jugadores.dat

typedef struct
{
    int idPartida;
    int idJugador;
    int puntaje;
    int cantMovimientos;
    //tFecha fecha;
    int vidasRestantes;
} tPartida; //partidas.dat

typedef struct
{
    char nombreJugador[30]; //clave por la q se ordena y busca en el abb
    long nroRegistro; //mi offset muak
} tRegIndice; //.idx

int buscarJugadorPorNombre(const char* nombre, const tArbolBinBusq* arbolIdx,
                           FILE* archJugadores, tJugadorIndice* jugadorClon);
int crearJugador(const char* nombre, tArbolBinBusq* arbolIdx,
                 FILE* archJugadores, tJugadorIndice* nuevoJugador);
int gestionarJugador(const char* nombreIngresado, tArbolBinBusq* arbolIdx,
                     FILE* archJugadores, tJugadorIndice* jugadorDeLaPartida);

int cargarIdxDesdeArch(tArbolBinBusq* arbolIdx, const char* nombreArchivoIndice);
void escribirNodoEnArch(void* elem, unsigned tam, unsigned nivel, void* param);
int cargarIdxDesdeArch(tArbolBinBusq* arbolIdx, const char* nombreArchivoIndice);
int guardarIdxEnArch(tArbolBinBusq* arbolIdx, const char* nombreArchIdx);

int cmpPorNombre(const void* a, const void* b);
int obtenerUltimoIdJugador(FILE* archJugadores);
int obtenerUltimoIdPartida(FILE* archPartidas);

#endif // GESTIONDATOS_H_INCLUDED
