/*                 160px
┌─────────────────────────────────────────┐
│  [BT]   [MP3]   [VOL]   [LED]   [MODE]  │  16px alto
├─────────────┬─────────────┬─────────────┤
│      0      │      1      │      2      │
│   patrones  │  pista MP3  │     RGB     │
├─────────────┼─────────────┼─────────────┤ 112px alto
│      3      │      4      │      5      │
│ Modo dibujo │  Modo MP3   │      ?      │
└─────────────┴─────────────┴─────────────┘
*/

#include "pantalla.h"
#include "menu.h"
#include "configuracion.h"
#include "audio.h"
#include "pines.h"
#include "rtc.h"
#include "globals.h"

extern Configuracion config;

// crear objeto tft
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// crear objeto menú
Menu menu(tft);

// barras de progreso
BarraProgreso barraVolumen(tft, 0, 30, 160, 15, ST7735_ORANGE, ST7735_GRIS, ST7735_WHITE, "Volumen", true);
BarraProgreso barraBrilloPantalla(tft, 10, 70, 100, 20, ST7735_BLUE, ST7735_RED, ST7735_YELLOW, "Brillo pantalla", true);
BarraProgreso barraWS2812B(tft, 10, 90, 100, 20, ST7735_BLUE, ST7735_RED, ST7735_YELLOW, "Brillo RGB", true);

extern Configuracion config;

void iniciarPantalla()
{
    analogWriteFrequency(FRECUENCIA_PWM_BACKLIGHT);
    pinMode(TFT_BLK, OUTPUT);

    uint8_t brillo = config.get().brillo;
    if (brillo == 0)
        brillo = 128;
    analogWrite(TFT_BLK, brillo);

    tft.initR(INITR_BLACKTAB);
    tft.setSPISpeed(FRECUENCIA_SPI);
    tft.setRotation(1);
    tft.fillScreen(ST7735_BLACK);

    // barras de progreso
    /*
    barraVolumen.begin();
    barraBrilloPantalla.begin();
    barraWS2812B.begin();
    */
}

// void mostrarSplash(unsigned long duracion)
void mostrarSplash()
{
    tft.fillScreen(ST7735_BLACK);
    tft.drawRGBBitmap(0, 0, (uint16_t *)logo_aspimaker, LOGO_WIDTH, LOGO_HEIGHT);
}

void barraEstado()
{
    // fondo de la barra (solo si no es negro)
    if (STATUS_BAR_BG != ST7735_BLACK)
    {
        tft.fillRect(0, 0, 160, STATUS_BAR_H, STATUS_BAR_BG);
    }

    // Iconos: x, y, bitmap, w, h, color
    tft.drawBitmap(2, 0, icono_bluetooth, 16, 16, 0x001F);  // BT azul
    tft.drawBitmap(2, 18, icono_bluetooth, 16, 16, 0x7BEF); // gris
    tft.drawBitmap(2, 36, icono_bluetooth, 16, 16, 0xF800); // rojo
    tft.drawBitmap(34, 0, icon_mp3_play, 16, 16, 0x07E0);   // MP3 verde
    tft.drawBitmap(66, 0, icon_vol_on, 16, 16, 0x001F);     // VOL azul
    tft.drawBitmap(98, 0, icon_led_on, 16, 16, 0xFFE0);     // LED amarillo
    tft.drawBitmap(130, 0, icon_mode_m, 16, 16, 0x801F);    // MODE violeta
}

void actualizarReloj()
{
    // variables estáticas para de los dígitos anteriores (repintado de hora, minutos y segundos)
    static uint8_t digitos_prev[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // 0xFF = no inicializado

    FechaHora fh;
    RTC_GetFechaHora(fh);
    // dPf("RTC: %02d:%02d:%02d %02d:%02d:%02d\n", fh.dia, fh.mes, fh.año, fh.horas, fh.minutos, fh.segundos);

    // los 6 dígitos de la hora: HH MM SS
    uint8_t digitos[6] = {
        (uint8_t)(fh.horas / 10), (uint8_t)(fh.horas % 10),
        (uint8_t)(fh.minutos / 10), (uint8_t)(fh.minutos % 10),
        (uint8_t)(fh.segundos / 10), (uint8_t)(fh.segundos % 10)};

    // posición X de cada dígito (ajustar según tu fuente y posición)
    // entre dígito 1-2 y 3-4 hay ':' que ocupa espacio
    const uint8_t CHAR_W = 6; // ancho de carácter en pixels (fuente por defecto)
    const uint16_t X_BASE = SCREEN_WIDTH - 60;
    const uint16_t Y = 20;

    // offsets X de cada dígito, saltando los ':'
    const uint8_t offsets[6] = {
        0,          // H decena
        CHAR_W,     // H unidad
        CHAR_W * 3, // M decena  (+1 por el ':')
        CHAR_W * 4, // M unidad
        CHAR_W * 6, // S decena  (+1 por el ':')
        CHAR_W * 7  // S unidad
    };

    tft.setTextSize(1);

    for (int i = 0; i < 6; i++)
    {
        if (digitos[i] != digitos_prev[i])
        {
            // borrar dígito anterior con color de fondo
            tft.setCursor(X_BASE + offsets[i], Y);
            tft.setTextColor(STATUS_BAR_BG); // color de fondo
            tft.print(digitos_prev[i]);

            // pintar nuevo dígito
            tft.setCursor(X_BASE + offsets[i], Y);
            tft.setTextColor(ST7735_WHITE);
            tft.print(digitos[i]);

            digitos_prev[i] = digitos[i];
        }
    }

    // pintar ':' solo la primera vez (no cambian nunca)
    static bool separadores = false;
    if (!separadores)
    {
        tft.setTextColor(ST7735_WHITE);
        tft.setCursor(X_BASE + CHAR_W * 2, Y);
        tft.print(':');
        tft.setCursor(X_BASE + CHAR_W * 5, Y);
        tft.print(':');
        separadores = true;
    }
}

void actualizarInterfaz()
{
    if (!sistemaEncendido)
        return;

    // actualizar barra de estado
    barraEstado();

    // actualizar reloj solo cada segundo
    static uint32_t ultimaActualizacion = 0;
    uint32_t ahora = HAL_GetTick();
    if (ahora - ultimaActualizacion >= 1000)
    {
        ultimaActualizacion = ahora;
        actualizarReloj();
    }
}

void brilloPantalla(int brillo)
{
    analogWrite(TFT_BLK, brillo);
}
