#include "../constantes.h"
#include "logica.h"
#include <ctype.h>


int verificar(tListaCD *m);
int avance();

int cargarMapa(tListaCD *mapa, tJugador *jugador, int vidasJugador)
{
    FILE *pf = fopen(CARAVANA_ARCH, "rt");
    if(!pf)
        return ERROR_ARCHIVO;

    char linea[TAMLINEA];
    tTerreno terreno;
    tBandido *bandido;

    ///cragamos el primer casillero a la lista
    fgets(linea, sizeof(linea), pf);
    if(strcmpi(linea,"01:[I J]\n") == 0)
    {
        terreno.icon = ICON_INICIO;
        terreno.temperatura = 0;
        terreno.bandido = NULL;
        terreno.jugador = jugador;

        jugador->icon = ICON_JUGADOR;
        jugador->proteccion = 0;
        jugador->turno = 1;
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

    ///cargamos los siguientes casilleros a la lista
    while(fgets(linea, sizeof(linea), pf))
    {
        char *iconoReg;

        iconoReg = strchr(linea, ':');
        if(!iconoReg)
        {
            fclose(pf);
            return ERROR_ARCHIVO;
        }

        iconoReg ++;

        if(*iconoReg == ICON_BANDIDO)
        {
            bandido = malloc(sizeof(tBandido));
            if(bandido == NULL)
            {
                fclose(pf);
                return SIN_MEMO;
            }

            terreno.icon = ICON_PUNTO;
            terreno.temperatura = 0;
            terreno.jugador = NULL;
            terreno.bandido = bandido;

            bandido->icon = ICON_BANDIDO;
            bandido ->ultimoMov = 0;
        }
        else
        {
            terreno.icon = *iconoReg;
            terreno.temperatura = 0;
            terreno.bandido = NULL;
            terreno.jugador = NULL;
        }

        ponerAlFinalCD(mapa, &terreno, sizeof(terreno));

        ///posicion de bandido (cada uno apunta a su nodo)
        if(terreno.bandido)
        {
//            tNodo *ult = *mapa;
//
//            while(ult->sig)
//                ult = ult->sig;

            bandido->posActual = (*mapa)->ant;
        }
    }

    ///(quitar) verificos q el txt sea igual a mis nodos
//    verificar(mapa);
//    puts("");
//    system("pause");

    fclose(pf);
    return TODO_OK; //Y N BANDIDOS PARA UNA CASASILLA?
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

        if(terreno.bandido)
        {
            printf("  BANDIDO -> icon: %c | ultMov: %d\n",
                   terreno.bandido->icon,
                   terreno.bandido->ultimoMov);
        }

        if(terreno.jugador)
        {
            printf("  JUGADOR -> icon: %c | vidas: %d | prot: %d | turno: %d | puntos: %d\n",
                   terreno.jugador->icon,
                   terreno.jugador->vidas,
                   terreno.jugador->proteccion,
                   terreno.jugador->turno,
                   terreno.jugador->puntos);
        }
    }

    puts("");
    system("pause");

    return TODO_OK;
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


        movJugador.posActual=jugador->posActual;
        movJugador.direccion=direccion;
        movJugador.pasos=numDado;
        colaAgregar(colaMovimientos, &movJugador, sizeof(movJugador));

    //MOVIMIENTO BANDIDOS aca se debe calcular el movimiento de los bandidos y acolar en colaMovimientos

    RealizarMovimiento(jugador, colaMovimientos, mapa);

}
int tirarDado()
{
    int num = rand()%6+1;
    return num;
}

int RealizarMovimiento(tJugador* jugador, tCola* colaMovimientos, tListaCD* mapa)
{
    int i;
    tMovimiento movArealizar;

    colaSacar(colaMovimientos, &movArealizar, sizeof(tMovimiento));
    //sacar jugador de casilla actual
    ((tTerreno*)jugador->posActual->info)->jugador = NULL;
    i = 0;
    if(movArealizar.direccion=='A')
    {
         while(i < movArealizar.pasos && jugador->posActual->sig != NULL)
        {
            jugador->posActual= jugador->posActual->sig;
            i++;
        }
    }
    else
    {
          while(i < movArealizar.pasos && jugador->posActual->ant != NULL)
        {
            jugador->posActual= jugador->posActual->ant;
            i++;
        }

    }



    ///poner jugador en nueva casilla
    ((tTerreno*)jugador->posActual->info)->jugador = jugador;

    return EXITO;

}

int avance()
{
    int pasos;

    printf("\nIngresa movimiento [1-6]: ");
    scanf("%d", &pasos);

    while(pasos < 1 || pasos > 6)
        scanf("%d", &pasos);

    return pasos;
}

