#define TAM_MIN 101 // Numero primo.
#define PIVOTE1 378551
#define PIVOTE2 63689

#include <stdbool.h>
#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "hash.h"

#define AUMENTA 2
#define REDUCE 4
#define CAP_MAX 0.8

/* Definición del struct nodo_hash. */
typedef enum{VACIO, BORRADO, OCUPADO} estado_t;

typedef struct nodo_hash{
	char* clave;
	void* dato;
	estado_t estado;
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
	const hash_t* hash;
	size_t act;
};


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
char* copiar_clave(const char* clave){
	char* copia = malloc(strlen(clave) + 1);
	strcpy(copia,clave);
	return copia;

}
nodo_hash_t* crear_tabla(size_t tamanio){
	nodo_hash_t* tabla = malloc(tamanio * sizeof(nodo_hash_t));
	if (!tabla){
		return NULL;
	}
	for(size_t i=0; i < tamanio;i++){
		tabla[i].clave = NULL;
		tabla[i].dato = NULL;
		tabla[i].estado = VACIO;
	}
	return tabla;
}
size_t buscar_posicion(const hash_t* hash, const char* clave){
	/*busca una posicion libre o que tenga la misma clave*/

	size_t posicion = hashing(hash -> capacidad, clave);
	while (hash -> tabla[posicion].estado == OCUPADO || hash -> tabla[posicion].estado == BORRADO){

		if(strcmp(hash -> tabla[posicion].clave, clave) == 0) return posicion;

		posicion++;

		if(posicion == (hash -> capacidad)) posicion = 0;
	}
	return posicion;
}

void asignar(hash_t* hash, size_t posicion,void* dato, estado_t estado){
	hash -> tabla[posicion].dato =  dato;
	hash -> tabla[posicion].estado = estado;
}

bool redimension(hash_t* hash, size_t nuevo_tam){
	/* crea una tabla nueva, recorre la vieja
	obtiene una posicion en la tabla nueva de la clave de la vieja*/

	nodo_hash_t* ant_tabla = hash -> tabla; 
	size_t capacidad_anterio = hash -> capacidad;

	hash -> tabla = crear_tabla(nuevo_tam);
	if(!hash->tabla){
		hash -> tabla = ant_tabla;
		return false;
	} 
	hash -> capacidad = nuevo_tam; 

	for(size_t i = 0; i < capacidad_anterio; i++){
		if(ant_tabla[i].estado != OCUPADO) continue;

		size_t posicion = buscar_posicion(hash, ant_tabla[i].clave);
		hash -> tabla[posicion].clave = ant_tabla[i].clave;
		asignar(hash, posicion,ant_tabla[i].dato, OCUPADO);
	}
	free(ant_tabla);
	return true;
}

/* *****************************************************************
 *                   	 PRIMITIVAS DEL HASH
 * *****************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if (!hash){
		return NULL;
	}

	hash->tabla = crear_tabla(TAM_MIN);
	if(!hash -> tabla){
		free(hash);
		return NULL;
	}
	
	hash->cantidad = 0;
	hash->capacidad = TAM_MIN;
	hash->destruir_dato = destruir_dato;
	return hash;
}


bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	if(!hash || !clave) return false;
	
	double factor_carga =(double)hash -> cantidad / (double)hash -> capacidad;
	if(factor_carga >= CAP_MAX){
		bool estado_redimension = redimension(hash, hash -> capacidad * AUMENTA);
		if(!estado_redimension) return false;
	}

	size_t posicion = buscar_posicion(hash, clave);
	if(hash -> tabla[posicion].estado == OCUPADO){
		if(hash -> destruir_dato){
			hash -> destruir_dato(hash -> tabla[posicion].dato);
		}
	}else{
		hash -> tabla[posicion].clave = copiar_clave(clave);
		hash -> cantidad ++;
	}
	asignar(hash, posicion, dato, OCUPADO);
	return true;

}


void *hash_borrar(hash_t *hash, const char *clave){
	if(!hash || !clave) return false;

	if(hash -> cantidad == hash -> capacidad/REDUCE && hash -> cantidad / REDUCE > TAM_MIN){
		bool estado_redimension = redimension(hash, hash -> capacidad /REDUCE);
		if(!estado_redimension) return NULL;
	}
	size_t posicion = buscar_posicion(hash, clave);
	if(hash -> tabla[posicion].estado != OCUPADO) return NULL;
	void* valor = hash -> tabla[posicion].dato;
	free(hash -> tabla[posicion].clave);
	hash -> tabla[posicion].clave = "";
	asignar(hash,posicion,NULL,BORRADO);
	hash -> cantidad --;
	return valor;
}


void *hash_obtener(const hash_t *hash, const char *clave){
	if(!hash_pertenece(hash, clave)) return NULL;
	size_t posicion = buscar_posicion(hash, clave);
	return hash -> tabla[posicion].dato;
}


bool hash_pertenece(const hash_t *hash, const char *clave){
	if(!hash || !clave) return false;
	size_t posicion = buscar_posicion(hash, clave);
	if(hash -> tabla[posicion].estado != OCUPADO) return false;
	return true;
}


size_t hash_cantidad(const hash_t *hash){
	return hash -> cantidad;
}


void hash_destruir(hash_t *hash){
	for (int i = 0; i < hash ->capacidad; i++){
		if(hash -> tabla[i].estado == OCUPADO){
			if(hash -> destruir_dato){
				hash -> destruir_dato(hash -> tabla[i].dato);
			}
			free(hash -> tabla[i].clave);
		}
	}
	free(hash -> tabla);
	free(hash);
}

/* *****************************************************************
 *                FUNCION AUXILIAR PARA EL ITERADOR
 * *****************************************************************/

size_t posicionar_actual(const hash_t *hash, size_t posicion){

	if (posicion == hash->capacidad){			// Si la posicion es igual a la capacidad total de la tabla de Hash.
		return hash->capacidad;
	}

	while(hash->tabla[posicion].estado != OCUPADO){	// Mientras que el estado del nodo actual de la tabla de Hash sea distinto de DATO.
		posicion++;
		if (posicion == hash->capacidad){			// Si la posicion es igual a la capacidad total de la tabla de Hash.
			return hash->capacidad;
		}	
	}
	return posicion;
}

/* *****************************************************************
 *                     PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

hash_iter_t *hash_iter_crear(const hash_t *hash){
	if(!hash) return NULL;
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if (!iter){
		return NULL;
	}

	iter->hash = hash;
	iter->act = posicionar_actual(iter->hash, 0);
	return iter;
}


bool hash_iter_avanzar(hash_iter_t *iter){
	if (hash_iter_al_final(iter)){
		return false;
	}

	iter->act = posicionar_actual(iter->hash, iter->act + 1);
	return true;
}


const char *hash_iter_ver_actual(const hash_iter_t *iter){
	if (hash_iter_al_final(iter)){
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

