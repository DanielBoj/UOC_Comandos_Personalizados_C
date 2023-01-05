#pragma once // Nos aseguramos de que la librer�a solo pueda incluirse una vez en la compilaci�n.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MAX_CHAR
#define MAX_CHAR 100		// Tama�o m�ximo de car�cteres en un string por defecto
#endif
#ifndef MAX_IPADRR_LNG
#define MAX_IPADRR_LNG 15	// Tamalo m�ximo de car�cteres en un string de IP tipo 000.000.000.000
#endif
#ifndef IP_TBL_SIZE
#define IP_TBL_SIZE 100		// Tama�o m�ximo de Ips que caben en la tabla de direcciones.
#endif
#ifndef FIRST_ELEM_IN_ARRAY
#define FIRST_ELEM_IN_ARRAY 0 // �ndice de posici�n del primer elemento de un array
#endif
#ifndef O_LECTURA
#define O_LECTURA "r"		// Opci�n de mode de apertura del archivo solo lectura.
#endif
#ifndef O_ESCRITURA
#define O_ESCRITURA "w+"
#endif // Modo de apertura de un archivo creaci�n y escritura.
#ifndef O_APPEND
#define O_APPEND "a"
#endif // !O_APPEND


/**
 * @brief Estructura para almacenar una direcci�n IP.
 *
 */
typedef char* ipAddr;

/**
 * @brief Estructura para almacenar un array de direcciones IP.
 *
 */
typedef struct tTablaIpAddr
{
    ipAddr addresses[IP_TBL_SIZE];
    int pingResults[IP_TBL_SIZE];
    int numElem;
} tablaIpAddr;

/**
 * @brief initTablaIpAdrr(): Inicializa un TAD de tabla de direcciones IP que recibe por par�metro. Todas las direcciones se inicializan a -1 para
 *
 * @param tabla Estructura TAD de elementos tipo ipAddr.
 */
void initTablaIpAdrr(tablaIpAddr* tabla);

/**
 * @brief getDatos(): Funci�n que le solicita al usuario la ruta absoluta de acceso al archivo de texto que comtiene las direcciones IP.
 *
 * @param rutaAbasoluta String Variable in/out a la que asignamos la ruta abasoluta.
 */
void getDatos(char rutaAbasoluta[]);

/**
 * @brief getDatosIp(): Funci�n que le solicita al usuario la ruta absoluta de acceso al archivo de texto que comtiene las direcciones IP
 * y la nueva IP.
 *
 * @param ruta String Variable in/out a la que asignamos la ruta abasoluta.
 * @param ip String Variable in/out a la que asignamos la nueva IP.
 */
void getDatosIp(char ruta[], char ip[]);

/**
 * @brief openFile(): Funci�n que abre un archivo externo y retorna un puntero al archivo.
 *
 * @param nombre String Ruta absoluta al archivo.
 * @param modo String Modo de apertura.
 * @return FILE* Puntero a FILE al archivo.
 */
FILE* openFile(const char* nombre, const char* modo);

/**
 * @brief listIp(): Funci�n que consulta el archivo con las direcciones IP, las muestra por pantalla, las almacena en una structura TDA tablaIpAddr y retorna la tabla.
 *
 * @param rutaAbsoluta String Ruta abasoluta del archivo.
 * @param modo String Modo de apertura
 * @return tablaIpAddr Structura TDA que almacena las direcciones IP.
 */
tablaIpAddr listIp(char rutaAbsoluta[], char modo[]);

/**
 * @brief pingIp(): Usa la funci�n system() para ejecutar un comando de PING a la direcci�n IP que recibe por par�metro. Para ello,
 * usa funciiones de concatenaci�n de strings para agragar la IP a la definici�n de comando p�r defecto. Se realiza el env�o de un
 * paquete de datos y evitamos que se muestre la ejecuci�n por terminal envi�ndola a NUL (equivalente de /dev/nul de Linux). Evitamos
 * tambi�n concatenar saltos de l�nea limpiando el car�vte \\n si se encuentra en el buffer. Si el ping se realiza con �xito, el
 * comando retorna 0, si no, 1.
 *
 *
 * @param ipAddr String Direcci�n IP a la que realizar el ping.
 * @return int Respuesta de retorno del comando.
 */
int pingIp(const char* ipAddr);

/**
 * @brief getPingResponse: Obtiene el valor de respuesta de un comando de
 * PING lanzado a cada una de las direcciones IP almacenadas en una tabla
 * de direcciones tipo tablaipAddr.
 * Es una funci�n recursiva que recorre la tabla hacia adelante.
 *
 * @param tabla tablaIpAddr Estructura para contener las direcciones IP y
 *  los resultados del response.
 * @param end int Indice de posici�n final.
 * @param ind int �ndice de posici�n del primer elemento.
 */
int getPingResponse(tablaIpAddr* tabla, const int end, const int ind);

/**
 * @brief printPingResult(): Imprime por pantalla los resultados del ping a las ips
 *
 * @param tabla tablaIpAddr Estructura TDA que contiene las direcciones IP y las respuestas de estas
 * al pint.
 */
void printPingResult(const tablaIpAddr tabla);

/**
 * @brief clearTable: Libera la memoria y elimina los registros de una tabla.
 * Es una funci�n recursiva.
 *
 * @param table Par�metro tablaIpAdrr de entrada y salida que queremos borrar.
 * @param ind Indice de posici�n del �ltimo elemento.
 */
int clearTable(tablaIpAddr* table, const int ind);

/**
 * @brief addIp(): A�ade una nueva direcci�n IP al archivo que recibe por par�metro, la direcci�n
 * tambi�n se recibe por par�metro.
 *
 * @param ruta String ruta del archivo de ips.
 * @param nombre String nombre dle archivo de ips.
 * @param ipDir String nueva direcci�n ip.
 */
void addIp(const char ruta[], const char ipDir[]);

/**
 * @brief pingDriver(): Es la funci�on driver para manejar el m�dulo.
 *
 */
int ipPingDriver();

/**
 * @brief pingDriver(): Es la funci�n driver para manejar la adici�n de IPs.
 *
 */
void addIpDriver();