// pines.h (o al inicio de main.cpp)
#pragma once

// SPI común
// D11 MOSI
// #define MISO D12
// D13 SCK

// depuración usb
//  D2
//  D3

// DFPlayer
#define DFPLAYER_RX D0
#define DFPLAYER_TX D1
#define DFPLAYER_BUSY A5 // LOW = reproduciendo, HIGH = parado

// WS2812B
#define PIN_WS2812B D6

// pantalla TFT (no necesita MISO porque es de sólo lectura)
#define TFT_BLK D7
#define TFT_RST D8 // se puede poner el pin de control de iluminación a 3V3 y liberar el pin del micro
#define TFT_DC D9
#define TFT_CS D10

// memoria flash W25Q128JVSIM
#define FLASH_CS D14

// bluetooth
#define BT_RX A1
#define BT_TX A0

// ── Botones táctiles TTP223 (activo HIGH) ────────────────────
// Usar pinMode(PIN, INPUT), sin pull-up interno.
#define BTN_ADC A2
// #define BTN_OK        D4   // Play / Pausa / Confirmar menú
// #define BTN_DERECHA   D5   // Siguiente pista
// #define BTN_IZQUIERDA A2   // Pista anterior
// #define BTN_ARRIBA    A3   // Subir (volumen / menú arriba)
// #define BTN_ABAJO     A4   // Bajar  (volumen / menú abajo)

// Alias para la alarma: el botón OK la desactiva
// #define BOTON_DESACTIVAR BTN_OK

// motor R
#define MOTOR_R_STEP D4
#define MOTOR_R_DIR D5

// motor θ
#define MOTOR_O_STEP A3
#define MOTOR_O_DIR A4 // PB15

// enable motores
#define MOTORES_ENABLE D15
