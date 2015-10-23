#include "2048back.h"

int creoTablero (sTablero * tablero, int dim, int undos, int ganador){
    int i;
    tablero->puntaje=0;
    tablero->undos=undos;
    tablero->numGanador=ganador;
    tablero->dim=dim;
    tablero->matriz= calloc(tablero->dim,sizeof(int*));
    if (tablero->matriz == NULL){
        return ERR_MEMORIA;
    }
    for(i=0;i<tablero->dim; i++){
        tablero->matriz[i]=calloc(tablero->dim, sizeof(int));
        if (tablero->matriz[i] == NULL){
            return ERR_MEMORIA;
        }
    }
    return 0;
}

int creoCasvacios (sCasVacios * casVacios, int dim){//matriz de tamaño #casilleros por 2
	int i,j,h=0;
	casVacios->num=dim*dim;//Incicializa cantidad de casilleros en dim*dim 
	casVacios->matriz= malloc(dim*dim*sizeof(int* ));
    if (casVacios->matriz == NULL){
        return ERR_MEMORIA;
    }
	for(i=0;i<dim*dim; i++){
        casVacios->matriz[i]= malloc(2*sizeof(int ));
        if (casVacios->matriz[i] == NULL){
            return ERR_MEMORIA;
        }
    }

    for(i=0;i<dim;i++){ //Guarda la posicion de todos los casilleros de la matriz
        for(j=0;j<dim;j++){
            casVacios->matriz[h][0]=i;
            casVacios->matriz[h++][1]=j;
        }
    }
    return 0;
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

int buscoCasillero(sCasVacios vacios, int * posI, int *posJ){ // eligo un casillero vacio aleatorio en base a la matriz de casilleros vacios que le paso por parametro
	int alearorio=randInt(0,(vacios.num)-1);

	*posI=vacios.matriz[alearorio][0];//devuelvo posicion elejida i,j
	*posJ=vacios.matriz[alearorio][1];

	return alearorio;
}

void pongoFicha(sTablero * nuevo, sCasVacios * vacios){//agrego una nueva ficha aleatoria al tablero
	int i,j,pos,ficha=nuevaFicha();
	pos=buscoCasillero(*vacios,&i,&j);
	nuevo->matriz[i][j]=ficha;
	//borro posicion vacia usada
	(vacios->num)--;
	for(;pos<(vacios->num);pos++){
		vacios->matriz[pos][0]=vacios->matriz[pos+1][0];
		vacios->matriz[pos][1]=vacios->matriz[pos+1][1];
	}
}


int sumoFila(sMovimiento I, sMovimiento J, sTablero m, sTablero * nueva, sCasVacios * vacios){

    int i,j,k=I.inicio, h=J.inicio, sume=0, gane=0;

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

                /*verifico si gane*/
                if(nueva->matriz[k-I.incremento][h-J.incremento]==nueva->numGanador){
                	gane=1;
                }


            }else{
                sume=0;//aviso que puedo sumar la proxima pasada
                k+=I.incremento;
                h+=J.incremento;
                /*si no sume si incremento los contadores de la matriz nueva*/
            }
        }
    }
    for( ; k!=I.final && h!=J.final; k+=I.incremento, h+=J.incremento ){//relleno con 0 al final de la fila-columna
        nueva->matriz[k][h]=0;

        /*guardo los casilleros vacios*/
        vacios->matriz[vacios->num][0]=k;
        vacios->matriz[(vacios->num)++][1]=h;
    }
    return gane;
}

void descifroMovimiento (int direccion, sMovimiento * I, sMovimiento * J,int dim){

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


int muevoTablero(int direccion, sTablero viejo, sTablero * nuevo, sCasVacios * vacios){
    /*Creo los dos sentidos de movimiento y recorro la matriz llamando a la funcion que suma las filas en sentido opuesto a la direccion*/
    sMovimiento I,J;
    int gane=0;

    descifroMovimiento(direccion,&I,&J,viejo.dim);

    nuevo->puntaje=viejo.puntaje;

    vacios->num=0;//inicializo el contador de la matriz que guarda los vacios

    for ( ; I.inicio!=I.final && J.inicio!=J.final; I.inicio+=abs(J.incremento), J.inicio+=abs(I.incremento) )
    {
        if(sumoFila(I,J,viejo,nuevo,vacios)){
        	gane=1;
        }

    }
    return gane;
}
void swapTableros (sTablero * tablero1, sTablero * tablero2, sTablero * aux){
    *aux=*tablero1;
    *tablero1=*tablero2;
    *tablero2=*aux;
}

void undo(sTablero * tablero1, sTablero * tablero2, sTablero * aux){
	swapTableros(tablero1, tablero2, aux);
	(tablero1->undos)--;
	(tablero2->undos)--;
}

void movimientosValidos(sTablero tablero1, int movimientos[]){
	int i,j;
	movimientos[0]=0;
	movimientos[1]=0;
	movimientos[2]=0;
	movimientos[3]=0;
	for(i=0;i<tablero1.dim && (movimientos[0]==0 || movimientos[1]==0 || movimientos[2]==0 || movimientos[3]==0);i++){
		for(j=0;j<tablero1.dim && (movimientos[0]==0 || movimientos[1]==0 || movimientos[2]==0 || movimientos[3]==0);j++){
			if(tablero1.matriz[i][j]!=0){//busco casillero que no es 0
				if(j!=0 && tablero1.matriz[i][j-1]==0){//si tengo 0 a la izquierda me puedo mover para la izquierda
					movimientos[0]=1;
				}
				if(i!=0 && tablero1.matriz[i-1][j]==0){//si tengo 0 arriba me puedo mover para arriba
					movimientos[1]=1;
				}
				if(j!=tablero1.dim-1 && tablero1.matriz[i][j+1]==0){//si tengo 0 a la derecha me puedo mover para la derecha
					movimientos[2]=1;
				}
				if(i!=tablero1.dim-1 && tablero1.matriz[i+1][j]==0){//si tengo un 0 abajo me puedo mover para abajo
					movimientos[3]=1;
				}
				if(j!=tablero1.dim-1 && tablero1.matriz[i][j]==tablero1.matriz[i][j+1]){//numeros pegados en fila
					movimientos[0]=movimientos[2]=1;
				}
				if(i!=tablero1.dim-1 && tablero1.matriz[i][j]==tablero1.matriz[i+1][j]){//numeros pegados en columna
					movimientos[1]=movimientos[3]=1;
				}
			}
		}
	}
}

int fperdi(int movimientos[], sTablero tablero){
	if(movimientos[0]==0 && movimientos[1]==0 && movimientos[2]==0 && movimientos[3]==0 && tablero.undos==0){
        return 1;
	}else{
		return 0;
	}
}

int inicializo(sTablero * tablero1, sTablero * tablero2, sCasVacios * casVacios, int dificultad, int movimientos[]){
    int error;
    switch(dificultad){
        case FACIL:
            error=creoTablero(tablero1,8,8,1024);
            if(error==0){
                error=creoTablero(tablero2,8,8,1024);
            }
            if(error==0){
                error=creoCasvacios(casVacios,8);
            }
            if(error==ERR_MEMORIA){
                return ERR_MEMORIA;
            }
            
            break;
        case INTERMEDIO:
            error=creoTablero(tablero1,6,4,2048);
            if(error==0){
                error=creoTablero(tablero2,6,4,2048);
            }
            if(error==0){
                error=creoCasvacios(casVacios,6);
            }
            if(error==ERR_MEMORIA){
                return ERR_MEMORIA;
            }
            break;
        case DIFICIL:
            error=creoTablero(tablero1,4,3,2048);
            if(error==0){
                error=creoTablero(tablero2,4,3,2048);
            }
            if(error==0){
                error=creoCasvacios(casVacios,4);
            }
            if(error==ERR_MEMORIA){
                return ERR_MEMORIA;
            }
            break;
    }
    pongoFicha (tablero1,casVacios);//agrego primer ficha
    //modifico casVacios
    pongoFicha (tablero1,casVacios);//agrego segunda ficha

    movimientosValidos(*tablero1, movimientos);

    return 0;
}

int jugar(sTablero * tablero1,sTablero * tablero2, sTablero * tableroAux,sCasVacios * casVacios, int * hiceUndo,int * gane, int * perdi,int movimientos[], int accion){
    int error=0;
        if(accion==UNDO){//hago undo
            if(tablero1->undos>0 && !*hiceUndo){
                undo (tablero2, tablero1, tableroAux);
                *hiceUndo=1;
                movimientosValidos(*tablero1, movimientos);
            }else{
                *hiceUndo=0;
                error=ERR_UNDO;
            }
        }else if(accion==IZQUIERDA || accion==ARRIBA || accion==DERECHA || accion==ABAJO){//si es movimiento valido
            if(movimientos[accion-1]!=0){
                *gane = muevoTablero(accion,*tablero1,tablero2,casVacios);
                swapTableros (tablero1, tablero2, tableroAux);
                pongoFicha (tablero1,casVacios);
                *hiceUndo=0;
                movimientosValidos(*tablero1, movimientos);
                *perdi=fperdi(movimientos, *tablero1);
            }
            else if (movimientos[IZQUIERDA-1]==0&& movimientos[ARRIBA-1]==0&& movimientos[DERECHA-1]==0&& movimientos[ABAJO-1]==0&& tablero1->undos!=0){
                error=ERR_FORZADO;
            }

            else{
                error=ERR_MOV;
            }

        }
    return error;
}

int guardar(char fileName[], sTablero tablero){
    int i;
    FILE * archivo;
    if (archivo==NULL){
        return ERR_COPIA;
    }
    unsigned short int dificultad;
    if (tablero.dim==4){
        dificultad=3;
    }
     if (tablero.dim==6){
        dificultad=2;
    }
     if (tablero.dim==8){
        dificultad=1;
    }
    archivo=fopen(fileName,"w");
    fwrite(&dificultad,sizeof(dificultad),1,archivo);
    fwrite(&(tablero.puntaje), sizeof(tablero.puntaje),1,archivo);
    for(i=0;i<tablero.dim; i++){
        fwrite(tablero.matriz[i],sizeof(tablero.matriz[i]),1,archivo);
    }
    fclose(archivo);

    return 0;


}