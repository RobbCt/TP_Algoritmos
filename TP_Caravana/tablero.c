#include "tablero.h"


int generarTablero( tTablero*tab, char* nArch)
{
    FILE* pf=fopen(nArch, "wt");
    int i=1;

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

    if(!pf || leerConfig(tab, CONFIG_ARCH)!=TODO_OK)
    {
        if(pf) fclose(pf);
        return ERROR_TABLERO;
    }
    int cantPuntos= tab->cantPos - tab->cantBand - tab->cantOasis - tab->cantPlusV - tab->cantPrem - tab->cantTorm - 2;
    int iconAnt=-1;

    if(cantPuntos<0)
    {
        puts("config.txt no aceptable para el tablero, modifiquelo y reintente...");
        fclose(pf);
        return ERROR_TABLERO;
    }

    fprintf(pf, "%02d:[%c %c]\n", i, icons[7], icons[0]);//seteamos jugador al inicio-> 01:[I J]
    i++;

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
    fprintf(pf, "%02d:%c\n", i, icons[8]);//seteamos salida al final

    fclose(pf);
    return TODO_OK;
}

int valRandom(tTablero* tab, int posiciones, int anterior)
{
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
