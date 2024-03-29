#include "miniwin.h"

using namespace std;
using namespace miniwin;

/// Constantes:
// Tamaño Relativo de la ventana
const int TAM = 25;
// Numero de Columnas y Filas
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
    rectangulo_lleno(1 + x * TAM, // 1 + --> damos borde
                     1 + y * TAM,
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

    //Pintamos la pieza en (0,0)
    int x = 0, y = 0;
    pieza_s1(x, y);
    refresca();
    //Accionamos una tecla
    int t = tecla();
    //Pulsar ESCAPE para cerrar la ventana
    while (t != ESCAPE) {
        //Movemos la pieza
        if (t == ARRIBA) {
            y--;
        } else if (t == ABAJO) {
            y++;
        } else if (t == DERECHA) {
            x++;
        } else if (t == IZQUIERDA) {
            x--;
        }
        //Cuando aprietas una tecla, refrescas la pantalla
        //1. Borramos la pieza anterior
        //2. Dibujamos la nueva
        //3. Refrescamos la pantalla
        if(t != NINGUNA){
            borra();
            pieza_s1(x, y);
            refresca();
        }
        //Pulsamos una nueva tecla
        t = tecla();
    }
    vcierra();

    return 0;
}
