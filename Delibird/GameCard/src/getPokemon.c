#include "../include/getPokemon.h"

int get_pokemon(char* idmensaje, char * pokemon) {

	int cantidad = 0;
	List listaCoordenadas;

	initlist(&listaCoordenadas);

	//Verificar si el Pokémon existe dentro de nuestro Filesystem. Para esto se deberá buscar dentro del directorio Pokemon, si existe el archivo con el nombre de nuestro pokémon. En caso de no existir se deberá informar el mensaje sin posiciones ni cantidades.

	char* pathPokemon = checkPokemon(pokemon);

	if (pathPokemon == NULL){
		printf("No existe ese pokemon\n");
		return 0;
	}

	//Verificar si se puede abrir el archivo (si no hay otro proceso que lo esté abriendo). En caso que el archivo se encuentre abierto se deberá reintentar la operación luego de un tiempo definido por configuración.

	Metadataarchivo contenidoArchivo = abrirArchivoTallgrass(pathPokemon);

	//Obtener todas las posiciones y cantidades de Pokemon requerido.

	obtenerPosicionesYCantidades(&cantidad, &listaCoordenadas, &contenidoArchivo);

	//Esperar la cantidad de segundos definidos por archivo de configuración.
	sleep(config_get_int_value(configuracion_get(), "TIEMPO_RETARDO_OPERACION"));

	//Cerrar el archivo

	cerrarArchivoTallgrass(contenidoArchivo, pathPokemon);

	//Conectarse al Broker y enviar el mensaje a la Cola de Mensajes APPEARED_POKEMON con los los datos:
	enviarLocalizedPokemon(idmensaje, pokemon, cantidad, &listaCoordenadas);

	return 0;
}

void agregarPokemon(int* cantidad, List* listaCoordenadas, char * line){
	char** saveptr = malloc(sizeof(char*));
	char * posx = strtok_r(line, "-", saveptr);

	char * resto = strtok_r(NULL, "-", saveptr);
	char * posy = strtok_r(resto, "=", saveptr);

	char * cantidadActual = strtok_r(NULL, "=", saveptr);

	int cantidadActualInt = atoi (cantidadActual);

	*cantidad += cantidadActualInt;

	Coordenadas *coordenada = malloc(sizeof(Coordenadas));

	coordenada->posx = string_new();
	coordenada->posy = string_new();

	string_append(&coordenada->posx, posx);
	string_append(&coordenada->posy, posy);

	pushbacklist(listaCoordenadas, coordenada);

}

void obtenerPosicionesYCantidades(int* cantidad, List* listaCoordenadas, Metadataarchivo* contenidoArchivo){
	char * texto = leerTextoDeBloques(contenidoArchivo->blocks, contenidoArchivo->blockslen);

	if (texto != NULL){
		char ** saveptr = malloc(sizeof(char*));
		char * first = strtok_r(texto, "\n", saveptr);

		while ( first != NULL ) {

			agregarPokemon(cantidad, listaCoordenadas, first);

			first = strtok_r(NULL, "\n", saveptr);

		}

		free(saveptr);
	}
}

int enviarLocalizedPokemonV2(char* idmensaje, char * pokemon, int cantidad, List * listaCordenadas){

	char* ipBroker = config_get_string_value(configuracion_get(), "IP_BROKER");
	char* puertoBroker = config_get_string_value(configuracion_get(), "PUERTO_BROKER");


	int cantidadLista = listaCordenadas->size;

	int cantidadMensajes = 4 + cantidadLista * 2;

	char* mensajes[cantidadMensajes];
	mensajes[0] = "PUBLISHER";
	mensajes[1] = "LOCALIZED_POKEMON";
	mensajes[2] = pokemon;
	mensajes[3] = malloc(200);
	sprintf(mensajes[4], "%d", cantidadLista);

	IteratorList iterator = beginlist(*listaCordenadas);

	int i = 0;
	while (iterator != NULL){

		Coordenadas* coordenadas = iterator->data;

		mensajes[4+i*2] = coordenadas->posx;
		mensajes[5+i*2] = coordenadas->posy;

		iterator = nextlist(iterator);
		i++;
	}



	printList(listaCordenadas);

	int socket =send_messages_and_return_socket(ipBroker, puertoBroker, mensajes, cantidadMensajes);

	if (socket != -1){
		liberar_conexion(socket);
	}

	free(mensajes[3]);

	return 0;
}

int enviarLocalizedPokemon(char* idmensaje, char * pokemon, int cantidad, List * listaCordenadas){

	char* ipBroker = config_get_string_value(configuracion_get(), "IP_BROKER");
	char* puertoBroker = config_get_string_value(configuracion_get(), "PUERTO_BROKER");

	//t_mensajes* mensajes =  malloc(sizeof(t_mensajes));
	t_mensajes* mensajes =  malloc(sizeof(t_mensajes));

	int cantidadLista = listaCordenadas->size;

	int cantidadMensajes = 4 + cantidadLista * 2;

	mensajes->mensajes = malloc((cantidadMensajes) * sizeof(char*));

	mensajes->mensajes[0] = "PUBLISHER";
	mensajes->mensajes[1] = "LOCALIZED_POKEMON";
	mensajes->mensajes[2] = pokemon;
	mensajes->mensajes[3] = itoa__(cantidadLista);

	IteratorList iterator = beginlist(*listaCordenadas);

	int i = 0;
	while (iterator != NULL){

		Coordenadas* coordenadas = iterator->data;

		mensajes->mensajes[4+(i*2)] = coordenadas->posx;
		mensajes->mensajes[5+(i*2)] = coordenadas->posy;

		iterator = nextlist(iterator);
		i++;
	}
/*
	for(int i=0;i<cantidad_coordenadas*2;i++){
		mensajes->mensajes[i+4] = argv[i+5];
	}
*/
	mensajes->size = malloc(sizeof(uint32_t));
	(*mensajes->size) = (cantidadMensajes);

	printList(listaCordenadas);

	int socket = send_messages_and_return_socket(ipBroker, puertoBroker, mensajes->mensajes, cantidadMensajes);

	if (socket != -1){
		liberar_conexion(socket);
	}

	//free(mensajes[3]);

	return 0;
}

int printList(List * lista){

	IteratorList iterator = beginlist(*lista);

	while (iterator != NULL){

		Coordenadas* coordenadas = iterator->data;

		printf("%s-%s\n", coordenadas->posx, coordenadas->posy);

		iterator = nextlist(iterator);
	}

	return 0;

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


