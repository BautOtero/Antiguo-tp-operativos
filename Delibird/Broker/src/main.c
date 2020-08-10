#include "../include/main.h"

void signal_handler(){

	log_info(logger,"Se ejecuta el dump de la cache con la signal SIGUSR1 \n");

	if(strcmp(algoritmoMemoria,"PARTICIONES")==0){
	agregar_dump_cache_particiones();
	}else if(strcmp(algoritmoMemoria,"BS")==0){
		agregar_dump_cache_bs();
	}
}

int main(int argc, char *argv[]) {



	m_idAsignado = malloc(sizeof(sem_t));
	sem_init(m_idAsignado, 0, 1);

	sem_listas_admin = malloc(sizeof(sem_t));
	sem_init(sem_listas_admin, 0, 1);

	espero_mensaje_cola = malloc(sizeof(sem_t));
	sem_init(espero_mensaje_cola, 0, 1);


	sem_respuestas_suscripcion = malloc(sizeof(sem_t));
	sem_init(sem_respuestas_suscripcion, 0, 1);



	sem_mensaje_recibido = malloc(sizeof(sem_t));
	sem_init(sem_mensaje_recibido, 0, 1);


	initlist(&particiones);

	// LISTA DE ESTRUCTURA ADMINISTRATIVA
	initlist(&lista_new);
	initlist(&lista_catch);
	initlist(&lista_caught);
	initlist(&lista_localized);
	initlist(&lista_appeared);
	initlist(&lista_get);
	initlist(&lista_suscriptores);

	initlist(&fifo_full);

	lista_fifo_sysBuddy = list_create();

	t_config* config = config_create("broker.config");
	logger = log_create("broker.log", "Broker", true, LOG_LEVEL_INFO);

	ip = config_get_string_value(config, "IP_BROKER");
	puerto = config_get_string_value(config, "PUERTO_BROKER");
	tamMemoriaMaxima = config_get_int_value(config, "TAMANO_MEMORIA");
	tamMinimoParticion = config_get_int_value(config, "TAMANO_MINIMO_PARTICION");
	frecuenciaCompactacion = config_get_int_value(config, "FRECUENCIA_COMPACTACION");
	algoritmoMemoria = config_get_string_value(config, "ALGORITMO_MEMORIA");
	algoritmoReemplazo = config_get_string_value(config, "ALGORITMO_REEMPLAZO");
	algoritmoParticion = config_get_string_value(config, "ALGORITMO_PARTICION_LIBRE");


	punteroAMemoriaPrincipal = malloc(tamMemoriaMaxima);
//TODO AGRREGP MEMORIA AL SISTEMA

	if(strcmp(algoritmoMemoria,"BS")==0){

		buddySystem = create_buddy(tamMemoriaMaxima, 0, punteroAMemoriaPrincipal);

	} else if(strcmp(algoritmoMemoria,"PARTICIONES")==0){

		pushbacklist(&particiones, creacionParticion(tamMemoriaMaxima, 0, punteroAMemoriaPrincipal,NULL));

	}



	signal(SIGUSR1,&signal_handler);

	recibeMensaje();


	return 0;

}

void agregar_dump_cache_particiones(){


	FILE* file_cache = fopen("dumpCacheParticiones.txt","append");

	int tamanio_lista = sizelist(particiones);

	int partid = 0;

	if(partid == tamanio_lista){
		partid = 0;

	}

		if(file_cache == NULL){
			printf("ERROR AL CREAR ARCHIVO dumpCache.txt \n");
		}

		ParticionGeneral* particion;
		IteratorList iter;

		time_t     now;
		struct tm *ts;
		char       buf[80];

		/* Obtener la hora actual */
		now = time(0);

		/* Formatear e imprimir el tiempo, "ddd yyyy-mm-dd hh:mm:ss zzz" */
		ts = localtime(&now);
		strftime(buf, sizeof(buf), "Dump: %a %Y-%m-%d %H:%M:%S", ts);
		fprintf(file_cache,"%s\n", buf);

		for (iter = beginlist(particiones); iter != NULL; iter = nextlist(iter)){

			particion = (ParticionGeneral*) dataiterlist(iter);

			fprintf(file_cache,"| Partición %d: %p - %p |  ",
					partid,
					particion->mensajeGeneral,
					particion->mensajeGeneral+particion->tam-1);
					partid++;
				if(particionLibre(particion)){
					fprintf(file_cache,"[L]  | Size: %d b \n", particion->tam);
				} else {
					fprintf(file_cache,"[X]  ");
					fprintf(file_cache,"| Size: %d b  |  LRU: %d  |  COLA: %s  | ID: %d \n",
							particion->tam,
							*particion->lru,
							particion->estructura_admin->cola_mensaje,
							particion->id_particion);
				}

		}
		fprintf(file_cache,"---------------------------------------------------\n");


		fflush(file_cache);
		fclose(file_cache);

}

void agregar_dump_cache_bs() {

	FILE* file_cache = fopen("dumpCacheBs.txt","append");


	time_t     now;
	struct tm *ts;
	char       buf[80];

	/* Obtener la hora actual */
	now = time(0);
	fprintf(file_cache,"---------------------------------------------------\n");
	/* Formatear e imprimir el tiempo, "ddd yyyy-mm-dd hh:mm:ss zzz" */
	ts = localtime(&now);
	strftime(buf, sizeof(buf), "Dump: %a %Y-%m-%d %H:%M:%S", ts);
	fprintf(file_cache,"%s\n", buf);

	fprintf(file_cache,"| Dirección 0: %p | Mem Libre: %d | Mem Ocupada: %d \n",
			punteroAMemoriaPrincipal,
			tamMemoriaMaxima - sumar_lista(lista_fifo_sysBuddy),
			sumar_lista(lista_fifo_sysBuddy));

	for(int i=0; i<lista_fifo_sysBuddy->elements_count; i++){

		t_list_buddySys *mainBuddy = list_get(lista_fifo_sysBuddy, i);

		fprintf(file_cache,"| Dirección %d: %p - %p | Size: %d b | LRU: %d | COLA: %s | ID: %d \n",
				i+1, mainBuddy->msg,
				mainBuddy->msg+mainBuddy->mem-1,
				mainBuddy->mem,
				*mainBuddy->contador,
				mainBuddy->estructuraAdministrativa->cola_mensaje,
				mainBuddy->estructuraAdministrativa->id_mensaje);

	}
	fprintf(file_cache,"---------------------------------------------------\n");

	fflush(file_cache);
	fclose(file_cache);
}




