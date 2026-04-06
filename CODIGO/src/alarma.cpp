#include "alarma.h"
#include "menu.h"
#include "pantalla.h"
#include "globals.h" // Añadir esta línea

extern Configuracion config;
extern DFRobotDFPlayerMini myDFPlayer;
extern Adafruit_NeoPixel rgb;
extern Menu menu;

void verificarAlarma()
{
    static bool alarmaDisparada = false;

    if (!sistemaEncendido)
        return;

    uint8_t hora, minuto, segundo, dia, mes;
    uint16_t año;
    config.getDateTime(hora, minuto, segundo, dia, mes, año);

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

    // Mostrar mensaje de alarma en la barra de estado
    menu.dibujarBarraEstado("!!! ALARMA !!!");

    // Subir volumen gradualmente
    for (int v = 0; v <= config.get().volumen; v++)
    {
        myDFPlayer.volume(v);
        delay(50);
    }

    // Reproducir sonido de alarma
    reproducirPista(3, 1);

    // LEDs rojo pulsante y verificar desactivación
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

    // Restaurar la interfaz
    redibujarInterfaz();
}

bool desactivarAlarmaPorUsuario()
{
    // Verificar botón físico BTN_OK (TTP223 activo HIGH)
    if (digitalRead(BOTON_DESACTIVAR) == HIGH)
    {
        Serial.println("Alarma desactivada por botón OK");
        return true;
    }

    // Verificar comando Bluetooth
    if (comandoBluetoothRecibido == "STOP_ALARM")
    {
        comandoBluetoothRecibido = "";
        Serial.println("Alarma desactivada por Bluetooth");
        return true;
    }

    return false;
}

void redibujarInterfaz()
{
    tft.fillScreen(ST7735_BLACK);
    menu.dibujarBarraEstado("MENU PRINCIPAL");
    menu.dibujarIconos();
    menu.dibujarSeleccion(menu.getIconoActivo());
}

void verificarApagadoProgramado()
{
    if (config.get().autoApagado == 0)
        return;
    if (!sistemaEncendido)
        return;

    unsigned long tiempoEncendido = (millis() - tiempoInicio) / 60000;

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

    uint8_t hora, minuto, segundo, dia, mes;
    uint16_t año;
    config.getDateTime(hora, minuto, segundo, dia, mes, año);

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
    redibujarInterfaz();
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