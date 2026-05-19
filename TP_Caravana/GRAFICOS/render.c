#include "render.h"
#include "logica.h"

#define COLUMNAS_MAPA 15
#define ANCHO_CASILLA 7

void renderizarMapa(tListaCD *m)
{
    tNodo *act = *m;
    tNodo *inicio = act;

    char fila[COLUMNAS_MAPA];

    int numCol = 0;
    int numFila = 0;

    system("cls");

    ///panel superior
    tTerreno *inicioTerreno = (tTerreno*) act->info;

    if(inicioTerreno->jugador)
    {
        printf("VIDAS: %d | PROT: %d | PUNTOS: %d | TURNO: %d\n\n",
               inicioTerreno->jugador->vidas,
               inicioTerreno->jugador->proteccion,
               inicioTerreno->jugador->puntos,
               inicioTerreno->jugador->turno);
    }

    do
    {
        tTerreno *terreno = (tTerreno*) act->info;

        if(terreno->jugador)
            fila[numCol] = terreno->jugador->icon;
        else if(terreno->bandido)
            fila[numCol] = terreno->bandido->icon;
        else
            fila[numCol] = terreno->icon;

        numCol++;

        if(numCol == COLUMNAS_MAPA)
        {
            int i;

            imprimirBorde(COLUMNAS_MAPA);

            for(i = 0; i < COLUMNAS_MAPA; i++)
                printf("|       ");
            printf("|\n");

            if(numFila % 2 == 0)
            {
                for(i = 0; i < COLUMNAS_MAPA; i++)
                    printf("|   %c   ", fila[i]);
            }
            else
            {
                for(i = COLUMNAS_MAPA - 1; i >= 0; i--)
                    printf("|   %c   ", fila[i]);
            }

            printf("|\n");

            for(i = 0; i < COLUMNAS_MAPA; i++)
                printf("|       ");
            printf("|\n");

            imprimirBorde(COLUMNAS_MAPA);

            numCol = 0;
            numFila++;
        }

        act = act->sig;

    } while(act != inicio);

    if(numCol > 0)
    {
        int i;

        fila[numCol] = '\0';

        imprimirBorde(numCol);

        for(i = 0; i < numCol; i++)
            printf("|       ");
        printf("|\n");

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
