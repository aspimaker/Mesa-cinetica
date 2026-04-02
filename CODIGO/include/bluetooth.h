#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>

extern char nombreBT[30];
extern const char *pinBT;

#define BT_BAUDRATE 115200
#define BT_BUFFER_SIZE 128
#define BT_TIMEOUT 1000

// Baudrates a probar en orden
#define BT_BAUD_DEFAULT 9600
#define BT_BAUD_TARGET 115200

// Cadenas de notificación del HM-05
#define BT_NOTIFY_CONN "OK+CONN"
#define BT_NOTIFY_LOST "OK+LOST"

class Bluetooth
{
public:
    // ── Inicialización ────────────────────────────────────────
    void begin(HardwareSerial &serial, uint32_t baudrate = BT_BAUDRATE);
    bool enableNotifications(); // Envía AT+NOTI1
    
    // ── Autoconfiguración ─────────────────────────────────────
   // bool autoConfig(const char *name = nombreBT, const char *pin = pinBT);

    bool autoConfig(char *name, const char *pin = "123456");

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

    // ── Llama a esto en cada iteración del loop() ─────────────
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

#endif // BLUETOOTH_H