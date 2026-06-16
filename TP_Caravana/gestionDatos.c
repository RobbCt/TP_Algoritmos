#include "gestionDatos.h"

int buscarJugadorPorNombre(const char* nombre, const tArbolBinBusq* arbolIdx,
                           FILE* archJugadores, tJugadorDatos* jugadorClon)
{
    tNombreRepetido jugadoresEncontrados;

    // lista temporal
    crearLista(&jugadoresEncontrados.listaAlias);
    jugadoresEncontrados.cantidad = 0; // Inicializar el contador
    printf("\nBuscando el nombre '%s' en el sistema...\n", nombre);

    recorrerArbolFiltradoInOrden(arbolIdx, (void*)nombre, cmpIndiceConNombre, mostrarYGuardarEnLista, &jugadoresEncontrados);

    if (jugadoresEncontrados.cantidad == 0)
        return NO_EXISTE;

    return seleccionarYRecuperarJugador(&jugadoresEncontrados, archJugadores, jugadorClon);
}
int seleccionarYRecuperarJugador(tNombreRepetido* jugadoresEncontrados, FILE* archJugadores, tJugadorDatos* jugadorClon)
{
    int opcion;
    tRegIndice* registroElegido;
    printf(" [%d] Ninguno de los anteriores (Crear un alias nuevo)\n", jugadoresEncontrados->cantidad + 1);

    do {
        printf("\nSeleccione su opcion: ");
        scanf("%d", &opcion);
    } while (opcion < 1 || opcion> jugadoresEncontrados->cantidad + 1);

    if (opcion == jugadoresEncontrados->cantidad + 1)
    {
        vaciarLista(&(jugadoresEncontrados->listaAlias));
        return NO_EXISTE;
    }

    registroElegido= (tRegIndice*)buscarPorPosicionLista(&(jugadoresEncontrados)->listaAlias, opcion);

    if (registroElegido)
    {
        fseek(archJugadores, registroElegido->nroRegistro, SEEK_SET);
        fread(jugadorClon, sizeof(tJugadorDatos), 1, archJugadores);
    }

    vaciarLista(&(jugadoresEncontrados->listaAlias));

    return TODO_OK;
}

void mostrarYGuardarEnLista(const void *infoNodo, void *param)
{
    const tRegIndice *reg = (const tRegIndice *)infoNodo;
    tNombreRepetido *jEncontrados = (tNombreRepetido *)param;

    ponerAlFinalLista(&(jEncontrados->listaAlias), reg, sizeof(tRegIndice));

    jEncontrados->cantidad++;
    printf(" [%d] Alias: %s\n", jEncontrados->cantidad, reg->alias);
}

int crearJugador(const char* nombre, tArbolBinBusq* arbolIdx,
                 FILE* archJugadores, tJugadorDatos* nuevoJugador)
{
    tRegIndice nuevoNodoIndice;
    char alias[30];
    int nuevoId = obtenerUltimoIdJugador(archJugadores) + 1;

    generarAliasUnico(nombre, arbolIdx, alias);

    //armamos la estructura del jugador q va al arch de datos
    nuevoJugador->id = nuevoId;
    strcpy(nuevoJugador->nombre, nombre);
    strcpy(nuevoJugador->alias, alias);

    fseek(archJugadores, 0, SEEK_END);//pal append

    ///guardamos pos antes de escribir
    //ftell nos da la pos exacta en bytes del final del arch,
    //q esjusto el byte donde se escribe el nuejug
    nuevoNodoIndice.nroRegistro = ftell(archJugadores);
    strcpy(nuevoNodoIndice.nombreJugador, nombre);
    strcpy(nuevoNodoIndice.alias, alias);

    if (fwrite(nuevoJugador, sizeof(tJugadorDatos), 1, archJugadores) != 1)
        return ERROR_ARCH;

    //insertamos clave+nroreg
    if (insertarEnArbol(arbolIdx, &nuevoNodoIndice, sizeof(tRegIndice),cmpPorClaveCompleta) != TODO_OK)
        return ERROR_ARBOL;

    return TODO_OK;

}
int gestionarJugador(const char* nombreIngresado, tArbolBinBusq* arbolIdx, FILE* archJugadores, tJugadorDatos* jugadorDeLaPartida)
{
    int estado= buscarJugadorPorNombre(nombreIngresado, arbolIdx, archJugadores, jugadorDeLaPartida);

    if (estado==NO_EXISTE)
        estado= crearJugador(nombreIngresado, arbolIdx, archJugadores, jugadorDeLaPartida);

    return estado;
}

/// Carga y guardado en ABB
//lee el arch idx y reconstruye el abb en memoria
int cargarIdxDesdeArch(tArbolBinBusq* arbolIdx, const char* nombreArchIdx)
{
    FILE* archIdx = fopen(nombreArchIdx, "rb");
    tRegIndice reg;

    if (!archIdx)
        return ERROR_ARCHIVO; //1ra vez q se juega->es normal q no exista

    //volvemos al inicio por seguridad¿
    rewind(archIdx);

    while(fread(&reg, sizeof(tRegIndice), 1, archIdx))
        insertarEnArbol(arbolIdx, &reg, sizeof(tRegIndice), cmpPorClaveCompleta);

    fclose(archIdx);
    return TODO_OK;
}
//pal el recorrido preorden q escribe en el arch
void escribirNodoEnArch(void* elem, unsigned tam, unsigned nivel, void* param)
{
    FILE* arch = (FILE*)param;

    fwrite(elem, tam, 1, arch);
}

int guardarIdxEnArch(tArbolBinBusq* arbolIdx, const char* nombreArchIdx)
{
    FILE* archIdx = fopen(nombreArchIdx, "wb");
    if (!archIdx)
        return ERROR_ARCHIVO;

    //nivel inicial 0
    recorrerPreOrdenRecArbolBinBusq(arbolIdx, 0, archIdx, escribirNodoEnArch);

    fclose(archIdx);
    return TODO_OK;
}

///cmp's¿
int cmpPorClaveCompleta(const void* a, const void* b)///
{
    const tRegIndice* regA= (const tRegIndice*)a;
    const tRegIndice* regB= (const tRegIndice*)b;

    int cmp= strcmp(regA->nombreJugador, regB->nombreJugador);

    if(cmp!= 0)
        return cmp;

    return strcmp(regA->alias,regB->alias);
}
int cmpIndiceConNombre(const void* infoNodo, const void* info)
{
    const tRegIndice* reg= infoNodo;
    const char* nombre= info;

    return strcmp(reg->nombreJugador, nombre);
}
///aux's de id
//pa poder generar el sig id
int obtenerUltimoIdJugador(FILE* archJugadores)
{
    tJugadorDatos ultJugador;
    long tamArchivo,cantRegistros;

    //calculamos el tam en bytespara obtener la cant de regs
    fseek(archJugadores, 0, SEEK_END);
    tamArchivo = ftell(archJugadores);
    cantRegistros = tamArchivo / sizeof(tJugadorDatos);

    //si no hay regs, se setea el 1er id como 0
    if(cantRegistros==0)
        return 0;

    //nos vamos al inicio del ult reg
    //ej: si tenemos 5 regs, saltamos 4 regs [0 1 2 3] y leemos el 5to [4]
    fseek(archJugadores, (cantRegistros-1)*sizeof(tJugadorDatos), SEEK_SET);

    //si falla la lectura del ult reg, asumimos id 0¿
    if (fread(&ultJugador, sizeof(tJugadorDatos), 1, archJugadores)!= 1)
        return 0;

    //dejamos p al final listo pa escribir
    fseek(archJugadores, 0, SEEK_END);

    return ultJugador.id;
}
//copia y pega muak, pero ahora de la partida¿
int obtenerUltimoIdPartida(FILE* archPartidas)
{
    tPartida ultPartida;
    long tamArchivo,cantRegistros;

    fseek(archPartidas, 0, SEEK_END);
    tamArchivo = ftell(archPartidas);
    cantRegistros = tamArchivo / sizeof(tPartida);

    if (cantRegistros == 0)
        return 0;

    fseek(archPartidas, (cantRegistros - 1) * sizeof(tPartida), SEEK_SET);
    if (fread(&ultPartida, sizeof(tPartida), 1, archPartidas) != 1)
        return 0;

    return ultPartida.idPartida;
}


int existeAlias(const tArbolBinBusq* arbolIdx, const char* nombre, const char* alias)
{
    tRegIndice clave;

    strcpy(clave.nombreJugador, nombre);
    strcpy(clave.alias, alias);

    return buscarEnArbol(arbolIdx, &clave, cmpPorClaveCompleta)!= NULL;
}

void generarAliasUnico(const char* nombre,  const tArbolBinBusq* arbolIdx, char* aliasGenerado)
{
    char nombreBase[30];
    int i = 1;

    strcpy(nombreBase, nombre);

    for(int j= 0; nombreBase[j]; j++)
        nombreBase[j] = tolower(nombreBase[j]);

    do
    {
        sprintf(aliasGenerado,"%s%02d",nombreBase, i);
        i++;

    } while(existeAlias(arbolIdx,nombre,aliasGenerado));
}
