#include "configuracion.h"

static t_config* config;

t_config* configuracion_get(){
	return config;
}

t_config* configuracion_init(char *path){
	config = config_create(path);
	return config;
}
