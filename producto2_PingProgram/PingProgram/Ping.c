#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Ping.h"

/**
 * @brief initTablaIpAdrr(): Inicializa un TAD de tabla de direcciones IP que recibe por parámetro.
 *
 * @param tabla Estructura TAD de elementos tipo ipAddr.
 */
void initTablaIpAdrr(tablaIpAddr* tabla) {
    tabla->numElem = 0;
    for (int i = 0; i < IP_TBL_SIZE; i++) {
        tabla->pingResults[i] = -1;
    }
}

/**
 * @brief getDatos(): Función que le solicita al usuario la ruta absoluta de acceso al archivo de texto que contiene las direcciones IP.
 *
 * @param rutaAbasoluta String Variable in/out a la que asignamos la ruta abasoluta.
 */
void getDatos(char rutaAbasoluta[]) {

    char askValue[MAX_CHAR], nombreFichero[MAX_CHAR], rutaAcceso[MAX_CHAR];

    puts("Por favor, indique la ruta de acceso al archivo, debe usar / para separar los directorios, no use \\.\nPor ejemplo, C:/dev/");
    rewind(stdin);
    fgets(askValue, MAX_CHAR, stdin);
    strtok(askValue, "\n");
    if (askValue) {
        strncpy(rutaAcceso, askValue, MAX_CHAR);
    }

    puts("Por favor, indique el nombre del archivo con su tipo.\nPor ejemplo, mifichero.txt");
    rewind(stdin);
    fgets(askValue, MAX_CHAR, stdin);
    strtok(askValue, "\n");
    if (askValue) {
        strncpy(nombreFichero, askValue, MAX_CHAR);
    }

    if (rutaAcceso != NULL && nombreFichero != NULL) {
        strncpy(rutaAbasoluta, strcat(rutaAcceso, nombreFichero), MAX_CHAR);
    }
}

/**
 * @brief getDatosIp(): Función que le solicita al usuario la ruta absoluta de acceso al archivo de texto que comtiene las direcciones IP
 * y la nueva IP.
 *
 * @param ruta String Variable in/out a la que asignamos la ruta abasoluta.
 * @param ip String Variable in/out a la que asignamos la nueva IP.
 */
void getDatosIp(char ruta[], char ip[]) {
    
    char askValue[MAX_CHAR], ipValue[MAX_IPADRR_LNG + 1], nombreFichero[MAX_CHAR], rutaAcceso[MAX_CHAR];

    puts("Por favor, indique la ruta de acceso al archivo, debe usar / para separar los directorios, no use \\.\nPor ejemplo, C:/dev/");
    rewind(stdin);
    fgets(askValue, MAX_CHAR, stdin);
    strtok(askValue, "\n");
    if (askValue) {
        strncpy(rutaAcceso, askValue, MAX_CHAR);
    }

    puts("Por favor, indique el nombre del archivo con su tipo.\nPor ejemplo, mifichero.txt");
    rewind(stdin);
    fgets(askValue, MAX_CHAR, stdin);
    strtok(askValue, "\n");
    if (askValue) {
        strncpy(nombreFichero, askValue, MAX_CHAR);
    }

    puts("Por favor, indique la nueva direccion IP.\nPor ejemplo, 192.168.1.2\n");
    rewind(stdin);
    fgets(ipValue, MAX_IPADRR_LNG + 1, stdin);
    strtok(ipValue, "\n");
    if (ipValue) {
        strncpy(ip, ipValue, MAX_IPADRR_LNG + 1);
    }

    if (rutaAcceso != NULL && nombreFichero != NULL) {
        strncpy(ruta, strcat(rutaAcceso, nombreFichero), MAX_CHAR);
    }
}

/**
 * @brief openFile(): Función que abre un archivo externo y retorna un puntero al archivo.
 *
 * @param nombre String Ruta absoluta al archivo.
 * @param modo String Modo de apertura.
 * @return FILE* Puntero a FILE al archivo.
 */
FILE* openFile1(const char* nombre, const char* modo) {
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
 * @brief listIp(): Función que consulta el archivo con las direcciones IP, las muestra por pantalla, las almacena en una structura TDA tablaIpAddr y retorna la tabla.
 *
 * @param rutaAbsoluta String Ruta abasoluta del archivo.
 * @param modo String Modo de apertura
 * @return tablaIpAddr Structura TDA que almacena las direcciones IP.
 */
tablaIpAddr listIp(char rutaAbsoluta[], char modo[]) {
    tablaIpAddr direcciones;
    char ipDir[MAX_IPADRR_LNG + 1];
    int ind;

    /* Inicialización de la tabla y acceso al archivo. */
    initTablaIpAdrr(&direcciones);
    FILE* pFichero = openFile1(rutaAbsoluta, modo);

    /* Mensaje al usuario */
    puts("");
    puts("Lista de IP:");
    puts("-------------------------------------------------");

    ind = 0;
    while (!feof(pFichero)) {
        if (fgets(ipDir, MAX_IPADRR_LNG + 1, pFichero)) {
            printf("#%d: %s", ++ind, ipDir);
            direcciones.addresses[ind - 1] = malloc(sizeof(ipDir));
            strncpy(direcciones.addresses[ind - 1], ipDir, MAX_IPADRR_LNG + 1);

            direcciones.numElem++;
        }
    }
    puts("");

    /**
     * Cierre del puntero mediante el método fclose(). Por parámetro pasamos el puntero al archivo.
     */
    fclose(pFichero);

    return direcciones;
}

/**
 * @brief pingIp(): Usa la función system() para ejecutar un comando de PING a la dirección IP que recibe por parámetro. Para ello,
 * usa funciiones de concatenación de strings para agragar la IP a la definición de comando pòr defecto. Se realiza el envío de un
 * paquete de datos y evitamos que se muestre la ejecución por terminal enviándola a NUL (equivalente de /dev/nul de Linux). Evitamos
 * también concatenar saltos de línea limpiando el carávte \\n si se encuentra en el buffer. Si el ping se realiza con éxito, el
 * comando retorna 0, si no, 1.
 *
 *
 * @param ipAddr String Dirección IP a la que realizar el ping.
 * @return int Respuesta de retorno del comando.
 */
int pingIp(const char* ipAddr)
{
    char* ipCommand = malloc(MAX_CHAR);
    int resValue;

    if (ipAddr != NULL && ipCommand != NULL) {
        strncpy(ipCommand, "ping -n 1 -w 1000 ", MAX_CHAR);
 
        strcat(ipCommand, ipAddr);
        if (ipCommand[strnlen(ipCommand, MAX_CHAR) - 1] == '\n') {
            ipCommand[strnlen(ipCommand, MAX_CHAR) - 1] = '\0';
        }
        strcat(ipCommand, " > NUL");

        resValue = system(ipCommand);

        free(ipCommand);

        return resValue;
    }

    return -1;
    
}

/**
 * @brief getPingResponse: Obtiene el valor de respuesta de un comando de
 * PING lanzado a cada una de las direcciones IP almacenadas en una tabla
 * de direcciones tipo tablaipAddr.
 * Es una función recursiva que recorre la tabla hacia adelante.
 *
 * @param tabla tablaIpAddr Estructura para contener las direcciones IP y
 *  los resultados del response.
 * @param end int Indice de posición final.
 * @param ind int Índice de posición del primer elemento.
 */
int getPingResponse(tablaIpAddr* tabla, const int end, const int ind)
{
    int resValue;

    if (ind == end) {
        resValue = pingIp(tabla->addresses[ind]);
        if (resValue == 1) {
            tabla->pingResults[ind] = 1;
        }
        else {
            tabla->pingResults[ind] = 0;
        }
        return 0;
    }

    resValue = pingIp(tabla->addresses[ind]);
    if (resValue == 1) {
        tabla->pingResults[ind] = 1;
    }
    else {
        tabla->pingResults[ind] = 0;
    }

    return getPingResponse(tabla, end, ind + 1);
}

/**
 * @brief printPingResult(): Imprime por pantalla los resultados del ping a las ips
 *
 * @param tabla tablaIpAddr Estructura TDA que contiene las direcciones IP y las respuestas de estas
 * al print.
 */
void printPingResult(const tablaIpAddr tabla) {
    char res[MAX_CHAR];
    char* ipAddr = NULL;

    puts("");
    puts("Resultados de los echo:");
    puts("-------------------------------------------------");

    for (int i = 0; i < tabla.numElem; i++) {
        ipAddr =(char *) malloc(MAX_IPADRR_LNG + 1);

        if (ipAddr != NULL) {

            if (strncpy(ipAddr, tabla.addresses[i], MAX_IPADRR_LNG + 1)) {
                if (ipAddr[strnlen(ipAddr, MAX_IPADRR_LNG + 1) - 1] == '\n') {
                    ipAddr[strnlen(ipAddr, MAX_IPADRR_LNG + 1) - 1] = '\0';
                }
            }
                

            if (tabla.pingResults[i] != -1) {
                switch (tabla.pingResults[i]) {
                    case 0: {
                        strncpy(res, "ha respondido con exito.", MAX_CHAR);
                        break;
                    }
                    case 1: {
                        strncpy(res, "no ha devuleto respuesta.", MAX_CHAR);
                        break;
                    }
                }
                printf("El ping a la IP %s %s\n", ipAddr, res);
            }
            else {
                puts("Error en la lectura de los datos");
                free(ipAddr);
                return;
            }
        }

        free(ipAddr);
    }
}

/**
 * @brief clearTable: Libera la memoria y elimina los registros de una tabla.
 * Es una función recursiva.
 *
 * @param table tablaIpAdrr que queremos borrar.
 * @param ind Indice de posición del último elemento.
 */
int clearTable(tablaIpAddr* table, const int ind)
{
    /* Caso base: Liberamos la memoría, asignamos valor por defecto y
     * reiniciamos el contador de elementos */
    if (ind == 0) {
        free(table->addresses[ind]);
        table->pingResults[ind] = -1;
        table->numElem = 0;

        return 1;
    }

    /* Liberamos memoria de la dirección y asignamos valor por defecto al
     * response */
    free(table->addresses[ind]);
    table->pingResults[ind] = -1;

    return clearTable(table, ind - 1);
}

/**
 * @brief addIp(): Añade una nueva dirección IP al archivo que recibe por parámetro, la dirección
 * también se recibe por parámetro.
 *
 * @param ruta String ruta del archivo de ips.
 * @param nombre String nombre dle archivo de ips.
 * @param ipDir String nueva dirección ip.
 */
void addIp(const char ruta[], const char ipDir[]) {
    FILE* pLogPing;

    /* El manejo de errores se realiza desde openFIle() */
    pLogPing = openFile(ruta, O_APPEND);

    /* Añadimos la nueva IP */
    fprintf(pLogPing, "%s" "%c", ipDir, '\n');
    
    fclose(pLogPing);
}

/**
 * @brief pingDriver(): Es la función driver para manejar el módulo.
 *
 */
int ipPingDriver() { 
    tablaIpAddr ipAddrs;
    char rutaAbsoluta[MAX_CHAR];
    int resVal = -1;

    getDatos(rutaAbsoluta);
    ipAddrs = listIp(rutaAbsoluta, O_LECTURA);

    resVal = getPingResponse(&ipAddrs, ipAddrs.numElem - 1, FIRST_ELEM_IN_ARRAY);
    if (resVal == -1) {
        puts("Ha habido un error en el envio de los ping.");
        return -1;
    }

    printPingResult(ipAddrs);

    puts("-------------------------------------------------");
    puts("");

    clearTable(&ipAddrs, ipAddrs.numElem - 1);

    return 0;
}

/**
 * @brief pingDriver(): Es la función driver para manejar la adición de IPs.
 *
 */
void addIpDriver() {
    char ip[MAX_IPADRR_LNG + 1], ruta[MAX_CHAR];

    getDatosIp(ruta, ip);
    addIp(ruta, ip);
}