/* main.c */

#include "../include/main.h"


int main(int argc, char *argv[]) {

	initlist(&listaentrenadores);

	sem_buscandoIntercambio = malloc(sizeof(sem_t));
	sem_init(sem_buscandoIntercambio, 0, 1);

	sem_entrenador_ciclo_ready = malloc(sizeof(sem_t));
	sem_init(sem_entrenador_ciclo_ready, 0, 0);

	initlist(&entrenadoresFifoReady);

	sem_entrenadores_libres = malloc(sizeof(sem_t));
	sem_init(sem_entrenadores_libres, 0, 0);

	m_pasarEstado = malloc(sizeof(sem_t));
	sem_init(m_pasarEstado, 0, 1);

	m_caughtPokemon = malloc(sizeof(sem_t));
	sem_init(m_caughtPokemon, 0, 1);

	m_listaObjetivos = malloc(sizeof(sem_t));
	sem_init(m_listaObjetivos, 0, 1);



	m_finProceso = malloc(sizeof(sem_t));
	sem_init(m_finProceso, 0, 1);

	m_desalojo = malloc(sizeof(sem_t));
	sem_init(m_desalojo, 0, 1);

	pokemonesAAtraparContador = malloc(sizeof(sem_t));
	sem_init(pokemonesAAtraparContador, 0, 0);

	sem_ciclo = malloc(sizeof(sem_t));
	sem_init(sem_ciclo, 0, 0);

	initlist(&pokemonesAAtrapar);
	initlist(&pokemonesObjetivosPendientes);
	initlist(&pokemonesRecibidos);

	t_config* config = configuracion_init("team.config");

	if (config == NULL){
		return -1;
	}

	logger = log_create("team.log", "Team", true, LOG_LEVEL_INFO);

	int i ;

	char** posicionesEntrenadores = config_get_array_value(config, "POSICIONES_ENTRENADORES");
	char** pokemonEntrenadores = config_get_array_value(config, "POKEMON_ENTRENADORES");
	char** objetivosEntrenadores = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");
	double estimacionInicial = config_get_double_value(config, "ESTIMACION_INICIAL");

	cantidadEntrenadores = len_array(posicionesEntrenadores);

	sem_avanzar  = (sem_t**)calloc(cantidadEntrenadores, sizeof(sem_t*));
	for(i=0 ; i < cantidadEntrenadores  ; i++)
	{
		sem_avanzar[i] = malloc(sizeof(sem_t));
		sem_init(sem_avanzar[i], 0, 0);
	}

	sem_caughtPokemonBroker  = (sem_t**)calloc(cantidadEntrenadores, sizeof(sem_t*));
	for(i=0 ; i < cantidadEntrenadores  ; i++)
	{
		sem_caughtPokemonBroker[i] = malloc(sizeof(sem_t));
		sem_init(sem_caughtPokemonBroker[i], 0, 0);
	}

	sem_crearEntrenador = malloc(sizeof(sem_t));
	sem_init(sem_crearEntrenador, 0, 1);

	printf("Hay %d entrenadores \n", cantidadEntrenadores);

	entrenadores = malloc(cantidadEntrenadores * sizeof(t_entrenador*));

	for(i=0 ; posicionesEntrenadores[i] != NULL ; i++)
	{
		//Espero a que el hilo correspondiente a este nuevo entrenador haya sido creado (este semaforo está inicializado en 1)
		sem_wait(sem_crearEntrenador);

		pthread_t* hiloEntrenador = malloc(sizeof(pthread_t));

		t_entrenador* nuevoEntrenador = malloc(sizeof(t_entrenador));
		nuevoEntrenador->id = i;
		nuevoEntrenador->coordenadas = posicionesEntrenadores[i];
	    nuevoEntrenador->pokemones_objetivo = objetivosEntrenadores[i];

	    char** array_objetivos =  string_split(nuevoEntrenador->pokemones_objetivo, "|");

		for(int b=0; b < len_array(array_objetivos); b++){
			char* a = string_new();
			string_append(&a,array_objetivos[b]);
			sem_wait(m_listaObjetivos);
			pushbacklist(&pokemonesObjetivosPendientes,a);
			sem_post(m_listaObjetivos);
		}

		nuevoEntrenador->pokemones_atrapados = NULL;
		nuevoEntrenador->estadoEntrenador = NEW;
	    nuevoEntrenador->esperandoIntercambiar = 0;
	    nuevoEntrenador->entrenadorVoyAIntercambiar = NULL;
	    nuevoEntrenador->valorPredichoUltimaRafaga = 0;
	    nuevoEntrenador->valorUltimaRafaga = 0;
	    nuevoEntrenador->valorEstimadoSiguiente = estimacionInicial;

	    entrenadores[i] = nuevoEntrenador;

		sem_post(sem_entrenadores_libres);

	    printf("[ID %d]: %d\n", i, entrenadores[i]->id);
	    printf("[Posicion %d]: %s\n", i, entrenadores[i]->coordenadas);
	  	printf("[Objetivos %d]: %s\n", i, entrenadores[i]->pokemones_objetivo);
	    printf("[Pokemones %d]: %s\n", i, entrenadores[i]->pokemones_atrapados);

	    pthread_create(hiloEntrenador, NULL, (void*)&entrenadorMain, entrenadores[i]);
		nuevoEntrenador->pid = *hiloEntrenador;
		printf("[PID %d]: %d\n", i, entrenadores[i]->pid);

	    //free(nuevoEntrenador);
	}

	int x = 0;

	for(i=0 ; pokemonEntrenadores[i] != NULL ; i++)
	{
		entrenadores[x]->pokemones_atrapados = pokemonEntrenadores[i];

	    char** array_atrapados =  string_split(entrenadores[x]->pokemones_atrapados, "|");
		for(int b=0; b < len_array(array_atrapados); b++){

			sem_wait(m_listaObjetivos);
			sacarDeListaPorNombre(&pokemonesObjetivosPendientes, array_atrapados[b]);
			sem_post(m_listaObjetivos);
		}


		x++;

		if(x > cantidadEntrenadores)
		{
			x -= cantidadEntrenadores;
		}
	}


	for(i=0 ; i < cantidadEntrenadores; i++)
	{
		printf("[Pokemones atrapados]: %s\n", entrenadores[i]->pokemones_atrapados);
	}



	pthread_t thread_pasaAReady;
	pthread_create(&thread_pasaAReady, NULL, (void*)&buscarEntrenadorYPasarAReady, NULL);

	//Inicializo la planificación de entrenadores (se va a quedar esperando el sem hasta que haya un nuevo pokemon APPEARED_POKEMON)
	iniciarPlanificacionEntrenadores();

	//Me suscribo a las colas APPEARED_POKEMON, LOCALIZED_POKEMON y CAUGHT_POKEMON
	suscribirseDesacoplado("APPEARED_POKEMON");
	suscribirseDesacoplado("LOCALIZED_POKEMON");
	suscribirseDesacoplado("CAUGHT_POKEMON");


	envioMensajesGet();
	//Me quedo escuchando para el Gameboy
	iniciar_servidor("127.0.0.1","14316", handle_message);

	log_destroy(logger);

	//sem_destroy(sem_pokemonesLibres);

	return 0;

}

void sacarDeListaPorNombre(List* lista, char* nombre)
{
	for(IteratorList i = beginlist(*lista) ; i != NULL; i = nextlist(i))
	{
		char* nombreActual = i->data;
		if(!strcmp(nombreActual,nombre))
		{
			popiterlist(lista,i);
			return;
		}
	}
}


void suscribirseDesacoplado(char* cola){
	pthread_t hiloSuscriptor;
	pthread_create(&hiloSuscriptor, NULL, (void*)suscribirse, cola);
	pthread_detach(hiloSuscriptor);
}


void suscribirse(char* cola){

	t_config* config = configuracion_get();
	char* id_suscriptor = config_get_string_value(config, "MI_ID");
	char* ip = config_get_string_value(config, "IP_BROKER");
	char* puerto = config_get_string_value(config, "PUERTO_BROKER");

	char * mensajes[3] = {"SUSCRIPTOR", cola, id_suscriptor};

	int socket = send_messages_and_return_socket(ip, puerto, mensajes, 3);

	printf("%d\n", socket);

	if (socket != -1) {

		char* respuesta = recibir_mensaje(socket);

		printf("%s\n", respuesta);

		escuchar_socket_sin_conexion(&socket, ackHandleclient);

	} else {

		sleep(config_get_int_value(configuracion_get(), "TIEMPO_RECONEXION"));
		suscribirse(cola);
	}



}

void envioMensajesGet(){

	for(int i=0 ; i < cantidadEntrenadores  ; i++){

		List misObjetivosSinAtraparL = misObjetivosSinAtrapar(entrenadores[i]);
		IteratorList iterator = beginlist(misObjetivosSinAtraparL);
		char* pokemonGetActual = iterator->data;

		while(iterator != NULL){

			pokemonGetActual = iterator->data;
			int respuesta = enviarGetPokemon(pokemonGetActual);
			if(respuesta == -1) //ESTO PASA SI EL BROKER NO ME RESPONDE
			{
				log_info(logger,"[PROCESO TEAM]: No pude comunicarme con el Broker para enviar GET_POKEMON de %s \n",pokemonGetActual);
			}
			else
			{
				log_info(logger,"[PROCESO TEAM]: GET_POKEMON de %s enviado. Respuesta Broker: %d \n",pokemonGetActual,respuesta);
			}

			iterator = nextlist(iterator);
		}

	}

}



void handle_message(t_result* resultado)
{
	char* error;

	if (!strcmp(resultado->mensajes->mensajes[1], "APPEARED_POKEMON")) {

		char* nombrePokemon = resultado->mensajes->mensajes[2];
		int posx = strtol(resultado->mensajes->mensajes[3], &error, 10);
		int posy = strtol(resultado->mensajes->mensajes[4], &error, 10);

		if (strcmp(error, "\0") != 0){
			printf("ERROR POSX \n");
			return;
		}

		if (strcmp(error, "\0") != 0){
			printf("ERROR POSY \n");
			return;
		}

		log_info(logger,"[PROCESO TEAM]: Me llegó el mensaje APPEARED_POKEMON. Pokemon %s en %d|%d \n",nombrePokemon, posx, posy);

		appearedPokemon(nombrePokemon, posx, posy);
	}

	if (!strcmp(resultado->mensajes->mensajes[1], "LOCALIZED_POKEMON")) {


		char* nombrePokemon = resultado->mensajes->mensajes[2];
		int cantidadPares = strtol(resultado->mensajes->mensajes[3], &error, 10);
		log_info(logger,"[PROCESO TEAM]: Me llegó el mensaje LOCALIZED_POKEMON. Pokemon %s cant cord %d \n",nombrePokemon, cantidadPares);

		List listaDeCoordenadas;

		initlist(&listaDeCoordenadas);

		for (int i = 0; i < cantidadPares; i++){
			Coordenadas* coordenadas = malloc(sizeof(Coordenadas));
			coordenadas->posX = strtol(resultado->mensajes->mensajes[4 + 2 * i], &error, 10);
			coordenadas->posY = strtol(resultado->mensajes->mensajes[5 + 2 * i], &error, 10);

			pushbacklist(&listaDeCoordenadas, coordenadas);
			log_info(logger,"[PROCESO TEAM]: Me llegó el mensaje LOCALIZED_POKEMON. Pokemon %s en %d|%d \n",nombrePokemon, coordenadas->posX, coordenadas->posY);

		}

		localizedPokemon(nombrePokemon, listaDeCoordenadas);
	}

	if (!strcmp(resultado->mensajes->mensajes[1], "CAUGHT_POKEMON")) {
		int idMensaje = atoi(resultado->mensajes->mensajes[3]);
		int resultadoCaught = atoi(resultado->mensajes->mensajes[2]);

		log_info(logger,"[PROCESO TEAM]: Me llegó el mensaje CAUGHT_POKEMON. Mensaje %d resultado %d \n",idMensaje, resultadoCaught);

		caughtPokemon(resultadoCaught, idMensaje);
	}

}

void ackHandleclient(t_result* msj)
{
	char *ip = config_get_string_value(configuracion_get(), "IP_BROKER");
	char *puerto = config_get_string_value(configuracion_get(), "PUERTO_BROKER");

	char* cola = string_new();
	string_append(&cola, msj->mensajes->mensajes[1]);

	char* id_mensaje = string_new();
	string_append(&id_mensaje,msj->mensajes->mensajes[0]);

	char* id_subscriptor = config_get_string_value(configuracion_get(), "MI_ID");

	char* ack[4] = {"ACK", cola, id_mensaje, id_subscriptor};

	int socket = send_messages_and_return_socket(ip, puerto, ack, 4);

	if(socket != -1)
	{
		liberar_conexion(socket);
	}
	handle_message(msj);
}

