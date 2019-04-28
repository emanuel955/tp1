#include <stdbool.h>
#include <stddef.h>

/* Definición del struct nodo_hash. */

typedef struct nodo_hash{
	char* clave;
	void* dato;
	int estado;
} nodo_hash_t;

/* Definición del struct hash. */

struct hash{
	size_t cantidad;
	size_t largo;
	size_t carga;
	noto_t* tabla;
	hash_destruir_dato_t destruir_dato;
};

/* Definición del struct hash_iter. */

struct hash_iter{

};

/* Tipo de función para destruir dato. */

/*
Guardar
Se debe guardar una copia de la clave!
En el abierto se inserta en la lista correspondiente al índice obtenido con la función de hashing.
En el cerrado se busca a partir del índice obtenido con la función de hashing una posición que esté libre (Linear Probing). Para esto es necesario tener en cuenta los estados (LIBRE,  OCUPADO y BORRADO, sobre el último se va a hablar en siguientes clases). Los estados lo debería implementar usando enum.
Si se quiere guardar con una clave que ya está en el TDA, se debe reemplazar el valor guardado por el nuevo.

buscar lo mismo que guardar

carga:
La cantidad de nodos no vacíos, esto es, en esatados ocupado o borrado. En el nodo cerrado, se usa este valor para calcular la carga, en lugar de la cantidad.
Por otra parte, date cuenta que solo hace falta actualizarlo en insertar, al pasar un nodo de vacío a ocupado.
*/
typedef void (*hash_destruir_dato_t)(void *);

/******************************************************************************
* 							FUNCIONES										  *
******************************************************************************/
#define TAM_INICIAL 101 //número primo.
#define PIVOTE1 378551
#define PIVOTE2 63689

//devuelve una posicion en el arreglo;
size_t hashing(size_t capacidad, const char *clave){
  size_t hash = 0, pivot1 = PIVOTE1, pivot2 = PIVOTE2;
  for(int i = 0; clave[i] != '\0'; i++){
    hash = hash * pivot1 + (size_t)clave[i];
    pivot1 *= pivot2;
  }
  return hash % capacidad;
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/
hash_t *hash_crear(hash_destruir_dato_t destruir_dato);


bool hash_guardar(hash_t *hash, const char *clave, void *dato);


void *hash_borrar(hash_t *hash, const char *clave);


void *hash_obtener(const hash_t *hash, const char *clave);


bool hash_pertenece(const hash_t *hash, const char *clave);


size_t hash_cantidad(const hash_t *hash);


void hash_destruir(hash_t *hash);

/* *****************************************************************
 *                     PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

// Crea iterador
hash_iter_t *hash_iter_crear(const hash_t *hash);

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t *iter);

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *hash_iter_ver_actual(const hash_iter_t *iter);

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t *iter);

// Destruye iterador
void hash_iter_destruir(hash_iter_t* iter);

#endif // HASH_H
