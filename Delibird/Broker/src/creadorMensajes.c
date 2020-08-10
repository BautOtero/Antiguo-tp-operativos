#include "../include/creadorMensajes.h"


MensajeGeneral* creacionMensajePosta(t_result* mensaje){

	MensajeGeneral *mensajeGeneral = malloc(sizeof(MensajeGeneral));

		int tamanio = 0;

		char * nombrePokemon = NULL;
		uint32_t mensajesNewPokemon[4];
		uint32_t mensajesCatchPokemon[3];
		uint32_t mensajesAppearedPokemon[3];
		uint32_t mensajesCaughtPokemon[1];
		uint32_t mensajesGetPokemon[1];

		if(strcmp(mensaje->mensajes->mensajes[1],"NEW_POKEMON") == 0){

			tamanio = calcularTamanioNewPokemon(mensaje);


			nombrePokemon = mensaje->mensajes->mensajes[2];
			mensajesNewPokemon[0] = (uint32_t) strlen(mensaje->mensajes->mensajes[2]);
			mensajesNewPokemon[1] = (uint32_t) strtoul(mensaje->mensajes->mensajes[3],NULL,0);
			mensajesNewPokemon[2] = (uint32_t) strtoul(mensaje->mensajes->mensajes[4],NULL,0);
			mensajesNewPokemon[3] = (uint32_t) strtoul(mensaje->mensajes->mensajes[5],NULL,0);


			void* mensajeNew = crearMensajeNewPokemon(tamanio,nombrePokemon,mensajesNewPokemon);


			mensajeGeneral->data=mensajeNew;
			mensajeGeneral->tam=tamanio;
			//mensajeGeneral->lru = lruInit;

			return mensajeGeneral;

		}
			 else if(strcmp(mensaje->mensajes->mensajes[1],"CATCH_POKEMON") == 0){

			tamanio = calcularTamanioCatchPokemon(mensaje);

			nombrePokemon = mensaje->mensajes->mensajes[2];

			mensajesCatchPokemon[0] = (uint32_t) strlen(mensaje->mensajes->mensajes[2]);
			mensajesCatchPokemon[1] = (uint32_t) strtoul(mensaje->mensajes->mensajes[3],NULL,0);
			mensajesCatchPokemon[2] = (uint32_t) strtoul(mensaje->mensajes->mensajes[4],NULL,0);

			void* mensajeCatch =crearMensajeCatchPokemon(tamanio,nombrePokemon,mensajesCatchPokemon);

			mensajeGeneral->data=mensajeCatch;
			mensajeGeneral->tam=tamanio;
			//mensajeGeneral->lru = lruInit;

			return mensajeGeneral;

		} else if(strcmp(mensaje->mensajes->mensajes[1],"APPEARED_POKEMON") == 0){

			tamanio = calcularTamanioAppearedPokemon(mensaje);

			nombrePokemon = mensaje->mensajes->mensajes[2];
			mensajesAppearedPokemon[0] = (uint32_t) strlen(mensaje->mensajes->mensajes[2]);
			mensajesAppearedPokemon[1] = (uint32_t) strtoul(mensaje->mensajes->mensajes[3],NULL,0);
			mensajesAppearedPokemon[2] = (uint32_t) strtoul(mensaje->mensajes->mensajes[4],NULL,0);

			void* mensajeAppeared =crearMensajeAppearedPokemon(tamanio,nombrePokemon,mensajesAppearedPokemon);

			mensajeGeneral->data=mensajeAppeared;
			mensajeGeneral->tam=tamanio;
			//mensajeGeneral->lru = lruInit;

			return mensajeGeneral;

		} else if(strcmp(mensaje->mensajes->mensajes[1],"CAUGHT_POKEMON") == 0){

			tamanio = 4;

			mensajesCaughtPokemon[0] = (uint32_t) strtoul(mensaje->mensajes->mensajes[2],NULL,0);

			void* mensajeCaught = crearMensajeCaughtPokemon(tamanio,mensajesCaughtPokemon);

			mensajeGeneral->data=mensajeCaught;
			mensajeGeneral->tam=tamanio;
			//mensajeGeneral->lru = lruInit;

			return mensajeGeneral;

		}else if(strcmp(mensaje->mensajes->mensajes[1],"GET_POKEMON") == 0){

			tamanio = calcularTamanioGetPokemon(mensaje);

			nombrePokemon = string_new();
			string_append(&nombrePokemon, mensaje->mensajes->mensajes[2]);

			int tam = (uint32_t) strlen(nombrePokemon);

			mensajesGetPokemon[0] = tam;

			void* mensajeGet = crearMensajeGetPokemon(tamanio,nombrePokemon,mensajesGetPokemon);

			mensajeGeneral->data=mensajeGet;
			mensajeGeneral->tam=tamanio;
			//mensajeGeneral->lru = lruInit;

			return mensajeGeneral;

		}
		else if(strcmp(mensaje->mensajes->mensajes[1],"LOCALIZED_POKEMON") == 0){

			int cantidad_coordenadas = atoi(mensaje->mensajes->mensajes[3]);

			tamanio = calcularTamanioLocalizedPokemon(mensaje);

			nombrePokemon = mensaje->mensajes->mensajes[2];

			uint32_t mensajesLocalizedPokemon[2+cantidad_coordenadas*2];

			nombrePokemon = mensaje->mensajes->mensajes[2];

			mensajesLocalizedPokemon[0] = (uint32_t) strlen(mensaje->mensajes->mensajes[2]);
			mensajesLocalizedPokemon[1] = (uint32_t) strtoul(mensaje->mensajes->mensajes[3],NULL,0);

			for(int i=0;i<cantidad_coordenadas*2;i++){
				mensajesLocalizedPokemon[i+2] = (uint32_t) strtoul(mensaje->mensajes->mensajes[i+4],NULL,0);
			}



			void* mensajeLocalized = crearMensajeLocalizedPokemon(nombrePokemon,cantidad_coordenadas,tamanio,mensajesLocalizedPokemon);

			mensajeGeneral->data=mensajeLocalized;
			mensajeGeneral->tam=tamanio;
			//mensajeGeneral->lru = lruInit;

			return mensajeGeneral;

		}

		return NULL;

}






void* crearMensajeNewPokemon(int tam,char* nombrePokemon, uint32_t mensajes[4]) {
	void* nuevo = (MensajeNewPokemon*) malloc(tam);
	void* recorro = nuevo;

	memcpy(recorro, &mensajes[0], sizeof(u_int32_t));
	recorro += sizeof(u_int32_t);

	memcpy(recorro, nombrePokemon, strlen(nombrePokemon));
	recorro += strlen(nombrePokemon);

	memcpy(recorro, &mensajes[1], sizeof(u_int32_t));
	recorro += sizeof(u_int32_t);

	memcpy(recorro, &mensajes[2], sizeof(u_int32_t));
	recorro += sizeof(u_int32_t);

	memcpy(recorro, &mensajes[3], sizeof(u_int32_t));
	recorro += sizeof(u_int32_t);


	return nuevo;
}

void* crearMensajeCatchPokemon(int tam,char* nombrePokemon, uint32_t mensajes[3]) {
	void* nuevo = (MensajeCatchPokemon*) malloc(tam);
	void* recorro = nuevo;

	memcpy(recorro, &mensajes[0], sizeof(u_int32_t));
	recorro += sizeof(u_int32_t);

	memcpy(recorro, nombrePokemon, strlen(nombrePokemon));
	recorro += strlen(nombrePokemon);

	memcpy(recorro, &mensajes[1], sizeof(u_int32_t));
	recorro += sizeof(u_int32_t);

	memcpy(recorro, &mensajes[2], sizeof(u_int32_t));
	recorro += sizeof(u_int32_t);



	return nuevo;
}

void* crearMensajeGetPokemon(int tam,char* nombrePokemon, uint32_t mensajes[1]) {
	void* nuevo = (MensajeGetPokemon*) malloc(tam);
	void* recorro = nuevo;

	memcpy(recorro, &mensajes[0], sizeof(u_int32_t));
	recorro += sizeof(u_int32_t);

	memcpy(recorro, nombrePokemon, strlen(nombrePokemon));
	recorro += strlen(nombrePokemon);

	return nuevo;
}

void* crearMensajeCaughtPokemon(int tam, uint32_t mensajes[1]) {
	void* nuevo = (MensajeCaughtPokemon*) malloc(tam);
	void* recorro = nuevo;

	memcpy(recorro, &mensajes[0], sizeof(u_int32_t));
	recorro += sizeof(u_int32_t);



	return nuevo;
}


void* crearMensajeAppearedPokemon(int tam,char* nombrePokemon, uint32_t mensajes[3]) {
	void* nuevo = (MensajeAppearedPokemon*) malloc(tam);
	void* recorro = nuevo;

	memcpy(recorro, &mensajes[0], sizeof(u_int32_t));
	recorro += sizeof(u_int32_t);

	memcpy(recorro, nombrePokemon, strlen(nombrePokemon));
	recorro += strlen(nombrePokemon);

	memcpy(recorro, &mensajes[1], sizeof(u_int32_t));
	recorro += sizeof(u_int32_t);

	memcpy(recorro, &mensajes[2], sizeof(u_int32_t));
	recorro += sizeof(u_int32_t);


	return nuevo;
}


void* crearMensajeLocalizedPokemon(char* nombrePokemon, int cantidad_coordenadas,int tamanio,uint32_t mensajes[]) {

	void* nuevo = (MensajeLocalizedPokemon*) malloc(tamanio);
	void* recorro = nuevo;


	memcpy(recorro, &mensajes[0], sizeof(u_int32_t));
	recorro += sizeof(u_int32_t);


	memcpy(recorro, nombrePokemon, strlen(nombrePokemon));
	recorro += strlen(nombrePokemon);


	memcpy(recorro, &mensajes[1], sizeof(u_int32_t));
	recorro += sizeof(u_int32_t);


	for(int i=0;i<cantidad_coordenadas*2;i++){

	memcpy(recorro, &mensajes[i+2], sizeof(u_int32_t));

	recorro += sizeof(u_int32_t);

	}


	return nuevo;
}




int calcularTamanioLocalizedPokemon(t_result* mensaje){
	int tamanio = 0;
	int cantidadCoord = atoi(mensaje->mensajes->mensajes[3]);

	tamanio+= strlen(mensaje->mensajes->mensajes[2]);
	tamanio+= sizeof ((uint32_t) (strlen(mensaje->mensajes->mensajes[2])));
	tamanio+= sizeof ((uint32_t) ((mensaje->mensajes->mensajes[3])));

	tamanio+= cantidadCoord*2*4;


	return tamanio;
}

int calcularTamanioAppearedPokemon(t_result* mensaje){
	int tamanio=0;

	tamanio+= strlen(mensaje->mensajes->mensajes[2]);
	tamanio+= sizeof ((uint32_t) (strlen(mensaje->mensajes->mensajes[2])));
	tamanio+= sizeof ((uint32_t) ((mensaje->mensajes->mensajes[3])));
	tamanio+= sizeof ((uint32_t) ((mensaje->mensajes->mensajes[4])));

	return tamanio;
}


int calcularTamanioGetPokemon(t_result* mensaje){
	int tamanio=0;

	tamanio+= strlen(mensaje->mensajes->mensajes[2]);
	tamanio+= sizeof ((uint32_t) (strlen(mensaje->mensajes->mensajes[2])));


	return tamanio;
}

int calcularTamanioNewPokemon(t_result* mensaje){

	int tamanio=0;

	tamanio+= strlen(mensaje->mensajes->mensajes[2]);
	tamanio+= sizeof ((uint32_t) (strlen(mensaje->mensajes->mensajes[2])));
	tamanio+= sizeof ((uint32_t) ((mensaje->mensajes->mensajes[3])));
	tamanio+= sizeof ((uint32_t) ((mensaje->mensajes->mensajes[4])));
	tamanio+= sizeof ((uint32_t) ((mensaje->mensajes->mensajes[5])));


	return tamanio;

}

int calcularTamanioCatchPokemon(t_result* mensaje){
	int tamanio=0;

	tamanio+= strlen(mensaje->mensajes->mensajes[2]);
	tamanio+= sizeof ((uint32_t) (strlen(mensaje->mensajes->mensajes[2])));
	tamanio+= sizeof ((uint32_t) ((mensaje->mensajes->mensajes[3])));
	tamanio+= sizeof ((uint32_t) ((mensaje->mensajes->mensajes[4])));

	return tamanio;
}


t_mensajes* desnormalizarMensaje(char* tipoDeMensaje, void* mensaje,int id_mensaje,char* id_mensaje_correlativo){

	t_mensajes* resultado = malloc(sizeof(t_mensajes));

	if(strcmp(tipoDeMensaje,"NEW_POKEMON") == 0){

		void* recorro = mensaje;

		uint32_t* punteroAlLargo = recorro;
		uint32_t largoNombrePokemon = *punteroAlLargo;
		recorro += sizeof(uint32_t);

		char* nombrePokemon = malloc(largoNombrePokemon+1);
		memcpy(nombrePokemon, recorro, largoNombrePokemon);
		nombrePokemon[largoNombrePokemon] = '\0';

		recorro += largoNombrePokemon * sizeof(char);

		uint32_t* punteroAPosx = recorro;
		uint32_t posx = *punteroAPosx;
		recorro += sizeof(uint32_t);

		uint32_t* punteroAPosy = recorro;
		uint32_t posy = *punteroAPosy;
		recorro += sizeof(uint32_t);


		uint32_t* punteroACantidad = recorro;
		uint32_t cantidad = *punteroACantidad;

		resultado->mensajes = malloc(7 * sizeof(char*));

		char* id_asignado = itoa__(id_mensaje);

		resultado->mensajes[0] = string_new();
		string_append(&resultado->mensajes[0], id_asignado);

		resultado->mensajes[1] = string_new();
		string_append(&resultado->mensajes[1], "NEW_POKEMON");

		resultado->mensajes[2] = string_new();
		string_append(&resultado->mensajes[2], nombrePokemon);

		resultado->mensajes[3] = string_new();
		string_append(&resultado->mensajes[3], string_itoa((int)posx));

		resultado->mensajes[4] = string_new();
		string_append(&resultado->mensajes[4], string_itoa((int)posy));

		resultado->mensajes[5] = string_new();
		string_append(&resultado->mensajes[5], string_itoa((int)cantidad));

		resultado->mensajes[6] = string_new();
		string_append(&resultado->mensajes[6], id_asignado);


		resultado->size = malloc(sizeof(uint32_t));
		(*resultado->size) = 7;

	}else if(strcmp(tipoDeMensaje,"CATCH_POKEMON") == 0){

		void* recorro = mensaje;

		uint32_t* punteroAlLargo = recorro;
		uint32_t largoNombrePokemon = *punteroAlLargo;
		recorro += sizeof(uint32_t);

		char* nombrePokemon = malloc(largoNombrePokemon+1);
		memcpy(nombrePokemon, recorro, largoNombrePokemon);
		nombrePokemon[largoNombrePokemon] = '\0';

		recorro += largoNombrePokemon * sizeof(char);

		uint32_t* punteroAPosx = recorro;
		uint32_t posx = *punteroAPosx;
		recorro += sizeof(uint32_t);

		uint32_t* punteroAPosy = recorro;
		uint32_t posy = *punteroAPosy;
		recorro += sizeof(uint32_t);

		resultado->mensajes = malloc(6 * sizeof(char*));

		char* id_asignado = itoa__(id_mensaje);

		resultado->mensajes[0] = string_new();
		string_append(&resultado->mensajes[0], id_asignado);


		resultado->mensajes[1] = string_new();
		string_append(&resultado->mensajes[1], "CATCH_POKEMON");

		resultado->mensajes[2] = string_new();
		string_append(&resultado->mensajes[2], nombrePokemon);

		resultado->mensajes[3] = string_new();
		string_append(&resultado->mensajes[3], string_itoa((int)posx));

		resultado->mensajes[4] = string_new();
		string_append(&resultado->mensajes[4], string_itoa((int)posy));

		resultado->mensajes[5] = string_new();
		string_append(&resultado->mensajes[5], id_asignado);


		resultado->size = malloc(sizeof(uint32_t));
		(*resultado->size) = 6;

	}else if(strcmp(tipoDeMensaje,"APPEARED_POKEMON") == 0){

		void* recorro = mensaje;

		uint32_t* punteroAlLargo = recorro;
		uint32_t largoNombrePokemon = *punteroAlLargo;
		recorro += sizeof(uint32_t);

		char* nombrePokemon = malloc(largoNombrePokemon+1);
		memcpy(nombrePokemon, recorro, largoNombrePokemon);
		nombrePokemon[largoNombrePokemon] = '\0';

		recorro += largoNombrePokemon * sizeof(char);

		uint32_t* punteroAPosx = recorro;
		uint32_t posx = *punteroAPosx;
		recorro += sizeof(uint32_t);

		uint32_t* punteroAPosy = recorro;
		uint32_t posy = *punteroAPosy;
		recorro += sizeof(uint32_t);

		resultado->mensajes = malloc(6 * sizeof(char*));

		char* id_asignado = itoa__(id_mensaje);

		resultado->mensajes[0] = string_new();
		string_append(&resultado->mensajes[0], id_asignado);

		resultado->mensajes[1] = string_new();
		string_append(&resultado->mensajes[1], "APPEARED_POKEMON");

		resultado->mensajes[2] = string_new();
		string_append(&resultado->mensajes[2], nombrePokemon);

		resultado->mensajes[3] = string_new();
		string_append(&resultado->mensajes[3], string_itoa((int)posx));

		resultado->mensajes[4] = string_new();
		string_append(&resultado->mensajes[4], string_itoa((int)posy));


		resultado->mensajes[5] = string_new();
		string_append(&resultado->mensajes[5], id_mensaje_correlativo);



		resultado->size = malloc(sizeof(uint32_t));
		(*resultado->size) = 6;

		}
		else if(strcmp(tipoDeMensaje,"CAUGHT_POKEMON") == 0){

		void* recorro = mensaje;


		uint32_t* punteroConfirmacion = recorro;
		uint32_t confirmacion = *punteroConfirmacion;
		recorro += sizeof(uint32_t);


		resultado->mensajes = malloc(4 * sizeof(char*));

		char* id_asignado = itoa__(id_mensaje);

		resultado->mensajes[0] = string_new();
		string_append(&resultado->mensajes[0], id_asignado);

		resultado->mensajes[1] = string_new();
		string_append(&resultado->mensajes[1], "CAUGHT_POKEMON");


		resultado->mensajes[2] = string_new();
		string_append(&resultado->mensajes[2], string_itoa((int)confirmacion));

		resultado->mensajes[3] = string_new();
		string_append(&resultado->mensajes[3], id_mensaje_correlativo);



		resultado->size = malloc(sizeof(uint32_t));
		(*resultado->size) = 4;



		}else if(strcmp(tipoDeMensaje,"GET_POKEMON") == 0){

		void* recorro = mensaje;

		uint32_t* punteroAlLargo = recorro;
		uint32_t largoNombrePokemon = *punteroAlLargo;
		recorro += sizeof(uint32_t);

		char* nombrePokemon = malloc(largoNombrePokemon+1);
		memcpy(nombrePokemon, recorro, largoNombrePokemon);
		nombrePokemon[largoNombrePokemon] = '\0';

		recorro += largoNombrePokemon * sizeof(char);

		resultado->mensajes = malloc(4 * sizeof(char*));

		char* id_asignado = itoa__(id_mensaje);

		resultado->mensajes[0] = string_new();
		string_append(&resultado->mensajes[0], id_asignado );

		resultado->mensajes[1] = string_new();
		string_append(&resultado->mensajes[1], "GET_POKEMON");

		resultado->mensajes[2] = string_new();
		string_append(&resultado->mensajes[2], nombrePokemon);

		resultado->mensajes[3] = string_new();
		string_append(&resultado->mensajes[3], id_asignado);

		resultado->size = malloc(sizeof(uint32_t));
		(*resultado->size) = 4;


	}else if(strcmp(tipoDeMensaje,"LOCALIZED_POKEMON")== 0){

		void* recorro = mensaje;

		uint32_t* punteroAlLargo = recorro;
		uint32_t largoNombrePokemon = *punteroAlLargo;
		recorro += sizeof(uint32_t);

		char* nombrePokemon = malloc(largoNombrePokemon+1);
		memcpy(nombrePokemon, recorro, largoNombrePokemon);
		nombrePokemon[largoNombrePokemon] = '\0';

		recorro += largoNombrePokemon * sizeof(char);

		uint32_t* puntero_cantidad_coordenadas = recorro;
		uint32_t cantidad_coordenadas = *puntero_cantidad_coordenadas;
		recorro += sizeof(uint32_t);


		resultado->mensajes = malloc((4+(cantidad_coordenadas*2)) * sizeof(char*));

		char* id_asignado = itoa__(id_mensaje);

		resultado->mensajes[0] = string_new();
		string_append(&resultado->mensajes[0], id_asignado);


		resultado->mensajes[1] = string_new();
		string_append(&resultado->mensajes[1], "LOCALIZED_POKEMON");

		resultado->mensajes[2] = string_new();
		string_append(&resultado->mensajes[2], nombrePokemon);

		resultado->mensajes[3] = string_new();
		string_append(&resultado->mensajes[3], string_itoa((int)cantidad_coordenadas));

		for(int i = 0; i<cantidad_coordenadas*2;i++){
			uint32_t* punteroCoordenadas = recorro;
			uint32_t coordenadas = *punteroCoordenadas;
			recorro += sizeof(uint32_t);

			resultado->mensajes[i+4] = string_new();
			string_append(&resultado->mensajes[i+4], string_itoa((int)coordenadas));


		}


		resultado->size = malloc(sizeof(uint32_t));
		(*resultado->size) = (4+(cantidad_coordenadas*2));




	}





	return resultado;
}





