#include "../include/deadlock.h"


int hayDeadlock(List listaCandidatos){

	//le digo al planificador si hay entrenadores listos para intercambiar
	int resultado = isemptylist(listaCandidatos);

	if(resultado == 0)
	{
		return 1;
	}

	return 0;
}

t_entrenador* buscarEntrenadorParaIntercambiar(t_entrenador* entrenador, List candidatos){

	List listaObjetivos;
	listaObjetivos = misObjetivosSinAtrapar(entrenador);
	IteratorList iterator = beginlist(listaObjetivos);
	char* pokemonActual = iterator->data;

	while(iterator != NULL){

		pokemonActual = iterator->data;

		IteratorList iteratorCandidatos = beginlist(candidatos);
		t_entrenador* entrenadorActual = iteratorCandidatos->data;

		while(iteratorCandidatos != NULL){

			entrenadorActual = iteratorCandidatos->data;

			if(cantidadAtrapadosEntrenador(pokemonActual, entrenadorActual) > cantidadObjetivosDeEntrenador(pokemonActual, entrenadorActual)){

					entrenador->nombrePokemonAIntercambiar = pokemonActual;
					entrenadorActual->nombrePokemonAIntercambiar = pokemonAtrapadoNoObjetivo(entrenador);
					return entrenadorActual;
			}

			iteratorCandidatos = nextlist(iteratorCandidatos);

		}

		iterator = nextlist(iterator);
	}

	return NULL;
}


int cantidadObjetivosDeEntrenador(char* pokemon,t_entrenador* entrenador)
{
	int resultado = 0;
	char** array_objetivos =  string_split(entrenador->pokemones_objetivo, "|");

	for(int b=0; b < len_array(array_objetivos); b++){
		char* pokemon_en_config = array_objetivos[b];
		if(!strcmp(pokemon_en_config,pokemon))
		{
			resultado++;
		}
	}

	return resultado;
}

char* pokemonAtrapadoNoObjetivo(t_entrenador* entrenador)
{
	List listaAtrapados = misPokemonesAtrapados(entrenador);

	IteratorList iterator = beginlist(listaAtrapados);
	char* pokemonAtrapadoActual = iterator->data;

	while(iterator != NULL){
		pokemonAtrapadoActual = iterator->data;

		if(cantidadAtrapadosEntrenador(pokemonAtrapadoActual, entrenador) > cantidadObjetivosDeEntrenador(pokemonAtrapadoActual, entrenador)){

			return pokemonAtrapadoActual;
		}

		iterator = nextlist(iterator);

	}

	return NULL;
}

List candidatosADeadlock(t_entrenador* entrenador){
	List resultados;
	initlist(&resultados);

	for(int i=0 ; i < cantidadEntrenadores  ; i++)
	{
		if(entrenadores[i]->id != entrenador->id && entrenadores[i]->estadoEntrenador == BLOCKED && entrenadores[i]->objetivoActual == NULL && entrenadores[i]->esperandoIntercambiar == 1 && entrenadores[i]->entrenadorVoyAIntercambiar == NULL)
		{
			pushfrontlist(&resultados,entrenadores[i]);
		}
	}

	return resultados;
}

t_entrenador * quienTieneMisPokemones(t_entrenador* entrenador, List listaCandidatos){

	List listaObjetivos;
	listaObjetivos = misObjetivosSinAtrapar(entrenador);
	IteratorList iterator = beginlist(listaObjetivos);
	char* pokemonActual = iterator->data;

	while(iterator != NULL){

		pokemonActual = iterator->data;

		IteratorList iteratorCandidatos = beginlist(listaCandidatos);
		t_entrenador* entrenadorActual = iteratorCandidatos->data;

		while(iteratorCandidatos != NULL){

			if(cantidadAtrapadosEntrenador(pokemonActual, entrenadorActual) > 0){
				return entrenadorActual;
			}

			iteratorCandidatos = nextlist(iteratorCandidatos);

		}

		iterator = nextlist(iterator);
	}

	return NULL;
}

List misObjetivosSinAtrapar(t_entrenador* entrenador)
{
	List objetivos;
	initlist(&objetivos);

	char** array_objetivos =  string_split(entrenador->pokemones_objetivo, "|");

	List atrapados = misPokemonesAtrapados(entrenador);

	for(int a=0; a < len_array(array_objetivos); a++){
		if(!estaEnLaLista(array_objetivos[a],atrapados)){
			pushfrontlist(&objetivos,array_objetivos[a]);
		}
	}

	return objetivos;
}

int estaEnLaLista(char* objetivo,List lista){
	int cont = 0;


	if(sizelist(lista) > 0)
	{
		IteratorList iteratorAtrapados = beginlist(lista);
		char* pokemonActual = iteratorAtrapados->data;

		while(iteratorAtrapados != NULL){
			pokemonActual = iteratorAtrapados->data; //TESTING!
			if(!strcmp(objetivo, pokemonActual)){
				cont++;
			}
			iteratorAtrapados = nextlist(iteratorAtrapados);
		}
	}
	return cont;
}

List misPokemonesAtrapados(t_entrenador* entrenador)
{
	List atrapados;
	initlist(&atrapados);

	if(entrenador->pokemones_atrapados != NULL)
	{
		char** array_atrapados =  string_split(entrenador->pokemones_atrapados, "|");

		for(int a=0; a < len_array(array_atrapados); a++){

			pushfrontlist(&atrapados,array_atrapados[a]);
		}
	}

	return atrapados;
}

List misPokemonesObjetivos(t_entrenador* entrenador)
{
	List objetivos;
	initlist(&objetivos);

	char** array_objetivos =  string_split(entrenador->pokemones_objetivo, "|");

	for(int a=0; a < len_array(array_objetivos); a++){

		pushfrontlist(&objetivos,array_objetivos[a]);
	}

	return objetivos;
}

int cantidadAtrapadosEntrenador(char* pokemon,t_entrenador* entrenador)
{
	//Es como la que tengo en appearedPokemonHandler.c pero pasandole un entrenador
	int result = 0;

	if (entrenador->pokemones_atrapados != NULL){
		char** array_atrapados =  string_split(entrenador->pokemones_atrapados, "|");
		for(int a=0; a < len_array(array_atrapados);a++){
			if(!strcmp(array_atrapados[a],pokemon))
			{
				result++;
			}
			}
		}

	return result;
}


