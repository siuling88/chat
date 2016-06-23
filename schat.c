/**
 **************************************************************************************
 *                                                                                    *
 *                                                                                    *
 *                             88                                                     *
 *                             88                         ,d                          *
 *                             88                         88                          *
 *      ,adPPYba,   ,adPPYba,  88,dPPYba,   ,adPPYYba,  MM88MMM        ,adPPYba,      *
 *      I8[    ""  a8"     ""  88P'    "8a  ""     `Y8    88          a8"     ""      *
 *       `"Y8ba,   8b          88       88  ,adPPPPP88    88          8b              *
 *      aa    ]8I  "8a,   ,aa  88       88  88,    ,88    88,    888  "8a,   ,aa      *  
 *      `"YbbdP"'   `"Ybbd8"'  88       88  `"8bbdP"Y8    "Y888  888   `"Ybbd8"'      *
 *                                                                                    *
 *                                                                                    *                                                                                       *
 **************************************************************************************
 ******-> A U T O R E S <-*******
 *                              *
 * Johanna Chan        08-10218 *
 * Carlos Rodriguez    06-40189 *
 *                              *
 ********************************    
 **/

#include "funciones.h"
#include "libSockets.h"
#define max_usuarios 20

/*
 * Tabla de hash que almacena la informacion referente a los usuarios.
 * Cada usuario posee una lista de salas a las que esta suscrito.
 */

  HASHTABLES * t_usuarios;

/*
 * Tabla de hash que almacena la informacion referente a las salas.
 * Cada sala posee una lista de usuarios que estan suscritos a ella.
 * En los nodos pila los descriptores no son necesarios asi que se almacena
 * el valor 0 en todos ellos. Los valores numSalas representan la cantidad
 * de usuarios asiciados y las listas salas representan listas de usuarios.
 */
  HASHTABLES * t_salas;

  void * manejar_cliente (void * arg);

  int main(int argc, char* argv[]){
    
    /* se inicializan las tablas de hash */
    createHashTable (&t_usuarios);
    createHashTable (&t_salas);

    int socket_servidor; 		/*socket que tiene como funcion recibir y encolar clientes*/
    struct sockaddr_in server ; 	/*Sirve solo para recibir y encolar clientes*/
    logo_servidor();	
    DatosServidor Datos = Verificacion_servidor(argc, argv);

    socket_servidor = socket(AF_INET,SOCK_STREAM,0);	/* Se procede a abrir el socket que retornara un entero y este sera nuestro 
							  * descriptor.
							  * utilizamos AF_INET para garantizar que el cliente pueda ubicarse en otro 
							  * ordenador.
							  * SOCK_STREAM indica que es un socket orientado a conexion.
							  *  0 corresponde al protocolo a utilizar, usualmente se utiliza 0.
							  */

    if(socket_servidor == -1){	/*Se verifica que la creacion del socket sea exitosa.*/
      printf("Error\n");
      exit(1);   			/*y sale del programa*/
    }					/*Si el socket se creo sin ningun problema, podemos pasar a hacer la asociacion entre 
					 *el socket y la direccion del servidor*/

    server.sin_family = AF_INET; 		/*Se especifica la familia del protocolo (tipo de conexion).*/
    server.sin_addr.s_addr = INADDR_ANY;	/*Direccion que atendera el servidor, con INADDR_ANY especifico que puede ser 
						 *cualquier direccion, proveniente de cualquier cliente.*/ 
    server.sin_port=htons(Datos.puerto);  	/*Asignacion del puerto, el especificado en la entrada estandar. Sera 20810*/

    if(bind(socket_servidor,(struct sockaddr *)&server,sizeof(server)) == -1){	/*Asociacion del socket a la direccion del servidor.*/
      printf("Error\n");								/*Ocurrio un error haciendo la asociacion entre el 
										  *socket y la direccion del servidor.*/
      exit(1);
    }

  
    if (listen (socket_servidor, 1) == -1) { 				/*Una vez hecha la asociacion procedemos a hacer la asociacion 
									 *se procede a avisarle al sistema que puede comenzar a 
									 *escuchar peticiones.*/ 
    printf ("Error\n");							/*Ocurrio un error*/
    }
    
    /* se anhade la sala indicada como parametro de entrada */
    add (Datos.sala, 0, 0, NULL, t_salas);
    
    pthread_t hilo[max_usuarios];
    int socket_cliente[max_usuarios * 2];
    int num_sockets = 0;
    Nombre_sala n[max_usuarios];
    Cabecera admitido[max_usuarios];
    
    /* mientras hayan entrado menos de max_usuarios usuarios, se siguen recibiendo solicitudes de conexion */
    while (num_sockets < max_usuarios)
    {
      /* se inician los sockets */
      socket_cliente[num_sockets] = accept(socket_servidor, 0, 0);         /*Se acepta la peticion, se genera un nuevo socket 
									    *que es por el cual el servidor "hablara" con el cliente.*/
      if (socket_cliente[num_sockets]  == -1) 
	printf ("Error\n"); 
      socket_cliente[num_sockets + max_usuarios] = accept(socket_servidor, 0, 0);         /*Se acepta la peticion, se genera un nuevo socket 
											   *que es por el cual el servidor "hablara" con el cliente.*/
      if (socket_cliente[num_sockets+ max_usuarios]  == -1) 
	printf ("Error\n"); 
      
      /* se recibe el nombre que utilizara el cliente */
      read (socket_cliente[num_sockets], &n[num_sockets], sizeof(Nombre_sala));
      int no_repetido = add (n[num_sockets].nombre, socket_cliente[num_sockets], socket_cliente[num_sockets + max_usuarios], Datos.sala, t_usuarios);
      
      /* si no hay otro usuario con ese nombre, se le informa que ha sido aceptado */
      if (no_repetido != FALSE)
      {
	subAdd (Datos.sala, n[num_sockets].nombre, t_salas);
	admitido[num_sockets].id = TRUE;
	write (socket_cliente[num_sockets], &admitido[num_sockets], sizeof(Cabecera));
	int ch;
	NODOPILA * aux = buscar (n[num_sockets].nombre, t_usuarios);
	if( (ch = pthread_create( &hilo[num_sockets], NULL, &manejar_cliente, (void * ) aux)))
	  printf("ERROR AL CREAR HILO\n");
	num_sockets++;
      }
      else
      {
	admitido[num_sockets].id = FALSE;
	write (socket_cliente[num_sockets], &admitido[num_sockets], sizeof(Cabecera));
	close (socket_cliente[num_sockets]);
      }
    }
    return 1;
  }

  
/*
 * Hilo que se encarga de la comunicacion con un cliente.
 * 
 * @param arg NODOPILA donde se encuentra la informacion del cliente.
 * 
 */

  void * manejar_cliente (void * arg) {
    NODOPILA * aux = (NODOPILA *) arg;
    int socket_cliente = aux->desc;
    char * nombre = aux->name;
    int i;
    do
    {
      i = servidor_recibe_mensaje (socket_cliente, t_usuarios, nombre, t_salas);
    }while (i);
    pthread_exit (NULL);
  }













