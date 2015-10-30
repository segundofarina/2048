#include <string.h>
#include <time.h>
#include "2048back.h"

#define LOAD 2
#define END 3
#define QUIT 6
#define SAVE 7

#define MAX_LENGTH_COMANDO 5
#define MAX_LENGTH_FILE_NAME 36

#define SI 1
#define NO 0
#define NOHAYERROR 0



void presentacion();
int menu();
int dificultad();
int pedirJugada(char fileName[]);
void getAccion(char comando[], char fileName[]);
int getsn();
int preguntoSave();
void preguntoFileName(char fileName[]);
void ImprimirTablero(sTablero tablero);
void ImprimirError(int error);



int main(){
	srand(time(NULL));
	int menuOption, resp,accion,jugada,direccion,hiceUndo=1,gane=0,perdi=0,error;
	sTablero tablero1, tablero2, tableroAux;
	sCasVacios casVacios;
	int movimientos[4];
	char fileName[MAX_LENGTH_FILE_NAME];

	presentacion();


	/* Me quedo en el menu hasta opcion de salir */
	do{
		accion=0;/*inicializo accion para evitar errores en segunda pasada*/
		menuOption=menu();

		/* valores iniciales del talblero dependen de la opcion elegida */
		switch(menuOption){
			case LOAD:/*cargo partida guradada*/
				preguntoFileName(fileName);
				error=cargoPartida(&tablero1,&tablero2,&casVacios,movimientos,fileName);
				if(error==ERR_MEMORIA){
					ImprimirError(ERR_MEMORIA);
					return 1;
				}else if(error==ERR_FILE){
					ImprimirError(ERR_FILE);
					return 1;
				}
				else if(error==ERR_FILE_VALID){
					ImprimirError(ERR_FILE_VALID);
					return 1;
				}
				ImprimirTablero(tablero1);//cambiar
			break;
			case END:

			break;
			case FACIL:case INTERMEDIO: case DIFICIL:
				error=inicializo(&tablero1,&tablero2,&casVacios,menuOption,movimientos);
				if(error==ERR_MEMORIA){
					ImprimirError(ERR_MEMORIA);
					return 1;
				}
				ImprimirTablero(tablero1);
			break;
		}

		/* me quedo jugando en la partida */
		while(menuOption!=END && !gane && !perdi && (accion=pedirJugada(fileName))!=QUIT && accion!=SAVE){
			jugada=jugar(&tablero1, &tablero2, &tableroAux,&casVacios ,&hiceUndo,&gane,&perdi,movimientos,accion);
			if(jugada==NOHAYERROR){
			    ImprimirTablero(tablero1);
			}else{
				ImprimirError(jugada);
			}
		}	
		
		/* termine la partida, informo estado en que termine*/
		if(gane){
			printf("\n\n********** Felicitaciones has ganado!!! **********\n");
			printf("**********     Tu puntuacion fue: %d    **********\n\n",tablero1.puntaje);
		}else if (perdi){
	        printf("\n\n********** Lo lamento has perdido! **********\n");
	        printf("**********  Tu puntuacion fue: %d  **********\n\n",tablero1.puntaje);
	    }else if(accion==QUIT){printf("accion es %d\n", accion);
	    	resp = preguntoSave();
	    	if(resp==SI){
	    		/*guardo*/
	    		preguntoFileName(fileName);
	    		resp=guardar(fileName,tablero1);
	    		if(resp==NOHAYERROR){
	    			printf("\n\n** La partida %s se guardo correctamente **\n\n", fileName);
	    		}else{
	    			ImprimirError(ERR_SAVE);
	    		}
	    	}

	    }else if(accion==SAVE){
	    	/*guardo*/
	    	if (fileName[0]==0){
	    		preguntoFileName(fileName);
	    	}
	    	resp=guardar(fileName,tablero1);
			if(resp==0){
				printf("\n\n** La partida %s se guardo correctamente **\n\n", fileName);
			}else{
				ImprimirError(ERR_SAVE);
			}
	    }else if(menuOption==END){
	    	printf("\n\n *************** Gracias por juagar al 2048 ***************\n\n");
	    }else{
	    	printf("\n\n********** Oh Oh, Algo salio mal! **********\n\n");
	    }


	}while(menuOption!=END);



	return 0;


}

/* Cartel de inicio del juego */
void presentacion(){
	printf("\n\n\n");
	printf("***********************************\n");
	printf("*                                 *\n");
	printf("*       Bienvenido al 2048        *\n");
	printf("*                                 *\n");
	printf("***********************************\n\n");
}

/* Presento el menu principal y valido que se ingrese una opcion correcta */
int menu(){
	int resp;
	printf("1-Juego nuevo\n");
	printf("2-Recuperar un juego guardado\n");
	printf("3-Salir\n");
	resp=getint("ELIJA UNA OPCION:");
	while (resp >3 || resp<1){
		printf("\nDato incorrecto\n");
		resp=getint("ELIJA UNA OPCION:");
	}
	if(resp==1){
		resp=dificultad()+3;
	}
	return resp;
}

/* Pido al usuario la dificultad de la partida  y la valido */
int dificultad(){
	int resp;
	printf("\nELIJA UNA DIFICULTAD:\n");
	printf("1-Facil\n");
	printf("2-Intermedio\n");
	printf("3-Dificil\n");
	resp=getint("ELIJA UNA OPCION:");
	while (resp >3 || resp<1){
		printf("\nDato incorrecto\n");
		resp=getint("ELIJA UNA OPCION:");
	}
	return resp;
}

/* Decodifico el comando ingreado por el usuario a las aciones 
** que realiza el juego */
int pedirJugada(char fileName[]){
	char comando[MAX_LENGTH_COMANDO];
	int resp=-1;
	while(resp==-1){
		printf("Ingrese un comando:\n");
		getAccion(comando,fileName);
		if(!strcmp(comando,"a")){
			resp=IZQUIERDA;
		}else if(!strcmp(comando,"w")){
			resp=ARRIBA;
		}else if(!strcmp(comando,"d")){
			resp=DERECHA;
		}else if(!strcmp(comando,"s")){
			resp=ABAJO;
		}else if(!strcmp(comando,"undo")){
			resp=UNDO;
		}else if(!strcmp(comando,"quit")){
			resp=QUIT;
		}else if(!strcmp(comando,"save")){//modificar
			resp=SAVE;
		}else{
			printf("** Comando no valido. **\n");
			resp=-1;
		}
	}
	return resp;
}

/* Le pido al jugador que ingrese un comando y si es save
** guardo el nombre del archivo en un string */
void getAccion(char comando[], char fileName[]){
	char c;
	int i=0,j=0,var=1;
	while((c=getchar())!='\n'){
		if (var==2 && j<MAX_LENGTH_FILE_NAME){
			fileName[j++]=c;
		}
		if (c==' '){
			var=2;
		}
		if (var==1 && i<MAX_LENGTH_COMANDO){
			comando[i++]=c;
		}
	}
	comando[i]=0;
	fileName[j]=0;
}

/* Pregunto si/no */
int getsn(){
	char c,sn=0;
	while((c=getchar())!='\n'){/* me quedo con el primer caracter y borro buffer*/
		if(sn==0){
			sn=c;
		}
	}
	if(sn=='s'){
		return SI;
	}else if(sn=='n'){
		return NO;
	}else{
		//error
		return -1;
	}
}

/* Funcion ejecutada previo a terminar la partida 
** presenta opcion de save al poner el comando "quit" */
int preguntoSave(){
	int resp;
	printf("Desea guradar la partida?(s/n)\n");
	while((resp=getsn())==-1){//valido s o n
		printf("Comando no valido (s/n)\n");
	}
	return resp;
}

/* Pido solo el nombre del archivo y lo almaneno en un string */
void preguntoFileName(char fileName[]){
	char c;
	int i=0;
	printf("Ingrese el nombre del archivo:\n");
	while((c=getchar())!='\n'){
		if (i<MAX_LENGTH_FILE_NAME){
			fileName[i++]=c;
		}
	}

	fileName[i]=0;
}

/* Imprimo tablero con formato acorde a la dimencion del tablero */
void ImprimirTablero(sTablero tablero){
    int i,j;
    printf("PUNTAJE:%d\n",tablero.puntaje);
    printf("UNDOS:%d\n",tablero.undos);
    for(i=0;i<tablero.dim;i++){
    	if(tablero.dim==4){
    		printf("---------------------\n");
    	}else if(tablero.dim==6){
    		printf("-------------------------------\n");
    	}else{
    		printf("-----------------------------------------\n");
    	}
        for(j=0;j<tablero.dim;j++){
            if(tablero.matriz[i][j]!=0){
            	printf("|%4d", tablero.matriz[i][j]);
            }else{
            	printf("|%4c",' ');
            }
        }
        printf("|\n");
    }
    if(tablero.dim==4){
		printf("---------------------\n");
	}else if(tablero.dim==6){
		printf("-------------------------------\n");
	}else{
		printf("-----------------------------------------\n");
	}
    printf("\n");
}

/* Decodifico errores devueltos por el back-end e informo al usuraio del mimso */
void ImprimirError(int error){
	switch(error){
		case ERR_UNDO:
			printf("\n** No puedes realizar undo **\n\n");
		break;
		case ERR_MOV:
			printf("\n** Movimiento invalido **\n\n");
		break;
		case ERR_FORZADO:
			printf("\n** No hay movimientos posibles.Realizar undo **\n\n");
		break;
		case ERR_MEMORIA:
			printf("\n** No dispone de memoria suficiente **\n\n");
		break;
		case ERR_FILE:
			printf("\n** Se ha producido un error al intentar cargar la partida **\n\n");
		break;
		case ERR_SAVE:
			printf("\n\n** Se ha producido un error al intentar guardar la partida **\n\n");
		break;
		case ERR_FILE_VALID:
			printf("\n\n** El archivo cargado no tiene un formato valido **\n\n");
		break;
	}
}