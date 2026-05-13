#pragma once

#include <Adafruit_ST7735.h>
#include <stdint.h>

// función para convertir RGB a RGB565 (no necesita la librería tft)
inline constexpr uint16_t RGB565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// colores personalizados que no están definidos en Adafruit_ST7735.h
inline constexpr uint16_t ST7735_VERDE_OSCURO = RGB565(0, 100, 0);
inline constexpr uint16_t ST7735_GRIS         = RGB565(128, 128, 128);
inline constexpr uint16_t COLOR_AMARILLO = ST7735_YELLOW;
inline constexpr uint16_t COLOR_CYAN = ST7735_CYAN;
inline constexpr uint16_t COLOR_GRIS = ST7735_GRIS;