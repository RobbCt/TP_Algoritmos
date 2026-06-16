#include "logica.h"

///tabla de temperaturas
#define RANGO_BANDIDOS      6 //casillas a la redonda
#define MEM_V               4
#define CASILLA_CLAVE       15
#define TURNO_ANT_V         30
#define ANTES_V             10
#define NUNCA_V             20
#define MISMA_DIRECCION     20
#define HAY_BANDIDOS        30
#define TEMP_MIN            5

typedef struct
{
    tNodo* destino;
    unsigned pasos;
    char direccion;
    int peso;
}tOpcionMov;



unsigned tirarDado();
char iconoDeLinea(char* linea);
int contarBandidosEnLinea(char *linea);
int verificar(tListaCD *m);
int contarBandidosEnLinea(char *linea);
void cargarMovJugador(tMovimiento *mov, tJugador *jugador);
void cargarMovBandido(tMovimiento *mov, tBandido *bandido, unsigned turno);
int posicionJugador(tJugador* j);



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
        terreno.turnoActualizado = 0;
        terreno.bandidos = 0;
        terreno.jugador = 1;

        jugador->icon = ICON_JUGADOR;
        jugador->proteccion = NO;
        jugador->turno = SI;
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
        int cantB = contarBandidosEnLinea(linea);
        char icon = iconoDeLinea(linea);

        if(icon == ICON_SALIDA && cantB > 0)
        {
            terreno.icon = ICON_SALIDA;
            terreno.turnoActualizado = 0;
            terreno.jugador = 0;
            terreno.bandidos = cantB;
        }
        else if(cantB > 0)
        {
            //bandidos en terreno
            terreno.icon = ICON_PUNTO;
            terreno.turnoActualizado = 0;
            terreno.jugador = 0;
            terreno.bandidos = cantB;

            //bandido.icon = ICON_BANDIDO;
            //bandido.ultimoMov = '\0';
        }
        else
        {
            //puntos, tormentas, oasis, vidas, salida en terreno
            terreno.icon = icon;
            terreno.turnoActualizado = 0;
            terreno.bandidos = 0;
            terreno.jugador = 0;
        }

        ///terreno cargado
        ponerAlFinalCD(mapa, &terreno, sizeof(terreno));

        ///posicion de bandido (cada uno apunta a su nodo)
        if(terreno.bandidos)
        {
            for(int i = 0; i < terreno.bandidos; i++)
            {
                bandido.icon = ICON_BANDIDO;
                bandido.ultimoMov = '\0';
                bandido.posActual = (*mapa)->ant;

                ponerAlFinal(bGlobales, &bandido, sizeof(bandido));
            }
        }

    }

    fclose(pf);
    return TODO_OK;
}

void procesarTurno(tListaCD *mapa, tJugador *jugador, tLista *bGlobales, unsigned turno, tLista* listaMovimientos) //considerando hacerlo int y usar macros para diferentes panoramas
{

    tMovimiento mov;
    tCola colaMovimientos;
    tIteradorLista it;
    tBandido *bandido;

    crearCola(&colaMovimientos);
    if(jugador->turno == SI)
    {
        cargarMovJugador(&mov, jugador);
        ponerAlFinal(listaMovimientos,&mov, sizeof(mov));
//        Sleep(5000);
        ponerEnCola(&colaMovimientos, &mov, sizeof(mov));
    }
    else
    {
        puts("Turno perdido por tormenta");
//        Sleep(5000);
    }

    ///calculo el movimiendo de cada bandido
    bandido = (tBandido*)obtenerPrimeroInfo(bGlobales, &it);
    while(bandido != NULL)
    {
        cargarMovBandido(&mov, bandido, turno);

        ponerEnCola(&colaMovimientos, &mov, sizeof(mov));

        bandido = (tBandido*)obtenerSiguienteInfo(&it);
    }

    realizarMovimientos(jugador, bGlobales, &colaMovimientos, mapa, turno);

    vaciarCola(&colaMovimientos);
}

void IABandidos(tMovimiento *mov, tBandido *bandido, unsigned turnoAc)
{
    /* --------------------------------------------------
        ALGORITMO CALIENTE-FRIO q m invente
        casilla caliente = posible presencia del jugador
        casilla fria  = casilla viglada
    ---------------------------------------------------*/

    tOpcionMov opciones[RANGO_BANDIDOS * 2];
    tNodo *destino;
    int resultado, acum = 0, nCasilla = 0, tempTotal = 0;

    ///AVANZAR Y RECONOCER TERRENO

    //nodo del bandido
    destino = bandido->posActual;

    //mientras iteramos las 6 casillas posteriores, guardamos los datos de cada una
    for(int i = 0; i < RANGO_BANDIDOS; i++)
    {
        destino = destino->sig;

        opciones[nCasilla].destino = destino;
        opciones[nCasilla].pasos = i+1;
        opciones[nCasilla].direccion = 'A'; //posterior-avance

        //con el nodo i posterior al bandido, calculo la temperatura de este terreno
        opciones[nCasilla].peso = calcularTemperaturaMov(bandido, destino, 'A', turnoAc);

        //acumulo el peso de todas las casillas para la ponderacion
        tempTotal += opciones[nCasilla].peso;
        nCasilla++;
    }

    ///RETROCEDER Y RECONOCER TERRENO

    //nodo del bandido
    destino = bandido->posActual;

    //mientras iteramos las 6 casillas anteriores, guardamos los datos de cada una
    for(int i = 0; i < RANGO_BANDIDOS; i++)
    {
        destino = destino->ant;

        opciones[nCasilla].destino = destino;
        opciones[nCasilla].pasos = i+1;
        opciones[nCasilla].direccion = 'R'; //anterior-retrocedo

        //con el nodo i anterior al bandido, calculo la temperatura de este terreno
        opciones[nCasilla].peso = calcularTemperaturaMov(bandido, destino, 'R', turnoAc);

        //acumulo el peso de todas las casillas para la ponderacion
        tempTotal += opciones[nCasilla].peso;
        nCasilla++;
    }

    ///SORTEO PONDERADO

    //resultado aleatorio entre [0, tempTotal - 1]
    resultado = rand() % tempTotal;

    //esta es la forma mas corta (me vi un tiktok)
    for(int i = 0; i < nCasilla; i++)
    {
        acum += opciones[i].peso;

        //acumulo el peso de las casillas para ver en cual cayo el resultado
        if(resultado < acum)
        {
            mov->destino = opciones[i].destino;
            mov->pasos = opciones[i].pasos;
            mov->direccion = opciones[i].direccion;
            return;
        }
    }

    /// si la ponderacion sale mal, elejir la primera opcion
    mov->destino = opciones[0].destino;
    mov->pasos = opciones[0].pasos;
    mov->direccion = opciones[0].direccion;
}

int calcularTemperaturaMov(tBandido *bandido, tNodo *destino, char sentido, unsigned turnoAc)
{
    tTerreno *terrenoAc = (tTerreno*)destino->info;
    int temp = 0;
    unsigned edad;

    /// ----- TEMPERATURA PERSISTENTE -----

    ///+15 si es terreno clave
    if(terrenoAc->icon == 'P' || terrenoAc->icon == 'V' || terrenoAc->icon == 'O')
        temp += CASILLA_CLAVE;

    //si el terreno fue visitado en algun turno...
    if(terrenoAc->turnoActualizado != 0)
    {
        //...vemos hace cuantos turnos sucedio (mas de 4 olvide si fue visitado)
        edad = turnoAc - terrenoAc->turnoActualizado;

        if(edad <= 1)
            temp -= TURNO_ANT_V; ///-30 si fue visitado en el turno anterior
        else if(edad <= MEM_V)
            temp -= ANTES_V; ///-10 si fue visitado (no en turno anterior)
    }
    else
        temp += NUNCA_V; ///+20 si nunca fue visitado

    /// ----- TEMPERATURA TEMPORAL -----

    ///+20 para q tenga inercia y siga le movimiento q tenia el bandido
    if(bandido->ultimoMov == 'A' || bandido->ultimoMov == 'R')
    {
        if(bandido->ultimoMov == sentido)
            temp += MISMA_DIRECCION;
        else
            temp -= MISMA_DIRECCION;
    }

    ///-30 si ya hay bandidos
    if(terrenoAc->bandidos > 0)
        temp -= HAY_BANDIDOS;

    ///=5 temperatura mínima (para q aparezcan en la ponderacion)
    if(temp <= 0)
        temp = TEMP_MIN;

    return temp;
}


int realizarMovimientos(tJugador* j, tLista *bGlobales, tCola* colaMovimientos, tListaCD* mapa, unsigned turno)
{
    tTerreno *terreno = NULL;
    tBandido *bandidoN = NULL;
    tNodoL *dirBandidoN = NULL;
    tIteradorLista itInfoBand;
    tIteradorLista itDirBand;
    tMovimiento movArealizar;
    int protegidoEsteTurno = j->proteccion; //para que la proteccion solo dure un turno
    j->proteccion = NO;

    sacarDeCola(colaMovimientos, &movArealizar, sizeof(tMovimiento));


    if(j->turno == SI)
    {
        ///sacar jugador de casilla actual
        terreno = (tTerreno*)j->posActual->info;
        terreno->jugador = 0;


        ///poner a jugador en casilla destino
        j->posActual = movArealizar.destino;
        terreno = (tTerreno*)j->posActual->info;
        terreno->jugador = 1;

//        //terreno es el destino del jugador
//        if(terreno->tormenta)
//        {
//            if(protegidoEsteTurno == SI)
//            {
//                printf("\nUna tormenta te intercepto, pero al estar protegido no pierdes el turno!");
//            }
//            else
//            {
//                j->turno = NO;
//                terreno->tormenta = 0; //el jugador hace desaparecer la tormenta?
//            }
//        }

        //terreno es el destino del jugador
        switch(terreno->icon)
        {
            case ICON_TORMENTA:

                if(protegidoEsteTurno == SI)
                {
                    printf("\nUna tormenta te intercepto, pero al estar protegido no pierdes el turno!");
                }
                else
                {

                    terreno->icon = ICON_PUNTO; //el jugador hace desaparecer la tormenta?
                    j->turno = NO;
                }


                break;

            case ICON_OASIS:
                j->proteccion = SI;
                terreno->icon = ICON_PUNTO;//el jugador hace desaparecer el oasis?
                break;

            case ICON_PREMIO:
                j->puntos++; //ajustar dependiendo de como funcione el sistema de puntos
                terreno->icon = ICON_PUNTO;
                break;

            case ICON_VIDA:
                j->vidas++;
                terreno->icon = ICON_PUNTO;
                break;

            case ICON_SALIDA:
                return EXITO;

        }

       // case ICON_SALIDA: considero agregar salida como variables



    }
    else
        j->turno = SI;

    bandidoN = (tBandido*)obtenerPrimeroInfo(bGlobales, &itInfoBand);
    dirBandidoN = obtenerPrimerNodo(bGlobales, &itDirBand);
    while(bandidoN != NULL)
    {
        sacarDeCola(colaMovimientos, &movArealizar, sizeof(tMovimiento));

        ///sacar bandido N de lista de casilla actual
        terreno = (tTerreno*)bandidoN->posActual->info;
        terreno->bandidos -= 1;

        ///poner a bandido N de lista en casilla destino
        bandidoN->posActual = movArealizar.destino;
        terreno = (tTerreno*)bandidoN->posActual->info;
        terreno->bandidos += 1;
        terreno->turnoActualizado = turno;

        bandidoN->ultimoMov = movArealizar.direccion;

        //j->posActual->info->bandidos > 0
        if(bandidoN->posActual == j->posActual)
        {
            //en una colison [B J] el bandido muere siempre
            terreno->bandidos -= 1;
            elimDirDeLista(bGlobales, dirBandidoN);

            //q pasa con el jugador?
            if(protegidoEsteTurno == NO)
            {
                j->vidas--;
                j->posActual = *mapa;
                terreno->jugador = 0;
                ((tTerreno*)(*mapa)->info)->jugador = 1;
                printf("\nperdiste una vida pero te cargaste al bandido!");
            }
            else
            {
                printf("\nsobreviviste por la proteccion y te cargaste al bandido!");
            }

            Sleep(5000);
        }

        bandidoN = (tBandido*)obtenerSiguienteInfo(&itInfoBand);
        dirBandidoN = obtenerSiguienteNodo(&itDirBand);
    }

    return EXITO;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned tirarDado()
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

    return *(reg+1) == '[' ? *(reg+2) : *(reg+1);
}

int contarBandidosEnLinea(char* linea)
{
    int i = 0;
    int cant = 0;

    while(*(linea+i))
    {
        if(*(linea+i) == 'B')
            cant++;

        i++;
    }

    return cant;
}

void cargarMovJugador(tMovimiento *mov, tJugador *jugador)
{
    tNodo *posDestino;
    unsigned numDado;
    char direccion;
    int puedeRetroceder;
    char direccionRebote;

    if(jugador->turno != SI)
        return;

    posDestino = jugador->posActual;
    numDado = tirarDado();

    printf("En el dado salio el numero: %u\n", numDado);

    puedeRetroceder = posicionJugador(jugador) >= numDado;

    puts("Desea Avanzar o Retroceder? (A/R)");

    do
    {
        scanf(" %c", &direccion);
        direccion = toupper(direccion);

        //busco q la segunda sentencia del AND sea verdadera su no puede retroceder (solo avance)
        if(direccion != 'A' && !(puedeRetroceder && direccion == 'R'))
            puts("NO ES UNA OPCION VALIDA, REINGRESE");

    }while(direccion != 'A' && !(puedeRetroceder && direccion == 'R'));


    direccionRebote = direccion;

    for(int i = 0; i < numDado; i++)
        {
            tTerreno* terrenoActual = (tTerreno*)posDestino->info;

            if(direccionRebote == 'A')
            {
                if(terrenoActual->icon == ICON_SALIDA)
                {
                    direccionRebote = 'R';
                    posDestino = posDestino->ant;
                }
                else
                {
                    posDestino = posDestino->sig;
                }
            }
            else
            {
                posDestino = posDestino->ant;
            }
        }

    mov->destino = posDestino;
    mov->pasos = numDado;
    mov->direccion = direccion;
}

int posicionJugador(tJugador* j)
{
    int i = 0;

    tNodo* pos = j->posActual;   // rompe encapsulamiento esto?

    tTerreno* terreno = (tTerreno*)pos->info;

    while(terreno->icon != ICON_INICIO)
    {
        pos = pos->ant;
        terreno = (tTerreno*)pos->info;
        i++;
    }

    return i;
}

void cargarMovBandido(tMovimiento *mov, tBandido *bandido, unsigned turnoAc)
{
    IABandidos(mov, bandido, turnoAc);
}




void otorgarPuntosVictoria(tJugador* jugador)
{
    //if(jugador->vidas > 0)
        jugador->puntos += PUNTOS_VICTORIA;
}
