#include "leds.h"
#include "configuracion.h"
#include "botones.h"

Adafruit_NeoPixel rgb(NUMERO_LEDS_WS2812B, PIN_WS2812B, NEO_GRB + NEO_KHZ800);
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

extern Configuracion config;

void iniciarLEDs()
{
    rgb.begin();
    rgb.setBrightness(config.get().ledBrillo);
    rgb.show();
}

void actualizarLEDs()
{
    if (!sistemaEncendido)
        return;

    static unsigned long ultimoFrameLED = 0;
    unsigned long ahora = millis();

    // velocidad de actualización según configuración
    int intervalo = map(config.get().ledVelocidad, 0, 255, 20, 200);

    if (ahora - ultimoFrameLED >= intervalo)
    {
        ultimoFrameLED = ahora;

        switch (config.get().ledModo)
        {
        case 0: // OFF
            rgb.clear();
            break;

        case 1: // ESTÁTICO
            rgb.fill(config.get().ledColor);
            break;

        case 2: // RESPIRACIÓN
            static int brilloResp = 0;
            static bool subiendo = true;

            if (subiendo)
            {
                brilloResp += 5;
                if (brilloResp >= 255)
                    subiendo = false;
            }
            else
            {
                brilloResp -= 5;
                if (brilloResp <= 0)
                    subiendo = true;
            }

            rgb.setBrightness(brilloResp);
            rgb.fill(config.get().ledColor);
            break;

        case 3: // MÚSICA (reacciona al volumen)
        {
            int nivelVolumen = map(config.get().volumen, 0, 30, 0, 255);
            rgb.setBrightness(nivelVolumen);
            rgb.fill(config.get().ledColor);
            break;
        }

        case 4: // RAINBOW
        {
            static uint8_t hue = 0;
            uint32_t color = rgb.ColorHSV(hue * 256, 255, 255);
            rgb.fill(color);
            hue += 5;
            break;
        }
        }

        rgb.show();
    }
}