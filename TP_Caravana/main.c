#include "juego.h"
#include "arbolBB.h"
#include "gestionDatos.h"

int main()
{
    int opc;
    char nombre[TAM_NOMBRE];
    srand(time(NULL));

    tArbolBinBusq arbolIdx;
    FILE* archJugadores;
    FILE* archPartidas;

    crearArbolBinBusq(&arbolIdx);

    if(abrirArchivo(&archJugadores, JUGADORES_ARCH, "a+b") != TODO_OK)
        return ERROR_ARCHIVO;
    if(abrirArchivo(&archPartidas, PARTIDAS_ARCH, "a+b") != TODO_OK)
    {
        fclose(archJugadores);
        return ERROR_ARCHIVO;
    }

    cargarIdxDesdeArch(&arbolIdx, INDICE_ARCH);

    do
    {
        puts("\nCARAVANA DEL DESIERTO");
        puts("Seleccione una opcion...");
        puts("1- Nueva Partida");
        puts("2- Como se juega?");
        puts("3- Rankings");
        puts("4- Salir");

        printf("Opcion: ");
        scanf("%d", &opc);

        int c;
        while((c = getchar()) != '\n' && c != EOF);
        switch(opc)
        {
            case 1:
                printf("Ingrese su nombre:");
                fgets(nombre, TAM_NOMBRE, stdin);
                nuevaPartida(nombre, &arbolIdx, archJugadores, archPartidas);
                system("pause");
                break;
            case 2:
                system("cls");
                mostrarReglas();
                break;
            case 3:
                verRanking(archJugadores, archPartidas);
                system("pause");
                break;
            case 4:
                guardarIdxEnArch(&arbolIdx, INDICE_ARCH);//actualizamos idx cada q salimos del juego¿¿
                puts("Saliendo..");
                system("pause");
                break;
            default:
                puts("Opcion invalida, reingrese...");
                system("pause");
                break;
        }
        system("cls");
    }while(opc!=4);

    vaciarArbolBinBusq(&arbolIdx);
    fclose(archJugadores);
    fclose(archPartidas);

    return TODO_OK;
}
