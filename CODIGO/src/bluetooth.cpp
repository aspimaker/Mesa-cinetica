#include "bluetooth.h"
#include "alarma.h"
#include "pantalla.h"
#include "leds.h"
#include "audio.h"
#include "configuracion.h"

// ── Objetos globales ──────────────────────────────────────────
HardwareSerial BTSerial(BT_RX, BT_TX);
Bluetooth bt;
char nombreBT[30];
String comandoBluetoothRecibido = "";

extern Configuracion config;
extern DFRobotDFPlayerMini myDFPlayer;

// ── Inicialización ────────────────────────────────────────────
void Bluetooth::begin(HardwareSerial &serial, uint32_t baudrate)
{
    _serial = &serial;
    _serial->begin(baudrate);
    _connected = false;
    _rxBuffer = "";
}

// ── Update (llamar en cada loop) ──────────────────────────────
void Bluetooth::update()
{
    while (_serial->available())
    {
        char c = (char)_serial->read();
        _rxBuffer += c;

        if (c == '\n' || _rxBuffer.length() >= BT_BUFFER_SIZE)
            _processBuffer();
    }

    if (_rxBuffer.length() > 0 && !_serial->available())
        _processBuffer();
}

void Bluetooth::_processBuffer()
{
    if (_rxBuffer.indexOf(BT_NOTIFY_CONN) >= 0)
        _connected = true;
    else if (_rxBuffer.indexOf(BT_NOTIFY_LOST) >= 0)
        _connected = false;

    _rxBuffer = "";
}

// ── Envío ─────────────────────────────────────────────────────
void Bluetooth::sendString(const char *str)  { _serial->print(str); }
void Bluetooth::sendString(const String &str) { _serial->print(str); }
void Bluetooth::sendByte(uint8_t byte)        { _serial->write(byte); }

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

String Bluetooth::readString()
{
    String result = "";
    while (_serial->available())
        result += (char)_serial->read();
    return result;
}

String Bluetooth::readAll()
{
    String result = "";
    while (_serial->available())
        result += (char)_serial->read();
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
bool Bluetooth::isConnected() { return _connected; }

void Bluetooth::reset()
{
    String response;
    sendAT("AT+RESET", response);
}

// ── Autoconfiguración ─────────────────────────────────────────
bool Bluetooth::autoConfig(char *name, const char *pin)
{
    String response;
    bool found = false;

    Serial.println("[BT] Probando 9600");
    _serial->end();
    _serial->begin(BT_BAUD_DEFAULT);
    delay(500);

    if (sendAT("AT", response) && response.indexOf("OK") >= 0)
    {
        Serial.println("[BT] Encontrado a 9600, migrando a 115200...");
        sendAT("AT+BAUD8", response);
        delay(500);
        found = true;
    }

    Serial.println("[BT] Probando 115200");
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
        return false;
    }

    bool ok = true;
    String cmd;

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

// ── Funciones de comandos ─────────────────────────────────────

bool hayComandoBluetooth()
{
    if (bt.available())
    {
        comandoBluetoothRecibido = bt.readString();
        comandoBluetoothRecibido.trim();
        return comandoBluetoothRecibido.length() > 0;
    }
    return false;
}

String leerComandoBluetooth()
{
    return comandoBluetoothRecibido;
}

void procesarComandosBluetooth()
{
    if (!hayComandoBluetooth()) return;

    String cmd = leerComandoBluetooth();
    Serial.print("[BT] Comando recibido: ");
    Serial.println(cmd);

    // ── Alarma ────────────────────────────────────────────────
    if (cmd.equalsIgnoreCase("ALARMA ON"))
    {
        activarAlarma();
        bt.sendString("OK:ALARMA_ON\n");
    }
    else if (cmd.equalsIgnoreCase("ALARMA OFF"))
    {
        desactivarAlarmaPorUsuario();
        bt.sendString("OK:ALARMA_OFF\n");
    }

    // ── Audio ─────────────────────────────────────────────────
    else if (cmd.equalsIgnoreCase("MP3 PLAY"))
    {
        reproducirPista(config.get().ultimaPista, config.get().ultimaCarpeta);
        bt.sendString("OK:MP3_PLAY\n");
    }
    else if (cmd.equalsIgnoreCase("MP3 STOP"))
    {
        detenerMP3();
        bt.sendString("OK:MP3_STOP\n");
    }
    else if (cmd.equalsIgnoreCase("MP3 NEXT"))
    {
        uint16_t pista = config.get().ultimaPista + 1;
        reproducirPista(pista, config.get().ultimaCarpeta);
        bt.sendString("OK:MP3_NEXT\n");
    }
    else if (cmd.equalsIgnoreCase("MP3 PREV"))
    {
        uint16_t pista = config.get().ultimaPista;
        reproducirPista(pista > 1 ? pista - 1 : 1, config.get().ultimaCarpeta);
        bt.sendString("OK:MP3_PREV\n");
    }
    else if (cmd.startsWith("VOL "))
    {
        int nivel = cmd.substring(4).toInt();
        nivel = constrain(nivel, 0, 30);
        volumenMP3((uint8_t)nivel);
        config.setVolumen((uint8_t)nivel);
        bt.sendString("OK:VOL_" + String(nivel) + "\n");
    }
    else if (cmd.equalsIgnoreCase("VOL UP"))
    {
        subirVolumenMP3();
        bt.sendString("OK:VOL_UP\n");
    }
    else if (cmd.equalsIgnoreCase("VOL DOWN"))
    {
        bajarVolumenMP3();
        bt.sendString("OK:VOL_DOWN\n");
    }

    // ── LEDs ──────────────────────────────────────────────────
    else if (cmd.equalsIgnoreCase("LED ON"))
    {
        config.setLedModo(2);
        actualizarLEDs();
        bt.sendString("OK:LED_ON\n");
    }
    else if (cmd.equalsIgnoreCase("LED OFF"))
    {
        config.setLedModo(0);
        actualizarLEDs();
        bt.sendString("OK:LED_OFF\n");
    }
    else if (cmd.startsWith("LED BRILLO "))
    {
        int brillo = cmd.substring(11).toInt();
        config.setLedBrillo((uint8_t)constrain(brillo, 0, 255));
        actualizarLEDs();
        bt.sendString("OK:LED_BRILLO_" + String(brillo) + "\n");
    }

    // ── Sistema ───────────────────────────────────────────────
    else if (cmd.equalsIgnoreCase("APAGAR"))
    {
        bt.sendString("OK:APAGANDO\n");
        apagarSistema();
    }
    else if (cmd.equalsIgnoreCase("ENCENDER"))
    {
        encenderSistema();
        bt.sendString("OK:ENCENDIDO\n");
    }
    else if (cmd.equalsIgnoreCase("STANDBY"))
    {
        bt.sendString("OK:STANDBY\n");
        entrarEnStandby();
    }

    // ── Desconocido ───────────────────────────────────────────
    else
    {
        Serial.print("[BT] Comando no reconocido: ");
        Serial.println(cmd);
        bt.sendString("ERR:CMD_DESCONOCIDO\n");
    }
}