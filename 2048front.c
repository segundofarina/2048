#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include "getnum.h"

typedef struct {
    int ** matriz;
   // int matriz[4][4];
    int dim;
    int undos;
    int puntaje;
} tablero;

void presentacion(){
	printf("***********************************\n");
	printf("*                                 *\n");
	printf("*       Bienvenidos al 2048       *\n");
	printf("*                                 *\n");
	printf("***********************************\n\n");
}

int opcion(){
	printf("ELIJA UNA OPCION:\n");
	printf("1-Juego nuevo\n");
	printf("2-Recuperar un juego guardado\n");
	printf("3-Salir\n");
	return getint("");
}
int juegoNuevo(){
	printf("\nELIJA UNA DIFICULTAD:\n");
	printf("1-Facil\n");
	printf("2-Intermedio\n");
	printf("3-Dificil\n");
	return getint("");
}

void ImprimirTablero( tablero tablero){
    int i,j;
    for(i=0;i<tablero.dim;i++){
        for(j=0;j<tablero.dim;j++){
            printf("%d\t", tablero.matriz[i][j]);
        }
        printf("\n");
    }
}

int main(){
	presentacion();
	if (opcion()==1){
		juegoNuevo();
	}


}