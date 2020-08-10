#include "../include/recibeMensaje.h"

void handle_client(t_result* resultado)
{

	for (int i = 0; i < *resultado->mensajes->size; i++){
		printf("%s ", resultado->mensajes->mensajes[i]);
	}
	printf("\n");

	if (strcmp(resultado->mensajes->mensajes[1], "NEW_POKEMON") == 0){
    	handle_client_new_pokemon(resultado);
    }

    if (strcmp(resultado->mensajes->mensajes[1], "CATCH_POKEMON") == 0){
		handle_client_catch_pokemon(resultado);
	}

    if (strcmp(resultado->mensajes->mensajes[1], "GET_POKEMON") == 0){
		handle_client_get_pokemon(resultado);
	}

}

void handle_client_new_pokemon(t_result* resultado)
{
	printf("ESTOY EN NEW_POKEMON\n");

	// {ID_MENSAJE} NEW_POKEMON {POKEMON} {POSX} {POSY} {CANTIDAD}
	// {mi_ID} ENVIAR_MENSAJE NEW_POKEMON {POKEMON} {POSX} {POSY} {CANTIDAD} {ID_MENSAJE}

	Coordenadas coordenadas = {resultado->mensajes->mensajes[3], resultado->mensajes->mensajes[4]};

	new_pokemon(resultado->mensajes->mensajes[0], resultado->mensajes->mensajes[2], coordenadas, resultado->mensajes->mensajes[5]);


}

void handle_client_catch_pokemon(t_result* resultado)
{

	// {ID_MENSAJE} CATCH_POKEMON {POKEMON} {POSX} {POSY}
	// {mi_ID} ENVIAR_MENSAJE CATCH_POKEMON {POKEMON} {POSX} {POSY} {ID_MENSAJE}

	Coordenadas coordenadas = {resultado->mensajes->mensajes[3], resultado->mensajes->mensajes[4]};

	catch_pokemon(resultado->mensajes->mensajes[0], resultado->mensajes->mensajes[2], coordenadas);


}

void handle_client_get_pokemon(t_result* resultado)
{
	// {ID_MENSAJE} GET_POKEMON {POKEMON}

	get_pokemon(resultado->mensajes->mensajes[0], resultado->mensajes->mensajes[2]);

}



