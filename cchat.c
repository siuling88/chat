/**
 *****************************************************************************************
 *                                                                                       *
 *                                                                                       *
 *                                88                                                     *
 *                                88                         ,d                          *
 *                                88                         88                          *
 *        ,adPPYba,   ,adPPYba,  88,dPPYba,   ,adPPYYba,  MM88MMM        ,adPPYba,       *
 *       a8"     ""  a8"     ""  88P'    "8a  ""     `Y8    88          a8"     ""       *  
 *       8b          8b          88       88  ,adPPPPP88    88          8b               *
 *       "8a,   ,aa  "8a,   ,aa  88       88  88,    ,88    88,    888  "8a,   ,aa       *
 *        `"Ybbd8"'   `"Ybbd8"'  88       88  `"8bbdP"Y8    "Y888  888   `"Ybbd8"'       *
 *                                                                                       *
 *                                                                                       *
 *                                                                                       *
 *****************************************************************************************
 ******-> A U T O R E S <-*******
 *                              *
 * Johanna Chan        08-10218 *
 * Carlos Rodriguez    06-40189 *
 *                              *
 ********************************    
 **/

#include "funciones.h"
#include "libSockets.h"

void * recibir (void * arg);

int main(int argc, char* argv[]){

  struct sockaddr_in server ; 				/*Sirve solo para recibir y encolar clientes*/
  int socket_cliente; 					/*Sirve para comunicarse con el servidor*/
  int socket_cliente2;
  struct sockaddr_in server2 ;                           /*Sirve solo para recibir y encolar clientes*/
  char buffer[1024];
  memset( buffer, 0, 1024*sizeof(char) );		/*Se inicializa el buffer*/
  
  logo_cliente();	
  DatosCliente Datos = Verificacion_cliente(argc, argv);

  socket_cliente = socket(AF_INET,SOCK_STREAM,0);	/* procedemos a abrir el socket que retornara un entero y este sera 
							 * nuestro descriptor.
							 * utilizamos AF_INET para garantizar que el cliente pueda ubicarse 
							 * en otro ordenador.
							 * SOCK_STREAM indica que es un socket orientado a conexion.
							 * 0 corresponde al protocolo a utilizar, usualmente se utiliza 0.*/ 

  if(socket_cliente == -1){  				/* Se verificamos que la creacion del socket sea exitosa.*/
    printf("Error creando socket\n");
    exit(1);   						/* y sale del programa*/
  }							/* Si el socket se creo sin ningun problema, podemos pasar a hacer la 
							 * asociacion entre el socket y la direccion del servidor*/

  server.sin_family = AF_INET; 				/* Se especifica la familia del protocolo (tipo de conexion).*/
  server.sin_addr.s_addr=inet_addr("127.0.0.1"); 	/* Direccion que atendera el servidor, con INADDR_ANY especifico que 
							 * puede ser cualquier direccion, proveniente de cualquier cliente.*/ 
  server.sin_port=htons(Datos.puerto);  		/* Asignacion del puerto, el especificado en la entrada estandar. Sera 20810*/



  if(connect(socket_cliente,(struct sockaddr *)&server,sizeof(server)) == -1){	/*Asociacion del socket a la direccion del servidor.*/
    printf("Error asociando\n");			/* Ocurrio un error haciendo la asociacion entre el socket y la direccion del 
							 * servidor.*/
    exit(1);
  }
  
  socket_cliente2 = socket(AF_INET,SOCK_STREAM,0);       /* procedemos a abrir el socket que retornara un entero y este sera 
                                                         * nuestro descriptor.
                                                         * utilizamos AF_INET para garantizar que el cliente pueda ubicarse 
                                                         * en otro ordenador.
                                                         * SOCK_STREAM indica que es un socket orientado a conexion.
                                                         * 0 corresponde al protocolo a utilizar, usualmente se utiliza 0.*/ 

                                                         
                                                         /* Se crea un segundo socket para que reciba mensajes de otros usuarios a traves del servidor */
  if(socket_cliente2 == -1){                             /* Se verificamos que la creacion del socket sea exitosa.*/
    printf("Error creando socket\n");
    exit(1);                                            /* y sale del programa*/
  }                                                     /* Si el socket se creo sin ningun problema, podemos pasar a hacer la 
                                                         * asociacion entre el socket y la direccion del servidor*/

  server2.sin_family = AF_INET;                          /* Se especifica la familia del protocolo (tipo de conexion).*/
  server2.sin_addr.s_addr=inet_addr("127.0.0.1");        /* Direccion que atendera el servidor, con INADDR_ANY especifico que 
                                                          * puede ser cualquier direccion, proveniente de cualquier cliente.*/ 
  server2.sin_port=htons(Datos.puerto);                  /* Asignacion del puerto, el especificado en la entrada estandar. Sera 20810*/



  if(connect(socket_cliente2,(struct sockaddr *)&server2,sizeof(server)) == -1){  /*Asociacion del socket a la direccion del servidor.*/
    printf("Error asociando\n");                        /* Ocurrio un error haciendo la asociacion entre el socket y la direccion del 
                                                         * servidor.*/
    exit(1);
  }
  
  /* se envia el nombre de usuario al servidor */
  enviar_mi_nombre (socket_cliente, Datos.nombre);

  /* se crea el hilo que se encargara de recibir mensajes del servidor enviados por otros usuarios */
  pthread_t hilo_recibir;
  int ch;
  if( (ch = pthread_create( &hilo_recibir, NULL, &recibir, &socket_cliente2)))
                        printf("ERROR INTERNO\n");
  
  /* se leen las instrucciones contenidas en el archivo */
  leer_archivo (socket_cliente, Datos.archivo);
  
  /* se reciben mensajes por la entrada estandar y se envian las instrucciones correspondientes al servidor */
  char mensaje_enviar[1024];
  while (TRUE)
  {
    gets (mensaje_enviar);
    verificar_mensaje (socket_cliente, mensaje_enviar);
  }
  
    return 1;
    
}

/*
 * Hilo que recibe mesajes del servidor.
 * 
 * @param arg Puntero al socket a traves del cual se recibiran los mensajes */
void * recibir (void * arg)
{
  int * socket = (int *) arg;
  Manda_mensaje m;
  int i;
  
  /* se reciben los mensajes que envie el servidor */
  do
  {
    i = read (*(socket), &m, sizeof(Manda_mensaje));
    printf ("%s%s%s@%s%s -> %s%s\n\n", KBLU, m.nombre, KCYN, KGRN,  m.sala, KWHT, m.mensaje);
  }while (i);
  pthread_exit (NULL);
}