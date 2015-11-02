#ifndef _2048_header

#define _2048_header

#include <stdio.h>
#include <stdlib.h>

#include "getnum.h"


#define IZQUIERDA 1
#define ARRIBA 2
#define DERECHA 3
#define ABAJO 4
#define UNDO 5

#define FACIL 4
#define INTERMEDIO 5
#define DIFICIL 6

#define GANE 1
#define PERDI 0
#define SIGO 3


/*
**Defino variables simbolicas con numeros negativos comenzando del -100 para indicar
**los distintos tipos de errores.
*/
#define ERR_UNDO -100       /*El jugador se ha quedado sin undos para realizar
                            **o realizo un undo en la jugaad anterior*/
#define ERR_FORZADO -98     /* No existen movimientos en ninguna direccion y solo se
                            **pueden realizar undos*/
#define ERR_MEMORIA -97     /*No hay memoria suficiente para inicializar la partida*/
#define ERR_FILE -96        /*No se ha podido leer el archivo correctamente*/
#define ERR_SAVE -95        /*No se ha podido escirbir el archivo correctamente*/
#define ERR_FILE_VALID -94  /*El archivo ingresado no tiene un formato valido*/

/*
** Defino la estructura sMovimiento que almacenara, segun el movimiento ingresado,
** los valores de inicio, final e incremento que utilizara para recorrer la matriz
** en ese sentido.
*/
typedef struct{             
    int inicio;
    int final;
    int incremento;

} sMovimiento;

/*
** Defino la estructura sTablero que alamacenara el tablero, su dimension,
** la cantidad
** de undos restantes, el puntaje y el numero con el que se gana el juego.
*/

typedef struct {
    unsigned short int ** matriz;
    int dim;
    unsigned short int undos;
    unsigned int puntaje;
    int numGanador;
} sTablero;

/*
** Defino la estructura sCasVacios que permite almacenar las posiciones del tablero
** que se encuentran vacias (con ceros). Estas se guardan en una matriz de 2x(dim*dim)
** siendo dim la dimension del tablero. Num sera la cantidad de espacios vaios que 
** tengo luego de cada jugada.
*/

typedef struct{
    int ** matriz;
    int num;
} sCasVacios;



/*
** La funcion incializo se llama al incio del programa y se encarga de crear los tableros,
** y la matriz de casilleros vacios, segun la dificultad ingresada. Y luego pone dos fichas
** en el tablero. Devuelve 0 si no hubieron errores o el codigo del error.
*/
int inicializo(sTablero * tablero1, sTablero * tablero2, sCasVacios * casVacios, int dificultad, int movimientos[]);


/*
** Jugar controla el flujo del juego, realizando la accion ingresada por el usuario. 
*/
int jugar(sTablero * tablero1,sTablero * tablero2, sTablero * tableroAux,sCasVacios * casVacios, int * hiceUndo,int * estado,
   int movimientos[], int accion);

/*
** La funcion guardar almacena la partida en el archivo pasado por parametro
*/
int guardar(const char fileName[], sTablero tablero);

/*
** La funcion cargoPartida lee el archivo pasado por parametro y de acuerdo a
** la informacion que contiene crea el entorno de juego y completa los tableros
** con la informacion del archivo. 
*/
int cargoPartida(sTablero * tablero1, sTablero * tablero2, sCasVacios * casVacios, int movimientos[],const char fileName[]);

/*
** liberoPartida libera los elementos dinamicos una vez terminada la partida
*/
void liberoPartida(sTablero tablero1,sTablero tablero2,sCasVacios casVacios);

#endif
