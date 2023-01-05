#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Adaptador.h"
#include "LinkedList.h"
#include "Ping.h"

/**
 * @brief openFile(): Abre un archivo. Esta función usa un método más deguro de
 * apertura -> foprn_s() que devuelve el número de error de haber cualquier
 * problema. Usamos una función porque así podemos checkear los errores y no hay que
 * repetir el código cada vez que usemos fopen().
 * Recibe la ruta absoluta o relativa y el modo de apertura por parámetro.
 *
 * @param nombre String Ruta abasoluta o relativa del archivo.
 * @param modo  String Modo de apertura del archivo.
 * @return FILE* Puntero a FILE.
 */
FILE* openFile(const char* nombre, const char* modo) {
    /* Declaración de la variable que contendrá el puntero al archivo de texto, * Para ello, necesitamos un puntero a FILE. */
    FILE* p;
    /* Tipo de datos para almacena run resultado de error. */
    errno_t err;

    /**
     * Debemos abrir el archivo mediante el método fopen(), por parámetros le pasamos el nombre del archivo y el modo de aperturo.
     */
    err = fopen_s(&p, nombre, modo);

    /**
     * Control de apertura: Si el fichero no existiera o no se puediera abrir, el método asignará un valor NULL al puntero. Para asegurar que el resto de funciones puedan ejecutarse, comprobamos si se ha abierto con éxito.
     */
    if (err != 0) {
        puts("No es posible abrir el fichero!");
        return NULL;
    }
    else {
        return p;
    }
}

/**
 * @brief createArchivoLog(): genera un archivo txt externo para escribir los
 * nombres de los adaptadores de red. Realiza la creación del puntero a FILE
 * mediante el método openfile() y cierra el archivo para que podamos usar
 * comandos de redireccionamiento desde la llamada a system.
 *
 * @return FILE* Puntero a FILE.
 *
 * @param nombre String Ruta relativa o absoluta del archivo.
 * @param modo String Modo de apertura, para que funcione bien deberemos usar
 * un modo con el flag '+' que permite crear el archivo si no existe.
 * @return FILE* Puntero a File.
 */
FILE* createArchivoLog(char* nombre, char* modo) {
    /* Creamos el puntero a FILE. */
    FILE* p;

    /* Abrimos el archivo en modo w+ para crearlo y lo cerramos para poder trabajar con redirección de system(). */
    p = openFile(nombre, modo);

    /* Liberamos la memoria para poder trabajar con el archivo externamente. */
    fclose(p);

    return p;
}

/**
 * @brief createFileNombresAdapter(): Escribe los nombres de los adaptadores en
 * un archivo log que genera automáticamente. Si ya existe, lo reescribe.
 *
 * @param nombreArchivo String Ruta relativa o absoluta del archivo.
 * @param modo String Modo de apertira del archivo, para nuestra intención
 *  usaremos un modo de escritura con el flag + que está creado como la
 * constante O_ESCRITURA.
 * @return int
 */
void createFileNombresAdptr(char* nombreArchivo, char* modo) {
    FILE* nombresAdptr;

    /* Strings: Reservamos la memoria al crear las variables */
    char* command = malloc(MAX_COMMAND_DESC);

    /* Creamos el archivo de log */
    nombresAdptr = createArchivoLog(nombreArchivo, modo);

    /* Si el retorno es correcto, obtenemos los nombres de los
     * adaptadores mediante una llamada a system(). */
    if (nombresAdptr != NULL) {

        if (command != NULL) {
            strncpy(command, "ipconfig | findstr Adaptador > ", MAX_COMMAND_DESC);
            strncat(command, LOG_ADPTRS, strnlen(LOG_ADPTRS, MAX_COMMAND_DESC));
            system(command);
        }
    }
    else {
        puts("El archivo no puede escribirse!");
        return;
    }

    /* Cerramos el archivo para liberar la memoria */
    fclose(nombresAdptr);
}

/**
 * @brief fillListAdaptadores(): Asigana los diferentes nombres de los adaptadores registrados
 * en el archivo log a nodos de una LinkedList.
 * Como los nombre se graban con el carácter final ':', buscamos la posición de ese char mediante
 * la función strcspn() y el tring carácter a carácter hasta justo antes de llegar a ':' en un
 * String limpio que añadiremos a la lista mediante la función apperndAdptr().
 *
 * @return listaAdptr Listed Link de TAD tipo adptr.
 */
listaAdptr fillListaAdaptadores() {
    /* Variables locales. */
    listaAdptr lista;
    char* actualNombreAdptr;
    char nombreLimpio[MAX_ADPT_NAME];
    FILE* pLog;

    char* limitChar = ":";
    int lenght;

    /* Inicializamos la lista de adaptadores. */
    initListaAdptr(&lista);

    /* Abrimos el archivo log */
    pLog = openFile(LOG_ADPTRS, O_LECTURA);
    
    if (pLog == NULL) {
        puts("Error de acceso a los datos.");
        return lista;
    }
    else {
        actualNombreAdptr = malloc(MAX_ADPT_NAME);

        while (!feof(pLog)) {
            /* Intentamos obtener una nueva línea. */
            if (fgets(actualNombreAdptr, MAX_ADPT_NAME, pLog)) {

                /* Queremos purgar el carácter ':'. */
                lenght = strcspn(actualNombreAdptr, limitChar);
                for (int i = 0; i < lenght; i++) {
                    nombreLimpio[i] = actualNombreAdptr[i];
                }
                nombreLimpio[lenght] = '\0';
                appendAdptr(&lista, nombreLimpio);
            }
        }
    }
    if (actualNombreAdptr) {
        free(actualNombreAdptr);
     }
   
    fclose(pLog);

    return lista;
}

/**
 * @brief cargarInformacionAdaptadores(): Genera un archivo de texto log en el directorio raiz
 * donde se guarda la información de todos los adpatadores de red.
 * Para hacerlo, se crea un comando de CMD ipconfig que se ejecuta mediante system().
 *
 */
void cargarInformacionAdaptadores() {
    char* command;
    FILE* pLog;

    command = malloc(MAX_COMMAND_DESC);
    if (command != NULL) {
        strncpy(command, "ipconfig > ", MAX_COMMAND_DESC);
        strncat(command, LOG_IPCONFIG, strnlen(LOG_IPCONFIG, MAX_COMMAND_DESC));
    }

    /* Creamos el archivo log. */
    pLog = createArchivoLog(LOG_IPCONFIG, O_ESCRITURA);

    /* Ejecutamos el comando CMD y guardamos la información en el archivo LOG */
    if (command != NULL) {
        system(command);
    }

    if (command) {
        free(command);
    }
}

/**
 * @brief mostrarInformacionAdptr(): Muestra la información de uno de los adaptadores accediendo
 * al documento log que se ha generado previamente.
 * Es el comando más complejo de este módulo; el comando busca el nombre del adaptador en la
 * Linked List de adaptadores mediante buscarAdptrPorKey(), para ello, usa la posición relativa de
 * guardado/carga del adaptador como índice d epposición de elemento en la lista.
 * Una vez obtenido el nodo, recupera el nombre del adaptador y lo busca en el log. Para ello,
 * realiza un recorrido línea por línea y busca una coincidendia exacta mediante la función strncmp()
 * que compara dos strings. Una vez reconocido el nombre, busca la información específica de ese adaptador
 * dentro del log. Para no volver a empezar a buscar desde el punto de incio, usamos la funcion ftell()
 * que obtiene la posición de lectura tras la lectura del nombre del adaptador, y la seteamso mediante
 * fseek().
 * La búsqueda devuelve la IPv4, la máscara de subred y la puerta de enlace del adaptador. Para hallar la
 * información, hacemos un recorrido línea a línea desde la línea que contiene el nombre del adaptador y
 * comprobamos si dentro de ese buffer se encuentra la palabra clave identificatoria, para ello, usamos
 * la función strstr() que busca un substring dentro de un string, pasándole el buffer con la línea y una
 * constante con la palabra clave buscada. strstr() devuelve un puntero a char al primer cáracter del
 * substring, así que almacenamos el string en una variable auxialira desde este puntero hasta el carácter
 * de fin de línea.
 * Todo la información se irá imprimiendo por pantalla al encontrarse. Para indicar la finalización de búsqueda,
 * usamos un booleano junto con la función feof().
 *
 * @param lista listaAdptr Linked List de elementos del tipo adptr.
 * @param numAdptr int Número del adaptador que buscamos.
 */
void mostrarInformacionAdptr(const listaAdptr lista, const int numAdptr) {
    char* fInfo, * nombreAdptr, * auxForInfo;
    adptr* actualAdptr;
    int actualPos = 0;
    bool isFound = false;
    FILE* pLog;


    /* Capturamos el nombre del adaptador esocogido */
    /* Buscamos el adaptador */
    actualAdptr = malloc(sizeof(adptr));
    actualAdptr = buscarAdptrPorKey(lista.head, numAdptr - 1);

    /* Asignamos el nombre del adaptador */
    nombreAdptr = malloc(MAX_ADPT_NAME);
    if (nombreAdptr != NULL) {
        strncpy(nombreAdptr, actualAdptr->nombre, MAX_ADPT_NAME);
    }

    /* Abrimos el archivo */
    pLog = openFile(LOG_IPCONFIG, O_LECTURA);

    /* Manejamos errores */
    if (pLog == NULL) {
        puts("Error en el acceso a los datos.");
        return;
    }
    else {
        /* Reservamos la memoria necesaria */
        fInfo = malloc(MAX_FILE_CHARS);
        auxForInfo = malloc(MAX_FILE_CHARS);

        /* Iniciamos el recorrido del archivo de texto línea a línea */
        while (!feof(pLog) && !isFound) {
            if (fgets(fInfo, MAX_FILE_CHARS, pLog)) {

                if (strncmp(fInfo, nombreAdptr, strlen(nombreAdptr)) == 0) {
                    actualPos = ftell(pLog) - 1;
                    fseek(pLog, (actualPos - strnlen(fInfo, MAX_FILE_CHARS)), SEEK_SET);
                    if (fgets(fInfo, MAX_FILE_CHARS, pLog)) {
                        printf("Informacion del %s", fInfo);
                        actualPos = ftell(pLog);
                        fseek(pLog, actualPos, SEEK_SET);
                    }

                    while (!feof(pLog) && !isFound) {
                        if (fgets(fInfo, MAX_FILE_CHARS, pLog)) {

                            if (strstr(fInfo, KEY_WORD_IPV4) != NULL) {
                                auxForInfo = strstr(fInfo, KEY_WORD_IPV4);
                                printf("Direccion %s", auxForInfo);
                                actualPos = ftell(pLog);
                                fseek(pLog, actualPos, SEEK_SET);
                                continue;
                            }
                            else if (strstr(fInfo, KEY_WORD_SUBRED) != NULL) {
                                auxForInfo = strstr(fInfo, KEY_WORD_SUBRED);
                                printf("Mascara de %s", auxForInfo);
                                actualPos = ftell(pLog);
                                fseek(pLog, actualPos, SEEK_SET);
                                continue;
                            }
                            else if (strstr(fInfo, KEY_WORD_PUERTA) != NULL) {
                                auxForInfo = strstr(fInfo, KEY_WORD_PUERTA);
                                printf("%s", auxForInfo);

                                isFound = true;
                            }
                        }
                    }
                }
            }
        }
    }

    /* Liberamos la memoria usada */
   if (fInfo) {
        free(fInfo);
    }

    if (nombreAdptr) {
        free(nombreAdptr);
    }

    if (auxForInfo) {
        auxForInfo = NULL;
        free(auxForInfo);
    }

    if (actualAdptr) {
        free(actualAdptr->nombre);
        actualAdptr->nombre = NULL;
        actualAdptr = NULL;
        free(actualAdptr);
    }

    /* Cerramos el archivo de texto */
    fclose(pLog);
}

/**
* @brief deleteLogAdptrs(): Borra el archivo de log que se ha generado con la lista de adaptadores.
* 
*/
void deleteLogAdptrs() {
    remove(LOG_ADPTRS);
    puts("Se ha eliminado el archivo.");
    puts("-------------------------------------------------");
}

/**
* @brief deleteLogAdptrs(): Borra el archivo de log que se ha generado con la información de los adaptadores.
*
*/
void deleteLogInfoAdptrs() {
    remove(LOG_IPCONFIG);
    puts("Se ha eliminado el archivo.");
    puts("-------------------------------------------------");
}


/** driverMostrarInformacionAdptrs(): Driver secundario que muestra la información sobre el adaptador
 * seleccionado.
 * @brief
 *
 * @param adptrs listaAdptrs Linked List de elementos TAD tipo adptr.
 * @param key int Número del adaptador del que queremos conocer la información.
 */
void driverMostrarInformacionAdptrs(listaAdptr* adptrs, int key) {
    /* Mostramos la información del adaptador escogido por el usuario */
    mostrarInformacionAdptr(*adptrs, key);

    /* Limpiamos la memoria */
    clearListaAdptr(adptrs, adptrs->head);
}

/**
 * @brief driverAdptrs(): Driver principal del módulo.
 *
 * @return listaAdptr Linked List con los adaptadores de red del sistema.
 */
listaAdptr driverAdptrs() {
    /* Creamos la Linked List */
    listaAdptr adptrs;

    /* Creamos el documento log para registrar la lista de adaptadores de red y capturamos
    * los adaptadores del documento registrándolos en la Linked List.
    */
    createFileNombresAdptr(LOG_ADPTRS, O_ESCRITURA);
    adptrs = fillListaAdaptadores();

    /* Imprimimos la lista de adaptadores */
    printListaAdptrs(adptrs.head);

    /* Generamos el documento log con la información de los adaptadores */
    cargarInformacionAdaptadores();

    return adptrs;
}