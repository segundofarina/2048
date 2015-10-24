#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "getnum.h"


#define IZQUIERDA 1
#define ARRIBA 2
#define DERECHA 3
#define ABAJO 4
#define UNDO 5
#define QUIT 6
#define SAVE 7

#define FACIL 4
#define INTERMEDIO 5
#define DIFICIL 6

#define ERR_UNDO -100
#define ERR_MOV -99
#define ERR_FORZADO -98
#define ERR_MEMORIA -97
#define ERR_COPIA -96

typedef struct{
    int inicio;
    int final;
    int incremento;

} sMovimiento;
typedef struct {
    int ** matriz;
    int dim;
    int undos;
    unsigned short int puntaje;
    int numGanador;
} sTablero;
typedef struct{
    int ** matriz;
    int num;
} sCasVacios;

int creoTablero (sTablero * tablero, int dim, int undos, int ganador);

int creoCasvacios (sCasVacios * casVacios, int dim);

int randInt(int inicio, int final);

int nuevaFicha();

int buscoCasillero(sCasVacios vacios, int * posI, int *posJ);

void pongoFicha(sTablero * nuevo, sCasVacios * vacios);

int sumoFila(sMovimiento I, sMovimiento J, sTablero m, sTablero * nueva, sCasVacios * vacios);

void descifroMovimiento (int direccion, sMovimiento * I, sMovimiento * J,int dim);

int muevoTablero(int direccion, sTablero viejo, sTablero * nuevo, sCasVacios * vacios);

void swapTableros (sTablero * tablero1, sTablero * tablero2, sTablero * aux);

void undo(sTablero * tablero1, sTablero * tablero2, sTablero * aux);

void movimientosValidos(sTablero tablero1, int movimientos[]);

int fperdi(int movimientos[], sTablero tablero);

int inicializo(sTablero * tablero1, sTablero * tablero2, sCasVacios * casVacios, int dificultad, int movimientos[]);

int jugar(sTablero * tablero1,sTablero * tablero2, sTablero * tableroAux,sCasVacios * casVacios, int * hiceUndo,int * gane, int * perdi,int movimientos[], int accion);

int guardar(char fileName[], sTablero tablero);

void cargarPartida(char fileName[],sTablero * tablero);