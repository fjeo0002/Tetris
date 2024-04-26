/**
 * @file tetris.cpp
 * @brief Juego de Tetris
 *
 * Este archivo contiene la implementación del juego Tetris.
 * El juego se juega en un tablero de 20 filas y 10 columnas.
 * Cada pieza del juego está compuesta por 4 bloques.
 * Las piezas pueden rotarse y moverse horizontalmente.
 * El objetivo del juego es completar filas para obtener puntos.
 *
 * @author Francisco José Escabias Ortega
 * @date 26/04/2024
 *
 * @see Para más información, puedes visitar https://github.com/fjeo0002/Tetris
 */

#include "miniwin.h"
#include <Windows.h>
#include <iostream>
#include <mmsystem.h>

using namespace std;
using namespace miniwin;


const int TAM = 25; ///< Tamaño de los bloques del juego
const int FILAS = 20; ///< Número de filas en el tablero del juego
const int COLUMNAS = 10; ///< Número de columnas en el tablero del juego
const int MARGEN = 10; ///< Margen alrededor del tablero del juego
const int ANCHO = TAM * COLUMNAS; ///< Ancho del tablero del juego
const int ALTO = TAM * FILAS; ///< Altura del tablero del juego

/** @struct Coord
 *  @brief Estructura para almacenar las coordenadas x e y.
 */
struct Coord {
    int x; ///< Coordenada x
    int y; ///< Coordenada y
};

/** @struct Pieza
 *  @brief Estructura para representar una pieza del juego.
 */
struct Pieza {
    Coord abs; ///< Coordenadas absolutas de la pieza
    Coord relat[3]; ///< Coordenadas relativas de la pieza
    int color; ///< Color de la pieza

    /** @brief Obtiene la posición del bloque.
     *  @param n Índice del bloque (0 = absoluto, entre 1 y 3 = relativos)
     *  @return Coordenadas del bloque.
     */
    Coord posicionBloque(int n) const {
        if (n == 0) return abs;
        return {abs.x + relat[n - 1].x, abs.y + relat[n - 1].y};
    }
};

typedef int Tablero[COLUMNAS][FILAS]; ///< Tablero del juego

/**
 * @brief Dibuja un cuadrado en las coordenadas dadas.
 * @param x Coordenada x del cuadrado.
 * @param y Coordenada y del cuadrado.
 */
void cuadrado(int x, int y) {
    rectangulo_lleno(MARGEN + 1 + x * TAM,
                     MARGEN + 1 + y * TAM,
                     MARGEN + x * TAM + TAM,
                     MARGEN + y * TAM + TAM);
}

/**
 * @brief Dibuja una pieza en el tablero del juego.
 * @param P Pieza a dibujar.
 */
void pinta_pieza(const Pieza &P) {
    color(P.color);
    cuadrado(P.abs.x, P.abs.y);
    for (int i = 0; i < 4; ++i) {
        Coord c = P.posicionBloque(i);
        cuadrado(c.x, c.y);
    }
}

/**
 * @brief Rota una coordenada en sentido horario.
 * @param c Coordenada a rotar.
 * @return Nueva coordenada rotada en sentido horario de la entrada.
 */
Coord rota_derecha(const Coord &c) {
    Coord ret = {-c.y, c.x};
    return ret;
}

/**
 * @brief Rota una pieza en sentido horario.
 * @param P Pieza a rotar.
 */
void rota_derecha(Pieza &P) {
    if (P.color == ROJO) return;
    for (int i = 0; i < 3; ++i) {
        P.relat[i] = rota_derecha(P.relat[i]);
    }
}

/**
 * @brief Rota una coordenada en sentido antihorario.
 * @param c Coordenada a rotar.
 * @return Nueva coordenada rotada en sentido antihorario de la entrada.
 */
Coord rota_izquierda(const Coord &c) {
    Coord ret = {c.y, -c.x};
    return ret;
}

/**
 * @brief Rota una pieza en sentido antihorario.
 * @param P Pieza a rotar.
 */
void rota_izquierda(Pieza &P) {
    if (P.color == ROJO) return;
    for (int i = 0; i < 3; ++i) {
        P.relat[i] = rota_izquierda(P.relat[i]);
    }
}

/**
 * @brief Vacía el Tablero de juego.
 * @post Establece todas sus celdas a NEGRO
 * @param T Tablero
 */
void vaciarTablero(Tablero &T) {
    for (int i = 0; i < COLUMNAS; ++i) {
        for (int j = 0; j < FILAS; ++j) {
            T[i][j] = NEGRO;
        }
    }
}

/**
 * @brief Actualiza el tablero del juego.
 * @post Aplica color correspondiente y dibuja un cuadrado en cada celda
 * @param T Tablero del juego
 */
void actualizaTablero(const Tablero &T) {
    for (int i = 0; i < COLUMNAS; ++i) {
        for (int j = 0; j < FILAS; ++j) {
            color(T[i][j]);
            cuadrado(i, j);
        }
    }
}

/**
 * @brief Inserta una pieza en el Tablero de juego
 * @post Coloca el color de la pieza en la posición correspondiente del Tablero.
 * @param T Tablero del juego
 * @param P Pieza a insertar
 */
void insertaPieza(Tablero &T, const Pieza &P) {
    for (int i = 0; i < 4; ++i) {
        Coord c = P.posicionBloque(i);
        T[c.x][c.y] = P.color;
    }
}

/**
 * @brief Comprueba si hay colisión entre una pieza y la celda del Tablero
 * @post Recorre cada bloque de la pieza y verifica
 *        -> la posición del bloque está fuera de los límites del tablero
 *        -> la posición del bloque en el tablero no es NEGRO
 * @param T Tablero del juego
 * @param P Pieza que pueda colisionar
 * @return bool -> true: la pieza colisiona en el tablero
 *              -> false: caso contrario.
 */
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

/**
 * @brief Comprueba si una fila del Tablero esta llena.
 * @post Recorre cada celda de la fila y comprueba que esté vacía
 * @param T Tablero del juego
 * @param fila Número de la fila que se va a verificar.
 * @return bool -> true: fila llena
 *              -> false: caso contrario.
 */
bool filaLlena(const Tablero &T, int fila) {
    for (int i = 0; i < COLUMNAS; ++i) {
        if (T[i][fila] == NEGRO) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Quita una fila del Tablero.
 * @post Desplaza todas las filas superiores a la indicada hacia abajo
 * @post Establece todas las celdas de la fila superior a NEGRO
 * @param T Tablero del juego
 * @param fila Número de la fila que se va a quitar.
 */
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

/**
 * @brief Cuenta y Quita las filas del Tablero que están llenas.
 * @param T Tablero del juego
 * @return int -> Cantidad de filas quitadas
 */
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

/**
 * @brief Coordenadas relativas para las diferentes Piezas
 * @post Las formas de las piezas son las siguientes:
 * - Cuadrado (ROJO)
 * - S (VERDE)
 * - 2 (AZUL)
 * - L (AMARILLO)
 * - L invertida (MAGENTA)
 * - Palo (CYAN)
 * - T (BLANCO)
 * @param RELATIVOS Matriz de coordenadas relativas para las diferentes formas de piezas.
 */
const Coord RELATIVOS[7][3] = {
        {{1,  0},  {1, 1}, {0,  1}},
        {{1,  0},  {0, 1}, {-1, 1}},
        {{-1, 0},  {0, 1}, {1,  1}},
        {{0,  -1}, {0, 1}, {1,  1}},
        {{0,  -1}, {0, 1}, {-1, 1}},
        {{0,  -1}, {0, 1}, {0,  2}},
        {{-1, 0},  {0, 1}, {1,  0}}
};

/**
 * @brief Crea una nueva pieza
 * @post Crea una nueva pieza al azar según los RELATIVOS
 * @param P Pieza a crear
 */
void pieza_nueva(Pieza &P) {
    P.abs.x = 13;
    P.abs.y = 3;

    //Pieza al azar
    int r = rand() % 7;
    for (int i = 0; i < 3; ++i) {
        P.relat[i] = RELATIVOS[r][i];
    }
    r++;
    P.color = r;
}

/**
 * @brief Dibuja la interfaz del juego Tetris.
 * @post Borra el tablero, actualiza el estado del mismo junto con Información de juego
 * @param T Tablero del juego
 * @param P Pieza actual en juego
 * @param N Siguiente Pieza en el juego.
 * @param ptos Puntos actuales del jugador.
 * @param level Nivel actual del juego.
 */
void pintarInterfaz(const Tablero &T, const Pieza &P, const Pieza &N, int ptos, int level) {
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

/**
 * @brief Finaliza la partida y muestra un mensaje
 * @post Borra el tablero con Animación y muestra el mensaje correspondiente
 * @param mensaje string: Mensaje a mostrar al final de la partida
 */
void finPartida(string mensaje) {
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS; ++j) {
            color(BLANCO);
            cuadrado(j, i);
            espera(1);
            refresca();
        }
    }

    color(AZUL);
    texto(COLUMNAS * TAM / 2 - 10 - 15, FILAS * TAM / 2 - 10, mensaje);
    refresca();
}

/**
 * @brief Dibuja la letra "T" con rectángulos de colores.
 * @post Presentacion visual del título del juego TETRIS
 * @param x Coordenada x de la letra
 * @param y Coordenada y de la letra
 * @param c Color de la letra
 */
void dibujaT(int x, int y, int c) {
    color(c);
    rectangulo_lleno(x, y, x + 125, y + 25); // Parte superior de la T
    rectangulo_lleno(x + 50, y, x + 75, y + 125); // Parte inferior de la T
}

/**
 * @brief Dibuja la letra "E" con rectángulos de colores.
 * @post Presentacion visual del título del juego TETRIS
 * @param x Coordenada x de la letra
 * @param y Coordenada y de la letra
 * @param c Color de la letra
 */
void dibujaE(int x, int y, int c) {
    color(c);
    rectangulo_lleno(x, y, x + 125, y + 25); // Parte superior de la E
    rectangulo_lleno(x, y + 50, x + 100, y + 75); // Parte media de la E
    rectangulo_lleno(x, y + 100, x + 125, y + 125); // Parte inferior de la E
    rectangulo_lleno(x, y, x + 25, y + 125); // Parte izquierda de la E
}

/**
 * @brief Dibuja la letra "R" con rectángulos de colores.
 * @post Presentacion visual del título del juego TETRIS
 * @param x Coordenada x de la letra
 * @param y Coordenada y de la letra
 * @param c Color de la letra
 */
void dibujaR(int x, int y, int c) {
    color(c);
    rectangulo_lleno(x, y, x + 125, y + 25); // Parte superior de la R
    rectangulo_lleno(x, y, x + 25, y + 125); // Parte izquierda de la R
    rectangulo_lleno(x, y + 50, x + 125, y + 75); // Parte media de la R
    rectangulo_lleno(x + 100, y, x + 125, y + 75); // Parte derecha de la R
    for (float i = -6; i <= 6; i += 0.1) { // Parte cabo de la R
        linea(x + 25 - i, y + 75 + i, x + 120 - i, y + 120 + i);
    }
}

/**
 * @brief Dibuja la letra "I" con rectángulos de colores.
 * @post Presentacion visual del título del juego TETRIS
 * @param x Coordenada x de la letra
 * @param y Coordenada y de la letra
 * @param c Color de la letra
 */
void dibujaI(int x, int y, int c) {
    color(c);
    rectangulo_lleno(x, y, x + 25, y + 125); // Letra I
}

/**
 * @brief Dibuja la letra "S" con rectángulos de colores.
 * @post Presentacion visual del título del juego TETRIS
 * @param x Coordenada x de la letra
 * @param y Coordenada y de la letra
 * @param c Color de la letra
 */
void dibujaS(int x, int y, int c) {
    color(c);
    rectangulo_lleno(x, y, x + 125, y + 25); // Parte superior de la S
    rectangulo_lleno(x, y + 50, x + 125, y + 75); // Parte media de la S
    rectangulo_lleno(x, y + 100, x + 125, y + 125); // Parte inferior de la S
    rectangulo_lleno(x, y, x + 25, y + 75); // Parte izquierda de la S
    rectangulo_lleno(x + 100, y + 50, x + 125, y + 125); // Parte derecha de la S
}

/**
 * @brief Dibuja el título "TETRIS" utilizando letras individuales.
 * @post Presentacion visual del título del juego TETRIS
 * Las letras se dibujan en las siguientes posiciones:
 * - "T" en la posición (20, 20) con el color ROJO.
 * - "E" en la posición (150, 20) con el color VERDE.
 * - "T" en la posición (280, 20) con el color BLANCO.
 * - "R" en la posición (410, 20) con el color AZUL.
 * - "I" en la posición (540, 20) con el color AMARILLO.
 * - "S" en la posición (570, 20) con el color MAGENTA.
 */
void dibujaTetris() {
    dibujaT(20, 20, ROJO);
    dibujaE(150, 20, VERDE);
    dibujaT(280, 20, BLANCO);
    dibujaR(410, 20, AZUL);
    dibujaI(540, 20, AMARILLO);
    dibujaS(570, 20, MAGENTA);
}

/**
 * @brief Dibuja los botones "Yes" y "No" en la ventana.
 * @post Cada botón se representa como un rectángulo con un texto en su centro.
 * Los botones se dibujan en las siguientes posiciones y dimensiones:
 * - "Yes": Rectángulo desde (275, 180) hasta (350, 205) con el texto centrado en (295, 185).
 * - "No": Rectángulo desde (355, 180) hasta (430, 205) con el texto centrado en (375, 185).
 */
void dibujaBotones() {
    rectangulo(275, 180, 350, 205);
    texto(295, 185, "Yes");
    rectangulo(355, 180, 430, 205);
    texto(375, 185, "No");
}

/**
 * @brief Verifica si el usuario hizo clic en un botón.
 * @param x Coordenada x del clic.
 * @param y Coordenada y del clic.
 * @return bool -> true: clic en botón "Yes"
 *              -> false: clic en botón "No"
 */
bool clicEnBoton(int x, int y) {
    if (x >= 275 && y >= 180 && x <= 350 && y <= 205) {
        return true;
    } else if (x >= 355 && y >= 180 && x <= 430 && y <= 205) {
        exit(0);
    }
    return false;
}

/**
 * @brief Verifica si el usuario quiere jugar de nuevo.
 * @post Unificacion de Título TETRIS, botones "Yes" y "No", y pregunta de confirmación
 * @return bool -> true: clic en botón "Yes"
 *              -> false: clic en botón "No"
 */
bool jugarOtraVez() {
    vredimensiona(715, 250); // Define las dimensiones de la ventana
    color(BLANCO);
    rectangulo(10, 10, 705, 240);

    dibujaTetris();
    color(BLANCO);
    texto(265, 160, "Do you want to play Tetris?");
    dibujaBotones();
    refresca();

    // Espera a que el usuario haga clic en uno de los botones
    bool clic_realizado = false;
    while (!clic_realizado) {
        if (raton_boton_izq()) { // Verifica si se ha presionado el botón izquierdo del ratón
            int x = raton_x(); // Obtiene la coordenada x del clic
            int y = raton_y(); // Obtiene la coordenada y del clic
            clic_realizado = clicEnBoton(x, y);
        }
    }
    return clic_realizado;
}
/**
 * @brief Puntos necesarios para alcanzar cada nivel
 * @post Los puntos necesarios para alcanzar cada nivel son:
 * - Nivel 1: 0 puntos
 * - Nivel 2: 100 puntos
 * - Nivel 3: 300 puntos
 * - Nivel 4: 600 puntos
 * - Nivel 5: 1000 puntos
 * - Nivel 6: 1500 puntos
 * - Nivel 7: 2000 puntos
 */
const int PUNTOS_NIVEL[7] = {0, 100, 300, 600, 1000, 1500, 2000};

/**
 * @brief Velocidad de caída de la pieza en cada nivel
 * @post La velocidad de caída de las piezas para cada nivel es:
 * - Nivel 1: 30 milisegundos por posición
 * - Nivel 2: 25 milisegundos por posición
 * - Nivel 3: 20 milisegundos por posición
 * - Nivel 4: 15 milisegundos por posición
 * - Nivel 5: 10 milisegundos por posición
 * - Nivel 6: 5 milisegundos por posición
 * - Nivel 7: 1 milisegundo por posición
 */
const int VELOCIDAD_NIVEL[7] = {30, 25, 20, 15, 10, 5, 1};

/**
 * @brief Funcion principal y control del juego Tetris
 * @post El juego se repite hasta que el usuario haga clic en el botón "No"
 * @return bool -> true: clic en botón "No"
 */
int main() {
    srand(time(nullptr));

//Bucle Principal de Aplicacion
JugarOtraVez:
    //Música para Ventana de Inicio
    PlaySound(TEXT("../title.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    if (!jugarOtraVez()) {

    }
    //Música para Juego
    PlaySound(TEXT("../tetris.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    //Redimensiona la ventana de juego
    vredimensiona(MARGEN * 20 + ANCHO, MARGEN * 2 + ALTO);

    //Variables necesarias para el juego
    Tablero T;
    vaciarTablero(T);

    Pieza P;
    pieza_nueva(P);
    P.abs.x = 4;
    P.abs.y = 1;

    Pieza N;
    pieza_nueva(N);

    int ptos = 0;
    int level = 1;
    int frame = 0;

    // Dibuja la interfaz gráfica inicial del juego
    pintarInterfaz(T, P, N, ptos, level);

    // Obtiene la tecla presionada por el jugador
    int t = tecla();

    //Bucle Principal de Juego
    while (t != ESCAPE) {
        Pieza copia = P;

        // Si el jugador alcanza el nivel máximo, gana el juego
        if (level == sizeof(VELOCIDAD_NIVEL) / sizeof(VELOCIDAD_NIVEL[0])) {
            PlaySound(TEXT("../you_win.wav"), NULL, SND_FILENAME | SND_ASYNC);
            finPartida("YOU WIN!");
            while (t != ESCAPE && t != ESPACIO) t = tecla();
            goto JugarOtraVez; // Sale del bucle de juego y pregunta si se desea jugar otra vez
        }

        // Si ha pasado el tiempo necesario, la pieza cae automáticamente
        if (t == NINGUNA && frame > VELOCIDAD_NIVEL[level - 1]) {
            frame = 0;
            t = ABAJO;
        }

        // Actualiza la posición de la pieza según la tecla presionada por el jugador
        if (t == ARRIBA || t == int('Z')) {
            rota_derecha(P);
        } else if (t == int('X')) {
            rota_izquierda(P);
        } else if (t == ABAJO) {
            P.abs.y++;
        } else if (t == IZQUIERDA) {
            P.abs.x--;
        } else if (t == DERECHA) {
            P.abs.x++;
        }

        // Si la pieza colisiona con el tablero, se restaura su posición original
        if (colisionPieza(T, P)) {
            P = copia;

            // Si la colisión es hacia abajo, la pieza se inserta en el tablero
            if (t == ABAJO) {
                insertaPieza(T, P);

                // Se cuentan y eliminan las filas llenas, y se actualizan los puntos y nivel
                int cont = cuentaFila(T);
                switch (cont) {
                    case 1:
                        ptos += 100;
                        break;
                    case 2:
                        ptos += 300;
                        break;
                    case 3:
                        ptos += 500;
                        break;
                    case 4:
                        ptos += 800;
                        break;
                }
                if (PUNTOS_NIVEL[level] <= ptos) {
                    level++;
                }

                // Se obtiene una nueva pieza para continuar el juego
                P = N;
                pieza_nueva(N);
                P.abs.x = 4;
                P.abs.y = 1;

                // Si la nueva pieza colisiona con el tablero, el jugador pierde
                if (colisionPieza(T, P)) {
                    PlaySound(TEXT("../game_over.wav"), NULL, SND_FILENAME | SND_ASYNC);
                    finPartida("GAME OVER");
                    while (t != ESCAPE && t != ESPACIO) t = tecla();
                    goto JugarOtraVez; // Sale del bucle de juego y pregunta si se desea jugar otra vez
                }
            }
        }

        // Si se presiona alguna tecla, se actualiza la interfaz gráfica del juego
        if (t != NINGUNA) {
            pintarInterfaz(T, P, N, ptos, level);
        }

        espera(30); // Espera 30 milisegundos entre cada iteración del bucle
        frame++; // Incrementa el contador de frames
        t = tecla(); // Obtiene la tecla presionada por el jugador
    }

    vcierra(); // Cierra la ventana de juego y termina el programa

    return 0;
}
