#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

//valores de retorno
#define TODO_OK          0
#define ERROR_ARCHIVO    1
#define ERROR_TABLERO    2
#define ERROR_PARTIDA    3
#define ERROR_CONFIG     4

//tamaños def
#define TAMLINEA        100
#define TAM_NOMBRE       50

//rutas archs
#define CONFIG_ARCH      "config.txt"
#define CARAVANA_ARCH    "caravana.txt"
#define RANKING_ARCH     "ranking.dat"

//valores tablero
#define ICON_JUGADOR     'J'
#define ICON_PUNTO       '.'
#define ICON_BANDIDO     'B'
#define ICON_PREMIO      'P'
#define ICON_VIDA        'V'
#define ICON_OASIS       'O'
#define ICON_TORMENTA    'T'
#define ICON_INICIO      'I'
#define ICON_SALIDA      'S'

//datos para los dados rand (1-6)
#define CASILLERO_PUNTO      1
#define CASILLERO_BANDIDO    2
#define CASILLERO_PREMIO     3
#define CASILLERO_VIDA       4
#define CASILLERO_OASIS      5
#define CASILLERO_TORMENTA   6


#endif // CONSTANTES_H_INCLUDED
