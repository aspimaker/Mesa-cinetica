#pragma once

#include <stdint.h>

// función para convertir RGB a RGB565 (no necesita la librería tft)
inline constexpr uint16_t RGB565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// colores personalizados que no están definidos en Adafruit_ST7735.h
constexpr uint16_t ST7735_VERDE_OSCURO = RGB565(0, 100, 0);
constexpr uint16_t ST7735_GRIS         = RGB565(128, 128, 128);

#define COLOR_AMARILLO  ST7735_YELLOW
#define COLOR_CYAN      ST7735_CYAN
#define COLOR_MAGENTA   COLOR_MAGENTA
#define COLOR_GRIS      ST7735_GRIS
#define COLOR_NARANJA   COLOR_NARANJA
