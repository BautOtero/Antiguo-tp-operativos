#include "../include/checkParametros.h"

int checkParametroTipoMensaje(char *parametros[]){
	if (parametros[2] == NULL){
		printf("El parametro 2 debe ser el tipo de mensaje");
		return -1;
	}
	return 0;
}

int checkParametrosMensajeNewPokemon(char *parametros[], int checkID) {

	if (parametros[3] == NULL){
		printf("El parametro 3 de NEW_POKEMON debe ser el POKEMON nuevo");
		return -1;
	}

	if (parametros[4] == NULL) {
		printf("El parametro 4 de NEW_POKEMON debe la posicion en el eje X del pokemon nuevo");
		return -1;
	}

	if (parametros[5] == NULL) {
		printf("El parametro 5 de NEW_POKEMON debe la posicion en el eje Y del pokemon nuevo");
		return -1;
	}

	if (parametros[6] == NULL) {
		printf("El parametro 6 de NEW_POKEMON debe se la cantidad de pokemones nuevos en esa posicion");
		return -1;
	}

	if (checkID && parametros[6] == NULL) {
		printf("El parametro 7 de NEW_POKEMON debe ser el ID de mensaje relacionado al mensaje");
		return -1;
	}

	return 0;
}

int checkParametrosMensajeAppearedPokemon(char *parametros[], int checkID) {

	if (parametros[3] == NULL){
		printf("El parametro 3 de APPEARED_POKEMON debe ser el POKEMON que aparecio");
		return -1;
	}

	if (parametros[4] == NULL) {
		printf("El parametro 4 de APPEARED_POKEMON debe la posicion en el eje X del pokemon que aparecio");
		return -1;
	}

	if (parametros[5] == NULL) {
		printf("El parametro 5 de APPEARED_POKEMON debe la posicion en el eje Y del pokemon que aparecio");
		return -1;
	}

	return 0;
}

int checkParametrosMensajeCatchPokemon(char *parametros[], int checkID) {

	if (parametros[3] == NULL){
		printf("El parametro 3 de CATCH_POKEMON debe ser el POKEMON a capturar");
		return -1;
	}

	if (parametros[4] == NULL) {
		printf("El parametro 4 de CATCH_POKEMON debe la posicion en el eje X del pokemon a capturar");
		return -1;
	}

	if (parametros[5] == NULL) {
		printf("El parametro 5 de CATCH_POKEMON debe la posicion en el eje Y del pokemon a capturar");
		return -1;
	}

	if (checkID && parametros[6] == NULL) {
		printf("El parametro 6 de CATCH_POKEMON debe ser el ID de mensaje relacionado al mensaje");
		return -1;
	}


	return 0;
}

int checkParametrosMensajeCaughtPokemon(char *parametros[]) {

	if (parametros[3] == NULL){
		printf("El parametro 3 de CAUGHT_POKEMON debe ser el ID del mensaje Caught Pokemon relacionado");
		return -1;
	}

	if (parametros[4] == NULL) {
		printf("El parametro 4 de CAUGHT_POKEMON debe ser el resultado de la captura de pokemon");
		return -1;
	}

	return 0;
}

int checkParametrosMensajeGetPokemon(char *parametros[], int checkID) {

	if (parametros[3] == NULL){
		printf("El parametro 3 de GET_POKEMON debe ser el Pokemon que se necesita");
		return -1;
	}

	if (checkID && parametros[4] == NULL) {
		printf("El parametro 4 de GET_POKEMON debe ser el ID de mensaje relacionado al mensaje");
		return -1;
	}

	return 0;
}

int checkParametrosAck(char *parametros[], int checkID) {

	if (parametros[3] == NULL){
		printf("El parametro 3 de GET_POKEMON debe ser el Pokemon que se necesita");
		return -1;
	}

	if (checkID && parametros[4] == NULL) {
		printf("El parametro 4 de GET_POKEMON debe ser el ID de mensaje relacionado al mensaje");
		return -1;
	}

	return 0;
}
