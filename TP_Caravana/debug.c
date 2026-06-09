#include <stdio.h>
#include <stdlib.h>
#include "gestionDatos.h"
#include "arbolBB.h" // Necesario para la estructura tRegIndice

////llamado en el main
/*
            case 99:
                system("cls");
                puts("--- VISTA DE ARCHIVOS BIN ---");
                espiarArchivoJugadores();
                espiarArchivoIndice();
                espiarArchivoPartidasLocal();
                system("pause");
                break;
*/
////


/// ============================================================================
/// 1. ESPÍA DE JUGADORES.DAT (El archivo pesado)
/// ============================================================================
void espiarArchivoJugadores()
{
    FILE* arch = fopen(JUGADORES_ARCH, "rb");
    tJugadorIndice j;
    int cant = 0;

    if(arch == NULL) return;

    while(fread(&j, sizeof(tJugadorIndice), 1, arch))
    {
        printf("\n[%03d] %10d %-20s", cant, j.id, j.nombre);
        cant++;
    }
    fclose(arch);
}

/// ============================================================================
/// 2. ESPÍA DE INDICE.DAT (El archivo serializado del Árbol)
/// ============================================================================
void espiarArchivoIndice()
{
    FILE* arch = fopen("indices.dat", "rb");
    tRegIndice reg; // Estructura liviana del árbol (nombre y nroRegistro)
    int cant = 0;

    if(arch == NULL)
    {
        printf("\n[DEBUG] No se pudo abrir indice.dat o aun no existe.\n");
        return;
    }

    printf("\n========================================================");
    printf("\n   [DEBUG] CONTENIDO REAL DE INDICE.DAT (SERIALIZADO)");
    printf("\n========================================================");
    printf("\n%s %-20s %12s", "NRO", "CLAVE (NOMBRE)", "NRO_REGISTRO");
    printf("\n--------------------------------------------------------");

    while(fread(&reg, sizeof(tRegIndice), 1, arch))
    {
        cant++;
        printf("\n[%02d] %-20s %12ld", cant, reg.nombreJugador, reg.nroRegistro);
    }

    if(cant == 0)
        printf("\n   El archivo fisico del indice esta vacio.");

    printf("\n========================================================\n\n");
    fclose(arch);
}

/// ============================================================================
/// 3. ESPÍA DE PARTIDAS.DAT (El historial de puntuaciones)
/// ============================================================================
void espiarArchivoPartidasLocal()
{
    FILE* arch = fopen(PARTIDAS_ARCH, "rb");
    tPartida p;
    int cant = 0;

    if(arch == NULL)
    {
        printf("\n[DEBUG] No se pudo abrir %s o aun no existe.\n", PARTIDAS_ARCH);
        return;
    }

    printf("\n========================================================");
    printf("\n   [DEBUG] CONTENIDO REAL DE PARTIDAS.DAT");
    printf("\n========================================================");
    printf("\n%s %10s %12s %10s %10s", "REG", "ID_PART", "ID_JUGADOR", "PUNTOS", "MOVIM.");
    printf("\n--------------------------------------------------------");

    while(fread(&p, sizeof(tPartida), 1, arch))
    {
        cant++;
        printf("\n[%02d] %10d %12d %10d %10d",
                cant, p.idPartida, p.idJugador, p.puntaje, p.cantMovimientos);
    }

    if(cant == 0)
        printf("\n   El archivo de partidas esta vacio.");

    printf("\n========================================================\n\n");
    fclose(arch);
}
