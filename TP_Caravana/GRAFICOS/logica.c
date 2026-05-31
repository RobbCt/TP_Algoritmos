#include "logica.h"

int verificar(tListaCD *m);
char iconoDeLinea(char* linea);
int reservarBandido(tBandido** b);

int cargarMapa(tListaCD *mapa, tJugador *jugador, int vidasJugador, tLista *bGlobales)
{
    FILE *pf = fopen(CARAVANA_ARCH, "rt");
    if(!pf)
        return ERROR_ARCHIVO;

    char linea[TAMLINEA];
    tTerreno terreno;

    ///cragamos el primer casillero a la listaCD
    fgets(linea, sizeof(linea), pf);
    if(strcmpi(linea,"01:[I J]\n") == 0)
    {
        terreno.icon = ICON_INICIO;
        terreno.temperatura = 0;
        terreno.bandidos = 0;
        terreno.jugador = 1;

        jugador->icon = ICON_JUGADOR;
        jugador->proteccion = 'N';
        jugador->turno = 'Y';
        jugador->vidas = vidasJugador;
        jugador->puntos = 0;

        ponerAlFinalCD(mapa, &terreno, sizeof(terreno));

        jugador->posActual = *mapa;
    }
    else
    {
        fclose(pf);
        return ERROR_TABLERO;
    }

    ///cargamos los siguientes casilleros a la listaCD
    while(fgets(linea, sizeof(linea), pf))
    {
        tBandido bandido;
        char icon = iconoDeLinea(linea);

        if(icon == ICON_BANDIDO)
        {
            //bandidos en terreno

            terreno.icon = ICON_PUNTO;
            terreno.temperatura = 0;
            terreno.jugador = 0;
            terreno.bandidos = 1;

            bandido.icon = ICON_BANDIDO;
            bandido.ultimoMov = '\0';
        }
        else
        {
            //puntos, tormentas, oasis, vidas, salida en terreno
            terreno.icon = icon;
            terreno.temperatura = 0;
            terreno.bandidos = 0;
            terreno.jugador = 0;
        }

        ///terreno cargado
        ponerAlFinalCD(mapa, &terreno, sizeof(terreno));

        ///posicion de bandido (cada uno apunta a su nodo)
        if(terreno.bandidos)
        {
            //anterior al primer nodo = ultimo nodo agregado
            bandido.posActual = (*mapa)->ant;
            //bandido cargado, lo enlistamos en bandidos Gloables
            ponerAlFinal(bGlobales, &bandido, sizeof(bandido));
        }

    }

    ///(quitar) verificos q el txt sea igual a mis nodos
//    verificar(mapa);
    puts("");
//    system("pause");

    fclose(pf);
    return TODO_OK; //Y N BANDIDOS PARA UNA CASASILLA?
}

void procesarTurno(tListaCD *mapa, tJugador *jugador, tCola* colaMovimientos)
{
    int numDado = tirarDado();
    char direccion = 'A';
    tMovimiento movJugador;
    printf("En el dado salio el numero: %d\n", numDado);


    //Aca se deberia verificar que la posicion del jugador le permita ir atras
    //if(PosicionJugador <= numDado)
    puts("Desea Avanzar o retrodecer?(A/R)");
        do
            {
                scanf("%c", &direccion);
                fflush(stdin);
            }while(direccion!='A' && direccion!='R');


        movJugador.posActual = jugador->posActual;
        movJugador.direccion = direccion;
        movJugador.pasos = numDado;
        ponerEnCola(colaMovimientos, &movJugador, sizeof(movJugador));

    //MOVIMIENTO BANDIDOS aca se debe calcular el movimiento de los bandidos y acolar en colaMovimientos

    RealizarMovimiento(jugador, colaMovimientos, mapa);
}

int RealizarMovimiento(tJugador* jugador, tCola* colaMovimientos, tListaCD* mapa)
{
    tMovimiento movArealizar;
    tTerreno *terrenoDeJugador;
    int i;

    sacarDeCola(colaMovimientos, &movArealizar, sizeof(tMovimiento));

    ///sacar jugador de casilla actual
    terrenoDeJugador = (tTerreno*)jugador->posActual->info;
    terrenoDeJugador->jugador = 0;

    for(i = 0; i < movArealizar.pasos; i++)
    {
        if(movArealizar.direccion == 'A')
            jugador->posActual= jugador->posActual->sig;
        else
            jugador->posActual= jugador->posActual->ant;
    }

    ///poner jugador en casilla destino
    terrenoDeJugador = (tTerreno*)jugador->posActual->info;
    terrenoDeJugador->jugador = 1;

    return EXITO;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int tirarDado()
{
    int num = rand()%6+1;
    return num;
}

char iconoDeLinea(char* linea)
{
    //devuelvo el char luego de ':' en caravana txt
    char *reg;

    reg = strchr(linea, ':');

    if(reg == NULL)
        return ERROR_ARCHIVO;

    return *(reg+1);
}

int reservarBandido(tBandido** b)
{
    *b = malloc(sizeof(tBandido));
    if (*b == NULL)
        return SIN_MEMO;

    return EXITO;
}

int verificar(tListaCD *m)
{
    tTerreno terreno;
    int i = 0;


    while(sacarPrimeroListaCD(m, &terreno, sizeof(terreno)) == EXITO)
    {
        i++;

        printf("\n[%d] icon: %c | temp: %d",
               i,
               terreno.icon,
               terreno.temperatura);

        if(terreno.bandidos)
            printf("BANDIDO -> cant: %d\n", terreno.bandidos);

        if(terreno.jugador)
            printf("JUGADOR \n");

    }

    puts("");
    system("pause");

    return TODO_OK;
}

