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
    tNodoCD* destino;
    unsigned pasos;
    char direccion;
    int temperatura;
}tOpcionMov;



int cargarTerrenoInicioDesdeArch(FILE *pf, tJugador *jugador, tListaCD *mapa);
int cargarTerrenosSiguientesDesdeArch(FILE *pf, tLista *bGlobales, tListaCD *mapa);
int cargarMovJugador(tListaCD *mapa, tJugador *jugador, tCola *colaMov, tLista *listaMovPartida);
int cargarMovBandidos(tListaCD *mapa, tLista *bGlobales, tCola *colaMovimientos, unsigned turno);
int realizarMovimientos(tJugador* j, tLista *bGlobales, tCola* colaMovimientos, tListaCD* mapa, unsigned turno);



char iconoDeLinea(char* linea);
int contarBandidosEnLinea(char *linea);
char ingresoDireccionDeMov(tListaCD *mapa, tJugador *jugador, unsigned numDado);
unsigned tirarDado();
int esMovValido(char direccion, int puedeRetroceder);
int destinoDeJugador(tMovimiento *mov, tJugador *jugador, tListaCD *mapa, unsigned numDado, char direccion);
void destinoDeBandido(tMovimiento *mov, tBandido *bandido, tListaCD *mapa, unsigned turnoAc);
void IABandidos(tMovimiento *mov, tBandido *bandido, tListaCD *mapa, unsigned turnoAc);
int calcularTemperaturaMov(tBandido *bandido, tTerreno *terrenoAc, char sentido, unsigned turnoAc);
int moverJugador(tJugador *jugador, tMovimiento *mov, char protegido);
int moverBandido(tBandido *bandido, tMovimiento *mov, unsigned turno);
int colisionJugadorBandido(tIteradorLista *itBandidos, tLista *bGlobales, tBandido *bandido, tJugador *jugador, tListaCD *mapa, char protegido);
int despejarInicio(tLista *bGlobales, tListaCD *mapa, tJugador *jugador);


int cargarMapa(tListaCD *mapa, tJugador *jugador, tLista *bGlobales)
{
    //cargo el mapa con los datos del tablero en CARAVANA.txt

    //quitar despues del push de dai
    FILE *pf = fopen(CARAVANA_ARCH, "rt");
    if(!pf)
        return ERROR_ARCHIVO;

//    FILE *pf;
//    abrirArchivo(&pf, CARAVANA_ARCH, "rt");

    cargarTerrenoInicioDesdeArch(pf, jugador, mapa);

    cargarTerrenosSiguientesDesdeArch(pf, bGlobales, mapa);

    fclose(pf);
    return TODO_OK;
}

void procesarTurno(tListaCD *mapa, tJugador *jugador, tLista *bGlobales, unsigned turno, tLista* listaMovPartida) //considerando hacerlo int y usar macros para diferentes panoramas
{
    //encolo los movimientos de jugador y cada bandido
    tCola colaMovimientos;

    crearCola(&colaMovimientos);

    cargarMovJugador(mapa, jugador, &colaMovimientos, listaMovPartida);

    cargarMovBandidos(mapa, bGlobales, &colaMovimientos, turno);

    realizarMovimientos(jugador, bGlobales, &colaMovimientos, mapa, turno);

    //para q no hayan bandidos cuando jugador vuelva a inicio
    despejarInicio(bGlobales, mapa, jugador);

    vaciarCola(&colaMovimientos);
}

///SUB-PROCESOS///

int cargarTerrenoInicioDesdeArch(FILE *pf, tJugador *jugador, tListaCD *mapa)
{
    ///cragamos el primer casillero a la listaCD
    tTerreno terreno;
    char linea[TAMLINEA];

    fgets(linea, sizeof(linea), pf);

    if(strcmpi(linea,"01:[I J]\n") == 0)
    {
        terreno.icon = ICON_INICIO;
        terreno.turnoActualizado = 0;
        terreno.bandidos = 0;
        terreno.jugador = 1;
        //vidas ya cargadas

        jugador->icon = ICON_JUGADOR;
        jugador->proteccion = NO;
        jugador->turno = SI;
        jugador->puntos = 0;

        ponerAlFinalCD(mapa, &terreno, sizeof(terreno));

        jugador->posActual = obtenerPrimerNodoListaCD(mapa);

    }
    else
        return ERROR_TABLERO;

    return TODO_OK;
}

int cargarTerrenosSiguientesDesdeArch(FILE *pf, tLista *bGlobales, tListaCD *mapa)
{
    //cargamos los demas casilleros a la listaCD
    tTerreno terreno;
    tBandido bandido;
    char linea[TAMLINEA];

    //itero hasta terminar CARAVANA.txt
    while(fgets(linea, sizeof(linea), pf))
    {
        //cargo terreno
        terreno.icon = iconoDeLinea(linea);
        terreno.turnoActualizado = 0;
        terreno.bandidos = contarBandidosEnLinea(linea);
        terreno.jugador = 0;

        //terreno en listaCD
        ponerAlFinalCD(mapa, &terreno, sizeof(terreno));

        if(terreno.bandidos)
        {
            //cargo bandidos en lista (cada uno apunta a su nodo)
            for(int i = 0; i < terreno.bandidos; i++)
            {
                bandido.icon = ICON_BANDIDO;
                bandido.ultimoMov = '\0';
                bandido.posActual = obtenerUltimoNodoListaCD(mapa);

                ponerAlFinalLista(bGlobales, &bandido, sizeof(bandido));
            }
        }
    }

    return TODO_OK;
}

int cargarMovJugador(tListaCD *mapa, tJugador *jugador, tCola *colaMov, tLista *listaMovPartida)
{
    //carga el mov de jugador en la cola de movimientos y la lista de resumen de movimientos
    tMovimiento mov;
    char direccion;
    unsigned numDado = tirarDado();

    if(jugador->turno == NO)
    {
        puts("La tormenta te atrapo, pierdes un turno!");
        //Sleep(5000);
        return TODO_OK;
    }

    printf("\nTiraste el dado y salio el numero %u\n", numDado);
    printf("\nDesea Avanzar o Retroceder? [A - R]: ");

    direccion = ingresoDireccionDeMov(mapa, jugador, numDado);

    //cargo la variable tMovimiento mov con sus valores finales
    destinoDeJugador(&mov, jugador, mapa, numDado, direccion);

    ponerAlFinalLista(listaMovPartida, &mov, sizeof(mov));
    ponerEnCola(colaMov, &mov, sizeof(mov));

    return TODO_OK;
}

int cargarMovBandidos(tListaCD *mapa, tLista *bGlobales, tCola *colaMovimientos, unsigned turno)
{
    //calculo el sigueinte movimiendo de cada bandido (si existen)
    tIteradorLista itBandidos;
    tMovimiento mov;
    tBandido bandido;

    if(iniciarPrimeroItLista(&itBandidos, bGlobales) == L_EXITO)
    {
        do{
            verActualItLista(&itBandidos, &bandido, sizeof(bandido));

            destinoDeBandido(&mov, &bandido, mapa, turno);

            ponerEnCola(colaMovimientos, &mov, sizeof(mov));

        }while(avanzarItLista(&itBandidos) == L_EXITO);

    }

    return TODO_OK;
}

int realizarMovimientos(tJugador* jugador, tLista *bGlobales, tCola* colaMovimientos, tListaCD* mapa, unsigned turno)
{
    tIteradorLista itBandidos;
    tBandido bandido;
    tMovimiento movArealizar;
    char protegidoEsteTurno = jugador->proteccion;

    jugador->proteccion = NO;//para que la proteccion solo dure un turno

    sacarDeCola(colaMovimientos, &movArealizar, sizeof(movArealizar));

    if(moverJugador(jugador, &movArealizar, protegidoEsteTurno) == FIN_PARTIDA)
        return TODO_OK;

    //mover a cada bandido de la lista bGlobales (si hay movimientos)
    if(iniciarPrimeroItLista(&itBandidos, bGlobales) == L_EXITO)
    {

        while(esValidoItLista(&itBandidos))
        {
            sacarDeCola(colaMovimientos, &movArealizar, sizeof(tMovimiento));

            //recupero un bandido de lista y lo actualizo (lo muevo)
            verActualItLista(&itBandidos, &bandido, sizeof(bandido));
            moverBandido(&bandido, &movArealizar, turno);
            modificarActualItLista(&itBandidos, &bandido, sizeof(bandido));


            if(jugador->posActual == bandido.posActual)
                colisionJugadorBandido(&itBandidos, bGlobales, &bandido, jugador, mapa, protegidoEsteTurno);
            else
                avanzarItLista(&itBandidos);

            //si un bandido murio y se saco de la lista
            //ya se avanzo
        }
    }

    return TODO_OK;
}

///HELPERS///

char iconoDeLinea(char* linea)
{
    char *reg;

    reg = strchr(linea, ':');

    //si no hay ':'
    if(reg == NULL)
        return ERROR_ARCHIVO;

    //despues de ':'
    reg++;

    //buscamos el primer item inmovil despues de ':'
    while(*reg)
    {
        if(*reg == ICON_PUNTO || *reg == ICON_PREMIO ||
           *reg == ICON_VIDA || *reg == ICON_OASIS ||
           *reg == ICON_TORMENTA || *reg == ICON_SALIDA)
        {
            return *reg;
        }
        reg++;
    }

    //si no hay item, retorna '.' por defecto
    return ICON_PUNTO;
}

int contarBandidosEnLinea(char* linea)
{
    //cuenta cuantos chars 'B' hay en la cadena a partir de ':'
    char *reg = strchr(linea, ':');
    int cant = 0;

    if(reg == NULL)
        return 0;

    reg++;

    while(*reg)
    {
        if(*reg == 'B')
            cant++;

        reg++;
    }

    return cant;
}

char ingresoDireccionDeMov(tListaCD *mapa, tJugador *jugador, unsigned numDado)
{
    int puedeRetroceder = posEnListaCD(mapa, jugador->posActual) >= numDado;
    char direccion;

    scanf(" %c", &direccion);
    direccion = toupper(direccion);

    while(!esMovValido(direccion, puedeRetroceder))
    {
        printf("\nNO ES UNA OPCION VALIDA, REINGRESE: ");
        scanf(" %c", &direccion);
        direccion = toupper(direccion);
    }

    return direccion;
}

unsigned tirarDado()
{
    int num = rand()%Ns_DADO + 1;
    return num;
}

int esMovValido(char direccion, int puedeRetroceder)
{
    if(direccion == AVANZAR)
        return 1;  //avanzar siempre es valido

    if(direccion == RETROCEDER && puedeRetroceder)
        return 1;  //retroceder solo si puede

    return 0;  //cualquier otra combinacion inválida
}

int destinoDeJugador(tMovimiento *mov, tJugador *jugador, tListaCD *mapa, unsigned numDado, char direccion)
{
    tTerreno terrDestino;
    tIteradorCD itMapa;
    char direccionRebote = direccion;

    //inicio el movimiento en la casilla del juagador
    iniciarNodoItCD(&itMapa, jugador->posActual);

    //calculo el destino tomando en cuenta el rebote de la salida
    for(int i = 0; i < numDado; i++)
    {
        verActualItCD(&itMapa, &terrDestino, sizeof(terrDestino));

        if(direccionRebote == AVANZAR)
        {
            if(terrDestino.icon == ICON_SALIDA)
            {
                direccionRebote = RETROCEDER;
                retrocederItCD(&itMapa);
            }
            else
                avanzarItCD(&itMapa);
        }
        else
            retrocederItCD(&itMapa);
    }

    //asigno el destino
    mov->destino = verNodoActualItCD(&itMapa);
    mov->pasos = numDado;
    mov->direccion = direccion;

    return TODO_OK;
}

void destinoDeBandido(tMovimiento *mov, tBandido *bandido, tListaCD *mapa, unsigned turnoAc)
{
    IABandidos(mov, bandido, mapa, turnoAc);
}

void IABandidos(tMovimiento *mov, tBandido *bandido, tListaCD *mapa, unsigned turnoAc)
{
    /* --------------------------------------------------
                        CALIENTE-FRIO
        casilla caliente = posible presencia del jugador
        casilla fria  = casilla viglada
    ---------------------------------------------------*/

    tOpcionMov opciones[RANGO_BANDIDOS * 2];
    tIteradorCD itMapa;
    tTerreno terrenoAct;
    int resultado, acum = 0, nCasilla = 0, tempTotal = 0;

    ///AVANZAR Y RECONOCER TERRENO

    //nodo del bandido
    iniciarNodoItCD(&itMapa, bandido->posActual);

    //mientras iteramos las 6 casillas posteriores, guardamos los datos de cada una
    for(int i = 0; i < RANGO_BANDIDOS; i++)
    {
        avanzarItCD(&itMapa);

        opciones[nCasilla].destino = verNodoActualItCD(&itMapa);
        opciones[nCasilla].pasos = i+1;
        opciones[nCasilla].direccion = AVANZAR; //posterior-avance

        //obtengo el terreno posterior
        verActualItCD(&itMapa, &terrenoAct, sizeof(terrenoAct));

        //con el terreno i posterior al bandido, calculo la temperatura del mismo
        opciones[nCasilla].temperatura = calcularTemperaturaMov(bandido, &terrenoAct, AVANZAR, turnoAc);

        //acumulo la temperatura de todas las casillas para la ponderacion
        tempTotal += opciones[nCasilla].temperatura;
        nCasilla++;
    }



    reiniciarItCD(&itMapa);



    ///RETROCEDER Y RECONOCER TERRENO

    //nodo del bandido
    iniciarNodoItCD(&itMapa, bandido->posActual);

    //mientras iteramos las 6 casillas anteriores, guardamos los datos de cada una
    for(int i = 0; i < RANGO_BANDIDOS; i++)
    {
        retrocederItCD(&itMapa);

        opciones[nCasilla].destino = verNodoActualItCD(&itMapa);
        opciones[nCasilla].pasos = i+1;
        opciones[nCasilla].direccion = RETROCEDER; //anterior-retrocedo

        //obtengo el terreno anterior
        verActualItCD(&itMapa, &terrenoAct, sizeof(terrenoAct));

        //con el terreno i anterior al bandido, calculo la temperatura del mismo
        opciones[nCasilla].temperatura = calcularTemperaturaMov(bandido, &terrenoAct, RETROCEDER, turnoAc);

        //acumulo la temperatura de todas las casillas para la ponderacion
        tempTotal += opciones[nCasilla].temperatura;
        nCasilla++;
    }

    ///SORTEO PONDERADO

    //resultado aleatorio entre [0, tempTotal - 1]
    resultado = rand() % tempTotal;

    //esta es la forma mas corta (me vi un tiktok)
    for(int i = 0; i < nCasilla; i++)
    {
        acum += opciones[i].temperatura;

        //acumulo el peso de las casillas para ver en cual cayo el resultado
        if(resultado < acum)
        {
            mov->destino = opciones[i].destino;
            mov->pasos = opciones[i].pasos;
            mov->direccion = opciones[i].direccion;
            return;
        }
    }
}

int calcularTemperaturaMov(tBandido *bandido, tTerreno *terrenoAc, char sentido, unsigned turnoAc)
{
    int temp = 0;
    unsigned edad;

    /// ----- TEMPERATURA PERSISTENTE -----

    ///+ si es terreno clave
    if(terrenoAc->icon == ICON_PREMIO || terrenoAc->icon == ICON_VIDA || terrenoAc->icon == ICON_OASIS)
        temp += CASILLA_CLAVE;

    //si el terreno fue visitado en algun turno...
    if(terrenoAc->turnoActualizado != 0)
    {
        //...vemos hace cuantos turnos sucedio (mas de 4 olvide si fue visitado)
        edad = turnoAc - terrenoAc->turnoActualizado;

        if(edad <= 1)
            temp -= TURNO_ANT_V; ///- si fue visitado en el turno anterior
        else if(edad <= MEM_V)
            temp -= ANTES_V; ///- si fue visitado (no en turno anterior)
    }
    else
        temp += NUNCA_V; ///+ si nunca fue visitado

    /// ----- TEMPERATURA TEMPORAL -----

    ///+ para q tenga inercia y siga le movimiento q tenia el bandido
    if(bandido->ultimoMov == AVANZAR || bandido->ultimoMov == RETROCEDER)
    {
        if(bandido->ultimoMov == sentido)
            temp += MISMA_DIRECCION;
        else
            temp -= MISMA_DIRECCION;
    }

    ///- si ya hay bandidos
    if(terrenoAc->bandidos > 0)
        temp -= HAY_BANDIDOS;

    ///= temperatura mínima (para q aparezcan en la ponderacion)
    if(temp <= 0)
        temp = TEMP_MIN;

    return temp;
}

int moverJugador(tJugador *jugador, tMovimiento *mov, char protegido)
{
    tIteradorCD itMapa;
    tTerreno terreno;

    if(jugador->turno == NO)
    {
        jugador->turno = SI;
        return TODO_OK;
    }

    //recupero el terreno donde esta el jugador
    iniciarNodoItCD(&itMapa, jugador->posActual);
    verActualItCD(&itMapa, &terreno, sizeof(terreno));

    //le quito el juagador
    terreno.jugador = 0;

    //actualizo el terreno en el mapa
    modificarActualItCD(&itMapa, &terreno, sizeof(terreno));

    //MUEVO EL JUGADOR
    jugador->posActual = mov->destino;
    jugador->puntos+= mov->pasos; //1 punto por cada casilla

    //recupero el terreno donde esta el jugador
    iniciarNodoItCD(&itMapa, jugador->posActual);
    verActualItCD(&itMapa, &terreno, sizeof(terreno));

    //le anado el jugador y...
    terreno.jugador = 1;

    //...modifico el terreno de la colision...
    switch(terreno.icon)
    {
        case ICON_TORMENTA:
            if(protegido == SI)
                {
                    printf("\n%cUna tormenta te intercept%c, pero al estar protegido no pierdes el turno!",173, 162);
                    //no suma ni pierde puntos
                    Sleep(5000);
                }
            else
                {
                    printf("\n%cUna tormenta te intercept%c, pierdes el turno!",173, 162);
                    jugador->puntos-=15;
                    terreno.icon = ICON_PUNTO;//el jugador hace desaparecer la tormenta
                    jugador->turno = NO;
                    Sleep(5000);
                }
        break;

        case ICON_OASIS:
            terreno.icon = ICON_PUNTO;//el jugador hace desaparecer el oasis
            printf("\nEl jugador ha ca%cdo en un Oasis, en el siguiente turno tendr%c protecci%cn!", 161, 160, 162);
            jugador->puntos+=10;
            jugador->proteccion = SI;
            Sleep(5000);
        break;

        case ICON_PREMIO:
            terreno.icon = ICON_PUNTO;
            printf("\n%cEnhorabuena!, el jugador ha sumado 25 puntos.", 173);
            jugador->puntos+=25;
            Sleep(5000);
        break;

        case ICON_VIDA:
            terreno.icon = ICON_PUNTO;
            printf("\n%cEl jugador ha conseguido una vida!", 173);
            jugador->puntos+=15;
            jugador->vidas++;
            Sleep(5000);
        break;

        case ICON_SALIDA:
            printf("\n%cHas ganado la partida!", 173);
            jugador->puntos+=100;
            return FIN_PARTIDA;
        }

    //actualizo el terreno en el mapa
    modificarActualItCD(&itMapa, &terreno, sizeof(terreno));

    return TODO_OK;
}

int moverBandido(tBandido *bandido, tMovimiento *mov, unsigned turno)
{
    //muevo la posicion del bandido y actualizo sus casilleros
    tIteradorCD itMapa;
    tTerreno terreno;

    //recupero terreno donde esta bandido
    iniciarNodoItCD(&itMapa, bandido->posActual);
    verActualItCD(&itMapa, &terreno, sizeof(terreno));

    //le quito el bandido
    terreno.bandidos -= 1;

    //actualizo el terreno del mapa
    modificarActualItCD(&itMapa, &terreno, sizeof(terreno));

    //MUEVO EL BANDIDO
    bandido->posActual = mov->destino;
    bandido->ultimoMov = mov->direccion;

    //recupero el nuevo terreno donde esta el bandido
    iniciarNodoItCD(&itMapa, bandido->posActual);
    verActualItCD(&itMapa, &terreno, sizeof(terreno));

    //le agrego el bandido y en q turno
    terreno.bandidos += 1;
    terreno.turnoActualizado = turno;

    //actualizo el nuevo terreno del mapa
    modificarActualItCD(&itMapa, &terreno, sizeof(terreno));

    return TODO_OK;
}

int colisionJugadorBandido(tIteradorLista *itBandidos, tLista *bGlobales, tBandido *bandido, tJugador *jugador, tListaCD *mapa, char protegido)
{
    tIteradorCD itMapa;
    tTerreno terreno;

    //recupero terreno del bandido
    iniciarNodoItCD(&itMapa, bandido->posActual);
    verActualItCD(&itMapa, &terreno, sizeof(terreno));

    terreno.bandidos -= 1;

    //guardo la modificacion
    modificarActualItCD(&itMapa, &terreno, sizeof(terreno));

    //el bandido q estoy procesando lo elimino de la lista
    eliminarActualItLista(itBandidos, bGlobales);

    //q pasa con el juagdor?
    if(protegido == NO)
    {
        //pierde 1 vida
        jugador->vidas--;

        //jugador se va de su casilla
        iniciarNodoItCD(&itMapa, jugador->posActual);
        verActualItCD(&itMapa, &terreno, sizeof(terreno));
        terreno.jugador = 0;
        modificarActualItCD(&itMapa, &terreno, sizeof(terreno));

        //jugador vuelve al incio
        iniciarPrimeroItCD(&itMapa, mapa);
        jugador->posActual = verNodoActualItCD(&itMapa);

        //recupero terreno de inicio y lo actualizo con jugador
        verActualItCD(&itMapa, &terreno, sizeof(terreno));
        terreno.jugador = 1;
        modificarActualItCD(&itMapa, &terreno, sizeof(terreno));

        printf("\n%cPerdiste una vida pero te cargaste al bandido!",173);
        jugador->puntos-= jugador->vidas? 5 : 65; //si tiene vidas pierde 5 puntos (+20 por matar al bandido - 15 por perder vida),
                                                  //si quedo sin vidas (perdio) entonces pierde 15 mas 50 de la derrota
    }
    else
    {
        printf("\n%cSobreviviste por la protecci%cn y te cargaste al bandido!",173,162);
        jugador->puntos+=20;
    }


    Sleep(5000);

    return TODO_OK;
}

int despejarInicio(tLista *bGlobales, tListaCD *mapa, tJugador *jugador)
{
    //quito a todos los bandidos q puedan haber en el inicio
    tIteradorLista itBandidos;
    tIteradorCD itInicio, itMapa;
    tBandido bandido;
    tTerreno terreno;

    iniciarPrimeroItCD(&itInicio, mapa);

    //si el jugador no esta en inicio chau
    if(jugador->posActual != verNodoActualItCD(&itInicio))
        return TODO_OK;

    if(iniciarPrimeroItLista(&itBandidos, bGlobales) != L_EXITO)
        return TODO_OK;

    //mientras itere en la lista de bandidos
    while(esValidoItLista(&itBandidos))
    {
        verActualItLista(&itBandidos, &bandido, sizeof(bandido));

        //muevo cada bandido q este en inicio
        if(bandido.posActual == verNodoActualItCD(&itInicio))
        {
            unsigned pasos = tirarDado();

            //quito bandido del inicio
            iniciarNodoItCD(&itMapa, bandido.posActual);
            verActualItCD(&itMapa, &terreno, sizeof(terreno));
            terreno.bandidos -= 1;
            modificarActualItCD(&itMapa, &terreno, sizeof(terreno));

            //mover n casillas hacia atrás
            for(unsigned i = 0; i < pasos; i++)
                retrocederItCD(&itMapa);

            bandido.posActual = verNodoActualItCD(&itMapa);

            //agrego bandido al nuevo terreno
            verActualItCD(&itMapa, &terreno, sizeof(terreno));
            terreno.bandidos += 1;
            modificarActualItCD(&itMapa, &terreno, sizeof(terreno));

            //guardo cambios del bandido
            modificarActualItLista(&itBandidos, &bandido, sizeof(bandido));
        }

        avanzarItLista(&itBandidos);
    }

    return TODO_OK;
}

