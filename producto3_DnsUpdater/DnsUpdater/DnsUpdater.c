#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "menu.h"

/// <summary>
/// Driver para el control de flujo del programa.
/// </summary>
/// <returns>int Devuelve 0 en caso de que las operaciones se realican con éxito, o 0 de lo contrario.</returns>
int mainDriver();

int main() {

	int retVal = 0;
	
	retVal = mainDriver();

	return retVal;
}

/// <summary>
/// Driver para el control de flujo del programa.
/// </summary>
/// <returns>int Devuelve 0 en caso de que las operaciones se realican con éxito, o 0 de lo contrario.</returns>
int mainDriver() {

	int retVal, adptrInd;
	adptsTable adptsTbl;
	dnsTable dnsDirs, adptrDnsSrvs;
	bool isSalir;

	retVal = 0;
	adptrInd = -1;
	isSalir = false;

	puts("---------------------------------------------------------------\n");
	puts("Bienvenido al programa de Actualizacion de direccion DNS.\n");

	loadData(&adptsTbl, &dnsDirs, &retVal);

	while (!isSalir) {
		callMenu(adptsTbl, dnsDirs, &adptrDnsSrvs, &isSalir, &adptrInd, &retVal);
	}

	/* Limpiamos las tablas. */
	clearAdptsTable(&adptsTbl);
	clearDnsTable(&dnsDirs);
	clearDnsTable(&adptrDnsSrvs);

	/* Borramos los logs. */
	eraseLogs();

	return retVal;
}