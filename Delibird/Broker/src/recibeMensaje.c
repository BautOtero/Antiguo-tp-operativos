#include "../include/recibeMensaje.h"

int id_asignado = 0;
int lruInti = 0;
int *lruIniti = &lruInti;



int recibeMensaje(void){

    iniciar_servidor(ip, puerto, handle_client);

    return 0;//EXIT_SUCCESS;
}


void handle_client(t_result* resultado)
{

	//PUBLISHER {{COLA}} [{{PARAMETROS}}]
	//SUSCRIPTOR {{COLA}} {{ID_SUSCRIPTOR}}
	//ACK {{COLA}} {{ID_MENSAJE}} {{ID_SUSCRIPTOR}}

	const char* tipoColaMensaje = resultado->mensajes->mensajes[0];
	const char* colaEnviarMensaje = "PUBLISHER";
	const char* colaSuscriptor = "SUSCRIPTOR";

	if(strcmp(tipoColaMensaje, colaEnviarMensaje) == 0){

		sem_wait(sem_respuestas_suscripcion);
		handle_client_publisher(resultado);
		sem_post(sem_respuestas_suscripcion);

	}

	else if(strcmp(tipoColaMensaje,colaSuscriptor) == 0){

		sem_wait(sem_respuestas_suscripcion);
		handle_client_suscriptor(resultado);
		sem_post(sem_respuestas_suscripcion);

	}

	else if(strcmp(tipoColaMensaje,"ACK")==0){

		log_info(logger,"Se recibe un ack del mensaje %s del modulo %s", resultado->mensajes->mensajes[2],resultado->mensajes->mensajes[3]);

		handle_client_ack(resultado);

	}


}

void handle_client_publisher(t_result* resultado){

	void* resultadoCache = NULL;
	int socketRespuesta = resultado->socket;
	char* colaMensaje = string_new();
	string_append(&colaMensaje, resultado->mensajes->mensajes[1]);

	char* id_correlativo = obtener_id_correlativo(resultado);

	int id = asignarID();

	char* idAsignado = itoa__(id);

	enviarID(idAsignado,socketRespuesta);

	if (socketRespuesta != -1){
		liberar_conexion(socketRespuesta);
	}


	sem_wait(espero_mensaje_cola);

	Administrativa * admin_almacenar = crear_estructura_administrativa(resultado, id, NULL, id_correlativo);

	resultadoCache = iniciarMemoriaCache(admin_almacenar, resultado);

	log_info(logger,"Se agrego a la direccion de memoria %p", resultadoCache);

	if(resultadoCache != NULL && admin_almacenar != NULL){
		pushfrontlist(lista_respuesta(colaMensaje), admin_almacenar);
	}



	sem_post(sem_listas_admin);


	//pthread_t thread_enviar_guardados;


	parametros_respuesta_suscripcion* parametros = malloc(sizeof(parametros_respuesta_suscripcion));

	parametros->colaMensaje = colaMensaje;



	parametros->socketRespuesta = -1;
	parametros->lista = lista_respuesta(colaMensaje);


	//pthread_create(&thread_enviar_guardados,NULL,(void*)enviar_mensajes_suscriptos,parametros);

	enviar_mensajes_suscriptos(parametros);

	//pthread_detach(thread_enviar_guardados);
	sem_post(espero_mensaje_cola);




}


void handle_client_suscriptor(t_result* resultado){

	sem_wait(espero_mensaje_cola);
	log_info(logger,"Se suscribe a la cola %s , el modulo con ID: %s",resultado->mensajes->mensajes[1],resultado->mensajes->mensajes[2]);

	sem_wait(sem_listas_admin);
	int estado = agregar_suscriptor_cola(resultado);
	sem_post(sem_listas_admin);

	if (estado == 0){
		printf("No se agrego el suscriptor a la cola, no tiene ID");
		return ;
	}
	int socketRespuesta = resultado->socket;

    char* colaMensaje = resultado->mensajes->mensajes[1];

	char * respuesta = string_new();

	string_append(&respuesta, "Confirmacion de suscripcion a la cola: ");

	string_append(&respuesta, colaMensaje);

	enviar_mensaje(respuesta, socketRespuesta);


	parametros_respuesta_suscripcion* parametros = malloc(sizeof(parametros_respuesta_suscripcion));

	parametros->colaMensaje = colaMensaje;
	parametros->socketRespuesta = socketRespuesta;
	parametros->lista = lista_respuesta(colaMensaje);
	parametros->modulo_suscriptor = resultado->mensajes->mensajes[2];


	enviarMensajesGuardadosCola(parametros);

	sem_post(espero_mensaje_cola);

}


void handle_client_ack(t_result* resultado){

	Administrativa* admin = NULL;
	IteratorList iterador = NULL;
	char* id_mensaje = resultado->mensajes->mensajes[2];
	char* id_modulo = resultado->mensajes->mensajes[3];

	for(iterador = beginlist(*lista_respuesta(resultado->mensajes->mensajes[1]));iterador!=NULL;iterador = nextlist(iterador)){
		admin = (Administrativa*) dataiterlist(iterador);

		if(admin->id_mensaje == atoi(id_mensaje)){

			setear_modulo(admin,2,id_modulo);

		}

	}

	if (resultado->socket != -1){
		liberar_conexion(resultado->socket);
	}


}

void setear_modulo(Administrativa* administrativa,int seteo, char* id_modulo){

	IteratorList iterador = NULL;
	IteratorList moduloEncontrado = NULL;

	int id_modulo_posta = atoi(id_modulo);

	ModulosEstados* modulos = NULL; //SI ROMPE ACA ESTA LA CUESTION

	if(isemptylist(administrativa->listaEstados)){
		pushfrontlist(&administrativa->listaEstados,crearModulo(id_modulo_posta,seteo));

	}else{

		for(iterador = beginlist((administrativa->listaEstados));iterador!=NULL;iterador = nextlist(iterador)){

			modulos = (ModulosEstados*) dataiterlist(iterador);

			if(modulos->id_modulo == id_modulo_posta){
				moduloEncontrado = iterador;
				break;

			}

		}

		if (moduloEncontrado != NULL){
			modulos->estado_mensaje = seteo;
		}
		else {
			pushfrontlist(&administrativa->listaEstados,crearModulo(id_modulo_posta,seteo));
		}
	}

}

void ver_seteo(Administrativa* admin){

	IteratorList iterador = NULL;

	ModulosEstados* modulos = NULL;


	for(iterador = beginlist((admin->listaEstados));iterador!=NULL;iterador = nextlist(iterador)){
		modulos = (ModulosEstados*)dataiterlist(iterador);


		printf("El estado del modulo es %d y el del mensaje es %d \n ",modulos->id_modulo,modulos->estado_mensaje);

	}

}



int agregar_suscriptor_cola(t_result* resultado){

	Suscriptor* suscriptor = malloc(sizeof(Suscriptor));

	Suscriptor* suscriptorAux;

	suscriptor->cola_suscripcion = malloc(strlen(resultado->mensajes->mensajes[1])+ 1);

	strcpy(suscriptor->cola_suscripcion, resultado->mensajes->mensajes[1]);

	int id = atoi(resultado->mensajes->mensajes[2]);

	if (id == 0){
		return 0;
	}

	suscriptor->id_suscripcion = string_new();

	string_append(&suscriptor->id_suscripcion, resultado->mensajes->mensajes[2]);

	suscriptor->socket_suscriptor = resultado->socket;

	for(IteratorList iter = beginlist(lista_suscriptores); iter != NULL; iter = nextlist(iter)){

		suscriptorAux = iter->data;

		if(strcmp(suscriptor->id_suscripcion, suscriptorAux->id_suscripcion) == 0){

			if(strcmp(suscriptor->cola_suscripcion, suscriptorAux->cola_suscripcion) == 0){

				suscriptor->socket_suscriptor = suscriptorAux->socket_suscriptor;

				return 1;

			}
		}

	}

	pushfrontlist(&lista_suscriptores,suscriptor);

	return 1;

};

char* obtener_id_correlativo(t_result* resultado){

	if(strcmp(resultado->mensajes->mensajes[1],"CAUGHT_POKEMON") == 0){
		return resultado->mensajes->mensajes[3];
	}
	else if(strcmp(resultado->mensajes->mensajes[1],"APPEARED_POKEMON")==0){
		return resultado->mensajes->mensajes[5];

	}

	return NULL;

}


void enviarMensajeASuscriptorSiNoLoRecibio(Administrativa* admin, Suscriptor* suscriptor){

	IteratorList miModuloIter = NULL;

	for (IteratorList iter = beginlist(admin->listaEstados); iter != NULL; iter = nextlist(iter)){

		ModulosEstados* moduloEstado = iter->data;

		if (moduloEstado->id_modulo == atoi(suscriptor->id_suscripcion)){
			miModuloIter = iter;
		}
	}

	if (miModuloIter != NULL){
		ModulosEstados* miModulo = miModuloIter->data;

		if (miModulo->estado_mensaje == 2 || miModulo->estado_mensaje == 1){
			return;
		}
	}

	t_mensajes* mensaje = desnormalizarMensaje(admin->cola_mensaje, admin->mensajes,admin->id_mensaje,admin->id_correlativo);

	sumar_uno_lru_lista();

	setear_modulo(admin,1, suscriptor->id_suscripcion);

	*admin->lru = 0;

	send_messages_socket(suscriptor->socket_suscriptor, mensaje->mensajes,(int)*mensaje->size);

	//ENVIAR EL MENSAJE Y AGREGARLO

}

void enviar_mensajes_suscriptos(parametros_respuesta_suscripcion* parametros){

	for (IteratorList iter = beginlist(*lista_respuesta(parametros->colaMensaje)); iter != NULL; iter = nextlist(iter)){

		Administrativa* admin = iter->data;

		if (admin != NULL && admin->mensajes != NULL){

			for (IteratorList iteracionSuscriptor = beginlist(lista_suscriptores); iteracionSuscriptor != NULL ;iteracionSuscriptor = nextlist(iteracionSuscriptor)){

				Suscriptor* suscriptor = iteracionSuscriptor->data;

				if (strcmp(suscriptor->cola_suscripcion, parametros->colaMensaje)==0){
					enviarMensajeASuscriptorSiNoLoRecibio(admin, suscriptor);
				}


			}

		}

	}
}


int socket_levantado(int socket_fd){

	int error = 0;
	socklen_t len = sizeof (error);
	int retval = getsockopt (socket_fd, SOL_SOCKET, SO_ERROR, &error, &len);

	if (retval != 0) {
	       return 0;
	}

	if (error != 0) {
	       return 0;
	}
	return 1;
}



void enviarMensajesGuardadosCola(parametros_respuesta_suscripcion* parametros){

	//LA COLA DE MENSAJES VA A SER DE TIPO ADMINISTRATIVA

	IteratorList iterador = NULL;

	Administrativa* administrativa = NULL;


	for(iterador = beginlist(*parametros->lista);iterador!=NULL;iterador = nextlist(iterador)){

		administrativa = (Administrativa*) dataiterlist(iterador);

		if(administrativa != NULL && administrativa->mensajes != NULL && !esta_seteado(administrativa,parametros->modulo_suscriptor)){

			t_mensajes* mensaje = desnormalizarMensaje(administrativa->cola_mensaje, administrativa->mensajes,administrativa->id_mensaje, administrativa->id_correlativo);

			//TODO SEND_MESSAGES_SOCKET TENDRIA QUE DEVOLVER UN INT

			send_messages_socket(parametros->socketRespuesta, mensaje->mensajes, (int)*mensaje->size);

			//TODO FALTA LOGGEAR ACA

			sumar_uno_lru_lista();

			setear_modulo(administrativa,1, parametros->modulo_suscriptor);

			*administrativa->lru = 0;


		}

	}

}


int esta_seteado(Administrativa* administrativa, char* id_modulo){

	int idModulo = atoi(id_modulo);
	IteratorList iterador = NULL;
	ModulosEstados* modulos_estados = NULL;

	for(iterador = beginlist(administrativa->listaEstados);iterador!=NULL;iterador = nextlist(iterador)){
		modulos_estados = (ModulosEstados*) dataiterlist(iterador);

		if(modulos_estados->id_modulo == idModulo &&  modulos_estados->estado_mensaje == 2){

			return 1;
		}


	}

	return 0;

}




int esta_en_memoria(List* memoria){

	IteratorList iterador = NULL;

	Administrativa* mensaje = NULL;

	for(iterador = beginlist(*memoria);iterador!=NULL;iterador = nextlist(iterador)){
		mensaje = (Administrativa*) iterador->data;

		if(mensaje->mensajes != NULL){
			return 1;

		}
	}

	return 0;
}






void sumar_uno_lru_lista(){

	Administrativa *estrucAdministrativa;
	IteratorList iter;

	for(iter = beginlist(lista_new); iter != NULL; iter = nextlist(iter)){

		estrucAdministrativa = iter->data;

		*estrucAdministrativa->lru += 1;

	} for(iter = beginlist(lista_caught); iter != NULL; iter = nextlist(iter)){

		estrucAdministrativa = iter->data;

		*estrucAdministrativa->lru += 1;

	} for(iter = beginlist(lista_catch); iter != NULL; iter = nextlist(iter)){

		estrucAdministrativa = iter->data;

		*estrucAdministrativa->lru += 1;

	} for(iter = beginlist(lista_get); iter != NULL; iter = nextlist(iter)){

		estrucAdministrativa = iter->data;

		*estrucAdministrativa->lru += 1;

	} for(iter = beginlist(lista_appeared); iter != NULL; iter = nextlist(iter)){

		estrucAdministrativa = iter->data;

		*estrucAdministrativa->lru += 1;

	} for(iter = beginlist(lista_localized); iter != NULL; iter = nextlist(iter)){

		estrucAdministrativa = iter->data;

		*estrucAdministrativa->lru += 1;

	}

}






List* lista_respuesta(char* cola_mensaje){

	if(strcmp(cola_mensaje,"NEW_POKEMON") == 0){
		return &lista_new;

	}
	else if(strcmp(cola_mensaje,"APPEARED_POKEMON") == 0){
		return &lista_appeared;

	}
	else if(strcmp(cola_mensaje,"CATCH_POKEMON") == 0){
		return &lista_catch;

	}
	else if(strcmp(cola_mensaje,"CAUGHT_POKEMON") == 0){
		return &lista_caught;
	}
	else if(strcmp(cola_mensaje,"GET_POKEMON") == 0){
		return &lista_get;
	}
	else if(strcmp(cola_mensaje,"LOCALIZED_POKEMON") == 0){
		return &lista_localized;
	}


	return NULL;
}



int asignarID(){

	sem_wait(m_idAsignado);
	id_asignado = id_asignado+1;
	int respuesta = id_asignado;
	sem_post(m_idAsignado);

	return respuesta;
}


char *itoa__(int value){
  int count,
    i,
    sign;
  char *ptr,
    *string,
    *temp;

  count = 0;
  if ((sign = value) < 0)
    {
      value = -value;
      count++;
    }

  temp = (char *) malloc(sizeof(int) + 2);
  if (temp == NULL)
    {
      return(NULL);
    }
  memset(temp,'\0', sizeof(int) + 2);

  string = (char *) malloc(sizeof(int) + 2);
  if (string == NULL)
    {
      return(NULL);
    }
  memset(string,'\0', sizeof(int) + 2);
  ptr = string; /* set temporary ptr to string */


  do {
    *temp++ = value % 10 + '0'; /* obtain modulus and or with '0' */
    count++; /* increment count, track iterations*/
  } while (( value /= 10) >0);

  if (sign < 0)
    *temp++ = '-';

  *temp-- = '\0';

  for (i = 0; i < count; i++, temp--, ptr++)
    {
      memcpy(ptr,temp,sizeof(char));
    }

  return(string);
}
