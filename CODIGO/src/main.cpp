#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "iconos.h"
#include "colores.h"
#include "bluetooth.h"
#include "BarraProgreso.h"
#include "qrcode.h"
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

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
#define FRECUENCIA_SPI 27000000

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128

/*             160px
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

#define NUMERO_LEDS_WS2812B 8
#define PIN_WS2812B D7
Adafruit_NeoPixel rgb(NUMERO_LEDS_WS2812B, PIN_WS2812B, NEO_GRB + NEO_KHZ800);
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// SoftwareSerial en pines libres D2 (RX) y D3 (TX) para el módulo DFPlayer Mini
SoftwareSerial mp3Serie(D2, D3); // RX = D2, TX = D3

DFRobotDFPlayerMini myDFPlayer;
#define VOLUMEN_INICIAL 20


// declarar las barras de progreso después de crear tft porque las barras necesitan un puntero a tft
BarraProgreso barra1(tft, 10, 30, 140, 15, ST7735_NARANJA, ST7735_GREY, ST7735_WHITE, "Progreso", true);
BarraProgreso barra2(tft, 10, 70, 100, 20, ST7735_BLUE, ST7735_RED, ST7735_YELLOW, "Datos", true);



// Configuración DMA para UART (usando HAL)
// #define RX_BUFFER_SIZE 64
// uint8_t rxBuffer[RX_BUFFER_SIZE];

// Altura y color de la barra de estado
#define STATUS_BAR_H 16
#define STATUS_BAR_BG ST7735_BLACK // o el color que elijas

void QR(const String &text)
{
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(3)]; // buffer para el QR (tamaño máximo para versión 3 y ECC LOW)

    qrcode_initText(&qrcode, qrcodeData, 3, ECC_LOW, text.c_str());

    // Serial.println(millis());

    // Calcular escala para que quepa en la pantalla
    // Con versión 3 el QR es 29x29 módulos
    int scale = min(tft.width(), tft.height()) / qrcode.size;

    // Centrar en pantalla
    int offsetX = (tft.width() - qrcode.size * scale) / 2;
    int offsetY = (tft.height() - qrcode.size * scale) / 2;

    tft.fillScreen(ST7735_WHITE);

    for (uint8_t y = 0; y < qrcode.size; y++)
    {
        for (uint8_t x = 0; x < qrcode.size; x++)
        {
            uint16_t color = qrcode_getModule(&qrcode, x, y) ? ST7735_DARKGREEN : ST7735_WHITE;
            tft.fillRect(offsetX + x * scale, offsetY + y * scale, scale, scale, color);
        }
    }

    // tft.fillRect(10, 5, 5, 5, ST7735_NARANJA);

    // Serial.println(millis());
}

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
    tft.fillScreen(ST7735_BLACK);                                                 // Limpiar pantalla
    tft.drawRGBBitmap(0, 0, (uint16_t *)logo_aspimaker, LOGO_WIDTH, LOGO_HEIGHT); // mostrar logo

    // borde decorativo
    // tft.drawRect(0, 0, LOGO_WIDTH, LOGO_HEIGHT, ST7735_GREEN);

    delay(duracion);
    tft.fillScreen(ST7735_BLACK); // Limpiar pantalla después de mostrar el logo
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

void setup()
{
    analogWriteFrequency(FRECUENCIA_PWM_BACKLIGHT); // frecuencia del pulso PWM para controlar el brillo del backlight

    Serial.begin(115200);     // inicializa el puerto USB-serie
    mp3Serie.begin(9600);     // inicializa la comunicación con DFPlayer Mini
    bt.begin(BTSerial, 9600); // inicializa el módulo Bluetooth usando el puerto UART4 (RX=A1, TX=A0)

    uint32_t hash = getDeviceIDHash();
    snprintf(nombreBT, sizeof(nombreBT), "%s%lu", nombreBase, hash);

    Serial.println();
    Serial.println();
    Serial.println(nombreBT);
    delay(10);

    // configurar el brillo de la pantalla TFT
    pinMode(TFT_BLK, OUTPUT);
    brilloOled(255); // brillo
    // digitalWrite(TFT_BLK, HIGH); // encience la pantalla al 100% de brillo
    tft.initR(INITR_BLACKTAB);
    tft.setSPISpeed(FRECUENCIA_SPI);
    tft.setRotation(1); // horizontal / 0=0°, 1=90°, 2=180°, 3=270°

    mostrarSplash(500); // mostrar logo de bienvenida


    // inicializar las barras de progreso
    barra1.begin();
    barra2.begin();

    barra1.setProgresoYActualizar(0);
    barra2.setProgresoYActualizar(0);

    // mostrar la barra de estado (iconos de conexión Bluetooth, reproducción MP3, volumen, LED y modo)
    // barraEstado();

    // configurar HC05
    bt.autoConfig(nombreBT, pinBT);

    // iniciar reproductor MP3
    if (!myDFPlayer.begin(mp3Serie))
    {
        Serial.println("No se pudo iniciar el DFPlayer Mini");
        // while (true)
        //     ;
    }

    // QR("https://github.com/aspimaker/Mesa-cinetica");

    Serial.println("Setup completo");
}

void loop()
{
    // drawStatusBar();
    /*
        tft.drawBitmap(2, 0, icono_bluetooth, 16, 16, 0x7BEF); // gris
        delay(500);
        tft.drawBitmap(2, 0, icono_bluetooth, 16, 16, 0x001F);  // BT azul
        delay(500);
        tft.drawBitmap(2, 0, icono_bluetooth, 16, 16, 0xF800); // rojo
        delay(500);
    /*

        /*
        while (1)
        {
            delay(1000);
            // Serial.println(millis());
            //  Serial.print(" ");
            //  Serial.println("Loop infinito...");                   // mensaje de prueba
        }
            */

            // Subir ambas barras sincronizadas
            for (int i = 0; i <= 100; i++) {
                barra1.setProgresoYActualizar(i);
                barra2.setProgresoYActualizar(i);
                delay(50);
            }
            
            delay(50);
            
            // Bajar ambas barras
            for (int i = 100; i >= 0; i--) {
                barra1.setProgresoYActualizar(i);
                barra2.setProgresoYActualizar(i);
                delay(50);
            }
            
            delay(50);
}
