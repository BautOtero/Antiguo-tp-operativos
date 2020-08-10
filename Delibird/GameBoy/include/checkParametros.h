#include <stdlib.h>
#include <stdio.h>

extern int checkParametroTipoMensaje(char *parametros[]);

extern int checkParametrosMensajeNewPokemon(char *parametros[], int checkID);

extern int checkParametrosMensajeAppearedPokemon(char *parametros[], int checkID);

extern int checkParametrosMensajeCatchPokemon(char *parametros[], int checkID);

extern int checkParametrosMensajeCaughtPokemon(char *parametros[]);

extern int checkParametrosMensajeGetPokemon(char *parametros[], int checkID);



int checkParametrosAck(char *parametros[], int checkID);
