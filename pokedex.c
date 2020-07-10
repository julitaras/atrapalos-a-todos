#include "pokedex.h"
#include "abb.h"
#include "lista.h"
#include <strings.h>

/*Funcion comparadora de elementos*/
//Adaptar a lo que va a ser la clave
int comparador(void* elem1, void* elem2){

    if(!elem1 || !elem2){
		return 0;
    }

    return strcmp(*(char*)elem1, *(char*)elem2);
}

pokedex_t* pokedex_crear(char entrenador[MAX_NOMBRE]){
    pokedex_t* pokedex = malloc(sizeof(pokedex_t));

    if(!pokedex){
        return NULL;
    }

    pokedex->nombre_entrenador = entrenador;

    pokedex->pokemones = arbol_crear(comparador, NULL);
    if(!pokedex->pokemones){
        return NULL;
    }
    pokedex->ultimos_capturados = lista_crear();
    if(! pokedex->ultimos_capturados){
        return NULL;
    }
    pokedex->ultimos_vistos = lista_crear();
    if(!pokedex->ultimos_vistos){
        return NULL;
    }

    return pokedex;
}