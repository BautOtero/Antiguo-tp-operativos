#include "../include/estructuraAdministrativa.h"

Administrativa* crear_estructura_administrativa(t_result* mensaje, int id_mensaje,void* mensaje_memoria,char* id_correlativo){

	Administrativa* estructura_administrativa = malloc(sizeof(Administrativa));

	int* lruInit = malloc(sizeof(int));

	*lruInit = 0;

//	estructura_administrativa->cola_mensaje = malloc(strlen(mensaje->mensajes->mensajes[2])+1);

	if(estructura_administrativa != NULL && mensaje != NULL){

	estructura_administrativa->cola_mensaje = string_new();
	string_append(&estructura_administrativa->cola_mensaje, mensaje->mensajes->mensajes[1]);
	estructura_administrativa->id_mensaje = id_mensaje;
	estructura_administrativa->mensajes = mensaje_memoria;
	initlist(&estructura_administrativa->listaEstados);
	estructura_administrativa->lru = lruInit;
	if(id_correlativo != NULL)
	{
		estructura_administrativa->id_correlativo = string_new();
		string_append(&estructura_administrativa->id_correlativo, id_correlativo);
	}
	else
	{
		estructura_administrativa->id_correlativo = NULL;
	}


	}

	 return estructura_administrativa;

}

ModulosEstados* crearModulo(int modulo,int estado_mensaje){

	ModulosEstados* modulo_estado = malloc(sizeof(ModulosEstados));

	modulo_estado->id_modulo = modulo;
	modulo_estado->estado_mensaje  = estado_mensaje;

	return modulo_estado;



}

void verEstructuraAdministrativa(List* lista){

}




void incrementar_lru(List* estructuras,List* mensaje){
//	IteratorList iterador = NULL;
//	Administrativa *elemento = NULL;
//
//		for(iterador = beginlist(*estructuras);iterador!=NULL;iterador = nextlist(iterador)){
//			elemento = (Administrativa*) iterador->data;
//
//		}

}





void asignarEnvioMensaje(List* estructuras_administrativas, int id_mensaje, int id_modulo){
	IteratorList iterador = NULL;
	Administrativa *elemento = NULL;

		for(iterador = beginlist(*estructuras_administrativas);iterador!=NULL;iterador = nextlist(iterador)){

			elemento = (Administrativa*) dataiterlist(iterador);
			if(elemento->id_mensaje == id_mensaje){
				if((se_encuentra_modulo(estructuras_administrativas,id_modulo)!=0)){

					break;
				}
			}
				break;
		}
}

void asignarConfirmacionMensaje(List* estructuras_administrativas, int id_mensaje, int id_modulo){
	IteratorList iterador = NULL;
	Administrativa *elemento = NULL;

		for(iterador = beginlist(*estructuras_administrativas);iterador!=NULL;iterador = nextlist(iterador)){

			elemento = (Administrativa*) dataiterlist(iterador);
			if(elemento->id_mensaje == id_mensaje){
				if((se_encuentra_modulo(estructuras_administrativas,id_modulo)!=0)){

					break;
				}
			}
				break;
		}
}



int se_encuentra_modulo(List* estructuras_administrativas,int id_modulo){
	IteratorList iterador = NULL;
		Administrativa *elemento = NULL;

			for(iterador = beginlist(*estructuras_administrativas);iterador!=NULL;iterador = nextlist(iterador)){
				elemento = (Administrativa*) dataiterlist(iterador);
				if(elemento->id_mensaje == id_modulo){
					return 1;

				}

			}
			return 0;
}
