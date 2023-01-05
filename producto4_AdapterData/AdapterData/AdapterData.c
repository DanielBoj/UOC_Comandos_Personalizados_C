/*
*	Programa: Adapter Data
*	Autor: Boj Cobos, Daniel
*	Version: 0.1
*	2022/12
*	Licencia Libre
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "data.h"
#include "api.h"

/// <summary>
/// Driver para controlar el flujo de ejecución del programa.
/// </summary>
/// <returns>int Devuelve 0 si las operaciones se ejecutan con éxito o 1 d elo contrario.</returns>
int mainDriver();

int main() {

	int retValue = 0;

	retValue = mainDriver();

	return retValue;
}

int mainDriver() {

	int retVal, adptrInd;
	bool isSalir;
	adaptersTable adptsTbl;

	/* Inicialización de variables. */
	retVal = 0;
	adptrInd = -1;
	isSalir = false;

	puts("=======================================================\n");
	puts("Bienvenido al programa XML Adapter Info.\n");
	puts("=======================================================\n");

	loadData(&adptsTbl, &retVal);

	while (!isSalir) {
		callMenu(&adptsTbl, &isSalir, &adptrInd, &retVal);
	}

	retVal = clearTable(&adptsTbl, 0, adptsTbl.numElem);

	puts("=======================================================\n");
	puts("Saliendo del programa.\nHasta pronto.\n");
	puts("=======================================================\n");

	return retVal;
}