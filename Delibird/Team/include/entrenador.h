#include "semaphores.h"
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/config.h>
#include  <delibirdcore/list.h>
#include <delibirdcore/sistemaDeMensajes.h>
#include  <delibirdcore/configuracion.h>
#include <commons/collections/queue.h>
#include "planificacionEntrenadores.h"
#include "deadlock.h"

int getID(int);
int enviarCatchPokemon();
int cumpliMisObjetivos(t_entrenador* entrenador);

void entrenadorMain();
void meAcercoDeAUno(t_entrenador*, int , int );
void catchPokemon(t_entrenador* entrenador);
void atraparPokemon(t_entrenador* entrenador, char* pokemonNombre);
void buscarIntercambio(t_entrenador* entrenador);
int estoyEnLaMismaPosicion(t_entrenador* localEnt, int cordX, int cordY);
sem_t* sem_buscandoIntercambio;
void agregoPokemonAtrapado(t_entrenador* entrenador, char* pokemon);
void sacoPokemonAtrapado(t_entrenador* entrenador, char* pokemon);
void intercambio(t_entrenador* entrenador);
void postIntercambio(t_entrenador* entrenador);
void trueque(t_entrenador* entrenador);
