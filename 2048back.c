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
    int * matriz[2];
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

void creoCasvacios (casVacios * casVacios, int dim){
	int i,j,h=0;
	casVacios->num=dim*dim;
	casVacios->matriz[0]=malloc(dim*dim*sizeof(int *));
	casVacios->matriz[1]=malloc(dim*dim*sizeof(int *));
	for(i=0; i<dim-1; i++){
		for(j=0; j<dim-1; j++){
		casVacios->matriz[h][0]=i;
		casVacios->matriz[h++][1]=j;
	}
	}
}

int main(){
	tablero tablero1;
	casVacios casVacios;
	creoTablero (&tablero1,4);
	creoCasvacios (&casVacios, 4);

	for (int i = 0; i < 16; ++i)
	{
		printf("%d\t%d\t",casVacios.matriz[i][0], casVacios.matriz[i][1] );
	}

	return 0;

}