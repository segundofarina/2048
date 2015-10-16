typedef struct{
    int inicio;
    int final;
    int incremento;

} movimiento;



void sumoFila(movimiento I, movimiento J, const int m[][DIM], int nueva[][DIM]){
    int i,j,k=inicioI, h=inicioJ, sume=0;
    
    for( i=I.inicio, j=J.inicio; i!=I.final && j!=J.final; i+=I.incremento, j+=J.incremento ){//recorro la fila o la columna dependiendo de los paramentros
        if(m[i][j]!=0){//salteo casilleros vacios
            nueva[k][h]=m[i][j];//copio los casilleros llenos a la matriz nueva
            /*voy a fijarme si el numero que acabo de copiar se deberia sumar para combinar con el anterior de la fila-columna */
            if( (k!=I.inicio || h!=J.inicio) && !sume && nueva[k][h]==nueva[k-I.incremento][h-J.incremento]){//si no es el primer casillero de la fila-columna, y si no acabo de hacer una suma, verifico si el numero es igual al casillero anterior
                nueva[k-I.incremento][h-J.incremento]*=2;//casillero anterior por 2
                nueva[k][h]=0;//casillero actual vacio
                sume=1;//aviso que la proxima pasada no tengo que sumar
                /*IMPORTANTE no muevo el contador de la matriz nueva para volver al casillero que vacie*/
            }else{
                sume=0;//aviso que puedo sumar la proxima pasada
                k+=I.incremento;
                h+=J.incremento;
                /*si no sume si incremento los contadores de la matriz nueva*/
            }
        }
    }
}

