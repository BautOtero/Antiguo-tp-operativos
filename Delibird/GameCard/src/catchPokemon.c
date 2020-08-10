#include "../include/catchPokemon.h"

int catch_pokemon(char* idmensaje, char * pokemon, Coordenadas coordenadas){

	//Verificar si el Pokémon existe dentro de nuestro Filesystem. Para esto se deberá buscar dentro del directorio Pokemon, si existe el archivo con el nombre de nuestro pokémon. En caso de no existir se deberá informar un error.

	char* pathPokemon = crearPokemonSiNoExiste(pokemon);

	//Verificar si se puede abrir el archivo (si no hay otro proceso que lo esté abriendo). En caso que el archivo se encuentre abierto se deberá reintentar la operación luego de un tiempo definido en el archivo de configuración.

	Metadataarchivo contenidoArchivo = abrirArchivoTallgrass(pathPokemon);

	//Verificar si las posiciones ya existen dentro del archivo. En caso de no existir se debe informar un error.

	int resultado = catchPokemon(&contenidoArchivo, coordenadas);

	//En caso que la cantidad del Pokémon sea “1”, se debe eliminar la línea. En caso contrario se debe decrementar la cantidad en uno.

	//Esperar la cantidad de segundos definidos por archivo de configuración
	sleep(config_get_int_value(configuracion_get(), "TIEMPO_RETARDO_OPERACION"));

	//Cerrar el archivo.

	cerrarArchivoTallgrass(contenidoArchivo, pathPokemon);

	//Conectarse al Broker y enviar el mensaje indicando el resultado correcto.
	enviarCaughtPokemon(idmensaje, resultado);

	free(pathPokemon);

	return 0;
}

int enviarCaughtPokemon(char* idmensaje, int resultado){

	char* ipBroker = config_get_string_value(configuracion_get(), "IP_BROKER");
	char* puertoBroker = config_get_string_value(configuracion_get(), "PUERTO_BROKER");

	char * resultadoChar = malloc(2);

	sprintf(resultadoChar, "%d", resultado);

	char* mensajes[4] = {"PUBLISHER", "CAUGHT_POKEMON", resultadoChar, idmensaje};

	int socket = send_messages_and_return_socket(ipBroker, puertoBroker, mensajes, 4);

	if (socket != -1){
		liberar_conexion(socket);
	}

	return 0;
}


int catchPokemon(Metadataarchivo* contenidoArchivo, Coordenadas coordenadas){

	char * texto = leerTextoDeBloques(contenidoArchivo->blocks, contenidoArchivo->blockslen);

	char * textoAGuardar = NULL;

	int resultado = 0;

	if (texto != NULL){
		char ** saveptr = malloc(sizeof(char*));
		char * first = strtok_r(texto, "\n", saveptr);

		while ( first != NULL ) {

			int eliminado = 0;

			char * line = malloc(sizeof(char) * (strlen(first)+1));
			memcpy(line, first, strlen(first) + 1);
			strcat(line, "\0");

			if (resultado == 0){

				int cantidadLine = checkline(first, coordenadas);

				if (cantidadLine > 0){

					cantidadLine -= 1;

					if (cantidadLine != 0){

						char updatedLine[200];
						sprintf(updatedLine, "%s-%s=%d", coordenadas.posx, coordenadas.posy, cantidadLine);

						line = realloc(line, sizeof(char) * (strlen(updatedLine) + 1));
						memcpy(line, updatedLine, strlen(updatedLine) + 1);

					} else {
						eliminado = 1;
					}

					resultado = 1;

				}

			}

			if (eliminado == 0){
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
			}

			first = strtok_r(NULL, "\n", saveptr);

			free(line);

		}

		free(saveptr);
	}

	guardarArchivo(contenidoArchivo, textoAGuardar);

	free(textoAGuardar);

	return resultado;



}
