#ifndef HASH_H
#define HASH_H

#include <stdbool.h>
#include <stddef.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* hash está planteado como un hash de punteros genéricos. */

struct hash;	// Definición completa en hash.c.
typedef struct hash hash_t;

/* hash_iter está planteado como un iterador del hash. */

struct hash_iter;	// Definición completa en hash.c.
typedef struct hash_iter hash_iter_t;

/* hash_destruir_dato_t está planteado como un puntero a una funcion que destruye los datos del hash. */

typedef void (*hash_destruir_dato_t)(void *);


/* ******************************************************************
 *                    	 PRIMITIVAS DEL HASH
 * *****************************************************************/

// Crea un hash.
// Post: Devuelve un nuevo hash vacío.
hash_t *hash_crear(hash_destruir_dato_t destruir_dato);

// Guarda un elemento en el hash, si la clave ya se encuentra en la
// estructura, la reemplaza. De no poder guardarlo devuelve false.
// Pre: El hash fue creado.
// Post: Se almacenó el par (clave, dato).
bool hash_guardar(hash_t *hash, const char *clave, void *dato);

// Borra un elemento del hash y devuelve el dato asociado. 
// Devuelve NULL si el dato no estaba.
// Pre: El hash fue creado.
// Post: El elemento fue borrado de la estructura y se lo devolvió, en el caso de que estuviera guardado.
void *hash_borrar(hash_t *hash, const char *clave);

// Obtiene el valor de un elemento del hash, si la clave no se encuentra
// devuelve NULL.
// Pre: El hash fue creado.
// Post: Obtiene el dato relacionado a la clave.
void *hash_obtener(const hash_t *hash, const char *clave);

// Determina si la clave pertenece o no al hash.
// Pre: El hash fue creado.
// Post: Devuelve true si la clave pertenece, false si no.
bool hash_pertenece(const hash_t *hash, const char *clave);

// Obtiene el valor de la cantidad de elementos del hash. Si el hash tiene elementos,
// se devuelve la cantidad total. Si está vacio devuelve 0.
// Pre: El hash fue creado.
// Post: Se devolvió la cantidad de elementos del hash.
size_t hash_cantidad(const hash_t *hash);

// Destruye el hash. Utiliza internamente la función hash_destruir_dato_t,
// para cada uno de los pares (clave, dato) del hash.
// Pre: El hash fue creado.
// Post: Se elimino el hash y todos sus elementos.
void hash_destruir(hash_t *hash);

/* *****************************************************************
 *                     PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

// Crea un iterador para el hash.
// Pre: Recibe un hash por parametro.
// Post: Devuelve un iterador con un nodo anterior y un nodo actual que apunta al primer elemento del hash.
hash_iter_t *hash_iter_crear(const hash_t *hash);

// Si el nodo actual del iterador es distinto de NULL,
// hace que el nodo anterior apunte al actual y que actual apunte al siguiente elemento del hash,
// devuelve true si pudo apuntar al siguiente, false en caso de que el actual sea NULL.
// Pre: El iterador fue creado.
// Post: Avanza en uno la iteracion del hash.
bool hash_iter_avanzar(hash_iter_t *iter);

// Devuelve el dato del elemento al que nodo actual esta apuntando,
// en caso de que el nodo actual sea NULL, devuelve NULL.
// Pre: EL iterador fue creado
// Post: Devuelve el elemento al que apunta el nodo actual.
const char *hash_iter_ver_actual(const hash_iter_t *iter);

// Se fija si el nodo actual del iterador apunta a NULL.
// Devuelve true si el nodo actual es NULL, false en caso contrario.
// Pre: El iterador fue creado.
// Post: Devuelve si el nodo actual es NULL o no.
bool hash_iter_al_final(const hash_iter_t *iter);

// Destruye el iterador del hash,
// no destruye el hash ni tampoco sus elementos.
// Pre: El iterador fue creado.
// Post: Destruye el iterador.
void hash_iter_destruir(hash_iter_t* iter);

#endif // HASH_H