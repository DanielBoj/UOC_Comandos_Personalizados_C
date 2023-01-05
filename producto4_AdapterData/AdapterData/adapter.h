#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

/// <summary>
/// Carga la informaci�n de los adaptadores de red activos en una tabla de elementos TAD de tipo adapter.
/// La informaci�n se obtine lanzando desde el c�digo un comando NETSH a trac�n de un pipe de lectura con _popen().
/// La lectura d elos datos se carga en una estructura TAD tipo adapter temporal. Los datos se obtienen mediante la lectura l�nea a l�nea
/// del comando CMD. Se descartan las l�neas sin inter�s mediant eun contador de l�neas y, cuando llegamos a una l�nea con datos
/// de un adaptador, realizamos la lectura de la l�nea y la dividimos en substrings mediante la funci�n strtok().
/// Escogemos las posiciones que nos interesan mediante un contador de palabras para obtener el �ndice del adaptador y su nombre.
/// Obtenemos tambi�n el estado del adpatador, si est� activo, usamos un booleano de control para indicarlo.
/// Tras la lectura de la l�nea, comporbamos si el adaptador est� conectado, si es as�, cargamos los datos en variables temporales.
/// Tras esto, pasamos los datos a la estructura adpater y la cargamos en la tabla.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adpater.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan con �xito o 1 si no es as�.</param>
void loadAdptrsInfo(adaptersTable* table, int* retVal);

/// <summary>
/// Carga las distintas direcciones asociadas a un adaptador de red en los campos de informaci�n de un elmenton TAD tipo adaptador en
/// concreto de una tabla de adaptadores.
/// La funci�n genera un comando CMD y captura su retorno a trav� d ela creaci�n y lectura de un pipe con _popen().
/// Se descartan las l�neas sin informaci�n a ser capturada y se realiza un control de la l�nea de posici�n en el documento para conocer
/// qu� datos s evan a ir cargando. Si nos encontramos en una l�nea con datos a capturar, se divide en substrings mendiante strtok() y se
/// va realizando la lectura de las palabras hasta que llegamos al dato a cargar. Se captura el dato con strncpy() y se limpian los
/// caracteres sobrantes ('\n', ')'). tras esto, se guardan los datos en el campo asignado del elemento TAD.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adpater.</param>
/// <param name="adptrInd">int �ndice de adaptador, se usa para facilitar la llamada a los comandos CMD.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan con �xito o 1 si no es as�.</param>
void getIpInfo(adaptersTable* table, const int adptrInd, int* retVal);

/// <summary>
/// Realiza un diagn�stico tippo tracert al servidor DNS del adaptador de red.
/// La funci�n recibe por par�metros la tabla de elementos TAD tipo adapter y el �ndice del adaptador.
/// A trav�s del �ndice, busca la posici�n del adaptador en la tabla para poderlo actualizar mediante searchAdapter().
/// Genera el comando y lo lanza creando un pipe de lectura a un puntero FILE mediante _popen() y realiza la lectura del retorno.
/// Se ignoran las l�neas sin informaci�n a capturar, una vez iniciada la lectura de las l�neas con informaci�n, se comprueba
/// mediante la b�squeda de car�cteres clave si se trata de un retorno nulo, usando strstr(). Si es as�, como el comando se ha
/// lanzado con una restricci�n de tiempo m�ximo de 1000ms, se le da esa duraci�n al salto.
/// Si el salto devuelve respuesta, se capturan el tiempo medio de respuesta y la direcci�n y, si lo hay, nombre del servidor.
/// La informaci�n se va cargando al adaptador, mediante un contador de posiciones, se ir� llenando la tabla de elementos tipo
/// jump con cada uno de los saltos. Una vez se llega al EOF, se realiza el c�lculo del tiempo medio y se carga la informaci�n
/// en el elemento adapter seleccionado de la tabla.
/// Los strings se gestionan mediante memoria din�mica.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adpater.</param>
/// <param name="adptrInd">int �ndice de adaptador, se usa para facilitar la llamada a los comandos CMD.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan con �xito o 1 si no es as�.</param>
void getDnsTest(adaptersTable* table, const int adptrInd, int* retVal);

/// <summary>
/// Realiza una carga inicial de datos a la tabla de elementos TAD tipo adaptador. Usa funciones de copia de string para el nombre,
/// strncpy_s(), carga el resto de datos por asignaci�n directa y actualiza el n�mero de elementos.
/// Se realiza el control de errores.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="adptr">adapter Estructura TAD de tipo adapter con los datos del adaptador.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan correctamente, o 1 si hay errores.</param>
void addAdpatersNameAndIndex(adaptersTable* table, const adapter adptr, int* retVal);

/// <summary>
/// Devuelve el �ndice de posici�n en una tabla de elementos TAD tipo adapter de un adaptador de red b�scado por su n�mero de �ndice que
/// recibe como par�metro.
/// Es una funci�n de tipo recursiva que recibe el punto de inicio de la tabla y el tama�o de lista como punto final por par�metro. Va
/// realizando una b�squeda en orden inverso por la lista. Si encuentra la posici�n, devuelve el �ndice, si no, devuelve -1.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="indToFind">int �ndice de adaptador del elemento a buscar.</param>
/// <param name="start">int Inicio de la tabla.</param>
/// <param name="end">int Posici�n final de la tabla.</param>
/// <returns></returns>
int searchAdapater(const adaptersTable table, const int indToFind, const int start, const int end);

/// <summary>
/// Imprime por pantalla los datos de un elemento TAD tipo adapter.
/// El elemento se escoge de una tabla tipo adaptersTable que recibe por par�metro a trav�s de un �ndice de posici�n que tambi�n
/// se recibe por par�metro.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="indPos">int �ndice de posici�n del adaptador.</param>
void printAdapterInfo(const adaptersTable table, const int indPos);