#include "render.h"
#include "logica.h"

#define COLUMNAS_MAPA 15
#define ANCHO_CASILLA 7

void renderizarMapa(tLista *m)
{
    tNodo *act = *m;

    char fila[COLUMNAS_MAPA];

    int numCol = 0;
    int numFila = 0;

    system("cls");

    ///panel superior
    if(act)
    {
        tTerreno *inicio = (tTerreno*) act->info;

        if(inicio->jugador)
        {
            printf("VIDAS: %d | PROT: %d | PUNTOS: %d | TURNO: %d\n\n",
                   inicio->jugador->vidas,
                   inicio->jugador->proteccion,
                   inicio->jugador->puntos,
                   inicio->jugador->turno);
        }
    }

    ///mapa
    while(act)
    {
        tTerreno *terreno = (tTerreno*) act->info;

        /// prioridad visual
        if(terreno->jugador)
            fila[numCol] = terreno->jugador->icon;

        else if(terreno->bandido)
            fila[numCol] = terreno->bandido->icon;

        else
            fila[numCol] = terreno->icon;

        numCol++;

        ///fila completa
        if(numCol == COLUMNAS_MAPA)
        {
            int i;

            imprimirBorde(COLUMNAS_MAPA);

            /// linea vacia superior
            for(i = 0; i < COLUMNAS_MAPA; i++)
                printf("|       ");
            printf("|\n");

            /// linea iconos
            if(numFila % 2 == 0)
            {
                /// izquierda -> derecha
                for(i = 0; i < COLUMNAS_MAPA; i++)
                    printf("|   %c   ", fila[i]);
            }
            else
            {
                /// derecha -> izquierda
                for(i = COLUMNAS_MAPA - 1; i >= 0; i--)
                    printf("|   %c   ", fila[i]);
            }

            printf("|\n");

            /// linea vacia inferior
            for(i = 0; i < COLUMNAS_MAPA; i++)
                printf("|       ");
            printf("|\n");

            imprimirBorde(COLUMNAS_MAPA);

            numCol = 0;
            numFila++;
        }

        act = act->sig;
    }

    ///ultima fila completa
    if(numCol > 0)
    {
        int i;

        imprimirBorde(numCol);

        ///linea vacia superior
        for(i = 0; i < numCol; i++)
            printf("|       ");
        printf("|\n");

        ///linea iconos
        if(numFila % 2 == 0)
        {
            for(i = 0; i < numCol; i++)
                printf("|   %c   ", fila[i]);
        }
        else
        {
            for(i = numCol - 1; i >= 0; i--)
                printf("|   %c   ", fila[i]);
        }

        printf("|\n");

        ///linea vacia inferior
        for(i = 0; i < numCol; i++)
            printf("|       ");
        printf("|\n");

        imprimirBorde(numCol);
    }
}

void imprimirBorde(int cantCasillas)
{
    int i, j;

    for(i = 0; i < cantCasillas; i++)
    {
        printf("+");

        for(j = 0; j < ANCHO_CASILLA; j++)
            printf("-");
    }

    printf("+\n");
}
