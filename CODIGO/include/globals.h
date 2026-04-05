#pragma once

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_ST7735.h>
#include "pines.h"

// Forward declaration para romper el ciclo:
// globals.h <- barraProgreso.h <- pantalla.h <- globals.h
// El .cpp que necesite usar BarraProgreso debe incluir barraProgreso.h explícitamente.
class BarraProgreso;

// ============================================
// VARIABLES GLOBALES
// ============================================

// Variables de estado del sistema
extern bool sistemaEncendido;
extern bool alarmaSonando;
extern unsigned long tiempoInicio;
extern unsigned long tiempoUltimoLED;
extern unsigned long tiempoUltimoCheckApagado;
extern bool lastConnected;

// Objetos globales
extern Adafruit_NeoPixel rgb;
extern SoftwareSerial mp3Serie;
extern DFRobotDFPlayerMini myDFPlayer;
extern Adafruit_ST7735 tft;

// Barras de progreso (forward-declared arriba; definición completa en barraProgreso.h)
extern BarraProgreso barraVolumen;
extern BarraProgreso barraBrilloPantalla;
extern BarraProgreso barraCancion;
extern BarraProgreso barraWS2812B;

// Bluetooth
extern HardwareSerial BTSerial;
extern char nombreBT[30];

// Comandos Bluetooth
extern String comandoBluetoothRecibido;

// ============================================
// CONSTANTES
// ============================================

#define UID_BASE_ADDR           0x1FFF7590UL

#define FRECUENCIA_PWM_BACKLIGHT 1000
#define FRECUENCIA_SPI           27000000

#define SCREEN_WIDTH    160
#define SCREEN_HEIGHT   128
#define STATUS_BAR_H    16
#define STATUS_BAR_BG   ST7735_BLACK

#define VOLUMEN_INICIAL         20
#define BRILLO_PANTALLA_INICIAL 128

#define NUMERO_LEDS_WS2812B 8