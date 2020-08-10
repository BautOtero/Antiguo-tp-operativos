/*
 * main.h
 *
 *  Created on: 12 abr. 2020
 *  Author: grupo z
 */

#include "localizedPokemonHandler.h"
#include "appearedPokemonHandler.h"
#include "caughtPokemonHandler.h"
#include <semaphore.h>
#include "semaphores.h"
#include "entrenador.h"
#include "deadlock.h"
#include <delibirdcore/sistemaDeMensajes.h>

void handle_message(t_result* resultado);
void envioMensajesGet();
void ackHandleclient(t_result* msj);
void suscribirseDesacoplado(char* cola);
void suscribirse(char* cola);
