#include "2048back.h"


static int creoTablero (sTablero * tablero, int dim, int undos, int ganador);
static int creoCasvacios (sCasVacios * casVacios, int dim);
static int randInt(int inicio, int final);
static int nuevaFicha();
static int buscoCasillero(sCasVacios vacios, int * posI, int *posJ);
static void pongoFicha(sTablero * nuevo, sCasVacios * vacios);
static int sumoFila(sMovimiento I, sMovimiento J, sTablero m, sTablero * nueva, sCasVacios * vacios);
static void descifroMovimiento (int direccion, sMovimiento * I, sMovimiento * J,int dim);
static int muevoTablero(int direccion, sTablero viejo, sTablero * nuevo, sCasVacios * vacios);
static void swapTableros (sTablero * tablero1, sTablero * tablero2, sTablero * aux);
static void undo(sTablero * tablero1, sTablero * tablero2, sTablero * aux);
static void movimientosValidos(sTablero tablero1, int movimientos[]);
static int fperdi(const int movimientos[], sTablero tablero);
static int casilleroValido(int val, int numGanador);
static int validoPartida(sTablero tablero);

/*
** La funcion creoTablero crea una matriz dinamica de dim*dim que sera el tablero en si
** y guarda en la estrucura el numero ganador,dimension e incializa el puntaje en 0 y 
** la cantidad de undos correspondiente. En caso de no disponer de memoria suficiente
** devuelve el error ERR_MEMORIA.
*/
int creoTablero (sTablero * tablero, int dim, int undos, int ganador){
    int i;
    tablero->puntaje=0;
    tablero->undos=undos;
    tablero->numGanador=ganador;
    tablero->dim=dim;
    tablero->matriz= calloc(tablero->dim,sizeof(unsigned short int*));
    if (tablero->matriz == NULL){
        return ERR_MEMORIA;
    }
    for(i=0;i<tablero->dim; i++){
        tablero->matriz[i]=calloc(tablero->dim,sizeof(unsigned short int));
        if (tablero->matriz[i] == NULL){
            return ERR_MEMORIA;
        }
    }
    return 0;
}

/*
** La funcion creoCasVacios crea una matriz dinamica de 2x(dim*dim), que guarda
** la posicion i y j de los espacios vacios del tablero. En un comienzo guarda todas
** las posicones del tablero ya que estan todas vacias.
*/

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

/* genera un aleatorio entre inicio y final*/
int randInt(int inicio, int final){
	int aux;
	aux=rand()%(final+1)+inicio;
	return aux;
}

/*genera el valor de la nueva ficha con 89% de probabilidad de ser un 2 y el resto 4*/
int nuevaFicha(){
	int aleatorio = randInt(1,100);
	if(aleatorio<=89){
		return 2;
	}else{
		return 4;
	}
}
/* Elijo un casillero vacio aleatorio en base a la matriz de casilleros 
** vacios que le paso por parametro y cargo en los parametros de salida
** la posicion elejida i,j.
*/
int buscoCasillero(sCasVacios vacios, int * posI, int *posJ){ 
	int aleatorio=randInt(0,(vacios.num)-1);

	*posI=vacios.matriz[aleatorio][0];
	*posJ=vacios.matriz[aleatorio][1];

	return aleatorio;
}
/*
** La funcion pongoFicha llama a las funciones nuevaFicha y buscoCasillero y de acuerdo
** a los valores obtenidos coloca la ficha en la posicion del tablero. Luego intercambia
** los valores de la matriz de casilleros vacios obtenidos con el ultimo y decrementa el 
** valor guardado del tamaño de la misma. No se borra la posicion usada para luego poder
** realizar undo y volover a tener disponible el casillero utilizado.
*/
void pongoFicha(sTablero * nuevo, sCasVacios * vacios){
	int i,j,pos,ficha=nuevaFicha(),aux[2];
	pos=buscoCasillero(*vacios,&i,&j);
	nuevo->matriz[i][j]=ficha;
    vacios->matriz[pos][0]=aux[0];
    vacios->matriz[pos][1]=aux[1];
    vacios->matriz[pos][0]=vacios->matriz[vacios->num-1][0];
    vacios->matriz[pos][1]=vacios->matriz[vacios->num-1][1];
    vacios->matriz[vacios->num-1][0]=aux[0];
    vacios->matriz[vacios->num-1][1]=aux[1];
	(vacios->num)--;
}

/*
** La funcion se encarga de mover y sumar los elemetos de una linea en la direccion
** ingresada por el usuario. Recibe por parametros las direcciones de movimiento
** que seran desde donde inicia hasta donde termina, y en que sentido se desplazara
** (el incremento). A medida que recorre la linea, saltea las posciones que contienen
** vacios (0), copia los valores distintas de 0 en la matriz nueva, y posteriormente 
** verifica si el numero exacamente pegado en la direccion del movimiento es igual a si
** mismo. Si no son iguales saltea al proximo casillero, y de ser iguales verifica si 
** en la copia anterior se realizo una suma. De haber una suma realizada, no se suman los
** casilleros, y sino estos se suman. Luego de realizar una suma verifico que el numero
** obtenido sea el numero establecido como ganador, y sumo al puntaje que tenia.
** Si gane termino la ejecucion, sino continuo al siguiente casillero.
** Al terminar de recorrer la linea, compĺeto con 0 al final, indicando que seran casilleros
** vacios. Y voy guardando estos casilleros en la matriz de casilleros vacios.
** Devuelvo si gane o no.
*/

int sumoFila(sMovimiento I, sMovimiento J, sTablero m, sTablero * nueva, sCasVacios * vacios){

    int i,j,k=I.inicio, h=J.inicio, sume=0, gane=0;
    /*recorro la fila o la columna dependiendo de los paramentros*/
    for( i=I.inicio, j=J.inicio; i!=I.final && j!=J.final; i+=I.incremento, j+=J.incremento ){
        /*salteo casilleros vacios*/
        if(m.matriz[i][j]!=0){
            /*copio los casilleros llenos a la matriz nueva*/
            nueva->matriz[k][h]=m.matriz[i][j];
            /*me fijo si el numero que acabo de copiar se deberia sumar
            **para combinar con el anterior de la fila-columna */
            if( (k!=I.inicio || h!=J.inicio) && !sume && nueva->matriz[k][h]==nueva->matriz[k-I.incremento][h-J.incremento]){//si no es el primer casillero de la fila-columna, y si no acabo de hacer una suma, verifico si el numero es igual al casillero anterior
                /*entro en la suma*/

                nueva->matriz[k-I.incremento][h-J.incremento]*=2;/*casillero anterior x2*/
                nueva->matriz[k][h]=0;/*casillero actual vacio*/
                sume=1;/*aviso que la proxima pasada no tengo que sumar*/

                /*IMPORTANTE no muevo el contador de la matriz nueva
                **para en la proxima pasada volver al casillero que vacie*/

                /*sumo puntaje*/
                nueva->puntaje+=nueva->matriz[k-I.incremento][h-J.incremento];

                /*verifico si gane*/
                if(nueva->matriz[k-I.incremento][h-J.incremento]==nueva->numGanador){
                	gane=1;
                }


            }else{/*si no sume*/
                sume=0;/*aviso que puedo sumar la proxima pasada*/
                k+=I.incremento;
                h+=J.incremento;
                /*incremento los contadores de la matriz nueva*/
            }
        }
    }
    /*relleno con 0 al final de la fila-columna*/
    for( ; k!=I.final && h!=J.final; k+=I.incremento, h+=J.incremento ){
        nueva->matriz[k][h]=0;

        /*guardo los casilleros vacios en la matriz de casilleros vacios*/
        vacios->matriz[vacios->num][0]=k;
        vacios->matriz[(vacios->num)++][1]=h;
    }
    return gane;
}
/*Guarda en la estrcura de movimento como debe recorrese la matriz*/
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

/*Indica como se debe mover cada linea de la matriz*/
int muevoTablero(int direccion, sTablero viejo, sTablero * nuevo, sCasVacios * vacios){
    /* Creo los dos sentidos de movimiento y recorro la matriz llamando a la funcion
    ** que suma las filas en sentido opuesto a la direccion.*/
    sMovimiento I,J;
    int gane=0;

    descifroMovimiento(direccion,&I,&J,viejo.dim);

    nuevo->puntaje=viejo.puntaje;

    vacios->num=0;/*inicializo el contador de la matriz que guarda los vacios*/

    for ( ; I.inicio!=I.final && J.inicio!=J.final; I.inicio+=abs(J.incremento), J.inicio+=abs(I.incremento) )
    {
        if(sumoFila(I,J,viejo,nuevo,vacios)){
        	gane=1;
        }

    }
    return gane;
}

/*Pone en tablero1 el tablero actual y en tablero2 el tablero de la jugada anterior*/
void swapTableros (sTablero * tablero1, sTablero * tablero2, sTablero * aux){
    *aux=*tablero1;
    *tablero1=*tablero2;
    *tablero2=*aux;
}

/* Rota los tableros y decremento la cantidad de undos*/
void undo(sTablero * tablero1, sTablero * tablero2, sTablero * aux){
	swapTableros(tablero1, tablero2, aux);
	(tablero1->undos)--;
	(tablero2->undos)--;
}
/* Alamcena en un vector movimentos 1 si el movimiento es valido en cada direccion o 0 en caso contrario */
void movimientosValidos(sTablero tablero1, int movimientos[]){
	int i,j;
	movimientos[0]=0;
	movimientos[1]=0;
	movimientos[2]=0;
	movimientos[3]=0;
	for(i=0;i<tablero1.dim && (movimientos[0]==0 || movimientos[1]==0 || movimientos[2]==0 || movimientos[3]==0);i++){
		for(j=0;j<tablero1.dim && (movimientos[0]==0 || movimientos[1]==0 || movimientos[2]==0 || movimientos[3]==0);j++){
			/*busco casillero que no es 0*/
			if(tablero1.matriz[i][j]!=0){
				/*si tengo 0 a la izquierda me puedo mover para la izquierda*/
				if(j!=0 && tablero1.matriz[i][j-1]==0){
					movimientos[0]=1;
				}
				/*si tengo 0 arriba me puedo mover para arriba*/
				if(i!=0 && tablero1.matriz[i-1][j]==0){
					movimientos[1]=1;
				}
				/* si tengo 0 a la derecha me puedo mover para la derecha*/
				if(j!=tablero1.dim-1 && tablero1.matriz[i][j+1]==0){
					movimientos[2]=1;
				}
				/* si tengo un 0 abajo me puedo mover para abajo*/
				if(i!=tablero1.dim-1 && tablero1.matriz[i+1][j]==0){
					movimientos[3]=1;
				}
				/*numeros pegados en fila*/
				if(j!=tablero1.dim-1 && tablero1.matriz[i][j]==tablero1.matriz[i][j+1]){
					movimientos[0]=movimientos[2]=1;
				}
				/* numeros pegados en columna*/
				if(i!=tablero1.dim-1 && tablero1.matriz[i][j]==tablero1.matriz[i+1][j]){
					movimientos[1]=movimientos[3]=1;
				}
			}
		}
	}
}
/* Si no hay movimientos validos en ninguna dirrecion y no tengo mas undos devulve 1*/
int fperdi(const int movimientos[], sTablero tablero){
	if(movimientos[0]==0 && movimientos[1]==0 && movimientos[2]==0 && movimientos[3]==0 && tablero.undos==0){
        	return 1;
	}else{
		return 0;
	}
}
/*Dependiendo de la dificultad ingresada por parametero se crean dos tableros, y la estructura de 
** casilleros vacios. Elprimer tablero alamcena la partida actual, el segundo tablero alamcena la
** juagada anterior para luego poder realizar el undo.*/
int creoEntorno(sTablero * tablero1, sTablero * tablero2, sCasVacios * casVacios, int dificultad){
	int error=0;
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
            error=creoTablero(tablero1,4,2,2048);
            if(error==0){
                error=creoTablero(tablero2,4,2,2048);
            }
            if(error==0){
                error=creoCasvacios(casVacios,4);
            }
            if(error==ERR_MEMORIA){
                return ERR_MEMORIA;
            }
            break;
    }
    return error;
}

/*Crea el entorno de juego y pone las dos fichas iniciales en el tablero1.*/
int inicializo(sTablero * tablero1, sTablero * tablero2, sCasVacios * casVacios, int dificultad, int movimientos[]){
    int error;
    error=creoEntorno(tablero1,tablero2,casVacios,dificultad);
    if(error!=0){
    	return error;
    }
    pongoFicha (tablero1,casVacios);//agrego primer ficha
    //modifico casVacios
    pongoFicha (tablero1,casVacios);//agrego segunda ficha

    movimientosValidos(*tablero1, movimientos);

    return 0;
}
/*Llama a las subfunciones que se encargan de realizar la jugada, devuelve si hubo un error y deja
** en parametros de salida si se gano o perdio la partida */
int jugar(sTablero * tablero1,sTablero * tablero2, sTablero * tableroAux,sCasVacios * casVacios, int * hiceUndo,int * gane, int * perdi,int movimientos[], int accion){
    int error=0;
        if(accion==UNDO){//hago undo
            if(tablero1->undos>0 && !*hiceUndo){
                undo (tablero2, tablero1, tableroAux);
                *hiceUndo=1;
                (casVacios->num)++;//vuelvo a agregar el casillro en el que puse la ficha en la jugada anterior
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
/*Guarada la partida en un archivo en el siguente orden: dificultad,puntaje,undos y fichas del tablero */
int guardar(const char fileName[], sTablero tablero){
   FILE * archivo;
   unsigned short int dificultad,i,j;
   if(tablero.dim==8){
       dificultad=1;
   }else if(tablero.dim==6){
       dificultad=2;
   }else{
       dificultad=3;
   }

   archivo=fopen(fileName,"w");

   if(archivo==NULL){
       //error de apertura de archivo
      return ERR_FILE;
   }

   fwrite(&dificultad,sizeof(dificultad),1,archivo);
   fwrite(&(tablero.puntaje),sizeof(tablero.puntaje),1,archivo);
   fwrite(&(tablero.undos),sizeof(tablero.undos),1,archivo);
    for(i=0;i<tablero.dim;i++){
        fwrite((tablero.matriz[i]),sizeof(tablero.matriz[0][0]),tablero.dim,archivo); 
    }

   fclose(archivo);

   return 0;
}

/*
*Esta funcion valida que el valor ingresado como paramaetro
*sea una potencia de 2, y que se encuentre entre 0 y el numero ganador.
*en caso de no ser valido devuelve la variable simbolica ERR_FILE_VALID
*sino devuelve 0
*/
int casilleroValido(int val, int numGanador){
    if(val==1){
        return ERR_FILE_VALID;
    }
    if(val>=0 && val<numGanador){
        while(val>1){
            if(val%2==0){
                val/=2;
            }else{
                return ERR_FILE_VALID;
            }
        }
    }else{
        return ERR_FILE_VALID;
    }
    return 0;
}

/*Valido el archivo cargado. Verificando que el puntaje sea mayor o igual a 0 y par,
** que la cantidad de undos sea mayor o igual a 0 y sea menor a lo pre-establecido 
** segun la dificultad y que las fichas sean potencias de dos.*/
int validoPartida(sTablero tablero){
    int i,j,error=0;
    if(tablero.puntaje<=0 || tablero.puntaje%2==1){//valido puntaje
        error=ERR_FILE_VALID;
    }
    if(!error && tablero.dim==8 && (tablero.undos>8 || tablero.undos<0)){//valido undos
        error=ERR_FILE_VALID;
    }else if(!error && tablero.dim==6 && (tablero.undos>4 || tablero.undos<0)){
        error=ERR_FILE_VALID;
    }else if(!error && tablero.dim==4 && (tablero.undos>2 || tablero.undos<0)){
        error=ERR_FILE_VALID;
    }
    for(i=0;i<tablero.dim && !error;i++){
        for(j=0;j<tablero.dim && !error;j++){
            error=casilleroValido(tablero.matriz[i][j],tablero.numGanador);
        }
    }
    return error;
}
/* Leo un archivo y lo valido. Creo el entorno de juego.
*/
int cargoPartida(sTablero * tablero1, sTablero * tablero2, sCasVacios * casVacios, int movimientos[], const char fileName[]){
    int error=0;
    FILE * archivo;
    unsigned short int dificultad,dim,puntaje,val,i,j;

    archivo=fopen(fileName,"r");
    if(archivo==NULL){
       //error de apertura de archivo
        return ERR_FILE;
    }

    fread(&dificultad,sizeof(unsigned short int),1,archivo);

    if(dificultad==1){
        dificultad=FACIL;
    }else if(dificultad==2){
        dificultad=INTERMEDIO;
    }else if(dificultad==3){
        dificultad=DIFICIL;
    }else{
        return ERR_FILE_VALID;
    }

    error=creoEntorno(tablero1,tablero2,casVacios,dificultad);//creo los tableros
    if(error!=0){
        return error;//si hay error termino la ejecucion
    }

    fread(&(tablero1->puntaje),sizeof(tablero1->puntaje),1,archivo);
    fread(&(tablero1->undos),sizeof(tablero1->undos),1,archivo);

    for(i=0;i<(tablero1->dim);i++){
        //for(j=0;j<(tablero1->dim);j++){
            fread((tablero1->matriz[i]),sizeof(tablero1->matriz[0][0]),tablero1->dim,archivo);
        //}
    }

    error=validoPartida(*tablero1);
    if(error!=0){
        return error;
    }

    movimientosValidos(*tablero1, movimientos);

    return 0;

}
