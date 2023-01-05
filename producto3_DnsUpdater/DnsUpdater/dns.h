// Nos aseguramos de linkar una única vez la librería
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

/// <summary>
/// Genera un archivo log para almacenar la información sobre los adaptadores de red del equipo.
/// La información se obtine lanzando desde el código un comando NETSH a través de la función system().
/// El retorno se redirige a un archivo .txt que se guarda en la carpeta raíz.
/// </summary>
void createAdptsLog();

/// <summary>
/// Genera un archivo de log con las direcciones DNS de un adaptador del equipo que seleccionamos por el número de índice
/// que se recibe como parámetro de entrada. Se realiza la selección por índice para evitar problemas surgidos por la
/// codificación de caracteres especiales. El log se generá mediante la ejecución de un comando de CMD a través de la 
/// función system(). EL instrucción ejecuta un comando NETSH que obtiene la información de los servidores DNS del adaptador.
/// El retorno se dirije a un archivo .txt que se guarda en la carpeta raíz.
/// command: String donde generaremos el comando que ejecutar.
/// </summary>
/// <param name="index">int Índice del adaptador de red.</param>
/// <param name="retVal">int Devuelve 0 si la operación se ejecuta correctamente, o 1 de lo contrario.</param>
void  createDnsLog(const int index, int* retVal);

/// <summary>
/// Añade un TAD de tipo "adpater" a una tabla de adaptadores tipo adptsTable.
/// </summary>
/// <param name="table">adptsTable Tabla de adaptadores.</param>
/// <param name="adptr">adpter Adaptador que queremos añadir a la tabla.</param>
/// <param name="retVal">int Vale 0 si la operación se realiza con éxito, 1 si hay algún fallo.</param>
void addAdapter(adptsTable* table, const adapter adptr, int* retVal);

/// <summary>
/// Carga la información de un archivo log con los datos de los adaptadores a una tabla de TAD de tipo
/// "adapters" con la información de estos.
/// La función gestiona los posibles errores mediante un valor de retorna. Implementa los siguientes pasos:
/// Aperura del documento, lectura y descarte de las primeras líneas sin información requerida, lectura línea a
/// línea de los datos de los adaptadores hasta llegar al fin del codumento o alcanzar el máximo de datos posibles
/// en la tabla, paso a un string auxiliar y creación del TAD tipo "adapter" para la carga de los datos. 
/// A continuación, carga el adaptador en la tabla, cierra el documento y libera la memoria reservada.
/// FILE *fAdapterInfo = Puntero al archivo log con la información de los adaptadores.
/// char *line = String auxiliar para la carga de una línea del archivo.
/// adapter newAdapter = TAD de tipo "adapter" auxiliar para la carga de los datos de un adaptador.
/// </summary>
/// <param name="table">adptsTable Tabla de TAD tipo "adpater".</param>
/// <param name="retVal">int Devuelve 0 si la operaicón se realiza con éxito, o 1 si falla.</param>
void adptsLoad(adptsTable* table, int* retVal);

/// <summary>
/// Carga la información del archivo con las direcciones DNS en una tabla tipo dnsTable.
/// La función gestiona los posibles errores mediante un valor de retorno. Implementa los siguientes pasos:
/// Aperura del documento, lectura línea a línea de los datos de los adaptadores hasta llegar al fin del codumento 
/// o alcanzar el máximo de datos posibles en la tabla, paso a un string auxiliar y creación del TAD tipo "adapter" 
/// para la carga de los datos. A continuación, carga el adaptador en la tabla, cierra el documento y libera la memoria 
/// reservada.
/// FILE *fDns = Puntero al archivo con las direcciones DNS.
/// char *line = String auxiliar para la carga de una línea del archivo. 
/// 
/// </summary>
/// <param name="table"></param>
/// <param name="retVal"></param>
void dnsLoad(dnsTable* table, const char* dnsPath, int* retVal);

/// <summary>
/// Creamos un elemento de tipo TAD adpater a través de una línea de información que hemos obtenido del documento
/// log con la información de los adaptadores de red del equipo.
/// auxState: Guarda el resultado del estado del adpatador para asignarlo más tarde a la estructura com un valor
/// de tipo booleano.
/// Usamos la función sscanf() que permite obtener una serie de valores aislados por el carácter ' ' de un string
/// y los vamos asignando a los campor de la estructura TAD adaptador. Cada valor es un campo de información del
/// adaptador que podremos usar más tarde.
/// </summary>
/// <param name="str">char* String desde el que capturaremos la información del adaptador. in</param>
/// <param name="adptr"> tAdaptador TAD tipo adpatador para volvar la información del adaptador. in/out</param>
void getAdptr(const char* str, adapter* adptr);

/// <summary>
/// Toma una tabla de TAD tipo adapter que contiene la información de los adaptadores de red y un entero que representa la selección que ha
/// realizado un usuario sobre una lista de adaptadores de red activos y devuelve la posición en la tabla del adaptador escogido.
/// La tabla contiene tanto adaptadores activos como inactivos, el usuario ha escogido el número de adaptador en una lista que contiene
/// solo adaptadores activos y que empieza por 1, no por 0. La función busca los adaptadores activos, les adjudica una posición relativa
/// en un contador de adaptadores activos y lo compara con la selección del usuario a la que restamos 1, para ajustarlo a la numeración de
/// posiciones de un array en C.
/// </summary>
/// <param name="table">adptsTable Tabla de TAD tipo adapter que contiene la información de los adaptadores de red del equipo.</param>
/// <param name="adptrNum">int Posición en la lista de adaptadores activos que ha escogido el usuario.</param>
/// <returns>int Posicion del adaptador seleccionado en la tabla de TAD tipo adapter.</returns>
int searchAdptrInd(const adptsTable table, const int adptrNum);

/// <summary>
/// Añade una dirección DNS a una tabla de tipo dnsTable.
/// insertInd: Índice d eposición de inserción en la tabla.
/// </summary>
/// <param name="table">dnsTable Tabla de direcciones DNS.</param>
/// <param name="str">char* String con la dirección DNS.</param>
/// <param name="retVal">int Vale 0 si la operación se realiza con éxito, 1 si hay algún fallo.</param>
void addDns(dnsTable* table, const char* str, int* retVal);

/// <summary>
/// Añade una dirección DNS a una tabla de tipo dnsTestTable que contendrá las direcciones DNS y los resultados de los tests de velocidad
/// a estas.
/// </summary>
/// <param name="table">dnsTestTable Tabla que contiene las direcciones DNS y los resultados de los tests de velocidad.</param>
/// <param name="str">char* String con la dirección DNS.</param>
/// <param name="retVal">int Devuelve 0 si la operación se realiza con ésxito, o 1 de lo contrario.</param>
void addDnsToTestTable(dnsTestTable* table, const char* str, int* retVal);

/// <summary>
/// Obtiene las direcciones DNS asociadas a un adaptador de red que ha escogido un usuario. Crea los elementos temporales necesarios
/// para mostrar la información por pantalla. La función delega en una serie de funciones auxialiares que se han creado en el programa:
/// initAdptr() e initDnsTable(): Inicializan, respectivamente, un TAD tipo adapter y una tabla de direcciones DNS.
/// searchAdapterInd(): Busca el índice de posición real del adaptador de red escogio por el usuario en la tabla a través de la opción
/// relativa que ha escogio el usuario.
/// adptrCopy(): Copia los datos de un TAD tipo adapter a otro TAD del mismo tipo.
/// createDnsLog(): Crea un archivo log con las direcciones DNS de un adaptador de red.
/// readDnsLog(): Lee un archivo de log que contiene las direcciones DNS de un adaptador de red, obtiene las direcciones y las carga
/// en una tabla de direcciones DNS.
/// printDnsTable(): Muestra las direcciones por pantalla al usuario.
/// freeAdapter(): Libera los recursos usados.
/// -------
/// actualAdptr: Estructura TAD tipo "adapter" que contiene la información del adaptador de red.
/// dnsRes: Tabla de direcciones DNS temporal que contendrá las direcciones DNS del adaptador de red seleccionado por el usuario.
/// adptrInd: Variable a la que asignaremos el índice del adaptador escogido por el usuario para lanzar el comando NETSH. Usamos un 
/// índice parar evitar problemas con el uso de caracteres especiales en los nombres de los adaptadores de red del equipo.
/// posInd: Índice de posición del adaptador en la tabla.
/// </summary>
/// <param name="table">adptsTable Tabla de TAD tipo adapter con la información de los adaptadores de res del equipo.</param>
/// <param name="adptrNum">Identificador relativo del adaptador que el usuario a escogido en una lista de adaptadores activos.</param>
/// <param name="adptrInd">El índice de posición del adaptador en la tabla de TAD tipo adapter.</param>
/// <param name="retVal">Retorno 0 se la operación se ejecuta con écito, 1 si falla.</param>
void getDnsIps(const adptsTable table, const int adptrNum, dnsTable* adptrDnsSrvs, int* adptrInd, int* retVal);

/// <summary>
/// Obtine los datos de las direcciones DNS que hemos registrado en un archivo log a través de la ejecución de un comando NETSH.
/// La función gestiona la apertura del archivo y el control de errores. Descarta las primeras líneas sin información útil y
/// obtiene las direcciones DNS de las siguientes líneas hasta la lectura de la palabra de control que indica el fin del registro
/// de las direcciones DNS en el log. En el caso de la primera dirección, descarta también todos los datos hasta llegar al número
/// de dirección DNS.
/// Para implementar las operaciones de búsqueda y descarte de información usamos funciones de la librería string.h():
/// strstr() busca un substring dentro de un string principal, y si lo encuentra, devuelve un puntero a char a partir del string localizado. En este caso, no nos interesa el valor
/// devuelto, solo comprobamos que no sea NULL para saber que hemos llegado a la línea a descartar y finalizar el proceso de obtención de datos.
/// strcspn() devuelve la longitud en número de carácteres hasta hallar la primera incidencia de un caracter que pasamos como argumento. Usamos esta longitud para saber en qué posición
/// de line debemos empezar a copiar carácteres. Para obtener el substring que nos interesa, realizamos una copia caracter a caracter de line a partir del punto que nos interesa e 
/// ignorando los caracteres de espacio y de tabulación ' ' y '\t'.
/// strtok() divide un String en substrings usando como delimitador los caracteres que indiquemos en un segundo string que recive como parámetro. Usamos esta funcion para ignorar los
/// espacios en blanco y tabulaciones de la línea y copiar solo la dirección DNS.
/// strnlen() devuelve como entero la longitud de un string.
/// fDnsLog: Puntero a FILE al archivo log.
/// line: String para registrar temporalmente los datos de cada línea.
/// actualDns: String para registrar el valor final de la dirección DNS que agregaremos a la tabla.
/// lineNum: Línea actual de lectura del archivo.
/// lenght: Longitud del substring con los datos que queremos descartar de la línea de la primera dirección DNS.
/// indPnt: Índice de posición en el string de destino para la copia de la primera dirección DNS.
/// isEnd: Booleano de control para identificar si hemos llegado a los datos triviales.
/// limitChar: String con el valor a buscar paara descartar la información no útil en la línea de la primera dirección DNS.
/// whiteSpace y tabulation: Carácteres que queremosignorar en la captura de las direcciones DNS del resto de líneas.
/// </summary>
/// <param name="table">dnsTable Tabla para almacenar las direcciones DNS en formato String.</param>
/// <param name="retVal">int Devuelve 0 si la operación se realiza con éxito, 1 si hay algún fallo.</param>
void readDnsLog(dnsTable* table, int* retVal);

/// <summary>
/// Funciona como un driver para realizar todas las funciones de test y asignación de DNS.
/// Realiza los tests de accesibilidad sobre los DNS del adaptador escogido por el usuario y, si son accesibles, los añade a la lista de
/// servidores DNS obtenida del archivo en la carga de datos.
/// Realiza los tests de velocidad sobre todos los adaptadores, carga la información en un tabla y ordena los adaptadores de la tabla según
/// los resultados (primero, por tiempo medio de respuesta, y si este es el mismo, por número de saltos).
/// Una vez ordenados, comprueba si ya está asignado el mejor servidor DNS, si no es así, asigna al adaptador escogido por el usuario
/// en el primer paso del programa el servidor más rápido.
/// fDnsTest: Puntero a FILE para crear el archivo temporal donde volcaremos el test de velocidad.
/// testTable: Tabla de tipo dnsTestTable formada por elementos TAD del tipo dnsTest que contienen la información de los test a los
/// servidores DNS.
/// </summary>
/// <param name="adptsTbl">adptsTable Tabla de elementos TAD tipo adapter con los datos de los adaptadores de red del sistema.</param>
/// <param name="dnsDirs">dnsTable Tabla de DNS con los servidores DNS leídos del archivo en la carga de datos.</param>
/// <param name="adprDnsSrvs">dnsTable Tabla DNS con los servidores DNS del adaptador escogido por el usuario.</param>
/// <param name="adptrInd">int Índice del adaptador escogido por el usuario.</param>
/// <param name="retVal">int Devuelve 0 si la operación se ejecuta con éxito, o 1 si falla.</param>
void testDnsServers(const adptsTable adptsTbl, const dnsTable dnsDirs, const dnsTable adprDnsSrvs, const int adptrInd, int* retVal);

/// <summary>
/// Comprueba la accesibilidad a un grupo de servidores DNS que obtiene por parámetro y guardo los resultados en un archivo temporal
/// que también obtiene como parámetro.
/// La función gestiona una llamada a un comando CMD tracert mediante el uso de un pipe, para ello sua la función _popen() que 
/// permite leer el retorno del comando directamente desde el programa. Usamos las posiciones de las DNS guardadas en la tabla para 
/// ir realizando una itereación que ejecute tracert sobre cada una de las direcciones.
/// Para realizar la llamada, primero concatena las diversas partes de la sentencia del comando mediante la función sprintf() una vez
/// obtenida, lanzamos el comando mediante un pipe con _popen() y comenzamos la lectura del retorno. Continuamos iniciando el buckle
/// de lecturas línea a línea y gestionando lso diversos escenarios que nos interesan:
/// 1. Mediante el uso de un contador de líneas, ignoramos las diferentes líneas que contienen información trivial.
/// 2. Proseguimos realizando la lectura de los resultados de la traza de rutas. Identificamos mediante una palabra clave si la línea
/// leída informa de que el adaptador es no accesible. Si es así, lo mostramos al usuario e indicamos, mediante un booleano de control
/// que ya hemos obtenido la respuesta, de esta forma, indicamos al programa que ya no debe buscar más información y que únicamente acabe
/// de ejecutar el comando para marcar el EOF.
/// 3. Si obtenemos respuesta del servidor y empezamos la traza de la ruta, dejamos que realice un segundo salto, identificamos que se ha
/// producido mediante un control de carácter clave y seteamos el booleano indicando que hemos hallado la respuesta.
/// 4. Una finalizado el comando, cerramos el pipe mediante _pclose() y continueamos con el bucle sobre la tabla de DNS.
/// Limitamos el comando tracert ha 2 saltos porque es suficiente para obtener la información deseada y así aligeramos el proceso. La
/// función tambien gestiona el control de errores, la asignación y la liberación de memoria.
/// ptr: Puntero al pipe de lectura del comando.
/// command: String para crear la sentencia del comando.
/// line: String para asignar la línea leída del pipe.
/// lineNum: Contador del número de líneas leídas.
/// isSet: Booleano de control de obtención de los datos.
/// </summary>
/// <param name="adptrDnsSrvs">dnsTable Tabla con las direcciones DNS de un adaptador de red..</param>
/// <param name="fTempFile">Archivo temporal.</param>
/// <param name="retVal">Vale 0 si la operación se ejecuta sin errores, si no, 1.</param>
void findDnsAccessibility(const dnsTable adptrDnsSrvs, FILE* fTempFile, int* retVal);

/// <summary>
/// Combina en una tabla de tipo dnsTestTable las direcciones DNS de una tabla de tipo dnsTable y de un archivo temporal que hemos generado
/// con DNS válidas que hemso testado.
/// </summary>
/// <param name="src1">dnsTable Primera tabla de origen.</param>
/// <param name="src2">dnsTable Segunda tabla de origen.</param>
/// <param name="retVal">Retorn 0 si la operación se ha realizado con éxito o 1 si ha fallado.</param>
void mergeToDnsTestTable(const dnsTable src, FILE* fTempFile, dnsTestTable* testTable, int* retVal);

/// <summary>
/// Realiza un test de velocidad sobre las direcciones DNS de una tabla tipo dnsTest pasada por parámetro que contiene elementos TAD de tipo dnsTest.
/// De cada elemento se leerá la dirección DNS donde ejecutar el test, y se grabarán el número total de saltos y el tiempo medio de respuesta que
/// calculamos dividiendo el tiempo total de respuesta por el número de salto. Para realizar la lectura de la ejecución del comando, se usa un pipe
/// que abre como un puntero a FILE la respuesta de la ejecución de un tracert por CMD mediante la función _popen().
/// Para realizar la prueba se usa el comando CMD tracert del cuál obtenemos tanto el número de saltos, como el tiempo de respuesta medio para cada salto.
/// En el caso de devolver un retorno tipo Tiempo d eespera agotado, como el comando se lanza con una opción de tiempo de espera máximo de 1000 ms, se
/// asignará ese valor a el tiempo de respuesta de ese salto.
/// La función gestiona los posibles errores de lectura del retorno del comando o de lectura de la tabla de direcciones.
/// </summary>
/// <param name="testTable">dnsTestTable Tabla con las direcciones DNS a leer y sobre la que se escribirán el resultado de las pruebas.</param>
/// <param name="retVal">int Devuelve 0 si las operaciones se ejecutan sin errores, o 1 de lo contrario.</param>
void speedTest(dnsTestTable* testTable, int* retVal);


void changeDns(const adptsTable adptsTbl, const dnsTestTable testTable, const dnsTable adptsDns, const int adptsInd, int* retVal);

/// <summary>
/// Ordenamos mediante una función recursiva tipo Quicksort los elementos de una tabla tipo dnsTestTable.
/// Quicksort se basa en el principio de "divide y vencerás"; toma un elemento de la tabla como pivote, en este caso el elemento final,
/// y realiza una partición de la tabla alrededor del pivote. Para realizar la operación, usa la función partitionDnsTestTable() que tiene
/// como objetivo, tomando un elemento n de la tabla como pivote, colocarlo en su posición correcta y ordenar a su izquierda los elementos
/// menores y a su derecha los elementos mayores. 
/// El objetivo del uso de esta funcion es mantener la ejecución en tiempo linear.
/// El factor usado para ordenar la tabla es, primero, tiempo medio de respuesta y, segundo, número de saltos en caso de que el tiempo medio
/// de respuesta sea igual entre los dos elementos.
/// pivotInd = Recibe el índice de posición del pivote devuelto por la función partitionDnsTestTable().
/// </summary>
/// <param name="table">dnsTestTable Tabla de elementos TAD dnsTest a ordenar.</param>
/// <param name="low">Índice del primer elemento elemento.</param>
/// <param name="high">Índice del último elemento.</param>
void qSortDnsTestTable(dnsTestTable* table, int low, int high);

/// <summary>
/// Genera los puntos de partición para implementar el quicksort, mueve los elementos más pequeños a la izquierda del pivote y los
/// elementos más grandes a la derecha del pivote. El pivote queda colocado en su sitio. De este modo, es más sencillo ordenar
/// la tabla porque ya hemos generado este orden previo.
/// generaremos el orden a través de la velocidad media de respuesta del servidor DNS, si esta es igual en dos servidores distintos,
/// escogeremos el servidor con menos saltos.
/// pivot = Elemento TAD tipo dnsTest que funcionará como pivote de la comparación.
/// ind = Índice de posición de la partición.
/// </summary>
/// <param name="table">dnsTestTable Tabla de elementos TAD dnsTest a ordenar..</param>
/// <param name="low">int Índice de posición del primer elemento de la tabla.</param>
/// <param name="high">int Índice de posición del último elemento de la tabla..</param>
/// <returns>int Índice de partición actual para aplicar la recursividad.</returns>
int partitionDnsTestTable(dnsTestTable* table, int low, int high);

/// <summary>
/// Intercambia dos elementos TAD tipo dnsTest.
/// temp = Elemento TAD tipo dnsTest que funciona como variable temporal para poder realizar el intercambio.
/// </summary>
/// <param name="src1">dnsTest Origen 1.</param>
/// <param name="src2">dnsTest Origen2.</param>
void swapDnsTest(dnsTest* src1, dnsTest* src2);

/// <summary>
/// Copia la información de un elemento TAD dnsTest de origen a otro de destino, obtenidos ambos por parámetros.
/// </summary>
/// <param name="src">dnsTest Elemento origen.</param>
/// <param name="dest">dnsTest Elemento destino.</param>
void cpyDnsTest(dnsTest src, dnsTest* dest);

/// <summary>
/// Copia la información de un TAD tipo "adpatader" a otro TAD tipo "adaptador".
/// </summary>
/// <param name="dst">adapter Tad tipo "adapter" de destino.</param>
/// <param name="src">TAD tipo "adapter" de origen.</param>
void adptrCopy(adapter* dst, const adapter src);

/// <summary>
/// Imprime toda la información de los adaptores almacenados en una tabla adptsTable que almacena estructuras TAD del tipo "adapter".
/// </summary>
/// <param name="table">adptsTable Tabla adptsTable que almacena estructuras TAD del tipo "adapter"</param>
void printAdptsList(const adptsTable table);

/// <summary>
/// Impreme las direcciones DNS de una tabla tipo dnsTable.
/// </summary>
/// <param name="table">dnsTable Tabla que contiene un array con las direcciones DNS.</param>
void printDnsTable(const dnsTable table);

/// <summary>
/// Muestra por pantalla una lista con el nombre de todos los adaptadores de red del equipo que estén conectados.
/// adptsNum: Número de adaptadores con conexión.
/// </summary>
/// <param name="table">adptsTable Tabla que contiene estructuras TAD de tipo adapter con la información de los adaptadores de red.</param>
/// <returns>int numAdpts Número de adaptadores con conexión.</returns>
int printAdptsNames(const adptsTable table);

/// <summary>
/// Imprime por pantalla la información del servidor DNS más rápido.
/// </summary>
/// <param name="testTable">dnsTestTable Tabla de elementos TAD tipo dnsTest.</param>
void printBestDns(const dnsTestTable testTable);

/// <summary>
/// Limpia completamente los datos de una tabla de DNS. Recorre todas las posiciones de la tabla, librea la memoria asignada y iguala
/// los punteros a NULL. Por último, resetea el número de elemento a 0.
/// </summary>
/// <param name="table">dnsTable Tabla de direcciones DNS.</param>
void clearDnsTable(dnsTable* table);

/// <summary>
/// Limpia completamente los datos de una tabla de adaptadores. Recorre todas las posiciones de la tabla, librea la memoria asignada e iguala
/// los punteros a NULL, resetea los campos a los valores por defecto. Por último, resetea el número de elemento a 0.
/// </summary>
/// <param name="adptsTbl">adptsTable Tabla de elementos TAD de tipo adapter.</param>
void clearAdptsTable(adptsTable* adptsTbl);

/// <summary>
/// Limpia completamente los datos de una tabla de elementos TAD tipo dnsTest. Recorre todas las posiciones de la tabla, librea la memoria asignada 
/// e iguala los punteros a NULL y resetea los campos a sus valores por defecto. Por último, resetea el número de elemento a 0.
/// </summary>
/// <param name="testTable">dnsTestTable Tabla de elementos TAD tipo dnsTest.</param>
void clearDnsTestTable(dnsTestTable* testTable);

/// <summary>
/// Liberamos una estructura TAD tipo dnsTest y reseteamos sus campos.
/// </summary>
/// <param name="test">dnsTest Estructura TAD tipo dnsTest que contiene los resultados de los test de velocidad a una dirección DNS.</param>
void freeDnsTest(dnsTest* test);

/// <summary>
/// Liberamos la memoria usada en una estructura TAD tipo adapter y reseteamos sus campos.
/// </summary>
/// <param name="adptr">adptr Estructura TAD que contiene los datos de un adaptador de red.</param>
void freeAdapter(adapter* adptr);

/// <summary>
/// Determina si la tabla específica está llena.
/// </summary>
/// <param name="table">adptsTable Tabla de TAD tipo "adaptador".</param>
/// <returns>
///   <c>true</c>, si la tabla está llena; Si queda espacio, <c>false</c>.
/// </returns>
bool isFullAdpts(const adptsTable table);

/// <summary>
/// Determina si la tabla específica está llena.
/// </summary>
/// <param name="table">adptsTable Tabla de TAD tipo "adaptador".</param>
/// <returns>
///   <c>true</c>, si la tabla está llena; Si queda espacio, <c>false</c>.
/// </returns>
bool isFullDns(const dnsTable table);

/// <summary>
/// Determina si la tabla específica está llena.
/// </summary>
/// <param name="table">adptsTable Tabla de TAD tipo "adaptador".</param>
/// <returns>
///   <c>true</c>, si la tabla está llena; Si queda espacio, <c>false</c>.
/// </returns>
bool isFullDnsTest(const dnsTestTable table);