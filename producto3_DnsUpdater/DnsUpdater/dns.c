#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "api.h"
#include "data.h"
#include "dns.h"

/// <summary>
/// Genera un archivo log para almacenar la informaci�n sobre los adaptadores de red del equipo.
/// La informaci�n se obtine lanzando desde el c�digo un comando NETSH a trav�s de la funci�n system().
/// El retorno se redirige a un archivo .txt que se guarda en la carpeta ra�z.
/// </summary>
void createAdptsLog() {

	/* Creamos el string para el comando y lo inicializamos. */
	char* command;
	command = (char*)malloc(MAX_COMMAND);

	/* Comprobamos que se haya reservado el espacio de forma correcta. */
	if (command != NULL) {
		
		/* Concatenamos las diferentes partes del comando. */
		sprintf(command, "%s %s", ADPTS_COMMAND, ADPTS_LOG_PATH);

		/* Lanzamos el comando. */
		system(command);
	}

	/* Liberamos la memoria usada. */
	if (command) {
		free(command);
	}
}
/// <summary>
/// Genera un archivo de log con las direcciones DNS de un adaptador del equipo que seleccionamos por el n�mero de �ndice
/// que se recibe como par�metro de entrada. Se realiza la selecci�n por �ndice para evitar problemas surgidos por la
/// codificaci�n de caracteres especiales. El log se gener� mediante la ejecuci�n de un comando de CMD a trav�s de la 
/// funci�n system(). EL instrucci�n ejecuta un comando NETSH que obtiene la informaci�n de los servidores DNS del adaptador.
/// El retorno se dirije a un archivo .txt que se guarda en la carpeta ra�z.
/// command: String donde generaremos el comando que ejecutar.
/// </summary>
/// <param name="index">int �ndice del adaptador de red.</param>
/// <param name="retVal">int Devuelve 0 si la operaci�n se ejecuta correctamente, o 1 de lo contrario.</param>
void  createDnsLog(const int index, int* retVal) {

	*retVal = 0;

	/* Creamos el string para asignar el comando y lo inicializamos. */
	char* command;
	command = (char*)malloc(MAX_COMMAND);

	if (command != NULL) {

		/* Concatenamos las diversas partes del string. */
		sprintf(command, "%s %i %s %s", DNS_DIRS_COMMAND, index, " > ", DNS_LOG_PATH);

		/* Lanzamos el comando. */
		system(command);
	}
	else {

		puts("Error: Memoria insuficiente.");
		*retVal = 1;
	}

	if (command) {
		free(command);
	}
}

/// <summary>
/// Carga la informaci�n de un archivo log con los datos de los adaptadores a una tabla de TAD de tipo
/// "adapters" con la informaci�n de estos.
/// La funci�n gestiona los posibles errores mediante un valor de retorno. Implementa los siguientes pasos:
/// Aperura del documento, lectura y descarte de las primeras l�neas sin informaci�n requerida, lectura l�nea a
/// l�nea de los datos de los adaptadores hasta llegar al fin del codumento o alcanzar el m�ximo de datos posibles
/// en la tabla, paso a un string auxiliar y creaci�n del TAD tipo "adapter" para la carga de los datos. 
/// A continuaci�n, carga el adaptador en la tabla, cierra el documento y libera la memoria reservada.
/// FILE *fAdapterInfo = Puntero al archivo log con la informaci�n de los adaptadores.
/// char *line = String auxiliar para la carga de una l�nea del archivo.
/// adapter newAdapter = TAD de tipo "adapter" auxiliar para la carga de los datos de un adaptador.
/// </summary>
/// <param name="table">adptsTable Tabla de TAD tipo "adpater".</param>
/// <param name="retVal">int Devuelve 0 si la operaic�n se realiza con �xito, o 1 si falla.</param>
void adptsLoad(adptsTable* table, int* retVal) {

	*retVal = 0;

	FILE* fAdptInfo = NULL;
	char* line;
	adapter newAdptr;

	/* Inicializamos las variables y la salida de datos */
	line = (char*)malloc(MAX_LINE);
	initAdptr(&newAdptr);
	intAdptrTable(table);

	/* Abrimos el archivo */
	if ((fAdptInfo = fopen(ADPTS_LOG_PATH, O_READ)) != NULL) {
		/* Creamos un contador para controlar las l�neas que no queremos capturar. */
		int ind = 0;

		/* Leemos las l�neas que contienen la informaci�n de los adaptadores */
		while ((!feof(fAdptInfo)) && (table->numElem < ADPTRS_TABLE_LENGHT)) {

			/* Pasamos las l�neas que no contienen datos. */
			while (ind < ADPTS_LOG_NULL_LINES) {
				fgets(line, MAX_LINE - 1, fAdptInfo);

				/* Actualizamos le contador. */
				ind++;
			}

			/* Purgamos la l�nea de lectura */
			line[0] = '\0';

			/* Leemos una l�nea del archivo */
			if (fgets(line, MAX_LINE - 1, fAdptInfo)) {
				/* Nos aseguramos de que la l�nea acaba con el car�cter de fin de l�nea */
				line[MAX_LINE - 1] = '\0';

				/* Obtenemos la informaci�n */
				if (strnlen(line, MAX_LINE - 1) > 1) {
					getAdptr(line, &newAdptr);

					/* A�adimos el nuevo adaptador a la tabla */
					addAdapter(table, newAdptr, retVal);
				}
			}
		}
	}
	else {
		puts("Error: No se puede crear o acceder al archivo.");
		*retVal = 1;
	}

	/* Cerramos el archivo. */
	if (fAdptInfo) {
		fclose(fAdptInfo);
	}

	if (newAdptr.name) {
		free(newAdptr.name);
	}

	if (line) {
		free(line);
	}
}

/// <summary>
/// Carga la informaci�n del archivo con las direcciones DNS en una tabla tipo dnsTable.
/// La funci�n gestiona los posibles errores mediante un valor de retorno. Implementa los siguientes pasos:
/// Apertura del documento, lectura l�nea a l�nea de los datos de los adaptadores hasta llegar al fin del codumento 
/// o alcanzar el m�ximo de datos posibles en la tabla, paso a un string auxiliar y creaci�n del TAD tipo "adapter" 
/// para la carga de los datos. A continuaci�n, carga el adaptador en la tabla, cierra el documento y libera la memoria 
/// reservada.
/// FILE *fDns = Puntero al archivo con las direcciones DNS.
/// char *line = String auxiliar para la carga de una l�nea del archivo. 
/// 
/// </summary>
/// <param name="table">dnsTable Tabla que contine Strings con las direcciones DNS.</param>
/// <param name="retVal">int Devuelve 0 si la operaic�n se realiza con �xito, o 1 si falla.</param>
void dnsLoad(dnsTable* table, const char* dnsPath, int* retVal) {
	
	*retVal = 0;

	FILE* fDns = NULL;
	char* line;

	/* Inicializamos las variables y la salida de datos */
	line = (char*)malloc(MAX_LINE);
	initDnsTable(table);

	/* Abrimos el archivo */
	if (fDns = fopen(dnsPath, O_READ)) {
	
		while (!feof(fDns) && (table->numElem < DNS_TABLE_LENGHT)) {

			/* Purgamos la l�nea de lectura */
			line[0] = '\0';

			/* Leemos una l�nea del archivo */
			if (fgets(line, MAX_ADTR_LINE - 1, fDns)) {

				/* Nos aseguramos de que la l�nea acaba con el car�cter de fin de l�nea */
				line[strnlen(line, MAX_LINE) - 1] = '\0';
				
				if (strnlen(line, MAX_ADTR_LINE - 1) > 1) {
					addDns(table, line, retVal);
				}
			}
		}
	}
	else {
		puts("Error: Ruta de acceso invalida o resulta imposible acceder al archivo.");
		*retVal = 1;
	}

	/* Cerramos el archivo. */
	if (fDns) {
		fclose(fDns);
	}

	if (line) {
		free(line);
	}
}

/// <summary>
/// Creamos un elemento de tipo TAD adpater a trav�s de una l�nea de informaci�n que hemos obtenido del documento
/// log con la informaci�n de los adaptadores de red del equipo.
/// auxState: Guarda el resultado del estado del adpatador para asignarlo m�s tarde a la estructura com un valor
/// de tipo booleano.
/// Usamos la funci�n sscanf() que permite obtener una serie de valores aislados por el car�cter ' ' de un string
/// y los vamos asignando a los campor de la estructura TAD adaptador. Cada valor es un campo de informaci�n del
/// adaptador que podremos usar m�s tarde.
/// </summary>
/// <param name="str">char* String desde el que capturaremos la informaci�n del adaptador. in</param>
/// <param name="adptr"> tAdaptador TAD tipo adpatador para volvar la informaci�n del adaptador. in/out</param>
void getAdptr(const char* str, adapter* adptr) {
	char* auxState;
	auxState = (char*)malloc(MAX_ADTR_LINE);

	/* Comprobamos que el par�metro adapter no sea NULL. */
	if (adptr) {

		/* Leemos los datos del string de entrada y los pasamos a los campos del adaptador. */
		sscanf(str, "%u %u %u %s %s",
			&adptr->id,
			&adptr->metric,
			&adptr->mtu,
			auxState,
			adptr->name);

		/* Si el daptador est� conectado, igualamos el estado a true. */
		if (strncmp(auxState, "connected", MAX_ADTR_LINE) == 0) {
			adptr->state = true;
		}
		else {

			/* Si el daptador est�a desconectado, lo igualamos a false. */
			adptr->state = false;
		}
	}
	else {
		return;
	}
}

/// <summary>
/// A�ade un TAD de tipo "adpater" a una tabla de adaptadores tipo adptsTable.
/// insertInd: �ndice d eposici�n de inserci�n en la tabla.
/// </summary>
/// <param name="table">adptsTable Tabla de adaptadores.</param>
/// <param name="adptr">adpter Adaptador que queremos a�adir a la tabla.</param>
/// <param name="retVal">int Vale 0 si la operaci�n se realiza con �xito, 1 si hay alg�n fallo.</param>
void addAdapter(adptsTable* table, const adapter adptr, int* retVal) {
	
	*retVal = 0;

	/* Comprobamos si hay espacio disponible */
	if (!isFullAdpts(*table)) {
		int insertInd;

		/* Asignamos la posici�n y actualizamos el contador */
		insertInd = table->numElem++;
		
		/* A�adimos el adaptador a la tabla. */
		adptrCopy(&table->adptr[insertInd], adptr);
	}
	else {
		puts("Error: La tabla est� llena.");
		*retVal = 1;
	}
}

/// <summary>
/// Toma una tabla de TAD tipo adapter que contiene la informaci�n de los adaptadores de red y un entero que representa la selecci�n que ha
/// realizado un usuario sobre una lista de adaptadores de red activos y devuelve la posici�n en la tabla del adaptador escogido.
/// La tabla contiene tanto adaptadores activos como inactivos, el usuario ha escogido el n�mero de adaptador en una lista que contiene
/// solo adaptadores activos y que empieza por 1, no por 0. La funci�n busca los adaptadores activos, les adjudica una posici�n relativa
/// en un contador de adaptadores activos y lo compara con la selecci�n del usuario a la que restamos 1, para ajustarlo a la numeraci�n de
/// posiciones de un array en C.
/// </summary>
/// <param name="table">adptsTable Tabla de TAD tipo adapter que contiene la informaci�n de los adaptadores de red del equipo.</param>
/// <param name="adptrNum">int Posici�n en la lista de adaptadores activos que ha escogido el usuario.</param>
/// <returns>int Posicion del adaptador seleccionado en la tabla de TAD tipo adapter.</returns>
int searchAdptrInd(const adptsTable table, const int adptrNum) {

	int index, actualInd;
	unsigned int arrayInd;
	bool isFound;

	index = -1;
	actualInd = 0;
	arrayInd = 0;
	isFound = false;

	/* Comprobamos que la tabla tenga elementos. */
	if (table.numElem > 0) {

		/* Recorremos la tabla hasta terminarla o encontrar el adaptador buscado. */
		while (!isFound && (arrayInd < table.numElem)) {

			/* Comprobamos que el adaptador est� activo. */
			if (table.adptr[arrayInd].state) {

				/* Comprobamos si la posici�n relativa del adaptador coincide con la opci�n del usuario. */
				if (actualInd == (adptrNum - 1)) {

					/* Igualamos el retorno a la posici�n del adaptador en la tabla. */
					index = arrayInd;
				}

				/* Si el adaptador est� activo, actualizamos el contador de adaptadores activos. */
				actualInd++;
			}

			/* Actualizamos el �ndice de posiciones. */
			arrayInd++;
		}
	}

	return index;
}

/// <summary>
/// A�ade una direcci�n DNS a una tabla de tipo dnsTable.
/// insertInd: �ndice d eposici�n de inserci�n en la tabla.
/// </summary>
/// <param name="table">dnsTable Tabla de direcciones DNS.</param>
/// <param name="str">char* String con la direcci�n DNS.</param>
/// <param name="retVal">int Vale 0 si la operaci�n se realiza con �xito, 1 si hay alg�n fallo.</param>
void addDns(dnsTable* table, const char* str, int* retVal) {
	
	*retVal = 0;

	/* Comprobamos si hay espacio disponible. */
	if (!isFullDns(*table)) {
		int insertInd;
		
		/* Asignamos la posici�n y actualizamos el contador */
		insertInd = table->numElem++;

		/* A�adimos las direcci�n DNS a la tabla */
		strncpy(table->dns[insertInd], str, MAX_DNS_LINE);
	}
	else {
		puts("Error: La tabla est� llena.");
		*retVal = 1;
	}
}

/// <summary>
/// A�ade una direcci�n DNS a una tabla de tipo dnsTestTable que contendr� las direcciones DNS y los resultados de los tests de velocidad
/// a estas.
/// </summary>
/// <param name="table">dnsTestTable Tabla que contiene las direcciones DNS y los resultados de los tests de velocidad.</param>
/// <param name="str">char* String con la direcci�n DNS.</param>
/// <param name="retVal">int Devuelve 0 si la operaci�n se realiza con �sxito, o 1 de lo contrario.</param>
void addDnsToTestTable(dnsTestTable* table, const char* str, int* retVal) {

	*retVal = 0;

	/* Comrprobamos si hay espacio disponible. */
	if (!isFullDnsTest(*table)) {
		int insertInd;

		/* Asignamos la posici�n y actualizamos el contador */
		insertInd = table->numElem++;

		/* A�adimos la direcci�n DNS a la tabla. */
		table->dirs[insertInd].dir = (char*)malloc(MAX_DNS_LINE);
		strncpy(table->dirs[insertInd].dir, str, MAX_DNS_LINE);
	}
	else {
		puts("Error: La tabla est� llena.");
		*retVal = 1;
	}
}

/// <summary>
/// Obtiene las direcciones DNS asociadas a un adaptador de red que ha escogido un usuario. Crea los elementos temporales necesarios
/// para mostrar la informaci�n por pantalla. La funci�n delega en una serie de funciones auxialiares que se han creado en el programa:
/// initAdptr() e initDnsTable(): Inicializan, respectivamente, un TAD tipo adapter y una tabla de direcciones DNS.
/// searchAdapterInd(): Busca el �ndice de posici�n real del adaptador de red escogio por el usuario en la tabla a trav�s de la opci�n
/// relativa que ha escogio el usuario.
/// adptrCopy(): Copia los datos de un TAD tipo adapter a otro TAD del mismo tipo.
/// createDnsLog(): Crea un archivo log con las direcciones DNS de un adaptador de red.
/// readDnsLog(): Lee un archivo de log que contiene las direcciones DNS de un adaptador de red, obtiene las direcciones y las carga
/// en una tabla de direcciones DNS.
/// printDnsTable(): Muestra las direcciones por pantalla al usuario.
/// freeAdapter(): Libera los recursos usados.
/// -------
/// actualAdptr: Estructura TAD tipo "adapter" que contiene la informaci�n del adaptador de red.
/// dnsRes: Tabla de direcciones DNS temporal que contendr� las direcciones DNS del adaptador de red seleccionado por el usuario.
/// adptrInd: Variable a la que asignaremos el �ndice del adaptador escogido por el usuario para lanzar el comando NETSH. Usamos un 
/// �ndice parar evitar problemas con el uso de caracteres especiales en los nombres de los adaptadores de red del equipo.
/// posInd: �ndice de posici�n del adaptador en la tabla.
/// </summary>
/// <param name="table">adptsTable Tabla de TAD tipo adapter con la informaci�n de los adaptadores de res del equipo.</param>
/// <param name="adptrNum">Identificador relativo del adaptador que el usuario a escogido en una lista de adaptadores activos.</param>
/// <param name="adptrInd">El �ndice de posici�n del adaptador en la tabla de TAD tipo adapter.</param>
/// <param name="retVal">Retorno 0 se la operaci�n se ejecuta con �cito, 1 si falla.</param>
void getDnsIps(const adptsTable table, const int adptrNum, dnsTable* adptrDnsSrvs, int* posInd, int* retVal) {

	*retVal = 0;
	
	adapter actualAdptr;
	int adptrInd;

	/* Inicializamos las estructuras */
	initAdptr(&actualAdptr);
	initDnsTable(adptrDnsSrvs);

	/* Obtenemos el �ndice de posici�n en la tabla del adaptador escogido por el usuario. */
	*posInd = searchAdptrInd(table, adptrNum);
	
	/* Obtenemos los datos del adaptador escogido por el usuario y asignamos el �ndice del adaptador. */
	adptrCopy(&actualAdptr, table.adptr[*posInd]);
	adptrInd = actualAdptr.id;
	
	/* Lanzamos el comando a trav�s del �ndice del adaptador escogido. */
	createDnsLog(adptrInd, retVal);

	/* Leemos la informaci�n del log generado. */
	readDnsLog(adptrDnsSrvs, retVal);

	/* Salida de informaci�n. */
	puts("----------------------------------------------");
	printf("Direcciones DNS para el adaptador %s:\n", actualAdptr.name);
	printDnsTable(*adptrDnsSrvs);

	/* Liberamos los recursos temporales usados. */
	freeAdapter(&actualAdptr);
}

/// <summary>
/// Obtine los datos de las direcciones DNS que hemos registrado en un archivo log a trav�s de la ejecuci�n de un comando NETSH.
/// La funci�n gestiona la apertura del archivo y el control de errores. Descarta las primeras l�neas sin informaci�n �til y
/// obtiene las direcciones DNS de las siguientes l�neas hasta la lectura de la palabra de control que indica el fin del registro
/// de las direcciones DNS en el log. En el caso de la primera direcci�n, descarta tambi�n todos los datos hasta llegar al n�mero
/// de direcci�n DNS.
/// Para implementar las operaciones de b�squeda y descarte de informaci�n usamos funciones de la librer�a string.h():
/// strstr() busca un substring dentro de un string principal, y si lo encuentra, devuelve un puntero a char a partir del string localizado. En este caso, no nos interesa el valor
/// devuelto, solo comprobamos que no sea NULL para saber que hemos llegado a la l�nea a descartar y finalizar el proceso de obtenci�n de datos.
/// strcspn() devuelve la longitud en n�mero de car�cteres hasta hallar la primera incidencia de un caracter que pasamos como argumento. Usamos esta longitud para saber en qu� posici�n
/// de line debemos empezar a copiar car�cteres. Para obtener el substring que nos interesa, realizamos una copia caracter a caracter de line a partir del punto que nos interesa e 
/// ignorando los caracteres de espacio y de tabulaci�n ' ' y '\t'.
/// strtok() divide un String en substrings usando como delimitador los caracteres que indiquemos en un segundo string que recive como par�metro. Usamos esta funcion para ignorar los
/// espacios en blanco y tabulaciones de la l�nea y copiar solo la direcci�n DNS.
/// strnlen() devuelve como entero la longitud de un string.
/// fDnsLog: Puntero a FILE al archivo log.
/// line: String para registrar temporalmente los datos de cada l�nea.
/// actualDns: String para registrar el valor final de la direcci�n DNS que agregaremos a la tabla.
/// lineNum: L�nea actual de lectura del archivo.
/// lenght: Longitud del substring con los datos que queremos descartar de la l�nea de la primera direcci�n DNS.
/// indPnt: �ndice de posici�n en el string de destino para la copia de la primera direcci�n DNS.
/// isEnd: Booleano de control para identificar si hemos llegado a los datos triviales.
/// limitChar: String con el valor a buscar paara descartar la informaci�n no �til en la l�nea de la primera direcci�n DNS.
/// whiteSpace y tabulation: Car�cteres que queremosignorar en la captura de las direcciones DNS del resto de l�neas.
/// </summary>
/// <param name="table">dnsTable Tabla para almacenar las direcciones DNS en formato String.</param>
/// <param name="retVal">int Devuelve 0 si la operaci�n se realiza con �xito, 1 si hay alg�n fallo.</param>
void readDnsLog(dnsTable* table, int* retVal) {

	*retVal = 0;

	FILE* fDnsLog = NULL;
	char* line, * actualDns;
	int lineNum = 0, lenght = 0, indPnt = 0;
	bool isEnd = false;

	/* Caracteres delimitadores para la b�squeda en la l�nea. */
	char* limitChar = ":";
	char whiteSpace = ' ', tabulation = '\t';

	/* Incializamos la tabla de DNS y line. */
	line = (char*)malloc(MAX_LINE);
	actualDns = (char*)malloc(MAX_LINE);
	initDnsTable(table);

	/* Abrimos el archivo */
	if ((fDnsLog = fopen(DNS_LOG_PATH, O_READ)) != NULL) {
		
		while (!feof(fDnsLog) && (table->numElem < DNS_TABLE_LENGHT && !isEnd)) {

			/* Descartamos la l�neas que no contienen la informaci�n que nos interesa. */
			while (lineNum < DNS_DIRS_LOG_NULL_LINES) {
				fgets(line, MAX_LINE - 1, fDnsLog);
				
				lineNum++;
			}
			
			/* Actualizamos el contador de l�neas. */
			lineNum++;

			/* Purgamos la l�nea */
			line[0] = '\0';
			actualDns[0] = '\0';

			/* Registramos la posici�n del cursor para usarlo en el caso de tener que ignorar parte de la l�nea en el caso
			* de la obtenci�n de la primera direcci�n DNS. */			
			if (fgets(line, MAX_LINE - 1, fDnsLog)) {

				/* Comprobamos que no estemos sobrepasando las l�neas de direcci�n. */
				if (strstr(line,DNS_LOG_END_KEY)) {
					
					isEnd = true;
					continue;
				}

				/* Si estamos en la primera l�nea de direcciones, movemos el cursor hasta la direcci�n. */
				if (lineNum == 3) {

					/* Buscamos el caracter de control ':' y obtenemos el resto del string ignorando los caracteres que no pertenecen a la direcci�n. */
					lenght = strcspn(line, DNS_LOG_CONTROL_KEY);
					
					for (int i = lenght + 1; i < ((int)strnlen(line, MAX_LINE - 1)); i++) {

						if (line[i] != (char)limitChar && line[i] != tabulation && line[i] != whiteSpace) {
							actualDns[indPnt] = line[i];
							
							indPnt++;
						}
					}

					/* Finalizamos la l�nea y purgamos el salto de l�nea final. */
					actualDns[indPnt - 1] = '\0';

					/* A�adimos la direcci�n a la tabla. */
					addDns(table, actualDns, retVal);
				}
				else {
					/* Si estamos en direcciones consecuentes, purgamos los caracteres no �tiles de la l�nea */
					char* delimiters = (char*)malloc(MAX_LINE);

					if (delimiters != NULL) {
						sprintf(delimiters, "%c%c", tabulation, whiteSpace);
						actualDns = strtok(line, delimiters);

						/* Purgamos el salto de l�nea final */
						actualDns[strnlen(actualDns, MAX_LINE) - 1] = '\0';

						/* A�adimos la direcci�n a la tabla. */
						addDns(table, actualDns, retVal);
					}

					if (delimiters) {
						free(delimiters);
					}
				}
			}
		}
	}
	else {
		*retVal = 1;
		puts("Error: No puede leerse el archivo log o no existe.");
	}

	/* Cerramos el archivo. */
	if (fDnsLog) {
		fclose(fDnsLog);
	}
	

	if (line) {
		free(line);
	}

	if (actualDns) {
		actualDns = NULL;
		free(actualDns);
	}
}

/// <summary>
/// Funciona como un driver para realizar todas las funciones de test y asignaci�n de DNS.
/// Realiza los tests de accesibilidad sobre los DNS del adaptador escogido por el usuario y, si son accesibles, los a�ade a la lista de
/// servidores DNS obtenida del archivo en la carga de datos.
/// Realiza los tests de velocidad sobre todos los adaptadores, carga la informaci�n en un tabla y ordena los adaptadores de la tabla seg�n
/// los resultados (primero, por tiempo medio de respuesta, y si este es el mismo, por n�mero de saltos).
/// Una vez ordenados, comprueba si ya est� asignado el mejor servidor DNS, si no es as�, asigna al adaptador escogido por el usuario
/// en el primer paso del programa el servidor m�s r�pido.
/// fDnsTest: Puntero a FILE para crear el archivo temporal donde volcaremos el test de velocidad.
/// testTable: Tabla de tipo dnsTestTable formada por elementos TAD del tipo dnsTest que contienen la informaci�n de los test a los
/// servidores DNS.
/// </summary>
/// <param name="adptsTbl">adptsTable Tabla de elementos TAD tipo adapter con los datos de los adaptadores de red del sistema.</param>
/// <param name="dnsDirs">dnsTable Tabla de DNS con los servidores DNS le�dos del archivo en la carga de datos.</param>
/// <param name="adprDnsSrvs">dnsTable Tabla DNS con los servidores DNS del adaptador escogido por el usuario.</param>
/// <param name="adptrInd">int �ndice del adaptador escogido por el usuario.</param>
/// <param name="retVal">int Devuelve 0 si la operaci�n se ejecuta con �xito, o 1 si falla.</param>
void testDnsServers(const adptsTable adptsTbl, const dnsTable dnsDirs, const dnsTable adprDnsSrvs, const int adptrInd, int* retVal) {

	*retVal = 0;

	FILE* fDnsTest;
	dnsTestTable testTable;

	/* Creamos un archivo temporal de tipo binario. */
	if (fDnsTest = tmpfile()) {
		findDnsAccessibility(adprDnsSrvs, fDnsTest, retVal);
	}
	else {

		puts("Error: No puede crearse el archivo temporal.");
		*retVal = 1;
	}

	/* Volvemos a la primera posici�n del archivo temporal. */
	if (fDnsTest) {
		rewind(fDnsTest);
	}

	/* Realizamos un merge entre las DNS del adaptador y las DNS de la lista de direcciones. */
	initDnsTestTable(&testTable);
	mergeToDnsTestTable(dnsDirs, fDnsTest, &testTable, retVal);

	/* Realizamos el test de velocidad. */
	speedTest(&testTable, retVal);
	//printf("Mejor Servidor: %s\n", testTable.dirs[0].dir);
	/* Ordenamos la tabla para encontrar el mejor servidor. */
	qSortDnsTestTable(&testTable, 0, testTable.numElem - 1);

	//printf("Mejor Servidor: %s\n", testTable.dirs[0].dir);

	/* Asignamos el servidor DNS m�s r�pido. */
	changeDns(adptsTbl, testTable, adprDnsSrvs, adptrInd, retVal);

	/* Cerramos el archivo temporal. */
	if (fDnsTest) {
		fclose(fDnsTest);
	}

	clearDnsTestTable(&testTable);	
}

/// <summary>
/// Comprueba la accesibilidad a un grupo de servidores DNS que obtiene por par�metro y guardo los resultados en un archivo temporal
/// que tambi�n obtiene como par�metro.
/// La funci�n gestiona una llamada a un comando CMD tracert mediante el uso de un pipe, para ello sua la funci�n _popen() que 
/// permite leer el retorno del comando directamente desde el programa. Usamos las posiciones de las DNS guardadas en la tabla para 
/// ir realizando una itereaci�n que ejecute tracert sobre cada una de las direcciones.
/// Para realizar la llamada, primero concatena las diversas partes de la sentencia del comando mediante la funci�n sprintf() una vez
/// obtenida, lanzamos el comando mediante un pipe con _popen() y comenzamos la lectura del retorno. Continuamos iniciando el buckle
/// de lecturas l�nea a l�nea y gestionando lso diversos escenarios que nos interesan:
/// 1. Mediante el uso de un contador de l�neas, ignoramos las diferentes l�neas que contienen informaci�n trivial.
/// 2. Proseguimos realizando la lectura de los resultados de la traza de rutas. Identificamos mediante una palabra clave si la l�nea
/// le�da informa de que el adaptador es no accesible. Si es as�, lo mostramos al usuario e indicamos, mediante un booleano de control
/// que ya hemos obtenido la respuesta, de esta forma, indicamos al programa que ya no debe buscar m�s informaci�n y que �nicamente acabe
/// de ejecutar el comando para marcar el EOF.
/// 3. Si obtenemos respuesta del servidor y empezamos la traza de la ruta, dejamos que realice un segundo salto, identificamos que se ha
/// producido mediante un control de car�cter clave y seteamos el booleano indicando que hemos hallado la respuesta.
/// 4. Una finalizado el comando, cerramos el pipe mediante _pclose() y continueamos con el bucle sobre la tabla de DNS.
/// Limitamos el comando tracert ha 2 saltos porque es suficiente para obtener la informaci�n deseada y as� aligeramos el proceso. La
/// funci�n tambien gestiona el control de errores, la asignaci�n y la liberaci�n de memoria.
/// ptr: Puntero al pipe de lectura del comando.
/// command: String para crear la sentencia del comando.
/// line: String para asignar la l�nea le�da del pipe.
/// lineNum: Contador del n�mero de l�neas le�das.
/// isSet: Booleano de control de obtenci�n de los datos.
/// </summary>
/// <param name="adptrDnsSrvs">dnsTable Tabla con las direcciones DNS de un adaptador de red..</param>
/// <param name="fTempFile">Archivo temporal.</param>
/// <param name="retVal">Vale 0 si la operaci�n se ejecuta sin errores, si no, 1.</param>
void findDnsAccessibility(const dnsTable adptrDnsSrvs, FILE* fTempFile, int* retVal) {

	*retVal = 0;

	FILE* ptr;
	char* command, * line;
	char controlChar;
	int lineNum;
	bool isSet;

	/* Inicializamos los Strings. */
	command = (char*)malloc(MAX_LINE);
	line = (char*)malloc(MAX_LINE);

	controlChar = '2';


	if (fTempFile) {

		/* Informaci�n de salida para el usuario. */
		puts("-------------------------------------------------");
		puts("Realizando el test de accesibilidad a los servidores del adaptador:\n"
			"(Las acciones pueden demorarse un poco)");
		puts("-------------------------------------------------");

		for (unsigned int i = 0; i < adptrDnsSrvs.numElem; i++) {

			/* Salida de datos informativos. */
			printf("\n#%d. Test sobre el servidor %s\n", (i + 1), adptrDnsSrvs.dns[i]);

			/* Reseteamos el control de l�nea con la lectura de cada test a una DNS y el control de b�squeda. */
			lineNum = 0;
			isSet = false;

			/* Generamos el comando para la DNS actual. */
			sprintf(command, "%s %s %s", DNS_TRACER_COMMAND, TRACE_2_MAX_JUMPS, adptrDnsSrvs.dns[i]);

			/* Leemos directamente el resultado del test y comprobamos los resultados. */
			if (ptr = _popen(command, O_READ)) {

				/* Ejecutamos la lectura mientras el comando devuelva una l�nea v�lida. */
				while ((!feof(ptr) && fgets(line, MAX_LINE - 1, ptr) != NULL)) {///

					/* Comprobamos si ya hemos establecido la accesibilidad. */
					if (isSet) {

						/* Finalizamos la lectura hasta EOF. */
						continue;
					}

					/* Ignoramos las primeras l�neas sin informaci�n importante. */
					if (lineNum < DNS_TEST_NULL_LINES) {

						/* Ignoramos la l�nea. */
						line[0] = '\0';

						/* Actualizamos el contador de l�neas. */
						lineNum++;

						continue;
					}

					/* Comprobamos si el destino es inaccesible. */
					if (strstr(line, DNS_TEST_NOT_RECHEABLE_KEY_WORD)) {

						/* Salida de datos informativos. */
						puts("No Accesible");

						/* Purgamos line. */
						line[0] = '\0';

						/* Marcamos como establecido. */
						isSet = true;

						continue;
					}

					if (line[2] == controlChar) {

						/* Escribimos la DIR en el archivo temp. */
						puts("Accesible");
						fprintf(fTempFile, "%s\n", adptrDnsSrvs.dns[i]);

						/* Purgamos line. */
						line[0] = '\0';


						/* Marcamos como establecido. */
						isSet = true;

						continue;
					}
				}

				/* Cerramos el pipe. */
				if (ptr) {
					fclose(ptr);
				}
			}
			else {
				puts("Error: No puede ejecutarse el comando.");
				*retVal = 1;
			}
		}
	}
	else {

		puts("Error: No puede crearse el archivo temporal.");
		*retVal = 1;
	}
	 
	/* Liberamos la memoria. */
	if (line) {
		free(line);
	}

	if (command) {
		free(command);
	}
}

/// <summary>
/// Combina en una tabla de tipo dnsTestTable las direcciones DNS de una tabla de tipo dnsTable y de un archivo temporal que hemos generado
/// con DNS v�lidas que hemso testado.
/// </summary>
/// <param name="src1">dnsTable Primera tabla de origen.</param>
/// <param name="src2">dnsTable Segunda tabla de origen.</param>
/// <param name="retVal">Retorn 0 si la operaci�n se ha realizado con �xito o 1 si ha fallado.</param>
void mergeToDnsTestTable(const dnsTable src, FILE* fTempFile, dnsTestTable* testTable, int* retVal) {

	*retVal = 0;

	char* line;
	
	/* Inicializamos los elementos. */
	line = (char*)malloc(MAX_DNS_LINE);

	/* A�adimos las direcciones le�das del documento de servidores DNS. */
	for (unsigned int i = 0; i < src.numElem; i++) {
		addDnsToTestTable(testTable, src.dns[i], retVal);
	}

	/* A�adimos las direcciones del documento temporal */
	if (fTempFile != NULL) {
		
		/* Leemos datos hasta llegar a EOF */
		while (!feof(fTempFile)) {

			/* Purgamos line.*/
			line[0] = '\0';

			/* Comprobamos que obtengamos un resultado de la lectura de la l�nea. */
			if (fgets(line, MAX_DNS_LINE - 1, fTempFile) != NULL) {
				strtok(line, "\n");
				addDnsToTestTable(testTable, line, retVal);
			}
		}
	}
	else {
		puts("Error: No puede abrirse el archivo.");
		*retVal = 1;
	}

	if (line) {
		free(line);
	}
}

/// <summary>
/// Realiza un test de velocidad sobre las direcciones DNS de una tabla tipo dnsTest pasada por par�metro que contiene elementos TAD de tipo dnsTest.
/// De cada elemento se leer� la direcci�n DNS donde ejecutar el test, y se grabar�n el n�mero total de saltos y el tiempo medio de respuesta que
/// calculamos dividiendo el tiempo total de respuesta por el n�mero de salto. Para realizar la lectura de la ejecuci�n del comando, se usa un pipe
/// que abre como un puntero a FILE la respuesta de la ejecuci�n de un tracert por CMD mediante la funci�n _popen().
/// Para realizar la prueba se usa el comando CMD tracert del cu�l obtenemos tanto el n�mero de saltos, como el tiempo de respuesta medio para cada salto.
/// En el caso de devolver un retorno tipo Tiempo d eespera agotado, como el comando se lanza con una opci�n de tiempo de espera m�ximo de 1000 ms, se
/// asignar� ese valor a el tiempo de respuesta de ese salto.
/// La funci�n gestiona los posibles errores de lectura del retorno del comando o de lectura de la tabla de direcciones.
/// </summary>
/// <param name="testTable">dnsTestTable Tabla con las direcciones DNS a leer y sobre la que se escribir�n el resultado de las pruebas.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se ejecutan sin errores, o 1 de lo contrario.</param>
void speedTest(dnsTestTable* testTable, int* retVal) {

	*retVal = 0;

	FILE* ptr;
	char* dnsDir, * command, * line, * dumbStr, * processCount;
	unsigned int totalJumps, actualLine, actualWord;
	double totalTime, actualTime, avrgTime;

	/* Asignamos la memoria. */
	dnsDir = (char*)malloc(MAX_DNS_LINE);
	command = (char*)malloc(MAX_COMMAND);
	line = (char*)malloc(MAX_LINE);
	dumbStr = (char*)malloc(MAX_WORD);
	processCount = (char*)malloc(31);

	if (testTable->numElem > 0) {
		
		/* Informaci�n de salida para el usuario. */
		puts("-------------------------------------------------");
		puts("Realizando el test de velocidad a los servidores DNS:\n"
			"(Las acciones pueden demorarse un poco)");
		puts("-------------------------------------------------");

		for (unsigned int i = 0; i < testTable->numElem; i++) {

			/* Salida de datos informativos. */
			printf("\n#%d. Test sobre el servidor %s\n", (i + 1), testTable->dirs[i].dir);
			 
			/* Obtenemos la direcci�n DNS. */
			strncpy(dnsDir, testTable->dirs[i].dir, MAX_DNS_LINE);

			/* Creamos el comando CMD. */
			if (command) {
				sprintf(command, "%s %s", TRACE_MAX_1000MSEC, dnsDir);
			}

			/* Inicializamos las variables de lectura de datos. */
			totalJumps = 0;
			actualTime = 0.0;
			totalTime = 0.0;
			avrgTime = 0.0;
			line[0] = '\0';
			
			/* Inicializamos el contador de l�neas y la variable de control. */
			actualLine = 0;

			/* Creamos un pipe a la ejecuci�n del comando tracert. */
			if (ptr = _popen(command, O_READ)) {
				
				/* Mientras obtengamos retorno, leemos las l�neas. */
				while (!feof(ptr) && (fgets(line, MAX_LINE - 1, ptr) != NULL)) {

					/* Descartamos las primeras l�neas sin datos a capturar. */
					if (actualLine < SPEED_TEST_NULL_LINES) {

						/* Purgamos line. */
						line[0] = '\0';

						/* Actualizamos el contador de l�neas. */
						actualLine++;

						continue;
					}

					/* Ignoramos l�neas en blanco y el mensaje final. */
					if (strnlen(line, MAX_LINE) > 20) {

						/* Informamos del proceso. */
						sprintf(processCount, "*");
						printf(processCount);

						/* Dividimos el string en substring usando ' ' como car�cter delimitador. */
						dumbStr = strtok(line, DNS_TEST_DELIM);

						/* Inicializamos el contador de palabras. */
						actualWord = 1;

						while (dumbStr) {
							
							if (actualWord == 1) {
								char* nptr;
								totalJumps = strtoul(dumbStr, &nptr, 10);
							}

							if (actualWord == 3) {

								if (strncmp(dumbStr, DNS_SPEED_TEST_KEY_CHAR, MAX_LINE) == 0) {
									actualTime = 1000.00;
								}
							}

							if (actualWord == 4) {

								if (strncmp(dumbStr, DNS_SPEED_TEST_KEY_CHAR, MAX_LINE) != 0) {
									char* nptr;
									actualTime = strtod(dumbStr, &nptr);
								}
							}

							/* Continuamos leyendo las subdivisiones. */
							dumbStr = strtok(NULL, DNS_TEST_DELIM);
							actualWord++;
						}

						/* Asignamos los resultados. */
						totalTime += actualTime;

						/* Purgamos line. */
						line[0] = '\0';
					}
				}
			}
			else {
				puts("Error: No puede ejecutarse el comando.");
				*retVal = 1;
			}

			/* Calculamos el tiempo medio de respuesta. */
			avrgTime = totalTime / (double)totalJumps;

			/* Asignamos los datos en la tabla. */
			testTable->dirs[i].jumps = totalJumps;
			testTable->dirs[i].avrgTime = avrgTime;

			/* Informamos de los resultados del test por pantalla. */
			puts("\nTest de velocidad finalizado.");
			puts("-------------------------------------------------");
			printf("Resultados del test sobre el servidor: %s\nSaltos totales: %u, Tiempo medio de respuesta: %.2lf\n", 
				testTable->dirs[i].dir, 
				totalJumps, 
				avrgTime);
			puts("-------------------------------------------------");

			/* Cerramos el pipe. */
			if (ptr) {
				_pclose(ptr);
			}
		}
	}
	else {
		puts("Error: La tabla no contiene elementos.");

		return;
	}

	/* Liberamos la memoria. */
	if (dnsDir) {
		free(dnsDir);
	}
	
	if (command) {
		free(command);
	}
	
	if (line) {
		free(line);
	}
	
	if (dumbStr) {
		free(dumbStr);
	}
	
	if (processCount) {
		free(processCount);
	}
}

void changeDns(const adptsTable adptsTbl , const dnsTestTable testTable, const dnsTable adptsDns, const int adptsInd, int* retVal) {

	*retVal = 0;

	char* command, * adminName;
	int commandRet = 0;

	/* Mostramos por pantalla la informaci�n del servidor m�s r�pido. */
	printBestDns(testTable);

	if (strncmp(testTable.dirs[0].dir, adptsDns.dns[0], MAX_DNS_LINE) == 0) {
		puts("Ya esta seleccionado el mejor adaptador.");

		return;
	}

	/* Inicializamos los strings. */
	command = (char*)malloc(MAX_COMMAND);
	adminName = (char*)malloc(MAX_WORD);

	if (command) {
		sprintf(command, "%s %u %s %s",
			ASSIGN_DNS_SERVER_1,
			adptsTbl.adptr[adptsInd].id,
			ASSIGN_DNS_SERVER_2,
			testTable.dirs[0].dir);
	}
	
	if (command) {
		commandRet = system(command);
		//commandRet = system("runas /user:Administrador \"netsh interface ipv4 set dnsservers 9 static 1.1.1.1\"");
	}

	if (commandRet < 0) {
		puts("Error: No se ha podido ejecutar el comando.");
		*retVal = 1;
	}
	else {
		puts("Se ha cambiado el servidor DNS del adaptador de red con exito!");
	}

	/* Liberamos la memoria. */
	if (command) {
		free(command);
	}

	if (adminName) {
		free(adminName);
	}
}

/// <summary>
/// Ordenamos mediante una funci�n recursiva tipo Quicksort los elementos de una tabla tipo dnsTestTable.
/// Quicksort se basa en el principio de "divide y vencer�s"; toma un elemento de la tabla como pivote, en este caso el elemento final,
/// y realiza una partici�n de la tabla alrededor del pivote. Para realizar la operaci�n, usa la funci�n partitionDnsTestTable() que tiene
/// como objetivo, tomando un elemento n de la tabla como pivote, colocarlo en su posici�n correcta y ordenar a su izquierda los elementos
/// menores y a su derecha los elementos mayores. 
/// El objetivo del uso de esta funcion es mantener la ejecuci�n en tiempo linear.
/// El factor usado para ordenar la tabla es, primero, tiempo medio de respuesta y, segundo, n�mero de saltos en caso de que el tiempo medio
/// de respuesta sea igual entre los dos elementos.
/// pivotInd = Recibe el �ndice de posici�n del pivote devuelto por la funci�n partitionDnsTestTable().
/// </summary>
/// <param name="table">dnsTestTable Tabla de elementos TAD dnsTest a ordenar.</param>
/// <param name="low">�ndice del primer elemento elemento.</param>
/// <param name="high">�ndice del �ltimo elemento.</param>
void qSortDnsTestTable(dnsTestTable* table, int low, int high) {

	/* Mientras no lleguemos al caso base, haber ordenado toda la tabla, encontramos el primer pivote de partici�n y dividimos la tabla
	* en dos, realizamos una llamada recursiva a cada mitad de la tabla para aplicar el concepto de divide y vencer�s. */
	if (low < high) {

		int pivotInd = partitionDnsTestTable(table, low, high);

		qSortDnsTestTable(table, low, pivotInd - 1);

		qSortDnsTestTable(table, pivotInd + 1, high);
	}
}

/// <summary>
/// Genera los puntos de partici�n para implementar el quicksort, mueve los elementos m�s peque�os a la izquierda del pivote y los
/// elementos m�s grandes a la derecha del pivote. El pivote queda colocado en su sitio. De este modo, es m�s sencillo ordenar
/// la tabla porque ya hemos generado este orden previo.
/// generaremos el orden a trav�s de la velocidad media de respuesta del servidor DNS, si esta es igual en dos servidores distintos,
/// escogeremos el servidor con menos saltos.
/// pivot = Elemento TAD tipo dnsTest que funcionar� como pivote de la comparaci�n.
/// ind = �ndice de posici�n de la partici�n.
/// </summary>
/// <param name="table">dnsTestTable Tabla de elementos TAD dnsTest a ordenar..</param>
/// <param name="low">int �ndice de posici�n del primer elemento de la tabla.</param>
/// <param name="high">int �ndice de posici�n del �ltimo elemento de la tabla..</param>
/// <returns>int �ndice de partici�n actual para aplicar la recursividad.</returns>
int partitionDnsTestTable(dnsTestTable* table, int low, int high) {

	dnsTest pivot;
	int ind;

	initDnsTest(&pivot);

	/* Escogemos el �ltimo elemento como pivote y situamos la posici�n inicial del �ndice para realizar el intercambio de elementos. */
	pivot = table->dirs[high];
	ind = (low - 1);

	for (int j = low; j < high; j++) {

		/* Comprobamos si el tiempo medio de respuesta del elemento es menor o igual al pivote. Si es menor, lo intercambiamos directamente, si es igual, 
		* comprobamos el n�mero de saltos.*/
		if (table->dirs[j].avrgTime <= pivot.avrgTime) {
			
			if (table->dirs[j].avrgTime < pivot.avrgTime) {
				
				/* Incrementamos el �ndice de posici�n del elemento menor. */
				ind++;

				/* Intercambiamos los elementos. */
				swapDnsTest(&table->dirs[j], &table->dirs[ind]);
			}
			else {

				if (table->dirs[j].jumps <= pivot.jumps) {
					
					/* Incrementamos el �ndice de posici�n del elemento menor. */
					ind++;

					/* Intercambiamos los elementos. */
					swapDnsTest(&table->dirs[j], &table->dirs[ind]);
				}
			}
		}
	}
	 
	/* Colocamos el pivote en su posici�n. */
	swapDnsTest(&table->dirs[ind + 1], &table->dirs[high]);

	return (ind + 1);
}

/// <summary>
/// Intercambia dos elementos TAD tipo dnsTest.
/// temp = Elemento TAD tipo dnsTest que funciona como variable temporal para poder realizar el intercambio.
/// </summary>
/// <param name="src1">dnsTest Origen 1.</param>
/// <param name="src2">dnsTest Origen2.</param>
void swapDnsTest(dnsTest* src1, dnsTest* src2) {

	/* Creamos una estructura TAD temporal. */
	dnsTest temp;

	/* Inicializamos la estructura. */
	initDnsTest(&temp);
	temp.dir = (char*)malloc(MAX_DNS_LINE);

	/* Intercambiamos los elementos. */
	if (temp.dir) {
		
		cpyDnsTest(*src1, &temp);
		cpyDnsTest(*src2, src1);
		cpyDnsTest(temp, src2);
	}

	/* Liberamos la memoria temporal. */
	if (temp.dir) {
		free(temp.dir);
	}
}

/// <summary>
/// Copia la informaci�n de un elemento TAD dnsTest de origen a otro de destino, obtenidos ambos por par�metros.
/// </summary>
/// <param name="src">dnsTest Elemento origen.</param>
/// <param name="dest">dnsTest Elemento destino.</param>
void cpyDnsTest(dnsTest src, dnsTest* dest) {
	
	dest->dir = (char*)malloc(MAX_DNS_LINE);
	if (dest->dir) {
		strncpy(dest->dir, src.dir, MAX_DNS_LINE);
	}
	
	dest->jumps = src.jumps;
	dest->avrgTime = src.avrgTime;
}

/// <summary>
/// Copia la informaci�n de un TAD tipo "adpatader" a otro TAD tipo "adaptador".
/// </summary>
/// <param name="dst">adapter Tad tipo "adapter" de destino.</param>
/// <param name="src">TAD tipo "adapter" de origen.</param>
void adptrCopy(adapter* dst, const adapter src) {
	dst->id = src.id;
	dst->metric = src.metric;
	dst->mtu = src.mtu;
	dst->state = src.state;
	dst->name = (char*)malloc(MAX_ADTR_LINE);
	if (src.name) {
		strncpy(dst->name, src.name, MAX_ADTR_LINE);
	}
	
}

/// <summary>
/// Imprime toda la informaci�n de los adaptores almacenados en una tabla adptsTable que almacena estructuras TAD del tipo "adapter".
/// </summary>
/// <param name="table">adptsTable Tabla adptsTable que almacena estructuras TAD del tipo "adapter"</param>
void printAdptsList(const adptsTable table) {
	for (unsigned int i = 0; i < table.numElem; i++) {
		printf("ID: %u Metric: %u MTU: %u State: %i Name: %s\n",
			table.adptr[i].id,
			table.adptr[i].metric,
			table.adptr[i].mtu,
			table.adptr[i].state,
			table.adptr[i].name);
	}
}

/// <summary>
/// Impreme las direcciones DNS de una tabla tipo dnsTable.
/// </summary>
/// <param name="table">dnsTable Tabla que contiene un array con las direcciones DNS.</param>
void printDnsTable(const dnsTable table) {
	for (unsigned int i = 0; i < table.numElem; i++) {
		printf("#%u. Direci�n DNS: %s\n", (i + 1), table.dns[i]);
	}
}

/// <summary>
/// Muestra por pantalla una lista con el nombre de todos los adaptadores de red del equipo que est�n conectados.
/// adptsNum: N�mero de adaptadores con conexi�n.
/// </summary>
/// <param name="table">adptsTable Tabla que contiene estructuras TAD de tipo adapter con la informaci�n de los adaptadores de red.</param>
/// <returns>int numAdpts N�mero de adaptadores con conexi�n.</returns>
int printAdptsNames(const adptsTable table) {

	int numAdpts = 0;

	for (unsigned int i = 0; i < table.numElem; i++) {

		/* Mostramos solo los adaptadores conectados. */
		if (table.adptr[i].state == true) {
			printf("#%u Name: %s\n", ++numAdpts, table.adptr[i].name);
		}
	}

	return numAdpts;
}

/// <summary>
/// Imprime por pantalla la informaci�n del servidor DNS m�s r�pido.
/// </summary>
/// <param name="testTable">dnsTestTable Tabla de elementos TAD tipo dnsTest.</param>
void printBestDns(const dnsTestTable testTable) {

	puts("\n-------------------------------------------------");
	puts("Datos del servidor mas rapido:");
	printf("Direccion: %s\tNumero de salto: %u\tTiempo de respuesta medio: %.2f\n",
		testTable.dirs[0].dir,
		testTable.dirs[0].jumps,
		testTable.dirs[0].avrgTime);
}

/// <summary>
/// Limpia completamente los datos de una tabla de DNS. Recorre todas las posiciones de la tabla, librea la memoria asignada y iguala
/// los punteros a NULL. Por �ltimo, resetea el n�mero de elemento a 0.
/// </summary>
/// <param name="table">dnsTable Tabla de direcciones DNS.</param>
void clearDnsTable(dnsTable* table) {

	/* Recorremos todas las posiciones de la tabla y liberamos la memoria asignada. */
	for (int i = 0; i < DNS_TABLE_LENGHT; i++) {
		free(table->dns[i]);
		table->dns[i] = NULL;
	}

	table->numElem = 0;
}

/// <summary>
/// Limpia completamente los datos de una tabla de adaptadores. Recorre todas las posiciones de la tabla, librea la memoria asignada e iguala
/// los punteros a NULL, resetea los campos a los valores por defecto. Por �ltimo, resetea el n�mero de elemento a 0.
/// </summary>
/// <param name="adptsTbl">adptsTable Tabla de elementos TAD de tipo adapter.</param>
void clearAdptsTable(adptsTable* adptsTbl) {

	for (unsigned int i = 0; i < adptsTbl->numElem; i++) {
		freeAdapter(&adptsTbl->adptr[i]);
	}

	adptsTbl->numElem = 0;
}

/// <summary>
/// Limpia completamente los datos de una tabla de elementos TAD tipo dnsTest. Recorre todas las posiciones de la tabla, librea la memoria asignada 
/// e iguala los punteros a NULL y resetea los campos a sus valores por defecto. Por �ltimo, resetea el n�mero de elemento a 0.
/// </summary>
/// <param name="testTable">dnsTestTable Tabla de elementos TAD tipo dnsTest.</param>
void clearDnsTestTable(dnsTestTable* testTable) {

	for (unsigned int i = 0; i < testTable->numElem; i++) {
		freeDnsTest(&testTable->dirs[i]);
	}

	testTable->numElem = 0;
}

/// <summary>
/// Liberamos una estructura TAD tipo dnsTest y reseteamos sus campos.
/// </summary>
/// <param name="test">dnsTest Estructura TAD tipo dnsTest que contiene los resultados de los test de velocidad a una direcci�n DNS.</param>
void freeDnsTest(dnsTest* test) {

	/* Liberamos la memoria del string. */
	free(test->dir);
	test->dir = NULL;

	/* Reseteamos el resto de valores. */
	test->jumps = 0;
	test->avrgTime = 0.0;
}

/// <summary>
/// Liberamos la memoria usada en una estructura TAD tipo adapter y reseteamos sus campos.
/// </summary>
/// <param name="adptr">adptr Estructura TAD que contiene los datos de un adaptador de red.</param>
void freeAdapter(adapter* adptr) {
	
	/* Liberamos la memoria del string. */
	free(adptr->name);
	adptr->name = NULL;

	/* Reseteamos el resto de valores. */
	adptr->id = -1;
	adptr->metric = -1;
	adptr->mtu = -1;
	adptr->state = false;
}

/// <summary>
/// Determina si la tabla espec�fica est� llena.
/// </summary>
/// <param name="table">adptsTable Tabla de TAD tipo "adaptador".</param>
/// <returns>
///   <c>true</c>, si la tabla est� llena; Si queda espacio, <c>false</c>.
/// </returns>
bool isFullAdpts(const adptsTable table) {
	return (table.numElem == ADPTRS_TABLE_LENGHT);
}

bool isFullDns(const dnsTable table) {
	return (table.numElem == DNS_TABLE_LENGHT);
}

bool isFullDnsTest(const dnsTestTable table) {
	return (table.numElem == DNS_TABLE_LENGHT);
}