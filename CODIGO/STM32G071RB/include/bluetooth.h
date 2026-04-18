#pragma once

#include <Arduino.h>
#include "pines.h"
#include "globals.h"

// ── Bluetooth (HM-05 / HC-06 compatible) ─────────────────────

#define BT_BUFFER_SIZE  128
#define BT_TIMEOUT      1000

#define BT_BAUD_DEFAULT 9600
#define BT_BAUDRATE     115200

#define BT_NOTIFY_CONN  "OK+CONN"
#define BT_NOTIFY_LOST  "OK+LOST"

// ── Protocolo de comandos ─────────────────────────────────────
// Todo comando enviado desde la app tiene la forma: _COMANDO#
// Ejemplo: _VOL UP#   _LED ON#   _MP3 PLAY#
#define BT_CMD_START    '_'
#define BT_CMD_END      '#'

class Bluetooth
{
public:
    // ── Inicialización ────────────────────────────────────────
    void begin(HardwareSerial &serial, uint32_t baudrate = BT_BAUDRATE);
    bool autoConfig(char *nombre, const char *pin);

    // ── Loop — llamar en cada iteración del loop() ────────────
    void update();

    // ── Comandos recibidos ────────────────────────────────────
    bool   hasCommand();  // true si hay un comando completo pendiente
    String getCommand();  // devuelve el comando (sin _ ni #) y lo borra

    // ── Estado ────────────────────────────────────────────────
    bool isConnected();

    // ── Envío ─────────────────────────────────────────────────
    void sendString(const char *str);
    void sendString(const String &str);
    void sendByte(uint8_t byte);
    void sendData(const uint8_t *data, uint16_t len);

    // ── Recepción raw (uso interno / AT) ──────────────────────
    bool    available();
    uint8_t readByte();
    String  readString();
    String  readLine();
    String  readAll();

    // ── Comandos AT ───────────────────────────────────────────
    bool sendAT(const char *cmd, String &response, uint16_t timeout = BT_TIMEOUT);
    bool setName(const char *name);
    bool setPin(const char *pin);
    bool setBaudrate(uint8_t baudrate);
    void reset();

private:
    HardwareSerial *_serial;
    bool            _connected;
    bool            _dentroDeComando;  // true cuando hemos recibido '_'
    String          _rxBuffer;         // acumula bytes del comando actual
    String          _lastCommand;      // comando completo listo para procesar

    void _procesarByte(char c);
    void _onConectado();
    void _onDesconectado();
};

extern HardwareSerial BTSerial;
extern Bluetooth      bt;
extern char           nombreBT[30];
extern String         comandoBluetoothRecibido;

// ── Funciones globales ────────────────────────────────────────
void procesarComandosBluetooth();