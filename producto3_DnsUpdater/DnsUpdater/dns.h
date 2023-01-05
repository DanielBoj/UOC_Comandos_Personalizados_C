// Nos aseguramos de linkar una �nica vez la librer�a
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

/// <summary>
/// Genera un archivo log para almacenar la informaci�n sobre los adaptadores de red del equipo.
/// La informaci�n se obtine lanzando desde el c�digo un comando NETSH a trav�s de la funci�n system().
/// El retorno se redirige a un archivo .txt que se guarda en la carpeta ra�z.
/// </summary>
void createAdptsLog();

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
void  createDnsLog(const int index, int* retVal);

/// <summary>
/// A�ade un TAD de tipo "adpater" a una tabla de adaptadores tipo adptsTable.
/// </summary>
/// <param name="table">adptsTable Tabla de adaptadores.</param>
/// <param name="adptr">adpter Adaptador que queremos a�adir a la tabla.</param>
/// <param name="retVal">int Vale 0 si la operaci�n se realiza con �xito, 1 si hay alg�n fallo.</param>
void addAdapter(adptsTable* table, const adapter adptr, int* retVal);

/// <summary>
/// Carga la informaci�n de un archivo log con los datos de los adaptadores a una tabla de TAD de tipo
/// "adapters" con la informaci�n de estos.
/// La funci�n gestiona los posibles errores mediante un valor de retorna. Implementa los siguientes pasos:
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
void adptsLoad(adptsTable* table, int* retVal);

/// <summary>
/// Carga la informaci�n del archivo con las direcciones DNS en una tabla tipo dnsTable.
/// La funci�n gestiona los posibles errores mediante un valor de retorno. Implementa los siguientes pasos:
/// Aperura del documento, lectura l�nea a l�nea de los datos de los adaptadores hasta llegar al fin del codumento 
/// o alcanzar el m�ximo de datos posibles en la tabla, paso a un string auxiliar y creaci�n del TAD tipo "adapter" 
/// para la carga de los datos. A continuaci�n, carga el adaptador en la tabla, cierra el documento y libera la memoria 
/// reservada.
/// FILE *fDns = Puntero al archivo con las direcciones DNS.
/// char *line = String auxiliar para la carga de una l�nea del archivo. 
/// 
/// </summary>
/// <param name="table"></param>
/// <param name="retVal"></param>
void dnsLoad(dnsTable* table, const char* dnsPath, int* retVal);

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
void getAdptr(const char* str, adapter* adptr);

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
int searchAdptrInd(const adptsTable table, const int adptrNum);

/// <summary>
/// A�ade una direcci�n DNS a una tabla de tipo dnsTable.
/// insertInd: �ndice d eposici�n de inserci�n en la tabla.
/// </summary>
/// <param name="table">dnsTable Tabla de direcciones DNS.</param>
/// <param name="str">char* String con la direcci�n DNS.</param>
/// <param name="retVal">int Vale 0 si la operaci�n se realiza con �xito, 1 si hay alg�n fallo.</param>
void addDns(dnsTable* table, const char* str, int* retVal);

/// <summary>
/// A�ade una direcci�n DNS a una tabla de tipo dnsTestTable que contendr� las direcciones DNS y los resultados de los tests de velocidad
/// a estas.
/// </summary>
/// <param name="table">dnsTestTable Tabla que contiene las direcciones DNS y los resultados de los tests de velocidad.</param>
/// <param name="str">char* String con la direcci�n DNS.</param>
/// <param name="retVal">int Devuelve 0 si la operaci�n se realiza con �sxito, o 1 de lo contrario.</param>
void addDnsToTestTable(dnsTestTable* table, const char* str, int* retVal);

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
void getDnsIps(const adptsTable table, const int adptrNum, dnsTable* adptrDnsSrvs, int* adptrInd, int* retVal);

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
void readDnsLog(dnsTable* table, int* retVal);

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
void testDnsServers(const adptsTable adptsTbl, const dnsTable dnsDirs, const dnsTable adprDnsSrvs, const int adptrInd, int* retVal);

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
void findDnsAccessibility(const dnsTable adptrDnsSrvs, FILE* fTempFile, int* retVal);

/// <summary>
/// Combina en una tabla de tipo dnsTestTable las direcciones DNS de una tabla de tipo dnsTable y de un archivo temporal que hemos generado
/// con DNS v�lidas que hemso testado.
/// </summary>
/// <param name="src1">dnsTable Primera tabla de origen.</param>
/// <param name="src2">dnsTable Segunda tabla de origen.</param>
/// <param name="retVal">Retorn 0 si la operaci�n se ha realizado con �xito o 1 si ha fallado.</param>
void mergeToDnsTestTable(const dnsTable src, FILE* fTempFile, dnsTestTable* testTable, int* retVal);

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
void speedTest(dnsTestTable* testTable, int* retVal);


void changeDns(const adptsTable adptsTbl, const dnsTestTable testTable, const dnsTable adptsDns, const int adptsInd, int* retVal);

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
void qSortDnsTestTable(dnsTestTable* table, int low, int high);

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
int partitionDnsTestTable(dnsTestTable* table, int low, int high);

/// <summary>
/// Intercambia dos elementos TAD tipo dnsTest.
/// temp = Elemento TAD tipo dnsTest que funciona como variable temporal para poder realizar el intercambio.
/// </summary>
/// <param name="src1">dnsTest Origen 1.</param>
/// <param name="src2">dnsTest Origen2.</param>
void swapDnsTest(dnsTest* src1, dnsTest* src2);

/// <summary>
/// Copia la informaci�n de un elemento TAD dnsTest de origen a otro de destino, obtenidos ambos por par�metros.
/// </summary>
/// <param name="src">dnsTest Elemento origen.</param>
/// <param name="dest">dnsTest Elemento destino.</param>
void cpyDnsTest(dnsTest src, dnsTest* dest);

/// <summary>
/// Copia la informaci�n de un TAD tipo "adpatader" a otro TAD tipo "adaptador".
/// </summary>
/// <param name="dst">adapter Tad tipo "adapter" de destino.</param>
/// <param name="src">TAD tipo "adapter" de origen.</param>
void adptrCopy(adapter* dst, const adapter src);

/// <summary>
/// Imprime toda la informaci�n de los adaptores almacenados en una tabla adptsTable que almacena estructuras TAD del tipo "adapter".
/// </summary>
/// <param name="table">adptsTable Tabla adptsTable que almacena estructuras TAD del tipo "adapter"</param>
void printAdptsList(const adptsTable table);

/// <summary>
/// Impreme las direcciones DNS de una tabla tipo dnsTable.
/// </summary>
/// <param name="table">dnsTable Tabla que contiene un array con las direcciones DNS.</param>
void printDnsTable(const dnsTable table);

/// <summary>
/// Muestra por pantalla una lista con el nombre de todos los adaptadores de red del equipo que est�n conectados.
/// adptsNum: N�mero de adaptadores con conexi�n.
/// </summary>
/// <param name="table">adptsTable Tabla que contiene estructuras TAD de tipo adapter con la informaci�n de los adaptadores de red.</param>
/// <returns>int numAdpts N�mero de adaptadores con conexi�n.</returns>
int printAdptsNames(const adptsTable table);

/// <summary>
/// Imprime por pantalla la informaci�n del servidor DNS m�s r�pido.
/// </summary>
/// <param name="testTable">dnsTestTable Tabla de elementos TAD tipo dnsTest.</param>
void printBestDns(const dnsTestTable testTable);

/// <summary>
/// Limpia completamente los datos de una tabla de DNS. Recorre todas las posiciones de la tabla, librea la memoria asignada y iguala
/// los punteros a NULL. Por �ltimo, resetea el n�mero de elemento a 0.
/// </summary>
/// <param name="table">dnsTable Tabla de direcciones DNS.</param>
void clearDnsTable(dnsTable* table);

/// <summary>
/// Limpia completamente los datos de una tabla de adaptadores. Recorre todas las posiciones de la tabla, librea la memoria asignada e iguala
/// los punteros a NULL, resetea los campos a los valores por defecto. Por �ltimo, resetea el n�mero de elemento a 0.
/// </summary>
/// <param name="adptsTbl">adptsTable Tabla de elementos TAD de tipo adapter.</param>
void clearAdptsTable(adptsTable* adptsTbl);

/// <summary>
/// Limpia completamente los datos de una tabla de elementos TAD tipo dnsTest. Recorre todas las posiciones de la tabla, librea la memoria asignada 
/// e iguala los punteros a NULL y resetea los campos a sus valores por defecto. Por �ltimo, resetea el n�mero de elemento a 0.
/// </summary>
/// <param name="testTable">dnsTestTable Tabla de elementos TAD tipo dnsTest.</param>
void clearDnsTestTable(dnsTestTable* testTable);

/// <summary>
/// Liberamos una estructura TAD tipo dnsTest y reseteamos sus campos.
/// </summary>
/// <param name="test">dnsTest Estructura TAD tipo dnsTest que contiene los resultados de los test de velocidad a una direcci�n DNS.</param>
void freeDnsTest(dnsTest* test);

/// <summary>
/// Liberamos la memoria usada en una estructura TAD tipo adapter y reseteamos sus campos.
/// </summary>
/// <param name="adptr">adptr Estructura TAD que contiene los datos de un adaptador de red.</param>
void freeAdapter(adapter* adptr);

/// <summary>
/// Determina si la tabla espec�fica est� llena.
/// </summary>
/// <param name="table">adptsTable Tabla de TAD tipo "adaptador".</param>
/// <returns>
///   <c>true</c>, si la tabla est� llena; Si queda espacio, <c>false</c>.
/// </returns>
bool isFullAdpts(const adptsTable table);

/// <summary>
/// Determina si la tabla espec�fica est� llena.
/// </summary>
/// <param name="table">adptsTable Tabla de TAD tipo "adaptador".</param>
/// <returns>
///   <c>true</c>, si la tabla est� llena; Si queda espacio, <c>false</c>.
/// </returns>
bool isFullDns(const dnsTable table);

/// <summary>
/// Determina si la tabla espec�fica est� llena.
/// </summary>
/// <param name="table">adptsTable Tabla de TAD tipo "adaptador".</param>
/// <returns>
///   <c>true</c>, si la tabla est� llena; Si queda espacio, <c>false</c>.
/// </returns>
bool isFullDnsTest(const dnsTestTable table);