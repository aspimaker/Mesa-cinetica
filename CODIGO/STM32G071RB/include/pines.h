#pragma once
#include <Arduino.h>

namespace Pinout {

    // --- Bus SPI (Compartido) ---
    namespace SPI {
        inline constexpr uint8_t MOSI = D11;
        inline constexpr uint8_t MISO = D12; // Solo para Flash
        inline constexpr uint8_t SCK  = D13;
    }

    // --- Reproductor Audio (DFPlayer) ---
    namespace Audio {
        inline constexpr uint8_t RX   = D0;
        inline constexpr uint8_t TX   = D1;
        inline constexpr uint8_t BUSY = A5; // LOW: reproduciendo, HIGH: stop
    }

    // --- Iluminación (WS2812B) ---
    inline constexpr uint8_t LED_STRIP = D6;

    // --- Pantalla TFT ---
    namespace TFT {
        inline constexpr uint8_t BLK = D7; // Control retroiluminación
        inline constexpr uint8_t RST = D8; // Reset
        inline constexpr uint8_t DC  = D9;  // Data/Command
        inline constexpr uint8_t CS  = D10; // Chip Select
    }

    // --- Almacenamiento Externo ---
    namespace Flash {
        inline constexpr uint8_t CS = D14;
    }

    // --- Comunicaciones (Bluetooth) ---
    namespace BT {
        inline constexpr uint8_t RX = A0;
        inline constexpr uint8_t TX = A1;
    }

    // --- Interfaz de Usuario ---
    namespace UI {
        inline constexpr uint8_t BTN_ADC = A2; // Multiplexado de los pulsadores táctiles (5 botones conectados a través de resistencias y diodos)
    }

    // --- Control de Movimiento (Motores) ---
    namespace Motores {
        // Motor R (Radial)
        inline constexpr uint8_t R_STEP = D4;
        inline constexpr uint8_t R_DIR  = D5;
        
        // Motor Theta (Rotacional)
        inline constexpr uint8_t O_STEP = A3;
        inline constexpr uint8_t O_DIR  = A4;
        
        // Control de energía
        inline constexpr uint8_t ENABLE = D15;

        // Bus de configuración TMC2209
        namespace UART {
            inline constexpr uint8_t TX = D2;
            inline constexpr uint8_t RX = D3;
        }
    }
}