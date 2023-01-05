#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

/// <summary>
/// Crea el documento XML para almacenar la informaci�n de un adaptador de red y genera las l�neas de cabecera.
/// Realiza el manejo de errores a trav�s del par�metro retVal, si no se puede crear el archivo o falla la escritura en este, el valor de retorno
/// ser� > 0.
/// </summary>
/// <param name="retVal">int Si se produce un error, devolver� > 0.</param>
void createXmlDoc(int* retVal);

/// <summary>
/// Toma los datos de una estrucutura TAD tipo adapter con la informaci�n de un adaptador y rellena un archivo XML con estos.
/// 1. La funci�n accede a un archivo XML previamente generado.
/// 2. Accede a los diferentes campos de la estructura y va rellenando el archivo con ellos, para seguir la sintaxis de un archivo .xml
/// usa las etiquetas de apertura y cierre, estas etiquetas se han creado como constantes en el archivo de cabeceras para poder modificarlas
/// m�s f�cilmente en caso necesario.
/// 3. Se realiza una gesti�n de posibles errores mediante un par�metro de entrada y salida, retVal que devuelve > 0 en caso de error.
/// </summary>
/// <param name="adptr">adptr Estructura TAD tipo adapter.</param>
/// <param name="retVal">int Devuelve > 0 en caso de error.</param>
void fillXmlDoc(const adapter adptr, int* retVal);

/// <summary>
/// Cierra el documento XML a�adiendo la etiqueta de cierra.
/// La funci�n maneja la apertura del archivo en formato append par a�adir el cierre mediante el uso de una constante.
/// </summary>
/// <returns>int Devuekve > 1 si se produce alg�n error.</returns>
int closeXmlDoc();

/// <summary>
/// Abre el archivo XML creado por el programa y lo presenta directamente por pantalla al usuario.
/// Usa un puntero a FILE y realiza la lectura del archivo l�nea a l�nea.
/// </summary>
/// <returns>int Devuelve 0 si las operaciones se realizan de forma correcta o 1 de lo contrario.</returns>
int rawReadxmlDoc();