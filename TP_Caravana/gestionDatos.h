#ifndef GESTIONDATOS_H_INCLUDED
#define GESTIONDATOS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "arbolBB.h"
#include "constantes.h"
#include "funciones_listaDinamica.h"

typedef struct
{
    int id;
    char nombre[30]; //nombre real ingresado
    char alias[30]; // identificador único
} tJugadorDatos; //jugadores.dat

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
    char alias[30];
    long nroRegistro; //mi offset muak
} tRegIndice; //.idx

typedef struct
{
    tLista listaAlias;   // lista de alias
    int cantidad;     //cuántos alias hay
} tNombreRepetido;


int buscarJugadorPorNombre(const char* nombre, const tArbolBinBusq* arbolIdx,
                           FILE* archJugadores, tJugadorDatos* jugadorClon);
int seleccionarYRecuperarJugador(tNombreRepetido* jugadoresEncontrados,
                                 FILE* archJugadores, tJugadorDatos* jugadorClon);

int crearJugador(const char* nombre, tArbolBinBusq* arbolIdx,
                 FILE* archJugadores, tJugadorDatos* nuevoJugador);
int gestionarJugador(const char* nombreIngresado, tArbolBinBusq* arbolIdx,
                     FILE* archJugadores, tJugadorDatos* jugadorDeLaPartida);

int cargarIdxDesdeArch(tArbolBinBusq* arbolIdx, const char* nombreArchivoIndice);
void escribirNodoEnArch(void* elem, unsigned tam, unsigned nivel, void* param);
int cargarIdxDesdeArch(tArbolBinBusq* arbolIdx, const char* nombreArchivoIndice);
int guardarIdxEnArch(tArbolBinBusq* arbolIdx, const char* nombreArchIdx);

int cmpPorClaveCompleta(const void* a, const void* b);
int cmpIndiceConNombre(const void* infoNodo, const void* info);
void mostrarYGuardarEnLista(const void *infoNodo, void *param);//accion

int obtenerUltimoIdJugador(FILE* archJugadores);
int obtenerUltimoIdPartida(FILE* archPartidas);
void generarAliasUnico(const char* nombre,  const tArbolBinBusq* arbolIdx, char* aliasGenerado);

#endif // GESTIONDATOS_H_INCLUDED
