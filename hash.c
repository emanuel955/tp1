#define TAM_MIN 101 // Numero primo.
#define PIVOTE1 378551
#define PIVOTE2 63689

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
	nodo_hash_t* tabla;					//	Arreglo conformado por nodos de hash.
	size_t cantidad;					//	Cantidad de nodos de hash almacenados.
	size_t capacidad;					//	Cantidad total de nodos que puede almacenar el hash.
	hash_destruir_dato_t destruir_dato;	//	Funcion para destruir los datos del hash.
};

/* Definición del struct hash_iter. */

struct hash_iter{
	hash_t* hash;
	size_t act;
};

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

/* *****************************************************************
 *                  	  FUNCION DE HASHING
 * *****************************************************************/

// Devuelve una posicion del hash para almacenar la pareja (clave, valor)
size_t hashing(size_t capacidad, const char *clave){
  size_t hash = 0, pivot1 = PIVOTE1, pivot2 = PIVOTE2;
  for(int i = 0; clave[i] != '\0'; i++){
    hash = hash * pivot1 + (size_t)clave[i];
    pivot1 *= pivot2;
  }
  return hash % capacidad;
}

/* *****************************************************************
 *                   	 PRIMITIVAS DEL HASH
 * *****************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if (!hash){
		return NULL;
	}

	hash->tabla = malloc(TAM_MIN * sizeof(nodo_hash_t));
	if (!hash->tabla){
		free(hash);
		return NULL;
	}

	hash->cantidad = 0;
	hash->capacidad = TAM_MIN;
	hash->destruir_dato = destruir_dato;

	return hash;
}


bool hash_guardar(hash_t *hash, const char *clave, void *dato);


void *hash_borrar(hash_t *hash, const char *clave);


void *hash_obtener(const hash_t *hash, const char *clave){

}


bool hash_pertenece(const hash_t *hash, const char *clave){
	size_t posicion = hashing(hash -> capacidad, clave);
		else if(hash -> tabla[posicion] -> clave == clave) return true;
	return false
}


size_t hash_cantidad(const hash_t *hash){
	return hash -> cantidad;
}


void hash_destruir(hash_t *hash);

/* *****************************************************************
 *                FUNCION AUXILIAR PARA EL ITERADOR
 * *****************************************************************/

size_t posicionar_actual(const hash_t *hash, int posicion){
	posicion++;

	while(hash->tabla[posicion].estado != DATO){	// Mientras que el estado del nodo actual de la tabla de Hash sea distinto de DATO.
		
		if (posicion == hash->capacidad){			// Si la posicion es igual a la capacidad total de la tabla de Hash.
			return hash->capacidad;
		}
		posicion++;
	}

	return posicion
}

/* *****************************************************************
 *                     PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if (!iter){
		return NULL;
	}

	iter->hash = hash;
	iter->act = posicionar_actual(iter->hash, -1)
	return iter;
}


bool hash_iter_avanzar(hash_iter_t *iter){
	if hash_iter_al_final(iter){
		return false;
	}

	iter->act == posicionar_actual(iter->hash, iter->act);
	return true;
}


const char *hash_iter_ver_actual(const hash_iter_t *iter){
	if hash_iter_al_final(iter){
		return NULL;
	}

	return iter->hash->tabla[iter->act].clave;
}


bool hash_iter_al_final(const hash_iter_t *iter){
	return iter->act == iter->hash->capacidad;
}


void hash_iter_destruir(hash_iter_t* iter){
	free(iter);
}