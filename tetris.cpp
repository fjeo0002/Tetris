#include "miniwin.h"

using namespace std;
using namespace miniwin;

const int TAM = 25;
const int COLUMNAS = 20;
const int FILAS = 10;

/**
 * @brief Dibuja un cuadrado relleno en la pantalla.
 *
 * @post Esta función dibuja un cuadrado relleno en la posición especificada
 *
 * @param x Coordenada x de la esquina superior izquierda del cuadrado.
 * @param y Coordenada y de la esquina superior izquierda del cuadrado.
 *
 * @return void
 *
 * @see color, rectangulo_lleno
 */
void cuadrado(int x, int y) {
    rectangulo_lleno(x * TAM,
                     y * TAM,
                     x * TAM + TAM,
                     y * TAM + TAM);
}

/**
 * @brief Dibuja una pieza tipo S1 en la pantalla.
 *
 * Esta función dibuja una pieza tipo S1 compuesta por cuatro cuadrados
 * de color rojo en la posición especificada.
 *
 * @param x Coordenada x del punto de inicio de la pieza.
 * @param y Coordenada y del punto de inicio de la pieza.
 *
 * @return void
 *
 * @see color, cuadrado
 */
void pieza_s1(int x, int y) {
    color(ROJO);
    cuadrado(x, y);
    cuadrado(x + 1, y);
    cuadrado(x + 1, y + 1);
    cuadrado(x + 2, y + 1);
}

int main() {
    //Dibujamos el tablero de 10*20;
    vredimensiona(TAM * FILAS, TAM * COLUMNAS);
    //Ejemplos para probar la nueva función pieza_s1
    pieza_s1(5,5);
    pieza_s1(3,10);

    //Al pintar siempre refresca la pantalla
    refresca();
    return 0;
}
