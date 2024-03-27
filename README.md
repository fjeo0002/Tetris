# Proyecto Elegido: Tetris 2D

![](https://lh7-us.googleusercontent.com/JBGxV5CSRY-HfPgkmYAAUK0BzjGBoU78BB4QCqz_8xjSVFqHHhJnHa_ouQWtbc_63sj4HVOk2ZGx8nuNRIVEeOMkzukhG8nHWJL6hYcNfvyROOpsMRPXHDfGtKTFHI1xnyoiCg8d3EZsxtbC7nVhqZY)

## Autor del proyecto: Francisco José Escabias Ortega

### Proyecto elegido: Videojuego Tetris 2D

![](https://lh7-us.googleusercontent.com/99j0t7TTd7fWpf7FKoByWvA4EBR6Dqkte2yDrFnlKfvyA7Fxtsdu3Fn8xHtOlff9PzIQAYK8xYteTopSeg_TODsCfDNZnptHGrkq2vOTlEeMNttPD9FFq-zodieAA7lDsyQgyLj4nbOqE0GEyCfCKFs)

### Lenguaje de programación: C++
### IDE utilizado: CLion


### Resultado esperado a conseguir:

Interfaz gráfica simple y pequeña para poder jugar al tetris como en los viejos tiempos.
Los **EXTRAS** de este videojuego podrían ser:

-   Incorporar música del Tetris mientras juegas (sin Copyright) y poder apagarla si te molesta

-   Interfaz de menú para seleccionar Nivel inicial, Altura inicial y Número de Jugadores.

-   Establecer récords en el juego por puntuación máxima (a lo tipo arcade en los recreativos)




Biblioteca prevista a usar: _**Miniwin.h**_



Esta biblioteca de Pau Fernández realizada en C++/CodeBlocks es un mini-conjunto de funciones para abrir una ventana, pintar en ella y detectar la presión de algunas teclas. Lo justo para poder implementar juegos sencillos sin necesidad de OpenGL como se hizo en la asignatura de Informática Gráfica y Visual.



El código pertinente y la documentación de las funciones a utilizar está colgada en su GitHub para su total acceso libre.



Documentación de MiniWin: [https://miniwin.readthedocs.io/en/latest/Utilizacion.html](https://miniwin.readthedocs.io/en/latest/Utilizacion.html)



Miniwin es una clase de C++ compuesta por dos ficheros miniwin.h y miniwin.cpp. Para poder usar esta clase, sólo hay que abrir un nuevo proyecto en CLion y agregar los ficheros a la carpeta raíz.



En el resto de archivos que utilicen la biblioteca, habrá que agregar las siguientes líneas de código:



	#include "miniwin.h"
	using namespace miniwin;

Referencias consultadas por el momento:

-   Lectura detenida del manual de MiniWin con su rápida instalación y prueba de dibujo de algunos elementos gráficos como puntos, líneas, rectángulos...![](https://lh7-us.googleusercontent.com/4gYGpj4d0aKwnKeL0BHlxN4AOyApEDVPfvR8Rdvd6q_TpixueGTUQz8H7zjfNKiT0sDhL8UzKcXY353TiH44hu7H85AuKsYO6uXfpiCZFi6Uzthe4vsO9ivMqtiX4Q-fyVZ3RXTWGPDwmfSdC5dS-zI)

-   Jugar mucho al Tetris: Hay múltiples versiones e instrucciones... Me guío por el Original de la Game Boy

-   Analizar patrones de piezas, tablero, puntuaciones, niveles y aparición y movimiento automático de las piezas. Me apoyo de vídeos que explican (sobre todo con dibujos) el funcionamiento interno del juego para poder entender su lógica y poder programarlo.




Tras haber consultado las distintas fuentes de información, en un principio, la estructura del UML podría ser de la siguiente manera:![](https://lh7-us.googleusercontent.com/ukGNvM_GpgjABQbJN2AmNBwJNYhhnfo2tLMCxXHaZSbm4bqmr57coCxHYegmtvHN_h4PM4sNTiTL1bGdfpPxPqJcyImLlluOtUPjANVuCu8W6rlg-PLVVgv3wnoizx6CeOFPGCs4ZBteyng_q6b7Rpo)

Aquí una pequeña explicación de lo que cada atributo/método de cada clase representa en el juego del Tetris:
#### Clase Coord

	Atributos:
		int x: Coordenada x.
		int y: Coordenada y.
	Funciones:
		Ninguna función específica.
	Propósito:
		Representa coordenadas x e y en el espacio bidimensional.

#### Clase Pieza
	Atributos:
		Coord abs: Coordenada del bloque central.
		Coord relat[3]: Coordenadas de bloques periféricos.
		int color: Color de la pieza.
	Funciones:
		Coord posicionBloque(int n): Devuelve la coordenada absoluta de un bloque ‘n’ relativo o central.
	Propósito:
		Representa una pieza del Tetris con su posición y forma.

#### Clase Tablero
	Atributos:
		int matriz[COLUMNAS][FILAS]: Matriz que representa el estado del tablero.
	Funciones:
		void vaciarTablero(): Establece todas las celdas del,tablero como vacías.
		void actualizaTablero(): Redibuja las celdas ocupadas del tablero.
		void insertaPieza(const Pieza& P): Coloca una pieza en el tablero.
		bool colisionPieza(const Pieza& P): Verifica si una pieza colisiona con el tablero.
		bool filaLlena(int fila): Verifica si una fila está completamente llena.
		void quitarFila(int fila): Desplaza las filas superiores hacia abajo cuando una fila está llena.
		int cuentaFila(): Cuenta y elimina las líneas llenas del tablero.
	Propósito:
		Gestiona el estado y las operaciones del tablero de juego.

#### Clase TetrisGame

	Atributos:
		Tablero tablero: Tablero de juego.
		Pieza piezaAct: Pieza que está en juego.
		Pieza piezaSig: Siguiente pieza a ser utilizada.
		int ptos: Puntuación acumulada.
		int level: Nivel actual del juego.
		int frame: Contador de tiempo para controlar el movimiento automático hacia abajo.
	Funciones:
		void iniciarPartida(): Inicializa el juego y las piezas.
		void pintarInterfaz(): Actualiza la pantalla de juego.
		void gameOver(): Muestra el mensaje de "Game Over".
		void run(): Controla el bucle principal del juego.
	Propósito:
		Gestiona el flujo del juego, controla las interacciones con el jugador.






Para terminar decir que todo esto es PROVISIONAL y puede que algunas de las funciones explicadas anteriormente no se lleguen a hacer o incluso aparezcan nuevas funciones por lances del proyecto o bien por cambio de metodología, pero actualmente es la aproximación más adecuada que tengo en mente para realizar el juego del Tetris.