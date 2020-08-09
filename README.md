


<h1 align="center">Trabajo Practico 2</h3>

---

<p align="center"> Desarrollo del TP2 - Atrapalos a todos
    <br> 
</p>

## 📝 Contenido

- [Enunciado](#enunciado)
- [Compilacion y Ejecucion](#compilacion_ejecucion)
- [Introducion](#introduccion)

## Enunciado <a name = "enunciado"></a>

Se pide crear el TDA Pokedex y un programa que simule el uso de la misma.

La Pokedex debe contener la información de todas las especies y de los Pokémon particulares vistos por el entrenador
dueño de dicho aparato.

Para esto se solicita que se haga uso de las estructuras ABB y Lista implementados con anterioridad (puede agregar
primitivas o modificar las existentes, justificando en el readme sin excepción).

Las especies de Pokémon vistas por el entrenador deberán ser almacenadas en un árbol (ordenadas por número de
Pokémon).

A su vez, por cada especie Pokémon, se almacena un listado de todos los Pokémon avistados y capturados de dicha
especie.

La información en la Pokedex podrá ser actualizada de dos formas diferentes: mediante el avistamiento de Pokémon y
mediante la evolución de Pokémon.

Cuando se avista un Pokémon, se debe verificar si la especie a la que pertenece está o no almacenada en la Pokedex.
Si no existe, debe agregarse la información de la especie para luego almacenar los datos particulares del Pokémon en
cuestión. Si la especie del Pokémon ya estaba presente, solamente deben agregarse los datos del Pokémon particular al
listado de Pokémon de la especie.

Al evolucionar un Pokémon particular (cambio de especie), dicho Pokémon es removido de el listado de Pokémon conocidos
en su especie y deben pasar al listado los Pokémon conocidos de la nueva especie. Por ejemplo, si el entrenador tiene un
Pokémon de la especie Pikachu identificado como Pikante, al evolucionar, Pikante deja de ser parte de la lista de Pikachus
y pasa a ser parte de la lista de Raichus.

La información de la especie Pikachu no deberá desaparecer de la Pokedex por más de que su lista de Pokémon esté
vacía (esto puede ocurrir al evolucionar un Pokémon único en su especie).

Como información extra, la Pokedex almacena en una pila los Pokémon mas recientemente capturados y en una cola los
Pokémon mas recientemente avistados. Esta información funciona a modo de historial y es limpiado cada vez que se lo
consulta. Por ejemplo si se quieren listar los últimos Pokémon avistados, se mostrará el listado pedido, pero al mismo
tiempo estos Pokémon seran removidos del historial.

Por último, existe la posibilidad de prender y apagar la Pokedex. Al apagarse, la información almacenada en memoria se
guarda a un archivo (pokedex.txt) y al prenderse la información es recuperada de ese mismo archivo.

## 🏁 Compilacion y Ejecucion <a name = "compilacion_ejecucion"></a>

- ¿Cómo compilamos?
    `````c
    gcc pokedex.c lista.c abb.c main.c  pokedex_pruebas.c -Wall -Werror -Wconversion -std=c99 -o pokedex -g
    `````

- ¿Cómo ejecutamos ?

    `````c
        ./pokedex
    `````
## Introduccion 

- El programa se encarga de crear, cargar, mostrar una pokedex.
