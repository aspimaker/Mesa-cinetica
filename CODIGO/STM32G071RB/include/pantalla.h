#pragma once

/*             160px
┌─────────────────────────────────────────┐
│  [BT]   [MP3]   [VOL]   [LED]   [MODE]  │ 16px 
├─────────────┬─────────────┬─────────────┤---
│      0      │      1      │      2      │  |
│  Diseño     │  Pista MP3  │  RGB        │  |        128px
├─────────────┼─────────────┼─────────────┤ 112px
│      3      │      4      │      5      │  |
│ Modo dibujo │  Modo MP3   │      ?      │  |
└─────────────┴─────────────┴─────────────┘---
*/

#include <Arduino.h>
#include "barraProgreso.h"
#include "iconos.h"
#include "logo.h"
#include "colores.h"

extern Adafruit_ST7735 tft;
extern class Menu      menu;

extern BarraProgreso barraVolumen;
extern BarraProgreso barraBrilloPantalla;
extern BarraProgreso barraWS2812B;

// Declaración de funciones básicas
void iniciarPantalla();
void limpiarPantalla();
void mostrarSplash();
void barraEstado();
void actualizarInterfaz();
void actualizarReloj();
void brilloPantalla(int brillo);
void mostrarPistaActual();
void dibujarRectangulo(int x, int y, int ancho, int alto, uint16_t color);
void dibujarRectanguloRelleno(int x, int y, int ancho, int alto, uint16_t color);
void dibujarLinea(int x0, int y0, int x1, int y1, uint16_t color);
void dibujarCirculo(int x, int y, int radio, uint16_t color);
void dibujarCirculoRelleno(int x, int y, int radio, uint16_t color);
void dibujarTriangulo(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color);