#pragma once

#include <Arduino.h>
#include "desactivaLog.h"
#include <DFRobotDFPlayerMini.h>
#include "rtc.h"


const unsigned long DEBOUNCE_DELAY = 150;

#define UID_BASE_ADDR 0x1FFF7590UL

#define FRECUENCIA_PWM_BACKLIGHT 1000
#define FRECUENCIA_SPI 27000000

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128
#define STATUS_BAR_H 16
#define NUMERO_LEDS_WS2812B 8

#define STATUS_BAR_BG 0x0000 // negro RGB565, sin depender de ST7735.h
// #define STATUS_BAR_BG ST7735_BLACK

// enumeración para el estados

enum EstadoSistema
{
    ESTADO_MENU_PRINCIPAL,
    ESTADO_AJUSTE_VOLUMEN,
    ESTADO_AJUSTE_ECUALIZADOR,
    ESTADO_AJUSTE_BRILLO_RGB,
    ESTADO_SELECCION_MODO,
    ESTADO_INFORMACION,
    ESTADO_CONFIGURACION
};

// Estructura para manejar la hora de forma sencilla
struct InformacionHardware
{
    uint8_t version;
    uint8_t revision;
    uint8_t memoria;
    bool rgb;
    bool mp3;
};

// El .cpp que necesite usar BarraProgreso debe incluir barraProgreso.h explícitamente.
class BarraProgreso;
class Menu;
// extern class Menu menu;

// variables globales
extern bool sistemaEncendido;
extern bool alarmaSonando;
extern unsigned long tiempoInicio;
extern EstadoSistema estadoActual;
extern unsigned long ultimoTiempoBoton;
extern String comandoBluetoothRecibido;

// Barras de progreso
extern BarraProgreso barraVolumen;
extern BarraProgreso barraBrilloPantalla;
extern BarraProgreso barraWS2812B;

// bluetooth
extern char nombreBT[30];

// Objetos globales
// extern Adafruit_NeoPixel rgb;
//// extern SoftwareSerial mp3Serie;
// extern DFRobotDFPlayerMini myDFPlayer;
// extern Adafruit_ST7735 tft;
// extern HardwareSerial BTSerial;

void mostrarPistaActual();