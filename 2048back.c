#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "getnum.h"

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
void ImprimirTablero( tablero tablero);
void Imprimecasvacios (casVacios casVacios);

void creoTablero (tablero * tablero, int dim){
    int i;
    tablero->puntaje=0;
    tablero->dim=dim;
    tablero->matriz=(int **) calloc(tablero->dim,sizeof(int*));
    for(i=0;i<tablero->dim; i++){
        tablero->matriz[i]=(int *) calloc(tablero->dim, sizeof(int));
    }
}

void creoCasvacios (casVacios * casVacios, int dim){//matriz de tamaño #casilleros por 2
	int i,j,h=0;
	casVacios->num=dim*dim;//Incicializa cantidad de casilleros en dim*dim 
	casVacios->matriz= malloc(dim*dim*sizeof(int* ));
	for(i=0;i<dim*dim; i++){
        casVacios->matriz[i]= malloc(2*sizeof(int ));
    }
    for(i=0;i<dim;i++){ //Guarda la posicion de todos los casilleros de la matriz
        for(j=0;j<dim;j++){
            casVacios->matriz[h][0]=i;
            casVacios->matriz[h++][1]=j;
        }
    }
}
int randInt(int inicio, int final){//aleatorio entre inicio y final
	int aux;
	aux=rand()%(final+1)+inicio;
	return aux;

}

int nuevaFicha(){//aleatorio 2 o 4

	int alearorio = randInt(1,100);
	if(alearorio<=89){
		return 2;
	}else{
		return 4;
	}
}

void buscoCasillero(casVacios vacios, int * posI, int *posJ){ // eligo un casillero vacio aleatorio en base a la matriz de casilleros vacios que le paso por parametro

	int alearorio=randInt(0,(vacios.num)-1);

	*posI=vacios.matriz[alearorio][0];//devuelvo posicion elejida i,j
	*posJ=vacios.matriz[alearorio][1];
}

void pongoFicha(tablero * nuevo, casVacios vacios){//agrego una nueva ficha aleatoria al tablero
	int i,j,ficha=nuevaFicha();
	buscoCasillero(vacios,&i,&j);
	nuevo->matriz[i][j]=ficha;
}


void sumoFila(movimiento I, movimiento J, tablero m, tablero * nueva, casVacios * vacios){

    int i,j,k=I.inicio, h=J.inicio, sume=0;

    for( i=I.inicio, j=J.inicio; i!=I.final && j!=J.final; i+=I.incremento, j+=J.incremento ){//recorro la fila o la columna dependiendo de los paramentros
        
        if(m.matriz[i][j]!=0){//salteo casilleros vacios
            
            nueva->matriz[k][h]=m.matriz[i][j];//copio los casilleros llenos a la matriz nueva
            /*voy a fijarme si el numero que acabo de copiar se deberia sumar para combinar con el anterior de la fila-columna */
            if( (k!=I.inicio || h!=J.inicio) && !sume && nueva->matriz[k][h]==nueva->matriz[k-I.incremento][h-J.incremento]){//si no es el primer casillero de la fila-columna, y si no acabo de hacer una suma, verifico si el numero es igual al casillero anterior
                
                nueva->matriz[k-I.incremento][h-J.incremento]*=2;//casillero anterior por 2
                nueva->matriz[k][h]=0;//casillero actual vacio
                sume=1;//aviso que la proxima pasada no tengo que sumar
                /*IMPORTANTE no muevo el contador de la matriz nueva para volver al casillero que vacie*/

                /*sumo puntaje*/
                nueva->puntaje+=nueva->matriz[k-I.incremento][h-J.incremento];


            }else{
                sume=0;//aviso que puedo sumar la proxima pasada
                k+=I.incremento;
                h+=J.incremento;
                /*si no sume si incremento los contadores de la matriz nueva*/
            }
        }
    }
    //vacios->num=0;//inicializo el contador de la matriz que guarda los vacios
    for( ; k!=I.final && h!=J.final; k+=I.incremento, h+=J.incremento ){//relleno con 0 al final de la fila-columna
        nueva->matriz[k][h]=0;

        /*guardo los casilleros vacios*/
        vacios->matriz[vacios->num][0]=k;
        vacios->matriz[(vacios->num)++][1]=h;
    }
}

void descifroMovimiento (int direccion, movimiento * I, movimiento * J,int dim){

    switch(direccion){
        case IZQUIERDA:
            I->inicio=0;
            J->inicio=0;
            I->final=dim;
            J->final=dim;
            I->incremento=0;
            J->incremento=1;
        break;
        case ARRIBA:
            I->inicio=0;
            J->inicio=0;
            I->final=dim;
            J->final=dim;
            I->incremento=1;
            J->incremento=0;
        break;
        case DERECHA:
            I->inicio=0;
            J->inicio=dim-1;
            I->final=dim;
            J->final=-1;
            I->incremento=0;
            J->incremento=-1;
        break;
        case ABAJO:
            I->inicio=dim-1;
            J->inicio=0;
            I->final=-1;
            J->final=dim;
            I->incremento=-1;
            J->incremento=0;
        break;
    }
}


void muevoTablero(int direccion, tablero viejo, tablero * nuevo, casVacios * vacios){
    /*Creo los dos sentidos de movimiento y recorro la matriz llamando a la funcion que suma las filas en sentido opuesto a la direccion*/
    movimiento I,J;

    descifroMovimiento(direccion,&I,&J,viejo.dim);

    nuevo->puntaje=viejo.puntaje;

    vacios->num=0;//inicializo el contador de la matriz que guarda los vacios

    for ( ; I.inicio!=I.final && J.inicio!=J.final; I.inicio+=abs(J.incremento), J.inicio+=abs(I.incremento) )
    {
        sumoFila(I,J,viejo,nuevo,vacios);

    }
}
void swapTableros (tablero * tablero1, tablero * tablero2, tablero * aux){/*no hace falta rotarlos solo los igualo*/
    *aux=*tablero1;
    *tablero1=*tablero2;
    *tablero2=*aux;
}


int main(){
	srand(time(NULL));
	int direccion;
	tablero tablero1;
	tablero tablero2;
	tablero tableroAux;
	casVacios casVacios;
	creoTablero (&tablero1,4);
	creoTablero (&tablero2,4);
	creoCasvacios (&casVacios, 4);
    Imprimecasvacios(casVacios);
	ImprimirTablero(tablero1);
	pongoFicha (&tablero1,casVacios);
	ImprimirTablero (tablero1);
	
	while((direccion=getint("Para que lado moves??\n"))!=5){
		muevoTablero(direccion,tablero1,&tablero2,&casVacios);
        swapTableros (&tablero1, &tablero2, &tableroAux);
		pongoFicha (&tablero1,casVacios);

		printf("\n");

		ImprimirTablero (tablero1);
		

	}

	return 0;

}

void ImprimirTablero( tablero tablero){
    int i,j;
    printf("PUNTAJE:%d\n",tablero.puntaje);
    for(i=0;i<tablero.dim;i++){
        for(j=0;j<tablero.dim;j++){
            printf("%d\t", tablero.matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void Imprimecasvacios (casVacios casVacios){
    printf("NUM=%d\n",casVacios.num );
	for (int i = 0; i < casVacios.num; ++i)
	{
		printf("%d\t%d\n",casVacios.matriz[i][0], casVacios.matriz[i][1] );
	}
	

}