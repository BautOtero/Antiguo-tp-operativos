#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <commons/config.h>
#include <commons/bitarray.h>
#include <delibirdcore/configuracion.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/file.h>
#include "coordenadas.h"

#ifndef TALLGRASS_HEADER
#define TALLGRASS_HEADER

typedef struct Metadatafs
{
	char* Block_size;

	char* Blocks;

	char* Magic_Number;

}Metadatafs;

typedef struct Metadataarchivo
{
	char directory;

	long size;

	int* blocks;

	char open;

	int blockslen;

} Metadataarchivo;

typedef struct Metadatadir
{
	char directory;

} Metadatadir;

char* mnt;
char metadatadir[100];
char filesdir[100];
char blocksdir[100];
int tamanoDeBloque;
t_bitarray* bitarraypointer;

#endif



int initializeTallGrass();
int checkPuntoDeMontaje();
int checkMetadata();
int checkFiles();
int checkBlocks();
void esperarReintento();
char* crearPokemonSiNoExiste(char * pokemon);
Metadataarchivo abrirArchivoTallgrass(char * pathPokemon);
void guardarMetadataArchivo(FILE * f, Metadataarchivo archivo);
int guardarTextoEnBloque(char* texto, int bloque);
char * leerTextoDeBloque(int bloque);
int guardarTextoEnBloques(char* texto, int* bloques, int cantidadDeBloques);
char * leerTextoDeBloques(int* bloques, int cantidadDeBloques);
void cerrarArchivoTallgrass(Metadataarchivo archivo, char * path);
void guardarArchivo(Metadataarchivo *contenidoArchivo, char* aguardar);
void asignarNuevosBloques(Metadataarchivo* contenidoArchivo, int bloquesNuevos );
char* crearOCheckearPokemon(char * pokemon, int crear);
char * checkPokemon(char * pokemon);
int checkline(char* line, Coordenadas coordenadas);
void desasignarBloques(Metadataarchivo* contenidoArchivo, int bloquesLiberados );


