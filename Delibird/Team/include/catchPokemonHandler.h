#include "semaphores.h"
#include <semaphore.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <delibirdcore/sistemaDeMensajes.h>
#include "planificacionEntrenadores.h"

void catchPokemon(t_entrenador* entrenador);
int enviarCatchPokemon(t_pokemon* pokemon);
void atraparPokemon(t_entrenador* entrenador, char* pokemonNombre);
