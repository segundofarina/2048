#include <string.h>
#include "2048back.h"

#define MAX_LENGTH_COMANDO 5
#define MAX_LENGTH_FILE_NAME 36

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
	int resp,accion,jugada,direccion,hiceUndo=1,gane=0,perdi=0,error;
	sTablero tablero1, tablero2, tableroAux;
	sCasVacios casVacios;
	int movimientos[4];
	char fileName[MAX_LENGTH_FILE_NAME];

	presentacion();
	resp=menu();
	
	switch(resp){
		case 2://cargo partida guradada

		break;
		case 3:
			return 0;
		break;
		case 4:case 5: case 6:
			error=inicializo(&tablero1,&tablero2,&casVacios,resp,movimientos);
			if(error==ERR_MEMORIA){
				ImprimirError(ERR_MEMORIA);
				return 1;
			}
			ImprimirTablero(tablero1);
		break;

	}

	while(!gane && !perdi && (accion=pedirJugada(fileName))!=QUIT && accion!=SAVE){
		jugada=jugar(&tablero1, &tablero2, &tableroAux,&casVacios ,&hiceUndo,&gane,&perdi,movimientos,accion);
		if(jugada==0){
		    ImprimirTablero(tablero1);
		}else{
			ImprimirError(jugada);
		}
	}	
	
	if(gane){
		printf("\n\n********** Felicitaciones has ganado!!! **********\n");
		printf("**********     Tu puntuacion fue: %d    **********\n\n",tablero1.puntaje);
	}else if (perdi){
        printf("\n\n********** Lo lamento has perdido! **********\n");
        printf("**********  Tu puntuacion fue: %d  **********\n\n",tablero1.puntaje);
    }else if(accion==QUIT){
    	resp = preguntoSave();
    	if(resp==1){
    		//guardo
    		preguntoFileName(fileName);
    		guardar(fileName,tablero1);
    	}else{
    		return 0;//salgo
    	}

    }else if(accion==SAVE){
    	//guardo
    	guardar(fileName,tablero1);
    }else{
    	printf("\n\n********** Oh Oh, Algo salio mal! **********\n\n");
    	return 1;
    }

	return 0;


}

void presentacion(){
	printf("\n\n\n");
	printf("***********************************\n");
	printf("*                                 *\n");
	printf("*       Bienvenido al 2048        *\n");
	printf("*                                 *\n");
	printf("***********************************\n\n");
}

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

int getsn(){
	char c,sn=0;
	while((c=getchar())!='\n'){
		if(sn==0){
			sn=c;
		}
	}
	if(sn=='s'){
		return 1;
	}else if(sn=='n'){
		return 0;
	}else{
		//error
		return -1;
	}
}

int preguntoSave(){
	int resp;
	printf("Desea guradar la partida?(s/n)\n");
	while((resp=getsn())==-1){//valido s o n
		printf("Comando no valido (s/n)\n");
	}
	return resp;
}

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

void ImprimirError(int error){
	switch(error){
		case ERR_UNDO:
			printf("** No puedes realizar undo **\n");
			break;
		case ERR_MOV:
			printf("** Movimiento invalido **\n");
			break;
		case ERR_FORZADO:
			printf("** No hay movimientos posibles.Realizar undo **\n");
			break;
		case ERR_MEMORIA:
			printf("** No dispone de memoria suficiente **\n");
			break;
		}
}