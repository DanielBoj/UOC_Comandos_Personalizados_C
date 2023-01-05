#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "adapter.h"
#include "api.h"

/// <summary>
/// Carga la informaci�n de los adaptadores de red activos en una tabla de elementos TAD de tipo adapter.
/// La informaci�n se obtine lanzando desde el c�digo un comando NETSH a trac�n de un pipe de lectura con _popen().
/// La lectura d elos datos se carga en una estructura TAD tipo adapter temporal. Los datos se obtienen mediante la lectura l�nea a l�nea
/// del comando CMD. Se descartan las l�neas sin inter�s mediant eun contador de l�neas y, cuando llegamos a una l�nea con datos
/// de un adaptador, realizamos la lectura de la l�nea y la dividimos en substrings mediante la funci�n strtok().
/// Escogemos las posiciones que nos interesan mediante un contador de palabras para obtener el �ndice del adaptador y su nombre.
/// Obtenemos tambi�n el estado del adpatador, si est� activo, usamos un booleano de control para indicarlo.
/// Tras la lectura de la l�nea, comporbamos si el adaptador est� conectado, si es as�, cargamos los datos en variables temporales.
/// Tras esto, pasamos los datos a la estructura adpater y la cargamos en la tabla.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adpater.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan con �xito o 1 si no es as�.</param>
void loadAdptrsInfo(adaptersTable* table, int* retVal) {

	*retVal = 0;

	FILE* ptr = NULL;
	int lineInd, actualWord, actIndex;
	bool isConnected;

	/* Adaptador temproal. */
	adapter actAdptr;

	/* Creamos el string para el comando, la lectura de l�neas y los datos y los inicializamos. */
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

			/* Inicializamos el contador de l�neas y de inserci�n. */
			lineInd = 0;
			
			while (!feof(ptr)) {
				
				line[0] = '\0';

				if (fgets(line, MAX_LINE - 1, ptr) != NULL) {
			
					/* Ignoramos las primeras l�neas. */
					if (lineInd < ADPTS_LOG_NULL_LINES) {

						/* Purgamos la l�nea. */
						line[0] = '\0';

						/* Avanzamos la l�nea. */
						lineInd++;
						
						continue;
					}

					/* Inicializamos el controlador de estado y el nombre del adaptador. */
					isConnected = false;
					actName[0] = '\0';

					/* Obtenemos los datos de la l�nea. */

					/* Dividimos el string en substring usando ' ' como car�cter delimitador. */
					dumbStr = strtok(line, ADPTR_DELIM);
					
					/* Inicializamos el contador de palabras. */
					actualWord = 1;

					/* Realizamos la lectura de los datos que nos interesan. */
					while (dumbStr) {
						/* Cargamos el �ndice de adaptador. */
						if (actualWord == 1) {
							char* nptr;
							actIndex = strtol(dumbStr, &nptr, 10);
						}

						/* Cargamos el estado del adaptardor. */
						if (actualWord == 4) {

							/* Comprobamos si el adaptador est� activo. */
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
					
					/* Comprobamos que el adaptador est� activo.*/
					if (isConnected && !adaptersTableIsFull(*table)) {
						
						/* Cargamos la informaci�n en un adaptador temporal. */
						strncpy(actAdptr.name, actName, MAX_ADPTR_INFO);
						actAdptr.index = actIndex;
						
						/* A�adimos el adaptador a la tabla. */
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
/// Carga las distintas direcciones asociadas a un adaptador de red en los campos de informaci�n de un elmenton TAD tipo adaptador en
/// concreto de una tabla de adaptadores.
/// La funci�n genera un comando CMD y captura su retorno a trav� d ela creaci�n y lectura de un pipe con _popen().
/// Se descartan las l�neas sin informaci�n a ser capturada y se realiza un control de la l�nea de posici�n en el documento para conocer
/// qu� datos s evan a ir cargando. Si nos encontramos en una l�nea con datos a capturar, se divide en substrings mendiante strtok() y se
/// va realizando la lectura de las palabras hasta que llegamos al dato a cargar. Se captura el dato con strncpy() y se limpian los
/// caracteres sobrantes ('\n', ')'). tras esto, se guardan los datos en el campo asignado del elemento TAD.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adpater.</param>
/// <param name="adptrInd">int �ndice de adaptador, se usa para facilitar la llamada a los comandos CMD.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan con �xito o 1 si no es as�.</param>
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

	/* Buscamos la posici�n del adaptador a trav�s del �ndice. */
	indPos = searchAdapater(*table, adptrInd, 0, (table->numElem - 1));

	/* Generamos el commando. */
	if (command != NULL) {
		sprintf_s(command, (sizeof(ADPTS_INFO_COMMAND) + sizeof(adptrInd)), "%s %d",
			ADPTS_INFO_COMMAND,
			adptrInd);

		/* Lanzamos el comando via pipe. */
		if (ptr = _popen(command, O_READ)) {

			/* Inicializamos el contador de l�neas y de inserci�n. */
			lineInd = 0;

			while (!feof(ptr)) {

				line[0] = '\0';

				if (fgets(line, MAX_LINE - 1, ptr) != NULL) {

					/* Ignoramos las primeras l�neas. */
					if (lineInd < ADPTS_DIRS_NULL_LINES) {

						/* Purgamos la l�nea. */
						line[0] = '\0';

						/* Avanzamos la l�nea. */
						lineInd++;

						continue;
					}

					/* Obtenemos la direcci�n IP. */
					if (lineInd == 3) {

						/* Dividimos el string en substrings usando ' ' como car�cter delimitador. */
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

					/* Obtenemos la m�scara. */
					if (lineInd == 4) {

						/* Dividimos el string en substrings usando ' ' como car�cter delimitador. */
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

						/* Dividimos el string en substrings usando ' ' como car�cter delimitador. */
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

						/* Dividimos el string en substrings usando ' ' como car�cter delimitador. */
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
/// Realiza un diagn�stico tippo tracert al servidor DNS del adaptador de red.
/// La funci�n recibe por par�metros la tabla de elementos TAD tipo adapter y el �ndice del adaptador.
/// A trav�s del �ndice, busca la posici�n del adaptador en la tabla para poderlo actualizar mediante searchAdapter().
/// Genera el comando y lo lanza creando un pipe de lectura a un puntero FILE mediante _popen() y realiza la lectura del retorno.
/// Se ignoran las l�neas sin informaci�n a capturar, una vez iniciada la lectura de las l�neas con informaci�n, se comprueba
/// mediante la b�squeda de car�cteres clave si se trata de un retorno nulo, usando strstr(). Si es as�, como el comando se ha
/// lanzado con una restricci�n de tiempo m�ximo de 1000ms, se le da esa duraci�n al salto.
/// Si el salto devuelve respuesta, se capturan el tiempo medio de respuesta y la direcci�n y, si lo hay, nombre del servidor.
/// La informaci�n se va cargando al adaptador, mediante un contador de posiciones, se ir� llenando la tabla de elementos tipo
/// jump con cada uno de los saltos. Una vez se llega al EOF, se realiza el c�lculo del tiempo medio y se carga la informaci�n
/// en el elemento adapter seleccionado de la tabla.
/// Los strings se gestionan mediante memoria din�mica.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adpater.</param>
/// <param name="adptrInd">int �ndice de adaptador, se usa para facilitar la llamada a los comandos CMD.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan con �xito o 1 si no es as�.</param>
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

	/* Buscamos la posici�n del adaptador a trav�s del �ndice. */
	indPos = searchAdapater(*table, adptrInd, 0, (table->numElem - 1));

	/* Generamos el commando. */
	if (command != NULL) {
		sprintf_s(command, (sizeof(ADPTS_INFO_COMMAND) + sizeof(table->adptrs[indPos].dnsServer)), "%s %s",
			DNS_TEST_COMAND,
			table->adptrs[indPos].dnsServer);

		/* Lanzamos el comando via pipe. */
		if (ptr = _popen(command, O_READ)) {

			/* Informaci�n de salida para el usuario. */
			puts("-------------------------------------------------");
			puts("Realizando el test de velocidad a los servidores DNS:\n"
				"(Las acciones pueden demorarse un poco)");
			puts("-------------------------------------------------");
			printf_s("%s %s %s",
				"Calculo de la traza al servidor DNS",
				table->adptrs[indPos].dnsServer,
				"en proceso:\n");

			/* Inicializamos el contador de l�neas y de saltos. */
			lineInd = 0;
			totalTime = 0.0;

			while (!feof(ptr)) {

				/* Iniciamos los contadores y el controlador. */
				actTime = 0.0;

				if (fgets(line, MAX_LINE - 1, ptr) != NULL) {
					
					/* Inicializamos el controlador, el contador y el string de direcci�n de salto. */
					isOutOfReach = false;
					actJumpIp[0] = '\0';

					/* Ignoramos las primeras l�neas. */
					if (lineInd < 4) {

						/* Purgamos la l�nea. */
						line[0] = '\0';

						/* Avanzamos la l�nea. */
						lineInd++;

						continue;
					}

					/* Ignoramos l�neas en blanco y el mensaje final. */
					if (strnlen(line, MAX_LINE) > 20) {
						
						/* Informamos del proceso. */
						sprintf_s(processCount, 31 , "*");
						printf_s(processCount);

						/* Dividimos el string en substrings usando ' ' como car�cter delimitador. */
						dumbStr = strtok(line, ADPTR_DELIM);

						/* Inicializamos el contador de palabras. */
						actWord = 1;

						/* Realizamos la lectura de los datos que nos interesan. */
						while (dumbStr) {

							/* Comporbamos si se ha producido un retorno "tiempo de espera agotado". */
							if ((actWord == 2 || actWord == 3) && strstr(dumbStr, DNS_CONTROL_CHAR)) {

								actTime = 1000.00;

								/* Asignamos un mensaje de error a la direcci�n. */
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

								/* Capturamos la direcci�n del salto. */
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
						
						/* Limpiamos el salto de l�nea. */
						if (!isOutOfReach) {
							char* nptr;
							nptr = strtok(actJumpIp, "\n");
							strncat(actJumpIp, "\0", 1);
						}

						/* Asignamos la direcci�n de salto. */
						if (!isOutOfReach) {
							table->adptrs[indPos].jumps[actJump].ip = (char*)malloc(MAX_ADPTR_INFO);
							if (strncpy(table->adptrs[indPos].jumps[actJump].ip, actJumpIp, MAX_ADPTR_INFO) == NULL) {
								puts("Error: No se ha podido escribir la informaci�n!");

								*retVal = 1;
							}
						}

						/* Asignamos el n�mero de salto y actualizamos le contador. */
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

		/* Asignamos el valor medio y el n�mero de saltos. */
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
/// strncpy(), carga el resto de datos por asignaci�n directa y actualiza el n�mero de elementos.
/// Se realiza el control de errores.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="adptr">adapter Estructura TAD de tipo adapter con los datos del adaptador.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se realizan correctamente, o 1 si hay errores.</param>
void addAdpatersNameAndIndex(adaptersTable* table, const adapter adptr, int* retVal) {

	*retVal = 0;

	int posInd;

	/* Asignamos el �ndice de inserci�n. */
	posInd = table->numElem;

	/* Cargamos los datos. */
	if (strncpy(table->adptrs[posInd].name, adptr.name, MAX_ADPTR_INFO) == NULL) {
		puts("Error: No pueden cargarse los datos!");

		*retVal = 1;
	}

	table->adptrs[posInd].index = adptr.index;

	/* Actualizamos el n�mero de elementos. */
	table->numElem++;
}

/// <summary>
/// Devuelve el �ndice de posici�n en una tabla de elementos TAD tipo adapter de un adaptador de red b�scado por su n�mero de �ndice que
/// recibe como par�metro.
/// Es una funci�n de tipo recursiva que recibe el punto de inicio de la tabla y el tama�o de lista como punto final por par�metro. Va
/// realizando una b�squeda en orden inverso por la lista. Si encuentra la posici�n, devuelve el �ndice, si no, devuelve -1.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="indToFind">int �ndice de adaptador del elemento a buscar.</param>
/// <param name="start">int Inicio de la tabla.</param>
/// <param name="end">int Posici�n final de la tabla.</param>
/// <returns></returns>
int searchAdapater(const adaptersTable table, const int indToFind, const int start, const int end) {

	/* Declaramos el caso base. */
	if (end < start) {
		return -1;
	}

	/* Declaramos la condici�n de final de b�squeda. */
	if (table.adptrs[end].index == indToFind) {
		return end;
	}

	/* Llamamos recursivamente a la funci�n. */
	return searchAdapater(table, indToFind, start, (end - 1));
}

/// <summary>
/// Imprime por pantalla los datos de un elemento TAD tipo adapter.
/// El elemento se escoge de una tabla tipo adaptersTable que recibe por par�metro a trav�s de un �ndice de posici�n que tambi�n
/// se recibe por par�metro.
/// </summary>
/// <param name="table">adaptersTable Tabla de elementos TAD tipo adapter.</param>
/// <param name="indPos">int �ndice de posici�n del adaptador.</param>
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