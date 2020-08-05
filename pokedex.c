#include "pokedex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EXITO 0
#define ERROR -1
#define FORMATO_LECTURA_AVISTAMIENTOS "%i;%[^;];%[^;];%[^;];%i;%c\n"
#define FORMATO_LECTURA_EVOLUCIONES "%i;%[^;];%i;%[^;];%[^\n]\n"
#define ABB_RECORRER_PREORDEN 1

/*Funcion comparadora de elementos*/
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
    //revisar si queda lindo la func 

    particular_pokemon_t* particular = malloc(sizeof(particular_pokemon_t));
    if(!particular){
        free(especie);
        return ERROR;
    }

    strcpy(particular->nombre, nuevo_particular.nombre);
    particular->nivel = nuevo_particular.nivel;
    particular->capturado = nuevo_particular.capturado;
    
    int exito_lista = lista_insertar(especie->pokemones, particular);
    if(exito_lista == ERROR){
        free(particular);
        free(especie);
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

        especie_pokemon_t* especie_actual = arbol_buscar(pokedex->pokemones, &actual_especie);
        for (size_t i = 0; i < lista_elementos(especie_actual->pokemones); i++){
            particular_pokemon_t* elem = lista_elemento_en_posicion(especie_actual->pokemones, i); 

            if(strcmp(elem->nombre, particular_pokemon.nombre) == 0){
                especie->pokemones = lista_crear();
                if(!especie->pokemones){
                    return ERROR;
                }
                
                int exito = lista_insertar(especie->pokemones, elem);
                if(exito == ERROR){
                    return ERROR;
                }

                int err = lista_borrar_de_posicion(especie_actual->pokemones, i);
                if(err == ERROR){
                    return ERROR;
                }
            }
        }

        int ver = arbol_insertar(pokedex->pokemones, especie);
        if(ver == ERROR){
            return ERROR;
        }

    }else{
        especie_pokemon_t* especie_actual = arbol_buscar(pokedex->pokemones, &actual_especie);
        
        for (size_t i = 0; i < lista_elementos(especie_actual->pokemones); i++){
            
            particular_pokemon_t* elem = lista_elemento_en_posicion(especie_actual->pokemones, i); 

            if(strcmp(elem->nombre, particular_pokemon.nombre) == 0){
                
                int exito = lista_insertar(especie->pokemones, elem);
                if(exito == ERROR){
                    return ERROR;
                }

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

bool guardar(void* elem, void* extra){
    if(!elem){
        return true;
    }

    FILE* fichero;

    fichero = fopen("pokedex.txt", "a");
    
    especie_pokemon_t* aux = ((especie_pokemon_t*) elem);
    fprintf(fichero, "E;%i;%s;%s\n",  aux->numero, aux->nombre, aux->descripcion);
    
    for (size_t i = 0; i < lista_elementos(aux->pokemones); i++){
        particular_pokemon_t* elem = lista_elemento_en_posicion(aux->pokemones, i);
        
        char capturado;
        
        if(elem->capturado){
            capturado = 'S';
        }else{
            capturado = 'N';
        }

        fprintf(fichero, "P;%i;%s;%c\n",  elem->nivel, elem->nombre, capturado);
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
    
    abb_con_cada_elemento(pokedex->pokemones, ABB_RECORRER_PREORDEN, guardar, NULL);

    return EXITO;
}

int main(){
    char* ruta_avistamientos = "avistamientos.txt";
    // char* ruta_evoluciones = "evoluciones.txt";
    pokedex_t* pokedex = pokedex_crear("JULIETA");
    int exito = pokedex_avistar(pokedex, ruta_avistamientos);
    
    // char* nombre = "";
    // pokedex_informacion(pokedex, 25, nombre);
    // nombre = "charly";
    // pokedex_informacion(pokedex, 1, nombre);
    // nombre = "Danaus";
    // pokedex_informacion(pokedex, 12, nombre);
    // nombre = "charly";
    // pokedex_informacion(pokedex, 12, nombre);
    pokedex_apagar(pokedex);
    //pokedex_destruir(pokedex);
    // exito = pokedex_evolucionar(pokedex, ruta_evoluciones);
    // pokedex_ultimos_capturados(pokedex);
    // pokedex_ultimos_vistos(pokedex);
    printf("%i", exito);
    return 1;
}