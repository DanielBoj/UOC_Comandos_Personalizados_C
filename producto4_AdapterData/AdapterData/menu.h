#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "api.h"

/// <summary>
/// Driver para inicializar el menú contextual y lanzar las diferentes funciones del programa.
/// </summary>
/// <param name="adptsTbl">adpatersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="isSalir">bool Booleano de control para informar de que el usuario ha decidido salir de la aplicación.</param>
/// <param name="adptrInd">int Índice del adaptador escogido por el usuario.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan con éxito, o 1 de lo contrario.</param>
void mainMenu(adaptersTable* adptsTbl, bool* isSalir, int* adptrPos, int* retVal);

/// <summary>
/// Obtenemos un dato de tipo entero introducido por el usuario mediante teclado.
/// </summary>
/// <param name="limit">int Entero que indica el tope del rango de enteros aceptados como respuesta.</param>
/// <returns>int Valor entrado por el usuario pos stdin.</returns>
int readInt(int limit);

/// <summary>
/// Driver que permite al usuario escoger el adaptador de red del que querrá generar información.
/// </summary>
/// <param name="adptsTbl">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="adptrInd">int Índice de adaptador, se usa para facilitar la llamada a los comandos CMD.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se ejecutan correctamente o 1 de lo contrario.</param>
void driverEscogerAdaptador(const adaptersTable adptsTbl, int* adptrPos, int* retVal);

/// <summary>
/// Driver para optener los datos de las direcciones y el test de velocidad de un adaptador de red.
/// Delega en la función getAdaptersData() del módulo API.
/// </summary>
/// <param name="adptsTbl">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="adptrInd">int Índice de adaptador, se usa para facilitar la llamada a los comandos CMD.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se ejecutan correctamente o 1 de lo contrario.</param>
void driverCargarDatosAdaptador(adaptersTable* adptsTbl, const int adptrInd, int* retVal);

/// <summary>
/// Driver para leer el archvi XML generado en bruto.
/// Delega en la función readData() del módulo API.
/// </summary>
/// <returns>int Devuelve 0 si las operaciones se realizan con éxito o 1 de lo contrario.</returns>
int driverLeerArchivoXMl();