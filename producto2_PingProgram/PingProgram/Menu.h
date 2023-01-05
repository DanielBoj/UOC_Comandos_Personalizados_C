#pragma once // Nos aseguramos de que la librería solo pueda incluirse una vez en la compilación.

#include <stdio.h>
#include <stdbool.h>

/**
 * @brief leerEntero: Función para obtener un dato de tipo entero
 * introducido por el usuario mediante el teclado.
 *
 * @param limit  int Entero tope del rango de enteros aceptados como
 *  repuesta.
 * @return int Valor entrado por el usuario obtenido por stdin.
 */
int leerEntero(int limit);

/**
 * @brief callMenu: Es la función de llamada al menñu principal.
 * Retorna un valor booleano true si el usuario decide salir del menú.
 *
 * @return true Indica al Driver del programa que salga del menú.
 * @return false Indica al Driver del programa que continue preguntando
 *  opciones de menú al usuario.
 */
bool callMenu();

/**
 * @brief Driver del menñu principal: Inicia el menú y controla el bucle
 * de ejecución mediante el retorno de la función callMenu(); *
 */
void menuDriver();

/**
 * @brief driverRealizarPing(): Driver que delega en el módulo Ping.c
 *
 */
void driverRealizarPing();

/**
 * @brief driverRealizarPing(): Driver que delega en el módulo Adaptador.c
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
 * @brief callSubMenuBorrado(): Función de llamada que carga el submeú para el borrado
 * de los archivos generados por el programa.
 *
 */
bool callSubmenuBorrado();

/**
 * @brief Driver del submenú de borrado de archivos: Inicia el menú y controla el bucle
 * de ejecución mediante el retorno de la función callMenu(); *
 */
void submenuDriver();

/**
 * @brief Driver para añadir una nueva IP.
 * Delega en Ping.c.
 */
void driverAniadirIp();