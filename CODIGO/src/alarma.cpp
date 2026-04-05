#include "alarma.h"

extern Configuracion config;
extern DFRobotDFPlayerMini myDFPlayer;
extern Adafruit_NeoPixel rgb;

void verificarAlarma()
{
    static bool alarmaDisparada = false;

    if (!sistemaEncendido)
        return;

    uint8_t hora, minuto, segundo, dia, mes, anio;
    config.getDateTime(hora, minuto, segundo, dia, mes, anio);

    if (config.get().alarmaActivada &&
        hora == config.get().alarmaHora &&
        minuto == config.get().alarmaMinuto)
    {
        if (!alarmaDisparada)
        {
            alarmaDisparada = true;
            activarAlarma();
        }
    }
    else
    {
        alarmaDisparada = false;
    }
}

void activarAlarma()
{
    Serial.println("¡ALARMA ACTIVADA!");
    alarmaSonando = true;

    uint8_t volumenOriginal = config.get().volumen;

    // Subir volumen gradualmente
    for (int v = 0; v <= config.get().volumen; v++)
    {
        myDFPlayer.volume(v);
        delay(50);
    }

    // Reproducir sonido de alarma
    myDFPlayer.play(999);

    // LEDs rojo pulsante
    unsigned long inicio = millis();
    while (alarmaSonando && millis() - inicio < 300000) // 5 minutos máximo
    {
        rgb.fill(rgb.Color(255, 0, 0));
        rgb.show();
        delay(500);
        rgb.clear();
        rgb.show();
        delay(500);

        if (desactivarAlarmaPorUsuario())
        {
            break;
        }
    }

    // Restaurar volumen
    myDFPlayer.volume(volumenOriginal);
    myDFPlayer.stop();
    alarmaSonando = false;
}

bool desactivarAlarmaPorUsuario()
{
    // Verificar botón físico
    if (digitalRead(BOTON_DESACTIVAR) == LOW)
    {
        return true;
    }

    // Verificar comando Bluetooth
    if (comandoBluetoothRecibido == "STOP_ALARM")
    {
        comandoBluetoothRecibido = ""; // Limpiar comando
        return true;
    }

    return false;
}

void verificarApagadoProgramado()
{
    if (config.get().autoApagado == 0)
        return;
    if (!sistemaEncendido)
        return;

    unsigned long tiempoEncendido = (millis() - tiempoInicio) / 60000; // en minutos

    if (tiempoEncendido >= config.get().autoApagado)
    {
        Serial.println("Apagado programado activado");
        apagarSistema();
    }
}

void verificarEncendidoProgramado()
{
    if (sistemaEncendido)
        return;

    uint8_t hora, minuto, segundo, dia, mes, anio;
    config.getDateTime(hora, minuto, segundo, dia, mes, anio);

    if (config.get().alarmaActivada &&
        hora == config.get().alarmaHora &&
        minuto == config.get().alarmaMinuto)
    {
        Serial.println("Encendido programado activado");
        encenderSistema();
        delay(5000);
    }
}

void encenderSistema()
{
    sistemaEncendido = true;
    tiempoInicio = millis();

    brilloPantalla(config.get().brillo);
    rgb.setBrightness(config.get().ledBrillo);
    redibujarTodo();
}

void apagarSistema()
{
    sistemaEncendido = false;

    config.save();
    brilloPantalla(0);
    rgb.clear();
    rgb.show();

    entrarEnStandby();
}

void entrarEnStandby()
{
    Serial.println("Entrando en modo standby...");

    config.save();
    myDFPlayer.stop();
    brilloPantalla(0);
    rgb.clear();
    rgb.show();

    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2_LOW);
    HAL_PWR_EnterSTANDBYMode();
}