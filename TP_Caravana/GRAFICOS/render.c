#include "render.h"
#include "logica.h"

#define COLUMNAS_MAPA 15
#define ANCHO_CASILLA 9
#define ALTO_CASILLA 5

void renderizarPantalla(tListaCD *m, int vidas, char proteccion, int puntos, char turno, unsigned nTurno)
{
    tNodo *act, *inicio;
    char fila[COLUMNAS_MAPA][16];
    int numCol = 0, numFila = 0;

    system("cls");

    act = inicio = *m;

    printf("VIDAS: %d | PROT: %c | PUNTOS: %d | TURNO: %c | NUMERO DE TURNO: %u\n\n",
           vidas, proteccion, puntos, turno, nTurno);

    do
    {
        tTerreno *terreno = (tTerreno*)act->info;
        if(terreno->jugador && terreno->inicio)
            sprintf(fila[numCol], "I + %c", ICON_JUGADOR);
        else if(terreno->jugador && !terreno->inicio)
        {
            sprintf(fila[numCol], "%c", ICON_JUGADOR);
        }
        else if(terreno->bandidos == 1 && terreno->salida)
        {
            sprintf(fila[numCol], "S + B");

            //bandidosImpresos++;
            //bandidosExistentes += terreno->bandidos;
        }
        else if(terreno->bandidos > 1 && terreno->salida)
        {
            sprintf(fila[numCol], "S + Bx%u", terreno->bandidos);

            //bandidosImpresos++;
            //bandidosExistentes += terreno->bandidos;
        }
        else if(terreno->bandidos > 1 && !terreno->salida)
        {
            sprintf(fila[numCol], "Bx%u", terreno->bandidos);

            //bandidosImpresos++;
            //bandidosExistentes += terreno->bandidos;
        }
        else if(terreno->bandidos == 1 && !terreno->salida)
        {
            sprintf(fila[numCol], "%c", ICON_BANDIDO);

            //bandidosImpresos++;
            //bandidosExistentes++;
        }
        else

        {
            if(terreno->vidas)
                sprintf(fila[numCol], "V");
            if(terreno->puntos)
                sprintf(fila[numCol], "P");
            if(terreno->oasis)
                sprintf(fila[numCol], "O");
            if(terreno->tormenta)
                sprintf(fila[numCol], "T");
            if(terreno->salida)
                sprintf(fila[numCol], "S");
            if(terreno->inicio)
                sprintf(fila[numCol], "I");
            else if(!terreno->vidas && !terreno->puntos && !terreno->oasis && !terreno->tormenta && !terreno->salida)
                sprintf(fila[numCol], ".");

        }


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
                        {
                            int len = strlen(fila[i]);
                            int izq = (ANCHO_CASILLA - len) / 2;
                            int der = ANCHO_CASILLA - len - izq;

                            printf("|");
                            printf("%*s", izq, "");
                            printf("%s", fila[i]);
                            printf("%*s", der, "");
                        }
                    }
                    else
                    {
                        for(i = COLUMNAS_MAPA - 1; i >= 0; i--)
                        {
                            int len = strlen(fila[i]);
                            int izq = (ANCHO_CASILLA - len) / 2;
                            int der = ANCHO_CASILLA - len - izq;

                            printf("|");
                            printf("%*s", izq, "");
                            printf("%s", fila[i]);
                            printf("%*s", der, "");
                        }
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

        imprimirBorde(numCol);

        for(j = 0; j < ALTO_CASILLA; j++)
        {
            int filaIcono = ALTO_CASILLA / 2;

            if(j == filaIcono)
            {
                if(numFila % 2 == 0)
                {
                    for(i = 0; i < numCol; i++)
                    {
                        int len = strlen(fila[i]);
                        int izq = (ANCHO_CASILLA - len) / 2;
                        int der = ANCHO_CASILLA - len - izq;

                        printf("|");
                        printf("%*s", izq, "");
                        printf("%s", fila[i]);
                        printf("%*s", der, "");
                    }
                }
                else
                {
                    for(i = numCol - 1; i >= 0; i--)
                    {
                        int len = strlen(fila[i]);
                        int izq = (ANCHO_CASILLA - len) / 2;
                        int der = ANCHO_CASILLA - len - izq;

                        printf("|");
                        printf("%*s", izq, "");
                        printf("%s", fila[i]);
                        printf("%*s", der, "");
                    }
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
