/**
 * ********************************************************************************************************************************
 *                                                                                                                                *                                                                                                                        
 *                                                                                                                                *
 *      88  88  88            ad88888ba                            88                                             88              *
 *      88  ""  88           d8"     "8b                           88                      ,d                     88              *
 *      88      88           Y8,                                   88                      88                     88              *
 *      88  88  88,dPPYba,   `Y8aaaaa,     ,adPPYba,    ,adPPYba,  88   ,d8   ,adPPYba,  MM88MMM  ,adPPYba,       88,dPPYba,      * 
 *      88  88  88P'    "8a    `"""""8b,  a8"     "8a  a8"     ""  88 ,a8"   a8P_____88    88     I8[    ""       88P'    "8a     *  
 *      88  88  88       d8          `8b  8b       d8  8b          8888[     8PP"""""""    88      `"Y8ba,        88       88     *
 *      88  88  88b,   ,a8"  Y8a     a8P  "8a,   ,a8"  "8a,   ,aa  88`"Yba,  "8b,   ,aa    88,    aa    ]8I  888  88       88     *
 *      88  88  8Y"Ybbd8"'    "Y88888P"    `"YbbdP"'    `"Ybbd8"'  88   `Y8a  `"Ybbd8"'    "Y888  `"YbbdP"'  888  88       88     *
 *                                                                                                                                *
 *                                                                                                                                *
 **********************************************************************************************************************************                                                                                                                     
 ******-> A U T O R E S <-*******
 *                              *
 * Johanna Chan        08-10218 *
 * Carlos Rodriguez    06-40189 *
 *                              *
 ********************************    
 **/  

#ifndef LIBSOCKETS
#define LIBSOCKETS

#include "hashtable.h"

/*
 * Estructura que almacena un entero
 * que representa un identificador de
 * alguna instruccion.
 * 
 */

  typedef struct t_cabecera {
    int id;
  }Cabecera;

  
/*
 * Estructura que contiene la informacion
 * asociada a un mensaje, como el nombre
 * del cliente que lo envia, la sala a la
 * que pertenece y el contenido del mensaje.
 * 
 */

  typedef struct t_manda_mensaje {
    char mensaje [1024];
    char nombre [200];
    char sala [200];
  }Manda_mensaje;

  
/*
 * Estructura que contiene una cadena de
 * caracteres asociada al nombre de una sala
 * o usuario.
 * 
 */

  typedef struct t_sala {
    char nombre [200];
  }Nombre_sala;

  
/*
 * Estructura que contiene un entero
 * que representa la cantidad de usuarios /
 * salas pertenecientes a un servidor.
 * 
 */

  typedef struct t_cantidad_usu_sala {
    int cantidad;
  }Cant_usu_sala;

  
/*
 * Lista que enumera el identificador de
 * una instruccion envia por un cliente
 * a un servidor.
 * 
 */

  typedef enum identificadores {
    Id_sal,
    Id_usu,
    Id_men,
    Id_sus,
    Id_des,
    Id_cre,
    Id_eli,
    Id_fue
  }Identificadores;

  
/*
 * Envia el nombre de un cliente a traves de un socket.
 * 
 * @param socket Socket a traves del cual se enviara el nombre.
 * @param nombre Nombre del cliente.
 * 
 */

  extern void enviar_mi_nombre (int socket, char * nombre);

/*
 * Envia una instruccion al servidor.
 * La accion a ejecutar depende
 * de la instruccion que se haya ingresado.
 * 
 * @param socket Socket a traves del cual se enviara la instruccion al servidor.
 * @param id Identificador del tipo de instruccion.
 * @param msj Mensaje que complementa la instruccion.
 * @param tam Tamanho del mensaje.
 */

  extern void cliente_escribe_mensaje (int socket, int id, char * msj, int tam);

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

  extern int servidor_recibe_mensaje (int socket, HASHTABLES * tU, char * name, HASHTABLES * tS);

/*
 * Recibe la lista de salas o usuarios segun sea el caso.
 * Se utiliza luego de que el cliente ejecuta la instruccion sal o usu.
 * 
 * @param id Identificador de la instruccion utilizada. 0 para sal, 1 para usu.
 * @param socket Socket a traves del cual el cliente se comunica con el servidor.
 * 
 */

  extern void cliente_recibe_lista (int id, int socket);

/*
 * Envia todos los usuarios / salas pertenecientes al servidor.
 * Inicialmente no sabe si envia salas o usuarios ya que esa
 * decision fue tomada por la funcion servidor_recibe_mensaje.
 * 
 * @param socket Socket a traves del cual el servidor se comunicara con el cliente.
 * @param t Tabla de hash que contiene los usuarios / salas.
 * 
 */

  extern void servidor_envia_lista (int socket, HASHTABLES * t);

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

  extern void enviar_mensaje (Manda_mensaje m, HASHTABLES * t_usuarios, HASHTABLES * t_salas, char * name);

#endif