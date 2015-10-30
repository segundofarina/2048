#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define IZQUIERDA 1
#define ARRIBA 2
#define DERECHA 3
#define ABAJO 4


typedef struct {
    int ** matriz;
   // int matriz[4][4];
    int dim;
    int undos;
    int puntaje;
} tablero;

void creoTablero (tablero * tablero, int dim){
    int i;
    tablero->dim=dim;
    tablero->matriz=(int **) calloc(tablero->dim,sizeof(int*));
    for(i=0;i<tablero->dim; i++){
        tablero->matriz[i]=(int *) calloc(tablero->dim, sizeof(int));
    }
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
    srand(time(NULL));
    tablero tablero;
    creoTablero(&tablero, 8);
    ImprimirTablero(tablero);
    return 0;
}

