#include "semaphores.h"
#include <semaphore.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/string.h>
#include <delibirdcore/sistemaDeMensajes.h>
#include  <delibirdcore/list.h>
#include  <delibirdcore/configuracion.h>
#include "planificacionEntrenadores.h"

int appearedPokemon(char*, int , int );
int requierePokemon(char*);
int agregarPokemon(char*, int , int );
int cantidadAtrapados(char*);
int cantidadObjetivos(char*);
t_entrenador* buscarEntrenadorMasCercano(t_pokemon* pok);
void buscarEntrenadorYPasarAReady();
int distancia(t_pokemon pok, t_entrenador entrenador);
List pokemonesAAtrapar;
sem_t* pokemonesAAtraparContador;
List pokemonesRecibidos;
List pokemonesObjetivosPendientes;
sem_t* m_listaObjetivos;
void sacarDeListaPorNombre(List* lista, char* nombre);
