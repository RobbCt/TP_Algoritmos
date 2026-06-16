#include "render.h"
#include "logica.h"

///CONFIGURACION DE RENDERIZADO
#define COLUMNAS_MAPA      15
#define ANCHO_CASILLA      9
#define ALTO_CASILLA       7
#define MAX_TEXTO_CASILLA  (ANCHO_CASILLA + 1)

typedef struct
{
    //vector de (COLUMNAS_MAPA) elementos, cada elemento es
    //una matriz de (ALTO_CASILLA) × (MAX_TEXTO_CASILLA) chars

    char fila[COLUMNAS_MAPA][ALTO_CASILLA][MAX_TEXTO_CASILLA];
    int numCol;
    int numFila;
    int vidasJugador;

}tContexto;



void cargarCasillaRender(void *info, void *contextoVoid);
void imprimirFilaRender(tContexto *contexto);
void imprimirBorde(int cant);



void renderizarPantalla(tListaCD *m, int vidas, char proteccion, int puntos, char turno, unsigned nTurno)
{
    tContexto contexto;

    //indices para ver q casillero se carga
    contexto.numCol = 0;
    contexto.numFila = 0;
    contexto.vidasJugador = vidas;

    system("cls");

    //encabezado de interfaz
    printf("VIDAS: %d | PROT: %c | PUNTOS: %d | TURNO: %c | NUMERO DE TURNO: %u\n\n",
           vidas, proteccion, puntos, turno, nTurno);

    //cargar cada nodo de CD en el vector de la struct de contexto
    mostrarLista2CD(m, cargarCasillaRender, &contexto);

    if(contexto.numCol > 0)
    {
        int i, j;

        //borde superior
        imprimirBorde(contexto.numCol);

        for(j = 0; j < ALTO_CASILLA; j++)
        {
            int largo, izq, der;

            //filas pares sentido izq--->der
            if(contexto.numFila % 2 == 0)
            {
                for(i = 0; i < contexto.numCol; i++)
                {
                    //tam casilla (para en centrado)
                    largo = strlen(contexto.fila[i][j]);
                    izq = (ANCHO_CASILLA - largo) / 2;
                    der = ANCHO_CASILLA - largo - izq;

                    //lleno el tam (centrado
                    printf("|");
                    printf("%*s", izq, "");
                    printf("%s", contexto.fila[i][j]);
                    printf("%*s", der, "");
                }
            }
            else
            {
                //filas pares sentido izq<---der
                for(i = contexto.numCol - 1; i >= 0; i--)
                {
                    //tam casilla (para en centrado)
                    largo = strlen(contexto.fila[i][j]);
                    izq = (ANCHO_CASILLA - largo) / 2;
                    der = ANCHO_CASILLA - largo - izq;

                    //lleno el tam (centrado)
                    printf("|");
                    printf("%*s", izq, "");
                    printf("%s", contexto.fila[i][j]);
                    printf("%*s", der, "");
                }
            }

            printf("|\n");
        }

        //borde inferior
        imprimirBorde(contexto.numCol);
    }
}

void cargarCasillaRender(void *info, void *contextoVoid)
{
    //funcion accion, cargo la cadena de cada casilla con la info de la lista
    tTerreno *terreno = (tTerreno*)info;
    tContexto *contexto = (tContexto*)contextoVoid;



    char items[ALTO_CASILLA][MAX_TEXTO_CASILLA];
    int i, cantItems = 0, inicioVertical = (ALTO_CASILLA - cantItems) / 2;//fila de en medio

    //icono de terreno (no imprime el '.')
    if(terreno->icon != ICON_PUNTO)
        sprintf(items[cantItems++], "%c", terreno->icon);

    //jugador en casilla SI TIENE VIDAS
    if(terreno->jugador && contexto->vidasJugador > 0)
        sprintf(items[cantItems++], "%c", ICON_JUGADOR);

    //bandido/s en casilla (si los hay)
    if(terreno->bandidos)
    {
        if(terreno->bandidos == 1)
            sprintf(items[cantItems++], "%c", ICON_BANDIDO);
        else
            sprintf(items[cantItems++], "%ux%c", terreno->bandidos, ICON_BANDIDO);
    }

    //llegar a la casilla siguiente sin basura de esta
    for(i = 0; i < ALTO_CASILLA; i++)
        contexto->fila[contexto->numCol][i][0] = '\0';

    //copio item de casilla al vector para despues imprimirlas con formato deseado
    for(i = 0; i < cantItems && inicioVertical + i < ALTO_CASILLA; i++)
            strcpy(contexto->fila[contexto->numCol][inicioVertical + i], items[i]);

    contexto->numCol++;

    //si terminamos de cargar una fila, la imprimimos y reiniciamos contadores para la siguiente
    if(contexto->numCol == COLUMNAS_MAPA)
    {
        imprimirFilaRender(contexto);

        contexto->numCol = 0;
        contexto->numFila++;
    }
}

void imprimirFilaRender(tContexto *contexto)
{
    int i, j;

    //borde superior
    imprimirBorde(COLUMNAS_MAPA);

    for(j = 0; j < ALTO_CASILLA; j++)
    {
        int largo, izq, der;

        //filas pares sentido izq--->der
        if(contexto->numFila % 2 == 0)
        {
            for(i = 0; i < COLUMNAS_MAPA; i++)
            {
                largo = strlen(contexto->fila[i][j]);
                izq = (ANCHO_CASILLA - largo) / 2;
                der = ANCHO_CASILLA - largo - izq;

                printf("|");
                printf("%*s", izq, "");
                printf("%s", contexto->fila[i][j]);
                printf("%*s", der, "");
            }
        }
        else
        {
            //filas pares sentido izq<---der
            for(i = COLUMNAS_MAPA - 1; i >= 0; i--)
            {
                largo = strlen(contexto->fila[i][j]);
                izq = (ANCHO_CASILLA - largo) / 2;
                der = ANCHO_CASILLA - largo - izq;

                printf("|");
                printf("%*s", izq, "");
                printf("%s", contexto->fila[i][j]);
                printf("%*s", der, "");
            }
        }

        printf("|\n");
    }

    imprimirBorde(COLUMNAS_MAPA);
}

void imprimirBorde(int cantCasillas)
{
    //dibuja linea de borde horizontal con el separador '+' por casilla
    //+----+----+----+
    int i, j;

    for(i = 0; i < cantCasillas; i++)
    {
        printf("+");

        for(j = 0; j < ANCHO_CASILLA; j++)
            printf("-");
    }

    printf("+\n");
}


