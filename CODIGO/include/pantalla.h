#pragma once

/*             160px
┌────────────────────────────────┐
│ [BT] [MP3] [VOL] [LED] [MODE]  │  16px alto
├────────────────────────────────┤
│                                │
│         contenido              │  112px
└────────────────────────────────┘
*/

#include <Arduino.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_GFX.h>
#include "globals.h"
#include "configuracion.h"
#include "colores.h"
#include "barraProgreso.h"
#include "iconos.h"
#include "logo.h"
#include "qrcode.h"

// Funciones de pantalla
void iniciarPantalla();
//void mostrarSplash(unsigned long duracion);
void mostrarSplash();
void barraEstado();
void redibujarTodo();
void actualizarInterfaz();
void brilloPantalla(int brillo);
void QR(const String &text);