#include "bluetooth.h"
#include "alarma.h"
#include "pantalla.h"
#include "leds.h"
#include "audio.h"
#include "configuracion.h"

// ── Objetos globales ──────────────────────────────────────────
HardwareSerial BTSerial(BT_RX, BT_TX);
Bluetooth      bt;
char           nombreBT[30];
String         comandoBluetoothRecibido = "";

extern Configuracion config;

// ── Inicialización ────────────────────────────────────────────
void Bluetooth::begin(HardwareSerial &serial, uint32_t baudrate)
{
    _serial           = &serial;
    _connected        = false;
    _dentroDeComando  = false;
    _rxBuffer         = "";
    _lastCommand      = "";

    if (baudrate > 0)
        _serial->begin(baudrate);
}

// ── Callbacks de conexión ─────────────────────────────────────
void Bluetooth::_onConectado()
{
    _connected       = true;
    _dentroDeComando = false;
    _rxBuffer        = "";
    _lastCommand     = "";
    Serial.println("[BT] Conectado");
    delay(500);                    // dar tiempo al módulo BT
    sendString("OK_CONECTADO\n"); // respuesta que espera la app
}

void Bluetooth::_onDesconectado()
{
    _connected       = false;
    _dentroDeComando = false;
    _rxBuffer        = "";
    _lastCommand     = "";
    Serial.println("[BT] Desconectado");
}

// ── Procesado byte a byte ─────────────────────────────────────
void Bluetooth::_procesarByte(char c)
{
    // Detectar notificaciones del módulo BT (pueden llegar en cualquier momento)
    // Las acumulamos en un buffer auxiliar estático para detectar el patrón completo
    static String notifBuf = "";

    if (!_dentroDeComando)
    {
        notifBuf += c;

        // Mantener solo los últimos N chars necesarios para detectar el patrón
        if (notifBuf.length() > 8)
            notifBuf = notifBuf.substring(notifBuf.length() - 8);

        if (notifBuf.endsWith(BT_NOTIFY_CONN))
        {
            notifBuf = "";
            _onConectado();
            return;
        }
        if (notifBuf.endsWith(BT_NOTIFY_LOST))
        {
            notifBuf = "";
            _onDesconectado();
            return;
        }

        // Inicio de comando
        if (c == BT_CMD_START)
        {
            _dentroDeComando = true;
            _rxBuffer        = "";
            notifBuf         = "";
        }
        return;
    }

    // Estamos dentro de un comando (_CMD#)
    if (c == BT_CMD_END)
    {
        // Comando completo recibido
        _rxBuffer.trim();
        if (_rxBuffer.length() > 0)
        {
            _lastCommand     = _rxBuffer;
            Serial.println("[BT] Comando: " + _lastCommand);
        }
        _rxBuffer        = "";
        _dentroDeComando = false;
        return;
    }

    if (c == BT_CMD_START)
    {
        // Nuevo '_' dentro de un comando → reiniciar (byte perdido anterior)
        _rxBuffer = "";
        return;
    }

    // Acumular byte del comando
    if (_rxBuffer.length() < BT_BUFFER_SIZE)
        _rxBuffer += c;
}

// ── Update ────────────────────────────────────────────────────
void Bluetooth::update()
{
    while (_serial->available())
        _procesarByte((char)_serial->read());
}

// ── Comandos recibidos ────────────────────────────────────────
bool Bluetooth::hasCommand()
{
    return _lastCommand.length() > 0;
}

String Bluetooth::getCommand()
{
    String cmd   = _lastCommand;
    _lastCommand = "";
    return cmd;
}

// ── Estado ────────────────────────────────────────────────────
bool Bluetooth::isConnected() { return _connected; }

// ── Envío ─────────────────────────────────────────────────────
void Bluetooth::sendString(const char *str)                  { _serial->print(str); }
void Bluetooth::sendString(const String &str)                { _serial->print(str); }
void Bluetooth::sendByte(uint8_t byte)                       { _serial->write(byte); }
void Bluetooth::sendData(const uint8_t *data, uint16_t len)  { _serial->write(data, len); }

// ── Recepción raw ─────────────────────────────────────────────
bool    Bluetooth::available() { return _serial->available() > 0; }
uint8_t Bluetooth::readByte()  { return (uint8_t)_serial->read(); }
String  Bluetooth::readLine()  { return _serial->readStringUntil('\n'); }

String Bluetooth::readString()
{
    String result = "";
    while (_serial->available())
        result += (char)_serial->read();
    return result;
}

String Bluetooth::readAll() { return readString(); }

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
}

void Bluetooth::reset()
{
    String response;
    sendAT("AT+RESET", response);
}

// ── Autoconfiguración ─────────────────────────────────────────
bool Bluetooth::autoConfig(char *nombre, const char *pin)
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
        delay(250);
        found = true;
    }

    Serial.println("[BT] Probando 115200");
    _serial->end();
    _serial->begin(BT_BAUDRATE);
    delay(250);

    if (sendAT("AT", response) && response.indexOf("OK") >= 0)
    {
        Serial.println("[BT] Modulo listo a 115200");
        found = true;
    }

    if (!found)
    {
        Serial.println("[BT] Modulo no encontrado");
        return false;
    }

    bool ok = true;
    ok &= sendAT("AT+DEFAULT", response);
    sendAT("AT+BAUD8", response);
    delay(250);
    ok &= sendAT((String("AT+NAME") + nombre).c_str(), response);
    ok &= sendAT((String("AT+PIN")  + pin).c_str(),    response);
    ok &= sendAT("AT+TYPE0", response);
    ok &= sendAT("AT+ROLE0", response);
    ok &= sendAT("AT+NOTI1", response);
    ok &= sendAT("AT+RESET", response);  // reinicia para guardar

    // Esperar arranque tras reset y vaciar basura
    delay(1500);
    while (_serial->available())
        _serial->read();

    // Resetear estado interno
    _connected       = false;
    _dentroDeComando = false;
    _rxBuffer        = "";
    _lastCommand     = "";

    Serial.println(ok ? "[BT] Listo" : "[BT] Error configuración");
    return ok;
}

// ── Procesamiento de comandos ─────────────────────────────────
void procesarComandosBluetooth()
{
    bt.update();

    if (!bt.hasCommand())
        return;

    String cmd = bt.getCommand();

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
        reproducirPista(config.get().ultimaPista + 1, config.get().ultimaCarpeta);
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
        int nivel = constrain(cmd.substring(4).toInt(), 0, 30);
        volumenMP3((uint8_t)nivel);
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
        bt.sendString("OK:LED_ON\n");
    }
    else if (cmd.equalsIgnoreCase("LED OFF"))
    {
        config.setLedModo(0);
        bt.sendString("OK:LED_OFF\n");
    }
    else if (cmd.startsWith("LED BRILLO "))
    {
        int brillo = constrain(cmd.substring(11).toInt(), 0, 255);
        config.setLedBrillo((uint8_t)brillo);
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
        Serial.println("[BT] No reconocido: " + cmd);
        bt.sendString("ERR:CMD_DESCONOCIDO\n");
    }
}