#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <delibirdcore/list.h>
#include <delibirdcore/sistemaDeMensajes.h>
#include <time.h>
//#include "../include/creadorMensajes.h"
#include "../include/estructuraAdministrativa.h"

t_log* 	logger;
typedef struct {
	int tam;
	int direccion;
	int* lru;
	void* mensajeGeneral;
	int id_particion;
	bool ocupado;
	Administrativa* estructura_admin;
} ParticionGeneral;

//DECLARACION CONFIG GLOBAL
char* ip;
char* puerto;
int tamMemoriaMaxima;
int tamMinimoParticion;
char* algoritmoMemoria;
char* algoritmoReemplazo;
char* algoritmoParticion;
int frecuenciaCompactacion;
List fifo_full;

//DECLARACION PUNTERO CACHE
void* punteroAMemoriaPrincipal;

void* almacenarUnMensaje(void* mensaje, Administrativa*);

void* iniciarParticionesDinamicas(Administrativa*, void*);

int algoritmo_fifo(Administrativa*);

int algoritmo_lru(Administrativa*);

ParticionGeneral* mayorLru(ParticionGeneral* particion1, ParticionGeneral* particion2);

void consolidacion_mensajes();
void compactacion_mensajesV2();
int eliminarMensaje(ParticionGeneral* particion);

void* algoritmo_best_fit(void* mensaje,Administrativa*);
void* algoritmo_first_fit(void* mensaje,Administrativa*);
int particionLibre(ParticionGeneral* particion);

ParticionGeneral* creacionParticion(int tam, int direccion, void* mensaje, Administrativa* admin);

void verTablaParticiones();
