#include "../include/creadorMensajes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct{
	int id_modulo;
	int estado_mensaje;
}ModulosEstados;

typedef struct{
	int id_mensaje;
	char* cola_mensaje;
	int tam;
	List listaEstados;
	int* lru;
	char* id_correlativo;
	void* mensajes; // Esto apuntaria a los mensajes que tengo en la cache
}Administrativa;

Administrativa* anterior;

Administrativa* crear_estructura_administrativa(t_result* mensaje,int id_mensaje,void*,char* id_correlativo);
ModulosEstados* crearModulo(int modulo,int estado_mensaje);

//DECLARACION LISTAS
List particiones;
List lista_new;
List lista_catch;
List lista_caught;
List lista_localized;
List lista_appeared;
List lista_get;
List lista_suscriptores;

void verEstructuraAdministrativa(List*);
int se_encuentra_modulo(List* estructuras_administrativas,int id_modulo);
void* obtener_mensaje(int pid);
void asignarConfirmacionMensaje(List* estructuras_administrativas, int id_mensaje, int id_modulo);
void asignarEnvioMensaje(List* estructuras_administrativas, int id_mensaje, int id_modulo);
