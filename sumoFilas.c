#include <stdio.h>
#include <stdlib.h>

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
    //int ** matriz;
    int matriz[4][4];
    int dim;
    int undos;
    int puntaje;
} tablero;
typedef struct{
    //int * matriz[2];
    int matriz[16][2];
    int num;
} casVacios;


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

    vacios->num=0;//inicializo el contador de la matriz que guarda los vacios

    for ( ; I.inicio!=I.final && J.inicio!=J.final; I.inicio+=abs(J.incremento), J.inicio+=abs(I.incremento) )
    {
        sumoFila(I,J,viejo,nuevo,vacios);

    }
}

int main(){

    tablero tablero1={ { {2,2,4,0},{4,2,2,0},{8,4,4,8},{2,2,2,2} }, 4 },tablero2={ {{1,0}}, 4 };
    casVacios vacios;

    int i,j;

    for(i=0;i<tablero1.dim;i++){
        for(j=0;j<tablero1.dim;j++){
            printf("%d\t", tablero1.matriz[i][j]);
        }
        printf("\n");
    }
    printf("**** Puntaje: %d\n", tablero1.puntaje);


printf("\n\n**********************\n\n");

    muevoTablero(2,tablero1,&tablero2,&vacios);

    for(int p=0;p<tablero1.dim;p++){
        for(int o=0;o<tablero1.dim;o++){
            printf("%d\t", tablero2.matriz[p][o]);
        }
        printf("\n");
    }
    printf("**** Puntaje: %d\n", tablero2.puntaje);


printf("\n\n**********************\n\n");

    printf("I\tJ\n");
    for(int p=0;p<vacios.num;p++){
        printf("%d\t%d", vacios.matriz[p][0],vacios.matriz[p][1]);
        printf("\n");
    }


    return 0;
}

