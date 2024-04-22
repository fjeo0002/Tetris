#include "miniwin.h"

using namespace std;
using namespace miniwin;


const int TAM = 25;
const int FILAS = 20;
const int COLUMNAS = 10;
const int MARGEN = 10;
const int ANCHO = TAM * COLUMNAS;
const int ALTO = TAM * FILAS;

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
    rectangulo_lleno(MARGEN + 1 + x * TAM,
                     MARGEN + 1 + y * TAM,
                     MARGEN + x * TAM + TAM,
                     MARGEN + y * TAM + TAM);
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

bool filaLlena(const Tablero &T, int fila) {
    for (int i = 0; i < COLUMNAS; ++i) {
        if (T[i][fila] == NEGRO) {
            return false;
        }
    }
    return true;
}

void quitarFila(Tablero &T, int fila) {
    for (int j = fila; j > 0; --j) {
        for (int i = 0; i < COLUMNAS; ++i) {
            T[i][j] = T[i][j - 1];
        }
    }
    for (int i = 0; i < COLUMNAS; ++i) {
        T[i][0] = NEGRO;
    }
}

int cuentaFila(Tablero &T) {
    int fila = FILAS - 1;
    int cont = 0;
    while (fila >= 0) {
        if (filaLlena(T, fila)) {
            quitarFila(T, fila);
            ++cont;
        } else {
            --fila;
        }
    }
    return cont;
}

const Coord relats[7][3] = {
        {{1,  0},  {1, 1}, {0,  1}}, // Cuadrado            ROJO
        {{1,  0},  {0, 1}, {-1, 1}}, // S                  VERDE
        {{-1, 0},  {0, 1}, {1,  1}}, // 2                  AZUL
        {{0,  -1}, {0, 1}, {1,  1}}, // L                  AMARILLO
        {{0,  -1}, {0, 1}, {-1, 1}}, // L invertida       MAGENTA
        {{0,  -1}, {0, 1}, {0,  2}}, // Palo               CYAN
        {{-1, 0},  {0, 1}, {1,  0}} // T                   BLANCO
};

void piezaNueva(Pieza &P) {
    P.abs.x = 13;
    P.abs.y = 3;

    //Pieza al azar
    int r = rand() % 7;
    for (int i = 0; i < 3; ++i) {
        P.relat[i] = relats[r][i];
    }
    r++;
    P.color = r;
}

void repinta(const Tablero &T, const Pieza &P, const Pieza &N, int ptos, int level) {
    borra();
    actualizaTablero(T);

    color(BLANCO);
    linea(MARGEN + 0, MARGEN + 0, MARGEN + 0, MARGEN + ALTO);
    linea(MARGEN + 0, MARGEN + ALTO, MARGEN + ANCHO, MARGEN + ALTO);
    linea(MARGEN + ANCHO, MARGEN + 0, MARGEN + ANCHO, MARGEN + ALTO);
    linea(MARGEN + 0, MARGEN + 0, MARGEN + ANCHO, MARGEN + 0);

    texto(MARGEN * 2 + TAM * COLUMNAS, MARGEN * 3, "Pieza Siguiente:");

    texto(MARGEN * 2 + TAM * COLUMNAS, MARGEN * 20, "Puntos: " + to_string(ptos));

    texto(MARGEN * 2 + TAM * COLUMNAS, MARGEN * 30, "Nivel: " + to_string(level));

    pinta_pieza(P);
    pinta_pieza(N);
    refresca();
}

const int NIVELES[7] = {0, 100, 300, 600, 1000, 1500, 2000};

const int VELOCIDAD[7] = {30,25,20,15,10,5,1};

int main() {
    srand(time(nullptr));

    vredimensiona(MARGEN * 20 + ANCHO, MARGEN * 2 + ALTO);

    Tablero T;
    vaciarTablero(T);

    Pieza P;
    piezaNueva(P);
    P.abs.x = 4;
    P.abs.y = 1;

    Pieza N;
    piezaNueva(N);

    int ptos = 0;
    int level = 1;

    repinta(T, P, N, ptos, level);

    int t = tecla();
    int frame = 0;

    while (t != ESCAPE) {
        Pieza copia = P;

        if (t == NINGUNA && frame > VELOCIDAD[level-1]) {
            frame = 0;
            t = ABAJO;
        }

        if (t == ARRIBA) {
            rota_derecha(P);
        } else if (t == ABAJO) {
            P.abs.y++;
        } else if (t == IZQUIERDA) {
            P.abs.x--;
        } else if (t == DERECHA) {
            P.abs.x++;
        }

        if (colisionPieza(T, P)) {
            P = copia;
            if (t == ABAJO) {
                insertaPieza(T, P);
                int cont = cuentaFila(T);
                switch (cont) {
                    case 1: ptos += 100; break;
                    case 2: ptos += 300; break;
                    case 3: ptos += 500; break;
                    case 4: ptos += 800; break;
                }
                if(NIVELES[level] <= ptos){
                    level++;
                }
                P = N;
                piezaNueva(N);
                P.abs.x = 4;
                P.abs.y = 1;

                if(colisionPieza(T, P)){
                    mensaje("GAME OVER");
                    espera(UINT_MAX);
                }
            }
        }

        if (t != NINGUNA) {
            repinta(T, P, N, ptos, level);
        }

        espera(30);
        frame++;
        t = tecla();
    }

    vcierra();

    return 0;
}
