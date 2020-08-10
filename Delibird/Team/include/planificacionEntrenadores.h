#ifndef PLANIFICACIONENTRENADORES_H_
#define PLANIFICACIONENTRENADORES_H_


#include <semaphore.h>
#include "semaphores.h"
#include <commons/config.h>
#include <commons/string.h>
#include <delibirdcore/sistemaDeMensajes.h>
#include <delibirdcore/list.h>
#include <delibirdcore/configuracion.h>

sem_t** sem_avanzar;
sem_t* sem_ciclo;
sem_t** sem_caughtPokemonBroker;
sem_t* sem_entrenador_ciclo_ready;
//sem_t* sem_pokemonesLibres;
sem_t* sem_crearEntrenador;
List listaentrenadores;

sem_t* sem_planificador;
sem_t* sem_entrenadorListo;
sem_t* m_pasarEstado;
sem_t* m_desalojo;
sem_t* m_finProceso;
sem_t* sem_entrenadores_libres;

int cantidadEntrenadores;

enum t_estadoEntrenador
{NEW=1,READY=2,EXEC=3,BLOCKED=4,EXIT=5};

typedef struct
{
	char* nombre;
	int cordX;
	int cordY;
} t_pokemon;

typedef struct t_entrenador_tag
{
	int id;
	int pid;
	char* coordenadas;
	char* pokemones_objetivo;
	char* pokemones_atrapados;
	enum t_estadoEntrenador estadoEntrenador;
	int idMensajeEsperado;
	int esperandoIntercambiar;
	t_pokemon* objetivoActual;
	struct t_entrenador_tag* entrenadorVoyAIntercambiar;
	char* nombrePokemonAIntercambiar;
	double valorPredichoUltimaRafaga;
	double valorUltimaRafaga;
	double valorEstimadoSiguiente;
} t_entrenador;

t_entrenador** entrenadores;


int iniciarPlanificacionEntrenadores();

void planificar();
void planificarFIFO();
void planificarRR();
void planificarSJFSD();
void planificarSJFCD();
t_entrenador* elegirEntrenadorPorSJF();
t_entrenador* predecirEntrenadorPorSJF();
double calcularRafagaSiguienteEstimada(double alfa, double rafagaEstimadaAnterior, double rafagaRealAnterior);

int estaEntreAtrapadosEntrenador(char* , t_entrenador*);

List entrenadoresFifoReady;
void pasarReady(t_entrenador* entrenador);
void pasarBlocked(t_entrenador* entrenador);
void pasarExit(t_entrenador* entrenador);
void finDeProceso();
t_entrenador* desalojo(t_entrenador* entrenadorActual);
int len_array(char**);
t_log* logger;


#endif /* PLANIFICACIONENTRENADORES_H_ */

