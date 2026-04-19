#include "alarma.h"
#include "globals.h"
#include "menu.h"
#include "pantalla.h"
#include "audio.h"
#include "leds.h" 
#include "pantalla.h"
#include "menu.h" 
#include "configuracion.h"
#include "botones.h"

extern Configuracion config;
extern DFRobotDFPlayerMini myDFPlayer;
//extern Adafruit_NeoPixel rgb;
//extern Menu menu;

void verificarAlarma()
{
    static bool alarmaDisparada = false;

    if (!sistemaEncendido)
        return;

        FechaHora t;

    //RTC_SetFechaHora(hora, minuto, segundo, dia, mes, año, diaSemana);
    RTC_GetFechaHora(t);


    if (config.get().alarmaActivada &&
        t.horas == config.get().alarmaHora &&
        t.minutos == config.get().alarmaMinuto)
        //diaSemana // de momento no se controla

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
    DEBUG_PRINTLN("¡ALARMA ACTIVADA!");
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
    if (digitalRead(BOTON_OK) == HIGH)
    {
        DEBUG_PRINTLN("Alarma desactivada por botón OK");
        return true;
    }

    // Verificar comando Bluetooth
    if (comandoBluetoothRecibido == "STOP_ALARM")
    {
        comandoBluetoothRecibido = "";
        DEBUG_PRINTLN("Alarma desactivada por Bluetooth");
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
        DEBUG_PRINTLN("Apagado programado activado");
        apagarSistema();
    }
}

void verificarEncendidoProgramado()
{
    if (sistemaEncendido)
        return;

    FechaHora fh;

    uint8_t horas, minutos, segundos, dia, mes, diaSemana;
    uint16_t año;

    RTC_GetFechaHora(fh);

    dia = fh.dia;
    mes = fh.mes;
    año = fh.año;
    horas = fh.horas;
    minutos = fh.minutos;
    segundos = fh.segundos;
    diaSemana = fh.diaSemana;

    if (config.get().alarmaActivada &&
        horas == config.get().alarmaHora &&
        minutos == config.get().alarmaMinuto)
    {
        DEBUG_PRINTLN("Encendido programado activado");
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
    DEBUG_PRINTLN("Entrando en modo standby...");

    config.save();
    myDFPlayer.stop();
    brilloPantalla(0);
    rgb.clear();
    rgb.show();

    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2_LOW);
    HAL_PWR_EnterSTANDBYMode();
}