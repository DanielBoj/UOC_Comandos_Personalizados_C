// Nos aseguramos de linkar una �nica vez la librer�a
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "dns.h"

/// <summary>
/// Inicializa una estructura de tipo TAD "adapter" que contiene la informaci�n de un adaptador de
/// red obtenida de la lectura del retorno de un comando NETSH. Asigna la memoria del campo name que
/// contendr� el nombre del adaptador.
/// </summary>
/// <param name="adptr"></param>
void initAdptr(adapter* adptr);

/// <summary>
/// Inicializa una tabla adptsTable que conttendr� esturcturas TAD tipo "adaptador" con la inormaci�n
/// de los adaptadores de red de un equipo.
/// </summary>
/// <param name="table"></param>
void intAdptrTable(adptsTable* table);

/// <summary>
/// Inicializa una estructura TAD tipo dnsTest.
/// </summary>
/// <param name="src">dnsTest Estructura origen a inicializar.</param>
void initDnsTest(dnsTest* src);

/// <summary>
/// Inicializa una tabla dnsTable que contendr� las diferentes direcciones DNS.
/// </summary>
/// <param name="table"></param>
void initDnsTable(dnsTable* table);

/// <summary>
/// Inicializa una tabla dnsTestTable que contendr� direcciones DNS y el resultado del test de velocidad.
/// </summary>
/// <param name="table">The table.</param>
void initDnsTestTable(dnsTestTable* table);

/// <summary>
///Inicializa y cargamos todos los datos para la implementaci�n de la aplicaci�n antes de iniciar el men� principal.
/// Generamos la informaci�n de los adaptadores y la cargamos al programa.
/// Obtenemos la ruta del archivo con la lista de direcciones DNS y la cargamos al programa.
/// Mostramos la lista de direcciones al usuario.
/// </summary>
/// <param name="adptsTab">adptsTable Tabla que almacena estructuras TAD de tipo "adapter".</param>
/// <param name="dnsTab">dnsTable Tabla que almacena Strings que contienen las direcciones DNS.</param>
/// <param name="retVal">int int Devuelve 0 si la operaic�n se realiza con �xito, o 1 si falla.</param>
void loadData(adptsTable* adptsTab, dnsTable* dnsTab, int* retVal);

/// <summary>
/// Solitamos los datos de la ruta de acceso al archivo que contiene las direcciones DNS al usuario.
/// </summary>
/// <returns>path Estructura TAD que contiene la informaci�n con una ruta de acceso al archivo de direcciones DNS.</returns>
char* askPath();

/// <summary>
/// Obtiene los datos de las direcciones IP asociadas a un adaptador escogido por el usuario.
/// Delega en la funci�n getDnsips() de dns.c.
/// </summary>
/// <param name="table">adptsTable Tabla de TAD de tipo adapter con la informaci�n de los adaptadores de red.</param>
/// <param name="adptrNum">N�mero relativo del adaptador de red escogido por el usuario sobre una lista de adaptadores activos.</param>
/// <param name="adptrInd">Par�metro de salida que devuelve el �ndice de posici�n del adaptador escogido por el usuario en la tabla de adaptadores.</param>
/// <param name="retVal">Devuelve 0 si la operaci�n se ejecuta con �xito o 1 si falla.</param>
void showDnsIps(const adptsTable table, const int adptrNum, dnsTable* adptrDnsSrvs, int* adptrInd, int* retVal);

/// <summary>
/// Realiza los tests de accesibilidad a los servidores DNS del adaptador escogido por el usuario, a�ade los servidores accesibles a la lista de DNS
/// a las que realizar el test de velocidad, realiza el test de velocidad, selecciona el servidor m�s r�pido y ejecuta el comando de cambio de DNS a
/// trav�s de un comando CMD netsh. 
/// Delega en la funci�n testDnsServers() de dns.c.
/// </summary>
/// <param name="adptsTbl">adptsTable Tabla de elementos TAD tipo adapter con la informaci�n de los adaptadores del equipo.</param>
/// <param name="dnsDirs">dnsTable Tabla de DNS con las direcciones DNS obtenidas del archio .txt externo.</param>
/// <param name="adptrDnsSrvs">dnsTable Tabla de DNS con las direcciones obtenidas del adaptador escogido por el usuario.</param>
/// <param name="adprInd">int �ndice de posici�n del adaptador escogido por el usuario.</param>
/// <param name="retVal">Devuelve 0 si las operaciones se ejecutan con �xtio, o 1 de lo contrario.</param>
void getDnsTests(const adptsTable adptsTbl, const dnsTable dnsDirs, const dnsTable adptrDnsSrvs, const int adprInd, int* retVal);

/// <summary>
/// Elimina los archivos log temporales.
/// </summary>
void eraseLogs();