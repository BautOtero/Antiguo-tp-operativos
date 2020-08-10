/*
 * main.h
 *
 *  Created on: 12 abr. 2020
 *  Author: grupo z
 */


#include "procesos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <delibirdcore/configuracion.h>
#include <delibirdcore/sistemaDeMensajes.h>

typedef enum tipos_mensajes
{
	NEW_POKEMON, //0

	APPEARED_POKEMON, //1

	CATCH_POKEMON,//2

	CAUGHT_POKEMON,//3

	GET_POKEMON//4
}tipo_mensaje;

