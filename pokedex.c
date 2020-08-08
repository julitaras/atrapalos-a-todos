#include "pokedex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EXITO 0
#define ERROR -1
#define FORMATO_LECTURA_AVISTAMIENTOS "%i;%[^;];%[^;];%[^;];%i;%c\n"
#define FORMATO_LECTURA_EVOLUCIONES "%i;%[^;];%i;%[^;];%[^\n]\n"
#define ABB_RECORRER_PREORDEN 1
#define PARTICULAR 'P'
#define ESPECIE 'E'
#define CAPTURADO 'S'
#define NO_CAPTURADO 'N'
#define ESCRITURA_ESPECIES "%c;%s;%i;%s\n"
#define ESCRITURA_POKEMONES "%c;%s;%i;%c\n"
#define FORMATO_LECTURA_ENTRENADOR "%[^\n]\n"
#define FORMATO_LECTURA_ESPECIES ";%[^;];%i;%[^\n]\n"
#define FORMATO_LECTURA_POKEMONES ";%[^;];%i;%c\n"

/*
* Funcion comparadora de elementos.
*/
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

/*
* Se encarga de destruir la especie que nos llega por parametro.
* Sera utilizada como funcion destructura en el arbol.
*/
void especie_destruir(void* especie){
    if(!especie){
        return;
    }
    
    especie_pokemon_t* aux = ((especie_pokemon_t*) especie);
    
    size_t tam = lista_elementos(aux->pokemones);
    for (size_t i = 0; i < tam; i++){
        particular_pokemon_t* elem = lista_elemento_en_posicion(aux->pokemones, i);
        free(elem); 
    }
    lista_destruir(aux->pokemones);

    free(especie);
}

pokedex_t* pokedex_crear(char entrenador[MAX_NOMBRE]){
    pokedex_t* pokedex = malloc(sizeof(pokedex_t));

    if(!pokedex){
        return NULL;
    }

   strcpy(pokedex->nombre_entrenador, entrenador);

    pokedex->pokemones = arbol_crear(comparacion, especie_destruir);
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

/*
* Agrega el pokemon particular que nos llega por parametro.
* A la pila si es que el pokemon fue capturado 
* y a la cola el pokemon que fue visto.
*/
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
        particular_pokemon_t* particular_capturado = malloc(sizeof(particular_pokemon_t));
        if(!particular_capturado){
            return ERROR;
        }

        strcpy(particular_capturado->nombre, nuevo_particular.nombre);
        particular_capturado->nivel = nuevo_particular.nivel;
        particular_capturado->capturado = nuevo_particular.capturado;

        int apilo = lista_apilar(pokedex->ultimos_capturados, particular_capturado);
        if(apilo == ERROR){
            return ERROR;
        }
    }

    return EXITO;
}

/*
* Agrega el pokemon a su respectiva especie,
* con los datos provenientes por parametros.
*/
int agregar_particular(pokedex_t* pokedex, especie_pokemon_t especie_guardar, particular_pokemon_t nuevo_particular){

    especie_pokemon_t* especie = arbol_buscar(pokedex->pokemones, &especie_guardar);
    
    particular_pokemon_t* particular = malloc(sizeof(particular_pokemon_t));
    if(!particular){
        return ERROR;
    }

    strcpy(particular->nombre, nuevo_particular.nombre);
    particular->nivel = nuevo_particular.nivel;
    particular->capturado = nuevo_particular.capturado;
    
    int exito_lista = lista_insertar(especie->pokemones, particular);
    if(exito_lista == ERROR){
        free(particular);
        return ERROR;
    }

    return EXITO;
}

/*
* Agrega la especie de un pokemon a la respectiva estructura,
* con los datos provenientes por parametros.
*/
int agregar_especie(pokedex_t* pokedex, especie_pokemon_t nueva_especie){
    
    if(!pokedex){
        return ERROR;
    }

    especie_pokemon_t* especie = arbol_buscar(pokedex->pokemones, &nueva_especie);
    if(!especie){ 
        especie = malloc(sizeof(especie_pokemon_t));
        if(!especie){
            return ERROR;
        }

        especie->pokemones = lista_crear();
        if(!especie->pokemones){
            free(especie);
            return ERROR;
        }

        especie->numero = nueva_especie.numero;
        strcpy(especie->nombre, nueva_especie.nombre);
        strcpy(especie->descripcion, nueva_especie.descripcion);

        int exito_abb = arbol_insertar(pokedex->pokemones, especie);
        if(exito_abb == ERROR){
            return ERROR;
        }
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
        if(capturado != CAPTURADO){
            particular_pokemon.capturado = false;
        }else{
            particular_pokemon.capturado = true;
        }

        int agregado_abb = agregar_especie(pokedex, nuevo_pokemon);
        if(agregado_abb == ERROR){
            fclose(pokemones_f);
            return ERROR;
        
        }

        int agregado_abb_lista = agregar_particular(pokedex, nuevo_pokemon, particular_pokemon);
        if(agregado_abb_lista == ERROR){
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

/*
* Se encarga de evolucionar el pokemon que nos llega por parametros
* buscandolo en la especie anterior, eliminadolo de la misma 
* y colocandolo en la nueva especie.
*/
int evolucionar_pokemon(pokedex_t* pokedex, especie_pokemon_t nueva_especie, especie_pokemon_t actual_especie, particular_pokemon_t particular_pokemon){
    
    if(!pokedex){
        return ERROR;
    }
    
    especie_pokemon_t* especie = arbol_buscar(pokedex->pokemones, &nueva_especie);
    
    if(!especie){ 
        especie = malloc(sizeof(especie_pokemon_t));
        if(!especie){
            return ERROR;
        }

        especie->numero = nueva_especie.numero;
        strcpy(especie->nombre, nueva_especie.nombre);
        strcpy(especie->descripcion, nueva_especie.descripcion);

        int ver = arbol_insertar(pokedex->pokemones, especie);
        if(ver == ERROR){
            free(especie);
            return ERROR;
        }

        especie_pokemon_t* especie_actual = arbol_buscar(pokedex->pokemones, &actual_especie);
        if(!especie_actual){
            return ERROR;
        }

        for (size_t i = 0; i < lista_elementos(especie_actual->pokemones); i++){
            particular_pokemon_t* elem = lista_elemento_en_posicion(especie_actual->pokemones, i); 

            if(strcmp(elem->nombre, particular_pokemon.nombre) == 0){
                especie->pokemones = lista_crear();
                if(!especie->pokemones){
                    return ERROR;
                }
                
                particular_pokemon_t* particular = malloc(sizeof(particular_pokemon_t));
                if(!particular){
                    return ERROR;
                }

                strcpy(particular->nombre, elem->nombre);
                particular->nivel = elem->nivel;
                particular->capturado = elem->capturado;
                
                int exito = lista_insertar(especie->pokemones, particular);
                if(exito == ERROR){
                    free(particular);
                    return ERROR;
                }

                free(elem);

                int err = lista_borrar_de_posicion(especie_actual->pokemones, i);
                if(err == ERROR){
                    return ERROR;
                }
            }
        }

    }else{
        especie_pokemon_t* especie_actual = arbol_buscar(pokedex->pokemones, &actual_especie);
        if(!especie_actual){
            return ERROR;
        }
        
        for (size_t i = 0; i < lista_elementos(especie_actual->pokemones); i++){
            
            particular_pokemon_t* elem = lista_elemento_en_posicion(especie_actual->pokemones, i); 

            if(strcmp(elem->nombre, particular_pokemon.nombre) == 0){
                
                particular_pokemon_t* particular = malloc(sizeof(particular_pokemon_t));
                if(!particular){
                    return ERROR;
                }

                strcpy(particular->nombre, elem->nombre);
                particular->nivel = elem->nivel;
                particular->capturado = elem->capturado;
                
                int exito = lista_insertar(especie->pokemones, particular);
                if(exito == ERROR){
                    free(particular);
                    return ERROR;
                }

                free(elem);

                int err = lista_borrar_de_posicion(especie_actual->pokemones, i);
                if(err == ERROR){
                    return ERROR;
                }
            }
        }
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
    
    while((leidos = fscanf(pokemones_f, FORMATO_LECTURA_EVOLUCIONES, &actual_especie.numero, particular_pokemon.nombre, &nueva_especie.numero, nueva_especie.nombre, nueva_especie.descripcion)) == 5){ 
        int evolucion_exito = evolucionar_pokemon(pokedex, nueva_especie, actual_especie, particular_pokemon);
        if(evolucion_exito == ERROR){
            fclose(pokemones_f);
            return ERROR;
        }
    }

    fclose(pokemones_f);

    return EXITO;
}

void pokedex_ultimos_capturados(pokedex_t* pokedex){
    
    if(!pokedex){
        return;
    }

    printf("\nULTIMOS CAPTURADOS\n");
    size_t tam = lista_elementos(pokedex->ultimos_capturados);
    for (size_t i = 0; i < tam; i++){
        
        particular_pokemon_t* pokemon = lista_ultimo(pokedex->ultimos_capturados);
        
        printf("\t- Pokemon: %s\n", pokemon->nombre);
        printf("\t\t Nivel: %i\n", pokemon->nivel);
        
        int exito = lista_desapilar(pokedex->ultimos_capturados);
        if(exito == ERROR){
            return;
        }
    }
}

void pokedex_ultimos_vistos(pokedex_t* pokedex){
    
    if(!pokedex){
        return;
    }

    printf("\nULTIMOS VISTOS\n");
    size_t tam = lista_elementos(pokedex->ultimos_vistos);
    for (size_t i = 0; i < tam; i++){
        
        particular_pokemon_t* pokemon = lista_primero(pokedex->ultimos_vistos);
        
        printf("\t- Pokemon: %s\n", pokemon->nombre);
        printf("\t\t Nivel: %i\n", pokemon->nivel);
        
        int exito = lista_desencolar(pokedex->ultimos_vistos);
        if(exito == ERROR){
            return;
        }
    }
}

void pokedex_informacion(pokedex_t* pokedex, int numero_pokemon, char nombre_pokemon[MAX_NOMBRE]){
    
    if(!pokedex){
        return;
    }
    
    especie_pokemon_t especie_buscada;
    especie_buscada.numero = numero_pokemon;

    especie_pokemon_t* especie = arbol_buscar(pokedex->pokemones, &especie_buscada);
    if(!especie){
        printf("\nLa especie buscada es desconocida...\n");
        return;
    }

    for (size_t i = 0; i < lista_elementos(especie->pokemones); i++){
        particular_pokemon_t* elem = lista_elemento_en_posicion(especie->pokemones, i); 
        
        if(strcmp(elem->nombre, nombre_pokemon) == 0){
            printf("\nPokemon: %s\n", elem->nombre);
            printf("\t Nivel: %i\n", elem->nivel);
            printf("\t Capturado: %d\n", elem->capturado);
        }else{
            if(strcmp("", nombre_pokemon) == 0){
                printf("\nPokemon: %s\n", elem->nombre);
                printf("\t Nivel: %i\n", elem->nivel);
                printf("\t Capturado: %d\n", elem->capturado);
            }else{
                printf("\nEl pokemon buscado es desconocido...\n");
                return;
            }   
        }          
    }
}

void pokedex_destruir(pokedex_t* pokedex){
    if(!pokedex){
        return;
    }
    size_t tam = lista_elementos(pokedex->ultimos_capturados);
    for (size_t i = 0; i < tam; i++){
        particular_pokemon_t* elem = lista_elemento_en_posicion(pokedex->ultimos_capturados, i);
        free(elem); 
    }
    lista_destruir(pokedex->ultimos_capturados);
    
    size_t size = lista_elementos(pokedex->ultimos_vistos);
    for (size_t i = 0; i < size; i++){
        particular_pokemon_t* elem = lista_elemento_en_posicion(pokedex->ultimos_vistos, i);
        free(elem); 
    }
    lista_destruir(pokedex->ultimos_vistos);

    arbol_destruir(pokedex->pokemones);

    free(pokedex);
}

/*
* Se encarga de guardar en el archivo 
* la informacion que nos llega por parametro.
* En caso de que el elemento que nos llega sea nulo,
* se terminara la ejecucion de la misma.
*/
bool guardar_pokedex(void* elem, void* extra){
    if(!elem){
        return true;
    }

    FILE* fichero;

    fichero = fopen("pokedex.txt", "a");
    
    especie_pokemon_t* aux = ((especie_pokemon_t*) elem);
    fprintf(fichero, ESCRITURA_ESPECIES, ESPECIE, aux->nombre, aux->numero, aux->descripcion);
    
    for (size_t i = 0; i < lista_elementos(aux->pokemones); i++){
        particular_pokemon_t* elem = lista_elemento_en_posicion(aux->pokemones, i);
        
        char capturado;
        
        if(elem->capturado){
            capturado = CAPTURADO;
        }else{
            capturado = NO_CAPTURADO;
        }

        fprintf(fichero, ESCRITURA_POKEMONES, PARTICULAR, elem->nombre, elem->nivel, capturado);
    }

    fclose(fichero);
    return false;
}

int pokedex_apagar(pokedex_t* pokedex){
    if(!pokedex){
        return ERROR;
    }

    FILE* fichero;

    fichero = fopen("pokedex.txt", "wt");
    fprintf(fichero, "%s\n", pokedex->nombre_entrenador);
    fclose(fichero);

    abb_con_cada_elemento(pokedex->pokemones, ABB_RECORRER_PREORDEN, guardar_pokedex, NULL);

    return EXITO;
}

pokedex_t* pokedex_prender(){

    especie_pokemon_t especie_leida;
    particular_pokemon_t pokemon_leido;
    char name[MAX_NOMBRE];
    char especie;
    char capturado;
    pokedex_t* pokedex;
    
    FILE *pokemones_f = fopen("pokedex.txt", "r");
    if (pokemones_f == NULL) {
        return NULL;
    }
    
    int leidos = fscanf(pokemones_f, FORMATO_LECTURA_ENTRENADOR, name);

    if(leidos == 1){
      pokedex = pokedex_crear(name);

      if(!pokedex){
        return NULL;
      }
        
        while((leidos = fscanf(pokemones_f, "%c", &especie)) == 1){
            if(especie == ESPECIE){
                while((leidos = fscanf(pokemones_f, FORMATO_LECTURA_ESPECIES, especie_leida.nombre, &especie_leida.numero, especie_leida.descripcion)) == 3){
                    int agregado_abb = agregar_especie(pokedex, especie_leida);
                    
                    if(agregado_abb == ERROR){
                        fclose(pokemones_f);
                        return NULL;
                    }
                }
            }else{
                while((leidos = fscanf(pokemones_f, FORMATO_LECTURA_POKEMONES, pokemon_leido.nombre, &pokemon_leido.nivel, &capturado)) == 3){
                    if(capturado != CAPTURADO){
                        pokemon_leido.capturado = false;
                    }else{
                        pokemon_leido.capturado = true;
                    }
                    
                    int agregado_abb = agregar_particular(pokedex, especie_leida, pokemon_leido);
                    if(agregado_abb == ERROR){
                        fclose(pokemones_f);
                        return NULL;
                    }

                    int agregado_listas = agregar_pokemon_listas(pokedex, pokemon_leido);
                    if(agregado_listas == ERROR){
                        fclose(pokemones_f);
                        return NULL;
                    }
                }
            }
        }
    }
    fclose(pokemones_f);
    
    return pokedex;
}