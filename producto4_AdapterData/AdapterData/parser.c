#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

/// <summary>
/// Crea el documento XML para almacenar la información de un adaptador de red y genera las líneas de cabecera.
/// Realiza el manejo de errores a través del parámetro retVal, si no se puede crear el archivo o falla la escritura en este, el valor de retorno
/// será > 0.
/// </summary>
/// <param name="retVal">int Si se produce un error, devolverá > 0.</param>
void createXmlDoc(int* retVal) {

	*retVal = 0;

	int res = 0;
	errno_t err;
	FILE* pXmlDoc = NULL;

	/* Creamos el archivo mediante la apertura de este en modo +. */
	err = fopen_s(&pXmlDoc, XML_DOC_PATH, O_WRITE);

	/* Comprobamos que se haya generado correctamente. */
	if (err == 0) {

		/* Creamos la cabecera */
		res = fputs(XML_DOC_HEADER, pXmlDoc);
	}
	else {
		puts("Error: No se ha podido crear el archio!");
		*retVal = 1;
	}

	/* Manejamos posibles errores de escritura. */
	if (res > 0) {
		puts("Se ha producido un error en la escritura del archivo.");
		*retVal = 1;
	}

	if (pXmlDoc) {
		fclose(pXmlDoc);
	}
}

/// <summary>
/// Toma los datos de una estrucutura TAD tipo adapter con la información de un adaptador y rellena un archivo XML con estos.
/// 1. La función accede a un archivo XML previamente generado.
/// 2. Accede a los diferentes campos de la estructura y va rellenando el archivo con ellos, para seguir la sintaxis de un archivo .xml
/// usa las etiquetas de apertura y cierre, estas etiquetas se han creado como constantes en el archivo de cabeceras para poder modificarlas
/// más fácilmente en caso necesario.
/// 3. Se realiza una gestión de posibles errores mediante un parámetro de entrada y salida, retVal que devuelve > 0 en caso de error.
/// </summary>
/// <param name="adptr">adptr Estructura TAD tipo adapter.</param>
/// <param name="retVal">int Devuelve > 0 en caso de error.</param>
void fillXmlDoc(const adapter adptr, int* retVal) {

	*retVal = 0;

	int res = 0;
	errno_t err;

	FILE* pXmlDoc = NULL;

	/* Creamos el archivo mediante la apertura de este en modo +. */
	err = fopen_s(&pXmlDoc, XML_DOC_PATH, O_APPEND);

	/* Comprobamos que se haya generado correctamente. */
	if (err == 0) {

		/* Generamos las líneas del archivo */
		res = fprintf_s(pXmlDoc, "\t%s%s%s\n", T_NAME_OPEN, adptr.name, T_NAME_CLOSE);
		res = fprintf_s(pXmlDoc, "\t%s%s%s\n", T_IP_OPEN, adptr.ip, T_IP_CLOSE);
		res = fprintf_s(pXmlDoc, "\t%s%s%s\n", T_MASK_OPEN, adptr.mask, T_MASK_CLOSE);
		res = fprintf_s(pXmlDoc, "\t%s%s%s\n", T_GATEWAY_OPEN, adptr.gateway, T_GATEWAY_CLOSE);
		res = fprintf_s(pXmlDoc, "\t%s%s%s\n", T_DNS_OPEN, adptr.dnsServer, T_DNS_CLOSE);
		res = fprintf_s(pXmlDoc, "\t%s%s\n", T_JUMP_OPEN, "Informacion de los saltos:");
		
		for (unsigned int i = 0; i < adptr.totalJumps; i++) {
			res = fprintf_s(pXmlDoc, "\t\t%s%d%s\n", T_JUMP_NUM_OPEN, adptr.jumps[i].jumpNum, T_JUMP_NUM_CLOSE);
			res = fprintf_s(pXmlDoc, "\t\t%s%s%s\n", T_IP_OPEN, adptr.jumps[i].ip, T_IP_CLOSE);
		}

		res = fprintf_s(pXmlDoc, "\t%s\n", T_JUMP_CLOSE);
	}
	else {
		puts("Error: No se ha podido crear el archio!");
		*retVal = 1;
	}

	/* Manejamos posibles errores de escritura. */
	if (res < 0) {
		puts("Se ha producido un error en la escritura del archivo.");
		*retVal = 1;
	}

	if (pXmlDoc) {
		fclose(pXmlDoc);
	}
}

/// <summary>
/// Cierra el documento XML añadiendo la etiqueta de cierra.
/// La función maneja la apertura del archivo en formato append par añadir el cierre mediante el uso de una constante.
/// </summary>
/// <returns>int Devuekve > 1 si se produce algún error.</returns>
int closeXmlDoc() {

	int res = 0;
	errno_t err;

	FILE* pXmlDoc = NULL;

	/* Creamos el archivo mediante la apertura de este en modo +. */
	err = fopen_s(&pXmlDoc, XML_DOC_PATH, O_APPEND);

	/* Comprobamos que se haya generado correctamente. */
	if (err == 0) {

		/* Añadimos la etiqueta de cierre. */
		res = fputs(XML_DOC_FOOTER, pXmlDoc);

		if (res < 0) {
			puts("Se ha producido un error de escritura.");
		}
	}
	else {
		puts("Error: No es posible acceder al documento.");
		res = -1;
	}

	if (err < 0) {
		res = 1;
	}

	return res;
}

/// <summary>
/// Abre el archivo XML creado por el programa y lo presenta directamente por pantalla al usuario.
/// Usa un puntero a FILE y realiza la lectura del archivo línea a línea.
/// </summary>
/// <returns>int Devuelve 0 si las operaciones se realizan de forma correcta o 1 de lo contrario.</returns>
int rawReadxmlDoc() {

	int retVal = 0;

	FILE* fXmlDoc = NULL;
	char* line;
	errno_t err;

	line = (char*)malloc(MAX_LINE);

	err = fopen_s(&fXmlDoc, XML_DOC_PATH, O_READ);

	if (err == 0 ) {

		while (!feof(fXmlDoc)) {

			line[0] = '\0';

			if (fgets(line, MAX_LINE - 1, fXmlDoc) != NULL) {

				if (line != NULL) {
					printf_s("%s", line);
				}
			}
		}
	}
	else {
		puts("Error: No puede abrirse el archivo.");

		retVal = 1;
	}

	/* Añadimos el footer a la presentación. */
	puts(XML_DOC_FOOTER);

	if (fXmlDoc) {
		fclose(fXmlDoc);
	}

	if (line) {
		free(line);
	}

	return retVal;
}