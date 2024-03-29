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
 * con el color especificado.
 *
 * @param x Coordenada x de la esquina superior izquierda del cuadrado.
 * @param y Coordenada y de la esquina superior izquierda del cuadrado.
 * @param rgb Color en para el relleno del cuadrado.
 *
 * @return void
 *
 * @see color, rectangulo_lleno
 */
void cuadrado(int x, int y, int rgb) {
    color(rgb);
    rectangulo_lleno(x * TAM,
                     y * TAM,
                     x * TAM + TAM,
                     y * TAM + TAM);
}

int main() {
    //Dibujamos el tablero de 10*20;
    vredimensiona(TAM * FILAS, TAM * COLUMNAS);
    //Ejemplos para probar la nueva función cuadrado
    cuadrado(0, 0, ROJO);
    cuadrado(1, 0, VERDE);
    cuadrado(2, 0, AMARILLO);
    cuadrado(0, 2, AZUL);

    for (int i = 0; i < 10; ++i) {
        cuadrado(9, i*2, BLANCO);
    }
    //Al pintar siempre refresca la pantalla
    refresca();
    return 0;
}
