#include "semaphores.h"
#include <semaphore.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <delibirdcore/sistemaDeMensajes.h>
#include "planificacionEntrenadores.h"
#include "../include/appearedPokemonHandler.h"
#include "../include/deadlock.h"

void caughtPokemon(int , int );
sem_t* m_caughtPokemon;
int enviarGetPokemon(char* pokemon);
