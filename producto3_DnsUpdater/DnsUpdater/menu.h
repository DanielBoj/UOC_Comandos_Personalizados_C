// Nos aseguramos de linkar una única vez la librería
#pragma once

#include <stdio.h>
#include <stdbool.h>

#include "data.h"

/// <summary>
/// Obtenemos un dato de tipo entero introducido por el usuario mediante teclado.
/// </summary>
/// <param name="limit">int Entero que indica el tope del rango de enteros aceptados como respuesta.</param>
/// <returns>int Valor entrado por el usuario pos stdin.</returns>
int readInt(int limit);

/// <summary>
/// Driver para inicializar el menú contextual y lanzar las diferentes funciones del programa.
/// </summary>
/// <param name="adpts">dptsTable Tabla de elementos TAD tipo adapter con la información de los adaptadores de red del equipo.</param>
/// <param name="dnsDirs">dnsTable Tabla de DNS con las direcciones obtenidas del archivo externo .txt.</param>
/// <param name="adptrDnsSrvs">dnsTable Tabla de DNS para cargar las direcciones DNS de un adaptador de red del equipo escogido por el usuario.</param>
/// <param name="isSalir">bool Booleano de control para informar de que el usuario ha decidido salir de la aplicación.</param>
/// <param name="adptrInd">int Índice de posición del adaptador de red escogido por el usuario en la tabla de adaptadores.</param>
/// <param name="retVal">Devuelve 0 si las operaciones se realizan con éxito, o 1 de lo contrario.</param>
void callMenu(const adptsTable adpts, const dnsTable dnsDirs, dnsTable* adptrDnsSrvs, bool* isSalir, int* adptrInd, int* retVal);


/// <summary>
/// Driver que inicializa las operaciones para la obtención de las DNS de un adaptador de red que escogerá el usuario..
/// </summary>
/// <param name="adpts">adptsTable Tabla de elementos TAD tipo adapter con la información de los adaptadores de red del equipo..</param>
/// <param name="adptrDnsSrvs">dnsTable Table de DNS para cargar las DNS del adaptador que escoja el usuario.</param>
/// <param name="adptrInd">int Índice de posición del adaptador que escoja el usuario en la tabla de adaptadores.</param>
/// <param name="retVal">Devuelve 0 si las operaciones se realizan con éxito, o 0 d elo contrario.</param>
void driverEscogerAdaptador(const adptsTable adpts, dnsTable* adptrDnsSrvs, int* adptrInd, int* retVal);

/// <summary>
/// Driver para manejar la inicialización de los tests de velocidad a los servidores y la selección .
/// </summary>
/// <param name="adptsTbl">adptsTable Tabla de elementos TAD tipo adapter con la información de los adaptadores de red del equipo.</param>
/// <param name="dnsDirs">dnsTable Tabla de DNS con las direcciones leídas del documento externo .txt.</param>
/// <param name="adptrDnsSrvr">dnsTable Tabla DNS con las direcciones obtenidas del adaptador de red escogido por el usuario.</param>
/// <param name="adptrInd">int Índice de posición del adaptador escogido por el usuario en la tabla de adaptadores.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se ejecutan con éxito, o 1 de lo contrario.</param>
void driverTestServidores(const adptsTable adptsTbl, const dnsTable dnsDirs, const dnsTable adptrDnsSrvr, const int adptrInd, int* retVal);