/* main.c */
#include "../include/main.h"
#include <mqueue.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {

	int valorRetorno = 0;

	t_config* config = configuracion_init("gameboy.config");

	if (config == NULL){
		printf("No se pudo cargar la configuracion");
		return -1;
	}

	char* nombreProceso = argv[1];

	if (nombreProceso == NULL) {
		printf("El parametro 1 debe ser el nombre del proceso");
		return -1;
	}

	valorRetorno = enviarMensaje(nombreProceso, argv);


	config_destroy(config);


	return valorRetorno;



}
