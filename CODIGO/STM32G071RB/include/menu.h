#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <Adafruit_ST7735.h>

// Colores del menú (usando tus definiciones de color)
#define COLOR_FONDO ST7735_BLACK
#define COLOR_SELECCION ST7735_YELLOW
#define COLOR_BARRA ST7735_BLUE
#define COLOR_TEXTO_BARRA ST7735_WHITE
#define COLOR_ICONO_NORMAL ST7735_WHITE
#define COLOR_ICONO_SELEC ST7735_BLACK

// Dimensiones
#define ANCHO_PANTALLA 160
#define ALTO_PANTALLA 128
#define ALTO_BARRA 16
#define ALTO_ICONOS (ALTO_PANTALLA - ALTO_BARRA) // 112
#define ANCHO_ICONO (ANCHO_PANTALLA / 3)         // 53
#define ALTO_ICONO (ALTO_ICONOS / 2)             // 56

// Márgenes para el cuadrado de selección
#define MARGEN_SELECCION 4
#define ANCHO_SELECCION (ANCHO_ICONO - MARGEN_SELECCION * 2) // 45
#define ALTO_SELECCION (ALTO_ICONO - MARGEN_SELECCION * 2)   // 48

// Posiciones X para cada columna
#define POS_X_COL0 MARGEN_SELECCION
#define POS_X_COL1 (ANCHO_ICONO + MARGEN_SELECCION)     // 57
#define POS_X_COL2 (ANCHO_ICONO * 2 + MARGEN_SELECCION) // 110

// Posiciones Y para cada fila
#define POS_Y_FILA0 (ALTO_BARRA + MARGEN_SELECCION)              // 20
#define POS_Y_FILA1 (ALTO_BARRA + ALTO_ICONO + MARGEN_SELECCION) // 76

class Menu
{
public:
    Menu(Adafruit_ST7735 &pantalla);            // Constructor
    void begin();                               // Inicializar menú
    void dibujarBarraEstado(const char *texto); // Dibujar barra superior
    void dibujarIconos();                       // Dibujar todos los iconos
    void dibujarSeleccion(int iconoIndex);      // Dibujar cuadrado en icono activo
    void borrarSeleccion(int iconoIndex);       // Borrar cuadrado de selección
    void moverSeleccion(int direccion);         // Mover selección (0=arriba,1=abajo,2=izq,3=der)
    int getIconoActivo();                       // Obtener icono activo
    void ejecutarAccion();                      // Ejecutar acción del icono seleccionado

private:
    Adafruit_ST7735 &tft; // Referencia a la pantalla
    int iconoActivo;      // Icono actualmente seleccionado (0-5)

    // Métodos privados
    void dibujarIcono(int index, bool seleccionado);
    void obtenerCoordenadasIcono(int index, int &x, int &y);
    int getXPorColumna(int columna);
    int getYPorFila(int fila);
    void dibujarRectanguloSeleccion(int x, int y, bool dibujar); // true=dibujar, false=borrar
};

#endif