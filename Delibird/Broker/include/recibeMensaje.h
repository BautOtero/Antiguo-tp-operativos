#include <commons/config.h>
#include <delibirdcore/sistemaDeMensajes.h>
#include <delibirdcore/list.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../include/memoriaCache.h"


typedef struct{
	int socket;
	int id;
}parametros_envio;


typedef struct{
	List* lista;
	char* colaMensaje;
	int socketRespuesta;
	char* modulo_suscriptor;
}parametros_respuesta_suscripcion;


sem_t* m_idAsignado;
sem_t* sem_listas_admin;
sem_t* sem_respuestas_suscripcion;
sem_t* sem_mensaje_recibido;
sem_t* espero_mensaje_cola;

//void enviar_mensajes_suscriptos(List* lista_suscripcion);

void enviarMensajeNewPokemon(int socketRespuesta,int id);


char* mensajes;
int recibeMensaje(void);
void handle_client(t_result* resultado);
void handle_client_suscriptor(t_result* mensaje);
int agregar_suscriptor_cola(t_result* mensaje);
int asignarID();
void respuestaID(int idMensaje,int);
char *itoa__(int value);
void enviarID(char*,int);
void obtenerMensajeAlmacenado();
void limpiarListaMensajes(List);
void handle_client_publisher(t_result* resultado);
void handle_client_ack(t_result* resultado);
void enviarMensajesGuardadosCola(parametros_respuesta_suscripcion* parametros);
void actualizar_enviados(List* lista,int id_mensaje);
void setear_modulo(Administrativa* administrativa,int seteo, char* id_modulo);

void ver_seteo(Administrativa* admin);

int esta_seteado(Administrativa* administrativa, char* id_modulo);
char* obtener_id_correlativo(t_result* resultado);
int esta_en_memoria(List* memoria);
void sumar_uno_lru_lista();
//void agregar_lista(Administrativa* administrativa);

int modulo_esta(List* lista_mensajes,int id_suscriptor);

int socket_levantado(int socket_fd);

//void enviar_mensajes_suscriptos(List* lista,char* colaMensaje,int socketRespuesta);
void enviar_mensajes_suscriptos(parametros_respuesta_suscripcion* parametros);
int enviar_mensajes_posta(int cantidadDeMensajes, char* mensajes[], int socket_cliente);
List* lista_respuesta(char* colaMensaje);




