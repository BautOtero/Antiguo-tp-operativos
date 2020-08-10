#include "../include/localizedPokemonHandler.h"

int localizedPokemon(char* pokemon, List listaCoordenadas){

/*	for (IteratorList iterator = beginlist(pokemonesRecibidos); iterator != NULL; iterator = iterator->next){
		char * pokemonQueYaFueRecibido = iterator->data;

		if (!strcmp(pokemon, pokemonQueYaFueRecibido)){
			return 0;
		}
	}*/

	for (IteratorList iterator = beginlist(listaCoordenadas); iterator != NULL; iterator = iterator->next){
		Coordenadas * coordenadas = iterator->data;

		appearedPokemon(pokemon, coordenadas->posX, coordenadas->posY);

		free(coordenadas);
	}

	return 0;
}
