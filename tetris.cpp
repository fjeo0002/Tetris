#include "miniwin.h"

using namespace std;
using namespace miniwin;


const int TAM = 25;
const int FILAS = 20;
const int COLUMNAS = 10;

struct Coord {
    int x;
    int y;
};

struct Pieza {
    Coord abs;
    Coord relat[3];
    int color;
};

typedef int Tablero[COLUMNAS][FILAS];

void cuadrado(int x, int y) {
    rectangulo_lleno(1 + x * TAM,
                     1 + y * TAM,
                     x * TAM + TAM,
                     y * TAM + TAM);
}

void pinta_pieza(const Pieza& P) {
    color(P.color);
    cuadrado(P.abs.x, P.abs.y);
    for (int i = 0; i < 3; ++i) {
        cuadrado(P.abs.x + P.relat[i].x,
                 P.abs.y + P.relat[i].y);
    }
}

Coord rota_derecha(const Coord& c){
    Coord ret = {-c.y, c.x};
    return ret;
}

void rota_derecha(Pieza& P) {
    for (int i = 0; i < 3; ++i) {
        P.relat[i] = rota_derecha(P.relat[i]);
    }
}

Coord rota_izquierda(const Coord& c){
    Coord ret = {c.y, -c.x};
    return ret;
}

void rota_izquierda(Pieza& P) {
    for (int i = 0; i < 3; ++i) {
        P.relat[i] = rota_izquierda(P.relat[i]);
    }
}

void vaciarTablero(Tablero& T) {
    for (int i = 0; i < COLUMNAS; ++i) {
        for (int j = 0; j < FILAS; ++j) {
            T[i][j] = NEGRO;
        }
    }
}

void actualizaTablero(const Tablero& T) {
    for (int i = 0; i < COLUMNAS; ++i) {
        for (int j = 0; j < FILAS; ++j) {
            color(T[i][j]);
            cuadrado(i, j);
        }
    }
}

void insertaPieza(Tablero& T, const Pieza& P) {
    T[P.abs.x][P.abs.y] = P.color;
    for (int i = 0; i < 3; ++i) {
        T[P.abs.x + P.relat[i].x][P.abs.y + P.relat[i].y] = P.color;
    }
}

int main() {
    vredimensiona(TAM * COLUMNAS, TAM * FILAS);

    Pieza P = {{4,8}, {{1,0},{1,1},{0,1}}, MAGENTA};

    Tablero T;
    vaciarTablero(T);
    insertaPieza(T,P);
    borra();
    actualizaTablero(T);
    refresca();

    return 0;
}
