#include "../include/appearedPokemonHandler.h"


int appearedPokemon(char* pokemon, int posx, int posy){

	sem_wait(m_listaObjetivos);

	if (requierePokemon(pokemon) == 1){
		pushbacklist(&pokemonesRecibidos, pokemon);
		agregarPokemon(pokemon, posx, posy);
		sacarDeListaPorNombre(&pokemonesObjetivosPendientes, pokemon);
	}

	sem_post(m_listaObjetivos);

	return 0;
}

int requierePokemon(char * pokemon){

//	if(cantidadAtrapados(pokemon) < cantidadObjetivos(pokemon))
//	{
//		return 1;
//	}
//	else
//	{
//		return 0;
//	};
	int r = 0;
	for(IteratorList i = beginlist(pokemonesObjetivosPendientes) ; i != NULL; i = nextlist(i))
	{
		char* nombreActual = i->data;
		if(!strcmp(nombreActual,pokemon))
		{
			r = 1;
			break;
		}
	}

	return r;
}

int agregarPokemon(char* pokemon, int posx, int posy){

	t_pokemon* pok = malloc(sizeof(t_pokemon));

	pok->nombre = malloc(strlen(pokemon) + 1);
	strcpy(pok->nombre,pokemon);

	pok->cordX = posx;
	pok->cordY = posy;

	pushbacklist(&pokemonesAAtrapar,pok);

	sem_post(pokemonesAAtraparContador);

	return 0;
}

void buscarEntrenadorYPasarAReady(){
	while(1){
		sem_wait(pokemonesAAtraparContador);

		t_pokemon* pok = popfrontlist(&pokemonesAAtrapar);

		sem_wait(m_pasarEstado);
		t_entrenador* entrenadorMasCercano = buscarEntrenadorMasCercano(pok);
		entrenadorMasCercano->objetivoActual = pok;
		pasarReady(entrenadorMasCercano);
		sem_post(m_pasarEstado);

		printf("Elegi a Entrenador %d para capturar a %s\n", entrenadorMasCercano->id, entrenadorMasCercano->objetivoActual->nombre);
		//free(entrenadorMasCercano);
	}
}

t_entrenador* buscarEntrenadorMasCercano(t_pokemon* pok)
{
	List entrenadoresLibres;
	initlist(&entrenadoresLibres);
	t_entrenador* entrenadorResultado = malloc(sizeof(t_entrenador));
	int distanciaTotalResultado = 9999;

	sem_wait(sem_entrenadores_libres);

	for(int i=0 ; i < cantidadEntrenadores  ; i++)
	{
		if(entrenadores[i]->estadoEntrenador == NEW || (entrenadores[i]->estadoEntrenador == BLOCKED && entrenadores[i]->objetivoActual == NULL && entrenadores[i]->esperandoIntercambiar == 0))
		{
			pushbacklist(&entrenadoresLibres,entrenadores[i]);
		}
	}

	IteratorList iterator = beginlist(entrenadoresLibres);
	t_entrenador* entrenadorActual;
	while(iterator != NULL)
	{
		entrenadorActual = iterator->data;
		int distanciaL = distancia(*pok, *entrenadorActual);
		if( distanciaL < distanciaTotalResultado)
		{
			entrenadorResultado = iterator->data;
			distanciaTotalResultado = distanciaL;
		}
		iterator = nextlist(iterator);
	}

	return entrenadorResultado;
}

int distancia(t_pokemon pok, t_entrenador entrenador)
{
	int result = 0;
	int ent_x = atoi(string_split(entrenador.coordenadas, "|")[0]);
	int ent_y = atoi(string_split(entrenador.coordenadas, "|")[1]);

	if(pok.cordX > ent_x)
	{
		result += (pok.cordX - ent_x);
	}
	else
	{
		result += (ent_x - pok.cordX);
	}

	if(pok.cordY > ent_y)
	{
		result += (pok.cordY - ent_y);
	}
	else
	{
		result += (ent_y - pok.cordY);
	}

	return result;
}

int cantidadAtrapados(char* pokemon)
{
	int a,b;
	int result = 0;
	for(a=0; a < cantidadEntrenadores;a++){
		if (entrenadores[a]->pokemones_atrapados != NULL){
			char** array_atrapados =  string_split(entrenadores[a]->pokemones_atrapados, "|");
			for(b=0; b < len_array(array_atrapados);b++){
				if(!strcmp(array_atrapados[b],pokemon))
				{
					result++;
				}
			}
		}
	}
	return result;
}

int cantidadObjetivos(char* pokemon)
{
	int a,b;
	int resultado = 0;
	for(a=0; a < cantidadEntrenadores;a++){

		char** array_objetivos =  string_split(entrenadores[a]->pokemones_objetivo, "|");

		for(b=0; b < len_array(array_objetivos); b++){
			char* pokemon_en_config = array_objetivos[b];
			if(!strcmp(pokemon_en_config,pokemon))
			{
				resultado++;
			}
		}
	}
	return resultado;
}

