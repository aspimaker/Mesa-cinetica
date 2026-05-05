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
//extern DFRobotDFPlayerMini myDFPlayer;

void verificarAlarma()
{
    static bool alarmaDisparada = false;

    if (!sistemaEncendido)
        return;

    FechaHora t;
    RTC_GetFechaHora(t);

    if (config.get().alarmaActivada &&
        t.horas == config.get().alarmaHora &&
        t.minutos == config.get().alarmaMinuto)
    // diaSemana // de momento no se controla

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
    dPln("¡ALARMA ACTIVADA!");
    alarmaSonando = true;

    uint8_t volumenOriginal = config.get().volumen;

    // mostrar mensaje de alarma en la barra de estado
    menu.dibujarBarraEstado("!!! ALARMA !!!");

    // Subir volumen gradualmente
    for (int v = 0; v <= config.get().volumen; v++)
    {
        myDFPlayer.setVolume(v);
        delay(50);
    }

    // reproducir sonido de alarma
    reproducirAviso(3); // aviso 003.mp3 en la carpeta /01

    // led rojo pulsante y verificar desactivación
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

    // restaurar volumen
    myDFPlayer.setVolume(volumenOriginal);
    myDFPlayer.stop();
    alarmaSonando = false;

    // restaurar la interfaz
    redibujarInterfaz();
}

bool desactivarAlarmaPorUsuario()
{
    // comprobar botón físico BTN_OK (TTP223 activo HIGH)
    if (digitalRead(BOTON_OK) == HIGH)
    {
        dPln("Alarma desactivada por botón OK");
        return true;
    }

    // comprobar comando Bluetooth
    if (comandoBluetoothRecibido == "STOP_ALARM")
    {
        comandoBluetoothRecibido = "";
        dPln("Alarma desactivada por Bluetooth");
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
        dPln("Apagado programado activado");
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
        dPln("Encendido programado activado");
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
    dPln("Entrando en modo standby...");

    config.save();
    myDFPlayer.stop();
    brilloPantalla(0);
    rgb.clear();
    rgb.show();

    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2_LOW);
    HAL_PWR_EnterSTANDBYMode();
}