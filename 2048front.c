#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "getnum.h"


#define MAX_LENGTH 41
#define IZQUIERDA 1
#define ARRIBA 2
#define DERECHA 3
#define ABAJO 4
#define SAVE 7
#define QUIT 6
#define UNDO 5
#define FACIL 4
#define INTERMEDIO 5
#define DIFICIL 6
#define ERR_UNDO -100
#define ERR_MOV -99

#include "2048back.c"
/*

typedef struct {
    int ** matriz;
    int dim;
    int undos;
    int puntaje;
} tablero;
typedef struct{
    int ** matriz;
    int num;
} casVacios;
*/
void presentacion(){
	printf("\n\n\n");
	printf("***********************************\n");
	printf("*                                 *\n");
	printf("*       Bienvenidos al 2048       *\n");
	printf("*                                 *\n");
	printf("***********************************\n\n");
}


int dificultad(){
	int resp;
	printf("\nELIJA UNA DIFICULTAD:\n");
	printf("1-Facil\n");
	printf("2-Intermedio\n");
	printf("3-Dificil\n");
	do {
		resp=getint("");
	}while (resp >3 || resp<1);
	return resp;
}
int menu(){
	int resp;
	//printf("ELIJA UNA OPCION:\n");
	printf("1-Juego nuevo\n");
	printf("2-Recuperar un juego guardado\n");
	printf("3-Salir\n");
	resp=getint("ELIJA UNA OPCION:");
	while (resp >3 || resp<1){
		resp=getint("que sea valida!!!!!:");
	}
	if(resp==1){
		resp=dificultad()+3;
	}
	return resp;
}


void ImprimirTablero( tablero tablero){
    int i,j;
    printf("PUNTAJE:%d\n",tablero.puntaje);
    printf("UNDOS:%d\n",tablero.undos);
    for(i=0;i<tablero.dim;i++){
    	printf("------------------------------------------\n");
        for(j=0;j<tablero.dim;j++){
            if(tablero.matriz[i][j]!=0){
            	printf("|%4d", tablero.matriz[i][j]);
            }else{
            	printf("|%4c",' ');
            }
        }
        printf("|\n");
    }
    printf("------------------------------------------\n");
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
	}
}

void getAccion(char comando[]){
	char c;
	int i=0;
	while((c=getchar())!='\n'){
		if(i<MAX_LENGTH-1){
			comando[i++]=c;
		}
	}
	comando[i]=0;
}

int pedirJugada(){
	char comando[MAX_LENGTH];
	printf("Ingrese un comando:\n");
	getAccion(comando);
	int resp;
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
		resp=QUIT;//modificar
		/*printf("Desea guardar la partida?(y/n)\n");
		getAccion(comando);
		if(!strcmp(comando,"y"){
			//guardo la partida
		}else if(){

		}else{

		}*/
	}else if(!strcmp(comando,"save")){
		resp=SAVE;
	}else{
		resp=-1;
	}
	return resp;

}

int main(){
	srand(time(NULL));
	int resp,accion,jugada;
	presentacion();
	resp=menu();
	int direccion,hiceUndo=1,gane=0,perdi=0;;
	tablero tablero1, tablero2, tableroAux;
	casVacios casVacios;
	int movimientos[4];
	

	switch(resp){
		case 2://cargo partida guradada

		break;
		case 3:
			return 0;
		break;
		case 4:case 5: case 6:
			inicializo(&tablero1,&tablero2,&casVacios,resp,movimientos);
			ImprimirTablero(tablero1);
		break;

	}
	do{
		while((accion=pedirJugada())==-1);
		jugada=jugar(&tablero1, &tablero2, &tableroAux,&casVacios ,&hiceUndo,&gane,&perdi,movimientos,accion);
		if(jugada==0){
		    ImprimirTablero(tablero1);
		}else{
			ImprimirError(jugada);
		}

	}while(!gane && !perdi && accion!=QUIT);
	
	if(gane){
		printf("\n\n********** Felicitaciones has ganado!!! **********\n\n");
	}else if (perdi){
        printf("\n\n********** Lo lamento has perdido! **********\n\n");
    }else{
    	printf("\n\n********** Oh Oh, Algo salio mal! **********\n\n");
    }

	return 0;


}