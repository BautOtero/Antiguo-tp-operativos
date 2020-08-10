#include "../include/planificacionEntrenadores.h"

int iniciarPlanificacionEntrenadores(){
	pthread_t thread;
	int result_code = pthread_create(&thread, NULL, (void*)&planificar, NULL);
	//pthread_detach(thread);
	return result_code;
}

void planificar(){

	char* algoritmo = config_get_string_value(configuracion_get(), "ALGORITMO_PLANIFICACION");
	sem_planificador = malloc(sizeof(sem_t));
	sem_entrenadorListo = malloc(sizeof(sem_t));

	sem_init(sem_planificador,0,1);
	sem_init(sem_entrenadorListo,0,0);

	printf("[Planificador]: Inicia el planificador con algoritmo seleccionado %s...\n", algoritmo);

	if(!strcmp(algoritmo,"FIFO")){
		planificarFIFO();
	} else if(!strcmp(algoritmo,"RR")){
		planificarRR();
	} else if(!strcmp(algoritmo,"SJF-SD")){
		planificarSJFSD();
	} else if(!strcmp(algoritmo,"SJF-CD")){
		planificarSJFCD();
	}
}


void planificarFIFO(){

	while(1){

		sem_wait(sem_entrenadorListo);
		sem_wait(sem_planificador);

		t_entrenador * entrenador_elegido_puntero = popfrontlist(&entrenadoresFifoReady);

		if(entrenador_elegido_puntero == NULL)				{
			continue;
		}

		log_info(logger,"[Planificador]: Le toca al entrenador %d\n", entrenador_elegido_puntero->id);

		////////////
		for(int i=0 ; i < cantidadEntrenadores  ; i++){
			printf("[Planificador]: <<Entrenador %d>> Estado %d - Esperando intercambiar %d \n",entrenadores[i]->id, entrenadores[i]->estadoEntrenador, entrenadores[i]->esperandoIntercambiar);
		}
		////////////

		entrenador_elegido_puntero->estadoEntrenador = EXEC;

		while (entrenador_elegido_puntero->estadoEntrenador == EXEC){
			sem_post(sem_avanzar[entrenador_elegido_puntero->id]);
			sem_wait(sem_ciclo);
		}
	}
}

void planificarRR(){

	while(1){

		sem_wait(sem_entrenadorListo);
		sem_wait(sem_planificador);

		t_entrenador * entrenador_elegido_puntero = popfrontlist(&entrenadoresFifoReady);

		if(entrenador_elegido_puntero == NULL)				{
			continue;
		}

		log_info(logger,"[Planificador]: Le toca al entrenador %d\n", entrenador_elegido_puntero->id);

		////////////
		for(int i=0 ; i < cantidadEntrenadores  ; i++){
			printf("[Planificador]: <<Entrenador %d>> Estado %d - Esperando intercambiar %d \n",entrenadores[i]->id, entrenadores[i]->estadoEntrenador, entrenadores[i]->esperandoIntercambiar);
		}
		////////////

		entrenador_elegido_puntero->estadoEntrenador = EXEC;

		int quantum = config_get_int_value(configuracion_get(), "QUANTUM");

		if (quantum == 0){
			printf("NO se puede ejecutar un RR con quantum 0");
			return;
		}

		for (int i = 0; i < quantum; i++){
			if (entrenador_elegido_puntero->estadoEntrenador == EXEC){
				sem_post(sem_avanzar[entrenador_elegido_puntero->id]);
				sem_wait(sem_ciclo);
			}
		}
		if (entrenador_elegido_puntero->estadoEntrenador == EXEC){
			pasarReady(entrenador_elegido_puntero);
			sem_post(sem_planificador);
		}
	}
}

void planificarSJFSD(){

	while(1){

		sem_wait(sem_entrenadorListo);
		sem_wait(sem_planificador);

		t_entrenador * entrenador_elegido_puntero = elegirEntrenadorPorSJF();

		if(entrenador_elegido_puntero == NULL)				{
			continue;
		}

		log_info(logger,"[Planificador]: Le toca al entrenador %d\n", entrenador_elegido_puntero->id);

		////////////
		for(int i=0 ; i < cantidadEntrenadores  ; i++){
			printf("[Planificador]: <<Entrenador %d>> Estado %d - Esperando intercambiar %d \n",entrenadores[i]->id, entrenadores[i]->estadoEntrenador, entrenadores[i]->esperandoIntercambiar);
		}
		////////////

		entrenador_elegido_puntero->estadoEntrenador = EXEC;

		while (entrenador_elegido_puntero->estadoEntrenador == EXEC){
			sem_post(sem_avanzar[entrenador_elegido_puntero->id]);
			sem_wait(sem_ciclo);
			entrenador_elegido_puntero->valorUltimaRafaga++;
		}
	}
}

void planificarSJFCD(){

	while(1){

		sem_wait(sem_entrenadorListo);
		sem_wait(sem_planificador);

		t_entrenador * entrenador_elegido_puntero = elegirEntrenadorPorSJF();

		if(entrenador_elegido_puntero == NULL){
			continue;
		}

		log_info(logger,"[Planificador]: Le toca al entrenador %d\n", entrenador_elegido_puntero->id);

		////////////
		for(int i=0 ; i < cantidadEntrenadores  ; i++){
			printf("[Planificador]: <<Entrenador %d>> Estado %d - Esperando intercambiar %d \n",entrenadores[i]->id, entrenadores[i]->estadoEntrenador, entrenadores[i]->esperandoIntercambiar);
		}
		////////////

		entrenador_elegido_puntero->estadoEntrenador = EXEC;

		while (entrenador_elegido_puntero->estadoEntrenador == EXEC){

			sem_post(sem_avanzar[entrenador_elegido_puntero->id]);
			sem_wait(sem_ciclo);

			entrenador_elegido_puntero->valorUltimaRafaga++;
			if(entrenador_elegido_puntero->estadoEntrenador == EXEC){
				entrenador_elegido_puntero = desalojo(entrenador_elegido_puntero);
			}
		}
	}
}

t_entrenador* desalojo(t_entrenador* entrenadorActual){

	double valorEstimadoMenor = (entrenadorActual->valorPredichoUltimaRafaga - entrenadorActual->valorUltimaRafaga) ;

	double alfa = config_get_double_value(configuracion_get(), "ALPHA");

	IteratorList* menor = NULL;

	for (IteratorList iterator = beginlist(entrenadoresFifoReady); iterator != NULL; iterator = nextlist(iterator)){
		t_entrenador* entrenadorAnalizado = iterator->data;
		if (entrenadorAnalizado->valorEstimadoSiguiente == 0){
			entrenadorAnalizado->valorEstimadoSiguiente = calcularRafagaSiguienteEstimada(alfa, entrenadorAnalizado->valorPredichoUltimaRafaga, entrenadorAnalizado->valorUltimaRafaga);
		}
		if(valorEstimadoMenor > entrenadorAnalizado->valorEstimadoSiguiente){
			if(menor == NULL)
			{
				menor = malloc(sizeof(IteratorList));
			}
			*menor = iterator;
		}
	}

	if(menor != NULL){

		entrenadorActual->valorEstimadoSiguiente = valorEstimadoMenor;
		pasarReady(entrenadorActual);

		int idAnterior = entrenadorActual->id;

		sem_wait(sem_entrenadorListo);

		entrenadorActual = popiterlist(&entrenadoresFifoReady, *menor);

		printf("[Planificador]: Desalojo al entrenador %d por el entrenador %d \n", idAnterior,entrenadorActual->id);

		entrenadorActual->valorPredichoUltimaRafaga = entrenadorActual->valorEstimadoSiguiente;
		entrenadorActual->valorEstimadoSiguiente = 0;
		entrenadorActual->valorUltimaRafaga = 0;
		entrenadorActual->estadoEntrenador = EXEC;
	}
	return entrenadorActual;
}

t_entrenador* elegirEntrenadorPorSJF(){

	double alfa = config_get_double_value(configuracion_get(), "ALPHA");

	sem_wait(m_desalojo);

	IteratorList menor;
	double valorEstimadoMenor = 99999999;

	for (IteratorList iterator = beginlist(entrenadoresFifoReady); iterator != NULL; iterator = nextlist(iterator)){
		t_entrenador* entrenadorActual = iterator->data;
		if (entrenadorActual->valorEstimadoSiguiente == 0){
			entrenadorActual->valorEstimadoSiguiente = calcularRafagaSiguienteEstimada(alfa, entrenadorActual->valorPredichoUltimaRafaga, entrenadorActual->valorUltimaRafaga);
		}
		if(valorEstimadoMenor > entrenadorActual->valorEstimadoSiguiente){
			menor = iterator;
		}
	}

	t_entrenador* entrenadorElegido = popiterlist(&entrenadoresFifoReady, menor);

	entrenadorElegido->valorPredichoUltimaRafaga = entrenadorElegido->valorEstimadoSiguiente;
	entrenadorElegido->valorEstimadoSiguiente = 0;
	entrenadorElegido->valorUltimaRafaga = 0;

	sem_post(m_desalojo);

	return entrenadorElegido;
}

double calcularRafagaSiguienteEstimada(double alfa, double rafagaEstimadaAnterior, double rafagaRealAnterior){
	double resultado = ((alfa * rafagaRealAnterior) + ((1 - alfa) * rafagaEstimadaAnterior));

	return resultado;
}

void pasarReady(t_entrenador* entrenador)
{
	sem_wait(m_desalojo);

	entrenador->estadoEntrenador = READY;
	pushbacklist(&entrenadoresFifoReady,entrenador);
	sem_post(sem_entrenadorListo);

	sem_post(m_desalojo);
}

void pasarBlocked(t_entrenador* entrenador)
{
	entrenador->estadoEntrenador = BLOCKED;
	sem_post(sem_planificador);
}

void pasarExit(t_entrenador* entrenador)
{
	entrenador->estadoEntrenador = EXIT;
	finDeProceso();
}

void finDeProceso()
{
	int resultado = 1;

	sem_wait(m_finProceso);

	for(int i=0 ; i < cantidadEntrenadores  ; i++){
		if(entrenadores[i]->estadoEntrenador != EXIT){
			resultado = 0;
		}
	}

	if(resultado==1){
		log_info(logger,"[PROCESO TEAM]: Todos los entrenadores están en estado EXIT. << FIN DEL PROCESO >> \n");
	}

	sem_post(m_finProceso);
}

int estaEntreAtrapadosEntrenador(char* pokemon, t_entrenador* entrenador)
{

	char** array_atrapados =  string_split(entrenador->pokemones_atrapados, "|");
	for(int b=0; b < len_array(array_atrapados);b++){
		if(!strcmp(array_atrapados[b],pokemon))
		{
			return 1;
		}
	}

	return 0;
}

int len_array(char** arrayInput)
{
	int i = 0;	int cont = 0;

	for(i=0 ; arrayInput[i] != NULL ; i++)
	{
		cont++;
	}
	return cont;
}
