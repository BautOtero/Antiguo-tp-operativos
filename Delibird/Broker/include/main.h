#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <delibirdcore/sistemaDeMensajes.h>
#include <signal.h>
#include <unistd.h>
#include <delibirdcore/list.h>
#include <commons/config.h>
#include <pthread.h>
#include <semaphore.h>
#include "../include/recibeMensaje.h"
#include <sys/msg.h>
#include <errno.h>

void agregar_dump_cache_particiones();

void agregar_dump_cache_bs();


int sumar_lista(t_list* lista);
