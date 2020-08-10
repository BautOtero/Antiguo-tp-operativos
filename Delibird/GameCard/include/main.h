/*
 * main.h
 *
 *  Created on: 12 abr. 2020
 *  Author: grupo z
 */

#include <commons/config.h>
#include <commons/string.h>
#include <delibirdcore/sistemaDeMensajes.h>
#include "recibeMensaje.h"
#include "tallgrass.h"
#include <delibirdcore/configuracion.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum tipos_mensajes
{
	NEW_POKEMON, //0

	CATCH_POKEMON,//1

	GET_POKEMON,//2

}tipo_mensaje;

void ackHandleclient(t_result* msj);
void suscribirseDesacoplado(char* cola);
void suscribirse(char* cola);
