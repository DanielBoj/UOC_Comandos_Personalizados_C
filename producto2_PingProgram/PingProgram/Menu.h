#pragma once // Nos aseguramos de que la librer�a solo pueda incluirse una vez en la compilaci�n.

#include <stdio.h>
#include <stdbool.h>

/**
 * @brief leerEntero: Funci�n para obtener un dato de tipo entero
 * introducido por el usuario mediante el teclado.
 *
 * @param limit  int Entero tope del rango de enteros aceptados como
 *  repuesta.
 * @return int Valor entrado por el usuario obtenido por stdin.
 */
int leerEntero(int limit);

/**
 * @brief callMenu: Es la funci�n de llamada al men�u principal.
 * Retorna un valor booleano true si el usuario decide salir del men�.
 *
 * @return true Indica al Driver del programa que salga del men�.
 * @return false Indica al Driver del programa que continue preguntando
 *  opciones de men� al usuario.
 */
bool callMenu();

/**
 * @brief Driver del men�u principal: Inicia el men� y controla el bucle
 * de ejecuci�n mediante el retorno de la funci�n callMenu(); *
 */
void menuDriver();

/**
 * @brief driverRealizarPing(): Driver que delega en el m�dulo Ping.c
 *
 */
void driverRealizarPing();

/**
 * @brief driverRealizarPing(): Driver que delega en el m�dulo Adaptador.c
 *
 */
void driverInformacionAdaptadores();

/**
 * @brief borrarLogListaAdptrs(): Borra el archivo de log que ha creado el programa.
 *
 */
void borrarLogListaAdptrs();

/**
 * @brief borrarLogInfoAdptrs(): Borra el archivo de log que ha creado el programa.
 *
 */
void borrarLogInfoAdptrs();

/**
 * @brief callSubMenuBorrado(): Funci�n de llamada que carga el subme� para el borrado
 * de los archivos generados por el programa.
 *
 */
bool callSubmenuBorrado();

/**
 * @brief Driver del submen� de borrado de archivos: Inicia el men� y controla el bucle
 * de ejecuci�n mediante el retorno de la funci�n callMenu(); *
 */
void submenuDriver();

/**
 * @brief Driver para a�adir una nueva IP.
 * Delega en Ping.c.
 */
void driverAniadirIp();