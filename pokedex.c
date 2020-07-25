#include "pokedex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EXITO 0
#define ERROR -1
#define FORMATO_LECTURA_AVISTAMIENTOS "%i;%[^;];%[^;];%[^;];%i;%c\n"
#define FORMATO_LECTURA_EVOLUCIONES "%i;%[^;];%i;%[^;];%[^\n]\n"

/*Funcion comparadora de elementos*/
//Adaptar a lo que va a ser la clave
int comparacion(void* elem1, void* elem2){

    if(!elem1 && elem2){
        return -1;
    }
    if(elem1 && !elem2){
        return 1;
    }
    if(!elem1 && !elem2){
        return 0;
    }

    if(((especie_pokemon_t*)elem1)->numero < ((especie_pokemon_t*)elem2)->numero){
        return -1;
    }
    if(((especie_pokemon_t*)elem1)->numero > ((especie_pokemon_t*)elem2)->numero){
        return 1;
    }

    return 0;
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

/*Agrega el pokemon a la respectiva estructura, con los datos provewnientes por parametros*/
int agregar_especie(pokedex_t* pokedex, especie_pokemon_t nueva_especie, particular_pokemon_t nuevo_particular){
    
    if(!pokedex){
        return ERROR;
    }

    especie_pokemon_t* especie = arbol_buscar(pokedex->pokemones, &nueva_especie);
    if(!especie){ 
        especie = malloc(sizeof(especie_pokemon_t));
        if(!especie){
            return ERROR;
        }
    }

    especie->numero = nueva_especie.numero;
    strcpy(especie->nombre, nueva_especie.nombre);
    strcpy(especie->descripcion, nueva_especie.descripcion);
    
    particular_pokemon_t* particular = malloc(sizeof(particular_pokemon_t));
    if(!particular){
        return ERROR;
    }

    strcpy(particular->nombre, nuevo_particular.nombre);
    particular->nivel = nuevo_particular.nivel;
    particular->capturado = nuevo_particular.capturado;

    especie->pokemones = lista_crear();
    if(!especie->pokemones){
        return ERROR;
    }
        
    int exito = lista_insertar(especie->pokemones, particular);
    if(exito == ERROR){
        return ERROR;
    }

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
            fclose(pokemones_f);
            return ERROR;
        }

        int agregado_listas = agregar_pokemon_listas(pokedex, particular_pokemon);
        if(agregado_listas == ERROR){
            fclose(pokemones_f);
            return ERROR;
        }
    }

    fclose(pokemones_f);

    return EXITO;   
}

void imprimir_pokedex(pokedex_t* pokedex){
    printf("El dueño de la pokedex es: %s\n", pokedex->nombre_entrenador);
}

int evolucionar_pokemon(pokedex_t*pokedex, especie_pokemon_t nueva_especie, especie_pokemon_t actual_especie, particular_pokemon_t particular_pokemon){
    
    if(!pokedex){
        return ERROR;
    }
    
    especie_pokemon_t* especie = arbol_buscar(pokedex->pokemones, &nueva_especie);
    if(!especie){ 
        especie = malloc(sizeof(especie_pokemon_t));
        if(!especie){
            return ERROR;
        }
        //si la especie exite estamos seguros de que va a tenr la misma descripcion que la nueva ?
        especie->numero = nueva_especie.numero;
        strcpy(especie->nombre, nueva_especie.nombre);
        strcpy(especie->descripcion, nueva_especie.descripcion);

        particular_pokemon_t* particular = malloc(sizeof(particular_pokemon_t));
        if(!particular){
            return ERROR;
        }

        strcpy(particular->nombre, particular_pokemon.nombre);
        particular->nivel = particular_pokemon.nivel;
        particular->capturado = particular_pokemon.capturado;

        especie->pokemones = lista_crear();
        if(!especie->pokemones){
            return ERROR;
        }
            
        int exito = lista_insertar(especie->pokemones, particular);
        if(exito == ERROR){
            return ERROR;
        }

        int ver = arbol_insertar(pokedex->pokemones, especie);
        if(ver == ERROR){
            return ERROR;
        }
    }else{
        //Existe en el abb, debo agarrar el elemento de lista o lo creo desde 0 ?, recorro la lista
        for (size_t i = 0; i < lista_elementos(especie->pokemones); i++){
            //cuando encontremos al pokemon por nombre sabremos la posicion
            // if(*(especie->pokemones[i]).nombre == "jUli"){

            // }
        }
        
        //lista_borrar_de_posicion() hay que pasar la posicion en la que esta, quizas se deba recorrer la lista y saber la posicion en la que esta el elemento (pokemon particular)
    }
    
    return EXITO;
}

int pokedex_evolucionar(pokedex_t* pokedex, char ruta_archivo[MAX_RUTA]){
    if(!pokedex){
        return ERROR;
    }

    FILE *pokemones_f = fopen(ruta_archivo, "r");
    if (pokemones_f == NULL) {
        return ERROR;
    }

    int leidos = 0;
    especie_pokemon_t nueva_especie;
    especie_pokemon_t actual_especie;
    particular_pokemon_t particular_pokemon;
    char capturado;
    
    while((leidos = fscanf(pokemones_f, FORMATO_LECTURA_EVOLUCIONES, &actual_especie.numero, particular_pokemon.nombre, actual_especie.descripcion, nueva_especie.nombre, nueva_especie.descripcion)) == 5){
        
        int evolucion_exito = evolucionar_pokemon(pokedex, nueva_especie, actual_especie, particular_pokemon);
        if(evolucion_exito == ERROR){
            fclose(pokemones_f);
            return ERROR;
        }
    }
    // Si no tengo la especie la empiezo a conocer en la evolucion
    //
    return EXITO;
}

int main(){
    char* ruta_avistamientos = "avistamientos.txt";
    char* ruta_evoluciones = "evoluciones.txt";
    pokedex_t* pokedex = pokedex_crear("JULIETA");
    //imprimir_pokedex(pokedex);
    int exito = pokedex_avistar(pokedex, ruta_avistamientos);
    printf("%i", exito);
    return 1;
}