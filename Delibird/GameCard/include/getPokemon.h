#include <delibirdcore/sistemaDeMensajes.h>
#include <delibirdcore/list.h>
#include "coordenadas.h"
#include "tallgrass.h"
#include <commons/string.h>

int enviarLocalizedPokemon(char* idmensaje, char * pokemon, int cantidad, List * listaCordenadas);
int get_pokemon(char* idmensaje, char * pokemon);
void obtenerPosicionesYCantidades(int* cantidad, List* listaCoordenadas, Metadataarchivo* contenidoArchivo);
void agregarPokemon(int* cantidad, List* listaCoordenadas, char * line);
void _list_elements_print(Coordenadas *cord);
int printList(List * lista);




char *itoa__(int value);
