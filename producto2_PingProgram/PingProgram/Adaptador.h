#pragma once // Nos aseguramos de que la librería solo pueda incluirse una vez en la compilación.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"

#ifndef MAX_ADPTR_NAME
#define MAX_ADPTR_NAME 255
#endif // Longitud máxima del nombre d eun adaptador.
#ifndef MAX_COMMAND_DESC
#define MAX_COMMAND_DESC 100
#endif // Longitud máxima del comando para system().
#ifndef MAX_FILE_CHARS
#define MAX_FILE_CHARS 255
#endif // Longitud máxima del buffer de lectura de archivo externo.
#ifndef O_ESCRITURA
#define O_ESCRITURA "w+"
#endif // Modo de apertura de un archivo creación y escritura.
#ifndef O_LECTURA
#define O_LECTURA "r"
#endif // Modo de apertura de un archivo solo lectura.
#ifndef LOG_ADPTRS
#define LOG_ADPTRS "./log_adaptadores.txt" // Nombre del archivo que contendrá la lista de adaptadores.
#endif
#ifndef LOG_IPCONFIG
#define LOG_IPCONFIG "./log_ipconfig.txt" // Nombre del archivo que contendrá las ipconfig de los adaptadores.
#endif
#ifndef KEY_WORD_IPV4
#define KEY_WORD_IPV4 "IPv4" // Palabra para identificar los datos referentes a la IP.
#endif
#ifndef KEY_WORD_SUBRED
#define KEY_WORD_SUBRED "de subred" // Palabra para identificar los datos referentes a la Máscara de subred.
#endif
#ifndef KEY_WORD_PUERTA
#define KEY_WORD_PUERTA "Puerta" // Palabra para identificar los datos referentes a la Puerta de Enlace.
#endif

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
FILE* createArchivoLog(char* nombre, char* modo);

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
void createFileNombresAdptr(char* nombreArchivo, char* modo);

/**
 * @brief fillListAdaptadores(): Asigana los diferentes nombres de los adaptadores registrados
 * en el archivo log a nodos de una LinkedList.
 * Como los nombre se graban con el carácter final ':', buscamos la posición de ese char mediante
 * la función strcspn() y el tring carácter a carácter hasta justo antes de llegar a ':' en un
 * String limpio que añadiremos a la lista mediante la función apperndAdptr().
 *
 * @return listaAdptr Listed Link de TAD tipo adptr.
 */
listaAdptr fillListaAdaptadores();

/**
 * @brief cargarInformacionAdaptadores(): Genera un archivo de texto log en el directorio raiz
 * donde se guarda la información de todos los adpatadores de red.
 * Para hacerlo, se crea un comando de CMD ipconfig que se ejecuta mediante system().
 *
 */
void cargarInformacionAdaptadores();

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
 * Al final de la búsqueda liberamos la memoria usada.
 *
 * @param lista listaAdptr Linked List de elementos del tipo adptr.
 * @param numAdptr int Número del adaptador que buscamos.
 */
void mostrarInformacionAdptr(const listaAdptr lista, const int numAdptr);

/**
* @brief deleteLogAdptrs(): Borra el archivo de log que se ha generado con la lista de adaptadores.
*
*/
void deleteLogAdptrs();

/**
* @brief deleteLogAdptrs(): Borra el archivo de log que se ha generado con la información de los adaptadores.
*
*/
void deleteLogInfoAdptrs();

/**
 * @brief driverAdptrs(): Driver principal del módulo.
 *
 * @return listaAdptr Linked List con los adaptadores de red del sistema.
 */
listaAdptr driverAdptrs();

/** driverMostrarInformacionAdptrs(): Driver secundario que muestra la información sobre el adaptador
 * seleccionado.
 * @brief
 *
 * @param adptrs listaAdptrs Linked List de elementos TAD tipo adptr.
 * @param key int Número del adaptador del que queremos conocer la información.
 */
void driverMostrarInformacionAdptrs(listaAdptr* adptrs, int key);
