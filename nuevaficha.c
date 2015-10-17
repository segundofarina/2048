#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
    //int * matriz[2];
    int matriz[16][2];
    int num;
} casVacios;

int randInt(int inicio, int final){
	int aux;
	aux=rand()%(final+1)+inicio;
	return aux;

}


int nuevaFicha(){

	int alearorio = randInt(1,100);
	if(alearorio<=89){
		return 2;
	}else{
		return 4;
	}
}

void buscoCasillero(casVacios vacios, int * posI, int *posJ){

	int alearorio=randInt(0,(vacios.num)-1);

	*posI=vacios.matriz[alearorio][0];
	*posJ=vacios.matriz[alearorio][1];
}

int main(){

	srand(time(NULL));

	int posI,posJ;

	casVacios vacios={ { {0,2},{0,3},{1,2},{1,3},{2,3},{3,2},{3,3} }, 7 };

	printf("%d\n", nuevaFicha());

	buscoCasillero(vacios,&posI,&posJ);

	printf("%d\t%d\n", posI, posJ);


	return 0;
}