/**
 * ******************************************************************************************************************************
 *                                                                                                                              *
 *                                                                                                                              *
 *      88                                  88                               88           88                   88               *
 *      88                                  88            ,d                 88           88                   88               *
 *      88                                  88            88                 88           88                   88               *
 *      88,dPPYba,   ,adPPYYba,  ,adPPYba,  88,dPPYba,  MM88MMM  ,adPPYYba,  88,dPPYba,   88   ,adPPYba,       88,dPPYba,       *
 *      88P'    "8a  ""     `Y8  I8[    ""  88P'    "8a   88     ""     `Y8  88P'    "8a  88  a8P_____88       88P'    "8a      *  
 *      88       88  ,adPPPPP88   `"Y8ba,   88       88   88     ,adPPPPP88  88       d8  88  8PP"""""""       88       88      *
 *      88       88  88,    ,88  aa    ]8I  88       88   88,    88,    ,88  88b,   ,a8"  88  "8b,   ,aa  888  88       88      *
 *      88       88  `"8bbdP"Y8  `"YbbdP"'  88       88   "Y888  `"8bbdP"Y8  8Y"Ybbd8"'   88   `"Ybbd8"'  888  88       88      *
 *                                                                                                                              *
 *                                                                                                                              *
 ********************************************************************************************************************************
 *                                                                                                                              *
 * CONTENIDO : Archivo de cabecera de la libreria hashtable. Implementa una tabla de hash con distintas funciones.              *
 *                                                                                                                              *
 ******-> A U T O R E S <-*******************************************************************************************************
 *                              *
 * Johanna Chan        08-10218 *
 * Carlos Rodriguez    06-40189 *
 *                              *
 ********************************    
 **/  

#ifndef HASHTABLE
#define HASHTABLE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define TRUE 1
#define FALSE -1
#define TAMHASH 7

/*
 * Nodo de la estructura subpila.
 * 
 * Contiene un apuntador al siguiente
 * elemento de una subpila y un apuntador
 * a una cadena de caracteres (nombre de la sala).
 */

  typedef struct t_nodoSubPila {
    char * nombre;
    struct t_nodoSubPila * sig;
  }NODOSUBPILA;

  
/*
 * Nodo de una pila.
 * 
 * Contiene un apuntador al siguiente elemento
 * de una pila, un apuntador a una subpila (salas
 * a las que esta subscrito el usurio),
 * un apuntador a una cadena de caracteres (nombre
 * del usuario) y un descriptor asociado al usuario.
 */

  typedef struct t_nodoPila {
    char * name;
    int desc;
    int desc2;
    int numSalas;
    struct t_nodoSubPila * salas;
    struct t_nodoPila * sig;
  }NODOPILA;

  
/*
 * Nodo inicial de una pila.
 * 
 * Contiene el numero de elementos en
 * dicha pila y un apuntador al comienzo
 * de la pila.
 */

  typedef struct t_pila {
    int tam;
    struct t_nodoPila * inic;
  }PILA;

  
/*
 * Tabla de hash.
 * 
 * Contiene el numero de elementos en la
 * tabla de hash y un arreglo de apuntadores
 * a pilas de tamanio TAMHASH. Este tamanio es
 * un numero primo para minimizar las "colisiones"
 * producidas en una tabla de hash.
 */

  typedef struct t_hashtable {
    struct t_pila * tabla[TAMHASH];
    int tam;
  }HASHTABLES;

  
/*
 * Inicializa una tabla de hash
 * 
 * @param t apuntador al apuntador de la tabla de hash
 * 
 * @return TRUE si puede inicializar la tabla, FALSE en caso contrario.
 * 
 */

  extern int createHashTable (HASHTABLES ** t);

/*
 * Agrega un cliente a la tabla de hash.
 * 
 * @param name Nombre del cliente.
 * @param desc Primer descriptor asociado.
 * @param desc2 Segundo descriptor asociado.
 * @param sala Sala a la que se asocia el cliente inicialmente. Puede ser NULL.
 * 
 * @return TRUE si logra finalizar, FALSE en caso contrario.
 * 
 */

  extern int add (char * name, int desc, int desc2, char * sala, HASHTABLES * t);

/*
 * Asocia una sala al cliente de nombre name.
 * 
 * @param name Nombre del cliente.
 * @param sala Nombre de la sala.
 * @param t Apuntador a la tabla de hash.
 * 
 * @return TRUE si logra finalizar, FALSE en caso contrario.
 * 
 */

  extern int subAdd (char * name, char * sala, HASHTABLES * t);

/*
 * Elimina a un cliente de la tabla de hash.
 * 
 * @param name Nombre del cliente.
 * @param t Apuntador a la tabla de hash.
 * 
 * @return TRUE si logra eliminarlo. FALSE si no pertenecia a la tabla.
 * 
 */

  extern int del (char * name, HASHTABLES * t);

/*
 * Elimina una sala asociada al cliente de nombre name.
 * 
 * @param name Nombre del cliente.
 * @param sala Nombre de la sala a eliminar.
 * @param t apuntador a la tabla de hash.
 * 
 * @return TRUE si logra eliminarlo. FALSE si la sala no esta asociada al cliente.
 * 
 */

  extern int subDel (char * name, char * sala, HASHTABLES * t);

/*
 * Crea una representacion numerica de una cadena de caracteres.
 * Multiplica el valor de cada caracter segun la tabla ascii
 * por la posicion que ocupa el caracter en la cadena.
 * 
 * @param c Cadena de caracteres a convertir a entero.
 * 
 * @return Representacion en entero de la cadena de caracteres.
 * 
 */

  extern int hashcode (char * n);

/*
 * Busca un cliente en la tabla de hash.
 * 
 * @param n Nombre del cliente.
 * @param t apuntador a la tabla de hash.
 * 
 * @return El nodo asociado al cliente.
 * 
 */
  
  extern NODOPILA* buscar (char * n, HASHTABLES * t);

/*
 * Busca una sala asociada a un cliente.
 * 
 * @param n Nodo asociado al cliente en una tabla de hash.
 * @param s Nombre de la sala a buscar.
 * 
 * @return Nodo que asocia la sala al cliente en una tabla de hash.
 * 
 */

  extern NODOSUBPILA* subbuscar (NODOPILA * n, char * s);

#endif
