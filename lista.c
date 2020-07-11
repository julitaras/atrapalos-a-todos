#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

/*Defino la estructura de nodo*/
typedef struct nodo {
    void * dato;
    struct nodo* siguiente;
} nodo_t;

/*Defino la estructura de lista*/
struct lista{
    nodo_t * inicio;
    nodo_t * fin;
    size_t cantidad;
};

/*Defino la estructura del iterador*/
struct lista_iterador{
    nodo_t* anterior;
	nodo_t* actual;
};

/*Crea la estructura de nodo_t, reservando memoria para la misma*/
nodo_t * nodo_crear(){

    nodo_t* nodo = calloc(1, sizeof(nodo_t));
    
    return nodo;
}

lista_t* lista_crear(){

    lista_t * lista =  malloc(sizeof(lista_t));

    if (!lista){
        return NULL;
    }

    lista->inicio = NULL;
    lista->fin = NULL;
    lista->cantidad = 0;

    return lista;
}

int lista_insertar(lista_t* lista, void* elemento){
    
    if (!lista){
        return -1;
    }

    nodo_t * nodo_nuevo = nodo_crear();

    if (!nodo_nuevo){
        return -1;
    }

    nodo_nuevo->dato = elemento;
    
    if (lista_vacia(lista)){
        lista->inicio = nodo_nuevo;
        lista->fin = nodo_nuevo;
    }

    lista->fin->siguiente = nodo_nuevo;
    lista->fin = nodo_nuevo;

    nodo_nuevo->siguiente = NULL;
    lista->cantidad++;

    return 0;
}

/*Devuelve el nodo en la posicion indicada. Siendo 0 el primer nodo. Si la posicion no existe, devuelve NULL*/
nodo_t* lista_nodo_en_posicion(lista_t* lista, size_t posicion){

    if (!lista || lista_vacia(lista) || posicion > lista->cantidad){
        return NULL;
    }

    nodo_t * nodo_buscado = lista->inicio;

    for (size_t i = 0; i < posicion && nodo_buscado != NULL; i++) {
        nodo_buscado = nodo_buscado->siguiente;
    }

    return nodo_buscado;
}

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){

    if(!lista){
        return -1;
    }

    if(posicion > lista_elementos(lista)){
        return lista_insertar(lista, elemento);
    }

    nodo_t * nodo_nuevo = nodo_crear();

    if (!nodo_nuevo){
        return -1;
    }

    nodo_nuevo->dato = elemento;

    if (lista_vacia(lista)){
        lista->inicio = nodo_nuevo;
        lista->fin = nodo_nuevo;
        nodo_nuevo->siguiente = NULL;
    }
    else{

        nodo_t * siguiente_nodo_nuevo = lista_nodo_en_posicion(lista, posicion);

        nodo_t **siguiente_nodo_viejo = &lista->inicio;

        if (posicion > 0){
            for (size_t i = 0;( i < posicion -1 && *siguiente_nodo_viejo != NULL); i++) {
                siguiente_nodo_viejo = &(*siguiente_nodo_viejo)->siguiente;
            }
        }
        nodo_nuevo->siguiente = siguiente_nodo_nuevo;

        if(posicion == 0){
            *siguiente_nodo_viejo = nodo_nuevo;
        }else{
            (*siguiente_nodo_viejo)->siguiente = nodo_nuevo;
            if(posicion == lista_elementos(lista)){
                lista->fin = nodo_nuevo;
            }
        }
    }

    lista->cantidad++;

    return 0;
}

int lista_borrar(lista_t* lista){

    if (!lista || lista_vacia(lista)){
        return -1;
    }

    nodo_t * nodo_eliminar = lista->fin;

    nodo_t **nuevo_fin = &lista->inicio;

    for (size_t i = 1; i < lista->cantidad - 1 && *nuevo_fin != NULL; i++) {
        nuevo_fin = &(*nuevo_fin)->siguiente;
    }    

    lista->fin = *nuevo_fin;
    lista->fin->siguiente = NULL;
    lista->cantidad --;
    
    if (lista_vacia(lista)){ 
        lista->fin = NULL;
    }
    free(nodo_eliminar);

    return 0;
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion){

    if (!lista || lista_vacia(lista)){
        return -1;
    }

    if(posicion >= lista->cantidad - 1){
        return lista_borrar(lista);
    }

    nodo_t * nodo_eliminar = lista_nodo_en_posicion(lista, posicion);

    if (posicion == 0){
        lista->inicio = nodo_eliminar->siguiente;
    }
    else{
        nodo_t **nuevo_siguiente = &lista->inicio;
        
        for (size_t i = 0; i < posicion - 1 && *nuevo_siguiente != NULL; i++) {
            nuevo_siguiente = &(*nuevo_siguiente)->siguiente;
        }   
    
        (*nuevo_siguiente)->siguiente = nodo_eliminar->siguiente;

        if(nodo_eliminar == lista->fin){
            lista->fin =  (*nuevo_siguiente)->siguiente;   
        }
    }

    lista->cantidad --;

    free(nodo_eliminar);

    return 0;
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){

    if (!lista || lista_vacia(lista) || posicion > lista->cantidad){
        return NULL;
    }

    nodo_t * nodo_buscado = lista->inicio;

    for (size_t i = 0; i < posicion && nodo_buscado != NULL; i++) {
        nodo_buscado = nodo_buscado->siguiente;
    }

    if (nodo_buscado != NULL){
        return nodo_buscado->dato;
    }

    return NULL;
}

bool lista_vacia(lista_t* lista){
    if (!lista){
        return true;
    }

    if (lista_elementos(lista) == 0){
        return true;
    }

    return false;
}

size_t lista_elementos(lista_t* lista){

    if(!lista){
        return 0;
    }

    return lista->cantidad;
}

void* lista_ultimo(lista_t* lista){

    if (!lista || lista_vacia(lista)){
        return NULL;
    }

   return  lista->fin->dato;
}

int lista_apilar(lista_t* lista, void* elemento){
    return lista_insertar(lista, elemento);
}

int lista_desapilar(lista_t* lista){
    return lista_borrar(lista);
}

void* lista_tope(lista_t* lista){
    return lista_ultimo(lista);
}

void* lista_primero(lista_t* lista){

    if (!lista || lista_vacia(lista)){
        return NULL;
    }

    return lista->inicio->dato;
}

int lista_encolar(lista_t* lista, void* elemento){
    if (!lista){
        return -1;
    }

    return lista_insertar(lista, elemento);
}

int lista_desencolar(lista_t* lista){

    if (!lista || lista_vacia(lista)){
        return -1;
    }

    return lista_borrar_de_posicion(lista, 0);
}

void lista_destruir(lista_t* lista){
    while (!lista_vacia(lista)) {
		lista_borrar(lista);
	}
	free(lista);
}

lista_iterador_t* lista_iterador_crear(lista_t* lista){

    if(!lista){
        return NULL;
    }
    
    lista_iterador_t* it = calloc(1, sizeof(lista_iterador_t));

    if(!it){
        return NULL;
    }

    it->anterior = NULL;
	it->actual = lista->inicio;
	return it;   
}

void lista_iterador_destruir(lista_iterador_t* iterador){
    free(iterador);
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
    if(!iterador){
        return false;
    }

    if(!iterador->actual){
        return false;
    }

    return true;
}

void* lista_iterador_siguiente(lista_iterador_t* iterador){
    if(!iterador || !lista_iterador_tiene_siguiente(iterador)){
        return NULL;
    }

    iterador->anterior = iterador->actual;
	iterador->actual = iterador->anterior->siguiente;

    return iterador->anterior->dato;
}

void lista_con_cada_elemento(lista_t* lista, void (*funcion)(void*, void*), void *contexto){
    if (!lista){
        return;
    }

    for (size_t i = 0; i < lista_elementos(lista); i++){
        funcion(lista_elemento_en_posicion(lista, i), contexto);
    }
}