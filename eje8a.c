#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//me llego

void ImprimirBolillero(char bolillero[], int restantes){
	int i;
	for(i=0; i<restantes; i++)
		printf("%d\n",bolillero[i]);
}
void CrearBolillero (int bolillero[90]){
	int i;
	for (i=0;i<90;i++)
		bolillero[i]=i+1;
}
void CrearCarton(int carton[3][5]){
	int auxiliar[90];
	int i,j,elementos=90,indice;
	CrearBolillero(auxiliar);
	for(i=0;i<3;i++){
		for (j=0;j<5;j++){
			indice=rand()%(elementos);
			carton[i][j]=auxiliar[indice];
			auxiliar[indice]=auxiliar[--elementos];
		}
	}
}



int SacarBolilla(int bolillero[], int * restantes){
	int indice, numero,quedan;
	quedan=*restantes;
	indice=rand()%quedan;
	numero=bolillero[indice];
	bolillero[indice]=bolillero[--quedan];
	*restantes=quedan;
	return numero;
}

void ImprimirCarton(int carton[3][5]){
	int i,j;
	for (i=0;i<3;i++){
		for(j=0;j<5;j++)
			printf("%d\t",carton[i][j]);
		printf("\n");
	}
}
int BuscarBolilla (int carton [3][5], int numero){
	int i,j,esta=0;
	for(i=0; i<3 && !esta; i++){
		for(j=0; j<5 && !esta; j++){
			if (carton[i][j]==numero){
				carton[i][j]=0;
				esta=1;
			}
		}
	}
	return esta;
}
int ControlarLinea(int carton [3][5], int linea){
	int i=0,eslinea=1;
	for (i=0; i<5 && eslinea; i++)
	{
		if (carton[linea][i]!=0){
			eslinea=0;
		}
	}
	return eslinea;
}

int ControlarCarton (int carton [3][5], int numero){
	int lineascompletas=0,i;
	BuscarBolilla(carton, numero);
	for (i=0; i<3;i++){
		if (ControlarLinea(carton, i))
			lineascompletas++;
	}
	return lineascompletas;

}

int jugar(int carton1[3][5] , int carton2[3][5], int bolillero[] ){
	int bingo=0, numero,restantes=90,i;
	while(!bingo){
		numero=SacarBolilla(bolillero,&restantes);
		printf("\n\nSalio el NUMERO %d\n",numero);
		printf("Numeros restantes en el bolillero:%d\n\n",restantes);
		if (ControlarCarton(carton1,numero)==3)
			bingo=1;
		if(ControlarCarton(carton2,numero)==3)
			bingo=2;
		if(ControlarCarton(carton2,numero)==3 && ControlarCarton(carton1,numero)==3)
			bingo=3;
		printf("carton1\n");
		ImprimirCarton(carton1);
		printf("\ncarton2\n");
		ImprimirCarton(carton2);
	}
	
	return bingo;
}


int main(){
	srand(time(NULL));
	int carton1[3][5], carton2[3][5], bolillero[90], bingo=0;
	CrearCarton(carton1);
	CrearCarton(carton2);
	CrearBolillero(bolillero);
	printf("carton1\n");
	ImprimirCarton(carton1);
	printf("\ncarton2\n");
	ImprimirCarton(carton2);
	bingo=jugar(carton1,carton2,bolillero);
	if (bingo==1)
		printf("Gano el jugador 1\n");
	if (bingo==2)
		printf("Gano el jugador 2\n");
	if(bingo==3)
		printf("Ambos Ganaron\n");
	return 0;
}













