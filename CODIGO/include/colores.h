// colores personalizados para la pantalla TFT ST7735
#pragma once

#define _COLOR565(r,g,b) ((uint16_t)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)))


// ST7735_NAVY
#define ST7735_NAVY _COLOR565(0, 0, 128)

// ST7735_DARKCYAN
#define ST7735_DARKCYAN _COLOR565(0, 139, 139)

// ST7735_DARKGREEN
#define ST7735_DARKGREEN _COLOR565(0, 100, 0)

// ST7735_ORANGE
#define ST7735_ORANGE _COLOR565(255, 165, 0)

#define ST7735_NARANJA _COLOR565(255, 166, 0)

#define ST7735_PURPURA _COLOR565(107, 105, 238)

#define ST7735_GREY 0x7BEF


/*
    RGBColor rgb565[] = {
        {0x0000, "NEGRO", "0x0000"},
        {0xFFFF, "BLANCO", "0xFFFF"},
        {0xF800, "ROJO", "0xF800"},
        {0x07E0, "VERDE", "0x07E0"},
        {0x001F, "AZUL", "0x001F"},
        {0xFFE0, "AMARILLO", "0xFFE0"},
        {0xF81F, "MAGENTA", "0xF81F"},
        {0x07FF, "CYAN", "0x07FF"},
        {0xFD20, "NARANJA", "0xFD20"},
        {0x7BEF, "GRIS CLARO", "0x7BEF"},
        {0x4208, "GRIS OSCURO", "0x4208"},
        {0xBDF7, "PLATA", "0xBDF7"},
        {0xE73C, "ROSA", "0xE73C"},
        {0x9E60, "MARRON", "0x9E60"},
        {0x6B5D, "PURPURA", "0x6B5D"},
        {0x2C80, "VERDE OLIVA", "0x2C80"},
        {0x5DBC, "TURQUESA", "0x5DBC"},
        {0xD69A, "SALMON", "0xD69A"},
        {0xAD5E, "LAVANDA", "0xAD5E"},
        {0xFE00, "ROJO INTENSO", "0xFE00"}};
*/