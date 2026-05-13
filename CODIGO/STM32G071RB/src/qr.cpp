#include "qr.h"
#include "pantalla.h"

void QR(const String &text)
{
    QRCode qrcode;
    static uint8_t qrcodeData[193]; // Para versión 3, el buffer es de 193 bytes

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