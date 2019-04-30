#define TAM_MIN 101 // Numero primo.
#define PIVOTE1 378551
#define PIVOTE2 63689

#include <stdbool.h>
#include <stddef.h>

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
	hash_t* hash;
	nodo_hash_t* ant;
	nodo_hash_t* act;
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

nodo_hash_t* crear_tabla(size_t tamaño){
	nodo_hash_t* tabla = malloc(tamaño * sizeof(nodo_hash_t));
	if (!hash->tabla){
		free(hash);
		return NULL;
	}
	for(size_t i=0; i < TAM_MIN;i++){
		hash -> tabla[i] -> clave = NULL;
		hash -> tabla[i] -> valor = NULL;
		hash -> tabla[i] -> estado = VACIO;
	}
	return tabla;
}
size_t buscar_posicion(hash_t* hash, char* clave, size_t posicion){
	size_t pos_inicial = posicion;
	while (hash -> tabla[posicion]->estado == OCUPADO){

		if(hash -> tabla[posicion] -> clave == clave) return posicion;

		posicion++;

		if(posicion == hash -> capacidad) posicion = 0;

		if(posicion == pos_inicial) return NULL;
	}
	return posicion
}

void asignar(hash_t* hash, size_t posicion, char* clave,void* dato, estado_t estado){
	hash -> tabla[posicion] -> clave = clave;
	hash -> tabla[posicion] -> dato =  dato;
	hash -> tabla[posicion] -> estado = estado;
}

bool redimencion(hash_t* hash, size_t nuevo_tam){
	/* crea una tabla nueva, recorre la vieja
	obtiene una posicion en la tabla nueva de la clave de la vieja
	si esta ocupada hay colision
	si el estado es vacio actualizo clave y dato*/

	nodo_hash_t* ant_tabla = hash -> tabla; 
	size_t tam_ant = hash -> capacidad; 

	hash -> tabla = crear_tabla(nuevo_tam);
	if(!hash->tabla) return false
	hash -> capacidad = nuevo_tam; 

	for(size_t i = 0; i < tam_ant; i++){ 

		if(ant_tabla[i] -> estado != OCUPADO) continue;

		size_t posicion = hashing(nuevo_tam, ant_tabla[i] -> clave); 
		if(hash -> tabla[posicion] -> estado == OCUPADO){ 
			posicion = buscar_posicion(hash, ant_tabla[i] -> clave, posicion);
		}
		asignar(hash ,posicion, ant_tabla[i] -> clave, ant[i] -> dato,OCUPADO);
		hash -> cantidad ++;
	}
	hash_destruir(ant_tabla);
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
	
	hash->cantidad = 0;
	hash->capacidad = TAM_MIN;
	hash->destruir_dato = destruir_dato;

	return hash;
}


bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	size_t posicion = hashing(hash -> capacidad, clave);

	size_t factor_carga = hash -> cantidad / hash -> capacidad;
	if(factor_carga >= CAP_MAX){
		bool estado_redimension = redimencion(hash, hash -> capacidad * AUMENTA);
		if(!estado_redimension) return false;
	}
	if(hash ->tabla[posicion]-> estado == OCUPADO){
		posicion = buscar_posicion(hash,clave,posicion);
	asignar(hash ,posicion, clave, dato,OCUPADO);

	hash -> cantidad ++;
	return true;

}


void *hash_borrar(hash_t *hash, const char *clave){
	size_t posicion = hashing(hash -> capacidad, clave);

	if(hash -> cantidad == hash -> capacidad/REDUCE && hash -> cantidad / REDUCE > TAM_MIN){
		bool estado_redimension = redimencion(hash, hash -> capacidad /REDUCE);
		if(!estado_redimension) return NULL;
	}
	void* dato = hash_obtener(hash,clave);
	posicion = buscar_posicion(hash,clave,posicion);
	asignar(hash ,posicion, NULL, NULL,BORRADO);
	hash -> cantidad --;
	return dato;
}


void *hash_obtener(const hash_t *hash, const char *clave){
	size_t posicion = hashing(hash -> capacidad, clave);
	if(!hash_pertenece(hash, clave){
		posicion = buscar_posicion(hash,clave,posicion);
		if(!posicion) return NULL;
	return hash -> tabla[posicion] -> dato;
}


bool hash_pertenece(const hash_t *hash, const char *clave){
	if(!hash || !clave) return false;
	size_t posicion = hashing(hash -> capacidad, clave);
	if(hash -> tabla[posicion] -> clave == clave) return true;
	return false
}


size_t hash_cantidad(const hash_t *hash){
	return hash -> cantidad;
}


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
