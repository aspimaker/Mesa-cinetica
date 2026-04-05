/*             160px
┌────────────────────────────────┐
│ [BT] [MP3] [VOL] [LED] [MODE]  │  16px alto
├────────────────────────────────┤
│                                │
│         contenido              │  112px
└────────────────────────────────┘
*/

#include "pantalla.h"

// Definición de objetos globales de pantalla
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// barras de progreso
BarraProgreso barraVolumen(tft, 0, 30, 160, 15, ST7735_ORANGE, ST7735_GRIS, ST7735_WHITE, "Volumen", true);
BarraProgreso barraBrilloPantalla(tft, 10, 70, 100, 20, ST7735_BLUE, ST7735_RED, ST7735_YELLOW, "Brillo pantalla", true);
BarraProgreso barraWS2812B(tft, 10, 90, 100, 20, ST7735_BLUE, ST7735_RED, ST7735_YELLOW, "Brillo RGB", true);

extern Configuracion config;
extern DFRobotDFPlayerMini myDFPlayer;

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

    barraVolumen.begin();
    barraBrilloPantalla.begin();
    barraWS2812B.begin();
}

//void mostrarSplash(unsigned long duracion)
void mostrarSplash()
{
    tft.fillScreen(ST7735_BLACK);
    tft.drawRGBBitmap(0, 0, (uint16_t *)logo_aspimaker, LOGO_WIDTH, LOGO_HEIGHT);
    //delay(duracion);
}

void barraEstado()
{
    // Fondo de la barra (solo si no es negro)
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

void redibujarTodo()
{
    tft.fillScreen(ST7735_BLACK);

    tft.drawBitmap(2, 0, icono_bluetooth, 16, 16, ST7735_BLUE);
    tft.drawBitmap(34, 0, icon_mp3_play, 16, 16, ST7735_VERDE_OSCURO);
    tft.drawBitmap(66, 0, icon_vol_on, 16, 16, ST7735_BLUE);
    tft.drawBitmap(98, 0, icon_led_on, 16, 16, ST7735_YELLOW);
    tft.drawBitmap(130, 0, icon_mode_m, 16, 16, ST7735_MAGENTA);
}

void actualizarInterfaz()
{
    if (!sistemaEncendido)
        return;

    uint8_t hora, minuto, segundo, dia, mes, anio;
    config.getDateTime(hora, minuto, segundo, dia, mes, anio);

    char horaStr[6];
    sprintf(horaStr, "%02d:%02d", hora, minuto);

    tft.setCursor(SCREEN_WIDTH - 40, 2);
    tft.setTextColor(ST7735_WHITE);
    tft.print(horaStr);

    barraEstado();
}

void brilloPantalla(int brillo)
{
    analogWrite(TFT_BLK, brillo);
}

void QR(const String &text)
{
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(3)];

    qrcode_initText(&qrcode, qrcodeData, 3, ECC_LOW, text.c_str());

    int scale = min(tft.width(), tft.height()) / qrcode.size;
    int offsetX = (tft.width() - qrcode.size * scale) / 2;
    int offsetY = (tft.height() - qrcode.size * scale) / 2;

    tft.fillScreen(ST7735_WHITE);

    for (uint8_t y = 0; y < qrcode.size; y++)
    {
        for (uint8_t x = 0; x < qrcode.size; x++)
        {
            uint16_t color = qrcode_getModule(&qrcode, x, y) ? ST7735_VERDE_OSCURO : ST7735_WHITE;
            tft.fillRect(offsetX + x * scale, offsetY + y * scale, scale, scale, color);
        }
    }
}

/*
uint16_t _color565(uint8_t r, uint8_t g, uint8_t b)
{
    return tft.color565(r, g, b);
}
*/
