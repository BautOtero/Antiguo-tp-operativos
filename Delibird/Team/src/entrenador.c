#include "../include/entrenador.h"

void entrenadorMain(void *esteEntrenador)
{
	t_entrenador* localEnt = (t_entrenador *)esteEntrenador;

	printf("[HILO - Entrenador %d]: nuevo hilo creado para entrenador %d \n",localEnt->id,localEnt->id);
	printf("[HILO - Entrenador %d]: PID %d\n", localEnt->id,localEnt->pid);
	printf("[HILO - Entrenador %d]: COORDENADAS %s\n",localEnt->id, localEnt->coordenadas);
	printf("[HILO - Entrenador %d]: ATRAPADOS %s\n",localEnt->id, localEnt->pokemones_atrapados);
	printf("[HILO - Entrenador %d]: OBJETIVOS %s\n",localEnt->id, localEnt->pokemones_objetivo);

	sem_post(sem_crearEntrenador);
	printf("[Entrenador %d]: estoy esperando que el planificador me llame. \n",localEnt->id);

	while(1)
	{

		sem_wait(sem_avanzar[localEnt->id]);

		if(localEnt->entrenadorVoyAIntercambiar == NULL)
		{
			while(!estoyEnLaMismaPosicion(localEnt,localEnt->objetivoActual->cordX,localEnt->objetivoActual->cordY))
			{

				meAcercoDeAUno(localEnt,localEnt->objetivoActual->cordX,localEnt->objetivoActual->cordY);

				log_info(logger,"[HILO - Entrenador %d]: Avance hacia el Pokemon, mis nuevas coordenadas son:%s \n",localEnt->id,localEnt->coordenadas);

				if (!estoyEnLaMismaPosicion(localEnt,localEnt->objetivoActual->cordX,localEnt->objetivoActual->cordY)){
					sem_post(sem_ciclo);
					sem_wait(sem_avanzar[localEnt->id]);
				}


			}

			pasarBlocked(localEnt);
			sem_post(sem_ciclo);
			catchPokemon(localEnt);
		}

		else{
			int posX = atoi(string_split(localEnt->entrenadorVoyAIntercambiar->coordenadas, "|")[0]);
			int posY = atoi(string_split(localEnt->entrenadorVoyAIntercambiar->coordenadas, "|")[1]);

			while(!estoyEnLaMismaPosicion(localEnt,posX,posY))
			{
				meAcercoDeAUno(localEnt,posX,posY);

				log_info(logger,"[HILO - Entrenador %d]: Avance hacia el entrenador de intercambio, mis nuevas coordenadas son:%s \n",localEnt->id,localEnt->coordenadas);

				if (!estoyEnLaMismaPosicion(localEnt,posX,posY)){
					sem_post(sem_ciclo);
					sem_wait(sem_avanzar[localEnt->id]);
				}
			}
			pasarBlocked(localEnt);
			sem_post(sem_ciclo);
			intercambio(localEnt);
		}
	}
}

void intercambio(t_entrenador* entrenador){

	trueque(entrenador);

	entrenador->nombrePokemonAIntercambiar = NULL;
	entrenador->entrenadorVoyAIntercambiar->nombrePokemonAIntercambiar = NULL;

	//TESTING
	t_entrenador* entrenadorB = entrenador->entrenadorVoyAIntercambiar;
	entrenador->entrenadorVoyAIntercambiar = NULL;
	entrenadorB->entrenadorVoyAIntercambiar = NULL;

	postIntercambio(entrenador);

	//postIntercambio(entrenador->entrenadorVoyAIntercambiar);
	postIntercambio(entrenadorB);

}

void trueque(t_entrenador* entrenador){

	log_info(logger,"[DEADLOCK]: Entrenador %d y entrenador %d intercambian %s por %s \n",entrenador->id, entrenador->entrenadorVoyAIntercambiar->id,entrenador->nombrePokemonAIntercambiar,entrenador->entrenadorVoyAIntercambiar->nombrePokemonAIntercambiar);

	sacoPokemonAtrapado(entrenador, entrenador->entrenadorVoyAIntercambiar->nombrePokemonAIntercambiar);
	agregoPokemonAtrapado(entrenador,entrenador->nombrePokemonAIntercambiar);

	sacoPokemonAtrapado(entrenador->entrenadorVoyAIntercambiar, entrenador->nombrePokemonAIntercambiar);
	agregoPokemonAtrapado(entrenador->entrenadorVoyAIntercambiar,entrenador->entrenadorVoyAIntercambiar->nombrePokemonAIntercambiar);

}

void sacoPokemonAtrapado(t_entrenador* entrenador, char* pokemon){

	List listaMisPokemonesAtrapados = misPokemonesAtrapados(entrenador);
	IteratorList iterator = beginlist(listaMisPokemonesAtrapados);
	char* pokemonAtrapadoActual = iterator->data;
	char* pokemonesAtrapadosActualizados = NULL;
	int cont = 0;

	while(iterator != NULL){

		pokemonAtrapadoActual = iterator->data;

		if(strcmp(pokemonAtrapadoActual,pokemon) || cont == 1){
			if(pokemonesAtrapadosActualizados != NULL){

				string_append(&pokemonesAtrapadosActualizados,"|");
				string_append(&pokemonesAtrapadosActualizados,pokemonAtrapadoActual);

			} else {

				pokemonesAtrapadosActualizados = malloc(strlen(pokemonAtrapadoActual)+1);
				strcpy(pokemonesAtrapadosActualizados, pokemonAtrapadoActual);
			}
		}
		else{
			cont++;
		}

		iterator = nextlist(iterator);
	}

	entrenador->pokemones_atrapados = pokemonesAtrapadosActualizados;

}

void agregoPokemonAtrapado(t_entrenador* entrenador, char* pokemon){

	if(entrenador->pokemones_atrapados != NULL)
	{
		string_append(&(entrenador->pokemones_atrapados),"|");
		string_append(&(entrenador->pokemones_atrapados),pokemon);

	} else {
		entrenador->pokemones_atrapados = malloc(strlen(pokemon)+1);
		strcpy(entrenador->pokemones_atrapados, pokemon);
	}


}

void postIntercambio(t_entrenador* entrenador)
{
	if(cumpliMisObjetivos(entrenador) == 1)
	{
		pasarExit(entrenador);
		entrenador->objetivoActual = NULL;
	}
	else
	{
		entrenador->esperandoIntercambiar = 1;
		entrenador->objetivoActual = NULL;
		entrenador->entrenadorVoyAIntercambiar = NULL;
		buscarIntercambio(entrenador);
	}
	////////////
	for(int i=0 ; i < cantidadEntrenadores  ; i++){
		printf("[Planificador]: <<Entrenador %d>> Estado %d - Esperando intercambiar %d - Pokemones atrapados %s - Pokemones objetivo %s \n",entrenadores[i]->id, entrenadores[i]->estadoEntrenador, entrenadores[i]->esperandoIntercambiar,entrenadores[i]->pokemones_atrapados, entrenadores[i]->pokemones_objetivo);
	}
	////////////
}

int estoyEnLaMismaPosicion(t_entrenador* localEnt,int cordX, int cordY){

	if ((atoi(string_split(localEnt->coordenadas, "|")[0]) == cordX && atoi(string_split(localEnt->coordenadas, "|")[1]) == cordY)){
		return 1;
	}

	return 0;
}

void catchPokemon(t_entrenador* entrenador)
{
	t_pokemon* pokemon = entrenador->objetivoActual;
	int respuesta = enviarCatchPokemon(pokemon);
	if(respuesta == -1) //ESTO PASA SI EL BROKER NO ME RESPONDE
	{
		log_info(logger,"[HILO - Entrenador %d]: No pude comunicarme con el Broker, por lo cual procedo a atrapar a %s en %d|%d \n",entrenador->id,pokemon->nombre,pokemon->cordX,pokemon->cordY);
		atraparPokemon(entrenador, pokemon->nombre);
	}
	else
	{
		entrenador->idMensajeEsperado = respuesta;
		sem_wait(sem_caughtPokemonBroker[entrenador->id]);
		if(entrenador->objetivoActual != NULL)
		{
			atraparPokemon(entrenador, pokemon->nombre);
		}
	}
}

int enviarCatchPokemon(t_pokemon* pokemon)
{
	char* ipBroker = config_get_string_value(configuracion_get(), "IP_BROKER");
	char* puertoBroker = config_get_string_value(configuracion_get(), "PUERTO_BROKER");

	char* cordX = string_itoa(pokemon->cordX);
	char* cordY = string_itoa(pokemon->cordY);

	// CATCH_POKEMON BROKER -> PUBLISHER CATCH_POKEMON [NOMBRE_POKEMON] POSX POSY

	char* mensajes[5] = {"PUBLISHER", "CATCH_POKEMON", pokemon->nombre, cordX ,cordY};

	int socket = send_messages_and_return_socket(ipBroker, puertoBroker, mensajes, 5);

	if(socket == -1)
	{
		return -1;
	}

	char* respuesta = recibir_mensaje(socket);
	liberar_conexion(socket);

	return atoi(respuesta);
}

void atraparPokemon(t_entrenador* entrenador, char* pokemonNombre)
{
	agregoPokemonAtrapado(entrenador, pokemonNombre);

	printf("[HILO - Entrenador %d]: CATCH POKEMON!! \n",entrenador->id);
	printf("[HILO - Entrenador %d]: ATRAPADOS %s\n",entrenador->id, entrenador->pokemones_atrapados);

	log_info(logger,"[HILO - Entrenador %d]: Atrapé a %s en las coordenadas %s \n",entrenador->id,pokemonNombre,entrenador->coordenadas);

	char** array_objetivos =  string_split(entrenador->pokemones_objetivo, "|");
	char** array_atrapados = string_split(entrenador->pokemones_atrapados,"|");
	int cumpliMisObjetivosV = cumpliMisObjetivos(entrenador);

	if(cumpliMisObjetivosV == 1)
	{
		pasarExit(entrenador);
		entrenador->objetivoActual = NULL;
	}
	else
	{
		if(len_array(array_objetivos) ==  len_array(array_atrapados))
		{
			entrenador->esperandoIntercambiar = 1;
			entrenador->objetivoActual = NULL;
			buscarIntercambio(entrenador);
		}

		else {
			entrenador->objetivoActual = NULL;
			sem_post(sem_entrenadores_libres);
		}
	}

	////////////
	for(int i=0 ; i < cantidadEntrenadores  ; i++){
		printf("[Planificador]: <<Entrenador %d>> Estado %d - Esperando intercambiar %d - Pokemones atrapados %s - Pokemones objetivo %s \n",entrenadores[i]->id, entrenadores[i]->estadoEntrenador, entrenadores[i]->esperandoIntercambiar,entrenadores[i]->pokemones_atrapados, entrenadores[i]->pokemones_objetivo);
	}
	////////////

}

void buscarIntercambio(t_entrenador* entrenador)
{
	sem_wait(sem_buscandoIntercambio);
	printf("[Entrenador %d] Ejecutando algoritmo de busqueda de deadlock \n", entrenador->id);
	List resultados = candidatosADeadlock(entrenador);

	if (!hayDeadlock(resultados)){
		printf("[Entrenador %d] NO HAY DEADLOCK. No hay candidatos para intercambio. \n", entrenador->id);
		sem_post(sem_buscandoIntercambio);
		return;
	}
	t_entrenador* entrenadorParaIntercambiar = buscarEntrenadorParaIntercambiar(entrenador, resultados);

	if(entrenadorParaIntercambiar == NULL){
		printf("[Entrenador %d] NO HAY DEADLOCK. De los candidatos ninguno es fit. \n", entrenador->id);
		sem_post(sem_buscandoIntercambio);
		return;
	}

	printf("[Entrenador %d] HAY DEADLOCK \n", entrenador->id);

	entrenador->entrenadorVoyAIntercambiar = entrenadorParaIntercambiar;
	entrenadorParaIntercambiar->entrenadorVoyAIntercambiar = entrenador;
	printf("Se planifica intercambio entre Entrenador %d(%s) y %d(%s) \n", entrenador->id, entrenadorParaIntercambiar->nombrePokemonAIntercambiar, entrenadorParaIntercambiar->id, entrenador->nombrePokemonAIntercambiar);
	pasarReady(entrenador);
	sem_post(sem_buscandoIntercambio);
}

int cumpliMisObjetivos(t_entrenador* entrenador)
{
	char** array_objetivos =  string_split(entrenador->pokemones_objetivo, "|");
	char** array_atrapados = string_split(entrenador->pokemones_atrapados,"|");
	int resultado = 0;

	for(int b=0; b < len_array(array_objetivos); b++){
		char* pokemon_objetivo = array_objetivos[b];
		int fue_atrapado = 0;
		for(int a=0; a < len_array(array_atrapados); a++){
			char* pokemon_atrapado = array_atrapados[a];
			if(!strcmp(pokemon_atrapado,pokemon_objetivo))
			{
				fue_atrapado = 1; break;
			}
		}
		if(fue_atrapado==0)
		{
			resultado = 1;
		}
	}

	return !resultado;
}

void meAcercoDeAUno(t_entrenador* entrenador, int posX, int posY)
{
	sleep(config_get_int_value(configuracion_get(), "RETARDO_CICLO_CPU"));

	int coordenadaX = atoi(string_split(entrenador->coordenadas, "|")[0]);
	int coordenadaY = atoi(string_split(entrenador->coordenadas, "|")[1]);

	if(coordenadaX  > posX)
	{
		char* resul = string_itoa(coordenadaX - 1);
		string_append(&resul, "|");
		string_append(&resul, string_itoa(coordenadaY));
		entrenador->coordenadas = resul;
	}
	else if(coordenadaX < posX)
	{
		char* resul = string_itoa(coordenadaX + 1);
		string_append(&resul, "|");
		string_append(&resul, string_itoa(coordenadaY));
		entrenador->coordenadas = resul;
	}
	else if(coordenadaX  == posX && coordenadaY  > posY)
	{
		char* resul = string_itoa(coordenadaX);
		string_append(&resul, "|");
		string_append(&resul, string_itoa(coordenadaY - 1));
		entrenador->coordenadas = resul;
	}
	else if(coordenadaX  == posX && coordenadaY  < posY)
	{
		char* resul = string_itoa(coordenadaX);
		string_append(&resul, "|");
		string_append(&resul, string_itoa(coordenadaY + 1));
		entrenador->coordenadas = resul;
	}
}
