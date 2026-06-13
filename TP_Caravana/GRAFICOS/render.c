#include "render.h"
#include "logica.h"

#define COLUMNAS_MAPA      15
#define ANCHO_CASILLA      9
#define ALTO_CASILLA       7
#define MAX_TEXTO_CASILLA  (ANCHO_CASILLA + 1)

//vector de (COLUMNAS_MAPA) elementos, cada elemento es
//una matriz de (ALTO_CASILLA) × (MAX_TEXTO_CASILLA) chars

void renderizarPantalla(tListaCD *m, int vidas, char proteccion, int puntos, char turno, unsigned nTurno)
{
    tNodo *act, *inicio;

    char fila[COLUMNAS_MAPA][ALTO_CASILLA][MAX_TEXTO_CASILLA];

    int numCol = 0;
    int numFila = 0;

    system("cls");

    act = inicio = *m;

    printf("VIDAS: %d | PROT: %c | PUNTOS: %d | TURNO: %c | NUMERO DE TURNO: %u\n\n",
           vidas, proteccion, puntos, turno, nTurno);

    do
    {
        tTerreno *terreno = (tTerreno*)act->info;
        char items[ALTO_CASILLA][MAX_TEXTO_CASILLA];
        int cantItems = 0;
        int i;

        if(terreno->icon != ICON_PUNTO)
            sprintf(items[cantItems++], "%c", terreno->icon);

        /* Jugador */
        if(terreno->jugador)
            sprintf(items[cantItems++], "%c", ICON_JUGADOR);

        /* Bandidos */
        if(terreno->bandidos)
        {
            if(terreno->bandidos == 1)
                sprintf(items[cantItems++], "%c", ICON_BANDIDO);
            else
                sprintf(items[cantItems++], "%ux%c",
                        terreno->bandidos,
                        ICON_BANDIDO);
        }

        /* Limpiar casilla */
        for(i = 0; i < ALTO_CASILLA; i++)
            fila[numCol][i][0] = '\0';

        /* Centrado vertical */
        {
            int inicioVertical = (ALTO_CASILLA - cantItems) / 2;

            for(i = 0;
                i < cantItems && inicioVertical + i < ALTO_CASILLA;
                i++)
            {
                strcpy(fila[numCol][inicioVertical + i], items[i]);
            }
        }

        numCol++;

        if(numCol == COLUMNAS_MAPA)
        {
            int i, j;

            imprimirBorde(COLUMNAS_MAPA);

            for(j = 0; j < ALTO_CASILLA; j++)
            {
                if(numFila % 2 == 0)
                {
                    for(i = 0; i < COLUMNAS_MAPA; i++)
                    {
                        int len = strlen(fila[i][j]);
                        int izq = (ANCHO_CASILLA - len) / 2;
                        int der = ANCHO_CASILLA - len - izq;

                        printf("|");
                        printf("%*s", izq, "");
                        printf("%s", fila[i][j]);
                        printf("%*s", der, "");
                    }
                }
                else
                {
                    for(i = COLUMNAS_MAPA - 1; i >= 0; i--)
                    {
                        int len = strlen(fila[i][j]);
                        int izq = (ANCHO_CASILLA - len) / 2;
                        int der = ANCHO_CASILLA - len - izq;

                        printf("|");
                        printf("%*s", izq, "");
                        printf("%s", fila[i][j]);
                        printf("%*s", der, "");
                    }
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
            if(numFila % 2 == 0)
            {
                for(i = 0; i < numCol; i++)
                {
                    int len = strlen(fila[i][j]);
                    int izq = (ANCHO_CASILLA - len) / 2;
                    int der = ANCHO_CASILLA - len - izq;

                    printf("|");
                    printf("%*s", izq, "");
                    printf("%s", fila[i][j]);
                    printf("%*s", der, "");
                }
            }
            else
            {
                for(i = numCol - 1; i >= 0; i--)
                {
                    int len = strlen(fila[i][j]);
                    int izq = (ANCHO_CASILLA - len) / 2;
                    int der = ANCHO_CASILLA - len - izq;

                    printf("|");
                    printf("%*s", izq, "");
                    printf("%s", fila[i][j]);
                    printf("%*s", der, "");
                }
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


