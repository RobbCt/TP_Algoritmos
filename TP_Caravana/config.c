#include "config.h"


//// trabajo con archivos .txt
int leerConfig(tTablero* tab, char* nArch)
{
    FILE* pf= fopen(nArch,"rt");
    if(!pf)
    {
        perror("msg");
        return ERROR_ARCHIVO;
    }

    char linea[TAMLINEA];
    fgets(linea, TAMLINEA, pf);

    char *fin = strchr(linea, ':');

    tab->cantPos = atoi(fin+1); //cant_posiciones

    fgets(linea, TAMLINEA, pf);
    fin=strchr(linea, ':');

    tab->vidasInicio=atoi(fin+1); //cant_vidas

    fgets(linea, TAMLINEA, pf);
    fin=strchr(linea, ':');

    tab->cantBand =atoi(fin+1); //cant_bandidos

    fgets(linea, TAMLINEA, pf);
    fin=strchr(linea, ':');

    tab->cantPrem =atoi(fin+1); //cant_premios

    fgets(linea, TAMLINEA, pf);
    fin=strchr(linea, ':');

    tab->cantPlusV =atoi(fin+1); //vidas_extras

    fgets(linea, TAMLINEA, pf);
    fin=strchr(linea, ':');

    tab->cantOasis =atoi(fin+1); //cant_oasis

    fgets(linea, TAMLINEA, pf);
    fin=strchr(linea, ':');

    tab->cantTorm=atoi(fin+1); //cant_tormentas

    fclose(pf);
    return TODO_OK;
}
