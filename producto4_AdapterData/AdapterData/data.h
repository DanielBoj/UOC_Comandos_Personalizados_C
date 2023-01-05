#pragma once

/* Constantes */

/* Tamaños máximos de tablas y buffers */

/* Tamaño máximo de la tabla de saltos. */
#ifndef MAX_JUMPS
#define MAX_JUMPS 30
#endif // !MAX_JUMPS

/* Tamaño máximo del campo de nombre de adaptador e IP de servidor DNS. */
#ifndef MAX_ADPTR_INFO
#define MAX_ADPTR_INFO 100
#endif // !MAX_ADPTR_INFO

/* Tamaño máximo de una dirección IP. */
#ifndef MAX_IP
#define MAX_IP 16
#endif // !MAX_IP

/* Tamaño máximo de un string de comando. */
#ifndef MAX_COMMAND
#define MAX_COMMAND 250
#endif // !MAX_COMMAND

/* Tamaño máximo del buffer de lectura de una líunea de archivo externo o pipe. */
#ifndef MAX_LINE
#define MAX_LINE 250
#endif // !MAX_LINE

/* Tamaño máximo de la tabla de adaptadores. */
#ifndef MAX_ADPTRS_TABLE
#define MAX_ADPTRS_TABLE 30
#endif // !MAX_ADPTRS_TABLE


/* Rutas de archivos */

/* Ruta al archivo XML. */
#ifndef XML_DOC_PATH
#define XML_DOC_PATH "./adpater_info.xml"
#endif // !XML_DOC_PATH
 
/* Modos de apertura */

/* Lectura. */
#ifndef O_READ
#define O_READ "r"
#endif // !O_READ

/* Escritura y creación en caso de no existir. */
#ifndef O_WRITE
#define O_WRITE "w+"
#endif // !O_WRITE

/* Agregación y creación en caso de no existir. */
#ifndef O_APPEND
#define O_APPEND "a+"
#endif // !O_APPEND

/* Comandos */

/* Comando para la obtención de los adaptadores de red de un equipo. */
#ifndef ADPTS_COMMAND
#define ADPTS_COMMAND "netsh interface ipv4 show interfaces"
#endif // !ADPTS_COMMAND

/* Comando para la obtención de la información de un adaptador de red. */
#ifndef ADPTS_INFO_COMMAND
#define ADPTS_INFO_COMMAND "netsh interface ipv4 show config"
#endif // !ADPTS_INFO_COMMAND

/* Test de velocidad de un servidor DNS. */
#ifndef DNS_TEST_COMAND
#define DNS_TEST_COMAND "tracert -w 1000"
#endif // !DNS_TEST_COMAND

/* Contadores líneas nulas. */

/* Líneas sin información en un comando de obtención de la lista de adaptadores. */
#ifndef ADPTS_LOG_NULL_LINES
#define ADPTS_LOG_NULL_LINES 4
#endif // !ADPTS_LOG_NULL_LINES

/* Líneas sin información en un comando de obtención de la configuración de un adaptador. */
#ifndef ADPTS_DIRS_NULL_LINES
#define ADPTS_DIRS_NULL_LINES 3
#endif // !ADPTS_DIRS_NULL_LINES

/* Delimitadores y palabras clave. */

/* Delimitador para obtención de palabras en una línea. */
#ifndef ADPTR_DELIM
#define ADPTR_DELIM " "
#endif // !ADPTR_DELIM

/* Palabra clave para identificar un adaptador sin conexión. */
#ifndef KEY_WORD_ADPTR_OFF
#define KEY_WORD_ADPTR_OFF "disconnected"
#endif // !KEY_WORD_ADPTR_OFF

/* Carácter clave para identificar un retorno sin respuesta. */
#ifndef DNS_CONTROL_CHAR
#define DNS_CONTROL_CHAR "*"
#endif // !DNS_CONTROL_CHAR

/* Carácter clave secundario para identificar un retorno con o sin respuesta. */
#ifndef DNS_CONTROL_CHAR_2
#define DNS_CONTROL_CHAR_2 "ms"
#endif // !DNS_CONTROL_CHAR_2

/* Retorno por defecto en caso de un servidor sin respuesta. */
#ifndef SRV_NO_REACHEABLE
#define SRV_NO_RECHEABLE "Servidor bloqueado!\0"
#endif // !SRV_NO_REACHEABLE

/* Header, footer y etiquetas del archivo XML. */

/* Header */
#ifndef XML_DOC_HEADER
#define XML_DOC_HEADER "<?xml version='1.0' encoding='UTF-8'?>\n\n<adapter_info xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\nxsi:noNamespaceSchemaLocation=\"./adapter_info.xsd\">\n"
#endif // !XML_DOC_HEADER

/* Footer */
#ifndef XML_DOC_FOOTER
#define XML_DOC_FOOTER "</adapter_info>"
#endif // !XML_DOC_FOOTER

/* Etiquetas */

#ifndef T_NAME_OPEN
#define T_NAME_OPEN "<name>"
#endif // !T_NAME_OPEN

#ifndef T_NAME_CLOSE
#define T_NAME_CLOSE "</name>"
#endif // !T_NAME_CLOSE


#ifndef T_IP_OPEN
#define T_IP_OPEN "<ip>"
#endif // !T_IP_OPEN

#ifndef T_IP_CLOSE
#define T_IP_CLOSE "</ip>"
#endif // !T_IP_CLOSE

#ifndef T_MASK_OPEN
#define T_MASK_OPEN "<mask>"
#endif // !T_MASK_OPEN

#ifndef T_MASK_CLOSE
#define T_MASK_CLOSE "</mask>"
#endif // !T_MASK_CLOSE

#ifndef T_GATEWAY_OPEN
#define T_GATEWAY_OPEN "<gateway>"
#endif // !T_GATEWAY_OPEN

#ifndef T_GATEWAY_CLOSE
#define T_GATEWAY_CLOSE "</gateway>"
#endif // !T_GATEWAY_CLOSE

#ifndef T_DNS_OPEN
#define T_DNS_OPEN "<dns_server>"
#endif // !T_DNS_OPEN

#ifndef T_DNS_CLOSE
#define T_DNS_CLOSE "</dns_server>"
#endif // !T_DNS_CLOSE

#ifndef T_AVG_SPEED_OPEN
#define T_AVG_SPEED_OPEN "<average_dns_speed>"
#endif // !T_AVG_SPEED_OPEN

#ifndef T_AVG_SPEED_CLOSE
#define T_AVG_SPEED_CLOSE "</average_dns_speed>"
#endif // !T_AVG_SPEED_CLOSE

#ifndef T_JUMP_OPEN
#define T_JUMP_OPEN "<jump>"
#endif // !T_JUMP_OPEN

#ifndef T_JUMP_CLOSE
#define T_JUMP_CLOSE "</jump>"
#endif // !T_JUMP_CLOSE

#ifndef T_JUMP_NUM_OPEN
#define T_JUMP_NUM_OPEN "<jump_number>"
#endif // !T_JUMP_NUM_OPEN

#ifndef T_JUMP_NUM_CLOSE
#define T_JUMP_NUM_CLOSE "</jump_number>"
#endif // !T_JUMP_NUM_CLOSE

/// <summary>
/// Estructura TAD para contener la información de cada salto en la ejecución de un test tracert.
/// </summary>
typedef struct tJump {
	unsigned int jumpNum;
	char* ip;
} jump;

/// <summary>
/// Estructura TAD para almacenar los datos de configuración y del test tracert de un adaptador de red.
/// </summary>
typedef struct tAdapter {
	char* name;
	unsigned int index;
	char* ip;
	char* mask;
	char* gateway;
	char* dnsServer;
	double avgRespTime;
	jump jumps[MAX_JUMPS];
	unsigned int totalJumps;
} adapter;

/// <summary>
/// Tabla de estructuras TAD tipo adapter.
/// </summary>
typedef struct tAdaptersTable {
	adapter adptrs[MAX_ADPTRS_TABLE];
	unsigned int numElem;
} adaptersTable;