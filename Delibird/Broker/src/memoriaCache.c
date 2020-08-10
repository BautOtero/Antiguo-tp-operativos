#include "../include/memoriaCache.h"


void* iniciarMemoriaCache(Administrativa * admin_almacenar, t_result* resultado){

	MensajeGeneral * mensajeCache = creacionMensajePosta(resultado);

	admin_almacenar->tam = mensajeCache->tam;

 	if(strcmp(algoritmoMemoria,"BS")==0){
 		return iniciarBuddySystem(admin_almacenar, mensajeCache->data);
 	}
 	else if(strcmp(algoritmoMemoria,"PARTICIONES")==0){
 		return iniciarParticionesDinamicas(admin_almacenar, mensajeCache->data);
 	}

 	return NULL;

}

