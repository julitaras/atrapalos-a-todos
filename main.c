#include "pokedex.h"
#include <stdio.h>
#define ERROR -1


bool comando_encender(char letra, pokedex_t** pkd, bool describir){
    if(describir){
        printf("\t I: Inicia la pokedex\n");
        return false;
    }

    if(letra != 'I'){
        return false;
    }

    if(*pkd){
        printf("Comando invalido\n");
        return true;
    }

    *pkd = pokedex_prender();
    return true;
}

bool comando_evolucionar(char letra, pokedex_t** pkd, bool describir){
    if(describir){
        printf("\t E: Actualiza la Pokedex evolucionando los Pokémon que así lo hayan hecho\n");
        return false;
    }

    if(letra != 'E'){
        return false;
    }

    if(!(*pkd)){
        printf("\t Comando invalido\n");
        return true;
    }

    char* ruta_evoluciones = "evoluciones.txt";
    int exito = pokedex_evolucionar(*pkd, ruta_evoluciones);
    if(exito == ERROR){
        printf("\t Upss... Hubo un error al evolucionar los pokemones, reintente mas tarde... \n");
        return false;
    }

    return true;
}

bool comando_apagar(char letra, pokedex_t** pkd, bool describir){
    if(describir){
        printf("\t G: Guarda la pokedex\n");
        return false;
    }

    if(letra != 'G'){
        return false;
    }

    if(!(*pkd)){
        printf("\t Comando invalido\n");
        return true;
    }

    
    int exito = pokedex_apagar(*pkd);
    if(exito == ERROR){
        printf("\t Upss... Hubo un problema al guardar la pokedex, reintente mas tarde...\n");
        return true;
    }

    pokedex_destruir(*pkd);
    return true;
}

bool comando_avistar(char letra, pokedex_t** pkd, bool describir){
    if(describir){
        printf("\t A: Actualiza la Pokedex incorporando a ella los Pokémon avistados \n");
        return false;
    }

    if(letra != 'A'){
        return false;
    }

    if(!(*pkd)){
        printf("\t Comando invalido\n");
        return true;
    }

    char* ruta_avistamientos = "avistamientos.txt";
    int exito = pokedex_avistar(*pkd, ruta_avistamientos);
    if(exito == ERROR){
        printf("\t Upss... Hubo un problema al avistar la pokedex, reintente mas tarde...\n");
        return true;
    }

    return true;
}

bool comando_ultimos_capturados(char letra, pokedex_t** pkd, bool describir){
    if(describir){
        printf("\t C: Muestra los últimos Pokémones capturados \n");
        return false;
    }

    if(letra != 'C'){
        return false;
    }

    if(!(*pkd)){
        printf("\t Comando invalido\n");
        return true;
    }
    
    pokedex_ultimos_capturados(*pkd);

    return true;
}

bool comando_ultimos_vistos(char letra, pokedex_t** pkd, bool describir){
    if(describir){
        printf("\t V: Muestra los últimos Pokémones vistos \n");
        return false;
    }

    if(letra != 'V'){
        return false;
    }

    if(!(*pkd)){
        printf("\t Comando invalido\n");
        return true;
    }
    
    pokedex_ultimos_vistos(*pkd);

    return true;
}

bool comando_informacion_especie(char letra, pokedex_t** pkd, bool describir){
    if(describir){
        printf("\t M: Muestra la información de la especie \n");
        return false;
    }

    if(letra != 'M'){
        return false;
    }

    if(!pkd){
        printf("\t Comando invalido\n");
        return true;
    }
    
    int numero;

    printf("\t Ingrese numero de especie que quiere buscar:\n");
    scanf("%i", &numero);

    pokedex_informacion(*pkd, numero, "");

    return true;
}

bool comando_informacion_pokemon(char letra, pokedex_t** pkd, bool describir){
    if(describir){
        printf("\t P: Muestra la información de un Pokémon de una determinada especie \n");
        return false;
    }

    if(letra != 'P'){
        return false;
    }

    if(!pkd){
        printf("\t Comando invalido\n");
        return true;
    }
    
    int numero;
    char nombre_pokemon[MAX_NOMBRE];

    printf("\t Ingrese numero de especie que quiere buscar:\n");
    scanf("%i", &numero);
    printf("\t Ingrese nombre de pokemon que quiere buscar:\n");
    scanf("%s", nombre_pokemon);

    pokedex_informacion(*pkd, numero, nombre_pokemon);

    return true;
}

bool (*comandos[])(char, pokedex_t**, bool) = {comando_encender, comando_apagar, comando_evolucionar, comando_avistar, comando_ultimos_capturados, comando_ultimos_vistos, comando_informacion};

void despachar_comando(char letra, pokedex_t** pkd){
    bool ejecutando = false;
    for (size_t i = 0; !ejecutando && comandos[i]; i++){
        ejecutando = comandos[i](letra, pkd, false);
        if(!ejecutando){
            printf("Comando invalido\n");
        }
    }
}

void mostrar_ayuda(){
    bool ejecutando = false;
    for (size_t i = 0; !ejecutando && comandos[i]; i++){
        ejecutando = comandos[i](0, NULL, true);
    }   
}

int main(){
//     char* ruta_avistamientos = "avistamientos.txt";
//     char* ruta_evoluciones = "evoluciones.txt";
//     pokedex_t* pokedex = pokedex_crear("JULIETA");
//     int exito = pokedex_avistar(pokedex, ruta_avistamientos);
    
// //     char* nombre = "";
// //     pokedex_informacion(pokedex, 25, nombre);
// //     nombre = "charly";
// //     pokedex_informacion(pokedex, 1, nombre);
// //     
// //     nombre = "charly";
// //     pokedex_informacion(pokedex, 12, nombre);
//  pokedex_apagar(pokedex);
//     pokedex_destruir(pokedex);
//     pokedex = pokedex_prender();
//     char* nombre = "Danaus";
//     pokedex_informacion(pokedex, 12, nombre);
//     exito = pokedex_evolucionar(pokedex, ruta_evoluciones);
// //    printf("Nombre: %s\n", pokedex->nombre_entrenador);
// //     // pokedex_ultimos_capturados(pokedex);
// //     // pokedex_ultimos_vistos(pokedex);
//     pokedex_destruir(pokedex);
//     printf("%i", exito);

    char letra;
    pokedex_t** pkd = NULL;
    mostrar_ayuda();
    scanf("%c", &letra);
    despachar_comando(letra, pkd);
    return 1;
}