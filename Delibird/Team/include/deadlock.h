#include "semaphores.h"
#include <semaphore.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <delibirdcore/sistemaDeMensajes.h>
#include  <delibirdcore/list.h>
#include  <delibirdcore/configuracion.h>
#include "planificacionEntrenadores.h"
#include "../include/appearedPokemonHandler.h"

List candidatosADeadlock(t_entrenador* entrenador);
int hayDeadlock(List listaCandidatos);
t_entrenador * quienTieneMisPokemones(t_entrenador* entrenador, List listaCandidatos);
List misObjetivosSinAtrapar(t_entrenador* entrenador);
int cantidadAtrapadosEntrenador(char* pokemon,t_entrenador* entrenador);
t_entrenador* buscarEntrenadorParaIntercambiar(t_entrenador* entrenador, List candidatos);
List misPokemonesAtrapados(t_entrenador* entrenador);
List misPokemonesObjetivos(t_entrenador* entrenador);
int cantidadObjetivosDeEntrenador(char* pokemon,t_entrenador* entrenador);
int estaEnLaLista(char* objetivo,List listaAtrapados);
char* pokemonAtrapadoNoObjetivo(t_entrenador* entrenador);
