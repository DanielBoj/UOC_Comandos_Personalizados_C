#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "api.h"
#include "parser.h"
#include "adapter.h"
#include "menu.h"

/// <summary>
/// Realiza la carga de datos iniciales delegando en los m�dulos necesarios.
/// 1. Inicializa la estructura TDA adapter par acargar los datos de un adaptador.
/// 2. Crea el documento XML delegando en la funci�n createXmlDoc().
/// </summary>
/// <param name="retVal">The ret value.</param>
void loadData(adaptersTable* adptrsTbl, int* retVal) {

	*retVal = 0;

	/* Inicializamos la estructura del adaptador. */
	initAdpatersTable(adptrsTbl);

	/* Cargampos el nombre e �ndice de los adaptadores activos */
	loadAdptrsInfo(adptrsTbl, retVal);

	/* Creamos el documento XML con las l�neas de cabecera. */
	createXmlDoc(retVal);
}

/// <summary>
/// Inicializa una estructura tipo TAD 'adapter' que contiene la informaci�n de  un adaptador de red.
/// Realiza la reserva de memoria din�mica para los campos de la estructura mediante malloc e inicializa todas las posiciones de la
/// tabla jumps, reservando la memoria para el campo ip de cada posici�n y seteando el n�mero de salto a 0.
/// Finaliza iniciando el contador de saltos.
/// </summary>
/// <param name="adptr">adapter Estructura tipo TDA para almacenar la informaci�n de un adaptador de red.</param>
void initAdapter(adapter* adptr) {
	adptr->name = (char*)malloc(MAX_ADPTR_INFO);
	adptr->index = -1;
	adptr->ip = (char*)malloc(MAX_IP);
	adptr->mask = (char*)malloc(MAX_IP);
	adptr->gateway = (char*)malloc(MAX_IP);
	adptr->dnsServer = (char*)malloc(MAX_IP);
	adptr->avgRespTime = 0.0;

	for (int i = 0; i < MAX_JUMPS; i++) {
		adptr->jumps[i].ip = (char*)malloc(MAX_IP);
		adptr->jumps[i].jumpNum = 0;
	}

	adptr->totalJumps = 0;
}

/// <summary>
/// Inicializa una tabla de elmentos TAD tipo adpater.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adapter.</param>
void initAdpatersTable(adaptersTable* table) { 
	
	for (int i = 0; i < MAX_ADPTRS_TABLE; i++) {
		table->adptrs[i].name = (char*)malloc(MAX_ADPTR_INFO);
		table->adptrs[i].ip = (char*)malloc(MAX_IP);
		table->adptrs[i].mask = (char*)malloc(MAX_IP);
		table->adptrs[i].gateway = (char*)malloc(MAX_IP);
		table->adptrs[i].dnsServer = (char*)malloc(MAX_IP);
		table->adptrs[i].index = -1;
		table->adptrs[i].avgRespTime = 0.0;
		table->adptrs[i].totalJumps = 0;
	}

	table->numElem = 0; 
}

/// <summary>
/// Parsea la informaci�n de una estructura TAD tipo adpater a un documento XML.
/// Delega en la funci�n fillXmlDoc() del m�dulo parser.
/// </summary>
/// <param name="adptr">adptr Estructura TAD tipo adapter con la informaci�n de un adaptador de red.</param>
/// <param name="retVal">int Deuelve > 0 en caso de error.</param>
void parseAdpatrInfo(const adapter adptr, int* retVal) {

	*retVal = 0;

	fillXmlDoc(adptr, retVal);
}

/// <summary>
/// Driver para llamar al men� principal.
/// </summary>
/// <param name="adptsTbl">adptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="isSalir">bool Booleano de control para la opci�n salir.</param>
/// <param name="adptrPos">int �ndice de posici�n en la tabla de adaptadores del adaptador de red escogido por el usuario.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se ejecutan con �xito o 1 de lo contrario.</param>
void callMenu(adaptersTable* adptsTbl, bool* isSalir, int* adptrInd, int* retVal) {
	
	mainMenu(adptsTbl, isSalir, adptrInd, retVal);

}

/// <summary>
/// Obtine las direcciones diferentes direcciones de un adaptador de red y las actualiza en el elemento correpondiente en la tabla
/// de adaptadores.
/// Delega en las funciones getIpInfo() y getDnsInfo() del m�dulo adapter.
/// </summary>
/// <param name="adptsTbl">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="adptrInd">int �ndice de adaptador, se usa para facilitar la llamada a los comandos CMD.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se ejecutan correctamente o 1 de lo contrario.</param>
void getAdaptersData(adaptersTable* adptsTbl, const int adptrInd, int* retVal) {

	*retVal = 0;

	int indPos = searchAdapater(*adptsTbl, adptrInd, 0, (adptsTbl->numElem - 1));

	/* Obtenemos las diferentes direccions ip del adaptador. */
	getIpInfo(adptsTbl, adptrInd, retVal);

	getDnsTest(adptsTbl, adptrInd, retVal);

	parseAdpatrInfo(adptsTbl->adptrs[indPos], retVal);;
}

/// <summary>
/// Al salir del programa, cierra el documento XML a�adiendo un footer con las etiquetas y sentencias de cierre.
/// Delega en la funci�n closXmlDoc() del m�dulo parser.
/// </summary>
/// <param name="retVal">int Devuelve 0 sei las operaciones se realizan con �xtio o 1 de lo contrario.</param>
void closeData(int* retVal) {

	/* Cerramos el documento XML a�adiendo el footer. */
	*retVal = closeXmlDoc();
}

/// <summary>
/// Realiza una lectura del archivo XML en bruto, mostrando todas las etiquetas.
/// Delega en la funci�n readXmlDoc() del m�dulo parser.
/// </summary>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan con �cito o 1 de lo contrario.</param>
void readData(int* retVal) {

	*retVal = rawReadxmlDoc();
}

/// <summary>
/// Muestra por pantalla el nombre de los adaptadores de red del equipo.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD de tipo adapter.</param>
void printAdaptersName(const adaptersTable table) {

	/* Presentamos la salida de datos. */
	puts("--------------------------------------------------------");
	puts("Num\tNombre Adaptador de Red");
	for (unsigned int i = 0; i < table.numElem; i++) {
		printf_s("%d.\t%s\n", (i + 1), table.adptrs[i].name);
	}
	puts("--------------------------------------------------------");
}

/// <summary>
/// Comprueba si una tabla de estructuras TAD tipo adaptador est� llena..
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <returns>bool Devuelve false si la tabla a�n tiene espacio disponible o true si est� llena.</returns>
bool adaptersTableIsFull(const adaptersTable table) { return (table.numElem == MAX_ADPTRS_TABLE); }

/// <summary>
/// Libera una estructura TAD de tipo adapter.
/// Libera la memoria din�mica reservada para los strings y setea los valores por defecto del resto de campos.
/// </summary>
/// <param name="adptr">adptr Estructura TAD tipo adapter.</param>
void freeAdapter(adapter* adptr) {

	if (adptr->index < 0) {
		return;
	}
	else {
		/* Liberamos la estructura principal. */
		free(adptr->name);
		adptr->index = 0;
		free(adptr->ip);
		free(adptr->mask);
		free(adptr->gateway);
		free(adptr->dnsServer);
		adptr->avgRespTime = 0.0;

		/* Liberamos la informaci�n de los saltos. */
		for (unsigned int i = 0; i < adptr->totalJumps; i++) {
			if (adptr->jumps[i].ip) {
				adptr->jumps[i].ip = NULL;
				free(adptr->jumps[i].ip);
			}
			
			adptr->jumps[i].jumpNum = 0;
		}

		adptr->totalJumps = 0;
	}
}

/// <summary>
/// Limpia todos los elementos de una tabla que recibe por par�metro.
/// Es una funci�n recursiva, recorre toda la tabla desde los puntos de inicio y final que recibe por par�metros.
/// Se libera la memoria de los campos string mediante el comando free() y se asigna el valor por defecto al resto de campos.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="start">Primera posici�n de la tabla.</param>
/// <param name="end">�ltima posici�n de la tabla.</param>
int clearTable(adaptersTable* table, const int start, const int end) {

	/* Caso base. */
	if (end < start) {

		table->numElem = 0;
		return 0;
	}

	/* Liberamos los campos de la tabla. */
	freeAdapter(&table->adptrs[end]);

	return clearTable(table, start, (end - 1));
}