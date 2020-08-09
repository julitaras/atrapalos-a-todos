#include "pokedex.h"
#include <stdio.h>
#define ERROR -1
#define COLOR_VIOLETA "\x1b[35;1m"
#define COLOR_NORMAL   "\x1b[0m"
#define COLOR_ROJO	   "\x1b[1m\x1b[31m"
#define COLOR_VERDE   "\x1b[1m\x1b[32m"
#define COLOR_BLANCO   "\x1b[37m\x1b[1m"
#define B_COLOR_BLANCO "\x1b[40m"
#define B_COLOR_NORMAL   "\x1b[0m"

void mostrar_ayuda(pokedex_t** pkd);

/*Se encarga de prender la pokedex y de saber cuando describirse*/
bool comando_encender(char letra, pokedex_t** pkd, bool describir){
    if(describir){
        if(*pkd){
            return false;
        }
        printf(B_COLOR_BLANCO "I:" B_COLOR_NORMAL COLOR_BLANCO " Inicia la pokedex\n");
        return false;
    }

    if(letra != 'I'){
        return false;
    }

    if(!(*pkd)){
        pokedex_t* pk = pokedex_prender();
        
        if(pk){
            printf(COLOR_VERDE "Pokedex encendida con exito!\n\n");
            *pkd = pk;
            return true;
        }
        printf(COLOR_ROJO "\nUpss...Hubo un error al abrir el archivo pokedex.txt\n");

        char nombre[MAX_NOMBRE];
        printf(COLOR_NORMAL "\nIngresa tu nombre para inicializar la pokedex:\n");
        scanf("%s", nombre);
        pk = pokedex_crear(nombre);
        if(pk){
            printf(COLOR_VERDE "\nPokedex encendida con exito!\n\n");
            *pkd = pk;
        }

        return true;
    }

    return false;
}

/*Se encarga de evolucionar la pokedex y de saber cuando describirse*/
bool comando_evolucionar(char letra, pokedex_t** pkd, bool describir){

    if(describir){
        if(*pkd && !arbol_vacio((*pkd)->pokemones)){
            printf(B_COLOR_BLANCO "E:" B_COLOR_NORMAL COLOR_BLANCO " Actualiza la Pokedex evolucionando los Pokémon que así lo hayan hecho\n");
            return false;
        }

        return false;
    }

    if(letra != 'E'){
        return false;
    }

    if(!pkd){
        printf(COLOR_ROJO"\nComando invalido. Por favor seleccione un comando valido.\n\n");
        return true;
    }
    
    if(*pkd && !arbol_vacio((*pkd)->pokemones)){
        char* ruta_evoluciones = "evoluciones.txt";
        int exito = pokedex_evolucionar(*pkd, ruta_evoluciones);
        if(exito == ERROR){
            printf(COLOR_ROJO "\nUpss... Hubo un error al evolucionar los pokemones, reintente mas tarde... \n");
            return false;
        }

        printf(COLOR_VERDE "\nPokedex evolucionada con exito!\n\n");

        return true;
    }

    return false;
}

/*Se encarga de apagar la pokedex y de saber cuando describirse*/
bool comando_apagar(char letra, pokedex_t** pkd, bool describir){

    if(describir){
        if(*pkd){
            printf(B_COLOR_BLANCO "G:" B_COLOR_NORMAL COLOR_BLANCO " Guarda la pokedex\n");
            return false;
        }

        return false;
    }

    if(letra != 'G'){
        return false;
    }

    if(!pkd){
        printf(COLOR_ROJO"\nComando invalido. Por favor seleccione un comando valido.\n\n");
        return true;
    }

    if(*pkd){
        int exito = pokedex_apagar(*pkd);
        if(exito == ERROR){
            printf(COLOR_ROJO "\nUpss... Hubo un problema al guardar la pokedex, reintente mas tarde...\n");
            return true;
        }
        pokedex_destruir(*pkd);
        *pkd = NULL;
        printf(COLOR_VERDE "\nPokedex guardada con exito!\n\n");
        
        return true;
    }

    return false;
}

/*Se encarga de avistar la pokedex y de saber cuando describirse*/
bool comando_avistar(char letra, pokedex_t** pkd, bool describir){
    
    if(describir){
        if(*pkd && arbol_vacio((*pkd)->pokemones)){
            printf(B_COLOR_BLANCO "A:" B_COLOR_NORMAL COLOR_BLANCO " Actualiza la Pokedex incorporando a ella los Pokémon avistados \n");
            return false;
        }

        return false;
    }

    if(letra != 'A'){
        return false;
    }

    if(!pkd){
        printf(COLOR_ROJO"\nComando invalido. Por favor seleccione un comando valido.\n\n");
        return true;
    }
    
    if(*pkd && arbol_vacio((*pkd)->pokemones)){
        char* ruta_avistamientos = "avistamientos.txt";
        int exito = pokedex_avistar(*pkd, ruta_avistamientos);
        if(exito == ERROR){
            printf(COLOR_ROJO "\nUpss... Hubo un problema al avistar la pokedex, reintente mas tarde...\n");
            return true;
        }

        printf(COLOR_VERDE "\nPokedex avistada con exito!\n\n");
        return true;
    }
    return false;
}

/*Se encarga de mostrar los ultimos capturados y de saber cuando describirse*/
bool comando_ultimos_capturados(char letra, pokedex_t** pkd, bool describir){
    if(describir){
        if(*pkd && !lista_vacia((*pkd)->ultimos_capturados)){
            printf(B_COLOR_BLANCO "C:" B_COLOR_NORMAL COLOR_BLANCO " Muestra los últimos Pokémones capturados \n");
            return false;
        }
        return false;
    }

    if(letra != 'C'){
        return false;
    }

    if(!pkd){
        printf(COLOR_ROJO"\nComando invalido. Por favor seleccione un comando valido.\n\n");
        return true;
    }

    if(*pkd && !lista_vacia((*pkd)->ultimos_capturados)){
        pokedex_ultimos_capturados(*pkd);

        return true;
    }

    return false;
}

/*Se encarga de mostrar los ultimos pokemones vistos y de saber cuando describirse*/
bool comando_ultimos_vistos(char letra, pokedex_t** pkd, bool describir){
    if(describir){
        if( *pkd && !lista_vacia((*pkd)->ultimos_vistos)){
            printf(B_COLOR_BLANCO "V:" B_COLOR_NORMAL COLOR_BLANCO " Muestra los últimos Pokémones vistos \n");
            return false;
        }

        return false;
    }

    if(letra != 'V'){
        return false;
    }

    if(!pkd){
        printf(COLOR_ROJO"\nComando invalido. Por favor seleccione un comando valido.\n\n");
        return true;
    }
    
    if(*pkd && !lista_vacia((*pkd)->ultimos_vistos)){
        pokedex_ultimos_vistos(*pkd);

        return true;
    }

    return false;
}

/*Se encarga de mostrar la informacion de una especie y de saber cuando describirse*/
bool comando_informacion_especie(char letra, pokedex_t** pkd, bool describir){
    if(describir){
        if(*pkd){
            printf(B_COLOR_BLANCO "M:" B_COLOR_NORMAL COLOR_BLANCO " Muestra la información de la especie \n");
            return false;
        }

        return false;
    }

    if(letra != 'M'){
        return false;
    }

    if(!pkd){
        printf(COLOR_ROJO"\nComando invalido. Por favor seleccione un comando valido.\n\n");
        return true;
    }
    
    if(*pkd){
        int numero;

        printf("Ingrese numero de especie que quiere buscar:\n");
        scanf("%i", &numero);

        pokedex_informacion(*pkd, numero, "");

        return true;
    }

    return false;
}

/*Se encarga de mostrar la informacion de un pokemon, de saber cuando describirse*/
bool comando_informacion_pokemon(char letra, pokedex_t** pkd, bool describir){
    if(describir){
        if(*pkd){
            printf(B_COLOR_BLANCO "P:" B_COLOR_NORMAL COLOR_BLANCO " Muestra la información de un Pokémon de una determinada especie \n");
            return false;
        }

        return false;
    }

    if(letra != 'P'){
        return false;
    }

    if(!pkd){
        printf(COLOR_ROJO"\nComando invalido. Por favor seleccione un comando valido.\n\n");
        return true;
    }
    
    if(*pkd){
        int numero;
        char nombre_pokemon[MAX_NOMBRE];

        printf("Ingrese numero de especie que quiere buscar:\n");
        scanf("%i", &numero);
        printf("Ingrese nombre de pokemon que quiere buscar:\n");
        scanf("%s", nombre_pokemon);

        pokedex_informacion(*pkd, numero, nombre_pokemon);

        return true;
    }

    return false;
}

/*Se encarga de salir del programa y de saber cuando describirse*/
bool comando_salir(char letra, pokedex_t** pkd, bool describir){
    if(describir){
        printf(B_COLOR_BLANCO "S:" B_COLOR_NORMAL COLOR_BLANCO " Salir del programa \n");
        return false;
    }

    if(letra != 'S'){
        return false;
    }
    pokedex_destruir(*pkd);
    exit(-1);
    return true;
}

/*Muestra los comandos disponibles y de saber cuando describirse*/
bool comando_ayuda(char letra, pokedex_t** pkd, bool describir){
    if(describir){
        printf(B_COLOR_BLANCO "H:" B_COLOR_NORMAL COLOR_BLANCO " Lista de comandos \n");
        return false;
    }

    if(letra != 'H'){
        return false;
    }

    mostrar_ayuda(pkd);
    return true;
}

bool (*comandos[])(char, pokedex_t**, bool) = {comando_encender , comando_apagar, comando_evolucionar, comando_avistar, comando_ultimos_capturados, comando_ultimos_vistos, comando_informacion_especie, comando_informacion_pokemon, comando_salir, comando_ayuda};

/*Se encarga de llamr a las funciones haciendo que se ejecute la correspondiente*/
void despachar_comando(char letra, pokedex_t** pkd){
    bool ejecutando = false;
    for (size_t i = 0; !ejecutando && comandos[i]; i++){
        ejecutando = comandos[i](letra, pkd, false);
    }
    if(!ejecutando){
        printf(COLOR_ROJO"\nComando invalido. Por favor seleccione un comando valido.\n\n");
    }
}

/*Se engarga de llamar a las funciones para que se describan*/
void mostrar_ayuda(pokedex_t** pkd){
    bool ejecutando = false;
    printf(COLOR_BLANCO "\nEstas opciones son las que tenes habilitadas:\n");
    for (size_t i = 0; !ejecutando && comandos[i]; i++){
        ejecutando = comandos[i](0, pkd, true);
    }   
}

int main(){
    bool salir = false;
    printf(COLOR_VIOLETA "\n\t\t\t\t ¡Bienvenido a la Pokedex! \n\t La Pokedex es una enciclopedia virtual portátil de alta tecnología que los entrenadores Pokémon llevan consigo, \n\t para registrar la información de todas las diversas especies Pokémon con las que se encuentran durante su viaje como entrenadores.\n\n\t\t\t\t\t¡Comencemos!\n\n");
    pokedex_t** pkd;
    pokedex_t* pk = NULL;
    pkd = &pk;
    do{
        char letra;
        mostrar_ayuda(pkd);
        printf(COLOR_NORMAL "\nIngresa la opcion:\n");
        scanf("%s", &letra);
        despachar_comando(letra, pkd);
    }while(!salir);
    pokedex_destruir(*pkd);
    return 1;
}