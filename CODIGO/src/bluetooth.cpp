#include "bluetooth.h"

// ── Inicialización ────────────────────────────────────────────
void Bluetooth::begin(HardwareSerial &serial, uint32_t baudrate)
{
    _serial = &serial;
    _serial->begin(baudrate);
    _connected = false;
    _rxBuffer = "";
}

bool Bluetooth::enableNotifications()
{
    String response;
    return sendAT("AT+NOTI1", response);
}

// ── Update (llamar en cada loop) ──────────────────────────────
void Bluetooth::update()
{
    while (_serial->available())
    {
        char c = (char)_serial->read();
        _rxBuffer += c;

        // Procesar cuando llega fin de línea o la cadena es reconocible
        if (c == '\n' || _rxBuffer.length() >= BT_BUFFER_SIZE)
        {
            _processBuffer();
        }
    }

    // También procesar si llevamos un rato sin recibir más datos
    // (el HM-05 no siempre manda '\n' al final de la notificación)
    if (_rxBuffer.length() > 0 && !_serial->available())
    {
        _processBuffer();
    }
}

void Bluetooth::_processBuffer()
{
    if (_rxBuffer.indexOf(BT_NOTIFY_CONN) >= 0)
    {
        _connected = true;
    }
    else if (_rxBuffer.indexOf(BT_NOTIFY_LOST) >= 0)
    {
        _connected = false;
    }
    _rxBuffer = "";
}

// ── Envío ─────────────────────────────────────────────────────
void Bluetooth::sendString(const char *str)
{
    _serial->print(str);
}

void Bluetooth::sendString(const String &str)
{
    _serial->print(str);
}

void Bluetooth::sendByte(uint8_t byte)
{
    _serial->write(byte);
}

void Bluetooth::sendData(const uint8_t *data, uint16_t len)
{
    _serial->write(data, len);
}

// ── Recepción ─────────────────────────────────────────────────
bool Bluetooth::available()
{
    return _serial->available() > 0;
}

uint8_t Bluetooth::readByte()
{
    return (uint8_t)_serial->read();
}

String Bluetooth::readLine()
{
    return _serial->readStringUntil('\n');
}

String Bluetooth::readAll()
{
    String result = "";
    while (_serial->available())
    {
        result += (char)_serial->read();
    }
    return result;
}

// ── Comandos AT ───────────────────────────────────────────────
bool Bluetooth::sendAT(const char *cmd, String &response, uint16_t timeout)
{
    while (_serial->available())
        _serial->read();

    _serial->print(cmd);
    _serial->print("\r\n");

    uint32_t start = millis();
    response = "";

    while (millis() - start < timeout)
    {
        if (_serial->available())
        {
            char c = (char)_serial->read();
            response += c;

            if (response.indexOf("OK") >= 0 || response.indexOf("ERROR") >= 0)
                break;
        }
    }

    return response.indexOf("OK") >= 0;
}

bool Bluetooth::setName(const char *name)
{
    String cmd = String("AT+NAME") + name;
    String response;
    return sendAT(cmd.c_str(), response);
}

bool Bluetooth::setPin(const char *pin)
{
    String cmd = String("AT+PIN") + pin;
    String response;
    return sendAT(cmd.c_str(), response);
}

bool Bluetooth::setBaudrate(uint8_t baudrate)
{
    String cmd = String("AT+UART") + baudrate;
    String response;
    return sendAT(cmd.c_str(), response);

    /*
    1 ->   1200
    2 ->   2400
    3 ->   4800
    4 ->   9600
    5 ->  19200
    6 ->  38400
    7 ->  57600
    8 -> 115200
    */
}

// ── Estado ────────────────────────────────────────────────────
bool Bluetooth::isConnected()
{
    return _connected;
}

void Bluetooth::reset()
{
    String response;
    sendAT("AT+RESET", response);
}

// ── Autoconfiguración ─────────────────────────────────────
bool Bluetooth::autoConfig(char *name, const char *pin)
{
    String response;
    bool found = false;

    Serial.println("[BT] Probando 9600");

    // 1. Probar a 9600
    _serial->end();
    _serial->begin(BT_BAUD_DEFAULT);
    delay(500);

    if (sendAT("AT", response) && response.indexOf("OK") >= 0)
    {
        Serial.println("[BT] Encontrado a 9600, migrando a 115200...");
        sendAT("AT+BAUD8", response);
        delay(500); // más margen tras cambio de baudrate
        found = true;
    }

    Serial.println("[BT] Probando 115200");
    // 2. Verificar a 115200
    _serial->end();
    _serial->begin(BT_BAUD_TARGET);
    delay(500);

    if (sendAT("AT", response) && response.indexOf("OK") >= 0)
    {
        Serial.println("[BT] Modulo listo a 115200");
        found = true;
    }

    Serial.println(response);

    if (!found)
    {
        Serial.println("[BT] Modulo no encontrado");
        return false; // salida temprana
    }

    // 3. Aplicar configuración
    bool ok = true;
    String cmd;

    /*
    Serial.println("cambiando a 9600");
    delay(1000);
    cmd = String("AT+DEFAULT");
    sendAT(cmd.c_str(), response);
    Serial.print(response);
    cmd = String("AT+RESET");
    sendAT(cmd.c_str(), response);
    Serial.print(response);
    Serial.print("TERMINADO");
    delay(500000);
*/

    cmd = String("AT+DEFAULT");
    ok &= sendAT(cmd.c_str(), response);
    Serial.print(response);

    cmd = String("AT+NAME") + name;
    ok &= sendAT(cmd.c_str(), response);
    Serial.print(response);

    cmd = String("AT+PIN") + pin;
    ok &= sendAT(cmd.c_str(), response);
    Serial.print(response);

    ok &= sendAT("AT+TYPE0", response);
    Serial.print(response);

    ok &= sendAT("AT+ROLE0", response);
    Serial.print(response);

    ok &= sendAT("AT+NOTI1", response);
    Serial.println(response);

    Serial.println(ok ? "[BT] Configuración aplicada" : "[BT] Error aplicando configuración");

    return ok;
}