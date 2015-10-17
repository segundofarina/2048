#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define IZQUIERDA 1
#define ARRIBA 2
#define DERECHA 3
#define ABAJO 4

typedef struct{
    int inicio;
    int final;
    int incremento;

} movimiento;
typedef struct {
    int ** matriz;
    //int matriz[4][4];
    int dim;
    int undos;
    int puntaje;
} tablero;
typedef struct{
    int ** matriz;
    //int matriz[16][2];
    int num;
} casVacios;

void creoTablero (tablero * tablero, int dim){
    int i;
    tablero->dim=dim;
    tablero->matriz=(int **) calloc(tablero->dim,sizeof(int*));
    for(i=0;i<tablero->dim; i++){
        tablero->matriz[i]=(int *) calloc(tablero->dim, sizeof(int));
    }
}

void creoCasvacios (casVacios * casVacios, int dim){//matriz de tamaño #casilleros por 2
	int i,j;
	casVacios->num=dim*dim;
	casVacios->matriz= malloc(dim*dim*sizeof(int* ));
	for(i=0;i<dim*dim; i++){
        casVacios->matriz[i]= malloc(2*sizeof(int ));
    }
}

int main(){
	tablero tablero1;
	casVacios casVacios;
	creoTablero (&tablero1,4);
	creoCasvacios (&casVacios, 4);

	


	return 0;

}