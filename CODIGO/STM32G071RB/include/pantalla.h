#pragma once

/*             160px
┌─────────────────────────────────────────┐
│  [BT]   [MP3]   [VOL]   [LED]   [MODE]  │  16px alto
├─────────────┬─────────────┬─────────────┤
│      0      │      1      │      2      │
│  Diseño     │  Pista MP3  │  RGB        │
├─────────────┼─────────────┼─────────────┤ 112px
│      3      │      4      │      5      │
│ Modo dibujo │  Modo MP3   │      ?      │
└─────────────┴─────────────┴─────────────┘
*/

#include <Arduino.h>
#include "barraProgreso.h"
#include "iconos.h"
#include "logo.h"
#include "qrcode.h"

//#include <Adafruit_ST7735.h>
//#include <Adafruit_GFX.h>
//#include "globals.h"
//#include "configuracion.h"
//#include "colores.h"

extern Adafruit_ST7735 tft;
extern class Menu      menu;


extern BarraProgreso barraVolumen;
extern BarraProgreso barraBrilloPantalla;
extern BarraProgreso barraWS2812B;

// Funciones de pantalla
void iniciarPantalla();
void mostrarSplash();
void barraEstado();
void redibujarTodo();
void actualizarInterfaz();
void actualizarReloj();
void brilloPantalla(int brillo);
void QR(const String &text);
void mostrarPistaActual();