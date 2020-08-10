#include <delibirdcore/list.h>
#include <delibirdcore/sistemaDeMensajes.h>
#include <commons/string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	void* data;
	int tam;
	//int* lru;
} MensajeGeneral;

typedef struct {
	char* nombrePokemon;
	uint32_t mensajes[4];
	int tam;
} MensajeNewPokemon;


typedef struct {
	char * nombrePokemon;
	uint32_t mensajes[2];
	int tam;
} MensajeCatchPokemon;


typedef struct {
	char *nombrePokemon;
	uint32_t mensajes[1];
	int tam;
} MensajeGetPokemon;

typedef struct {
	char * nombrePokemon;
	uint32_t mensajes[2];
	int tam;
} MensajeAppearedPokemon;

typedef struct {
	uint32_t mensaje[1];
	int tam;
} MensajeCaughtPokemon;

typedef struct {
	char * nombrePokemon;
	uint32_t cantidadCoordenadas;
	uint32_t* coordenadas;
	int tam;
} MensajeLocalizedPokemon;


typedef struct{
	int socket_suscriptor;
	char* cola_suscripcion;
	char* id_suscripcion;

}Suscriptor;

int asignarID();
char *itoa__(int value);


//CREO LOS POSIBLES MENSAJES QUE PUEDEN ENTRAR EN LAS PARTICIONES
void* crearMensajeNewPokemon(int tam, char* nombre,uint32_t mensajes[]);
void* crearMensajeGetPokemon(int tam,char* nombrePokemon,uint32_t mensajes[]);
void* crearMensajeCatchPokemon(int tam,char* nombrePokemon, uint32_t mensajes[]);
void* crearMensajeCaughtPokemon(int tam, uint32_t mensajes[]);
void* crearMensajeAppearedPokemon(int tam,char* nombrePokemon, uint32_t mensajes[]);
void* crearMensajeLocalizedPokemon(char* mensaje, int cantidad_coordenadas,int tamanio,uint32_t mensajes[]);

int calcularTamanioNewPokemon(t_result* mensaje);
int calcularTamanioCatchPokemon(t_result* mensaje);
int calcularTamanioGetPokemon(t_result* mensaje);
int calcularTamanioAppearedPokemon(t_result* mensaje);
int calcularTamanioLocalizedPokemon(t_result* mensaje);


char *itoa__(int value);

MensajeGeneral* creacionMensajePosta(t_result*);
t_mensajes* desnormalizarMensaje(char* tipoDeMensaje, void* mensaje,int id_mensaje,char* id_mensaje_correlativo);









