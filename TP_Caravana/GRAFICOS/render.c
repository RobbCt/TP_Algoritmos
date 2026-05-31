#include "render.h"
#include "logica.h"

#define COLUMNAS_MAPA 15
#define ANCHO_CASILLA 10
#define ALTO_CASILLA 5

void renderizarPantalla(tListaCD *m, int vidas, char proteccion, int puntos, char turno)
{
    tNodo *act, *inicio;
    char fila[COLUMNAS_MAPA];
    int numCol = 0, numFila = 0;
    system("cls");

    act = inicio = *m;

    ///panel superior
    printf("VIDAS: %d | PROT: %c | PUNTOS: %d | TURNO: %c\n\n",
           vidas, proteccion, puntos, turno);

    do
    {
        tTerreno *terreno = (tTerreno*) act->info;

        if(terreno->jugador)
            fila[numCol] = ICON_JUGADOR;
        else if(terreno->bandidos)
            fila[numCol] = ICON_BANDIDO;
        else
            fila[numCol] = terreno->icon;

        numCol++;

        if(numCol == COLUMNAS_MAPA)
        {
            int i, j;

            imprimirBorde(COLUMNAS_MAPA);

            for(j = 0; j < ALTO_CASILLA; j++)
            {
                int filaIcono = ALTO_CASILLA / 2;

                if(j == filaIcono)
                {
                    if(numFila % 2 == 0)
                    {
                        for(i = 0; i < COLUMNAS_MAPA; i++)
                            printf("|%*c%*s",
                                   ANCHO_CASILLA / 2 + 1,
                                   fila[i],
                                   ANCHO_CASILLA - (ANCHO_CASILLA / 2 + 1),
                                   "");
                    }
                    else
                    {
                        for(i = COLUMNAS_MAPA - 1; i >= 0; i--)
                            printf("|%*c%*s",
                                   ANCHO_CASILLA / 2 + 1,
                                   fila[i],
                                   ANCHO_CASILLA - (ANCHO_CASILLA / 2 + 1),
                                   "");
                    }
                }
                else
                {
                    for(i = 0; i < COLUMNAS_MAPA; i++)
                        printf("|%*s", ANCHO_CASILLA, "");
                }

                printf("|\n");
            }

            imprimirBorde(COLUMNAS_MAPA);

            numCol = 0;
            numFila++;
        }

        act = act->sig;

    } while(act != inicio);

    if(numCol > 0)
    {
        int i, j;

        fila[numCol] = '\0';

        imprimirBorde(numCol);

        for(j = 0; j < ALTO_CASILLA; j++)
        {
            int filaIcono = ALTO_CASILLA / 2;

            if(j == filaIcono)
            {
                if(numFila % 2 == 0)
                {
                    for(i = 0; i < numCol; i++)
                        printf("|%*c%*s",
                               ANCHO_CASILLA / 2 + 1,
                               fila[i],
                               ANCHO_CASILLA - (ANCHO_CASILLA / 2 + 1),
                               "");
                }
                else
                {
                    for(i = numCol - 1; i >= 0; i--)
                        printf("|%*c%*s",
                               ANCHO_CASILLA / 2 + 1,
                               fila[i],
                               ANCHO_CASILLA - (ANCHO_CASILLA / 2 + 1),
                               "");
                }
            }
            else
            {
                for(i = 0; i < numCol; i++)
                    printf("|%*s", ANCHO_CASILLA, "");
            }

            printf("|\n");
        }

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
