/* main.c */

#include "../include/main.h"

int main(int argc, char *argv[]) {

t_config* config = configuracion_init("gamecard.config");

	if (config == NULL ){
		printf("No se pudo leer la configuracion \n");
		return -1;
	}

	initializeTallGrass();


	suscribirseDesacoplado("NEW_POKEMON");
	suscribirseDesacoplado("CATCH_POKEMON");
	suscribirseDesacoplado("GET_POKEMON");

	iniciar_servidor("127.0.0.1", "6014", handle_client);

	return 0;

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

	handle_client(msj);
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


