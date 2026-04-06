#pragma once

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_ST7735.h>
#include "pines.h"
#include "rtc.h"

const unsigned long DEBOUNCE_DELAY = 150;

// Estados del sistema
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

// Estructura para botones TTP223
struct BotonTTP223
{
    int pin;
    bool estadoActual;
    bool estadoAnterior;
    unsigned long ultimoDebounce;
};

// El .cpp que necesite usar BarraProgreso debe incluir barraProgreso.h explícitamente.
class BarraProgreso;

// ============================================
// VARIABLES GLOBALES
// ============================================

// Variables de estado del sistema
extern bool sistemaEncendido;
extern bool alarmaSonando;
extern unsigned long tiempoInicio;
extern String comandoBluetoothRecibido;
extern class Menu menu;
extern unsigned long tiempoInicio;
extern unsigned long tiempoUltimoLED;
extern unsigned long tiempoUltimoCheckApagado;
extern bool lastConnected;
extern String comandoBluetoothRecibido;
extern EstadoSistema estadoActual;

// Objetos globales
extern Adafruit_NeoPixel rgb;
extern SoftwareSerial mp3Serie;
extern DFRobotDFPlayerMini myDFPlayer;
extern Adafruit_ST7735 tft;

// Variables de botones (declaraciones)
extern BotonTTP223 botonOK;
extern BotonTTP223 botonDerecha;
extern BotonTTP223 botonIzquierda;
extern BotonTTP223 botonArriba;
extern BotonTTP223 botonAbajo;

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

#define UID_BASE_ADDR 0x1FFF7590UL

#define FRECUENCIA_PWM_BACKLIGHT 1000
#define FRECUENCIA_SPI 27000000

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128
#define STATUS_BAR_H 16
#define NUMERO_LEDS_WS2812B 8

#define STATUS_BAR_BG ST7735_BLACK

void iniciarBotonesTTP223();
bool leerBotonTTP223(BotonTTP223 &boton);
void mostrarPistaActual();