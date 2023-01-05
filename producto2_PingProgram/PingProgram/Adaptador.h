#pragma once // Nos aseguramos de que la librer�a solo pueda incluirse una vez en la compilaci�n.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"

#ifndef MAX_ADPTR_NAME
#define MAX_ADPTR_NAME 255
#endif // Longitud m�xima del nombre d eun adaptador.
#ifndef MAX_COMMAND_DESC
#define MAX_COMMAND_DESC 100
#endif // Longitud m�xima del comando para system().
#ifndef MAX_FILE_CHARS
#define MAX_FILE_CHARS 255
#endif // Longitud m�xima del buffer de lectura de archivo externo.
#ifndef O_ESCRITURA
#define O_ESCRITURA "w+"
#endif // Modo de apertura de un archivo creaci�n y escritura.
#ifndef O_LECTURA
#define O_LECTURA "r"
#endif // Modo de apertura de un archivo solo lectura.
#ifndef LOG_ADPTRS
#define LOG_ADPTRS "./log_adaptadores.txt" // Nombre del archivo que contendr� la lista de adaptadores.
#endif
#ifndef LOG_IPCONFIG
#define LOG_IPCONFIG "./log_ipconfig.txt" // Nombre del archivo que contendr� las ipconfig de los adaptadores.
#endif
#ifndef KEY_WORD_IPV4
#define KEY_WORD_IPV4 "IPv4" // Palabra para identificar los datos referentes a la IP.
#endif
#ifndef KEY_WORD_SUBRED
#define KEY_WORD_SUBRED "de subred" // Palabra para identificar los datos referentes a la M�scara de subred.
#endif
#ifndef KEY_WORD_PUERTA
#define KEY_WORD_PUERTA "Puerta" // Palabra para identificar los datos referentes a la Puerta de Enlace.
#endif

/**
 * @brief createArchivoLog(): genera un archivo txt externo para escribir los
 * nombres de los adaptadores de red. Realiza la creaci�n del puntero a FILE
 * mediante el m�todo openfile() y cierra el archivo para que podamos usar
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
 * un archivo log que genera autom�ticamente. Si ya existe, lo reescribe.
 *
 * @param nombreArchivo String Ruta relativa o absoluta del archivo.
 * @param modo String Modo de apertira del archivo, para nuestra intenci�n
 *  usaremos un modo de escritura con el flag + que est� creado como la
 * constante O_ESCRITURA.
 * @return int
 */
void createFileNombresAdptr(char* nombreArchivo, char* modo);

/**
 * @brief fillListAdaptadores(): Asigana los diferentes nombres de los adaptadores registrados
 * en el archivo log a nodos de una LinkedList.
 * Como los nombre se graban con el car�cter final ':', buscamos la posici�n de ese char mediante
 * la funci�n strcspn() y el tring car�cter a car�cter hasta justo antes de llegar a ':' en un
 * String limpio que a�adiremos a la lista mediante la funci�n apperndAdptr().
 *
 * @return listaAdptr Listed Link de TAD tipo adptr.
 */
listaAdptr fillListaAdaptadores();

/**
 * @brief cargarInformacionAdaptadores(): Genera un archivo de texto log en el directorio raiz
 * donde se guarda la informaci�n de todos los adpatadores de red.
 * Para hacerlo, se crea un comando de CMD ipconfig que se ejecuta mediante system().
 *
 */
void cargarInformacionAdaptadores();

/**
 * @brief mostrarInformacionAdptr(): Muestra la informaci�n de uno de los adaptadores accediendo
 * al documento log que se ha generado previamente.
 * Es el comando m�s complejo de este m�dulo; el comando busca el nombre del adaptador en la
 * Linked List de adaptadores mediante buscarAdptrPorKey(), para ello, usa la posici�n relativa de
 * guardado/carga del adaptador como �ndice d epposici�n de elemento en la lista.
 * Una vez obtenido el nodo, recupera el nombre del adaptador y lo busca en el log. Para ello,
 * realiza un recorrido l�nea por l�nea y busca una coincidendia exacta mediante la funci�n strncmp()
 * que compara dos strings. Una vez reconocido el nombre, busca la informaci�n espec�fica de ese adaptador
 * dentro del log. Para no volver a empezar a buscar desde el punto de incio, usamos la funcion ftell()
 * que obtiene la posici�n de lectura tras la lectura del nombre del adaptador, y la seteamso mediante
 * fseek().
 * La b�squeda devuelve la IPv4, la m�scara de subred y la puerta de enlace del adaptador. Para hallar la
 * informaci�n, hacemos un recorrido l�nea a l�nea desde la l�nea que contiene el nombre del adaptador y
 * comprobamos si dentro de ese buffer se encuentra la palabra clave identificatoria, para ello, usamos
 * la funci�n strstr() que busca un substring dentro de un string, pas�ndole el buffer con la l�nea y una
 * constante con la palabra clave buscada. strstr() devuelve un puntero a char al primer c�racter del
 * substring, as� que almacenamos el string en una variable auxialira desde este puntero hasta el car�cter
 * de fin de l�nea.
 * Todo la informaci�n se ir� imprimiendo por pantalla al encontrarse. Para indicar la finalizaci�n de b�squeda,
 * usamos un booleano junto con la funci�n feof().
 * Al final de la b�squeda liberamos la memoria usada.
 *
 * @param lista listaAdptr Linked List de elementos del tipo adptr.
 * @param numAdptr int N�mero del adaptador que buscamos.
 */
void mostrarInformacionAdptr(const listaAdptr lista, const int numAdptr);

/**
* @brief deleteLogAdptrs(): Borra el archivo de log que se ha generado con la lista de adaptadores.
*
*/
void deleteLogAdptrs();

/**
* @brief deleteLogAdptrs(): Borra el archivo de log que se ha generado con la informaci�n de los adaptadores.
*
*/
void deleteLogInfoAdptrs();

/**
 * @brief driverAdptrs(): Driver principal del m�dulo.
 *
 * @return listaAdptr Linked List con los adaptadores de red del sistema.
 */
listaAdptr driverAdptrs();

/** driverMostrarInformacionAdptrs(): Driver secundario que muestra la informaci�n sobre el adaptador
 * seleccionado.
 * @brief
 *
 * @param adptrs listaAdptrs Linked List de elementos TAD tipo adptr.
 * @param key int N�mero del adaptador del que queremos conocer la informaci�n.
 */
void driverMostrarInformacionAdptrs(listaAdptr* adptrs, int key);
