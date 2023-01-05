#include <stdio.h>
#include <stdbool.h>

#include "api.h"
#include "data.h"
#include "menu.h"

/// <summary>
/// Driver para inicializar el menú contextual y lanzar las diferentes funciones del programa.
/// </summary>
/// <param name="adptsTbl">adpatersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="isSalir">bool Booleano de control para informar de que el usuario ha decidido salir de la aplicación.</param>
/// <param name="adptrInd">int Índice del adaptador escogido por el usuario.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan con éxito, o 1 de lo contrario.</param>
void mainMenu(adaptersTable* adptsTbl, bool* isSalir, int* adptrInd, int* retVal) {

    *retVal = 0;

    int opcion;

    *isSalir = false;

    puts("");
    puts("-------------------------------------------------");
    puts("1. Escoger adaptador de red.");
    puts("2. Generar documento XML con los datos del adaptador.");
    puts("3. Mostrar archivo XML generado.");
    puts("0. Salir");
    puts("Escoja 1, 2, 3 o 0:");
    puts("-------------------------------------------------");

    opcion = readInt(3);

    switch (opcion) {
    case 1: {

        *adptrInd = -1;

        driverEscogerAdaptador(*adptsTbl, adptrInd, retVal);
        break;
    }

    case 2: {
        driverCargarDatosAdaptador(adptsTbl, *adptrInd, retVal);
        break;
    }

    case 3: {
        *retVal = driverLeerArchivoXMl();
        break;
    }

    case 0: {

        /* Cerramos el archivo XML. */
        closeData(retVal);

        *isSalir = true;
    }
    }
}

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
/// Driver que permite al usuario escoger el adaptador de red del que querrá generar información.
/// </summary>
/// <param name="adptsTbl">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="adptrInd">int Índice de adaptador, se usa para facilitar la llamada a los comandos CMD.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se ejecutan correctamente o 1 de lo contrario.</param>
void driverEscogerAdaptador(const adaptersTable adptsTbl, int* adptrInd, int* retVal) {

    *retVal = 0;

    int opcion, numAdpts, posInd;

    puts("\n-------------------------------------------------");

    puts("Lista de adaptadores de red de este equipo: ");
    numAdpts = adptsTbl.numElem;
    printAdaptersName(adptsTbl);

    printf("\nPor favor, indique el numero de adaptador del que desea obtener informacion.\nEscoja un entero entre 1 y % d:\n", numAdpts);
    opcion = readInt(adptsTbl.numElem);
    puts("-------------------------------------------------");

    puts("Informacion del adaptador escogido:");

    /* Asignamos la posición del adaptador en la tabla. */
    posInd = --opcion;

    /* Guardamos el índice del adapatador seleccionado. */
    *adptrInd = adptsTbl.adptrs[posInd].index;

    printf_s("Nombre: %s\nIndice de adaptador: %d\n",
        adptsTbl.adptrs[posInd].name,
        adptsTbl.adptrs[posInd].index);
}

/// <summary>
/// Driver para optener los datos de las direcciones y el test de velocidad de un adaptador de red.
/// Delega en la función getAdaptersData() del módulo API.
/// </summary>
/// <param name="adptsTbl">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="adptrInd">int Índice de adaptador, se usa para facilitar la llamada a los comandos CMD.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se ejecutan correctamente o 1 de lo contrario.</param>
void driverCargarDatosAdaptador(adaptersTable* adptsTbl, const int adptrInd, int* retVal) {

    if (adptrInd < 0) {
        puts("\n-------------------------------------------------");
        puts("\nPor favor, seleccione un adaptador de red antes de continuar.");
        return;
    }

    getAdaptersData(adptsTbl, adptrInd, retVal);
}

/// <summary>
/// Driver para leer el archvi XML generado en bruto.
/// Delega en la función readData() del módulo API.
/// </summary>
/// <returns>int Devuelve 0 si las operaciones se realizan con éxito o 1 de lo contrario.</returns>
int driverLeerArchivoXMl() {

    int retVal;

    readData(&retVal);

    return retVal;
}