#include <stdio.h>

typedef struct {
    //int ** matriz;
    int matriz[4][4];
    int dim;
    int undos;
    int puntaje;
    int numGanador;
} tablero;

void movimientosValidos(tablero tablero1, int movimientos[]){
	int i,j;
	movimientos[0]=0;
	movimientos[1]=0;
	movimientos[2]=0;
	movimientos[3]=0;
	for(i=0;i<tablero1.dim && (movimientos[0]==0 || movimientos[1]==0 || movimientos[2]==0 || movimientos[3]==0);i++){
		for(j=0;j<tablero1.dim && (movimientos[0]==0 || movimientos[1]==0 || movimientos[2]==0 || movimientos[3]==0);j++){
			//si hay un 0 en un borde se puede mover en la direccion del borde, para arriba y para abajo
			/*if(i==0 && tablero1.matriz[i][j]==0){//borde derecho
				movimientos[0]=movimientos[1]=movimientos[3]=1;
			}else if(j==tablero1.dim-1 && tablero1.matriz[i][j]==0){//borde izquierdo
				movimientos[1]=movimientos[2]=movimientos[3]=1;
			}else if(j==0 && tablero1.matriz[i][j]==0){//borde superior
				movimientos[0]=movimientos[1]=movimientos[2]=1;
			}else if(i==tablero1.dim-1 && tablero1.matriz[i][j]==0){//borde inferior
				movimientos[0]=movimientos[2]=movimientos[3]=1;
			}else if(tablero1.matriz[i][j]==0){//si hay un 0 y no es borde se puede mover en cualquier direccion
				movimientos[0]=movimientos[1]=movimientos[2]=movimientos[3]=1;
			}//si hay dos numeros pegados iguales se puede mover en fila o columna dependiendo de como estan pegados
			else if(j!=tablero1.dim-1 && tablero1.matriz[i][j]==tablero1.matriz[i][j+1]){//numeros pegados en fila
				movimientos[0]=movimientos[2]=1;
			}else if(i!=tablero1.dim-1 && tablero1.matriz[i][j]==tablero1.matriz[i+1][j]){//numeros pegados en columna
				movimientos[1]=movimientos[3]=1;
			}*/



			
			
			//suma de numeros
			if(tablero1.matriz[i][j]!=0){//busco casillero que no es 0
				if(j!=0 && tablero1.matriz[i][j-1]==0){//si tengo 0 a la izquierda me puedo mover para la izquierda
					movimientos[0]=1;printf("aca\n");
				}
				if(i!=0 && tablero1.matriz[i-1][j]==0){//si tengo 0 arriba me puedo mover para arriba
					movimientos[1]=1;printf("aca1\n");
				}
				if(j!=tablero1.dim-1 && tablero1.matriz[i][j+1]==0){//si tengo 0 a la derecha me puedo mover para la derecha
					movimientos[2]=1;printf("aca2\n");
				}
				if(i!=tablero1.dim-1 && tablero1.matriz[i+1][j]==0){//si tengo un 0 abajo me puedo mover para abajo
					movimientos[3]=1;printf("aca3\n");
				}
				if(j!=tablero1.dim-1 && tablero1.matriz[i][j]==tablero1.matriz[i][j+1]){//numeros pegados en fila
					movimientos[0]=movimientos[2]=1;printf("aca4\n");
				}else if(i!=tablero1.dim-1 && tablero1.matriz[i][j]==tablero1.matriz[i+1][j]){//numeros pegados en columna
					movimientos[1]=movimientos[3]=1;printf("aca5\n");
				}
			}
		}
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
    printf("\n");
}

void ImprimeMovimientos (int movimientos[]){
	for (int i = 0; i < 4; i++)
	{
		printf("%d\t",movimientos[i]);
	}
	printf("\n");
	

}

int main(){
	tablero tablero1={ { {1,2,4,5},{2,7,8,9},{10,11,12,0},{14,15,16,17} }, 4 };

	int movimientos[4];
	ImprimirTablero(tablero1);
	movimientosValidos(tablero1, movimientos);
	ImprimeMovimientos(movimientos);

	return 0;
}