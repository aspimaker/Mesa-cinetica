// pines.h (o al inicio de main.cpp)
#pragma once

// pantalla TFT
#define TFT_CS D10
#define TFT_DC D9
#define TFT_RST D8
#define TFT_BLK D6

// DFPlayer
#define DFPLAYER_RX D2
#define DFPLAYER_TX D3

// bluetooth
#define BT_RX A1
#define BT_TX A0

// WS2812B
#define PIN_WS2812B D7

// ── Botones táctiles TTP223 (activo HIGH) ────────────────────
// Usar pinMode(PIN, INPUT), sin pull-up interno.
#define BTN_OK        D4   // Play / Pausa / Confirmar menú
#define BTN_DERECHA   D5   // Siguiente pista
#define BTN_IZQUIERDA A2   // Pista anterior
#define BTN_ARRIBA    A3   // Subir (volumen / menú arriba)
#define BTN_ABAJO     A4   // Bajar  (volumen / menú abajo)

// Alias para la alarma: el botón OK la desactiva
#define BOTON_DESACTIVAR BTN_OK

