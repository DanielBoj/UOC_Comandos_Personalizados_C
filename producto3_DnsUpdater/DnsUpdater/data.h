// Nos aseguramos de linkar una �nica vez la librer�a
#pragma once

#include <stdbool.h>

/* Longitudes y tama�os m�ximos */

/* #ifndef nos sirve para evitar que se declare una constante m�s de una vez.
* Define la longitud m�xima en car�cteres (bytes) que tendr�
* una lectura de archivo 
*/
#ifndef MAX_LINE
#define MAX_LINE 255
#endif // !MAX_READ_LENGHT

/* Define la longitud m�xima en car�cteres que tendr� una direcci�n DNS. */
#ifndef MAX_DNS_LINE
#define MAX_DNS_LINE 30
#endif // !MAX_DNS_LINE

/* Tama�o m�ximo de la tabla que contiene la informaci�n de los adaptadores. */
#ifndef ADPTRS_TABLE_LENGHT
#define ADPTRS_TABLE_LENGHT 100
#endif // !ADPTRS_TABLE_LENGHT

/* Tama�o m�ximo de la tabla que contiene las direcciones. */
#ifndef DNS_TABLE_LENGTH
#define DNS_TABLE_LENGHT 100
#endif // !DNS_TABLE_LENGTH

/* Car�cteres m�ximos a capturar como palabra en el test de velocidad. */
#ifndef MAX_WORD
#define MAX_WORD 20
#endif // !MAX_WORD

/* Definimos el m�ximo de car�cteres que puede tener cada atributo de la estructura
* path que contiene los datos para localizar la ruta absoluta de un archivo.
*/
#ifndef MAX_PATH_BLOCK
#define MAX_PATH_BLOCK 100
#endif // !MAX_PATH_BLOCK

#ifndef MAX_ADPTR_LINE
#define MAX_ADTR_LINE 100
#endif // !MAX_ADPTR_LINE

/* Definimos el m�ximo de car�cteres que puede tener una l�nea de comando. */
#ifndef MAX_COMMAND
#define MAX_COMMAND 250
#endif // !MAX_COMMAND

/* COMANDOS */

/* Obtenci�n de la informaci�n de los adaptadores de red del equipo. */
#ifndef ADPTS_COMMAND
#define ADPTS_COMMAND "netsh interface ipv4 show interfaces > "
#endif // !ADPTS_COMMAND

/* Obtenci�n de las direcciones DNS de un adaptador. */
#ifndef DNS_DIRS_COMMAND
#define DNS_DIRS_COMMAND "netsh interface ipv4 show dnsservers "
#endif // !DNS_DIRS_COMMAND

/* Comprobaci�n de las rutas de acceso a las direcciones DNS. */
#ifndef DNS_TRACER_COMMAND
#define DNS_TRACER_COMMAND "tracert "
#endif // !DNS_TRACER_COMMAND

/* Asignaci�n de n�mer m�ximo de saltos. */
#ifndef TRACE_2_MAX_JUMPS
#define TRACE_2_MAX_JUMPS "-h 2 -w 1000"
#endif // !TRACE_2_MAX_JUMPS

/* Comprobaci�n del n�mero de saltos y de los tiempos de espera de las direcciones DNS. */
#ifndef TRACE_MAX_1000MSEC
#define TRACE_MAX_1000MSEC "tracert -w 1000 "
#endif // !TRACE_MAX_1000MSEC

/* Asignaci�n de un nuevo servidor DNS. */
#ifndef ASSIGN_DNS_SERVER_1
#define ASSIGN_DNS_SERVER_1 "netsh interface ipv4 set dnsserver "
#endif // !ASSIGN_DNS_SERVER

#ifndef ASSIGN_DNS_SERVER_2
#define ASSIGN_DNS_SERVER_2 "static "
#endif // !ASSIGN_DNS_SERVER_2

/* PATHS */

/* Ruta para generar el log que guarda la informaci�n de los adaptadores de red. */
#ifndef ADPTS_LOG_PATH
#define ADPTS_LOG_PATH "./adptslog.txt"
#endif // !ADPTS_LOG_PATH

/* Ruta para generar el log con las diferentes direcciones DNS de un adaptador. */
#ifndef DNS_LOG_PATH
#define DNS_LOG_PATH "./dnslog.txt"
#endif // !DNS_LOG_PATH

/* Modos de apertura de archivos */

/* Modo lectura. */
#ifndef O_READ
#define O_READ "r"
#endif // !O_READ
 
/* Modo escritura tipo agregar +, crea el archivo si no existe. */
#ifndef O_APPEND
#define O_APPEND "a+"
#endif // !O_APPEND

/* Control de Archivos de lectura */

/* L�neas hasta llegar a la informaci�n en el log de los adaptadores de red.*/
#ifndef ADPTS_LOG_NULL_LINES
#define ADPTS_LOG_NULL_LINES 4
#endif // !ADPTS_LOG_NULL_LINES

/* L�neas hasta llegar a la informaci�n en el log de las direcciones DNS de un adaptador de red. */
#ifndef DNS_DIRS_LOG_NULL_LINES
#define DNS_DIRS_LOG_NULL_LINES 2
#endif // !DNS_DIRS_LOG_NULL_LINES

/* L�neas hasta llegar a la informaci�n que nos interesa en la ejecuci�n del comando para testar la velocidad de los servidores DNS. */
#ifndef SPEED_TEST_NULL_LINES
#define SPEED_TEST_NULL_LINES 3
#endif // !SPEED_TEST_NULL_LINES

/* Lineas hasta llegar a la informaci�n en el log del test tracert a las direcciones DNS. */
#ifndef DNS_TEST_NULL_LINES
#define DNS_TEST_NULL_LINES 3
#endif // !DNS_TEST_NULL_LINES

 
/* Palabra clave para identificar el fin de las direccones DNS en el archivo log. */
#ifndef DNS_LOG_END_KEY
#define DNS_LOG_END_KEY "Registrar"
#endif // !DNS_LOG_END_KEY

/* Car�cter clave para encontrar la primera direcci�n DNS en el log con las direcciones DNS de un adaptador de red. */
#ifndef DNS_LOG_CONTROL_KEY
#define DNS_LOG_CONTROL_KEY ":"
#endif // !DN_LOG_CONTROL_KEY

/* Palabra clave para identificar que una servidor DNS no es accesible. */
#ifndef DNS_TEST_NOT_RECHEABLE_KEY_WORD
#define DNS_TEST_NOT_RECHEABLE_KEY_WORD "inaccesible"
#endif // !DNS_TEST_NOT_RECHEABLE_KEY_WORD

/* Palabra clave para identificar el fina de lectura de datos en un test de velocidad. */
#ifndef DNS_TEST_DELIM
#define DNS_TEST_DELIM " "
#endif // !DNS_TEST_DELIM

#ifndef DNS_SPEED_TEST_KEY_CHAR
#define DNS_SPEED_TEST_KEY_CHAR "*"
#endif // !DNS_SPEED_TEST_KEY_CHAR


/// <summary>
/// Esturctura de tipo TAD que contine la informaci�n de un adaptador que obtendremos a
/// trav�s d ela lectura del retorno de un comando NETSH.
/// id: Identificador del adaptador.
/// metric: M�trica del adaptador (Coste asociado al uso de la ruta del adaptador).
/// mtu: Tama�o m�ximo de de paquete que puede ser enviado.
/// state: Indica si el adaptador est� conectado.
/// name: Nombre del adaptador.
/// </summary>
typedef struct tAdapter {
	unsigned int id;
	unsigned int metric;
	unsigned long mtu;
	bool state;
	char* name;
} adapter;

/// <summary>
/// Tabla para almacenar TAD del tipo adaptador.
/// </summary>
typedef struct tAdptsTable {
	adapter adptr[ADPTRS_TABLE_LENGHT];
	unsigned int numElem;
} adptsTable;

/// <summary>
/// Tabla para almacenar direciones DNS.
/// </summary>
typedef struct tDnsTable {
	char* dns[DNS_TABLE_LENGHT];
	unsigned int numElem;
} dnsTable;

/// <summary>
/// Esturctura de tipo TAD que contine la informaci�n del resultado de un test de velocidad a una direcci�n DNS.
/// dir: Direcci�n DNS.
/// avrgTime: Tiempo medio de respuesta.
/// jumps: N�mero total de saltos.
/// </summary>
typedef struct tDnsTest {
	char* dir;
	double avrgTime;
	unsigned int jumps;
} dnsTest;

/// <summary>
/// Tabla para almacenar direciones DNS y el resultado del test de velocidad asociado.
/// </summary>
typedef struct tDnsTestTable {
	dnsTest dirs[DNS_TABLE_LENGHT];
	unsigned int numElem;
} dnsTestTable;