#include <stdio.h>
#include <stdbool.h>

#include "Adaptador.h"
#include "LinkedList.h"
#include "Menu.h"
#include "Ping.h"

/**
 * @brief leerEntero: Función para obtener un dato de tipo entero
 * introducido por el usuario mediante el teclado.
 *
 * @param limit  int Entero tope del rango de enteros aceptados como
 *  repuesta.
 * @return int Valor entrado por el usuario obtenido por stdin.
 */
int leerEntero(int limit) {
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

/**
 * @brief callMenu: Es la función de llamada al menú principal.
 * Retorna un valor booleano true si el usuario decide salir del menú.
 *
 * @return true Indica al Driver del programa que salga del menú.
 * @return false Indica al Driver del programa que continue preguntando
 *  opciones de menú al usuario.
 */
bool callMenu() {
    int opcion;
    bool isSalir = false;

    puts("");
    puts("Bienvenido al programa de llamadas ping!\nPor favor, escoja una opcion del menu:");
    puts("-------------------------------------------------");
    puts("1. Realizar ping a direccion IP -> Debera escoger un archivo donde esten almacenadas las direcciones.");
    puts("2. Mostrar parametros de su adaptador de red.");
    puts("3. Borrar los archivos de log generados.");
    puts("4. Anadir una nueva direccion IP");
    puts("0. Salir");
    puts("Escoja 1, 2, 3, 4 o 0:");
    puts("-------------------------------------------------");

    opcion = leerEntero(4);

    switch (opcion) {
        case 1: {
            driverRealizarPing();
            break;
        }

        case 2: {
            driverInformacionAdaptadores();
            puts("-------------------------------------------------");
            break;
        }

        case 3: {
            submenuDriver();
            break;
        }

        case 4: {
            driverAniadirIp();
            break;
        }

        case 0: isSalir = true;
    }

    return isSalir;
}

/**
 * @brief driverRealizarPing(): Driver que delega en el módulo Ping.c
 *
 */
void driverRealizarPing() {
    int resVal;
    
    resVal = ipPingDriver();
}

/**
 * @brief driverRealizarPing(): Driver que delega en el módulo Adaptador.c
 *
 */
void driverInformacionAdaptadores() {
    int opcion;
    listaAdptr adptrs;

    puts("\nLista de adaptadores de red de este equipo: ");
    adptrs = driverAdptrs();
    puts("-------------------------------------------------");

    printf("\nPor favor, indique el numero de adaptador del que desea obtener informacion.\nEscoja un entero entre 1 y % d:\n", adptrs.numElem);
    opcion = leerEntero(adptrs.numElem);
    printf("%d\n", opcion);

    driverMostrarInformacionAdptrs(&adptrs, opcion);
    puts("-------------------------------------------------");
}

/**
 * @brief borrarLogListaAdptrs(): Borra el archivo de log que ha creado el programa.
 *
 */
void borrarLogListaAdptrs() {
    
    deleteLogAdptrs();
}

/**
 * @brief borrarLogInfoAdptrs(): Borra el archivo de log que ha creado el programa.
 *
 */
void borrarLogInfoAdptrs() {

    deleteLogInfoAdptrs();
}

/**
 * @brief callSubMenuBorrado(): Función de llamada que carga el submeú para el borrado
 * de los archivos generados por el programa.
 *
 */
bool callSubmenuBorrado() {
    int opcion;
    bool isSalir = false;

    puts("\nPor favor, escoja el Log que desea borrar:");
    puts("1. Log con la lista de adaptadores del equipo.");
    puts("2. Log con la informacion de los adaptadores del equipo.");
    puts("0. Volver al menu principal.");
    puts("Escoja una opcion 0, 1 o 2:");
    puts("-------------------------------------------------");

    opcion =  leerEntero(2);


    switch (opcion) {
        case 1: {
            borrarLogListaAdptrs();
            break;
        }
        case 2: {
            borrarLogInfoAdptrs();
            break;
        }
        case 0: isSalir = true;
    }

    return isSalir;
}

/**
 * @brief Driver para añadir una nueva IP.
 * Delega en Ping.c.
 */
void driverAniadirIp() {
    addIpDriver();
    puts("-------------------------------------------------");
    puts("Se ha anadido la IP con exito!");
    puts("-------------------------------------------------");
}

/**
 * @brief Driver del menú principal: Inicia el menú y controla el bucle
 * de ejecución mediante el retorno de la función callMenu(); *
 */
void menuDriver() {

    bool isEnd;

    do {
        isEnd = callMenu();
    } while (!isEnd);
}

/**
 * @brief Driver del submenú de borrado de archivos: Inicia el menú y controla el bucle
 * de ejecución mediante el retorno de la función callMenu(); *
 */
void submenuDriver() {

    bool isEnd;

    do {
        isEnd = callSubmenuBorrado();
    } while (!isEnd);
}