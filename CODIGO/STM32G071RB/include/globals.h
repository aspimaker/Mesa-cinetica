#pragma once

#include <Arduino.h>
#include "depuracion.h"
#include <DFMiniMp3.h>
#include "rtc.h"
#include "qr.h"


inline constexpr uint8_t DEBOUNCE_DELAY_MS = 150;

inline constexpr uintptr_t UID_BASE_ADDR = 0x1FFF7590UL;

inline constexpr uint16_t FRECUENCIA_PWM_BACKLIGHT = 1000;
inline constexpr uint32_t FRECUENCIA_SPI = 27000000;

inline constexpr uint8_t SCREEN_WIDTH = 160;
inline constexpr uint8_t SCREEN_HEIGHT = 128;
inline constexpr uint8_t STATUS_BAR_H = 16;

inline constexpr uint8_t STATUS_BAR_BG = 0x0000; // color de fondo de la barra de estado

inline constexpr uint8_t NUMERO_LEDS_WS2812B = 8;

inline constexpr uint8_t pasoVolumen =1; // paso de aumento/disminución de volumen

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

struct InformacionHardware
{
    uint8_t version;
    uint8_t revision;
    uint8_t memoria;
    bool rgb;
    bool mp3;
};

// el .cpp que necesite usar BarraProgreso debe incluir barraProgreso.h explícitamente.
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

void mostrarPistaActual();