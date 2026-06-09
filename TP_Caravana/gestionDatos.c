#include "gestionDatos.h"


int buscarJugadorPorNombre(const char* nombre, const tArbolBinBusq* arbolIdx,
                           FILE* archJugadores, tJugadorIndice* jugadorClon)

{
    //clave-offset
    tRegIndice* registroIndice;

    registroIndice= (tRegIndice*)buscarEnArbol(arbolIdx, nombre, cmpPorNombre);

    if (!registroIndice)
        return NO_EXISTE;

    //si existe, usamos nroReg(guarda la pos en bytes) para ir directo en el arch
    fseek(archJugadores, registroIndice->nroRegistro, SEEK_SET);
    fread(jugadorClon, sizeof(tJugadorIndice), 1, archJugadores);

    return TODO_OK;
}

int crearJugador(const char* nombre, tArbolBinBusq* arbolIdx,
                 FILE* archJugadores, tJugadorIndice* nuevoJugador)

{
    tRegIndice nuevoNodoIndice;
    int nuevoId = obtenerUltimoIdJugador(archJugadores) + 1;

    //armamos la estructura del jugador q va al arch de datos
    nuevoJugador->id = nuevoId;
    strcpy(nuevoJugador->nombre, nombre);

    fseek(archJugadores, 0, SEEK_END);//pal append

    ///guardamos pos antes de escribir
    //ftell nos da la pos exacta en bytes del final del arch,
    //q esjusto el byte donde se escribe el nuejug
    nuevoNodoIndice.nroRegistro = ftell(archJugadores);
    strcpy(nuevoNodoIndice.nombreJugador, nombre);

    if (fwrite(nuevoJugador, sizeof(tJugadorIndice), 1, archJugadores) != 1)
        return ERROR_ARCH;

    //insertamos clave+nroreg
    if (insertarEnArbol(arbolIdx, &nuevoNodoIndice, sizeof(tRegIndice),cmpPorNombre) != TODO_OK)
        return ERROR_ARBOL;

    return TODO_OK;

}
int gestionarJugador(const char* nombreIngresado, tArbolBinBusq* arbolIdx, FILE* archJugadores, tJugadorIndice* jugadorDeLaPartida)
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
        insertarEnArbol(arbolIdx, &reg, sizeof(tRegIndice), cmpPorNombre);

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
int cmpPorNombre(const void* a, const void* b)
{
    const tRegIndice* regA = (const tRegIndice*)a;
    const tRegIndice* regB = (const tRegIndice*)b;

    return strcmp(regA->nombreJugador, regB->nombreJugador);
}

///aux's de id
//pa poder generar el sig id
int obtenerUltimoIdJugador(FILE* archJugadores)
{
    tJugadorIndice ultJugador;
    long tamArchivo,cantRegistros;

    //calculamos el tam en bytespara obtener la cant de regs
    fseek(archJugadores, 0, SEEK_END);
    tamArchivo = ftell(archJugadores);
    cantRegistros = tamArchivo / sizeof(tJugadorIndice);

    //si no hay regs, se setea el 1er id como 0
    if(cantRegistros==0)
        return 0;

    //nos vamos al inicio del ult reg
    //ej: si tenemos 5 regs, saltamos 4 regs [0 1 2 3] y leemos el 5to [4]
    fseek(archJugadores, (cantRegistros-1)*sizeof(tJugadorIndice), SEEK_SET);

    //si falla la lectura del ult reg, asumimos id 0¿
    if (fread(&ultJugador, sizeof(tJugadorIndice), 1, archJugadores)!= 1)
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
