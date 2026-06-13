#include "tablero.h"


int generarTablero(tTablero *tab, char *nArch)
{
    FILE* pf=fopen(nArch, "wt");
    int totalEventos, casillasJugables, bandEnSalida=0, eventosNoBand;

    ///bajamos define's a vector
    char icons[]= {
        ICON_JUGADOR,   //0
        ICON_PUNTO,     //1
        ICON_BANDIDO,   //2
        ICON_PREMIO,    //3
        ICON_VIDA,      //4
        ICON_OASIS,     //5
        ICON_TORMENTA,  //6
        ICON_INICIO,    //7
        ICON_SALIDA     //8
        }; //rand de 1-6

    ///leemos en tab los valores inicales del tablero desde config.txt
    if(!pf || leerConfig(tab, CONFIG_ARCH)!=TODO_OK)
    {
        if(pf) fclose(pf);
        return ERROR_TABLERO;
    }

    //aumentamos las posibilidades de q hayan bandidos en la salida al iniciar partida
    if(tab->cantBand > 0) //3
    {
        if(rand()%2) //xx
        {
            bandEnSalida = rand() % tab->cantBand;
            tab->cantBand -= bandEnSalida;
        }
    }

    ///validamos si es jugable el tablero
    eventosNoBand = tab->cantPrem+ tab->cantPlusV +tab->cantOasis +tab->cantTorm;
    totalEventos=tab->cantBand +eventosNoBand;
    casillasJugables= tab->cantPos -2;

    if(eventosNoBand > casillasJugables)
    {
        fclose(pf);
        return ERROR_TABLERO;
    }

    ///de ser jugable 01:[I J]...
    fprintf(pf, "%02d:[%c %c]\n", 1, icons[7], icons[0]);//seteamos jugador al inicio-> 01:[I J]


    //si los eventos entran uno por casilla, se generan separados y repartidos por zonas..
    if(totalEventos<=casillasJugables)
        generarPorZonas(pf,tab,icons);
    else
        generarPorPaquetes(pf,tab,icons);
    //si hay más eventos que casillas, se generan paquetes empezando por el evento seguido del band

    //seteamos salida al final con la cant de bands calculadas aleatoriamente
    if(bandEnSalida==0)
        fprintf(pf, "%02d:%c\n", tab->cantPos, icons[8]);
    else
    {
        int i;

        fprintf(pf, "%02d:[%c", tab->cantPos, icons[8]);

        for(i=0; i<bandEnSalida; i++)
            fprintf(pf, "%c", icons[CASILLERO_BANDIDO]);

        fprintf(pf, "]\n");
    }

    ///...creamos y asignamos q habra en todos los casilleros del tablero

    ///tablero "caravana.txt" creado
    fclose(pf);
    return TODO_OK;
}

int generarPorZonas(FILE* pf, tTablero* tab, char icons[])
{
    int totalEventos = tab->cantBand +tab->cantPrem +tab->cantPlusV +tab->cantOasis +tab->cantTorm;
    int casillasJugables = tab->cantPos - 2;
    int cantZonas, i, pos;
    int* eventosPorZona;

    //para evitar q la 1ra parte del tablero este llena y la 2da vacia:
    //calculo de cantZonas, algoritmo q la determina para q cada zona tenga una buena distribucion de eventos
    //tab mas chico= menos zonas, tab mas grande= mas zonas (max 10 para no fragmentar tanto el tablero)
    if(casillasJugables <= 0)
        cantZonas= 0;
    else if(casillasJugables < 3)
        cantZonas= casillasJugables;
    else
    {
        cantZonas = casillasJugables/10;

        if(cantZonas<3)
            cantZonas=3;
        if(cantZonas>10)
            cantZonas=10;
    }

    if(cantZonas <= 0)
        return ERROR_TABLERO;

    eventosPorZona = malloc(cantZonas * sizeof(int));

    if(!eventosPorZona)
        return SIN_MEMO;

    ///reparte eventos entre zonas, pa q no se gasten todos al principio
    for(i = 0; i < cantZonas; i++)
        eventosPorZona[i] = ((i + 1) * totalEventos) / cantZonas - (i * totalEventos) / cantZonas;

    for(pos = 2; pos < tab->cantPos; pos++)
    {
        int zona = obtenerZonaDePos(pos, tab->cantPos, cantZonas);

        int inicioZona, finZona, casillasRestantesZona, evento;

        calcularRangoZona(zona, tab->cantPos, cantZonas, &inicioZona, &finZona);

        ///casillas q quedan por recorrer dentro de esta zona incluyendocla pos actual
        casillasRestantesZona = finZona - pos + 1;

        //si la zona quedó sin eventos, recien entra puntos
        if(eventosPorZona[zona] <= 0)
            fprintf(pf, "%02d:%c\n", pos, icons[CASILLERO_PUNTO]);
        else
        {
            ///probabilidad controlada:
            //mientras + eventos falten en la zona y - casillas queden, + probable es poner evento
            if(rand() % casillasRestantesZona < eventosPorZona[zona])
            {
                evento = selecEvento(tab);

                if(evento != -1)
                {
                    fprintf(pf, "%02d:%c\n", pos, icons[evento]);
                    eventosPorZona[zona]--;
                }
                else
                    fprintf(pf, "%02d:%c\n", pos, icons[CASILLERO_PUNTO]);
            }
            else
                fprintf(pf, "%02d:%c\n", pos, icons[CASILLERO_PUNTO]);
        }
    }

    free(eventosPorZona);

    return TODO_OK;
}

void generarPorPaquetes(FILE* pf, tTablero* tab, char icons[])
{
    int pos;

    for(pos = 2; pos < tab->cantPos; pos++)
    {
        int casillasRestantes = tab->cantPos - pos;
        int eventosNoBandRestantes=tab->cantPrem +tab->cantPlusV +tab->cantOasis +tab->cantTorm;
        int eventosRestantes =tab->cantBand +eventosNoBandRestantes;

        int cantEventosEnCasilla, resto, i;

        //si ya no quedaran eventos, dejamos un punto
        if(eventosRestantes <= 0)
            fprintf(pf, "%02d:%c\n", pos, icons[CASILLERO_PUNTO]);
        else
        {
            //si las casillas son exactamente igual a la cant de eventos
            //ponemos de a uno en cada casilla para evitar el solape
            if(casillasRestantes==eventosNoBandRestantes)///caso especial
            {
                //obligamos un item noB y reparto bands
                cantEventosEnCasilla=1+ (tab->cantBand /casillasRestantes);

                //lo sobrante se distribuye de forma aleatoria
                int restB=tab->cantBand %casillasRestantes;
                if(restB>0 && rand()%casillasRestantes <restB)
                    cantEventosEnCasilla++;
            }
            else///reparto normal
            {
                //cant min de eventos q se debe poner en esta casilla para q entren todos antes de la salida
                cantEventosEnCasilla = eventosRestantes / casillasRestantes;
                resto = eventosRestantes % casillasRestantes;

                //resto de eventos q no entran en la div excta
                //a veces agrega uno extra para repartir el resto¿
                if(resto > 0 && rand() % casillasRestantes < resto)
                    cantEventosEnCasilla++;
            }

            int totalParaImprimir= cantEventosEnCasilla;//total original planificado pa los []
            int itemNoB= -1; //-1-> q no se selecciono evento especial

            //decide si la casilla recibe un item noB
            if(eventosNoBandRestantes > 0 &&
              ((casillasRestantes == eventosNoBandRestantes) ||
               (rand() % casillasRestantes < eventosNoBandRestantes)))
                itemNoB= selecEventoNoBandido(tab);

            //el resto de lugares va para los bands
            //si sacamos un item noB los bands ocuparan el espacio restante (cantEventos-1)
            int cantBandidos= cantEventosEnCasilla- (itemNoB!=-1? 1:0);

            //evitan q se asignen mas bands de los q queden
            if(cantBandidos > tab->cantBand)
                cantBandidos=tab->cantBand;

            if(cantBandidos< 0)
                cantBandidos= 0;

            tab->cantBand-= cantBandidos;

            //grabamos el .txt
            fprintf(pf, "%02d:", pos);

            //si a final quedo vacia va un puntito
            if(cantBandidos == 0 && itemNoB == -1)
                fprintf(pf, "%c\n", icons[CASILLERO_PUNTO]);
            else
            {
                //total original calculado para saber si abrimos []
                if(totalParaImprimir > 1)
                    fprintf(pf, "[");

                //el item noB se imprime siempre primero si existe..
                if(itemNoB!=-1)
                    fprintf(pf, "%c", icons[itemNoB]);

                //luego van los bands si es q estan
                for(i=0;i<cantBandidos;i++)
                    fprintf(pf, "%c", icons[CASILLERO_BANDIDO]);

                if(totalParaImprimir>1)
                    fprintf(pf, "]");

                fprintf(pf, "\n");
            }
        }
    }
}

void calcularRangoZona(int zona, int cantPos, int cantZonas, int* inicio, int* fin)
{
    //zonas incluyen solo casillas jugables, se setea despues del inicio y antes de salida
    int primeraJugable =2;
    int casillasJugables = cantPos-2;

    int tamZona = casillasJugables/cantZonas;
    int resto = casillasJugables %cantZonas;

    int i;

    *inicio = primeraJugable;

    //se reparte casilla "sobrante" en el caso de q la division no de exacra uwu
    for(i=0; i<zona; i++)
    {
        *inicio +=tamZona;

        if(i<resto)
            (*inicio)++;
    }

    int largoZona = tamZona;

    if(zona<resto)
        largoZona++;

    *fin = *inicio +largoZona-1;
}

int obtenerZonaDePos(int pos, int cantPos, int cantZonas)
{
    int zona, inicio, fin;

    for(zona=0; zona<cantZonas; zona++)
    {
        calcularRangoZona(zona, cantPos, cantZonas, &inicio, &fin);

        if(pos>=inicio && pos<=fin)
            return zona;
    }

    return cantZonas-1;
}

//mi nuevo valRandom()¿
int selecEvento(tTablero* tab)
{
    int total = tab->cantBand +tab->cantPrem +tab->cantPlusV +tab->cantOasis +tab->cantTorm;
    int random;

    if(total <= 0)
        return -1;

    //se toma las cantidades restantes de cada evento
    //ej, si quedan mas bandidos qpremios, hay mas probabilidad de q salga un bandido¿

    random = rand() % total;

    if(random < tab->cantBand)
    {
        tab->cantBand--;
        return CASILLERO_BANDIDO;
    }

    random -= tab->cantBand;

    if(random < tab->cantPrem)
    {
        tab->cantPrem--;
        return CASILLERO_PREMIO;
    }

    random -= tab->cantPrem;

    if(random < tab->cantPlusV)
    {
        tab->cantPlusV--;
        return CASILLERO_VIDA;
    }

    random -= tab->cantPlusV;

    if(random < tab->cantOasis)
    {
        tab->cantOasis--;
        return CASILLERO_OASIS;
    }

    tab->cantTorm--;
    return CASILLERO_TORMENTA;
}

int selecEventoNoBandido(tTablero* tab)
{
    int totalNoBand = tab->cantPrem + tab->cantPlusV + tab->cantOasis + tab->cantTorm;

    if (totalNoBand <= 0)
        return -1;

    int random = rand() % totalNoBand;

    if (random < tab->cantPrem)
    {
        tab->cantPrem--;
        return CASILLERO_PREMIO;
    }

    random -= tab->cantPrem;
    if (random < tab->cantPlusV)
    {
        tab->cantPlusV--;
        return CASILLERO_VIDA;
    }

    random -= tab->cantPlusV;
    if (random < tab->cantOasis)
    {
        tab->cantOasis--;
        return CASILLERO_OASIS;
    }

    tab->cantTorm--;
    return CASILLERO_TORMENTA;
}
