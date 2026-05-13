// depuracion.h
#pragma once

#include "pantalla.h"

// extern HardwareSerial SerialDebug;
extern Adafruit_ST7735 tft;
extern bool debugEnabled;

// ============================================
// ÁREA DE PANTALLA PARA DEPURACIÓN
// ============================================

static int _indiceActual = 0;
static bool _primerMensaje = true;

// Posiciones fijas para el área de depuración en pantalla
inline constexpr uint8_t DEBUG_PANTALLA_X = 2;

inline constexpr uint8_t LINEAS_Y[] = {0, 60, 70, 80, 90, 100, 110};

// Colores para depuración en pantalla
inline constexpr uint16_t DEBUG_COLOR_NORMAL = ST7735_WHITE;
inline constexpr uint16_t DEBUG_COLOR_ERROR = ST7735_RED;
inline constexpr uint16_t DEBUG_COLOR_OK = ST7735_VERDE_OSCURO;
inline constexpr uint16_t DEBUG_COLOR_INFO = ST7735_BLUE;

// Variables internas para control de líneas
inline static bool _pantallaInicializada = false;

inline constexpr uint16_t UI_X_INICIO = 0;
inline constexpr uint16_t UI_Y_MENU = 60;
inline constexpr uint16_t UI_ANCHO = 160;
inline constexpr uint16_t UI_ALTO_MENU = 68;

// Limpiar SOLO el área de depuración en pantalla
inline void dClearPantalla()
{
    tft.fillRect(UI_X_INICIO, UI_Y_MENU, UI_ANCHO, UI_ALTO_MENU, ST7735_VERDE_OSCURO);
    tft.drawRect(UI_X_INICIO, UI_Y_MENU, UI_ANCHO, UI_ALTO_MENU, ST7735_RED);
}

inline void dSiguienteLinea()
{
    _indiceActual++;

    if (_indiceActual > 6)
    {
        _indiceActual = 1;
        dClearPantalla();
    }
}

// Inicializar área de depuración en pantalla
inline void dInitPantalla()
{
    if (!debugEnabled)
    return;
    
    if (!_pantallaInicializada)
    {
        // Limpiar el área de depuración
        dClearPantalla();
        _pantallaInicializada = true;
    }
}

// Escribir en pantalla en la línea actual
inline void dPrintPantalla(const char *texto, uint16_t color = DEBUG_COLOR_NORMAL)
{
    if (!debugEnabled)
        return;

    dSiguienteLinea();

    tft.setCursor(DEBUG_PANTALLA_X, LINEAS_Y[_indiceActual]);
    tft.setTextColor(color);
    tft.print(texto);
}

inline void dPrintPantalla(const String &texto, uint16_t color = DEBUG_COLOR_NORMAL)
{
    dPrintPantalla(texto.c_str(), color);
}

inline void dPrintPantalla(int x, int y, const char *texto, uint16_t color = DEBUG_COLOR_NORMAL)
{
    if (!debugEnabled)
        return;

    dSiguienteLinea();

    tft.setCursor(DEBUG_PANTALLA_X, LINEAS_Y[_indiceActual]);
    tft.setTextColor(color);
    tft.print(texto);
}

// Escribir en pantalla y avanzar línea
inline void dPlnPantalla(const char *texto, uint16_t color = DEBUG_COLOR_NORMAL)
{
    if (!debugEnabled)
        return;

    dPrintPantalla(texto, color);
}

inline void dPlnPantalla(const String &texto, uint16_t color = DEBUG_COLOR_NORMAL)
{
    dPlnPantalla(texto.c_str(), color);
}

// ============================================
// FUNCIONES ORIGINALES (Serial)
// ============================================

inline void dP(const char *x)
{
    if (debugEnabled)
        Serial.print(x);
}

inline void dP(const String &x)
{
    if (debugEnabled)
        Serial.print(x);
}

inline void dP(int x)
{
    if (debugEnabled)
        Serial.print(x);
}

inline void dP(uint8_t x)
{
    if (debugEnabled)
        Serial.print(x);
}

inline void dP(uint16_t x)
{
    if (debugEnabled)
        Serial.print(x);
}

inline void dP(uint32_t x)
{
    if (debugEnabled)
        Serial.print(x);
}

inline void dP(float x)
{
    if (debugEnabled)
        Serial.print(x);
}

inline void dP(bool x)
{
    if (debugEnabled)
        Serial.print(x);
}

inline void dPln(const char *x)
{
    if (debugEnabled)
    {
        Serial.println(x);
        dPlnPantalla(x); // 🔥 También va a pantalla
    }
}

inline void dPln(const String &x)
{
    if (debugEnabled)
    {
        Serial.println(x);
        dPlnPantalla(x); // 🔥 También va a pantalla
    }
}

inline void dPln(int x)
{
    if (debugEnabled)
    {
        Serial.println(x);
        dPlnPantalla(String(x)); // 🔥 También va a pantalla
    }
}

inline void dPln(uint8_t x)
{
    if (debugEnabled)
    {
        Serial.println(x);
        dPlnPantalla(String(x));
    }
}

inline void dPln(uint16_t x)
{
    if (debugEnabled)
    {
        Serial.println(x);
        dPlnPantalla(String(x));
    }
}

inline void dPln(uint32_t x)
{
    if (debugEnabled)
    {
        Serial.println(x);
        dPlnPantalla(String(x));
    }
}

inline void dPln(float x)
{
    if (debugEnabled)
    {
        Serial.println(x);
        dPlnPantalla(String(x));
    }
}

inline void dPln(bool x)
{
    if (debugEnabled)
    {
        Serial.println(x);
        dPlnPantalla(x ? "true" : "false");
    }
}

inline void dPf(const char *fmt, ...)
{
    if (!debugEnabled)
        return;

    va_list args;
    va_start(args, fmt);
    char buffer[512];
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    Serial.print(buffer);
    dPlnPantalla(buffer); // 🔥 También va a pantalla
}

// ============================================
// FUNCIONES ESPECIALES PARA PANTALLA
// ============================================

// Enviar mensaje de error a pantalla (color rojo)
inline void dErrorPantalla(const char *msg)
{
    if (!debugEnabled)
        return;
    dPlnPantalla(msg, DEBUG_COLOR_ERROR);
}

inline void dErrorPantalla(const String &msg)
{
    dErrorPantalla(msg.c_str());
}

// Enviar mensaje de éxito a pantalla (color verde)
inline void dOkPantalla(const char *msg)
{
    if (!debugEnabled)
        return;
    dPlnPantalla(msg, DEBUG_COLOR_OK);
}

// Enviar mensaje informativo (color cyan)
inline void dInfoPantalla(const char *msg)
{
    if (!debugEnabled)
        return;
    dPlnPantalla(msg, DEBUG_COLOR_INFO);
}

// Escribir en posición específica (sin avanzar línea)
inline void dPosPantalla(int x, int y, const char *msg, uint16_t color = DEBUG_COLOR_NORMAL)
{
    if (!debugEnabled)
        return;
    dPrintPantalla(x, y, msg, color);
}