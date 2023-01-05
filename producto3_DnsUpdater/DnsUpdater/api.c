#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "data.h"
#include "dns.h"

/// <summary>
/// Inicializa una estructura de tipo TAD "adapter" que contiene la información de un adaptador de
/// red obtenida de la lectura del retorno de un comando NETSH. Asigna la memoria del campo name que
/// contendrá el nombre del adaptador e inicializa el resto de campos a valores por defecto.
/// </summary>
/// <param name="adptr"></param>
void initAdptr(adapter* adptr) {
	adptr->name = (char*)malloc(MAX_ADTR_LINE);
	adptr->name[0] = '\0';

	adptr->id = -1;
	adptr->metric = -1;
	adptr->mtu = -1;
	adptr->state = false;
}

/// <summary>
/// Inicializa una tabla adptsTable que conttendrá esturcturas TAD tipo "adaptador" con la información
/// de los adaptadores de red de un equipo.
/// </summary>
/// <param name="table"></param>
void intAdptrTable(adptsTable* table) {
	table->numElem = 0;
}

/// <summary>
/// Inicializa una tabla dnsTable que contendrá las diferentes direcciones DNS.
/// </summary>
/// <param name="table"></param>
void initDnsTable(dnsTable* table) {
	for (int i = 0; i < DNS_TABLE_LENGHT; i++) {
		table->dns[i] = (char*)malloc(MAX_DNS_LINE);
	}
	table->numElem = 0;
}

/// <summary>
/// Inicializa una estructura TAD tipo dnsTest.
/// </summary>
/// <param name="src">dnsTest Estructura origen a inicializar.</param>
void initDnsTest(dnsTest* src) {
	src->dir = NULL;
	src->jumps = 0;
	src->avrgTime = 0.0;
}

/// <summary>
/// Inicializa una tabla dnsTestTable que contendrá direcciones DNS y el resultado del test de velocidad.
/// </summary>
/// <param name="table">The table.</param>
void initDnsTestTable(dnsTestTable* table) {
	for (int i = 0; i < DNS_TABLE_LENGHT; i++) {
		table->dirs[i].dir = NULL;
		table->dirs[0].avrgTime = 0.0;
		table->dirs[0].jumps = 0;
	}
	table->numElem = 0;
}

/// <summary>
///Inicializa y cargamos todos los datos para la implementación de la aplicación antes de iniciar el menú principal.
/// Generamos la información de los adaptadores y la cargamos al programa.
/// Obtenemos la ruta del archivo con la lista de direcciones DNS y la cargamos al programa.
/// Mostramos la lista de direcciones al usuario.
/// </summary>
/// <param name="adptsTab">adptsTable Tabla que almacena estructuras TAD de tipo "adapter".</param>
/// <param name="dnsTab">dnsTable Tabla que almacena Strings que contienen las direcciones DNS.</param>
/// <param name="retVal">int int Devuelve 0 si la operaicón se realiza con éxito, o 1 si falla.</param>
void loadData(adptsTable *adptsTab, dnsTable *dnsTab, int *retVal) {

	*retVal = 0;

	char *dnsPath;

	puts("---------------------------------------------------------------\n");
	puts("Carga de los datos iniciales.");

	dnsPath = (char*)malloc(MAX_LINE);
	dnsPath[0] = '\0';

	 /* Creamos el log de adaptadores. */
	createAdptsLog();

	/* Inicializamos y cargamos en la tabla de adaptadores. */
	adptsLoad(adptsTab, retVal);

	/* Obtenemos la ruta del archivo que contiene las direcciones DNS. 
	* Preguntamos los datos de la ruta del archivo con las direcciones DNS al usaurio, incializamos la tabla y creamos la ruta absoluta.
	*/
	dnsPath = askPath();
	
	/* Inicializamos y cargamos los datos en la tabla de DNS. */
	dnsLoad(dnsTab, dnsPath, retVal);

	/* Mostramos las direcciones DNS por pantalla. */
	printDnsTable(*dnsTab);

	/* Liberamos la memoria temporal. */
	if (dnsPath) {
		free(dnsPath);
	}
}

/// <summary>
/// Solitamos los datos de la ruta de acceso al archivo que contiene las direcciones DNS al usuario.
/// </summary>
/// <returns>path Estructura TAD que contiene la información con una ruta de acceso al archivo de direcciones DNS.</returns>
char* askPath() {

	char* askValue;
	char* rutaDns;

	/* Inicializamos la estructura TAD tipo "path" y la variable temporal. */
	askValue = (char*)malloc(MAX_LINE);
	rutaDns = (char*)malloc(MAX_LINE);
	askValue[0] = '\0';
	rutaDns[0] = '\0';

	/* Preguntamos los datos al usuario. */
	puts("---------------------------------------------------------------\n");
	puts("Por favor, indique la ruta de acceso al archivo, debe usar / para separar los directorios, no use \\.\nPor ejemplo, C:/dev/");
	rewind(stdin);
	fgets(askValue, MAX_LINE, stdin);
	strtok(askValue, "\n");
	if (askValue) {
		strncat(rutaDns, askValue, strnlen(askValue, MAX_LINE));
	}

	/* Purgamos la variable temporal. */
	askValue[0] = '\0';

	puts("Por favor, indique el nombre del archivo con su tipo.\nPor ejemplo, mifichero.txt");
	rewind(stdin);
	fgets(askValue, MAX_LINE, stdin);
	strtok(askValue, "\n");
	if (askValue) {
		strncat(rutaDns, askValue, strnlen(askValue, MAX_LINE));
	}
	
	return rutaDns;
}

/// <summary>
/// Obtiene los datos de las direcciones IP asociadas a un adaptador escogido por el usuario.
/// Delega en la función getDnsips() de dns.c.
/// </summary>
/// <param name="table">adptsTable Tabla de TAD de tipo adapter con la información de los adaptadores de red.</param>
/// <param name="adptrNum">Número relativo del adaptador de red escogido por el usuario sobre una lista de adaptadores activos.</param>
/// <param name="adptrInd">Parámetro de salida que devuelve el índice de posición del adaptador escogido por el usuario en la tabla de adaptadores.</param>
/// <param name="retVal">Devuelve 0 si la operación se ejecuta con éxito o 1 si falla.</param>
void showDnsIps(const adptsTable table, const int adptrNum, dnsTable* adptrDnsSrvs, int* adptrInd, int* retVal) {

	*retVal = 0;

	getDnsIps(table, adptrNum, adptrDnsSrvs, adptrInd, retVal);	
}

/// <summary>
/// Realiza los tests de accesibilidad a los servidores DNS del adaptador escogido por el usuario, añade los servidores accesibles a la lista de DNS
/// a las que realizar el test de velocidad, realiza el test de velocidad, selecciona el servidor más rápido y ejecuta el comando de cambio de DNS a
/// través de un comando CMD netsh. 
/// Delega en la función testDnsServers() de dns.c.
/// </summary>
/// <param name="adptsTbl">adptsTable Tabla de elementos TAD tipo adapter con la información de los adaptadores del equipo.</param>
/// <param name="dnsDirs">dnsTable Tabla de DNS con las direcciones DNS obtenidas del archio .txt externo.</param>
/// <param name="adptrDnsSrvs">dnsTable Tabla de DNS con las direcciones obtenidas del adaptador escogido por el usuario.</param>
/// <param name="adprInd">int Índice de posición del adaptador escogido por el usuario.</param>
/// <param name="retVal">Devuelve 0 si las operaciones se ejecutan con éxtio, o 1 de lo contrario.</param>
void getDnsTests(const adptsTable adptsTbl, const dnsTable dnsDirs, const dnsTable adptrDnsSrvs, const int adprInd, int* retVal) {
	
	*retVal = 0;

	testDnsServers(adptsTbl, dnsDirs, adptrDnsSrvs, adprInd, retVal);
}

/// <summary>
/// Elimina los archivos log temporales.
/// </summary>
void eraseLogs() {
	remove(ADPTS_LOG_PATH);
	remove(DNS_LOG_PATH);
}