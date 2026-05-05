#pragma once

// SPI común
// D11 MOSI
// D12 MISO para la memoria flash, pero no se conecta a la pantalla TFT
// D13 SCK

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

// bluetooth HM-10
#define BT_RX A0
#define BT_TX A1

// ── Botones táctiles TTP223 (activo HIGH) ────────────────────
// Usar pinMode(PIN, INPUT), sin pull-up interno.
#define BTN_ADC A2 //usar diodos y resistencias para multiplexar varios botones en este pin ADC

// motor R
#define MOTOR_R_STEP D4
#define MOTOR_R_DIR D5

// motor θ
#define MOTOR_O_STEP A3
#define MOTOR_O_DIR A4

// enable motores
#define MOTORES_ENABLE D15

// uart motores (para TMC2209)
#define UART_TX_TMC2209 D2
#define UART_RX_TMC2209 D3

// uart para depuración (pines morpho)
//#define UART_TX_DEBUG PB6
//#define UART_RX_DEBUG PB7
