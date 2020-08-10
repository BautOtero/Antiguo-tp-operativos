#include <commons/collections/list.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <delibirdcore/sistemaDeMensajes.h>
#include <time.h>
#include "../include/particionesDinamicas.h"


typedef struct mainBuddy{
	int mem;
	int lvl;
	bool busy;
	void* msg;
	int *contador;
	Administrativa* estructuraAdministrativa;
	struct mainBuddy* left;
	struct mainBuddy* right;

} t_list_buddySys;


t_list *lista_fifo_sysBuddy;


t_list_buddySys *buddySystem;

t_list_buddySys *create_buddy(int, int, void*);

void actualizar_busy_sysBuddy(t_list_buddySys*);

void div_buddy(t_list_buddySys*);

void div_buddy_hasta(t_list_buddySys*, int);

void liberar_buddy(t_list_buddySys*);

void liberar_hijos_y_buddy(t_list_buddySys*);

void desocupar_buddy(t_list_buddySys*, Administrativa*);

void liberar_hijos_buddy(t_list_buddySys*);

void* ocupar_buddy(t_list_buddySys*, void*, Administrativa*);

t_list_buddySys *buscar_libre_nivel(t_list_buddySys*, int, int);

void list_buddy_lvl(t_list_buddySys *, t_list *, int);

int lvl_por_memoria(int);

void* ejecutar_fifo(t_list_buddySys*, void*, Administrativa*);

void* asignar_memoria_fifo(t_list_buddySys*, void*, Administrativa*);

void* ejecutar_lru(t_list_buddySys*, void*, Administrativa*);

void* asignar_memoria_lru(t_list_buddySys*, void*, Administrativa*);

void* almacenarMensajeBuddySystem(void*, Administrativa*);

void consolidar_sysBuddy(t_list_buddySys*);

void* iniciarBuddySystem(Administrativa*, void*);

void verTablaBuddySystem();


