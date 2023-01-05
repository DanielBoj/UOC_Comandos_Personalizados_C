#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "adapter.h"
#include "api.h"

/// <summary>
/// Carga la información de los adaptadores de red activos en una tabla de elementos TAD de tipo adapter.
/// La información se obtine lanzando desde el código un comando NETSH a tracén de un pipe de lectura con _popen().
/// La lectura d elos datos se carga en una estructura TAD tipo adapter temporal. Los datos se obtienen mediante la lectura línea a línea
/// del comando CMD. Se descartan las líneas sin interés mediant eun contador de líneas y, cuando llegamos a una línea con datos
/// de un adaptador, realizamos la lectura de la línea y la dividimos en substrings mediante la función strtok().
/// Escogemos las posiciones que nos interesan mediante un contador de palabras para obtener el índice del adaptador y su nombre.
/// Obtenemos también el estado del adpatador, si está activo, usamos un booleano de control para indicarlo.
/// Tras la lectura de la línea, comporbamos si el adaptador está conectado, si es así, cargamos los datos en variables temporales.
/// Tras esto, pasamos los datos a la estructura adpater y la cargamos en la tabla.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adpater.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan con éxito o 1 si no es así.</param>
void loadAdptrsInfo(adaptersTable* table, int* retVal) {

	*retVal = 0;

	FILE* ptr = NULL;
	int lineInd, actualWord, actIndex;
	bool isConnected;

	/* Adaptador temproal. */
	adapter actAdptr;

	/* Creamos el string para el comando, la lectura de líneas y los datos y los inicializamos. */
	char* command, * line, * dumbStr, * actName;
	command = (char*)malloc(MAX_COMMAND);
	line = (char*)malloc(MAX_LINE);
	dumbStr = (char*)malloc(MAX_LINE);
	actName = (char*)malloc(MAX_ADPTR_INFO);

	/* Inicializamos el adaptador. */
	initAdapter(&actAdptr);

	/* Comprobamos que se haya reservado el espacio de forma correcta. */
	if (command != NULL) {

		/* Concatenamos las diferentes partes del comando. */
		sprintf_s(command, sizeof(ADPTS_COMMAND), "%s", ADPTS_COMMAND);
		
		/* Lanzamos el comando via pipe. */
		if (ptr = _popen(command, O_READ)) {

			/* Inicializamos el contador de líneas y de inserción. */
			lineInd = 0;
			
			while (!feof(ptr)) {
				
				line[0] = '\0';

				if (fgets(line, MAX_LINE - 1, ptr) != NULL) {
			
					/* Ignoramos las primeras líneas. */
					if (lineInd < ADPTS_LOG_NULL_LINES) {

						/* Purgamos la línea. */
						line[0] = '\0';

						/* Avanzamos la línea. */
						lineInd++;
						
						continue;
					}

					/* Inicializamos el controlador de estado y el nombre del adaptador. */
					isConnected = false;
					actName[0] = '\0';

					/* Obtenemos los datos de la línea. */

					/* Dividimos el string en substring usando ' ' como carácter delimitador. */
					dumbStr = strtok(line, ADPTR_DELIM);
					
					/* Inicializamos el contador de palabras. */
					actualWord = 1;

					/* Realizamos la lectura de los datos que nos interesan. */
					while (dumbStr) {
						/* Cargamos el índice de adaptador. */
						if (actualWord == 1) {
							char* nptr;
							actIndex = strtol(dumbStr, &nptr, 10);
						}

						/* Cargamos el estado del adaptardor. */
						if (actualWord == 4) {

							/* Comprobamos si el adaptador está activo. */
							if (!strstr(dumbStr, KEY_WORD_ADPTR_OFF)) {
								isConnected = true;
							}
						}

						/* Cargamos el nombre del adaptador. */
						if (actualWord >= 5) {
							strncat(actName, dumbStr, strnlen(dumbStr, MAX_ADPTR_INFO));
							strncat(actName, " ", 2);
						}

						/* Continuamos con la lectura de los substrings. */
						dumbStr = strtok(NULL, ADPTR_DELIM);
						actualWord++;
					}

					dumbStr = strtok(actName, "\n");
					
					/* Comprobamos que el adaptador esté activo.*/
					if (isConnected && !adaptersTableIsFull(*table)) {
						
						/* Cargamos la información en un adaptador temporal. */
						strncpy(actAdptr.name, actName, MAX_ADPTR_INFO);
						actAdptr.index = actIndex;
						
						/* Añadimos el adaptador a la tabla. */
						addAdpatersNameAndIndex(table, actAdptr, retVal);
					}
					else {
						continue;
					}
				}
			}

		}
		else {
			puts("Error: No puede lanzarse el comando o no puede generarse el pipe.");

			*retVal = 1;
		}
	}
	else {
		puts("Error: El comando no se ha generado correctamente.");

		*retVal = 1;
	}

	/* Cerramos el pipe. */
	if (ptr) {
		_pclose(ptr);
	}

	/* Liberamos la memoria usada. */
	if (command) {
		free(command);
	}

	if (line) {
		free(line);
	}

	if (dumbStr) {
		free(dumbStr);
	}

	if (actName) {
		free(actName);
	}

	if (actAdptr.index != -1) {
		freeAdapter(&actAdptr);
	}
}

/// <summary>
/// Carga las distintas direcciones asociadas a un adaptador de red en los campos de información de un elmenton TAD tipo adaptador en
/// concreto de una tabla de adaptadores.
/// La función genera un comando CMD y captura su retorno a travé d ela creación y lectura de un pipe con _popen().
/// Se descartan las líneas sin información a ser capturada y se realiza un control de la línea de posición en el documento para conocer
/// qué datos s evan a ir cargando. Si nos encontramos en una línea con datos a capturar, se divide en substrings mendiante strtok() y se
/// va realizando la lectura de las palabras hasta que llegamos al dato a cargar. Se captura el dato con strncpy() y se limpian los
/// caracteres sobrantes ('\n', ')'). tras esto, se guardan los datos en el campo asignado del elemento TAD.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adpater.</param>
/// <param name="adptrInd">int Índice de adaptador, se usa para facilitar la llamada a los comandos CMD.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan con éxito o 1 si no es así.</param>
void getIpInfo(adaptersTable* table, const int adptrInd, int* retVal) {

	*retVal = 0;

	FILE* ptr = NULL;
	char* command, * line, * actIpDir, * dumbStr, * nullPtr;
	int lineInd, indPos, actWord;

	/* Inicializamos los strings. */
	command = (char*)malloc(MAX_COMMAND);
	line = (char*)malloc(MAX_LINE);
	actIpDir = (char*)malloc(MAX_IP);
	dumbStr = (char*)malloc(MAX_LINE);
	nullPtr = (char*)malloc(1);

	/* Buscamos la posición del adaptador a través del índice. */
	indPos = searchAdapater(*table, adptrInd, 0, (table->numElem - 1));

	/* Generamos el commando. */
	if (command != NULL) {
		sprintf_s(command, (sizeof(ADPTS_INFO_COMMAND) + sizeof(adptrInd)), "%s %d",
			ADPTS_INFO_COMMAND,
			adptrInd);

		/* Lanzamos el comando via pipe. */
		if (ptr = _popen(command, O_READ)) {

			/* Inicializamos el contador de líneas y de inserción. */
			lineInd = 0;

			while (!feof(ptr)) {

				line[0] = '\0';

				if (fgets(line, MAX_LINE - 1, ptr) != NULL) {

					/* Ignoramos las primeras líneas. */
					if (lineInd < ADPTS_DIRS_NULL_LINES) {

						/* Purgamos la línea. */
						line[0] = '\0';

						/* Avanzamos la línea. */
						lineInd++;

						continue;
					}

					/* Obtenemos la dirección IP. */
					if (lineInd == 3) {

						/* Dividimos el string en substrings usando ' ' como carácter delimitador. */
						dumbStr = strtok(line, ADPTR_DELIM);

						/* Inicializamos el contador de palabras. */
						actWord = 1;

						/* Realizamos la lectura de los datos que nos interesan. */
						while (dumbStr) {
						
							if (actWord == 3) {

								strncpy(actIpDir, dumbStr, MAX_IP);
								nullPtr = strtok(actIpDir, "\n");

								/* Carganmos los datos. */
								strncpy(table->adptrs[indPos].ip, actIpDir, MAX_IP);
							}

							/* Continuamos con la lectura de los substrings. */
							dumbStr = strtok(NULL, ADPTR_DELIM);
							actWord++;
						}
					}

					/* Obtenemos la máscara. */
					if (lineInd == 4) {

						/* Dividimos el string en substrings usando ' ' como carácter delimitador. */
						dumbStr = strtok(line, ADPTR_DELIM);

						/* Inicializamos el contador de palabras. */
						actWord = 1;

						/* Realizamos la lectura de los datos que nos interesan. */
						while (dumbStr) {

							if (actWord == 6) {
								strncpy(actIpDir, dumbStr, MAX_IP);
								nullPtr = strtok(actIpDir, "\n");
								nullPtr = strtok(actIpDir, ")");

								/* Carganmos los datos. */
								strncpy(table->adptrs[indPos].mask, actIpDir, MAX_IP);
							}

							/* Continuamos con la lectura de los substrings. */
							dumbStr = strtok(NULL, ADPTR_DELIM);
							actWord++;
						}
					}

					/* Obtenemos la puerta de enlace */
					if (lineInd == 5) {

						/* Dividimos el string en substrings usando ' ' como carácter delimitador. */
						dumbStr = strtok(line, ADPTR_DELIM);

						/* Inicializamos el contador de palabras. */
						actWord = 1;

						/* Realizamos la lectura de los datos que nos interesan. */
						while (dumbStr) {

							if (actWord == 5) {
								strncpy(actIpDir, dumbStr, MAX_IP);
								nullPtr = strtok(actIpDir, "\n");

								/* Carganmos los datos. */
								strncpy(table->adptrs[indPos].gateway, actIpDir, MAX_IP);
							}

							/* Continuamos con la lectura de los substrings. */
							dumbStr = strtok(NULL, ADPTR_DELIM);
							actWord++;
						}
					}

					/* Obtenemos el servidor DNS. */
					if (lineInd == 8) {

						/* Dividimos el string en substrings usando ' ' como carácter delimitador. */
						dumbStr = strtok(line, ADPTR_DELIM);

						/* Inicializamos el contador de palabras. */
						actWord = 1;

						/* Realizamos la lectura de los datos que nos interesan. */
						while (dumbStr) {

							if (actWord == 5) {
								strncpy(actIpDir, dumbStr, MAX_IP);
								nullPtr = strtok(actIpDir, "\n");

								/* Carganmos los datos. */
								strncpy(table->adptrs[indPos].dnsServer, actIpDir, MAX_IP);
							}

							/* Continuamos con la lectura de los substrings. */
							dumbStr = strtok(NULL, ADPTR_DELIM);
							actWord++;
						}
					}

					lineInd++;
				}
			}

		}
		else {
			puts("Error: No puede lanzarse el comando o no puede generarse el pipe.");

			*retVal = 1;
		}

	}
	else {
		puts("Error: El comando no se ha generado correctamente.");

		*retVal = 1;
	}

	/* Cerramos el pipe. */
	if (ptr) {
		_pclose(ptr);
	}

	/* Liberamos la memoria usada. */
	if (command) {
		free(command);
	}

	if (line) {
		free(line);
	}

	if (dumbStr) {
		free(dumbStr);
	}

	if (actIpDir) {
		free(actIpDir);
	}

	if (nullPtr) {
		nullPtr = NULL;
		free(nullPtr);
	}

}

/// <summary>
/// Realiza un diagnóstico tippo tracert al servidor DNS del adaptador de red.
/// La función recibe por parámetros la tabla de elementos TAD tipo adapter y el índice del adaptador.
/// A través del índice, busca la posición del adaptador en la tabla para poderlo actualizar mediante searchAdapter().
/// Genera el comando y lo lanza creando un pipe de lectura a un puntero FILE mediante _popen() y realiza la lectura del retorno.
/// Se ignoran las líneas sin información a capturar, una vez iniciada la lectura de las líneas con información, se comprueba
/// mediante la búsqueda de carácteres clave si se trata de un retorno nulo, usando strstr(). Si es así, como el comando se ha
/// lanzado con una restricción de tiempo máximo de 1000ms, se le da esa duración al salto.
/// Si el salto devuelve respuesta, se capturan el tiempo medio de respuesta y la dirección y, si lo hay, nombre del servidor.
/// La información se va cargando al adaptador, mediante un contador de posiciones, se irá llenando la tabla de elementos tipo
/// jump con cada uno de los saltos. Una vez se llega al EOF, se realiza el cálculo del tiempo medio y se carga la información
/// en el elemento adapter seleccionado de la tabla.
/// Los strings se gestionan mediante memoria dinámica.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adpater.</param>
/// <param name="adptrInd">int Índice de adaptador, se usa para facilitar la llamada a los comandos CMD.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan con éxito o 1 si no es así.</param>
void getDnsTest(adaptersTable* table, const int adptrInd, int* retVal) {

	*retVal = 0;

	FILE* ptr = NULL;
	char* command, * line, * dumbStr, * actJumpIp, * processCount;
	int lineInd, indPos, actWord, actJump;
	double actTime, totalTime;
	bool isOutOfReach;

	/* Inicializamos los strings. */
	command = (char*)malloc(MAX_COMMAND);
	line = (char*)malloc(MAX_LINE);
	dumbStr = (char*)malloc(MAX_LINE);
	actJumpIp = (char*)malloc(MAX_ADPTR_INFO);
	processCount = (char*)malloc(31);

	//totalTime = 0.0;
	actJump = 0;

	/* Buscamos la posición del adaptador a través del índice. */
	indPos = searchAdapater(*table, adptrInd, 0, (table->numElem - 1));

	/* Generamos el commando. */
	if (command != NULL) {
		sprintf_s(command, (sizeof(ADPTS_INFO_COMMAND) + sizeof(table->adptrs[indPos].dnsServer)), "%s %s",
			DNS_TEST_COMAND,
			table->adptrs[indPos].dnsServer);

		/* Lanzamos el comando via pipe. */
		if (ptr = _popen(command, O_READ)) {

			/* Información de salida para el usuario. */
			puts("-------------------------------------------------");
			puts("Realizando el test de velocidad a los servidores DNS:\n"
				"(Las acciones pueden demorarse un poco)");
			puts("-------------------------------------------------");
			printf_s("%s %s %s",
				"Calculo de la traza al servidor DNS",
				table->adptrs[indPos].dnsServer,
				"en proceso:\n");

			/* Inicializamos el contador de líneas y de saltos. */
			lineInd = 0;
			totalTime = 0.0;

			while (!feof(ptr)) {

				/* Iniciamos los contadores y el controlador. */
				actTime = 0.0;

				if (fgets(line, MAX_LINE - 1, ptr) != NULL) {
					
					/* Inicializamos el controlador, el contador y el string de dirección de salto. */
					isOutOfReach = false;
					actJumpIp[0] = '\0';

					/* Ignoramos las primeras líneas. */
					if (lineInd < 4) {

						/* Purgamos la línea. */
						line[0] = '\0';

						/* Avanzamos la línea. */
						lineInd++;

						continue;
					}

					/* Ignoramos líneas en blanco y el mensaje final. */
					if (strnlen(line, MAX_LINE) > 20) {
						
						/* Informamos del proceso. */
						sprintf_s(processCount, 31 , "*");
						printf_s(processCount);

						/* Dividimos el string en substrings usando ' ' como carácter delimitador. */
						dumbStr = strtok(line, ADPTR_DELIM);

						/* Inicializamos el contador de palabras. */
						actWord = 1;

						/* Realizamos la lectura de los datos que nos interesan. */
						while (dumbStr) {

							/* Comporbamos si se ha producido un retorno "tiempo de espera agotado". */
							if ((actWord == 2 || actWord == 3) && strstr(dumbStr, DNS_CONTROL_CHAR)) {

								actTime = 1000.00;

								/* Asignamos un mensaje de error a la dirección. */
								table->adptrs[indPos].jumps[actJump].ip = (char*)malloc(MAX_ADPTR_INFO);
								if (strncpy(table->adptrs[indPos].jumps[actJump].ip,
									SRV_NO_RECHEABLE,
									MAX_ADPTR_INFO) == NULL) {
									puts("Error: No puede escribirse la informacion!");

									*retVal = 1;
								}

								/* Seteamos el controlador */
								isOutOfReach = true;
							}

							if (actWord == 4 && !isOutOfReach) {

								/* Capturamos el tiempo medio del salto. */
								char* nptr;
								actTime = strtod(dumbStr, &nptr);
							}

							if (actWord >= 7 && !isOutOfReach) {

								/* Capturamos la dirección del salto. */
								if (!strstr(dumbStr, DNS_CONTROL_CHAR) &&
									!strstr(dumbStr, DNS_CONTROL_CHAR_2)) {
									strncat(actJumpIp, dumbStr, strnlen(dumbStr, MAX_ADPTR_INFO));
									strncat(actJumpIp, " ", 2);
								}
							}

							/* Continuamos con la lectura de los substrings. */
							dumbStr = strtok(NULL, ADPTR_DELIM);
							actWord++;
						}
						
						/* Limpiamos el salto de línea. */
						if (!isOutOfReach) {
							char* nptr;
							nptr = strtok(actJumpIp, "\n");
							strncat(actJumpIp, "\0", 1);
						}

						/* Asignamos la dirección de salto. */
						if (!isOutOfReach) {
							table->adptrs[indPos].jumps[actJump].ip = (char*)malloc(MAX_ADPTR_INFO);
							if (strncpy(table->adptrs[indPos].jumps[actJump].ip, actJumpIp, MAX_ADPTR_INFO) == NULL) {
								puts("Error: No se ha podido escribir la información!");

								*retVal = 1;
							}
						}

						/* Asignamos el número de salto y actualizamos le contador. */
						table->adptrs[indPos].jumps[actJump].jumpNum = actJump + 1;
						actJump++;
					}	
				}

				/* Actualizamos el tiempo medio. */
				totalTime += actTime;
			}
		}
		else {
			puts("Error: No puede lanzarse el comando o no puede generarse el pipe.");

			*retVal = 1;
		}

		/* Asignamos el valor medio y el número de saltos. */
		table->adptrs[indPos].avgRespTime = totalTime / (double)actJump;
		table->adptrs[indPos].totalJumps = actJump;

		puts("\n-------------------------------------------------");
		printAdapterInfo(*table, indPos);

		/* Cerramos el pipe. */
		if (ptr) {
			_pclose(ptr);
		}
	}
	else {
		puts("Error: El comando no se ha generado correctamente.");

		*retVal = 1;
	}

	/* Liberamos la memoria temporal. */

	if (command) {
		free(command);
	}

	if (line) {
		free(line);
	}

	if (dumbStr) {
		free(dumbStr);
	}

	if (actJumpIp) {
		free(actJumpIp);
	}

	if (processCount) {
		free(processCount);
	}
}

/// <summary>
/// Realiza una carga inicial de datos a la tabla de elementos TAD tipo adaptador. Usa funciones de copia de string para el nombre,
/// strncpy(), carga el resto de datos por asignación directa y actualiza el número de elementos.
/// Se realiza el control de errores.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="adptr">adapter Estructura TAD de tipo adapter con los datos del adaptador.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan correctamente, o 1 si hay errores.</param>
void addAdpatersNameAndIndex(adaptersTable* table, const adapter adptr, int* retVal) {

	*retVal = 0;

	int posInd;

	/* Asignamos el índice de inserción. */
	posInd = table->numElem;

	/* Cargamos los datos. */
	if (strncpy(table->adptrs[posInd].name, adptr.name, MAX_ADPTR_INFO) == NULL) {
		puts("Error: No pueden cargarse los datos!");

		*retVal = 1;
	}

	table->adptrs[posInd].index = adptr.index;

	/* Actualizamos el número de elementos. */
	table->numElem++;
}

/// <summary>
/// Devuelve el índice de posición en una tabla de elementos TAD tipo adapter de un adaptador de red búscado por su número de índice que
/// recibe como parámetro.
/// Es una función de tipo recursiva que recibe el punto de inicio de la tabla y el tamaño de lista como punto final por parámetro. Va
/// realizando una búsqueda en orden inverso por la lista. Si encuentra la posición, devuelve el índice, si no, devuelve -1.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="indToFind">int Índice de adaptador del elemento a buscar.</param>
/// <param name="start">int Inicio de la tabla.</param>
/// <param name="end">int Posición final de la tabla.</param>
/// <returns></returns>
int searchAdapater(const adaptersTable table, const int indToFind, const int start, const int end) {

	/* Declaramos el caso base. */
	if (end < start) {
		return -1;
	}

	/* Declaramos la condición de final de búsqueda. */
	if (table.adptrs[end].index == indToFind) {
		return end;
	}

	/* Llamamos recursivamente a la función. */
	return searchAdapater(table, indToFind, start, (end - 1));
}

/// <summary>
/// Imprime por pantalla los datos de un elemento TAD tipo adapter.
/// El elemento se escoge de una tabla tipo adaptersTable que recibe por parámetro a través de un índice de posición que también
/// se recibe por parámetro.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="indPos">int Índice de posición del adaptador.</param>
void printAdapterInfo(const adaptersTable table, const int indPos) {

	
	puts("Datos del adaptador de red seleccionado:");
	printf("Nombre: %s\nDireccion IP: %s\nMascara de subred: %s\nPuerta de enlace: %s\nDNS: %s\nTiempo medio de respuesta: %.2f\nSaltos totales: %d\n",
		table.adptrs[indPos].name,
		table.adptrs[indPos].ip,
		table.adptrs[indPos].mask,
		table.adptrs[indPos].gateway,
		table.adptrs[indPos].dnsServer,
		table.adptrs[indPos].avgRespTime,
		table.adptrs[indPos].totalJumps);
	for (unsigned int i = 0; i < table.adptrs[indPos].totalJumps; i++) {
		printf("Numero de salto: %d, Direccion del servidor: %s\n",
			table.adptrs[indPos].jumps[i].jumpNum,
			table.adptrs[indPos].jumps[i].ip);
	}
}