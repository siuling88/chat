/**
 *******************************************************************************************************************************
 *                                                                                                                             *
 *                                                                                                                             *
 *      88                                  88                               88           88                                   *
 *      88                                  88            ,d                 88           88                                   *
 *      88                                  88            88                 88           88                                   *
 *      88,dPPYba,   ,adPPYYba,  ,adPPYba,  88,dPPYba,  MM88MMM  ,adPPYYba,  88,dPPYba,   88   ,adPPYba,        ,adPPYba,      *
 *      88P'    "8a  ""     `Y8  I8[    ""  88P'    "8a   88     ""     `Y8  88P'    "8a  88  a8P_____88       a8"     ""      *
 *      88       88  ,adPPPPP88   `"Y8ba,   88       88   88     ,adPPPPP88  88       d8  88  8PP"""""""       8b              *
 *      88       88  88,    ,88  aa    ]8I  88       88   88,    88,    ,88  88b,   ,a8"  88  "8b,   ,aa  888  "8a,   ,aa      *
 *      88       88  `"8bbdP"Y8  `"YbbdP"'  88       88   "Y888  `"8bbdP"Y8  8Y"Ybbd8"'   88   `"Ybbd8"'  888   `"Ybbd8"'      *  
 *                                                                                                                             *
 *                                                                                                                             *
 *******************************************************************************************************************************
 ******-> A U T O R E S <-*******
 *                              *
 * Johanna Chan        08-10218 *
 * Carlos Rodriguez    06-40189 *
 *                              *
 ********************************    
 **/

#include "hashtable.h"

void liberarPila (NODOPILA * n);

/* 
 * Inicializa una tabla de hash
 * 
 * @param t apuntador al apuntador de la tabla de hash
 * @return TRUE si puede inicializar la tabla, FALSE en caso contrario.
 * 
 */

  int createHashTable (HASHTABLES ** t) {
    if (!((*t) = (HASHTABLES *) malloc (sizeof (HASHTABLES))))
      return FALSE;
    int i;
    for (i = 0; i < TAMHASH; i++)
    {
      PILA * p;
      if (!(p = (PILA *) malloc (sizeof (PILA))))
	return FALSE;
      NODOPILA * inicio;
      if (!(inicio = (NODOPILA *) malloc (sizeof (NODOPILA))))
	return FALSE;
      p->inic = inicio;
      p->tam = 0;
      (*t)->tabla[i] = p;
    }
    (*t)->tam = 0;
    return TRUE;
  }

  
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

  int add (char * name, int desc, int desc2, char * sala, HASHTABLES * t) {
    /* se verifica que no haya otro cliente con el mismo nombre */
    NODOPILA * n = buscar (name, t);
    if (n != NULL)
      return FALSE;
    
    /* Se crea un nodo y se llena con la informacion necesaria */
    NODOPILA * aux;
    if (!(aux = (NODOPILA *) malloc(sizeof(NODOPILA))))
      return FALSE;
    aux->name = name;
    aux->desc = desc;
    aux->desc2 = desc2;
    if (sala != NULL)
    {
      aux->numSalas = 1;
      
      /* se crea un nodo para la subpila y se llena con la informacion necesaria */
      NODOSUBPILA * aux2;
      if (!(aux2 = (NODOSUBPILA *) malloc (sizeof (NODOSUBPILA))))
	return FALSE;
      
      /* se asocia el sobnodo al nodo antes creado */
      aux2->nombre = sala;
      aux2->sig= aux->salas;
      aux->salas = aux2;
    }
    else
    {
      aux->numSalas = 0;
      aux->salas = NULL;
    }
    
    /* se busca la posicion de la tabla donde ira el cliente */
    int h = hashcode (name) % TAMHASH;
    
    /* se asocia el nodo con la informacion del cliente con la tabla */
    aux->sig = t->tabla[h]->inic->sig;
    t->tabla[h]->inic->sig = aux;
    t->tabla[h]->tam++;
    t->tam++;
    return TRUE;
  }

  
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

  int subAdd (char * name, char * sala, HASHTABLES * t) {
    /* se busca el cliente al que sera asociada la sala */
    NODOPILA * n = buscar (name, t);
    if (n == NULL)
      return FALSE;
    
    /* se verifica que no haya una sala con el mismo nombre */
    NODOSUBPILA * s = subbuscar (n, sala);
    if (s != NULL)
      return FALSE;
    
    /* se crea un subnodo donde se carga la informacion de la sala */
    NODOSUBPILA * aux;
    if (!(aux = (NODOSUBPILA *) malloc(sizeof(NODOSUBPILA))))
      return FALSE;
    
    /* se asocia el subnodo al cliente */
    aux->nombre = sala;
    aux->sig = n->salas;
    n->salas = aux;
    n->numSalas++;
    return TRUE;
  }

  
/* 
 * Elimina a un cliente de la tabla de hash.
 * 
 * @param name Nombre del cliente.
 * @param t Apuntador a la tabla de hash.
 * 
 * @return TRUE si logra eliminarlo. FALSE si no pertenecia a la tabla.
 * 
 */

  int del (char * name, HASHTABLES * t) {
    /* si la tabla esta vacia, se retorna FALSE */
    if (t->tam == 0)
      return FALSE;
    
    /* se busca la posicion de la tabla donde puede estar el cliente */
    int h = hashcode(name) % TAMHASH;
    int i = t->tabla[h]->tam;
    if (i == 0)
      return FALSE;
    NODOPILA * aux = t->tabla[h]->inic;
    int j = 1;
    
    /* se revisa la lista para buscar al cliente */
    while ((j <= i) && strcmp (name, aux->sig->name) != 0)
    {
      aux = aux->sig;
      j++;
    }
    if (j > i)
      return FALSE;
    /* si se consigue se libera su lista de salas asociadas */
    liberarPila (aux->sig);
    NODOPILA * aux2 = aux->sig;
    aux->sig = aux2->sig;
    /* se libera la memoria */
    free (aux2);
    t->tabla[h]->tam--;
    t->tam--;
    return TRUE;
  }

  
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

  int subDel (char * name, char * sala, HASHTABLES * t) {
    /* se busca el cliente al que esta asociada la sala */
    NODOPILA * n = buscar (name, t);
    if (n == NULL)
      return FALSE;
    if (n->numSalas == 0)
      return FALSE;
    
    /* si la sala esta de primera en la lista de salas asociadas, se elimina facilmente */
    NODOSUBPILA * aux = n->salas;
    if (strcmp (sala, aux->nombre) == 0)
    {
      n->salas = aux->sig;
      free (aux);
      n->numSalas--;
      return TRUE;
    }
    int i = 1;
    
    /* se busca la sala en la lista de salas asociadas al cliente */
    while ((i < n->numSalas) && strcmp (aux->sig->nombre, sala) != 0)
    {
      i++;
      aux = aux->sig;
    }
    if (i == n->numSalas)
      return FALSE;
    NODOSUBPILA * aux2 = aux->sig;
    aux->sig = aux2->sig;
    
    /* se libera la memoria */
    free(aux2);
    n->numSalas--;
    return TRUE;
  }

  
/*
 * Busca un cliente en la tabla de hash.
 * 
 * @param n Nombre del cliente.
 * @param t apuntador a la tabla de hash.
 * 
 * @return El nodo asociado al cliente.
 * 
 */

  NODOPILA* buscar (char * n, HASHTABLES * t) {
    if (t->tam == 0)
      return NULL;
    
    /* se busca la posicion donde debe estar el cliente */
    int h = hashcode(n) % TAMHASH;
    int i = t->tabla[h]->tam;
    if (i > 0)
    {
      int j = 1;
      NODOPILA * aux = t->tabla[h]->inic->sig;
      
      /* se busca al cliente */
      while ((j <= i) && (strcmp (n, aux->name) != 0))
      {
	aux = aux->sig;
	j++;
      }
      
      /* si no se consigue se retorna NULL */
      if (j > i)
	return NULL;
      else
	return aux;
    }
    return NULL;
  }

  
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

  int hashcode (char * c) {
      int l = strlen (c);
      int h = 0;
      int i;
      for (i =  0; i < l; i++)
      {
	h+= *(c + i) * (l - i);
      }
      return h;
  }

  
/*
 * Busca una sala asociada a un cliente.
 * 
 * @param n Nodo asociado al cliente en una tabla de hash.
 * @param s Nombre de la sala a buscar.
 * 
 * @return Nodo que asocia la sala al cliente en una tabla de hash.
 * 
 */

  NODOSUBPILA* subbuscar (NODOPILA * n, char * s) {
      if (n->numSalas == 0)
	  return NULL;
      int i = n->numSalas;
      int j = 1;
      NODOSUBPILA * aux = n->salas;
      
      /* se busca la sala en la lista de salas asociada al cliente */
      while ((j <= i) && strcmp (s, aux->nombre))
      {
	  aux = aux->sig;
	  j++;
      }
      
      /* si no se consigue se retorna NULL */
      if (j > i)
	  return NULL;
      else
	  return aux;
  }

  
/*
 * Libera un nodo de una pila perteneciente a una tabla de hash.
 * 
 * @param n Nodo a liberar.
 * 
 */

  void liberarPila (NODOPILA * n) {
      int i = n->numSalas;
      if (i == 1)
	  free (n->salas);
      else if (i > 1)
      {
	  int j = 2;
	  NODOSUBPILA * aux = n->salas->sig;
	  NODOSUBPILA * aux2 = n->salas;
	  while (j < i)
	  {
	      free (aux2);
	      aux2 = aux;
	      aux = aux->sig;
	  }
	  free(aux2);
	  free (aux);
	  n->numSalas = 0;
      }
  }