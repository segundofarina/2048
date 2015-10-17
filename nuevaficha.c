#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
    //int * matriz[2];
    int matriz[16][2];
    int num;
} casVacios;

int randInt(int inicio, int final){

}


int nuevaFicha(){
	srand(time(NULL));

	int alearorio = randInt(0,100);
	if(alearorio<=89){
		return 2;
	}else{
		return 4;
	}
}

void buscoCasillero(casVacios vacios, int * posI, int *posJ){
	srand(time(NULL));

	int alearorio=randInt(0,(vacios.num)-1);

	*posI=vacios.matriz[alearorio][0];
	*posJ=vacios.matriz[alearorio][1];
}

int main(){

	return 0;
}