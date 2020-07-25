#include <stdio.h>
#include "abb.h"
#define EXITO 0
#define ERROR -1

abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){

    abb_t* abb = malloc(sizeof(abb_t));

    if(!abb){
        return NULL;
    }

    abb->nodo_raiz = NULL;

    abb->comparador = comparador;
    abb->destructor = destructor;

    return abb;
}

/*Creo un nodo de un abb y reservando memoria para el mismo*/
nodo_abb_t * nodo_abb_crear(){

    nodo_abb_t * nodo = calloc(1, sizeof(nodo_abb_t));

    return nodo;
}

nodo_abb_t* arbol_buscar_nodo_padre(nodo_abb_t* nodo, nodo_abb_t* padre, abb_comparador comparador, void* elemento){
    if(!nodo){
        return padre;
    }

    padre = nodo;

    int resultado = comparador(nodo->elemento, elemento);

	if (resultado >= 0){
		return arbol_buscar_nodo_padre(nodo->izquierda, padre, comparador, elemento);
    }

	return arbol_buscar_nodo_padre(nodo->derecha, padre, comparador, elemento);
}

/*Funcion que busca un nodo en el arbol*/
nodo_abb_t* arbol_buscar_nodo(nodo_abb_t* nodo, abb_comparador comparador, void* elemento){
    if(!nodo){
        return NULL;
    }

    int resultado = comparador(nodo->elemento, elemento);

	if(resultado == 0){
		return nodo;
    }

	if (resultado == 1){
		return arbol_buscar_nodo(nodo->izquierda, comparador, elemento);
    }

	return arbol_buscar_nodo(nodo->derecha, comparador, elemento);
}

void* arbol_buscar(abb_t* arbol, void* elemento){
    if(!arbol || arbol_vacio(arbol)){
        return NULL;
    }

   nodo_abb_t* nodo_buscado = arbol_buscar_nodo(arbol->nodo_raiz, arbol->comparador, elemento);

   if(!nodo_buscado){
       return NULL;
   }

   return nodo_buscado->elemento;
}

int arbol_insertar(abb_t* arbol, void* elemento){
    if (!arbol){
        return ERROR;
    }

    nodo_abb_t * nodo_nuevo = nodo_abb_crear();

    if (!nodo_nuevo){
        return ERROR;
    }

    nodo_nuevo->elemento = elemento;

    if(arbol_vacio(arbol)){
        nodo_nuevo->izquierda = NULL;
        nodo_nuevo->derecha = NULL;

        arbol->nodo_raiz = nodo_nuevo;
        return EXITO;
    }

    nodo_abb_t* padre = NULL;

    padre =  arbol_buscar_nodo_padre(arbol->nodo_raiz, padre, arbol->comparador, elemento);

    int resultado = arbol->comparador(padre->elemento, elemento);

    if(resultado >= 0){
        padre->izquierda = nodo_nuevo;

    }else{
        padre->derecha = nodo_nuevo;
    }

    return EXITO;
}

bool arbol_vacio(abb_t* arbol){
    if(!arbol){
        return true;
    }

    if(!arbol->nodo_raiz){
        return true;
    }

    return false;
}

void* arbol_raiz(abb_t* arbol){
    if(!arbol || !arbol->nodo_raiz){
        return NULL;
    }

    return arbol->nodo_raiz->elemento;
}

/*
* Agrega el elemento a la posicion
* del array correspondiente
*/
void agregar_elemento(void* elemento, int* i, void** array){

    array[*i] = elemento;

    (*i)++;
}

/*
* Funcion que hace el recorrido in_order
* y se encarga de llamar a una funcion que
* asigna el elemento al array
*/
void in_order(nodo_abb_t* nodo, int* i, void** array, int tamanio_array){
    if(!nodo){
        return;
    }

    if(nodo->izquierda){
        in_order(nodo->izquierda, i, array, tamanio_array);
    }

    if(nodo){
        if(*i < tamanio_array){
            agregar_elemento(nodo->elemento, i, array);
        }
    }

    if(nodo->derecha){
        in_order(nodo->derecha, i, array, tamanio_array);
    }
}

/*
* Funcion que hace el recorrido pre_order
* y se encarga de llamar a una funcion que
* asigna el elemento al array
*/
void pre_order(nodo_abb_t* nodo, int* i, void** array, int tamanio_array){
    if(!nodo){
        return;
    }

    if(nodo){
        if(*i < tamanio_array){
            agregar_elemento(nodo->elemento, i, array);
        }
    }

    if(nodo->izquierda){
        pre_order(nodo->izquierda, i, array, tamanio_array);
    }


    if(nodo->derecha){
        pre_order(nodo->derecha, i, array, tamanio_array);
    }
}

/*
* Funcion que hace el recorrido post_order
* y se encarga de llamar a una funcion que
* asigna el elemento al array
*/
void post_order(nodo_abb_t* nodo, int* i, void** array, int tamanio_array){
    if(!nodo){
        return;
    }

    if(nodo->izquierda){
        post_order(nodo->izquierda, i, array, tamanio_array);
    }

    if(nodo->derecha){
        post_order(nodo->derecha, i, array, tamanio_array);
    }

    if(nodo){
        if(*i < tamanio_array){
            agregar_elemento(nodo->elemento, i, array);
        }
    }
}

int arbol_recorrido_inorden(abb_t* arbol, void** array, int tamanio_array){
    if(!arbol){
        return 0;
    }

    int a = 0;
    int* i = &a;
    if(!arbol_vacio(arbol)){
        in_order(arbol->nodo_raiz, i, array, tamanio_array);
    }

    return *i;
}

int arbol_recorrido_preorden(abb_t* arbol, void** array, int tamanio_array){
    if(!arbol){
        return 0;
    }

    int a = 0;
    int* i = &a;
    if(!arbol_vacio(arbol)){
       pre_order(arbol->nodo_raiz, i, array, tamanio_array);
    }

    return *i;
}

int arbol_recorrido_postorden(abb_t* arbol, void** array, int tamanio_array){
    if(!arbol){
        return 0;
    }
    int a = 0;
    int* i = &a;
    if(!arbol_vacio(arbol)){
       post_order(arbol->nodo_raiz, i, array, tamanio_array);
    }

    return *i;
}

/*
* Funcion que busca el nodo
* que reemplazaria el nodo a eliminar.
* Se busca el inmediato superior o inmediato inferior.
*/
nodo_abb_t* arbol_buscar_nodo_reemplazo(nodo_abb_t* nodo){
    if(nodo->derecha){
        nodo = arbol_buscar_nodo_reemplazo(nodo->derecha);
    }

    return nodo;
}

/*
* Funcion que se encarga de buscar el padre de un nodo
*/
nodo_abb_t* arbol_buscar_padre(abb_t* arbol, nodo_abb_t* nodo_eliminar, nodo_abb_t* nodo_actual, nodo_abb_t* nodo_anterior){
    if(!arbol){
        return NULL;
    }

    int result = arbol->comparador(nodo_actual->elemento, nodo_eliminar->elemento);

    if(result == 1){
        return arbol_buscar_padre(arbol, nodo_eliminar, nodo_actual->izquierda, nodo_actual);
    }
    if(result == -1){
        return arbol_buscar_padre(arbol, nodo_eliminar, nodo_actual->derecha, nodo_actual);
    }

    return nodo_anterior;
}

/*
* Funcion que asigna los punteros como corresponden
* en el caso de que el nodo que estemos eliminando 
* no tenga hijos
*/
void arbol_borrar_0_hijos(abb_t* arbol, void* elemento, nodo_abb_t* nodo_eliminar, nodo_abb_t* nodo_anterior){
    if(!nodo_anterior){
        arbol->nodo_raiz = NULL;
    }else{
        int resultado = arbol->comparador(nodo_anterior->elemento, elemento);
        if(resultado >= 0){
            nodo_anterior->izquierda = NULL;
        }else{
            nodo_anterior->derecha = NULL;
        }
    }
} 

/*
* Funcion que asigna los punteros como corresponden
* en el caso de que el nodo que estemos eliminando 
* tenga un hijo
*/
void arbol_borrar_1_hijo(abb_t* arbol, void* elemento, nodo_abb_t* nodo_eliminar, nodo_abb_t* nodo_anterior){
    if(!nodo_anterior){
        if(nodo_eliminar->izquierda){
            arbol->nodo_raiz = nodo_eliminar->izquierda;
        }else{
            arbol->nodo_raiz = nodo_eliminar->derecha;
        }
    }else{
        int resultado = arbol->comparador(nodo_anterior->elemento, elemento);
        if(resultado >= 0){
            if(nodo_eliminar->izquierda){
                nodo_anterior->izquierda = nodo_eliminar->izquierda;
            }else{
                nodo_anterior->izquierda = nodo_eliminar->derecha;
            }
        }else{
            if(nodo_eliminar->derecha){
                nodo_anterior->derecha = nodo_eliminar->derecha;
            }else{
                nodo_anterior->derecha = nodo_eliminar->izquierda;
            }
        }
    }
}

/*
* Funcion que asigna los punteros como corresponden
* en el caso de que el nodo que estemos eliminando
* tenga dos hijos
*/
void arbol_borrar_2_hijos(abb_t* arbol, void* elemento, nodo_abb_t* nodo_eliminar, nodo_abb_t* nodo_anterior){
    nodo_abb_t* nodo_reemplazo = arbol_buscar_nodo_reemplazo(nodo_eliminar->izquierda);
    nodo_abb_t* nodo_anterior_reemplazo = NULL;
    nodo_anterior_reemplazo = arbol_buscar_padre(arbol, nodo_reemplazo, arbol->nodo_raiz, nodo_anterior_reemplazo);
    if(!nodo_anterior){
        int resultado = arbol->comparador(nodo_anterior_reemplazo->elemento, nodo_reemplazo->elemento);
        if(resultado >= 0){
            nodo_anterior_reemplazo->izquierda = NULL;
        }else{
            nodo_anterior_reemplazo->derecha = NULL;
        }
        if(!nodo_reemplazo->izquierda){
            nodo_reemplazo->izquierda = nodo_eliminar->izquierda;
        }
        nodo_reemplazo->derecha = nodo_eliminar->derecha;
        arbol->nodo_raiz = nodo_reemplazo;
    }else{
        int resultado = arbol->comparador(nodo_anterior->elemento, nodo_reemplazo->elemento);
        if(resultado >= 0){
            nodo_anterior->izquierda = nodo_reemplazo;
            nodo_reemplazo->derecha = nodo_eliminar->derecha;
        }else{
            nodo_anterior->derecha = nodo_reemplazo;
            nodo_reemplazo->derecha = nodo_eliminar->derecha;
        }
    }
}

int arbol_borrar(abb_t* arbol, void* elemento){
    if(!arbol){
        return ERROR;
    }

    nodo_abb_t* nodo_eliminar = arbol_buscar_nodo(arbol->nodo_raiz, arbol->comparador, elemento);
    nodo_abb_t* nodo_anterior = NULL;
    nodo_anterior = arbol_buscar_padre(arbol, nodo_eliminar, arbol->nodo_raiz, nodo_anterior);

    if(!nodo_eliminar->derecha && !nodo_eliminar->izquierda){
        arbol_borrar_0_hijos(arbol, elemento, nodo_eliminar, nodo_anterior);
    }

    if((nodo_eliminar->derecha && !nodo_eliminar->izquierda) || (!nodo_eliminar->derecha && nodo_eliminar->izquierda)){
        arbol_borrar_1_hijo(arbol, elemento, nodo_eliminar, nodo_anterior);
    }

    if(nodo_eliminar->derecha && nodo_eliminar->izquierda){
        arbol_borrar_2_hijos(arbol, elemento, nodo_eliminar, nodo_anterior);
    }

    if(arbol->destructor){
        arbol->destructor(nodo_eliminar->elemento);
    }

    free(nodo_eliminar);
    return EXITO;
}

/*
* Funcion que destruye el elemento
* con el destructor del arbol y se
* libera la memoria reservada para el nodo
*/
void arbol_destruir_nodo(nodo_abb_t* nodo, abb_liberar_elemento destructor){
    if(!nodo){
        return;
    }

    if(nodo->izquierda){
        arbol_destruir_nodo(nodo->izquierda, destructor);
    }

    if(nodo->derecha){
        arbol_destruir_nodo(nodo->derecha, destructor);
    }

    if(destructor){
        destructor(nodo->elemento);
    }

   free(nodo);
}

void arbol_destruir(abb_t* arbol){
    if(!arbol){
        return;
    }

    arbol_destruir_nodo(arbol->nodo_raiz, arbol->destructor);

	free(arbol);
}

/*
* Funcion que se encarga del recorrido del in-order, recibe un nodo del arbol, 
* una funcion que si devuelve true no debera recorrer mas 
* y un puntero 'extra' que va como parametro en la funcion pasada por parametro.
* Se tiene en cuenta que devuleve la llamada recursiva. Si devuelve false, 
* significa que no debe recorrer mas. 
*/
bool recorrer_in_order(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra){
    if(!nodo){
        return true;
    }

    if(!recorrer_in_order(nodo->izquierda, funcion, extra)){
        return false;
    }
    
    if(funcion(nodo->elemento, extra)){
        return false;
    } 

    if(!recorrer_in_order(nodo->derecha, funcion, extra)){
        return false;
    }

    return true;
}

/*
* Funcion que se encarga del recorrido del pre-order, recibe un nodo del arbol, 
* una funcion que si devuelve true no debera recorrer mas 
* y un puntero 'extra' que va como parametro en la funcion pasada por parametro.
* Se tiene en cuenta que devuleve la llamada recursiva. Si devuelve false, 
* significa que no debe recorrer mas
*/
bool recorrer_pre_order(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra){
    if(!nodo){
        return true;
    }
 
    if(funcion(nodo->elemento, extra)){
        return false;
    } 
 

    if(!recorrer_pre_order(nodo->izquierda, funcion, extra)){
        return false;
    }

    if(!recorrer_pre_order(nodo->derecha, funcion, extra)){
        return false;
    }

    return true;
}

/*
* Funcion que se encarga del recorrido del post-order, recibe un nodo del arbol, 
* una funcion que si devuelve true no debera recorrer mas 
* y un puntero 'extra' que va como parametro en la funcion pasada por parametro.
* Se tiene en cuenta que devuleve la llamada recursiva. Si devuelve false, 
* significa que no debe recorrer mas. 
*/
bool recorrer_post_order(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra){
    if(!nodo){
        return true;
    }

    if(!recorrer_post_order(nodo->izquierda, funcion, extra)){
        return false;
    }

    if(!recorrer_post_order(nodo->derecha, funcion, extra)){
        return false;
    }

    if(funcion(nodo->elemento, extra)){
        return false;
    }

    return true;
}

void abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){
    if(!arbol){
        return;
    }

    switch (recorrido)
    {
    case ABB_RECORRER_INORDEN:
        recorrer_in_order(arbol->nodo_raiz, funcion, extra);
        break;
    case ABB_RECORRER_PREORDEN:
        recorrer_pre_order(arbol->nodo_raiz, funcion, extra);
        break;
    case ABB_RECORRER_POSTORDEN:
        recorrer_post_order(arbol->nodo_raiz, funcion, extra);
        break;
    }
}