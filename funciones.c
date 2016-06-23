/**
 *********************************************************************************************************************************
 *                                                                                                                               *
 *                                                                                                                               *
 *         ad88                                        88                                                                        *
 *        d8"                                          ""                                                                        *
 *        88                                                                                                                     *
 *      MM88MMM  88       88  8b,dPPYba,    ,adPPYba,  88   ,adPPYba,   8b,dPPYba,    ,adPPYba,  ,adPPYba,        ,adPPYba,      *
 *        88     88       88  88P'   `"8a  a8"     ""  88  a8"     "8a  88P'   `"8a  a8P_____88  I8[    ""       a8"     ""      *
 *        88     88       88  88       88  8b          88  8b       d8  88       88  8PP"""""""   `"Y8ba,        8b              *
 *        88     "8a,   ,a88  88       88  "8a,   ,aa  88  "8a,   ,a8"  88       88  "8b,   ,aa  aa    ]8I  888  "8a,   ,aa      *  
 *        88      `"YbbdP'Y8  88       88   `"Ybbd8"'  88   `"YbbdP"'   88       88   `"Ybbd8"'  `"YbbdP"'  888   `"Ybbd8"'      *
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
#include "funciones.h"
#include "libSockets.h"

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

  int EsNombreArchivo(char * cadena) {
    int boolean, i;
    i = 0;
    boolean = 0;
    while(i != strlen(cadena)){
      if(cadena[i] != '/'){
	i++;
	boolean = 1;
      }else{
	printf(" La cadena de caracteres no corresponde al nombre de un archivo.\n");
	boolean = 0;
	break;
      }
    }
    return boolean;
  }
  
  
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

  int EsPalabra(char * nombre){
    int i;
    i = 0;
    int boolname;
    boolname = 1;
    while(i<strlen(nombre)-1){
      if(isalpha(nombre[i])== 0){
	boolname = 0;
	break;
      }
      i++;
    }
    return boolname;
  }

  
 /*
  * 
  * EsNumero es una funcion que se encarga de verificar si la cadena de
  * caracteres introducida corresponde a un numero.
  * 
  * @param numero, es la cadena de caracteres a ser analizada.
  * @return 1 si es un numero, 0 de lo contrario.
  */
 
  int EsNumero(char * numero){
    int i,boolean;
    i = 0;
    boolean = 0;
    while(i != strlen(numero)){
      if(isdigit(numero[i]) != 0 ){
	i++;
	boolean = 1;
      }else{
	boolean = 0;
	break;
      }
    }
    return boolean;
  }

  
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

  int ContarPalabras(int socket_cliente, char * NombreArchivo, char * Palabra, char* nombre){
    char *s2 = ".,:;`'\"+-_(){}[]<>*&^%$#@!?~/|\\= \t\r\n1234567890#";
    char LineaEntrada[1024];	/* Contiene una linea del archivo que se abrio.*/
    memset( LineaEntrada, 0, 1024*sizeof(char) );/*inicializacion del arreglo "buffer"*/
    FILE *Entrada;
    int ocur = 0;
    char *ptr;	/* Se almacena la palabra (token) delimitada por los caracteres de la cadena s2 */
    if((Entrada = fopen(NombreArchivo, "r")) != NULL){
      while (fgets(LineaEntrada, sizeof(LineaEntrada), Entrada) != NULL){
	escribir_cliente(socket_cliente, LineaEntrada, nombre);
	ptr = strtok( LineaEntrada, s2 );	/* Primera llamada => Primer token, separo las palabras por los delimitadores. */
	if(ptr != NULL){
	  if(strcmp(ptr, Palabra)==0){	/* Comparo si el token es igual a la palabra que busco.*/
	    ocur++;
	  }
	}
	while( ptr != NULL ){	/* Posteriores llamadas */
	  ptr = strtok( NULL, s2 );
	  if(ptr != NULL){
	    if(strcmp(ptr, Palabra)==0){
	      ocur++;
	    }
	  }
	}
      }
    }else{
      printf("ERROR\n	- El Archivo %s no se pudo abrir.\n", NombreArchivo);
    }
    return ocur;
  }
 
 
 /*
  * 
  * msjdivision es una funcion que se encarga de recibir un string y separarlo en partes,
  * y de esta forma obtener el comando y el mensaje enviado a traves del socket.
  * 
  * @param msjcompleto es el string completo que se pretende dividir.
  * @return una estructura del tipo MSJdiv que almacena el comando y el resto del mensaje.
  * 
  */   

  MSJdiv msjdivision(char *msjcompleto){
    MSJdiv New;
    char s1[1024];
    char buffer[1024];
    memset( s1, 0, 1024*sizeof(char) );/*inicializacion del arreglo*/
    memset( buffer, 0, 1024*sizeof(char) );/*inicializacion del arreglo*/
    strcpy(s1, msjcompleto);
    char s2[2] = " ";
    char *ptr;
    ptr = strtok( s1, s2 );    // Primera llamada => Primer token
    New.comando =(char*)malloc(strlen(ptr)*sizeof(char));
    strcpy(New.comando, ptr);
    while( (ptr = strtok( NULL, s2 )) != NULL ){    // Posteriores llamadas
      New.resto =(char*)malloc(1024*sizeof(char));
      strcat(buffer, ptr);
      strcat(buffer, " ");
      strcpy(New.resto, buffer);
    }
    printf("New.comando contiene %s\n", New.comando);
    printf("New.resto contiene %s\n", New.resto);
    printf("buffer contiene %s\n", buffer);
    return New;
  }
  
  
  
 /*
  * 
  * ImprimirAyuda, como su nombre lo dice imprime la ayuda para la buena utilizacion
  * del programa.
  * 
  */
 
  void ImprimirAyuda_servidor(){
    printf("\n");
    printf(" La sintaxis para la invocación del servidor será la siguiente: \n\n");
    printf(" schat [-p <puerto>] [-s <sala>]\n\n");
    printf(" Donde: \n\n");
    printf("\t<puerto> Es el número de puerto que el servidor utilizará para colocar un socket para\n");
    printf("\t         esperar por las solicitudes de conexión que el servidor puede recibir, estas\n");
    printf("\t         solicitudes podrán ser originadas por diferentes programas cchat.\n\n");
    printf("\t <sala>  Es el nombre de la sala de chat por defecto que tendrá el servidor. No incluye\n");
    printf("\t         numeros.\n\n");

    }
    
    
 /*
  * 
  * ImprimirAyuda, como su nombre lo dice imprime la ayuda para la buena utilizacion
  * del programa.
  * 
  */
  void ImprimirAyuda_cliente(){
    printf(" La sintaxis para la invocación del cchat será la siguiente:\n\n");
    printf(" cchat [-h <host>] [-p <puerto>] [-n <nombre>][-a <archivo>]\n\n");
    printf(" Donde:\n\n");
    printf("\t  <host>   Es el nombre o dirección IP del computador donde está corriendo el programa\n\n");
    printf("\t           schat.\n\n");
    printf("\t <puerto>  Es el número de puerto que el programa schat utilizará para recibir las conexiones\n");
    printf("\t           de los diferentes programas cchat.\n\n");
    printf("\t <nombre>  Es el nombre de usuario que será usado en todos los mensajes que el usuario\n");
    printf("\t           envíe al servidor y que el servidor enviara a todos los otros usuarios, incluyéndolo\n");
    printf("\t           a el mismo.\n\n");
    printf("\t <archivo> Es el nombre y dirección relativa o absoluta de un archivo e texto en el que en\n");
    printf("\t           cada línea habrá un comando para cchat. Al cchat terminar de ejecuatr los comandos\n");
    printf("\t           presentes en el archivo, debe permanecer a la espera de nuevos comandos por el teclado.\n");
    printf("\t           A menos que, en el archivo este presente el comando fue .\n\n");
  }

  
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

  DatosServidor Verificacion_servidor(int NumArg, char *argu[]){
    printf("\n");
    DatosServidor New;
    int c,s,p,h;
    c = 0;
    s = 0; /*indica si el usuario efectivamente coloco el nombre correspondiente a la sala*/
    p = 0; /*indica si el usuario efectivamente coloco el numero correspondiente a la puerto*/
    h = 0; /*indica si el usuario solicito ayuda*/
    opterr = 0;
    while((c = getopt(NumArg, argu, "hp:s:")) != -1){ /*"p:s:" Son los comandos aceptados*/
      
      switch(c){
	
	case 'h':
	  if(NumArg ==2 && p == 0 && s == 0){
	    h = 1;
	    ImprimirAyuda_servidor();
	    break;
	  }else{
	    printf(" Para utilizar el comando -h, debe escribir en la linea de comandos solamente pargrep -h.\n");
	    break;
	  }
	  
	case 'p':
	  if(NumArg == 5 || NumArg == 3){  
	    if(optarg){
	      if(EsNumero(optarg) == 1){
		p = 1;
		New.puerto = atoi(optarg);
		break;
	      } else {
		printf("\"%s\" No es valido para la opcion -p.\n", optarg);
		break;
	      } 
	    }
	  }
	  
	  case 's':
	    if(NumArg == 5){  
	      if(optarg){
		if(EsPalabra(optarg) == 1){
		  s = 1;
		  New.sala = malloc(sizeof(char)*strlen(optarg));
		  New.sala = optarg;
		  break;
		} else {
		  printf("\"%s\" No es valido para la opcion -s.\n", optarg);
		  break;
		} 
	      }
	    }
	  
	  /*
	  * en optopt se guardan los comandos invalidos o que les falta el argumento.
	  */		 
	  case '?':
	    if ((optopt != 'p') && (optopt != 's') && (optopt != 'h')){
	      fprintf (stderr, "El comando -%c es invalido.\n", optopt);
	    }else if (isprint (optopt)){
	      fprintf (stderr, " El Comando '-%c' requiere un argumento.\n", optopt);
	    }else{
	      fprintf (stderr,"Comando Invalido `\\x%x'.\n",optopt);
	    }
	    break;
      }
    }

    if(p == 0){ // si no especifico el puerto
      if(h == 0){
	    printf("\nLo que acaba de escribir es inválido.\nSi desea ayuda ingrese ./schat -h\n\n");
      }
      exit(1);
    }
    if(s == 0){
      New.sala = malloc(sizeof(char)*strlen("actual"));
      New.sala = "actual";
    }
    return New;
  }

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

  DatosCliente Verificacion_cliente(int NumArg, char *argu[]){
    printf("\n");
    DatosCliente New;	//Estructura definida en el archivo funciones.h para guardar los argumentos.
    int c,h,p,n,a,o;
    c = 0;
    h = 0; //host
    p = 0; //puerto
    n = 0; //nombre
    a = 0; //archivo
    o = 0; //ayuda
    opterr = 0;
    while((c = getopt(NumArg, argu, "h:p:n:a:o")) != -1){ //"p:s:" Son los comandos aceptados
      switch(c){
	case 'o':
	  if(NumArg ==2 && h == 0 && p == 0 && n == 0 && a == 0){
	    o = 1;
	    ImprimirAyuda_cliente();
	    break;
	  }else{
	    printf(" Para utilizar el comando -h, debe escribir en la linea de comandos solamente pargrep -h.\n");//OJO
	    break;
	  }
	  
	  
    case 'h':
	  if(NumArg == 9 ){  
	    if(optarg){
	      printf("La IP es %s\n", optarg);
		  h = 1;
		  New.host = (char*)malloc(sizeof(char)*strlen(optarg));
		  New.host = optarg;
		  break;
	    }
	  }
	  
	  case 'p':
		  printf("estoy en p con %s\n", optarg);
	    if(NumArg == 9){  
	      if(optarg){
		if(EsNumero(optarg) == 1){
			  p = 1;
			  printf("El numero de puerto es %s\n", optarg);
			  New.puerto = atoi(optarg);
			  break;
		} else {
			  printf("\"%s\" No es valido para la opcion -p.\n", optarg);
			  break;
		} 
	      }
	      printf("sali del case p\n");
	    }
	    break;
	  
	  case 'n':
	    if(NumArg == 9){  
	      if(optarg){
		if(EsPalabra(optarg) == 1){
			  n = 1;
			  printf("El nombre que introdujo es %s\n", optarg);
			  New.nombre = malloc(sizeof(char)*strlen(optarg));
			  New.nombre = optarg;
			  break;
		} else {
			  printf("\"%s\" No es valido para la opcion -n.\n", optarg);
			  break;
		} 
	      }
	    }
	  
	  case 'a':
	    if(NumArg == 9){  
	      if(optarg){
		if(EsNombreArchivo(optarg) == 1){
			  a = 1;
			  printf("EL nombre del archivo es %s\n", optarg);
			  New.archivo = malloc(sizeof(char)*strlen(optarg));
			  New.archivo = optarg;
			  break;
		} else {
			  printf("\"%s\" No es valido para la opcion -a.\n", optarg);
			  break;
		} 
	      }
	    }
	  
	  /*
	   * en optopt se guardan los comandos invalidos o que les falta el argumento.
	   */
	  case '?':
	    if ((optopt != 'p') && (optopt != 's') && (optopt != 'h')){
	      fprintf (stderr, "El comando -%c es invalido.\n", optopt);
	    }else if (isprint (optopt)){
	      fprintf (stderr, " El Comando '-%c' requiere un argumento.\n", optopt);
	    }else{
	      fprintf (stderr,"Comando Invalido `\\x%x'.\n",optopt);
	    }
	    break;
      }
    }

    if(h == 0 || p == 0 || n == 0 || a == 0){ // si especifico el numero de jugadores y pidio ayuda
      if(o == 0){
	    printf("\nLo que acaba de escribir es inválido.\nSi desea ayuda ingrese ./schat -o\n\n");
      }
      exit(1);
    }
    return New;
  }

  
 /*
  * 
  * Esta funcion se encarga de imprimir el logo del programa del servidor.
  * 
  */
 
  void logo_servidor(){
    printf("\n");                                                                                                                                                                                           
    printf("%s                                     hhhhhhh                                        tttt      \n", KBLU);    
    printf("%s                                     h:::::h                                     ttt:::t      \n", KBLU);    
    printf("%s                                     h:::::h                                     t:::::t      \n", KBLU);    
    printf("%s                                     h:::::h                                     t:::::t      \n", KBLU);    
    printf("%s    ssssssssss       cccccccccccccccc h::::h hhhhh         aaaaaaaaaaaaa   ttttttt:::::ttttttt\n", KBLU);    
    printf("%s  ss::::::::::s    cc:::::::::::::::c h::::hh:::::hhh      a::::::::::::a  t:::::::::::::::::t\n", KBLU);
    printf("%sss:::::::::::::s  c:::::::::::::::::c h::::::::::::::hh    aaaaaaaaa:::::a t:::::::::::::::::t\n", KBLU);    
    printf("%ss::::::ssss:::::sc:::::::cccccc:::::c h:::::::hhh::::::h            a::::a tttttt:::::::tttttt\n", KBLU);    
    printf("%s s:::::s  ssssss c::::::c     ccccccc h::::::h   h::::::h    aaaaaaa:::::a       t:::::t      \n", KBLU);    
    printf("%s   s::::::s      c:::::c              h:::::h     h:::::h  aa::::::::::::a       t:::::t      \n", KBLU);    
    printf("%s      s::::::s   c:::::c              h:::::h     h:::::h a::::aaaa::::::a       t:::::t      \n", KBLU);   
    printf("%sssssss   s:::::s c::::::c     ccccccc h:::::h     h:::::ha::::a    a:::::a       t:::::t    tttttt\n", KBLU);
    printf("%ss:::::ssss::::::sc:::::::cccccc:::::c h:::::h     h:::::ha::::a    a:::::a       t::::::tttt:::::t\n", KBLU);
    printf("%ss::::::::::::::s  c:::::::::::::::::c h:::::h     h:::::ha:::::aaaa::::::a       tt::::::::::::::t\n", KBLU);
    printf("%s s:::::::::::ss    cc:::::::::::::::c h:::::h     h:::::h a::::::::::aa:::a        tt:::::::::::tt\n", KBLU);
    printf("%s  sssssssssss        cccccccccccccccc hhhhhhh     hhhhhhh  aaaaaaaaaa  aaaa          ttttttttttt  \n", KBLU);
    printf("%s\n", KWHT);                                                                                                     
  }
  
  
 /*
  * 
  * Esta funcion se encarga de imprimir el logo del programa del cliente.
  * 
  */

  void logo_cliente(){
												  
    printf("%s                                        hhhhhhh                                        tttt          \n",KRED);
    printf("%s                                        h:::::h                                     ttt:::t          \n",KRED);
    printf("%s                                        h:::::h                                     t:::::t          \n",KRED);
    printf("%s                                        h:::::h                                     t:::::t          \n",KRED);
    printf("%s    cccccccccccccccc    cccccccccccccccc h::::h hhhhh         aaaaaaaaaaaaa   ttttttt:::::ttttttt    \n",KRED);
    printf("%s  cc:::::::::::::::c  cc:::::::::::::::c h::::hh:::::hhh      a::::::::::::a  t:::::::::::::::::t    \n",KRED);
    printf("%s c:::::::::::::::::c c:::::::::::::::::c h::::::::::::::hh    aaaaaaaaa:::::a t:::::::::::::::::t    \n",KRED);
    printf("%sc:::::::cccccc:::::cc:::::::cccccc:::::c h:::::::hhh::::::h            a::::a tttttt:::::::tttttt    \n",KRED);
    printf("%sc::::::c     cccccccc::::::c     ccccccc h::::::h   h::::::h    aaaaaaa:::::a       t:::::t          \n",KRED);
    printf("%sc:::::c             c:::::c              h:::::h     h:::::h  aa::::::::::::a       t:::::t          \n",KRED);
    printf("%sc:::::c             c:::::c              h:::::h     h:::::h a::::aaaa::::::a       t:::::t          \n",KRED);
    printf("%sc::::::c     cccccccc::::::c     ccccccc h:::::h     h:::::ha::::a    a:::::a       t:::::t    tttttt\n",KRED);
    printf("%sc:::::::cccccc:::::cc:::::::cccccc:::::c h:::::h     h:::::ha::::a    a:::::a       t::::::tttt:::::t\n",KRED);
    printf("%s c:::::::::::::::::c c:::::::::::::::::c h:::::h     h:::::ha:::::aaaa::::::a       tt::::::::::::::t\n",KRED);
    printf("%s  cc:::::::::::::::c  cc:::::::::::::::c h:::::h     h:::::h a::::::::::aa:::a        tt:::::::::::tt\n",KRED);
    printf("%s    cccccccccccccccc    cccccccccccccccc hhhhhhh     hhhhhhh  aaaaaaaaaa  aaaa          ttttttttttt  \n",KRED);
    printf("%s\n",KWHT);
  }
  
  
 /*
  * 
  * leer_servidor es una funcion que se encarga de leer la informacion contenida
  * en el socket, este se especifica en el argumento de la funcion.
  * 
  * @socket_cliente corresponde al descriptor del socket de donde se desea leer el mensaje.
  * @return una estructura que contiene el mensaje y su tamano
  * 
  */
 
  MSJ leer_servidor(int socket_cliente) {
    MSJ New;					/*Estructura que almacena el mensaje y su tamaño*/
    char Datos[1024]; 				/* Buffer donde guardaremos los caracteres a leer*/ 
    memset( Datos, 0, 1024*sizeof(char) ); 	/*Inicializacion del arreglo*/
    int Aux = read (socket_cliente, Datos, sizeof(Datos)); 	/*Se procede a leer del socket*/
    
    if (Aux == 0) { 						/*Si no se ha podido escribir, 
								 *se verifica la condicion de socket cerrado*/
      printf ("Socket cerrado\n"); 
    } else if (Aux == -1) { 
	printf ("Error\n"); 
    } else if(Aux >0) {
      New.mensaje = Datos;
      New.numero = Aux;
    } 
    return New;
  }

  
 /*
  * 
  * leer_cliente es una funcion que se encarga de leer la informacion contenida
  * en el socket, este se especifica en el argumento de la funcion.
  * 
  * @socket_cliente corresponde al descriptor del socket de donde se desea leer el mensaje.
  * @return una estructura que contiene el mensaje y su tamano
  * 
  */
  
  MSJ leer_cliente(int socket_cliente) {
    MSJ New;
    char Datos[1024]; /* Buffer donde guardaremos los caracteres */ 
      memset( Datos, 0, 1024*sizeof(char) );//inicializacion del arreglo "buffer"
    int Aux = read (socket_cliente, Datos, sizeof(Datos)); 
    /* Si no hemos podido escribir caracteres, 
    comprobamos la condici��n de socket cerrado */ 
    if (Aux == 0) { 
      printf ("Socket cerrado\n"); 
    } else if (Aux == -1) { 
	printf ("Error\n"); 
    } else if(Aux >0) {
      New.mensaje = Datos;
      New.numero = Aux;
    } 
    return New;
  }
  
  
  
 /*
  * 
  * escribir_servidor es una funcion que se encarga de escribir informacion en el socket especificado
  * en el argumento de la funcion.
  * 
  * @param socket_cliente correspondel al descriptor del socket en donde se desea escribir.
  * @param mensaje es el mensaje que se desea enviar
  * @paramm nombre corresponde al nombre del usuario que esta escribiendo el mensaje
  * @return una estructura con el mensaje enviado y su tamano.
  * 
  */
    
  MSJ escribir_servidor(int socket_cliente, char* mensaje, char* nombre) {
    MSJ New;						/*Estructura que almacena el mensaje y su tamaño*/
    int tamano = strlen(nombre) + strlen(mensaje)+2; 	/*tamano del buffer, el tamano del usuario mas el 
							 *tamano del mensaje mas el espacio que separa el 
							 *nombre del usuario y el mensaje y el caracter de fin de linea*/
    char Datos[tamano];
    memset( Datos, 0, tamano*sizeof(char) );		/*inicializacion del arreglo "buffer"*/
    strcat(Datos, nombre);				/*Concatenacion del nombre*/
    strcat(Datos, " ");					/*Concatenacion del espacio separador*/
    strcat(Datos, mensaje);				/*Concatenacion del mensaje*/
    Datos[tamano]='\0';					/*Colocacion del caracter fin de linea*/

    int Aux = write(socket_cliente, Datos, sizeof(Datos)); 	/*Se procede a escribir en el socket*/
    if (Aux == 0) { 						/*Si el socket en donde se desea escribir esta cerrado*/
      printf ("Socket cerrado\n"); 
    } else if (Aux == -1) { 					/*Si ocurrio un error en el socket*/
	printf ("Error\n"); 
    } else if(Aux >0) {
      New.mensaje = Datos;
      New.numero = Aux;
    } 
    return New;
  }
  
  
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
  
  MSJ escribir_cliente(int socket_cliente, char* mensaje, char* nombre) {
    MSJ New;
    int tamano = strlen(nombre) + strlen(mensaje)+2; 
    char Datos[tamano];
      memset( Datos, 0, tamano*sizeof(char) );//inicializacion del arreglo "buffer"
      strcat(Datos, nombre);
      strcat(Datos, " ");
    strcat(Datos, mensaje);
    Datos[strlen(mensaje)+1]='\0';
    int Aux = write(socket_cliente, Datos, sizeof(Datos)); 
    if (Aux == 0) { 
      printf ("Socket cerrado\n"); 
    } else if (Aux == -1) { 
	printf ("Error\n"); 
    } else if(Aux >0) {
      New.mensaje = Datos;
      New.numero = Aux;
    } 
    return New;
  }
  
  
 /*
  * 
  * Verifica si la cadena de caracteres mensaje es un comando valido.
  * De ser corrrecto, lo ejecuta.
  * 
  * @param socket Socket a traves del cual se enviara el comando al servidor.
  * @param mensaje Cadena de caracteres que contiene un comando a ejecutar por el cliente.
  * 
  */
  void verificar_mensaje (int socket, char * mensaje) {
    char inicio[4];
    int i;
    /* se extraen los primeros 3 caracteres, correspondientes al comando */
    for (i = 0; i < 3; i++) {
      inicio[i] = *(mensaje);
      mensaje++;
    }
    mensaje++;
    
    /* se ejecuta la instruccion segun sea el comando */
    if (strcmp (inicio, "sal") == 0){
      cliente_escribe_mensaje (socket, Id_sal, NULL, 0);
    }
    
    else if (strcmp (inicio, "usu") == 0){
      cliente_escribe_mensaje (socket, Id_usu, NULL, 0);
    }
    
    else if ((strcmp (inicio, "men") == 0) && (strlen(mensaje) > 0)){
      cliente_escribe_mensaje (socket, Id_men, mensaje, strlen (mensaje));
    }
    
    else if ((strcmp (inicio, "sus") == 0) && (strlen(mensaje) > 0)){
      cliente_escribe_mensaje (socket, Id_sus, mensaje, strlen (mensaje));
    }
    
    else if (strcmp (inicio, "des") == 0){
      cliente_escribe_mensaje (socket, Id_des, NULL, 0);
    }
    
    else if ((strcmp (inicio, "cre") == 0) && (strlen(mensaje) > 0)){
      cliente_escribe_mensaje (socket, Id_cre, mensaje, strlen (mensaje));
    }
    
    else if ((strcmp (inicio, "eli") == 0) && (strlen(mensaje) > 0)){
      cliente_escribe_mensaje (socket, Id_eli, mensaje, strlen (mensaje));
    }
    
    else if (strcmp (inicio, "fue") == 0){
      cliente_escribe_mensaje (socket, Id_fue, NULL, 0);
    }
    
    /* si el comando no es valido, se notifica al usuario */
    else{
      printf ("\nERROR! Comando no valido\n");
    }
  }
  
  
  /*
   * 
   * Lee un archivo de texto y ejecuta los comandos
   * que hayan en el.
   * 
   * @param socket El socket a traves del cual enviara los comandos al servidor schat.
   * @param archivo Nombre del archivo a abrir.
   * 
   */
  void leer_archivo (int socket, char * archivo) 
  {
    char mensaje_enviar[1024];
    FILE * Entrada;
    
    /* se abre el archivo */
    if((Entrada = fopen(archivo, "r")) != NULL)
    {
      int i;
      /* mientras no se haya llegado al final se lee una linea y se llama a la funcion que lo ejecuta */
      while (fgets (mensaje_enviar,1024, Entrada) != NULL){
        i = strlen (mensaje_enviar);
        
        /* se elimina el salto de linea del final de la linea */
        if (i > 3){
        mensaje_enviar[i - 1] = '\0';
        }
        
        /* se envia la linea a la funcion que la analiza para luego enviarla al servidor */
	verificar_mensaje (socket, mensaje_enviar);
      }
      printf ("\n");
    }
    /* en caso de error, se notifica al usuario */
    else
      printf ("\nERROR al abrir el archivo.\n");
  }