#include "juego.h"

int main()
{
    int opc;
    srand(time(NULL));

    do
    {
        puts("\nCARAVANA DEL DESIERTO");
        puts("Seleccione una opcion..");
        puts("1- Nueva Partida");
        puts("2- Como se juega?");
        puts("3- Rankings");
        puts("4- Salir");

        printf("Opcion: ");
        scanf("%d", &opc);

        switch(opc)
        {
            char nombre[TAM_NOMBRE];
            case 1:
                printf("Ingrese su nombre:");
                fflush(stdin);
                gets(nombre);
                nuevaPartida(nombre);
                break;
            case 2:
                mostrarReglas();
                break;
            case 3:
                verRanking();
                break;
            case 4:
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

    return TODO_OK;
}
