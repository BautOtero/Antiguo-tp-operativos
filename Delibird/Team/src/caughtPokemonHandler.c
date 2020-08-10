#include "../include/caughtPokemonHandler.h"


void caughtPokemon(int resultado, int idMensaje){

	for(int a=0; a < cantidadEntrenadores; a++){

		sem_wait(m_caughtPokemon);

		if(entrenadores[a]->estadoEntrenador == BLOCKED && entrenadores[a]->idMensajeEsperado == idMensaje)
		{			sem_post(sem_caughtPokemonBroker[entrenadores[a]->id]);

			/*if(resultado==1){
			sem_post(sem_caughtPokemonBroker[entrenadores[a]->id]);
			}
			else{
				sem_wait(m_listaObjetivos);

				char* r = string_new();
				string_append(&r,entrenadores[a]->objetivoActual->nombre);
				pushbacklist(&pokemonesObjetivosPendientes,r);
				sem_post(m_listaObjetivos);
				entrenadores[a]->objetivoActual = NULL;
				entrenadores[a]->esperandoIntercambiar = 0;

				List lista = misObjetivosSinAtrapar(entrenadores[a]);

				for(IteratorList i = beginlist(lista) ; i != NULL; i = nextlist(i))
				{
					char* nombreActual = i->data;
					enviarGetPokemon(nombreActual);
				}

				entrenadores[a]->objetivoActual = NULL;
				sem_post(sem_caughtPokemonBroker[entrenadores[a]->id]);
			}
			entrenadores[a]->idMensajeEsperado = -1;*/
		}

		sem_post(m_caughtPokemon);
	}
}

int enviarGetPokemon(char* pokemon)
{
	char* ipBroker = config_get_string_value(configuracion_get(), "IP_BROKER");
	char* puertoBroker = config_get_string_value(configuracion_get(), "PUERTO_BROKER");

	//PUBLISHER {{COLA}} [{{PARAMETROS}}]
	char* mensajes[3] = {"PUBLISHER", "GET_POKEMON", pokemon};

	int socket = send_messages_and_return_socket(ipBroker, puertoBroker, mensajes, 3);

	if(socket == -1)
	{
		return -1;
	}

	char* respuesta = recibir_mensaje(socket);
	liberar_conexion(socket);

	return atoi(respuesta);
}
