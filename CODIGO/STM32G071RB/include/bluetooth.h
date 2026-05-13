#pragma once

// ── Bluetooth (HM-10) ─────────────────────

// ── Protocolo de comandos ─────────────────────────────────────
// formato: _comando#
// ejemplos: _VOL UP#   _LED ON#   _MP3 PLAY#

#include <Arduino.h>
#include "pines.h"
#include "globals.h"

inline constexpr uint8_t BT_BUFFER_SIZE = 128;
inline constexpr uint16_t BT_TIMEOUT = 1000;

inline constexpr uint16_t BT_BAUD_DEFAULT = 9600;
inline constexpr uint32_t BT_BAUDRATE = 115200;

inline constexpr const char* BT_NOTIFY_CONN = "OK+CONN";
inline constexpr const char* BT_NOTIFY_LOST = "OK+LOST";

inline constexpr char BT_CMD_START = '_';
inline constexpr char BT_CMD_END = '#';

class Bluetooth
{
public:
    // Inicialización
    void begin(HardwareSerial &serial, uint32_t baudrate = BT_BAUDRATE);
    bool autoConfig(char *nombre, const char *pin);

    // llamar desde loop() en cada iteración
    void update();

    // comandos recibidos
    bool hasCommand();   // true si hay un comando completo pendiente
    String getCommand(); // devuelve el comando (sin _ ni #) y lo borra

    // estado
    bool isConnected();

    // envío
    void sendString(const char *str);
    void sendString(const String &str);
    void sendByte(uint8_t byte);
    void sendData(const uint8_t *data, uint16_t len);

    // recepción
    bool available();
    uint8_t readByte();
    String readString();
    String readLine();
    String readAll();

    // comandos AT
    bool sendAT(const char *cmd, String &response, uint16_t timeout = BT_TIMEOUT);
    bool setName(const char *name);
    bool setPin(const char *pin);
    bool setBaudrate(uint8_t baudrate);
    void reset();

private:
    HardwareSerial *_serial;
    bool _connected;
    bool _dentroDeComando; // true cuando hemos recibido '_'
    String _rxBuffer;      // acumula bytes del comando actual
    String _lastCommand;   // comando completo listo para procesar

    void _procesarByte(char c);
    void _onConectado();
    void _onDesconectado();
    void _actualizarFechaHora(String &fechaHora);
};

extern HardwareSerial BTSerial;
extern Bluetooth bt;
extern char nombreBT[30];
extern String comandoBluetoothRecibido;

void procesarComandosBluetooth();