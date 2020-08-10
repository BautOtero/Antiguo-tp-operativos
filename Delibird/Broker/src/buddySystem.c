#include "../include/buddySystem.h"

//int contadorID = 0;

t_list_buddySys *create_buddy(int mem, int lvl, void* puntero){
	t_list_buddySys *mainBuddy = malloc(sizeof(t_list_buddySys));
	mainBuddy->mem = mem;
	mainBuddy->lvl = lvl;
	mainBuddy->busy = false;
	mainBuddy->msg = puntero;
	mainBuddy->contador = NULL;
	mainBuddy->left = NULL;
	mainBuddy->right = NULL;
	return mainBuddy;

}

bool tiene_hijos(t_list_buddySys *mainBuddy){

	if(mainBuddy->left != NULL && mainBuddy->right != NULL){
		return 1;
	} else
		return 0;
}


void div_buddy(t_list_buddySys *mainBuddy){

	if(mainBuddy != NULL){

		if(!mainBuddy->busy && !tiene_hijos(mainBuddy)){
			int memSons = mainBuddy->mem / 2;
			int lvlSons = mainBuddy->lvl + 1;

			mainBuddy->left = create_buddy(memSons, lvlSons, mainBuddy->msg);
			mainBuddy->right = create_buddy(memSons, lvlSons, (mainBuddy->msg + memSons));
		}
	}
}


void div_buddy_hasta(t_list_buddySys *mainBuddy, int lvlMaximo){

	if(mainBuddy != NULL){

		if (!(mainBuddy->lvl == lvlMaximo)) {

			div_buddy(mainBuddy);

			div_buddy_hasta(mainBuddy->left, lvlMaximo);
			div_buddy_hasta(mainBuddy->right, lvlMaximo);

		}
	}

}

void liberar_buddy(t_list_buddySys *mainBuddy){


	mainBuddy->msg = NULL;
	free(mainBuddy);

}


int lvl_por_memoria(int memoriaBuscar){

	int memoriaAux = tamMemoriaMaxima;

	if(tamMemoriaMaxima<memoriaBuscar){

		return -1;

	}

	int lvl = 0;

	while(memoriaAux > tamMinimoParticion && (memoriaAux/2)>=memoriaBuscar){

		memoriaAux = memoriaAux / 2;

		lvl++;

	}

	return lvl;

}

void desocupar_buddy(t_list_buddySys *mainBuddy, Administrativa* estrucAdmin){

	if(mainBuddy != NULL){


		mainBuddy->estructuraAdministrativa->mensajes = NULL;
		mainBuddy->busy = false;

	}

}

void liberar_hijos_y_buddy(t_list_buddySys *mainBuddy){

	if(mainBuddy != NULL && ((mainBuddy->left != NULL && tiene_hijos(mainBuddy->left)) || (mainBuddy->right != NULL && tiene_hijos(mainBuddy->right)))){

		liberar_hijos_y_buddy(mainBuddy->left);
		liberar_hijos_y_buddy(mainBuddy->right);

		if(tiene_hijos(mainBuddy) && !mainBuddy->left->busy && !mainBuddy->right->busy){

			log_info(logger,"Se consolidan los buddys con direccion: %p - %p", mainBuddy->left->msg, mainBuddy->right->msg);

			//printf("Se consolidan los buddys con direccion: %p - %p\n", mainBuddy->left->msg, mainBuddy->right->msg);

			liberar_buddy(mainBuddy->left);
			liberar_buddy(mainBuddy->right);

		}


	}

}


void liberar_hijos_buddy(t_list_buddySys *mainBuddy){


	if(mainBuddy != NULL && (mainBuddy->left != NULL || mainBuddy->right != NULL)){

		liberar_hijos_buddy(mainBuddy->left);
		liberar_hijos_buddy(mainBuddy->right);

		if(tiene_hijos(mainBuddy) && !mainBuddy->left->busy && !mainBuddy->right->busy){

			log_info(logger,"Se consolidan los buddys con direccion: %p - %p", mainBuddy->left->msg, mainBuddy->right->msg);

			//printf("Se consolidan los buddys con direccion: %p - %p\n", mainBuddy->left->msg, mainBuddy->right->msg);

			liberar_buddy(mainBuddy->left);
			mainBuddy->left = NULL;
			liberar_buddy(mainBuddy->right);
			mainBuddy->right = NULL;

		}


	}

	/*
	if(mainBuddy != NULL){
		liberar_hijos_y_buddy(mainBuddy->left);
		mainBuddy->left = NULL;
		liberar_hijos_y_buddy(mainBuddy->right);
		mainBuddy->right = NULL;
	}
	*/
}


void* ocupar_buddy(t_list_buddySys *mainBuddy, void* mensaje, Administrativa* estrucAdmin){
	if(mainBuddy != NULL){
		mainBuddy->busy = true;
		liberar_hijos_buddy(mainBuddy);
		mainBuddy->contador = estrucAdmin->lru;
		mainBuddy->left = NULL;
		mainBuddy->right = NULL;
		memcpy(mainBuddy->msg, mensaje, estrucAdmin->tam);
		estrucAdmin->mensajes = mainBuddy->msg;
		mainBuddy->estructuraAdministrativa = estrucAdmin;
		//free(mensaje);
		return mainBuddy->msg;
	}
	return NULL;
}


bool condicion_buddy_libre(t_list_buddySys *mainBuddy){

	if(tiene_hijos(mainBuddy)){
		return (!mainBuddy->left->busy && !mainBuddy->right->busy);
	}

	return (!mainBuddy->busy);

}

void list_buddy_lvl(t_list_buddySys *mainBuddy, t_list *lista_buddys, int lvlBuscar){

	if(mainBuddy != NULL){

		if(mainBuddy->lvl == lvlBuscar){

			list_add(lista_buddys, mainBuddy);

		}

		list_buddy_lvl(mainBuddy->left, lista_buddys, lvlBuscar);

		list_buddy_lvl(mainBuddy->right, lista_buddys, lvlBuscar);

	}

}


t_link_element* lista_find_element(t_list *self, bool(*condition)(t_list_buddySys*), int* index){

	t_link_element *element = self->head;
	int position = 0;

	while (element != NULL && !condition(element->data)){

		element = element->next;
		position++;

	}

	if(index != NULL){

		*index = position;
	}

	return element;

}

void* lista_find(t_list *self, bool(*condition)(t_list_buddySys*)){

	t_link_element *element = lista_find_element(self, condition, NULL);

	return element != NULL ? element->data : NULL;

}

t_list_buddySys *buscar_buddy_libre(t_list_buddySys *mainBuddy, int lvlRecursivo, int lvlBuscar){

	t_list *lista_buddys = list_create();

	list_buddy_lvl(mainBuddy, lista_buddys, lvlRecursivo);

	t_list_buddySys *aux_buddy = lista_find(lista_buddys, condicion_buddy_libre);

	if(aux_buddy != NULL && lvlRecursivo != lvlBuscar){

		div_buddy(aux_buddy);

		list_destroy(lista_buddys);

		return buscar_buddy_libre(mainBuddy, lvlRecursivo+1, lvlBuscar);

	} if(aux_buddy == NULL && lvlRecursivo != 0){

		list_destroy(lista_buddys);

		return buscar_buddy_libre(mainBuddy, lvlRecursivo-1, lvlBuscar);

	}

	list_destroy(lista_buddys);

	return aux_buddy;

}

void actualizar_busy_sysBuddy(t_list_buddySys *mainBuddy){

	if(mainBuddy != NULL){

		actualizar_busy_sysBuddy(mainBuddy->left);
		actualizar_busy_sysBuddy(mainBuddy->right);

		if(tiene_hijos(mainBuddy) && (mainBuddy->left->busy || mainBuddy->right->busy)){
			mainBuddy->busy = true;
		} else if(condicion_buddy_libre(mainBuddy)){
			mainBuddy->busy = false;
		}
	}
}

void consolidar_sysBuddy(t_list_buddySys *mainBuddy){

	if(mainBuddy != NULL){

		consolidar_sysBuddy(mainBuddy->left);
		consolidar_sysBuddy(mainBuddy->right);

		if (!mainBuddy->busy){
			liberar_hijos_buddy(mainBuddy);
		}

	}

}


void* ejecutar_fifo(t_list_buddySys *mainBuddy, void* mensaje, Administrativa* estrucAdmin){

	int lvlMaximo = lvl_por_memoria(estrucAdmin->tam);
	//FIFO
	while (buscar_buddy_libre(mainBuddy, lvlMaximo, lvlMaximo) == NULL){

		t_list *listaAux = list_take_and_remove(lista_fifo_sysBuddy, 1);

		desocupar_buddy(listaAux->head->data, estrucAdmin);

		actualizar_busy_sysBuddy(mainBuddy);

		list_destroy(listaAux);

	}

	return asignar_memoria_fifo(mainBuddy, mensaje, estrucAdmin);

}

void* asignar_memoria_fifo(t_list_buddySys *mainBuddy, void* mensaje, Administrativa* estrucAdmin){

	int lvlMaximo = lvl_por_memoria(estrucAdmin->tam);

	if(lvlMaximo >= 0){

		if(mainBuddy != NULL){

			t_list_buddySys *aux_buddy = buscar_buddy_libre(mainBuddy, lvlMaximo, lvlMaximo);

			if(aux_buddy != NULL){

				void* posicionMensaje = ocupar_buddy(aux_buddy, mensaje, estrucAdmin);

				printf("\nMemoria asignada correctamente\n");

				list_add(lista_fifo_sysBuddy, aux_buddy);

				actualizar_busy_sysBuddy(mainBuddy);

				liberar_hijos_buddy(mainBuddy);

				return posicionMensaje;

			} else {

				printf("Insuficiente memoria disponible. Ejecutando algoritmo de reemplazo: FIFO\n");

				return ejecutar_fifo(mainBuddy, mensaje, estrucAdmin);

			}
		}
	}

	return NULL;

}



bool mayorAOtro(t_list_buddySys* uno, t_list_buddySys* otro){

	return *uno->contador >= *otro->contador;

}

void sumar_uno(t_list* self){
	t_link_element *element = self->head;
	t_link_element *aux = NULL;
	while (element != NULL) {
		aux = element->next;
		element->data += 1;
		element = aux;
	}
}

void ordenar_lista(t_list *self, bool (*comparator)(t_list_buddySys *, t_list_buddySys *)) {
	int unsorted_elements = self->elements_count;
	if(unsorted_elements < 2) {
		return;
	}
	t_link_element *auxiliar = NULL;
	bool sorted = true;
	do {
		t_link_element *previous_element = self->head, *cursor = previous_element->next;
		sorted = true;
		int index = 0, last_changed = unsorted_elements;
		while(index < unsorted_elements && cursor != NULL) {
			if(!comparator(previous_element->data, cursor->data)) {
			   auxiliar = cursor->data;
			   cursor->data = previous_element->data;
			   previous_element->data = auxiliar;
			   last_changed = index;
			   sorted = false;
			}
			previous_element = cursor;
			cursor = cursor->next;
			index++;
		}
		unsorted_elements = last_changed;
	} while(!sorted);

}

void* ejecutar_lru(t_list_buddySys *mainBuddy, void* mensaje, Administrativa* estrucAdmin){

	int lvlMaximo = lvl_por_memoria(estrucAdmin->tam);
	//LRU
	ordenar_lista(lista_fifo_sysBuddy, mayorAOtro);

	while (buscar_buddy_libre(mainBuddy, lvlMaximo, lvlMaximo) == NULL){

		t_list *listaAux = list_take_and_remove(lista_fifo_sysBuddy, 1);

		desocupar_buddy(listaAux->head->data, estrucAdmin);

		actualizar_busy_sysBuddy(mainBuddy);

		list_destroy(listaAux);

	}

	return asignar_memoria_fifo(mainBuddy, mensaje, estrucAdmin);

}


void* asignar_memoria_lru(t_list_buddySys *mainBuddy, void* mensaje, Administrativa* estrucAdmin){

	int lvlMaximo = lvl_por_memoria(estrucAdmin->tam);

	if(lvlMaximo >= 0){

		if(mainBuddy != NULL){

			t_list_buddySys *aux_buddy = buscar_buddy_libre(mainBuddy, lvlMaximo, lvlMaximo);

			if(aux_buddy != NULL){

				void* posicionMensaje = ocupar_buddy(aux_buddy, mensaje, estrucAdmin);

				printf("\nMemoria asignada correctamente\n");

				list_add(lista_fifo_sysBuddy, aux_buddy);

				actualizar_busy_sysBuddy(mainBuddy);

				liberar_hijos_buddy(mainBuddy);

				return posicionMensaje;

			} else {

				printf("Insuficiente memoria disponible. Ejecutando algoritmo de reemplazo: FIFO\n");

				return ejecutar_lru(mainBuddy, mensaje, estrucAdmin);

			}
		}
	}

	return NULL;

}


void* almacenarMensajeBuddySystem(void* mensaje, Administrativa* estrucAdmin){

	if(strcmp(algoritmoReemplazo, "FIFO") == 0){

		return asignar_memoria_fifo(buddySystem, mensaje, estrucAdmin);

	} else if (strcmp(algoritmoReemplazo, "LRU") == 0){

		return asignar_memoria_lru(buddySystem, mensaje, estrucAdmin);

	}

	return NULL;


}

void* iniciarBuddySystem(Administrativa* estrucAdmin, void* data){

	return almacenarMensajeBuddySystem(data, estrucAdmin);


}

int sumar_lista(t_list* lista){

	int aux = 0;

	for(int i=0; i<lista_fifo_sysBuddy->elements_count; i++){

		t_list_buddySys *mainBuddy = list_get(lista, i);

		aux += mainBuddy->mem;

	}

	return aux;

}

void mostrarBuddySystem(t_list_buddySys *unBuddy){

	if(unBuddy == NULL){
		return;
	} else {
		printf("%p \t%d \t%d ", unBuddy->msg, unBuddy->mem, unBuddy->lvl);
		mostrarBuddySystem(unBuddy->right);
		mostrarBuddySystem(unBuddy->left);
	}
}

void verTablaBuddySystem() {

	time_t     now;
	struct tm *ts;
	char       buf[80];

	/* Obtener la hora actual */
	now = time(0);
	printf("---------------------------------------------------\n");
	/* Formatear e imprimir el tiempo, "ddd yyyy-mm-dd hh:mm:ss zzz" */
	ts = localtime(&now);
	strftime(buf, sizeof(buf), "Dump: %a %Y-%m-%d %H:%M:%S", ts);
	printf("%s\n", buf);

	printf("| Dirección 0: %p | Mem Libre: %d | Mem Ocupada: %d \n",
			punteroAMemoriaPrincipal,
			tamMemoriaMaxima - sumar_lista(lista_fifo_sysBuddy),
			sumar_lista(lista_fifo_sysBuddy));

	for(int i=0; i<lista_fifo_sysBuddy->elements_count; i++){

		t_list_buddySys *mainBuddy = list_get(lista_fifo_sysBuddy, i);

		printf("| Dirección %d: %p - %p | Size: %d b | LRU: %d | COLA: %s | ID: %d \n",
				i+1, mainBuddy->msg,
				mainBuddy->msg+mainBuddy->mem-1,
				mainBuddy->mem,
				*mainBuddy->contador,
				mainBuddy->estructuraAdministrativa->cola_mensaje,
				mainBuddy->estructuraAdministrativa->id_mensaje);

	}
	printf("---------------------------------------------------\n");

}



