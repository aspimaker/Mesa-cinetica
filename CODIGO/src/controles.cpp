// ============================================================
// controles.cpp — Implementación de las acciones de botón
// para cada estado de la máquina de estados.
//
// Patrón de cada función:
//   1. Modifica el dato en config (setter puro, no guarda)
//   2. Aplica el cambio al hardware directamente
//   3. Actualiza la barra de progreso en pantalla
// ============================================================

#include "controles.h"
#include "audio.h"          // subirVolumenMP3(), bajarVolumenMP3()
#include "leds.h"           // rgb
#include "configuracion.h"  // config
#include "pantalla.h"       // barraVolumen, barraWS2812B, barraBrilloPantalla, brilloPantalla()
#include "pines.h"          // TFT_BLK

#include <Adafruit_NeoPixel.h>
#include <DFRobotDFPlayerMini.h>

extern Configuracion        config;
extern Adafruit_NeoPixel    rgb;
extern DFRobotDFPlayerMini  myDFPlayer;

// ── Paso de ajuste para cada control ─────────────────────
static constexpr uint8_t PASO_VOLUMEN  = 1;    // 1 unidad DFPlayer  (rango 0-30)
static constexpr uint8_t PASO_BRILLO   = 15;   // ~6% del rango      (rango 0-255)
static constexpr uint8_t PASO_EQ       = 1;    // 1 modo             (rango 0-5)

// ============================================================
// VOLUMEN
// ============================================================

void aumentarVolumen()
{
    subirVolumenMP3();   // modifica config + aplica al DFPlayer (ver audio.cpp)
    barraVolumen.setProgresoYActualizar(
        map(config.get().volumen, 0, 30, 0, 100));
}

void disminuirVolumen()
{
    bajarVolumenMP3();
    barraVolumen.setProgresoYActualizar(
        map(config.get().volumen, 0, 30, 0, 100));
}

// ============================================================
// BRILLO LEDs WS2812B
// ============================================================

void aumentarBrilloRGB()
{
    uint8_t actual = config.get().ledBrillo;
    uint8_t nuevo  = (actual + PASO_BRILLO > 255) ? 255 : actual + PASO_BRILLO;
    config.setLedBrillo(nuevo);
    rgb.setBrightness(nuevo);
    rgb.show();
    barraWS2812B.setProgresoYActualizar(map(nuevo, 0, 255, 0, 100));
}

void disminuirBrilloRGB()
{
    uint8_t actual = config.get().ledBrillo;
    uint8_t nuevo  = (actual < PASO_BRILLO) ? 0 : actual - PASO_BRILLO;
    config.setLedBrillo(nuevo);
    rgb.setBrightness(nuevo);
    rgb.show();
    barraWS2812B.setProgresoYActualizar(map(nuevo, 0, 255, 0, 100));
}

// ============================================================
// BRILLO PANTALLA TFT
// ============================================================

void aumentarBrilloPantalla()
{
    uint8_t actual = config.get().brillo;
    uint8_t nuevo  = (actual + PASO_BRILLO > 255) ? 255 : actual + PASO_BRILLO;
    config.setBrillo(nuevo);
    brilloPantalla(nuevo);   // analogWrite directo
    barraBrilloPantalla.setProgresoYActualizar(map(nuevo, 0, 255, 0, 100));
}

void disminuirBrilloPantalla()
{
    uint8_t actual = config.get().brillo;
    // Mínimo 10 para no apagar la pantalla completamente durante el ajuste
    uint8_t nuevo  = (actual < PASO_BRILLO + 10) ? 10 : actual - PASO_BRILLO;
    config.setBrillo(nuevo);
    brilloPantalla(nuevo);
    barraBrilloPantalla.setProgresoYActualizar(map(nuevo, 0, 255, 0, 100));
}

// ============================================================
// ECUALIZADOR
// ============================================================

// Nombres de los modos EQ del DFPlayer Mini
static const char* const NOMBRES_EQ[] = {
    "Normal", "Pop", "Rock", "Jazz", "Classic", "Bass"
};

void siguienteEcualizador()
{
    uint8_t nuevo = (config.get().ecualizador + PASO_EQ) % 6;
    config.get().ecualizador = nuevo;   // setter directo — no hay setEcualizador()
    myDFPlayer.EQ(nuevo);
    // Mostrar nombre del modo en barra de estado (pantalla.h lo exporta via menu)
    // El llamador (main.cpp / menu) puede actualizar la UI si lo necesita
}

void anteriorEcualizador()
{
    uint8_t actual = config.get().ecualizador;
    uint8_t nuevo  = (actual == 0) ? 5 : actual - PASO_EQ;
    config.get().ecualizador = nuevo;
    myDFPlayer.EQ(nuevo);
}