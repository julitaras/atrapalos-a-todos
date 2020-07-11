#include "pokedex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EXITO 0
#define ERROR -1

/*Funcion comparadora de elementos*/
//Adaptar a lo que va a ser la clave
int comparacion(void* elem1, void* elem2){

    if(!elem1 || !elem2){
		return 0;
    }

    return strcmp((char*)elem1, (char*)elem2);
}

pokedex_t* pokedex_crear(char entrenador[MAX_NOMBRE]){
    pokedex_t* pokedex = malloc(sizeof(pokedex_t));

    if(!pokedex){
        return NULL;
    }

   strcpy(pokedex->nombre_entrenador, entrenador);

    pokedex->pokemones = arbol_crear(comparacion, NULL);
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

int pokedex_avistar(pokedex_t* pokedex, char ruta_archivo[MAX_RUTA]){
    if(!pokedex){
        return ERROR;
    }
    

    FILE *pokemones_f = fopen(ruta_archivo, "r");
    //Deberia fijarme si hay algo mas para cargar, reservo memoria para el siguiente ...
    fclose(pokemones_f);

    return EXITO;   
}