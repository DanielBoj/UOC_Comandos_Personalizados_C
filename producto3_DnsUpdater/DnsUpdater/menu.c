#include <stdio.h>
#include <stdbool.h>

#include "menu.h"
#include "data.h"
#include "dns.h"
#include "api.h"

/// <summary>
/// Obtenemos un dato de tipo entero introducido por el usuario mediante teclado.
/// </summary>
/// <param name="limit">int Entero que indica el tope del rango de enteros aceptados como respuesta.</param>
/// <returns>int Valor entrado por el usuario pos stdin.</returns>
int readInt(int limit) {
    int res;
    bool isValid = false;

    do {
        rewind(stdin);
        scanf_s("%d", &res);

        if (res > limit || res < 0) {
            printf("La opcion escogida no es valida.");
        }
        else {
            isValid = true;
        }

    } while (!isValid);

    return res;
}

/// <summary>
/// Driver para inicializar el menú contextual y lanzar las diferentes funciones del programa.
/// </summary>
/// <param name="adpts">dptsTable Tabla de elementos TAD tipo adapter con la información de los adaptadores de red del equipo.</param>
/// <param name="dnsDirs">dnsTable Tabla de DNS con las direcciones obtenidas del archivo externo .txt.</param>
/// <param name="adptrDnsSrvs">dnsTable Tabla de DNS para cargar las direcciones DNS de un adaptador de red del equipo escogido por el usuario.</param>
/// <param name="isSalir">bool Booleano de control para informar de que el usuario ha decidido salir de la aplicación.</param>
/// <param name="adptrInd">int Índice de posición del adaptador de red escogido por el usuario en la tabla de adaptadores.</param>
/// <param name="retVal">Devuelve 0 si las operaciones se realizan con éxito, o 1 de lo contrario.</param>
void callMenu(const adptsTable adpts, const dnsTable dnsDirs, dnsTable* adptrDnsSrvs, bool* isSalir, int* adptrInd, int* retVal) {
    
    *retVal = 0;
    
    int opcion;
    *isSalir = false;

    puts("");
    puts("-------------------------------------------------");
    puts("1. Escoger adaptador de red.");
    puts("2. Realizar test de servidores DNS sobre el adaptador.");
    /*puts("3. Borrar los archivos de log generados.");
    puts("4. Anadir una nueva direccion IP");*/
    puts("0. Salir");
    puts("Escoja 1, 2, o 0:");
    puts("-------------------------------------------------");

    opcion = readInt(2);

    switch (opcion) {
    case 1: {

        *adptrInd = -1;

        driverEscogerAdaptador(adpts, adptrDnsSrvs, adptrInd, retVal);
        break;
    }

    case 2: {
        driverTestServidores(adpts, dnsDirs, *adptrDnsSrvs, *adptrInd, retVal);
        break;
    }

    case 0: *isSalir = true;
    }
}

/// <summary>
/// Driver que inicializa las operaciones para la obtención de las DNS de un adaptador de red que escogerá el usuario..
/// </summary>
/// <param name="adpts">adptsTable Tabla de elementos TAD tipo adapter con la información de los adaptadores de red del equipo..</param>
/// <param name="adptrDnsSrvs">dnsTable Table de DNS para cargar las DNS del adaptador que escoja el usuario.</param>
/// <param name="adptrInd">int Índice de posición del adaptador que escoja el usuario en la tabla de adaptadores.</param>
/// <param name="retVal">Devuelve 0 si las operaciones se realizan con éxito, o 0 d elo contrario.</param>
void driverEscogerAdaptador(const adptsTable adpts, dnsTable* adptrDnsSrvs, int* adptrInd, int* retVal) {

    *retVal = 0;

    int opcion, numAdpts;

    puts("\n-------------------------------------------------");

    puts("Lista de adaptadores de red de este equipo: ");
    numAdpts = printAdptsNames(adpts);
    puts("-------------------------------------------------");

    printf("\nPor favor, indique el numero de adaptador del que desea obtener informacion.\nEscoja un entero entre 1 y % d:\n", numAdpts);
    opcion = readInt(adpts.numElem);
    puts("-------------------------------------------------");

    puts("Las direcciones DNS para este adaptador son:");
    *adptrInd = -1;
    showDnsIps(adpts, opcion, adptrDnsSrvs, adptrInd, retVal);

}

/// <summary>
/// Driver para manejar la inicialización de los tests de velocidad a los servidores y la selección. En el caso de que el usuario no haya escogido un
/// adaptador de red, lo devuelve al menú principal.
/// </summary>
/// <param name="adptsTbl">adptsTable Tabla de elementos TAD tipo adapter con la información de los adaptadores de red del equipo.</param>
/// <param name="dnsDirs">dnsTable Tabla de DNS con las direcciones leídas del documento externo .txt.</param>
/// <param name="adptrDnsSrvr">dnsTable Tabla DNS con las direcciones obtenidas del adaptador de red escogido por el usuario.</param>
/// <param name="adptrInd">int Índice de posición del adaptador escogido por el usuario en la tabla de adaptadores.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se ejecutan con éxito, o 1 de lo contrario.</param>
void driverTestServidores(const adptsTable adptsTbl, const dnsTable dnsDirs, const dnsTable adptrDnsSrvr, const int adptrInd, int* retVal) {
    
    if (adptrInd < 0) {

        /* Si no hay un adaptador de red escogido, vuelve al menú principal */
        puts("Por favor, escoja primero un adaptador de red mediante el paso 1.");

        return;
    }
    puts("\n-------------------------------------------------");
    puts("Iniciando la actualizacion del servidor DNS:");
    puts("-------------------------------------------------");

    getDnsTests(adptsTbl, dnsDirs, adptrDnsSrvr, adptrInd, retVal);
}