#include "bluetooth.h"
#include "alarma.h"
#include "pantalla.h"
#include "leds.h"
#include "audio.h"
#include "configuracion.h"

HardwareSerial BTSerial(BT_RX, BT_TX);
Bluetooth bt;
char nombreBT[30];
String comandoBluetoothRecibido = "";

extern Configuracion config;

void Bluetooth::begin(HardwareSerial &serial, uint32_t baudrate)
{
    _serial = &serial;
    _connected = false;
    _dentroDeComando = false;
    _rxBuffer = "";
    _lastCommand = "";

    if (baudrate > 0)
        _serial->begin(baudrate);
}

// callbacks de conexión
void Bluetooth::_onConectado()
{
    DEBUG_PRINTLN("[BT] Conectado");
    // delay(20);

    _connected = true;
    _dentroDeComando = false;
    _rxBuffer = "";
    _lastCommand = "";

    // eliminar cualquier byte residual que el módulo envíe tras OK+CONN
    delay(200);
    while (_serial->available())
        _serial->read();

    delay(300);                   // dar tiempo al módulo BT
    sendString("OK_CONECTADO\n"); // respuesta que espera la app
}

void Bluetooth::_onDesconectado()
{
    DEBUG_PRINTLN("[BT] Desconectado");
    delay(20);

    _connected = false;
    _dentroDeComando = false;
    _rxBuffer = "";
    _lastCommand = "";
}

// procesar byte a byte
void Bluetooth::_procesarByte(char c)
{
    // detectar datos del módulo BT (pueden llegar en cualquier momento)
    // se acumulan en un buffer auxiliar estático para detectar el patrón completo
    static String notifBuf = "";

    if (!_dentroDeComando)
    {
        // primero comprobar inicio de comando, antes de acumular en notifBuf
        if (c == BT_CMD_START)
        {
            _dentroDeComando = true;
            _rxBuffer = "";
            notifBuf = ""; // limpiar el buffer
            return;
        }

        notifBuf += c;

        // DEBUG_PRINTLN(notifBuf);

        // mantener solo los últimos N chars necesarios para detectar el patrón
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

        // inicio de comando
        if (c == BT_CMD_START)
        {
            _dentroDeComando = true;
            _rxBuffer = "";
            notifBuf = "";
        }
        return;
    }

    // detectar el final del comando (_comando#)
    if (c == BT_CMD_END)
    {
        // Comando completo recibido
        _rxBuffer.trim();
        if (_rxBuffer.length() > 0)
        {
            _lastCommand = _rxBuffer;
            DEBUG_PRINTLN("[BT] Comando: " + _lastCommand);
        }
        _rxBuffer = "";
        _dentroDeComando = false;
        return;
    }

    if (c == BT_CMD_START)
    {
        // nuevo '_' dentro de un comando → reiniciar (byte perdido anterior)
        _rxBuffer = "";
        return;
    }

    // acumular byte del comando
    if (_rxBuffer.length() < BT_BUFFER_SIZE)
        _rxBuffer += c;
}

void Bluetooth::update()
{
    while (_serial->available())
        _procesarByte((char)_serial->read());
}

// comandos recibidos
bool Bluetooth::hasCommand()
{
    return _lastCommand.length() > 0;
}

String Bluetooth::getCommand()
{
    String cmd = _lastCommand;
    _lastCommand = "";
    return cmd;
}

// estado
bool Bluetooth::isConnected() { return _connected; }

// envío
void Bluetooth::sendString(const char *str) { _serial->print(str); }
void Bluetooth::sendString(const String &str) { _serial->print(str); }
void Bluetooth::sendByte(uint8_t byte) { _serial->write(byte); }
void Bluetooth::sendData(const uint8_t *data, uint16_t len) { _serial->write(data, len); }

// recepción
bool Bluetooth::available() { return _serial->available() > 0; }
uint8_t Bluetooth::readByte() { return (uint8_t)_serial->read(); }
String Bluetooth::readLine() { return _serial->readStringUntil('\n'); }

String Bluetooth::readString()
{
    String result = "";
    while (_serial->available())
        result += (char)_serial->read();
    return result;
}

String Bluetooth::readAll() { return readString(); }

// comandos AT
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

// autoconfiguración
bool Bluetooth::autoConfig(char *nombre, const char *pin)
{
    String response;
    bool found = false;

    DEBUG_PRINTLN("[BT] Probando 9600");
    _serial->end();
    _serial->begin(BT_BAUD_DEFAULT);
    delay(500);

    if (sendAT("AT", response) && response.indexOf("OK") >= 0)
    {
        DEBUG_PRINTLN("[BT] Encontrado a 9600, migrando a 115200...");
        sendAT("AT+BAUD8", response);
        delay(250);
        found = true;
    }

    DEBUG_PRINTLN("[BT] Probando 115200");
    _serial->end();
    _serial->begin(BT_BAUDRATE);
    delay(250);

    if (sendAT("AT", response) && response.indexOf("OK") >= 0)
    {
        DEBUG_PRINTLN("[BT] Modulo listo a 115200");
        found = true;
    }

    if (!found)
    {
        DEBUG_PRINTLN("[BT] Modulo no encontrado");
        return false;
    }

    bool ok = true;
    ok &= sendAT("AT+DEFAULT", response);
    sendAT("AT+BAUD8", response);
    delay(250);
    ok &= sendAT((String("AT+NAME") + nombre).c_str(), response);
    ok &= sendAT((String("AT+PIN") + pin).c_str(), response);
    ok &= sendAT("AT+TYPE0", response); // modo normal (puerto serie)
    ok &= sendAT("AT+ROLE0", response); // modo esclavo
    ok &= sendAT("AT+NOTI1", response); // envía por uart el aviso de conexión/desconexión
    ok &= sendAT("AT+RESET", response); // reinicia para guardar

    // esperar arranque tras reset y vaciar basura
    delay(1500);
    while (_serial->available())
        _serial->read();

    // resetear estado interno
    _connected = false;
    _dentroDeComando = false;
    _rxBuffer = "";
    _lastCommand = "";

    DEBUG_PRINTLN(ok ? "[BT] Listo" : "[BT] Error configuración");
    return ok;
}

void _actualizarFechaHora(String &fechaHora)
{
    // DEBUG_PRINTLN(fechaHora);
    // ddmmyyhhmmssx
    if (fechaHora.length() < 13)
    {
        DEBUG_PRINTLN("Error: Formato invalido - longitud incorrecta");
        return;
    }

    // Extraer
    uint8_t d = (fechaHora[11+0] - '0') * 10 + (fechaHora[11+1] - '0');   // Día (01-31)
    uint8_t mes = (fechaHora[11+2] - '0') * 10 + (fechaHora[11+3] - '0'); // Mes (01-12)
    uint8_t yy = (fechaHora[11+4] - '0') * 10 + (fechaHora[11+5] - '0');  // Año (2 dígitos)
    uint8_t h = (fechaHora[11+6] - '0') * 10 + (fechaHora[11+7] - '0');   // Hora (00-23)
    uint8_t min = (fechaHora[11+8] - '0') * 10 + (fechaHora[11+9] - '0'); // Minuto (00-59)
    uint8_t s = (fechaHora[11+10] - '0') * 10 + (fechaHora[11+11] - '0'); // Segundo (00-59)
    uint8_t diaSemana = fechaHora[11+12] - '0';                        // Día semana (1-7)
    uint16_t a = 2000 + yy;

    DEBUG_PRINTF("d=%d, mes=%d, yy=%d, a=%d, h=%d, min=%d, s=%d, diaSemana=%d\n",
                  d, mes, yy, a, h, min, s, diaSemana);

    // Mostrar confirmación
    DEBUG_PRINTLN("RTC actualizado correctamente:");
    DEBUG_PRINT("  Fecha: ");
    DEBUG_PRINT(d);
    DEBUG_PRINT("/");
    DEBUG_PRINT(mes);
    DEBUG_PRINT("/");
    DEBUG_PRINTLN(a);
    DEBUG_PRINT("  Hora: ");
    DEBUG_PRINT(h);
    DEBUG_PRINT(":");
    DEBUG_PRINT(min);
    DEBUG_PRINT(":");
    DEBUG_PRINTLN(s);

    // convertir día semana a nombre
    const char *diasSemana[] = {"", "Lunes", "Martes", "Miercoles",
                                "Jueves", "Viernes", "Sabado", "Domingo"};
    DEBUG_PRINT("  Dia: ");
    DEBUG_PRINTLN(diasSemana[diaSemana]);

    delay(50);

    RTC_SetFechaHora(d, mes, a, h, min, s, diaSemana);
}

void procesarComandosBluetooth()
{
    bt.update();

    if (!bt.hasCommand())
        return;

    String cmd = bt.getCommand();

    
    // alarma
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

    // audio
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

    // leds
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

    // sistema
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
    else if (cmd.startsWith("SINCROFECHA"))
    {
        bt.sendString("OK:SINCROFECHA\n");
        _actualizarFechaHora(cmd);
    }

    // desconocido
    else
    {
        DEBUG_PRINTLN("[BT] No reconocido: " + cmd);
        bt.sendString("ERR:CMD_DESCONOCIDO\n");
    }
}
