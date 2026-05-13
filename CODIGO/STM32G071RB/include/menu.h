#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <Adafruit_ST7735.h>

// colores del menú
inline constexpr uint16_t COLOR_FONDO = ST7735_BLACK;
inline constexpr uint16_t COLOR_SELECCION = ST7735_YELLOW;
inline constexpr uint16_t COLOR_BARRA = ST7735_BLUE;
inline constexpr uint16_t COLOR_TEXTO_BARRA = ST7735_WHITE;
inline constexpr uint16_t COLOR_ICONO_NORMAL = ST7735_WHITE;
inline constexpr uint16_t COLOR_ICONO_SELEC = ST7735_BLACK;

// dimensiones
inline constexpr uint8_t ANCHO_PANTALLA = 160;
inline constexpr uint8_t ALTO_PANTALLA = 128;
inline constexpr uint8_t ALTO_BARRA = 16;
inline constexpr uint8_t ALTO_ICONOS(ALTO_PANTALLA - ALTO_BARRA); // 112
inline constexpr uint8_t ANCHO_ICONO(ANCHO_PANTALLA / 3);         // 53
inline constexpr uint8_t ALTO_ICONO(ALTO_ICONOS / 2);             // 56

// márgenes para el cuadrado de selección
inline constexpr uint8_t MARGEN_SELECCION = 4;
inline constexpr uint8_t ANCHO_SELECCION(ANCHO_ICONO - MARGEN_SELECCION * 2); // 45
inline constexpr uint8_t ALTO_SELECCION(ALTO_ICONO - MARGEN_SELECCION * 2);   // 48

// posiciones X para cada columna
inline constexpr uint8_t POS_X_COL0 = MARGEN_SELECCION;
inline constexpr uint8_t POS_X_COL1(ANCHO_ICONO + MARGEN_SELECCION);      // 57
inline constexpr uint8_t POS_X_COL2(ANCHO_ICONO * 2 + MARGEN_SELECCION);  // 110

// posiciones Y para cada fila
inline constexpr uint8_t POS_Y_FILA0(ALTO_BARRA + MARGEN_SELECCION);              // 20
inline constexpr uint8_t POS_Y_FILA1(ALTO_BARRA + ALTO_ICONO + MARGEN_SELECCION); // 76

class Menu
{
public:
    Menu(Adafruit_ST7735 &pantalla);            // vonstructor
    void begin();                               // inicializar menú
    void dibujarBarraEstado(const char *texto); // dibujar barra superior
    void dibujarIconos();                       // dibujar todos los iconos
    void dibujarSeleccion(int iconoIndex);      // dibujar cuadrado en icono activo
    void borrarSeleccion(int iconoIndex);       // norrar cuadrado de selección
    void moverSeleccion(int direccion);         // mover selección (0=arriba,1=abajo,2=izq,3=der)
    int getIconoActivo();                       // obtener icono activo
    void ejecutarAccion();                      // rjecutar acción del icono seleccionado

private:
    Adafruit_ST7735 &tft; // referencia a la pantalla
    int iconoActivo;      // icono actualmente seleccionado (0-5)

    // Métodos privados
    void dibujarIcono(int index, bool seleccionado);
    void obtenerCoordenadasIcono(int index, int &x, int &y);
    int getXPorColumna(int columna);
    int getYPorFila(int fila);
    void dibujarRectanguloSeleccion(int x, int y, bool dibujar); // true=dibujar, false=borrar
};

#endif