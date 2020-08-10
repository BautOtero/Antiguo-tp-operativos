#include "../include/procesos.h"


t_log* logger;


int enviarMensaje(char* nombreProceso, char *argv[]) {

	logger = log_create("gameboy.log", "GameBoy", true, LOG_LEVEL_INFO);

	char* ip;
	char* puerto;

	t_config* config = configuracion_get();

	if (!strcmp(nombreProceso,"BROKER")){
		if(checkParametroTipoMensaje(argv)){
			return -1;
		}
		ip = config_get_string_value(config, "IP_BROKER");
		puerto = config_get_string_value(config, "PUERTO_BROKER");
		printf("%s(%s:%s): %s \n", nombreProceso, ip, puerto, argv[2]);
		procesosBroker(argv, ip, puerto);

	} else if (!strcmp(nombreProceso, "SUSCRIPTOR")) {
		ip = config_get_string_value(config, "IP_BROKER");
		puerto = config_get_string_value(config, "PUERTO_BROKER");

		procesosSuscriptorSinReintento(argv, ip, puerto);
	}
	else if (!strcmp(nombreProceso, "TEAM")) {
		if(checkParametroTipoMensaje(argv)){
			return -1;
		}
		ip = config_get_string_value(config, "IP_TEAM");
		puerto = config_get_string_value(config, "PUERTO_TEAM");
		printf("%s(%s:%s): %s \n", nombreProceso, ip, puerto, argv[2]);
		procesosTeam(argv, ip, puerto);

	} else if (!strcmp(nombreProceso, "GAMECARD")) {
//		if(checkParametroTipoMensaje(argv)){
//			return -1;
//		}
		ip = config_get_string_value(config, "IP_GAMECARD");
		puerto = config_get_string_value(config, "PUERTO_GAMECARD");
		printf("%s(%s:%s): %s \n", nombreProceso, ip, puerto, argv[2]);
		procesosGameCard(argv, ip, puerto);

	}else {
		printf("No existe el proceso %s \n", nombreProceso);
		return -1;

	}

	log_destroy(logger);
	return 0;
}



int procesosBroker(char *argv[], char *ip, char *puerto) {

	char* tipo_mensaje2 = argv[2];

	if(!strcmp(tipo_mensaje2 , "NEW_POKEMON")){

		if (checkParametrosMensajeNewPokemon(argv, 0)){
			return -1;
		}
		log_info(logger, "Enviando el mensaje de tipo : %s",tipo_mensaje2 );
		char* mensajes[6];
		mensajes[0] = "PUBLISHER";
		mensajes[1] = tipo_mensaje2;
		mensajes[2] = argv[3]; //[POKEMON]
		mensajes[3] = argv[4]; //[POSX]
		mensajes[4] = argv[5]; //[POSY]
		mensajes[5] = argv[6]; //[CANTIDAD]

		int socketRespuesta = send_messages_and_return_socket(ip, puerto,mensajes,6);


		char* idMensaje = recibir_mensaje(socketRespuesta);

		log_info(logger, "El id asignado por el broker es : %s",idMensaje );


	}



	else if(!strcmp(tipo_mensaje2 , "APPEARED_POKEMON"))
	{
		if (checkParametrosMensajeAppearedPokemon(argv, 1)){
			return -1;
		}
		log_info(logger, "Enviando el mensaje de tipo : %s",tipo_mensaje2 );
		char* mensajes[6];
		mensajes[0] = "PUBLISHER";
		mensajes[1] = tipo_mensaje2;
		mensajes[2] = argv[3]; //[POKEMON]
		mensajes[3] = argv[4]; //[POSX]
		mensajes[4] = argv[5]; //[POSY]
		mensajes[5] = argv[6]; //[ID_MENSAJE_CORRELATIVO]
		int socketRespuesta = send_messages_and_return_socket(ip, puerto,mensajes,6);

		printf("%i \n",socketRespuesta);

		char* idMensaje = recibir_mensaje(socketRespuesta);

		log_info(logger, "El id asignado por el broker es : %s",idMensaje );

	}

	else if(!strcmp(tipo_mensaje2 , "CATCH_POKEMON"))
	{
		if (checkParametrosMensajeCatchPokemon(argv, 0)){
			return -1;
		}
		log_info(logger, "Enviando el mensaje de tipo : %s",tipo_mensaje2 );
		char* mensajes[5];
		mensajes[0] = "PUBLISHER";
		mensajes[1] = tipo_mensaje2;
		mensajes[2] = argv[3]; //[POKEMON]
		mensajes[3] = argv[4]; //[POSX]
		mensajes[4] = argv[5]; //[POSY]

		int socketRespuesta = send_messages_and_return_socket(ip, puerto,mensajes,5);

		printf("%i \n",socketRespuesta);

		char* idMensaje = recibir_mensaje(socketRespuesta);

		log_info(logger, "El id asignado por el broker es : %s",idMensaje );

	}

	else if(!strcmp(tipo_mensaje2 , "CAUGHT_POKEMON"))
	{
		if (checkParametrosMensajeCaughtPokemon(argv)){
			return -1;
		}
		log_info(logger, "Enviando el mensaje de tipo : %s",tipo_mensaje2 );
		char* mensajes[4];
		mensajes[0] = "PUBLISHER";
		mensajes[1] = tipo_mensaje2;
		mensajes[2] = argv[3]; //[ID_MENSAJE_CORRELATIVO]
		mensajes[3] = argv[4]; //[OK/FAIL]

		int socketRespuesta = send_messages_and_return_socket(ip, puerto,mensajes,4);

		char* idMensaje = recibir_mensaje(socketRespuesta);

		log_info(logger,"El id asignado por el broker es : %s",idMensaje );



	}

	else if(!strcmp(tipo_mensaje2 , "GET_POKEMON"))
	{
		if (checkParametrosMensajeGetPokemon(argv,0)){
			return -1;
		}
		log_info(logger, "Enviando el mensaje de tipo : %s",tipo_mensaje2 );
		char* mensajes[3];
		mensajes[0] = "PUBLISHER";
		mensajes[1] = tipo_mensaje2;
		mensajes[2] = argv[3]; //[POKEMON]

		int socketRespuesta = send_messages_and_return_socket(ip, puerto,mensajes,3);

		char* idMensaje = recibir_mensaje(socketRespuesta);

		log_info(logger, "El id asignado por el broker es : %s",idMensaje );



	}else if(!strcmp(tipo_mensaje2 , "LOCALIZED_POKEMON"))
	{

		log_info(logger, "Enviando el mensaje de tipo : %s",tipo_mensaje2 );
		t_mensajes* mensajes =  malloc(sizeof(t_mensajes));
		int cantidad_coordenadas = atoi(argv[4]);
		char* publisher = "PUBLISHER";

		mensajes->mensajes = malloc((4+(cantidad_coordenadas*2)) * sizeof(char*));

		mensajes->mensajes[0] = publisher;
		mensajes->mensajes[1] = tipo_mensaje2;
		mensajes->mensajes[2] = argv[3]; // NOMBRE POKEMON
		mensajes->mensajes[3] = argv[4]; // CANTIDAD COORDENADAS


		for(int i=0;i<cantidad_coordenadas*2;i++){
			mensajes->mensajes[i+4] = argv[i+5];
		}


		mensajes->size = malloc(sizeof(uint32_t));
		(*mensajes->size) = (4+(cantidad_coordenadas*2));

		int socket = send_messages_and_return_socket(ip, puerto,mensajes->mensajes,(int) 4 + cantidad_coordenadas*2);


		printf( "Socket respuesta %i \n",socket );


		printf( "Mensaje LOCALIZED_POKEMON en Broker enviado \n" );

	}


	return 1;
}

int procesosTeam(char *argv[],char *ip,char *puerto) {

	char* tipo_mensaje2 = argv[2];

	if(!strcmp(tipo_mensaje2 , "APPEARED_POKEMON")){
		if (checkParametrosMensajeAppearedPokemon(argv, 0)){
			return -1;
		}

		log_info(logger, "Enviando el mensaje de tipo : %s",tipo_mensaje2 );
		char* mensajes[6];
		mensajes[0] = "PUBLISHER";
		mensajes[1] = tipo_mensaje2;
		mensajes[2] = argv[3]; //[POKEMON]
		mensajes[3] = argv[4]; //[POSX]
		mensajes[4] = argv[5]; //[POSY]
		mensajes[5] = "0"; //argv[6]; [ID_MENSAJE_CORRELATIVO]

		send_messages_and_return_socket(ip, puerto, mensajes, 6);


		log_info(logger, " Mensaje APPEARED_POKEMON en Team enviado \n" );

	}

	return 1;
}

int procesosGameCard(char *argv[],char *ip,char *puerto) {

	char* tipo_mensaje2 = argv[2];

	if(!strcmp(tipo_mensaje2 , "NEW_POKEMON")){

//		if (checkParametrosMensajeNewPokemon(argv, 1)){
//			return -1;
//		}
		log_info(logger, "Enviando el mensaje de tipo : %s",tipo_mensaje2 );

		// {ID_MENSAJE} NEW_POKEMON {POKEMON} {POSX} {POSY} {CANTIDAD}

		char* mensajes[6] = {argv[7], tipo_mensaje2, argv[3], argv[4], argv[5], argv[6]};

		send_messages_and_return_socket(ip, puerto, mensajes, 6);
		log_info(logger, " Mensaje NEW_POKEMON en GameCard enviado \n");
	}



	else if(!strcmp(tipo_mensaje2 , "CATCH_POKEMON")){
		if (checkParametrosMensajeCatchPokemon(argv, 1)){
			return -1;
		}
		log_info(logger, "Enviando el mensaje de tipo : %s",tipo_mensaje2 );

		// {ID_MENSAJE} NEW_POKEMON {POKEMON} {POSX} {POSY}
		char* mensajes[5] = {argv[6], tipo_mensaje2, argv[3], argv[4], argv[5]};

		send_messages_and_return_socket(ip, puerto, mensajes, 5);
		log_info(logger, " Mensaje CATCH_POKEMON en GameCard enviado \n");
	}

	else if(!strcmp(tipo_mensaje2 , "GET_POKEMON")){
		if (checkParametrosMensajeGetPokemon(argv,1)){
			return -1;
		}
		log_info(logger, "Enviando el mensaje de tipo : %s",tipo_mensaje2 );

		// {ID_MENSAJE} GET_POKEMON {POKEMON}
		char* mensajes[3] = {argv[4], tipo_mensaje2, argv[3]};

		send_messages_and_return_socket(ip, puerto, mensajes, 5);
		log_info(logger, " Mensaje GET_POKEMON en GameCard enviado \n");
	}

	return 1;
}

int procesosSuscriptorSinReintento(char *argv[], char *ip, char *puerto) {


	int tiempo_suscripcion = atoi(argv[3]);


	suscribirseDesacoplado(argv[2]);

	sleep(tiempo_suscripcion);

	return 1;

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

	char* respuesta = recibir_mensaje(socket);

	printf("%s\n", respuesta);

	escuchar_socket_sin_conexion(&socket, handle_client_suscripcion);

}

void handle_client_suscripcion(t_result* mensaje){

	pthread_t thread_confirmacion;
	char* mensajesLog = string_new();
	log_info(logger, "Recibiendo los mensajes :" );
	for(int i = 0; i<*mensaje->mensajes->size;i++){
		string_append(&mensajesLog,mensaje->mensajes->mensajes[i]);
		string_append(&mensajesLog," ");
	}printf("\n");



	log_info(logger, " El mensaje recibido es: %s ",mensajesLog );


	pthread_create(&thread_confirmacion,NULL,(void*)procesosAck, mensaje);


	pthread_join(thread_confirmacion,NULL);


}
void procesosAck(t_result* mensaje){

	t_config* config = configuracion_get();
	char* id_suscriptor = config_get_string_value(config, "MI_ID");
	char* ip = config_get_string_value(config, "IP_BROKER");
	char* puerto = config_get_string_value(config, "PUERTO_BROKER");
	char* ack = "ACK";
	char* cola_de_mensajes = mensaje->mensajes->mensajes[1];
	char* id_mensaje = mensaje->mensajes->mensajes[0] ;


	char* mensajes[4];
	mensajes[0] = ack;
	mensajes[1] = cola_de_mensajes;
	mensajes[2] = id_mensaje;
	mensajes[3] = id_suscriptor;



	log_info(logger, "Enviando ack a la cola de mensajes %s :",cola_de_mensajes );
	send_messages_and_return_socket(ip, puerto, mensajes, 4);


	pthread_exit(NULL);
}




