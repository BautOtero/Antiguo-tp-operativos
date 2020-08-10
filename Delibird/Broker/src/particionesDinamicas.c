#include "../include/particionesDinamicas.h"

int particionCompactacion = 0;
int particion_eliminar = 0;
int contador_id = 0;
void* iniciarParticionesDinamicas(Administrativa* estrucAdmin, void* data) {

	return almacenarUnMensaje(data, estrucAdmin);

}


void* almacenarUnMensaje(void* mensaje, Administrativa* estrucAdmin){
	void* resultado = NULL;

	if(strcmp(algoritmoParticion,"BF")==0){
		resultado = algoritmo_best_fit(mensaje, estrucAdmin);


	}else if(strcmp(algoritmoParticion,"FF")==0){
		resultado = algoritmo_first_fit(mensaje, estrucAdmin);

	}

	if (resultado == NULL) {
		log_info(logger,"No se tienen particiones para ese tamaño, se procede a eliminar segun %s",algoritmoReemplazo);

		if(strcmp(algoritmoReemplazo,"FIFO")==0){

				algoritmo_fifo(estrucAdmin);

				consolidacion_mensajes(&particiones);

				if(particionCompactacion == frecuenciaCompactacion){
					log_info(logger,"Se ejecuta compactacion");
					compactacion_mensajesV2(&particiones);
					particionCompactacion=0;
				}
				particionCompactacion++;

				return almacenarUnMensaje(mensaje,estrucAdmin);


		}else if(strcmp(algoritmoReemplazo,"LRU")==0){


				algoritmo_lru(estrucAdmin);

				consolidacion_mensajes(&particiones);

				if(particionCompactacion == frecuenciaCompactacion){
						log_info(logger,"Se ejecuta compactacion");
						compactacion_mensajesV2(&particiones);
						particionCompactacion=0;
				}
				particionCompactacion++;

				return almacenarUnMensaje(mensaje,estrucAdmin);

			}

	}
	else {
			log_info(logger,"Se agrego exitosamente con el ID %d",contador_id);
			contador_id++;
			return resultado;


		} printf("\n");


		return NULL;



}



int algoritmo_fifo(Administrativa* estructAdmin){

	ParticionGeneral* particion = popfrontlist(&fifo_full);
	eliminarMensaje(particion);

	return 0;

}




int algoritmo_lru(Administrativa* estructAdmin){

	ParticionGeneral* particion = NULL;
	IteratorList iter =  NULL;
	iter =  beginlist(particiones);
	ParticionGeneral* eliminar = (ParticionGeneral*) dataiterlist(iter);


		if (!isemptylist(particiones)) {

			for (iter = beginlist(particiones) ; iter != NULL ; iter = nextlist(iter)) {
					particion = (ParticionGeneral*) dataiterlist(iter->next);
					if(particion != NULL && !particionLibre(particion)){

						eliminar = mayorLru(eliminar,particion);

					}
			}

	}


			if(eliminar!= NULL){
				eliminarMensaje(eliminar);
				return 1;

			}

				return 0;

}



ParticionGeneral* mayorLru(ParticionGeneral* particion1, ParticionGeneral* particion2){

	if(particion1->lru != NULL && particion2->lru == NULL){

		return particion1;

	} else if (particion2->lru != NULL && particion1->lru == NULL){

		return particion2;

	}else if(particion1->lru != NULL && particion2->lru != NULL){

		if(*particion1->lru >= *particion2->lru){
			return particion1;
		}else if(*particion2->lru > *particion1->lru){
			return particion2;
		}
	}
	return NULL;


}




ParticionGeneral* creacionParticion(int tam, int direccion, void* mensaje,Administrativa* administrativa) {

	ParticionGeneral* particion = (ParticionGeneral*) malloc(sizeof(ParticionGeneral));

	if (particion != NULL) {
		particion->tam = tam;
		particion->direccion = direccion;
		particion->mensajeGeneral = mensaje+direccion;
		particion->id_particion = contador_id;
		particion->ocupado = 0;
		if(administrativa == NULL){
			particion->estructura_admin = NULL;
			particion->lru = NULL;
		}
		else{
		particion->estructura_admin = administrativa;
		particion->lru = administrativa->lru;
		}


	}

	return particion;
}




int eliminarMensaje(ParticionGeneral* particion) {

	particion->estructura_admin->mensajes = NULL;
	particion->estructura_admin = NULL;
	particion->ocupado = 0;
	log_info(logger,"Se elimino la particion %p",particion->mensajeGeneral);

	return 0;
}


void* algoritmo_first_fit(void* mensajeGeneral, Administrativa* estrucAdmin) {

	IteratorList iter, fiter = NULL;
	ParticionGeneral *aux, *first = NULL;

		for (iter = beginlist(particiones); iter != NULL; iter = nextlist(iter)) {
			aux = (ParticionGeneral*) dataiterlist(iter);
			if (particionLibre(aux) && aux->tam >= estrucAdmin->tam)
				if (first == NULL || estrucAdmin->tam < first->tam) {
					first = aux;
					fiter = iter;
					break;
			}
		}


		if (first != NULL && fiter != NULL) {
			if (first->tam == estrucAdmin->tam) {
				memcpy(first->mensajeGeneral, mensajeGeneral, estrucAdmin->tam);
				first->ocupado = 1;
				first->estructura_admin = estrucAdmin;
				estrucAdmin->mensajes = first->mensajeGeneral;
				pushbacklist(&fifo_full, first);
				return first->mensajeGeneral;
			} else {
				aux = creacionParticion(estrucAdmin->tam, first->direccion, punteroAMemoriaPrincipal,estrucAdmin);
				memcpy(aux->mensajeGeneral, mensajeGeneral, estrucAdmin->tam);
				estrucAdmin->mensajes = aux->mensajeGeneral;
				aux->ocupado = 1;
				aux->estructura_admin = estrucAdmin;
				first->tam -= estrucAdmin->tam;
				first->direccion += estrucAdmin->tam;
				first->mensajeGeneral += estrucAdmin->tam;
				aux->lru = estrucAdmin->lru;
				pushiterlist(&particiones, fiter, aux);
				pushbacklist(&fifo_full, aux);
				return aux->mensajeGeneral;
			}

		}

	return NULL;
}




void* algoritmo_best_fit(void* mensajeGeneral, Administrativa* estrucAdmin) {

	IteratorList iter, biter = NULL;
	ParticionGeneral *aux, *best = NULL;

		for (iter = beginlist(particiones); iter != NULL; iter = nextlist(iter)) {
			aux = (ParticionGeneral*) dataiterlist(iter);
			if (particionLibre(aux) && aux->tam >= estrucAdmin->tam)
				if (best == NULL || estrucAdmin->tam < best->tam) {
					best = aux;
					biter = iter;
			}
		}


		if (best != NULL && biter != NULL) {
			if (best->tam == estrucAdmin->tam) {
				memcpy(best->mensajeGeneral, mensajeGeneral, estrucAdmin->tam);
				best->ocupado = 1;
				best->id_particion = contador_id;
				estrucAdmin->mensajes = best->mensajeGeneral;
				best->estructura_admin = estrucAdmin;
				pushbacklist(&fifo_full, best);
				return best->mensajeGeneral;
			} else {
				aux = creacionParticion(estrucAdmin->tam, best->direccion, punteroAMemoriaPrincipal,estrucAdmin);
				memcpy(aux->mensajeGeneral, mensajeGeneral, estrucAdmin->tam);
				aux->estructura_admin = estrucAdmin;
				estrucAdmin->mensajes = aux->mensajeGeneral;
				aux->ocupado = 1;
				best->tam -= estrucAdmin->tam;
				best->direccion += estrucAdmin->tam;
				best->mensajeGeneral += estrucAdmin->tam;
				aux->lru = estrucAdmin->lru;
				pushiterlist(&particiones, biter, aux);
				pushbacklist(&fifo_full, aux);
				return aux->mensajeGeneral;
			}

		}

	return NULL;
}


void consolidacion_mensajes(List* particiones) {
	ParticionGeneral *actual;
	ParticionGeneral *siguiente;
	IteratorList ic, in;

	int continuar;
	do {
		continuar = 0;
		if (sizelist(*particiones) > 1) {
			for (ic = beginlist(*particiones); ic != NULL && nextlist(ic) != NULL; ic = nextlist(ic)) {
				in = nextlist(ic);
				actual = (ParticionGeneral*) dataiterlist(ic);
				siguiente = (ParticionGeneral*) dataiterlist(in);
				if (particionLibre(actual) && particionLibre(siguiente)) {
					siguiente = (ParticionGeneral*) popiterlist(particiones, in);
					actual->tam += siguiente->tam;
					actual->lru = NULL;
					//free(siguiente);
					continuar = 1;
					break;
				}
			}
		}
	} while (continuar);

}


void compactacion_mensajesV2(List* particiones) {
	ParticionGeneral* particion = NULL, *particionAux = NULL, *particionAus = NULL;
	IteratorList iter, iterAux;
	int i = 1, j = 1;
		if (sizelist(*particiones) > 1) {
			for (iter = beginlist(*particiones); i < sizelist(*particiones); iter = nextlist(iter), i++) {
				particion = (ParticionGeneral*) dataiterlist(iter);
				particionAus = (ParticionGeneral*) dataiterlist(iter->next);
				if (particionLibre(particion)) {
					particionAus->mensajeGeneral = particion->mensajeGeneral;
					particionAus->direccion = particion->direccion;
					for (iterAux = nextlist(iter), j = i + 1; j < sizelist(*particiones); iterAux = nextlist(iterAux), j++){
						particionAus = (ParticionGeneral*) dataiterlist(iterAux);
						particionAux = (ParticionGeneral*) dataiterlist(iterAux->next);
						particionAux->direccion = particionAus->direccion+particionAus->tam;
						particionAux->mensajeGeneral = particionAus->mensajeGeneral+particionAus->tam;

					}
					particion = (ParticionGeneral*) popiterlist(particiones, iter);
					pushbacklist(particiones, particion);
				break;
				}
			}
			consolidacion_mensajes(particiones);
		}

}

void verTablaParticiones() {
	//int partid = 0;

	ParticionGeneral* particion;
	IteratorList iter;

/*
	if (!isemptylist(particiones)) {
		printf("Part. \tTam \tDir \tEstado \n");
		for (iter = beginlist(particiones); iter != NULL; iter = nextlist(iter)) {
			particion = (ParticionGeneral*) dataiterlist(iter);
			printf("%d \t%d \t%d ", partid++, particion->tam, particion->direccion);
			if (particionLibre(particion)) {
					printf("\tLIBRE (direccion de memoria: %p - %p) \n",
							particion->mensajeGeneral,
							particion->mensajeGeneral+particion->tam-1);
			} else {
				printf("\tID %d (tamaño: %d) (direccion de memoria: %p - %p) \n",
						particion->id_particion,
						particion->tam,
						particion->mensajeGeneral,
						particion->mensajeGeneral+particion->tam-1
						*particion->lru
					);

				}
			}
		}
*/
	printf("---------------------------------------------------\n");

	time_t     now;
    struct tm *ts;
    char       buf[80];

    /* Obtener la hora actual */
    now = time(0);

    /* Formatear e imprimir el tiempo, "ddd yyyy-mm-dd hh:mm:ss zzz" */
    ts = localtime(&now);
    strftime(buf, sizeof(buf), "Dump: %a %Y-%m-%d %H:%M:%S", ts);
    printf("%s\n", buf);

	for (iter = beginlist(particiones); iter != NULL; iter = nextlist(iter)){

		particion = (ParticionGeneral*) dataiterlist(iter);

		printf("| Partición %d: %p - %p |  ",
				particion->id_particion,
				particion->mensajeGeneral,
				particion->mensajeGeneral+particion->tam-1);
			if(particionLibre(particion)){
				printf("[L]  | Size: %d b \n", particion->tam);
			} else {
				printf("[X]  ");
				printf("| Size: %d b  |  LRU: %d  |  COLA: %s  | ID: %d \n",
						particion->tam,
						*particion->lru,
						particion->estructura_admin->cola_mensaje,
						particion->id_particion);
			}

	}
	printf("---------------------------------------------------\n");
}



int particionLibre(ParticionGeneral* particion){

	return !particion->ocupado;

}
