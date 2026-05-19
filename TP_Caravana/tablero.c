#include "tablero.h"


int generarTablero(tTablero *tab, char *nArch)
{
    FILE* pf=fopen(nArch, "wt");
    int totalEventos, casillasJugables;

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

    ///validamos si es jugable el tablero
    totalEventos=tab->cantBand +tab->cantPrem +tab->cantPlusV +tab->cantOasis +tab->cantTorm;
    casillasJugables= tab->cantPos-2;

    ///de ser jugable 01:[I J]...
    fprintf(pf, "%02d:[%c %c]\n", 1, icons[7], icons[0]);//seteamos jugador al inicio-> 01:[I J]


    //si los eventos entran uno por casilla, se generan separados y repartidos por zonas..
    if(totalEventos<=casillasJugables)
        generarPorZonas(pf,tab,icons);
    else
        generarPorPaquetes(pf,tab,icons);
    //si hay más eventos que casillas, se generan paquetes [BB, VT, BP, etc] - prox validar combos posible¿

    fprintf(pf, "%02d:%c\n", tab->cantPos, icons[8]);//seteamos salida al final

/*
    if(cantPuntos<0) //juntamos combos al azar
    {
        puts("config.txt no aceptable para el tablero, modifiquelo y reintente...");
        fclose(pf);
        return ERROR_TABLERO;
    }
*/
    ///...creamos y asignamos q habra en todos los casilleros del tablero
    /*
    while(i<tab->cantPos)
    {
        int random= valRandom(tab, cantPuntos, iconAnt);

        switch(random)
        {
            case CASILLERO_PUNTO: cantPuntos--; break;
            case CASILLERO_BANDIDO: tab->cantBand--; break;
            case CASILLERO_PREMIO: tab->cantPrem--;  break;
            case CASILLERO_VIDA: tab->cantPlusV--; break;
            case CASILLERO_OASIS: tab->cantOasis--; break;
            case CASILLERO_TORMENTA: tab->cantTorm--; break;
        }

        iconAnt=random;

        fprintf(pf, "%02d:%c\n", i, icons[random]);
        i++;
    }

    ///de ser terminado 0n:S
    fprintf(pf, "%02d:%c\n", i, icons[8]);//seteamos salida al final
*/
    ///tablero "caravana.txt" creado
    fclose(pf);
    return TODO_OK;
}
/*
int valRandom(tTablero* tab, int posiciones, int anterior)
{
    ///se itera aleatoriamente hasta elegir q hay en cada casillero
    ///(respetando cantidades y secuencias)
    int random, valido;

    do
    {
        valido=1;
        random = rand()%6+1;

        switch(random)
        {
            case CASILLERO_PUNTO:
                if(posiciones<=0)
                    valido=0;
                break;
            case CASILLERO_BANDIDO:
                if(tab->cantBand <= 0 ||  ((anterior==CASILLERO_BANDIDO || anterior==CASILLERO_TORMENTA) && posiciones > 0))
                    valido=0;
                break;
            case CASILLERO_PREMIO:
                if(tab->cantPrem <= 0 || (anterior!=CASILLERO_PUNTO && posiciones!=0))
                    valido=0;
                break;
            case CASILLERO_VIDA:
                if(tab->cantPlusV <= 0 || (anterior!=CASILLERO_PUNTO && posiciones!=0))
                    valido=0;
                break;
            case CASILLERO_OASIS:
                if(tab->cantOasis <= 0 || (anterior!=CASILLERO_PUNTO && posiciones!=0))
                    valido=0;
                break;
            case CASILLERO_TORMENTA:
                if(tab->cantTorm <= 0 || ((anterior==CASILLERO_BANDIDO || anterior==CASILLERO_TORMENTA) && posiciones > 0))
                    valido=0;
                break;
        }
    }while(!valido);

    return random;
}
*/

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
        int eventosRestantes =tab->cantBand +tab->cantPrem +tab->cantPlusV +tab->cantOasis +tab->cantTorm;
        int casillasRestantes = tab->cantPos - pos;

        int cantEventosEnCasilla, resto, i;

        if(eventosRestantes <= 0)
            fprintf(pf, "%02d:%c\n", pos, icons[CASILLERO_PUNTO]);
        else
        {
            //cant min de eventos q se debe poner en esta casilla para q entren todos antes de la salida
            cantEventosEnCasilla = eventosRestantes / casillasRestantes;
            resto = eventosRestantes % casillasRestantes;

            //resto de eventos q no entran en la div excta
            //a veces agrega uno extra para repartir el resto¿
            if(resto > 0 && rand() % casillasRestantes < resto)
                cantEventosEnCasilla++;

            fprintf(pf, "%02d:", pos);

            //solo si hay mas de un evento ponemos nuestros corchetes
            if(cantEventosEnCasilla > 1)
                fprintf(pf, "[");

            for(i = 0; i < cantEventosEnCasilla; i++)
            {
                int evento = selecEvento(tab);

                if(evento != -1)
                    fprintf(pf, "%c", icons[evento]);
            }

            if(cantEventosEnCasilla > 1)
                fprintf(pf, "]");

            fprintf(pf, "\n");
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
