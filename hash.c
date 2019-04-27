#include <stdbool.h>
#include <stddef.h>

// Los structs deben llamarse "hash" y "hash_iter".
struct hash;

struct hash_iter;

// tipo de función para destruir dato
typedef void (*hash_destruir_dato_t)(void *);

/******************************************************/
//       PRIMITIVAS HASH                              //
//                                                    //
/******************************************************/
hash_t *hash_crear(hash_destruir_dato_t destruir_dato);


bool hash_guardar(hash_t *hash, const char *clave, void *dato);


void *hash_borrar(hash_t *hash, const char *clave);


void *hash_obtener(const hash_t *hash, const char *clave);


bool hash_pertenece(const hash_t *hash, const char *clave);


size_t hash_cantidad(const hash_t *hash);


void hash_destruir(hash_t *hash);

/******************************************************/
//       PRIMITIVAS ITERADOR                          //
//                                                    //
/******************************************************/

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
