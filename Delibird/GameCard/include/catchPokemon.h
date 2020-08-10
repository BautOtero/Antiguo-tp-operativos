#include "coordenadas.h"
#include "tallgrass.h"
#include <delibirdcore/sistemaDeMensajes.h>

int catch_pokemon(char* idmensaje, char * pokemon, Coordenadas cordenadas);
int enviarCaughtPokemon(char* idmensaje, int resultado);
int catchPokemon(Metadataarchivo* contenidoArchivo, Coordenadas coordenadas);
