#include <stdlib.h>
#include <delibirdcore/sistemaDeMensajes.h>
#include "coordenadas.h"
#include "tallgrass.h"
#include <string.h>
#include <stdio.h>

int enviarAppearedPokemon(char* idmensaje, char * pokemon, Coordenadas cordenadas);
int new_pokemon(char* idmensaje, char * pokemon, Coordenadas cordenadas, char* cantidad);
void agregarPokemones(Metadataarchivo* contenidoArchivo, Coordenadas cordenadas, char* cantidad );
