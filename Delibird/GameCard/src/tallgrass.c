#include "../include/tallgrass.h"

int initializeTallGrass(){


	int resultado = checkPuntoDeMontaje();

	if (resultado == -1) {
		return -1;
	}

	resultado = checkMetadata();

	if (resultado == -1) {
		return -1;
	}

	resultado = checkFiles();

	if (resultado == -1) {
		return -1;
	}

	resultado = checkBlocks();

	if (resultado == -1) {
		return -1;
	}

	return 0;
}


char * checkPokemon(char * pokemon){
	return crearOCheckearPokemon(pokemon, 0);
}

char* crearOCheckearPokemon(char * pokemon, int crear){

	__MODE_T_TYPE permisos = 0700;
	char * pokemondir = malloc(100 * sizeof(char));

	memcpy(pokemondir, filesdir, strlen(filesdir)+1);

	strcat(pokemondir, "/");

	strcat(pokemondir, pokemon);
	DIR* dir = opendir(pokemondir);
	if (dir) {
	    closedir(dir);
	} else if (ENOENT == errno) {
		if (crear == 1){
			mkdir(pokemondir, permisos);
		} else {
			return NULL;
		}

	} else {
	    printf("Fallo el checkeo del directorio \n");
	    return NULL;
	}

	strcat(pokemondir, "/Metadata.bin");

	FILE *fp = fopen(pokemondir, "ab+");

	if(fp != NULL){
		fclose(fp);
	}

	return pokemondir;

}

char* crearPokemonSiNoExiste(char * pokemon){

	return crearOCheckearPokemon(pokemon, 1);

}

int checkPuntoDeMontaje(){

	__MODE_T_TYPE permisos = 0700;
	mnt = config_get_string_value(configuracion_get(), "PUNTO_MONTAJE_TALLGRASS");

	struct stat st = {0};

	if (stat(mnt, &st) == -1) {
		int respuesta = mkdir(mnt, permisos);
		if (respuesta == -1){
			printf("El directorio ingresado no existe y no puede ser creado \n");
			return -1;
		}
	}

	return 0;
}

int checkMetadata(){

	__MODE_T_TYPE permisos = 0700;
	struct stat st = {0};

	memcpy(&metadatadir, mnt, strlen(mnt)+1);

	strcat(metadatadir, "/Metadata\0");

	if (stat(metadatadir, &st) == -1) {
		mkdir(metadatadir, permisos);
	}

	char metadatabin[100];

	memcpy(&metadatabin, metadatadir, strlen(metadatadir)+1);

	strcat(metadatabin, "/Metadata.bin\0");

	FILE *fmetadatabin = fopen(metadatabin,"wb");

   if (fmetadatabin == NULL){
	   printf("Error abriendo archivo binario");

	   // Program exits if the file pointer returns NULL.
	   exit(1);
   }

   char * metadata = "BLOCK_SIZE=64\nBLOCKS=5192\nMAGIC_NUMBER=TALL_GRASS";
   tamanoDeBloque = 64;

   int bytesEscritos = fwrite(metadata, strlen(metadata) * sizeof(char), 1, fmetadatabin);

   if (bytesEscritos == 0) {
	   printf("Error escribiendo archivo binario");
   }

   fflush(fmetadatabin);

   fclose(fmetadatabin);

	char bitarraydir[100];

	memcpy(&bitarraydir, metadatadir, strlen(metadatadir)+1);

	strcat(bitarraydir, "/Bitmap.bin\0");

	int fd = open(bitarraydir, O_RDWR, S_IRGRP | S_IWGRP);

	void* bitarray = NULL;

	struct stat sb;
	if (fstat(fd, &sb) == -1) {
		FILE* file = fopen(bitarraydir, "wb");

		int tamanio = (5192/8);

		bitarray = malloc(tamanio);

		bitarraypointer = bitarray_create(bitarray, (5192/8));

		for (int i = 0; i < 5192; i++){
			bitarray_clean_bit(bitarraypointer, i);
		}

		fwrite(bitarray, tamanio, 1, file);
		fclose(file);
		free(bitarray);

		fd = open(bitarraydir, O_RDWR, S_IRGRP | S_IWGRP);
		if (fstat(fd, &sb) == -1) {
			printf("Sigo sin leer el archivo %ld \n", sb.st_size);
		}
	}

	bitarray = mmap(NULL, sb.st_size, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_SHARED, fd, 0);

	bitarraypointer = bitarray_create(bitarray, (5192/8));

	return 0;

}

int checkFiles(){

	__MODE_T_TYPE permisos = 0700;
	struct stat st = {0};

	memcpy(&filesdir, mnt, strlen(mnt)+1);

	strcat(filesdir, "/Files\0");

	if (stat(filesdir, &st) == -1) {
		mkdir(filesdir, permisos);
	}

	char metadatabin[100];

	memcpy(&metadatabin, filesdir, strlen(filesdir)+1);

	strcat(metadatabin, "/Metadata.bin\0");


	FILE * fmetadatafiles;

	if ((fmetadatafiles = fopen(metadatabin,"wb")) == NULL){
	   printf("Error abriendo archivo binario");

	   // Program exits if the file pointer returns NULL.
	   exit(1);
   }

   char* directoryFile = "DIRECTORY=Y";

   int bytesEscritos = fwrite(&directoryFile, strlen(directoryFile) * sizeof(char), 1, fmetadatafiles);

   if (bytesEscritos == 0) {
	   printf("Error escribiendo archivo binario");
   }

	fclose(fmetadatafiles);


	return 0;
}

int checkBlocks(){

	__MODE_T_TYPE permisos = 0700;
	struct stat st = {0};

	memcpy(&blocksdir, mnt, strlen(mnt)+1);

	strcat(blocksdir, "/Blocks\0");

	if (stat(blocksdir, &st) == -1) {

		mkdir(blocksdir, permisos);

	}

	return 0;
}

void guardarMetadataArchivo(FILE * f, Metadataarchivo archivo){

	char normalizado[1024];
	char buffer[1024];

	sprintf(normalizado, "DIRECTORY=%c\nSIZE=%ld\nBLOCKSLEN=%d\nBLOCKS=[", archivo.directory, archivo.size, archivo.blockslen);

	if (archivo.blocks != NULL){
		for (int i = 0; i < archivo.blockslen; i++){
			if (i != 0){
				strcat(normalizado,",");
			}
			sprintf(buffer,"%d", archivo.blocks[i]);
			strcat(normalizado,buffer);
		}
	}
	strcat(normalizado,"]\nOPEN=");
	sprintf(buffer, "%c", archivo.open);
	strcat(normalizado,buffer);

	fwrite(normalizado, strlen(normalizado) * sizeof(char), 1, f);
}

void RemoveChars(char *s, char c)
{
    int writer = 0, reader = 0;

    while (s[reader])
    {
        if (s[reader]!=c)
        {
            s[writer++] = s[reader];
        }

        reader++;
    }

    s[writer]=0;
}

Metadataarchivo leerMetadataArchivo(FILE * f){


	Metadataarchivo archivo;
	archivo.blocks = NULL;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	char* bufferBlocks;

	while ((read = getline(&line, &len, f)) != -1) {

		char**saveptr = malloc(sizeof(char*));

		char * key = strtok_r(line, "=", saveptr);


		char * value = strtok_r(NULL, "=", saveptr);

		free(saveptr);

		if (strcmp(key, "DIRECTORY") == 0){
			archivo.directory = *value;
		} else if (strcmp(key, "SIZE") == 0){
			char * ptr;
			archivo.size = strtol(value, &ptr, 10);
		} else if (strcmp(key, "OPEN") == 0){
			archivo.open = *value;
		} else if (strcmp(key, "BLOCKSLEN") == 0){
			archivo.blockslen = atoi(value);
		} else if (strcmp(key, "BLOCKS") == 0){
			bufferBlocks = calloc(strlen(value)+1, sizeof(char));
			strcpy(bufferBlocks, value);
		}
	}

	if (archivo.blockslen != 0){
		RemoveChars(bufferBlocks, '[');
		RemoveChars(bufferBlocks, ']');
		char ** saveptr = malloc(sizeof(char*));
		char * bloque = strtok_r(bufferBlocks, ",", saveptr);
		archivo.blocks = malloc(sizeof(int));
		archivo.blocks[0] = atoi(bloque);

		for (int i = 1; i < archivo.blockslen; i++){
			bloque = strtok_r(NULL, ",", saveptr);
			archivo.blocks = realloc(archivo.blocks, ((i+1) * sizeof(int)));
			archivo.blocks[i] = atoi(bloque);
		}

		free (saveptr);

	} else {
		archivo.blocks = NULL;
	}

	return archivo;

}

Metadataarchivo abrirArchivoTallgrass(char * pathPokemon){

	Metadataarchivo archivo;

	FILE* f = fopen(pathPokemon, "rb+");

	if (f == NULL){
		esperarReintento();
		archivo = abrirArchivoTallgrass(pathPokemon);
		return archivo;
	}

	int respuesta = flock(fileno(f), LOCK_EX);

	if (respuesta != 0){
		esperarReintento();
		archivo = abrirArchivoTallgrass(pathPokemon);
		return archivo;
	}

	fseek(f, 0, SEEK_END); // goto end of file
	long tamanoarchivo = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (tamanoarchivo == 0)
	{
		archivo.directory = 'N';
		archivo.open = 'Y';
		archivo.size = 0;
		archivo.blockslen = 0;
		archivo.blocks = NULL;

		guardarMetadataArchivo(f, archivo);
		//file empty
	} else {

		archivo = leerMetadataArchivo(f);

		if (archivo.open == 'Y'){
			flock(fileno(f), LOCK_UN);
			esperarReintento();
			archivo = abrirArchivoTallgrass(pathPokemon);
			return archivo;
		}

		archivo.open = 'Y';

		f = freopen(pathPokemon, "w", f);

		guardarMetadataArchivo(f, archivo);
	}

	fflush(f);

	flock(fileno(f), LOCK_UN);

	fclose(f);

	return archivo;


}

void esperarReintento(){
	int tiempoReintento = config_get_int_value(configuracion_get(), "TIEMPO_DE_REINTENTO_OPERACION" );
	sleep(tiempoReintento);
}


int guardarTextoEnBloque(char* texto, int bloque){

	char dir[100];

	sprintf(dir, "%s/%d.bin", blocksdir, bloque);

	FILE* f = fopen(dir, "wb");

	fwrite(texto, (strlen(texto)+1) * sizeof(char), 1, f);

	fflush(f);

	int status = fclose(f);

	return status;
}

char * leerTextoDeBloque(int bloque){
	char dir[100];

	sprintf(dir, "%s/%d.bin", blocksdir, bloque);

	FILE* f = fopen(dir, "rb");

	char * buffer = NULL;

	long bufsize;

	if (f != NULL) {
		/* Go to the end of the file. */
		if (fseek(f, 0L, SEEK_END) == 0) {
			/* Get the size of the file. */

			bufsize = ftell(f);

			if (bufsize == -1) { /* Error */ }

			/* Allocate our buffer to that size. */
			buffer = malloc(sizeof(char) * (bufsize + 1));

			/* Go back to the start of the file. */
			if (fseek(f, 0L, SEEK_SET) != 0) { /* Error */ }

			/* Read the entire file into memory. */
			size_t newLen = fread(buffer, sizeof(char), bufsize, f);
			if ( ferror( f ) != 0 ) {
				fputs("Error reading file", stderr);
			} else {
				buffer[newLen++] = '\0'; /* Just to be safe. */
			}

		}

		fclose(f);

	}

	return buffer;
}

int guardarTextoEnBloques(char* texto, int* bloques, int cantidadDeBloques){

	for (int i = 0; i < cantidadDeBloques; i++){

		if (i != 0){
			texto = texto + tamanoDeBloque -1;
		}

		char parte[tamanoDeBloque];

		strncpy(parte, texto, tamanoDeBloque - 1);
		parte[tamanoDeBloque - 1] = '\0';

		printf("%s\n", parte);

		guardarTextoEnBloque(parte, bloques[i]);

	}

	return 0;
}

char * leerTextoDeBloques(int* bloques, int cantidadDeBloques){

	char * leido = NULL;

	long tamanioLeido = 0;

	for (int i = 0; i < cantidadDeBloques; i++){

		char* parte = leerTextoDeBloque(bloques[i]);

		if (parte == NULL){
			return NULL;
		}

		tamanioLeido += strlen(parte);

		if (leido == NULL){
			leido = malloc(sizeof(char) * (tamanioLeido + 1));
			memcpy(leido, parte, strlen(parte) + 1);
			strcat(leido, "\0");
		} else {
			leido = realloc(leido, sizeof(char) * (strlen(leido) + tamanioLeido + 1));
			strcat(leido, parte);
			strcat(leido, "\0");
		}



	}

	return leido;
}


void cerrarArchivoTallgrass(Metadataarchivo archivo, char * path){
	archivo.open = 'N';

	FILE * f = fopen(path, "w");

	if (f == NULL){
		esperarReintento();
		cerrarArchivoTallgrass(archivo, path);
		return;
	}

	int respuesta = flock(fileno(f), LOCK_EX);

	if (respuesta != 0){
		fclose(f);
		esperarReintento();
		cerrarArchivoTallgrass(archivo, path);
		return;
	}

	guardarMetadataArchivo(f, archivo);

	fflush(f);

	flock(fileno(f), LOCK_UN);

	fclose(f);

}


void guardarArchivo(Metadataarchivo *contenidoArchivo, char* aguardar){

	int tamaniotexto = 0;
	int bloquesNecesarios = 0;

	if (aguardar != NULL){
		tamaniotexto = strlen(aguardar);
		bloquesNecesarios = tamaniotexto / tamanoDeBloque;
		if ((strlen(aguardar) % tamanoDeBloque) > 0){
			bloquesNecesarios++;
		}
	}

	if (bloquesNecesarios < contenidoArchivo->blockslen){
		desasignarBloques(contenidoArchivo, contenidoArchivo->blockslen - bloquesNecesarios);
	}

	if (bloquesNecesarios > contenidoArchivo->blockslen){
		asignarNuevosBloques(contenidoArchivo, bloquesNecesarios - contenidoArchivo->blockslen);
	}

	guardarTextoEnBloques(aguardar, contenidoArchivo->blocks, contenidoArchivo->blockslen);

}

void asignarNuevosBloques(Metadataarchivo* contenidoArchivo, int bloquesNuevos ){
	int j = 0;
	for (int i = 0; i < bloquesNuevos; i++){
		for (; j < bitarraypointer->size; j++){
			if (!bitarray_test_bit(bitarraypointer, j+1)){
				bitarray_set_bit(bitarraypointer, j+1);
				contenidoArchivo->blockslen++;
				contenidoArchivo->blocks = realloc(contenidoArchivo->blocks, contenidoArchivo->blockslen * sizeof(int));
				contenidoArchivo->blocks[contenidoArchivo->blockslen - 1] = j;
				break;
			}
		}
	}
	msync(bitarraypointer, bitarraypointer->size, MS_SYNC);
}

void desasignarBloques(Metadataarchivo* contenidoArchivo, int bloquesLiberados ){
	int indiceDelUltimo = contenidoArchivo->blockslen - 1;
	for (int i = 0; i < bloquesLiberados; i++){
		bitarray_clean_bit(bitarraypointer, contenidoArchivo->blocks[indiceDelUltimo - i]);
		contenidoArchivo->blockslen--;
		contenidoArchivo->blocks = realloc(contenidoArchivo->blocks, contenidoArchivo->blockslen * sizeof(int));

		break;
	}
	msync(bitarraypointer, bitarraypointer->size, MS_SYNC);
}


int checkline(char* line, Coordenadas coordenadas){
	char ** saveptr = malloc(sizeof(char*));
	char * str = strtok_r(line, "-", saveptr);

	if (strcmp(str, coordenadas.posx) != 0){
		return 0;
	}

	str = strtok_r(NULL, "-", saveptr);

	char* second = strtok_r(str, "=", saveptr);

	if (strcmp(str, coordenadas.posy) != 0){
		return 0;
	}

	second = strtok_r(NULL, "=", saveptr);

	return atoi(second);
}
