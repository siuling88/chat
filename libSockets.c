/**
 * ********************************************************************************************************************************
 *                                                                                                                                *                                                                                                                        
 *                                                                                                                                *
 *      88  88  88            ad88888ba                            88                                                             *
 *      88  ""  88           d8"     "8b                           88                      ,d                                     *
 *      88      88           Y8,                                   88                      88                                     *
 *      88  88  88,dPPYba,   `Y8aaaaa,     ,adPPYba,    ,adPPYba,  88   ,d8   ,adPPYba,  MM88MMM  ,adPPYba,        ,adPPYba,      *
 *      88  88  88P'    "8a    `"""""8b,  a8"     "8a  a8"     ""  88 ,a8"   a8P_____88    88     I8[    ""       a8"     ""      *  
 *      88  88  88       d8          `8b  8b       d8  8b          8888[     8PP"""""""    88      `"Y8ba,        8b              *
 *      88  88  88b,   ,a8"  Y8a     a8P  "8a,   ,a8"  "8a,   ,aa  88`"Yba,  "8b,   ,aa    88,    aa    ]8I  888  "8a,   ,aa      *
 *      88  88  8Y"Ybbd8"'    "Y88888P"    `"YbbdP"'    `"Ybbd8"'  88   `Y8a  `"Ybbd8"'    "Y888  `"YbbdP"'  888   `"Ybbd8"'      *
 *                                                                                                                                *
 *                                                                                                                                *
 **********************************************************************************************************************************                                                                                                                                                                                                                                                                                                                                                 *
 *                                                                                                                                *
 * CONTENIDO : Archivo de cabecera de la libreria libSockets. Implementa distintas funciones a utilizar por cchat y schat         *
 *                                                                                                                                *
 ******-> A U T O R E S <-*********************************************************************************************************
 *                              *
 * Johanna Chan        08-10218 *
 * Carlos Rodriguez    06-40189 *
 *                              *
 ********************************    
 **/  

#include "libSockets.h"

/*
 * Envia el nombre de un cliente a traves de un socket.
 * 
 * @param socket Socket a traves del cual se enviara el nombre.
 * @param nombre Nombre del cliente.
 * 
 */

  void enviar_mi_nombre (int socket, char * nombre) {
    /* se declara y rellena la estructura que se utilizara para enviar el nombre */
    Nombre_sala n;
    strcpy (n.nombre, nombre);
    
    /* se envia la informacion */
    write (socket, &n, sizeof(Nombre_sala));
    Cabecera i;
    
    /* se lee la respuesta del servidor */
    read (socket, &i, sizeof(Cabecera));
    
    /* si ya existe un cliente con el mismo nombre, se notifica al usuario y se cierra el programa */
    if (i.id == FALSE)
    {
      printf ("\nERROR: ya existe un usuario con el mismo nombre conectado al servidor\n");
      close (socket);
      exit (0);
    }
  }

  
/*
 * Envia una instruccion al servidor.
 * La accion a ejecutar depende
 * de la instruccion que se haya ingresado.
 * 
 * @param socket Socket a traves del cual se enviara la instruccion al servidor.
 * @param id Identificador del tipo de instruccion.
 * @param msj Mensaje que complementa la instruccion.
 * @param tam Tamanho del mensaje.
 * 
 */

  void cliente_escribe_mensaje (int socket, int id, char * msj, int tam) {  
    /* Se declara y rellena la cabecera */
    Cabecera cab;
    cab.id = id;
    
    /* Se envia la cabecera */
    write (socket, &cab, sizeof(cab));

    /* se realiza la opcion necesaria */
    switch (id)
    {
      case Id_sal:
	cliente_recibe_lista (0, socket);
	break;
    
      case Id_usu:
	cliente_recibe_lista (1, socket);
	break;
      
      /* en estos casos solo seenvia el complemento de la instruccion.El resto es hecho por el servidor */
      case Id_men:
      case Id_sus:
      case Id_cre:
      case Id_eli:
	write (socket, msj, tam);
	break;
    
      case Id_des:
	break;
    
      case Id_fue:
	
	/* si el usuario introdujo fue, se cierra el socket y la aplicacion */
	close (socket);
	exit (0);
    }
    
    /* se espera a que el servidor termine de interpretar la instruccion */
    read (socket, &cab, sizeof (Cabecera));
  }

  
/*
 * Envia un mensaje a todos los usuarios que pertenezcan a alguna
 * sala a la que pertenece el usuario que envia el mensaje.
 * 
 * @param m Estructura con informacion necesaria acerca del mensaje.
 * @param t_usuarios Tabla de hash que contiene a los usuarios.
 * @param t_salas Tabla de hash que contiene a las salas.
 * @param name Nombre del cliente que envia el mensaje.
 * 
 */

  void enviar_mensaje (Manda_mensaje m, HASHTABLES * t_usuarios, HASHTABLES * t_salas, char * name) {
    /* se busca al cliente para acceder a las salas a las que esta suscrito */
    NODOPILA * aux = buscar (name, t_usuarios);
    NODOSUBPILA * aux2 = aux->salas;
    NODOPILA * usuario;
    NODOPILA * auxx;
    NODOSUBPILA * auxx2;
    int i, j;
    
    /* para cada sala se busca la lista de usuarios suscritos a ella */
    for (i = 0; i < aux->numSalas; i++)
    {
      auxx = buscar (aux2->nombre, t_salas);
      strcpy (m.sala, auxx->name);
      auxx2 = auxx->salas;
      
      /* se envia el mensaje a cada usuario perteneciente a la sala */
      for (j = 0; j < auxx->numSalas; j++)
      {
	usuario = buscar (auxx2->nombre, t_usuarios);
	write (usuario->desc2, &m, sizeof(Manda_mensaje));
	auxx2 = auxx2->sig;
      }
      aux2 = aux2->sig;
    }
  }

/*
 * Recibe una instruccion de un cliente y decide que operacion debe realizar
 * en funcion de la instruccion recibida.
 * 
 * @param socket Socket a traves del cual se comunica el servidor con el cliente.
 * @param tU Tabla de hash de los usuarios.
 * @param name Nombre del usuario que envia la instruccion.
 * @param tS Tabla de hash de las salas.
 * 
 * @return TRUE si logra realizar su funcion. FALSE en caso contrario.
 * 
 */

  int servidor_recibe_mensaje (int socket, HASHTABLES * tU, char * name, HASHTABLES * tS) {
    Cabecera cab;
    /* se lee la cabecera de la instruccion (el tipo de instruccion) */
    read (socket, &cab, sizeof (Cabecera));
    int i;
    
    /* se decide que operacion realizar en funcion de la instruccion que desea ejecutar el cliente */
    switch (cab.id)
    {

      /* caso instruccion sal */
      case Id_sal:
      {
	servidor_envia_lista (socket, tS);
	break;
      }

      /* caso instruccion usu */
      case Id_usu:
      {
	servidor_envia_lista (socket, tU);
	break;
      }

      /* caso instruccion men */
      case Id_men:
      {
	Nombre_sala s;
	memset (s.nombre, 0, 200);
	read (socket, &s, sizeof (Nombre_sala));
	Manda_mensaje m;
	strcpy (m.nombre, name);
	strcpy(m.mensaje, s.nombre);
	enviar_mensaje (m, tU, tS, name);
	break;
      }
	
      /* caso instruccion sus */
      case Id_sus:
      {
	Nombre_sala s;
	memset (s.nombre, 0, 200);
	read (socket, &s, sizeof (Nombre_sala));
	char * c = strdup (s.nombre);
	i = subAdd (name, c, tU);
	if (i == TRUE)
	  subAdd (s.nombre, name, tS);
	break;
      }
      
      /* caso instruccion des */
      case Id_des:
      {
	NODOPILA * aux = buscar (name, tU);
	NODOSUBPILA * aux2;
	while (aux->numSalas > 0)
	{
	  aux2 = aux->salas;
	  i = subDel (aux2->nombre, name, tS);
	  if (i == TRUE)
	    subDel (name, aux2->nombre, tU);
	}
	break;
      }
      
      /* caso instruccion cre */
      case Id_cre:
      {
	Nombre_sala s;
	memset (s.nombre, 0, 200);
	read (socket, &s, sizeof (Nombre_sala));
	char * c = strdup (s.nombre);
	add (c, 0, 0, NULL, tS);
	break;
      }
      
      /* caso instruccion eli */
      case Id_eli:
      {
	Nombre_sala s;
	memset (s.nombre, 0, 200);
	read (socket, &s, sizeof (Nombre_sala));
	char * c = strdup (s.nombre);
	NODOPILA * aux = buscar (c, tS);
	if (aux != NULL)
	{
	  NODOSUBPILA * aux2;
	  while (aux->numSalas > 0)
	  {
	    aux2 = aux->salas;
	    subDel (aux2->nombre, c, tU);
	    subDel (c, aux2->nombre, tS);
	  }
	  del (c, tS);
	}
	break;
      }
      
      /* caso instruccion fue */
      case Id_fue:
      {
	NODOPILA * aux = buscar (name, tU);
	NODOSUBPILA * aux2;
	while (aux->numSalas > 0)
	{
	  aux2 = aux->salas;
	  subDel (aux2->nombre, name, tS);
	  subDel (name, aux2->nombre, tU);
	}
	del (name, tU);
	close (socket);
	return 0;
      }
    }
    
    /* se informa al cliente que termino de interpretar la instruccion */
    write (socket, &cab, sizeof(Cabecera));
    return FALSE;
  }

  
/*
 * Recibe la lista de salas o usuarios segun sea el caso.
 * Se utiliza luego de que el cliente ejecuta la instruccion sal o usu.
 * 
 * @param id Identificador de la instruccion utilizada. 0 para sal, 1 para usu.
 * @param socket Socket a traves del cual el cliente se comunica con el servidor.
 * 
 */

  void cliente_recibe_lista (int id, int socket) {
    Cant_usu_sala cant;
    Nombre_sala nombre;
    
    /* Se lee cantidad de usuarios / salas a recibir */
    read (socket, &cant, sizeof (Cant_usu_sala));
    int i;
    if (id == 0)
      printf ("Salas:\n");
    else
      printf ("Usuarios:\n");
    
    /* se recibe el nombre de cada usuario / sala en el servidor */ 
    for (i = 0; i < cant.cantidad; i++)
    {
      strcpy (nombre.nombre, "");
      read (socket, &nombre, sizeof (Nombre_sala));
      printf (" %s", nombre.nombre);
    }
    printf ("\n\n");
  }

  
/*
 * Envia todos los usuarios / salas pertenecientes al servidor.
 * Inicialmente no sabe si envia salas o usuarios ya que esa
 * decision fue tomada por la funcion servidor_recibe_mensaje.
 * 
 * @param socket Socket a traves del cual el servidor se comunicara con el cliente.
 * @param t Tabla de hash que contiene los usuarios / salas.
 * 
 */

  void servidor_envia_lista (int socket, HASHTABLES * t) {
    Cant_usu_sala cant;
    cant.cantidad = t->tam;
    write (socket, &cant, sizeof (Cant_usu_sala));
    NODOPILA * aux;
    Nombre_sala nsala;
    int i, j;
    
    /* para cada lista de sala se envian los usuarios / salas que contiene */
    for (i = 0; i < TAMHASH; i++)
    {
      if (t->tabla[i]->tam > 0)
      {
	aux = t->tabla[i]->inic->sig;
	for (j = t->tabla[i]->tam; j > 0; j--)
	{
	  strcpy (nsala.nombre, aux->name);
	  write (socket, &nsala, sizeof(Nombre_sala));
	  aux = aux->sig;
	}
      }
    }
  }