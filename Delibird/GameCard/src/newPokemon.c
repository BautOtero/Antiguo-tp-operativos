#include "../include/newPokemon.h"

int new_pokemon(char* idmensaje, char * pokemon, Coordenadas cordenadas, char* cantidad) {

	//Verificar si el Pokémon existe dentro de nuestro Filesystem. Para esto se deberá buscar dentro del directorio Pokemon si existe el archivo con el nombre de nuestro pokémon. En caso de no existir se deberá crear.

	char* pathPokemon = crearPokemonSiNoExiste(pokemon);

	//Verificar si se puede abrir el archivo (si no hay otro proceso que lo esté abriendo). En caso que el archivo se encuentre abierto se deberá reintentar la operación luego de un tiempo definido en el archivo de configuración.

	Metadataarchivo contenidoArchivo = abrirArchivoTallgrass(pathPokemon);

	//Verificar si las posiciones ya existen dentro del archivo. En caso de existir, se deben agregar la cantidad pasada por parámetro a la actual. En caso de no existir se debe agregar al final del archivo una nueva línea indicando la cantidad de pokémon pasadas.

	agregarPokemones(&contenidoArchivo, cordenadas, cantidad);

	//Esperar la cantidad de segundos definidos por archivo de configuración
	sleep(config_get_int_value(configuracion_get(), "TIEMPO_RETARDO_OPERACION"));

	//Cerrar el archivo.
	cerrarArchivoTallgrass(contenidoArchivo, pathPokemon);

	//Conectarse al Broker y enviar el mensaje a la Cola de Mensajes APPEARED_POKEMON con los los datos:
	enviarAppearedPokemon(idmensaje, pokemon,  cordenadas);


	return 0;
}

int enviarAppearedPokemon(char* idmensaje, char * pokemon, Coordenadas coordenadas){

	char* ipBroker = config_get_string_value(configuracion_get(), "IP_BROKER");
	char* puertoBroker = config_get_string_value(configuracion_get(), "PUERTO_BROKER");

	char* mensajes[6] = {"PUBLISHER", "APPEARED_POKEMON", pokemon, coordenadas.posx, coordenadas.posy, idmensaje};

	int socket = send_messages_and_return_socket(ipBroker, puertoBroker, mensajes, 6);

	if (socket != -1){
		liberar_conexion(socket);
	}

	return 0;
}

void agregarPokemones(Metadataarchivo* contenidoArchivo, Coordenadas coordenadas, char* cantidad ){

	char * texto = leerTextoDeBloques(contenidoArchivo->blocks, contenidoArchivo->blockslen);

	char * textoAGuardar = NULL;

	int actualizado = 0;

	if (texto != NULL){
		char ** saveptr = malloc(sizeof(char*));
		char * first = strtok_r(texto, "\n", saveptr);

		while ( first != NULL ) {

			char * line = malloc(sizeof(char) * (strlen(first)+1));
			memcpy(line, first, strlen(first) + 1);
			strcat(line, "\0");

			int cantidadLine = checkline(first, coordenadas);
			if (cantidadLine > 0){
				char updatedLine[200];
				cantidadLine += atoi(cantidad);
				sprintf(updatedLine, "%s-%s=%d", coordenadas.posx, coordenadas.posy, cantidadLine);

				line = realloc(line, sizeof(char) * (strlen(updatedLine) + 1));
				memcpy(line, updatedLine, strlen(updatedLine) + 1);

				actualizado = 1;

			}

			if (textoAGuardar == NULL){
				textoAGuardar = malloc((strlen(line) + 2) * sizeof(char));
				memcpy(textoAGuardar, line, strlen(line) + 1);
				strcat(textoAGuardar, "\0");
			} else {
				textoAGuardar = realloc(textoAGuardar, (strlen(textoAGuardar) +  strlen(line) + 2) * sizeof(char));
				strcat(textoAGuardar, "\n");
				strcat(textoAGuardar, line);
				strcat(textoAGuardar, "\0");
			}

			first = strtok_r(NULL, "\n", saveptr);

			free(line);


		}

		free(saveptr);
	}

	if (actualizado == 0){
		char nuevaLinea[200];
		sprintf(nuevaLinea, "%s-%s=%s\n", coordenadas.posx, coordenadas.posy, cantidad);
		if (textoAGuardar == NULL){
			textoAGuardar = malloc((strlen(nuevaLinea) + 1) * sizeof(char));
			memcpy(textoAGuardar, nuevaLinea, strlen(nuevaLinea) + 1);
			strcat(textoAGuardar, "\0");
		} else {
			int memoria = (strlen(textoAGuardar) +  strlen(nuevaLinea) + 2) * sizeof(char);
			textoAGuardar = realloc(textoAGuardar, memoria);
			strcat(textoAGuardar, "\n");
			strcat(textoAGuardar, nuevaLinea);
			strcat(textoAGuardar, "\0");
		}
	}

	guardarArchivo(contenidoArchivo, textoAGuardar);

	free(textoAGuardar);



}
