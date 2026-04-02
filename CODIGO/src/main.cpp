#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "iconos.h"
#include "colores.h"
#include "bluetooth.h"
#include "qrcode.h"

#ifdef RELEASE
#undef Serial
class DummySerial
{
public:
    void begin(unsigned long) {}
    template <typename T>
    void print(const T &) {}
    template <typename T>
    void println(const T &) {}
    void println() {}
    void printf(const char *, ...) {}
    void write(const char *) {}
    void write(int) {}
    void flush() {}
    int available() { return 0; }
    int read() { return -1; }
    operator bool() { return false; }
};
DummySerial Serial;
#endif

// dirección de memoria base del ID único de 96 bits
#define UID_BASE_ADDR 0x1FFF7590UL

// pines para la pantalla TFT
#define TFT_CS D10
#define TFT_DC D9
#define TFT_RST D8
#define TFT_BLK D6

#define FRECUENCIA_PWM_BACKLIGHT 1000
#define FRECUENCIA_SPI 24000000

// #define LED_BUILTIN PA5 // led de la placa

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128

/*
┌────────────────────────────────┐
│ [BT] [MP3] [VOL] [LED] [MODE]  │  16px alto
├────────────────────────────────┤
│                                │
│         contenido              │  112px
└────────────────────────────────┘
*/

const char *nombreBase = "aspiKntc-"; // prefijo para el nombre del dispositivo Bluetooth (se le añadirá un hash del UID para hacerlo único)
const char *pinBT = "123456";         // no se usa el pin, pero se configura este valor por defecto
char nombreBT[30];                    // variable para el nombre completo del dispositivo Bluetooth (prefijo + hash del UID)

HardwareSerial BTSerial(PA1, PA0); // RX=A1, TX=A0 → UART4

Bluetooth bt;
bool lastConnected = false;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Altura y color de la barra de estado
#define STATUS_BAR_H 16
#define STATUS_BAR_BG ST77XX_BLACK // o el color que elijas

void resetearDispositivo() // reiniciar el dispositivo
{
    HAL_NVIC_SystemReset();
}

uint16_t color565(uint8_t r, uint8_t g, uint8_t b) // devuelve un color en formato RGB565 a partir de valores RGB de 8 bits
{
    return tft.color565(r, g, b);
}

void brilloOled(int brillo) // controla el brillo de backlight usando PWM (0-255)
{
    analogWrite(TFT_BLK, brillo);
}

void mostrarSplash(unsigned long duracion) // mostrar logo de bienvenida
{
    tft.fillScreen(ST77XX_BLACK);                                                 // Limpiar pantalla
    tft.drawRGBBitmap(0, 0, (uint16_t *)logo_aspimaker, LOGO_WIDTH, LOGO_HEIGHT); // mostrar logo

    // borde decorativo
    // tft.drawRect(0, 0, LOGO_WIDTH, LOGO_HEIGHT, ST77XX_GREEN);

    delay(duracion);
}

String getDeviceID() // devuevle el UID completo como string hexadecimal (96 bits)
{
    uint32_t word0 = *(__IO uint32_t *)(UID_BASE_ADDR);
    uint32_t word1 = *(__IO uint32_t *)(UID_BASE_ADDR + 4);
    uint32_t word2 = *(__IO uint32_t *)(UID_BASE_ADDR + 8);

    char uid[27];
    snprintf(uid, sizeof(uid), "%08lX-%08lX-%08lX", word0, word1, word2);

    return String(uid);
}

uint32_t getDeviceIDHash() // Devuelve un hash de 32 bits del UID
{
    uint32_t word0 = *(__IO uint32_t *)(UID_BASE_ADDR);
    uint32_t word1 = *(__IO uint32_t *)(UID_BASE_ADDR + 4);
    uint32_t word2 = *(__IO uint32_t *)(UID_BASE_ADDR + 8);

    // XOR simple de las tres palabras
    return word0 ^ word1 ^ word2;
}

void drawStatusBar()
{
    // Fondo de la barra (solo si no es negro)
    if (STATUS_BAR_BG != ST77XX_BLACK)
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

void setup()
{
    Serial.begin(115200);

    uint32_t hash = getDeviceIDHash();
    snprintf(nombreBT, sizeof(nombreBT), "%s%lu", nombreBase, hash);

    Serial.println(nombreBT);
    delay(10);

    analogWriteFrequency(FRECUENCIA_PWM_BACKLIGHT); // frecuencia del pulso PWM para controlar el brillo del backlight

    pinMode(TFT_BLK, OUTPUT);

    brilloOled(255); // brillo
    // digitalWrite(TFT_BLK, HIGH); // encience la pantalla al 100% de brillo

    // Configurar LED para indicar estado
    // pinMode(LED_BUILTIN, OUTPUT);

    tft.initR(INITR_BLACKTAB);
    tft.setSPISpeed(FRECUENCIA_SPI);
    tft.setRotation(1); // horizontal / 0=0°, 1=90°, 2=180°, 3=270°

    mostrarSplash(5000);          // mostrar logo de bienvenida
    tft.fillScreen(ST77XX_BLACK); // Limpiar pantalla

    drawStatusBar();

    // configurar HC05
    bt.begin(BTSerial, 9600);
    bt.autoConfig(nombreBT, pinBT);
}

void loop()
{
    // drawStatusBar();

    while (1)
    {
        // drawStatusBar();
        delay(1000);
        Serial.println(millis());
        // Serial.print(" ");
        // Serial.println("Loop infinito...");                   // mensaje de prueba
    }
}
