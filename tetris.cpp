#include "miniwin.h"

using namespace std;
using namespace miniwin;


const int TAM = 25;
const int COLUMNAS = 20;
const int FILAS = 10;

struct Coord {
    int x;
    int y;
};


struct Pieza {
    Coord abs;
    Coord relat[3];
};

void cuadrado(int x, int y) {
    rectangulo_lleno(1 + x * TAM,
                     1 + y * TAM,
                     x * TAM + TAM,
                     y * TAM + TAM);
}

void pinta_pieza(const Pieza& P) {
    color(ROJO);
    cuadrado(P.abs.x, P.abs.y);
    for (int i = 0; i < 3; ++i) {
        cuadrado(P.abs.x + P.relat[i].x,
                 P.abs.y + P.relat[i].y);
    }
}

int main() {
    vredimensiona(TAM * FILAS, TAM * COLUMNAS);

    Pieza s = { {2,2}, {{-1, -1}, {0, -1}, {1, 0}} };
    Pieza l = { {2,8}, {{-1, 0}, {-1, -1}, {1, 0}} };

    pinta_pieza(s);
    pinta_pieza(l);
    refresca();
    
    return 0;
}
