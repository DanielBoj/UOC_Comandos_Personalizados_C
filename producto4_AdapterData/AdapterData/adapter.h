#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

/// <summary>
/// Carga la información de los adaptadores de red activos en una tabla de elementos TAD de tipo adapter.
/// La información se obtine lanzando desde el código un comando NETSH a tracén de un pipe de lectura con _popen().
/// La lectura d elos datos se carga en una estructura TAD tipo adapter temporal. Los datos se obtienen mediante la lectura línea a línea
/// del comando CMD. Se descartan las líneas sin interés mediant eun contador de líneas y, cuando llegamos a una línea con datos
/// de un adaptador, realizamos la lectura de la línea y la dividimos en substrings mediante la función strtok().
/// Escogemos las posiciones que nos interesan mediante un contador de palabras para obtener el índice del adaptador y su nombre.
/// Obtenemos también el estado del adpatador, si está activo, usamos un booleano de control para indicarlo.
/// Tras la lectura de la línea, comporbamos si el adaptador está conectado, si es así, cargamos los datos en variables temporales.
/// Tras esto, pasamos los datos a la estructura adpater y la cargamos en la tabla.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adpater.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan con éxito o 1 si no es así.</param>
void loadAdptrsInfo(adaptersTable* table, int* retVal);

/// <summary>
/// Carga las distintas direcciones asociadas a un adaptador de red en los campos de información de un elmenton TAD tipo adaptador en
/// concreto de una tabla de adaptadores.
/// La función genera un comando CMD y captura su retorno a travé d ela creación y lectura de un pipe con _popen().
/// Se descartan las líneas sin información a ser capturada y se realiza un control de la línea de posición en el documento para conocer
/// qué datos s evan a ir cargando. Si nos encontramos en una línea con datos a capturar, se divide en substrings mendiante strtok() y se
/// va realizando la lectura de las palabras hasta que llegamos al dato a cargar. Se captura el dato con strncpy() y se limpian los
/// caracteres sobrantes ('\n', ')'). tras esto, se guardan los datos en el campo asignado del elemento TAD.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adpater.</param>
/// <param name="adptrInd">int Índice de adaptador, se usa para facilitar la llamada a los comandos CMD.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan con éxito o 1 si no es así.</param>
void getIpInfo(adaptersTable* table, const int adptrInd, int* retVal);

/// <summary>
/// Realiza un diagnóstico tippo tracert al servidor DNS del adaptador de red.
/// La función recibe por parámetros la tabla de elementos TAD tipo adapter y el índice del adaptador.
/// A través del índice, busca la posición del adaptador en la tabla para poderlo actualizar mediante searchAdapter().
/// Genera el comando y lo lanza creando un pipe de lectura a un puntero FILE mediante _popen() y realiza la lectura del retorno.
/// Se ignoran las líneas sin información a capturar, una vez iniciada la lectura de las líneas con información, se comprueba
/// mediante la búsqueda de carácteres clave si se trata de un retorno nulo, usando strstr(). Si es así, como el comando se ha
/// lanzado con una restricción de tiempo máximo de 1000ms, se le da esa duración al salto.
/// Si el salto devuelve respuesta, se capturan el tiempo medio de respuesta y la dirección y, si lo hay, nombre del servidor.
/// La información se va cargando al adaptador, mediante un contador de posiciones, se irá llenando la tabla de elementos tipo
/// jump con cada uno de los saltos. Una vez se llega al EOF, se realiza el cálculo del tiempo medio y se carga la información
/// en el elemento adapter seleccionado de la tabla.
/// Los strings se gestionan mediante memoria dinámica.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adpater.</param>
/// <param name="adptrInd">int Índice de adaptador, se usa para facilitar la llamada a los comandos CMD.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan con éxito o 1 si no es así.</param>
void getDnsTest(adaptersTable* table, const int adptrInd, int* retVal);

/// <summary>
/// Realiza una carga inicial de datos a la tabla de elementos TAD tipo adaptador. Usa funciones de copia de string para el nombre,
/// strncpy_s(), carga el resto de datos por asignación directa y actualiza el número de elementos.
/// Se realiza el control de errores.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="adptr">adapter Estructura TAD de tipo adapter con los datos del adaptador.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan correctamente, o 1 si hay errores.</param>
void addAdpatersNameAndIndex(adaptersTable* table, const adapter adptr, int* retVal);

/// <summary>
/// Devuelve el índice de posición en una tabla de elementos TAD tipo adapter de un adaptador de red búscado por su número de índice que
/// recibe como parámetro.
/// Es una función de tipo recursiva que recibe el punto de inicio de la tabla y el tamaño de lista como punto final por parámetro. Va
/// realizando una búsqueda en orden inverso por la lista. Si encuentra la posición, devuelve el índice, si no, devuelve -1.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="indToFind">int Índice de adaptador del elemento a buscar.</param>
/// <param name="start">int Inicio de la tabla.</param>
/// <param name="end">int Posición final de la tabla.</param>
/// <returns></returns>
int searchAdapater(const adaptersTable table, const int indToFind, const int start, const int end);

/// <summary>
/// Imprime por pantalla los datos de un elemento TAD tipo adapter.
/// El elemento se escoge de una tabla tipo adaptersTable que recibe por parámetro a través de un índice de posición que también
/// se recibe por parámetro.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="indPos">int Índice de posición del adaptador.</param>
void printAdapterInfo(const adaptersTable table, const int indPos);