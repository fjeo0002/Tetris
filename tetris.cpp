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

    //n entre 0 y 3 (0 = absoluto, entre 1 y 3 = relativos)
    Coord posicionBloque(int n) const {
        if (n == 0) return abs;
        return {abs.x + relat[n - 1].x, abs.y + relat[n - 1].y};
    }
};

typedef int Tablero[COLUMNAS][FILAS];

void cuadrado(int x, int y) {
    rectangulo_lleno(1 + x * TAM,
                     1 + y * TAM,
                     x * TAM + TAM,
                     y * TAM + TAM);
}

void pinta_pieza(const Pieza &P) {
    color(P.color);
    cuadrado(P.abs.x, P.abs.y);
    for (int i = 0; i < 4; ++i) {
        Coord c = P.posicionBloque(i);
        cuadrado(c.x, c.y);
    }
}

Coord rota_derecha(const Coord &c) {
    Coord ret = {-c.y, c.x};
    return ret;
}

void rota_derecha(Pieza &P) {
    for (int i = 0; i < 3; ++i) {
        P.relat[i] = rota_derecha(P.relat[i]);
    }
}

Coord rota_izquierda(const Coord &c) {
    Coord ret = {c.y, -c.x};
    return ret;
}

void rota_izquierda(Pieza &P) {
    for (int i = 0; i < 3; ++i) {
        P.relat[i] = rota_izquierda(P.relat[i]);
    }
}

void vaciarTablero(Tablero &T) {
    for (int i = 0; i < COLUMNAS; ++i) {
        for (int j = 0; j < FILAS; ++j) {
            T[i][j] = NEGRO;
        }
    }
}

void actualizaTablero(const Tablero &T) {
    for (int i = 0; i < COLUMNAS; ++i) {
        for (int j = 0; j < FILAS; ++j) {
            color(T[i][j]);
            cuadrado(i, j);
        }
    }
}

void insertaPieza(Tablero &T, const Pieza &P) {
    for (int i = 0; i < 4; ++i) {
        Coord c = P.posicionBloque(i);
        T[c.x][c.y] = P.color;
    }
}

bool colisionPieza(const Tablero &T, const Pieza &P) {
    for (int i = 0; i < 4; ++i) {
        Coord c = P.posicionBloque(i);
        if (c.x < 0 || c.x >= COLUMNAS || c.y < 0 || c.y >= FILAS) {
            return true;
        } else if (T[c.x][c.y] != NEGRO) {
            return true;
        }
    }
    return false;

}

const Coord relats[7][3] = {
        {{1,0},{1,1},{0,1}}, // Cuadrado            ROJO
        {{1,0},{0,1},{-1,1}}, // S                  VERDE
        {{-1,0},{0,1},{1,1}}, // 2                  AZUL
        {{0,-1},{0,1},{1,1}}, // L                  AMARILLO
        {{0,-1},{0,1},{-1,1}}, // L invertida       MAGENTA
        {{0,-1},{0,1},{0,2}}, // Palo               CYAN
        {{-1,0},{0,1},{1,0}} // T                   BLANCO
};

void piezaNueva(Pieza& P){
    P.abs.x = 4;
    P.abs.y = 5;

    //Pieza al azar
    int r = rand() % 7;
    for (int i = 0; i < 3; ++i) {
        P.relat[i] = relats[r][i];
    }
    r++;
    P.color = r;
}

int main() {
    srand(time(nullptr));

    vredimensiona(TAM * COLUMNAS, TAM * FILAS);

    Tablero T;
    vaciarTablero(T);
    actualizaTablero(T);

    Pieza P;
    piezaNueva(P);
    pinta_pieza(P);
    refresca();

    int t = tecla();

    while (t != ESCAPE) {
        Pieza copia = P;

        if(t == ARRIBA) {
            rota_derecha(P);
        } else if(t == ABAJO) {
            P.abs.y++;
        } else if(t == IZQUIERDA) {
            P.abs.x--;
        } else if(t == DERECHA) {
            P.abs.x++;
        }

        if (colisionPieza(T, P)) {
            P = copia;
        }

        if(t == ESPACIO) {
            insertaPieza(T,P);
            piezaNueva(P);
        }

        if(t != NINGUNA){
            borra();
            actualizaTablero(T);
            pinta_pieza(P);
            refresca();
        }

        t = tecla();
    }

    vcierra();

    return 0;
}
