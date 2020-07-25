#include "pokedex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EXITO 0
#define ERROR -1
#define FORMATO_LECTURA_AVISTAMIENTOS "%i;%[^;];%[^;];%[^;];%i;%c\n"

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
    if(!pokedex->ultimos_capturados){
        return NULL;
    }

    pokedex->ultimos_vistos = lista_crear();
    if(!pokedex->ultimos_vistos){
        return NULL;
    }

    return pokedex;
}


int agregar_pokemon_listas(pokedex_t* pokedex, particular_pokemon_t nuevo_particular){
    if(!pokedex){
        return ERROR;
    }
    
    particular_pokemon_t* particular = malloc(sizeof(particular_pokemon_t));
    if(!particular){
        return ERROR;
    }

    /*INICIALIZO*/
    strcpy(particular->nombre, nuevo_particular.nombre);
    particular->nivel = nuevo_particular.nivel;
    particular->capturado = nuevo_particular.capturado;
    
    int encolo = lista_encolar(pokedex->ultimos_vistos, particular);
    if(encolo == ERROR){
        return ERROR;
    }
    
    if(nuevo_particular.capturado){
        int apilo = lista_apilar(pokedex->ultimos_capturados, particular);
        if(apilo == ERROR){
            return ERROR;
        }
    }

    return EXITO;
}

/*Quizas queda mejor el nombre de agregar especie*/
/*Agrega el pokemon a la respectiva estructura, con los datos provewnientes por parametros*/
int agregar_especie(pokedex_t* pokedex, especie_pokemon_t nueva_especie, particular_pokemon_t nuevo_particular){
    if(!pokedex){
        return ERROR;
    }

    //Antes de crear en la memoria, busco en el abb si existe el elemento(especie de pokemon) con ese nombre
    especie_pokemon_t* especie = malloc(sizeof(especie_pokemon_t));
    if(!especie){
        printf("ERROR MALLOC ESPECIE");
        return ERROR;
    }

    /*INICIALIZO*/
    especie->numero = nueva_especie.numero;
    strcpy(especie->nombre, nueva_especie.nombre);
    strcpy(especie->descripcion, nueva_especie.descripcion);
    
    particular_pokemon_t* particular = malloc(sizeof(particular_pokemon_t));
    if(!particular){
        printf("ERROR MALLOC PARTICULAR");
        return ERROR;
    }

    /*INICIALIZO*/
    strcpy(particular->nombre, nuevo_particular.nombre);
    particular->nivel = nuevo_particular.nivel;
    particular->capturado = nuevo_particular.capturado;

    /*Creamos la lista e insertamos*/
    especie->pokemones = lista_crear();
    if(!especie->pokemones){
        return ERROR;
    }
    int exito = lista_insertar(especie->pokemones, particular);
    if(exito == ERROR){
        printf("ERROR INSERTAR PARTICULAR");
        return ERROR;
    }

    /*FINAL*/
    int ver = arbol_insertar(pokedex->pokemones, especie);
    if(ver == ERROR){
        return ERROR;
    }
    
    return EXITO;
}


int pokedex_avistar(pokedex_t* pokedex, char ruta_archivo[MAX_RUTA]){
    if(!pokedex){
        return ERROR;
    }
    printf("%s\n", ruta_archivo);
    FILE *pokemones_f = fopen(ruta_archivo, "r");
    if (pokemones_f == NULL) {
        return ERROR;
    }

    int leidos = 0;
    especie_pokemon_t nuevo_pokemon;
    particular_pokemon_t particular_pokemon;
    char capturado;
    
    while((leidos = fscanf(pokemones_f, FORMATO_LECTURA_AVISTAMIENTOS, &nuevo_pokemon.numero, nuevo_pokemon.nombre, nuevo_pokemon.descripcion, particular_pokemon.nombre, &particular_pokemon.nivel, &capturado)) == 6){
        if(capturado == 'N'){
            particular_pokemon.capturado = false;
        }else{
            particular_pokemon.capturado = true;
        }

        int agregado_abb = agregar_especie(pokedex, nuevo_pokemon, particular_pokemon);
        if(agregado_abb == ERROR){
            printf("FAIL AGREGAR A ABB");
            fclose(pokemones_f);
            return ERROR;
        }
        int agregado_listas = agregar_pokemon_listas(pokedex, particular_pokemon);
        if(agregado_listas == ERROR){
            fclose(pokemones_f);
            return ERROR;
        }
    }
    //verificar que la especie no exista, si existe solo actualizar la lista
    // if(leidos != 6){
    //     /*Fijarse en tp pokemones*/
    // }
    fclose(pokemones_f);

    return EXITO;   
}

void imprimir_pokedex(pokedex_t* pokedex){
    printf("El dueño de la pokedex es: %s\n", pokedex->nombre_entrenador);
}



int main(){
    char* ruta = "avistamientos.txt";
    pokedex_t* pokedex = pokedex_crear("JULIETA");
    imprimir_pokedex(pokedex);
    int exito = pokedex_avistar(pokedex, ruta);
    printf("%i", exito);
    return 1;
}