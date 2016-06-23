/**
 *********************************************************************************************************************************
 *                                                                                                                               *
 *                                                                                                                               *
 *         ad88                                        88                                                        88              *
 *        d8"                                          ""                                                        88              *
 *        88                                                                                                     88              *
 *      MM88MMM  88       88  8b,dPPYba,    ,adPPYba,  88   ,adPPYba,   8b,dPPYba,    ,adPPYba,  ,adPPYba,       88,dPPYba,      *
 *        88     88       88  88P'   `"8a  a8"     ""  88  a8"     "8a  88P'   `"8a  a8P_____88  I8[    ""       88P'    "8a     *
 *        88     88       88  88       88  8b          88  8b       d8  88       88  8PP"""""""   `"Y8ba,        88       88     *
 *        88     "8a,   ,a88  88       88  "8a,   ,aa  88  "8a,   ,a8"  88       88  "8b,   ,aa  aa    ]8I  888  88       88     *
 *        88      `"YbbdP'Y8  88       88   `"Ybbd8"'  88   `"YbbdP"'   88       88   `"Ybbd8"'  `"YbbdP"'  888  88       88     *  
 *                                                                                                                               *
 *                                                                                                                               *
 *********************************************************************************************************************************
 ******-> A U T O R E S <-*******
 *                              *
 * Johanna Chan        08-10218 *
 * Carlos Rodriguez    06-40189 *
 *                              *
 ********************************    
 **/

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/dir.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <netdb.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>


/*structura que se encarga de almacenar los parametros de la sintaxis.
 * Campo p: corresponde al numero de puerto habilitado por el servidor.
 * Campo s: corresponde al nombre de la sala.
 * La utiliza el cliente
 */

struct DatosC {
  char* host;
  int   puerto;
  char* nombre;
  char* archivo;
};typedef struct DatosC DatosCliente;

/* Estructura que se encarga de almacenar los parametros de la sintaxis.
 * Campo p: corresponde al numero de puerto habilitado por el servidor.
 * Campo s: corresponde al nombre de la sala.
 * La utiliza el servidor
 */
  struct DatosS {
    int puerto;
    char* sala;
  };
  typedef struct DatosS DatosServidor;

/* Estructura que se encarga de almacenar el tamaño del mensaje y el mensaje
 */
  struct tammensaje {
    char* mensaje;
    int   numero;
  };
  typedef struct tammensaje MSJ;
  
  struct MensajeDividido {
    char* comando;
    char* resto;
  };
  typedef struct MensajeDividido MSJdiv;
  
 /*
  * 
  * Esta funcion se encarga de verificar si la cadena introducida corresponde al
  * nombre de un archivo, el nombre de un archivo puee estar constituido por
  * caracteres o digitos, es por eso que se utilizo la funcion isalnum
  * correspondiente a la libreria ctype.
  * 
  * @param cadena es el string que se desea analizar
  * @return un booleano, true si la cadena corresponde al nombre de un archivo.
  * 
  */

  extern int EsNombreArchivo(char * cadena);
  
 /*
  * 
  * EsNombre es una funcion que se encarga de verificar si la cadena de
  * caracteres introducida es una palabra, esto lo logra mediante la funcion 
  * isalpha() que retorna un 0 si no lo es.
  * 
  * @param nombre es el string a ser analizado.
  * @return 1 si es una palabra, 0 de lo contrario.
  * 
  */

  extern int EsPalabra(char * nombre);

  
 /*
  * 
  * EsNumero es una funcion que se encarga de verificar si la cadena de
  * caracteres introducida corresponde a un numero.
  * 
  * @param numero, es la cadena de caracteres a ser analizada.
  * @return 1 si es un numero, 0 de lo contrario.
  */
 
  extern int EsNumero(char * numero);

  
 /*
  * 
  * ContarPalabra es una funcion que se encarga de abrir un archivo y contar las
  * apariciones de una determinada palabra, esta funcion recibe como parametros
  * 2 cadenas de caracteres, la primera es el nombre del archivo a abrir y la
  * segunda es la palabra a la que se quiere contabilizar el numero de
  * apariciones.
  * 
  * @param socket_cliente es el socket al que se enviara la informacion de cada linea.
  * @param NombreArchivo es el nombre del archivo que se va a examinar.
  * @param Palabra es la palabra que se desea contar a lo largo del archivo.
  * @param nombre nombre del usuario.
  * @return el numero de veces que aparece Palabra en el archivo NombreArchivo.
  * 
  */

  extern int ContarPalabras(int socket_cliente, char * NombreArchivo, char * Palabra, char* nombre);
  
  
 /*
  * 
  * msjdivision es una funcion que se encarga de recibir un string y separarlo en partes,
  * y de esta forma obtener el comando y el mensaje enviado a traves del socket.
  * 
  * @param msjcompleto es el string completo que se pretende dividir.
  * @return una estructura del tipo MSJdiv que almacena el comando y el resto del mensaje.
  * 
  */   

  extern MSJdiv msjdivision(char *msjcompleto);
  
  
 /*
  * 
  * ImprimirAyuda, como su nombre lo dice imprime la ayuda para la buena utilizacion
  * del programa.
  * 
  */
 
  extern void ImprimirAyuda_servidor();
  
    
 /*
  * 
  * ImprimirAyuda, como su nombre lo dice imprime la ayuda para la buena utilizacion
  * del programa.
  * 
  */
 
  extern void ImprimirAyuda_cliente();
  

 /*
  * 
  * Esta funcion se encarga de verificar que se introdujo en la entrada estandar
  * si fue un schat [-p <puerto>] [-s <sala>], si la entrada es valida, 
  * retorna 
  * 
  * @param NumArg corresponde al numero de argumentos introducidos por el usuario.
  * @argu es el arreglo de caracteres que contienen las opciones y argumentos.
  * @return una estructura del tipo DatosServidor que almacena la informacion
  * colocada en la entrada estandar.
  * 
  */

  extern DatosServidor Verificacion_servidor(int NumArg, char *argu[]);
  

 /*
  * 
  * Esta funcion se encarga de verificar que se introdujo en la entrada estandar
  * si fue un cchat [-h host] [-p <puerto>] [-n <nombre>] [-a <archivo>]
  * 
  * @param NumArg corresponde al numero de argumentos introducidos por el usuario.
  * @argu es el arreglo de caracteres que contienen las opciones y argumentos.
  * @return una estructura del tipo DatosServidor que almacena la informacion
  * colocada en la entrada estandar.
  * 
  */

  extern DatosCliente Verificacion_cliente(int NumArg, char *argu[]);
  

 /*
  * 
  * Esta funcion se encarga de imprimir el logo del programa del servidor.
  * 
  */
 
  extern void logo_servidor();
  
  
 /*
  * 
  * Esta funcion se encarga de imprimir el logo del programa del cliente.
  * 
  */

  extern void logo_cliente();
  
  
 /*
  * 
  * leer_servidor es una funcion que se encarga de leer la informacion contenida
  * en el socket, este se especifica en el argumento de la funcion.
  * 
  * @socket_cliente corresponde al descriptor del socket de donde se desea leer el mensaje.
  * @return una estructura que contiene el mensaje y su tamano
  * 
  */
 
  extern MSJ leer_servidor(int socket_cliente);

  
 /*
  * 
  * leer_servidor es una funcion que se encarga de leer la informacion contenida
  * en el socket, este se especifica en el argumento de la funcion.
  * 
  * @socket_cliente corresponde al descriptor del socket de donde se desea leer el mensaje.
  * @return una estructura que contiene el mensaje y su tamano
  * 
  */
 
  extern MSJ leer_cliente(int socket_cliente);
  
    
 /*
  * 
  * escribir_cliente es una funcion que se encarga de escribir informacion en el socket especificado
  * en el argumento de la funcion.
  * 
  * @param socket_cliente correspondel al descriptor del socket en donde se desea escribir.
  * @param mensaje es el mensaje que se desea enviar
  * @paramm nombre corresponde al nombre del usuario que esta escribiendo el mensaje
  * @return una estructura con el mensaje enviado y su tamano.
  * 
  */
    
  extern MSJ escribir_servidor(int socket_cliente, char* mensaje, char* nombre);
  
  
 /*
  * 
  * escribir_cliente es una funcion que se encarga de escribir informacion en el socket especificado
  * en el argumento de la funcion.
  * 
  * @param socket_cliente correspondel al descriptor del socket en donde se desea escribir.
  * @param mensaje es el mensaje que se desea enviar
  * @paramm nombre corresponde al nombre del usuario que esta escribiendo el mensaje
  * @return una estructura con el mensaje enviado y su tamano.
  * 
  */
 
  extern MSJ escribir_cliente(int socket_cliente, char* mensaje, char* nombre);
  
  /*
   * 
   * Verifica si la cadena de caracteres mensaje es un comando valido.
   * De ser corrrecto, lo ejecuta.
   * 
   * @param socket Socket a traves del cual se enviara el comando al servidor.
   * @param mensaje Cadena de caracteres que contiene un comando a ejecutar por el cliente.
   * 
   */
  
  extern void verificar_mensaje (int socket, char *mensaje);
  
  /*
   * 
   * Lee un archivo de texto y ejecuta los comandos
   * que hayan en el.
   * 
   * @param socket El socket a traves del cual enviara los comandos al servidor schat.
   * @param archivo Nombre del archivo a abrir.
   * 
   */
  
  extern void leer_archivo (int socket, char * archivo);
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  