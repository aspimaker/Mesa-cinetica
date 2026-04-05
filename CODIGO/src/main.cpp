#include <Arduino.h>

// ── Standalone (sin dependencias propias) ─────────────────────
#include "pines.h"
#include "colores.h"
#include "iconos.h"
#include "logo.h"

// ── Base del sistema ──────────────────────────────────────────
#include "globals.h"

// ── Módulos sin dependencias cruzadas ────────────────────────
#include "audio.h"
#include "leds.h"
#include "utils.h"

// ── BarraProgreso antes que pantalla ─────────────────────────
#include "barraProgreso.h"

// ── Módulos de alto nivel ─────────────────────────────────────
#include "configuracion.h"
#include "pantalla.h"
#include "alarma.h"
#include "bluetooth.h"

// ── Último siempre ────────────────────────────────────────────
#include "desactivaLog.h"

RTC_HandleTypeDef rtc;
Configuracion config;

// ============================================================
// VARIABLES GLOBALES (declaradas como extern)
// ============================================================
bool sistemaEncendido = true;
bool alarmaSonando = false;
unsigned long tiempoInicio = 0;
unsigned long tiempoUltimoLED = 0;
unsigned long tiempoUltimoCheckApagado = 0;
bool lastConnected = false;

void setup()
{
    // inicializa el puerto USB-serie
    Serial.begin(115200);

    // Cargar configuración guardada (también inicializa el RTC)
    config.begin();

    Serial.print("Config cargada: ");
    Serial.println(config.isCargada() ? "SI" : "NO");
    Serial.print("Brillo: ");
    Serial.println(config.get().brillo);

    // Construir nombre Bluetooth único a partir del UID del chip
    uint32_t hash = getDeviceIDHash();
    const char *nombreBase = "aspiKntc-";
    snprintf(nombreBT, sizeof(nombreBT), "%s%lu", nombreBase, hash);

    Serial.println();
    Serial.println();
    Serial.println(nombreBT);

    // Inicializar pantalla TFT, barras de progreso y splash
    iniciarPantalla();
    mostrarSplash();

    // Inicializar reproductor MP3
    iniciarMP3();

    // Mostrar barra de estado inicial
    barraEstado();

    // Inicializar LEDs WS2812B
    iniciarLEDs();

    // Inicializar módulo Bluetooth y autoconfigurar
    bt.begin(BTSerial, 9600);
    bt.autoConfig(nombreBT, "123456");

    // Aplicar ajustes guardados al hardware
    config.aplicarConfiguracion();

    tiempoInicio = millis();

    // QR("https://github.com/aspimaker/Mesa-cinetica");

    //reproducirPista(1, 2);
    Serial.println("Setup finalizado");
}

void loop()
{
    unsigned long ahora = millis();

    // 1. VERIFICAR ALARMA (cada segundo)
    static unsigned long ultimaVerificacionAlarma = 0;
    if (ahora - ultimaVerificacionAlarma >= 1000)
    {
        ultimaVerificacionAlarma = ahora;
        verificarAlarma();
    }

    // 2. VERIFICAR APAGADO PROGRAMADO (cada minuto)
    static unsigned long ultimaVerificacionApagado = 0;
    if (ahora - ultimaVerificacionApagado >= 60000)
    {
        ultimaVerificacionApagado = ahora;
        verificarApagadoProgramado();
    }

    // 3. VERIFICAR ENCENDIDO PROGRAMADO (cada minuto)
    static unsigned long ultimaVerificacionEncendido = 0;
    if (ahora - ultimaVerificacionEncendido >= 60000)
    {
        ultimaVerificacionEncendido = ahora;
        verificarEncendidoProgramado();
    }

    // 4. ACTUALIZAR LEDs según modo
    actualizarLEDs();

    // 5. PROCESAR COMANDOS BLUETOOTH
    procesarComandosBluetooth();

    // 6. PROCESAR REPRODUCCIÓN MP3
    procesarMP3();

    // 7. ACTUALIZAR INTERFAZ (si es necesario)
    static unsigned long ultimaActualizacionUI = 0;
    if (ahora - ultimaActualizacionUI >= 100)
    {
        ultimaActualizacionUI = ahora;
        actualizarInterfaz();
    }

    // 8. PEQUEÑO DELAY para no saturar el CPU
    delay(10);
}
