#pragma once

#include <Arduino.h>
#include "pines.h"
#include "globals.h"

// ── Bluetooth (HM-05 / HC-06 compatible) ─────────────────────

#define BT_BAUDRATE     115200
#define BT_BUFFER_SIZE  128
#define BT_TIMEOUT      1000

#define BT_BAUD_DEFAULT 9600
#define BT_BAUD_TARGET  115200

#define BT_NOTIFY_CONN "OK+CONN"
#define BT_NOTIFY_LOST "OK+LOST"

class Bluetooth
{
public:
    // ── Inicialización ────────────────────────────────────────
    void begin(HardwareSerial &serial, uint32_t baudrate = BT_BAUDRATE);
    //void autoConfig(const char *nombre, const char *pin);
    String readString();

    bool autoConfig(char *nombre, const char *pin);

    // ── Envío ─────────────────────────────────────────────────
    void sendString(const char *str);
    void sendString(const String &str);
    void sendByte(uint8_t byte);
    void sendData(const uint8_t *data, uint16_t len);

    // ── Recepción ─────────────────────────────────────────────
    bool available();
    uint8_t readByte();
    String readLine();
    String readAll();

    // ── Loop ──────────────────────────────────────────────────
    void update();

    // ── Comandos AT ───────────────────────────────────────────
    bool sendAT(const char *cmd, String &response, uint16_t timeout = BT_TIMEOUT);
    bool setName(const char *name);
    bool setPin(const char *pin);
    bool setBaudrate(uint8_t baudrate);

    // ── Estado ────────────────────────────────────────────────
    bool isConnected();
    void reset();

private:
    HardwareSerial *_serial;
    bool _connected;
    String _rxBuffer;
    void _processBuffer();
};

extern HardwareSerial BTSerial;
extern Bluetooth bt;
extern char nombreBT[30];
extern String comandoBluetoothRecibido;

// ── Funciones de procesamiento (implementadas en bluetooth.cpp) ──
void procesarComandosBluetooth();
bool hayComandoBluetooth();
String leerComandoBluetooth();