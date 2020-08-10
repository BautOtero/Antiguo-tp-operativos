#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <commons/log.h>
#include <commons/string.h>
#include <delibirdcore/configuracion.h>
#include <delibirdcore/list.h>
#include "checkParametros.h"
#include <delibirdcore/sistemaDeMensajes.h>
#include <semaphore.h>

typedef struct {
	char* posx;
	char* posy;
} Coordenadas;

extern int enviarMensaje(char*, char*[]);

extern int procesosBroker(char *[], char*, char*);

extern int procesosTeam(char *[], char*, char*);

extern int procesosGameCard(char *[], char*, char*);

int procesosSuscriptorSinReintento(char *argv[], char *ip, char *puerto);

void handle_client_suscripcion(t_result* mensaje);

void procesosAck(t_result* mensaje);

void suscribirseDesacoplado(char* cola);

void suscribirse(char* cola);




