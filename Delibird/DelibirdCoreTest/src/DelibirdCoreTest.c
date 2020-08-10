/*
 ============================================================================
 Name        : DelibirdCoreTest.c
 Author      : Walter
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../DelibirdCore/delibirdcore/server.h"
#include "../../DelibirdCore/delibirdcore/client.h"


void handle_client(t_result* resultado);

void *perform_work(void *arguments){
  int arg = *((int *)arguments);
  int sleep_time = 3;
  printf("THREAD: Started.\n");
  printf("THREAD: Will be sleeping for %d seconds.\n", sleep_time);
  sleep(sleep_time);
  printf("THREAD: Time to send message.\n");

  char* mensajes[2];
  mensajes[0] = "Hola";
  mensajes[1] = "Mundo";
  send_messages("127.0.0.1", "5010", mensajes, 2);

}

int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	pthread_t thread;
	int arg = 0;
	int result_code = pthread_create(&thread, NULL, perform_work, &arg );
	iniciar_servidor("127.0.0.1","5010", handle_client);
	return EXIT_SUCCESS;
}

void handle_client(t_result* resultado)
{
	for (int i = 0; i < *resultado->mensajes->size; i++){
		printf("%s ", resultado->mensajes->mensajes[i]);
	}
	printf("\n");
}
