#include <commons/config.h>
#include <delibirdcore/sistemaDeMensajes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "newPokemon.h"
#include "catchPokemon.h"
#include "getPokemon.h"
#include "tallgrass.h"

void handle_client(t_result* resultado);
void handle_client_new_pokemon(t_result* resultado);
void handle_client_catch_pokemon(t_result* resultado);
void handle_client_get_pokemon(t_result* resultado);
