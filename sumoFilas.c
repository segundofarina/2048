void sumoFila(int inicioI, int inicioJ, int finalI, int finalJ, int incrementoI, int incrementoJ, int m[][DIM], int nueva[][DIM]){
    int i,j,k=inicioI, h=inicioJ, sume=0;
    
    for( i=inicioI, j=inicioJ; i!=finalI && j!=finalJ; i+=incrementoI, j+=incrementoJ ){
        if(m[i][j]!=0){
            nueva[k][h]=m[i][j];
            if( (k!=inicioI || h!=inicioJ) && !sume && nueva[k][h]==nueva[k-incrementoI][h-incrementoJ]){
                nueva[k-incrementoI][h-incrementoJ]*=2;
                nueva[k][h]=0;
                sume=1;
            }else{
                sume=0;
                k+=incrementoI;
                h+=incrementoJ;
            }
        }
    }
}
