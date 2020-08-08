#include "pokedex.h"
#include <stdio.h>


bool comando_encender(char letra, pokedex_t** pkd, bool describir){
    if(describir){
        printf("P: Enciende la pokedex");
        return false;
    }

    if(letra != 'P'){
        return false;
    }

    if(*pkd){
        printf("Comando invalido\n");
        return true;
    }

    *pkd = pokedex_prender();
    return true;
}

bool (*comandos[])(char, pokedex_t**, bool) = {comando_encender};

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
    mostrar_ayuda();
    return 1;
}