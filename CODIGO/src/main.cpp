#include <Arduino.h>

// ── Standalone (sin dependencias propias) ─────────────────────
#include "pines.h"
#include "colores.h"
#include "iconos.h"
#include "logo.h"

// ── Base del sistema ──────────────────────────────────────────
#include "globals.h"

// ── Módulos sin dependencias cruzadas ─────────────────────────
#include "audio.h"
#include "leds.h"
#include "utils.h"

// ── BarraProgreso ─────────────────────────────────────────────
#include "barraProgreso.h"

// ── Módulos de alto nivel ─────────────────────────────────────
#include "configuracion.h"
#include "pantalla.h"
#include "menu.h"
#include "alarma.h"
#include "bluetooth.h"

#include "controles.h"
#include "desactivaLog.h"

extern Menu menu;

BotonTTP223 botonOK = {BTN_OK, false, false, 0};
BotonTTP223 botonDerecha = {BTN_DERECHA, false, false, 0};
BotonTTP223 botonIzquierda = {BTN_IZQUIERDA, false, false, 0};
BotonTTP223 botonArriba = {BTN_ARRIBA, false, false, 0};
BotonTTP223 botonAbajo = {BTN_ABAJO, false, false, 0};

RTC_HandleTypeDef rtc;
Configuracion config;

// ============================================================
// VARIABLES GLOBALES (declaradas como extern)
// ============================================================
bool sistemaEncendido = true;
bool alarmaSonando = false;
unsigned long tiempoInicio = 0;
// String comandoBluetoothRecibido = "";
EstadoSistema estadoActual = ESTADO_MENU_PRINCIPAL;
unsigned long ultimoTiempoBoton = 0;

/*
void activar_reloj_LSI()
{
    // utilizar reloj interno LSI para el RTC (aunque es menos preciso, no requiere cristal externo)

    // Relojes de interfaz
    RCC->APBENR1 |= RCC_APBENR1_PWREN;
    RCC->APBENR1 |= RCC_APBENR1_RTCAPBEN;

    // acceso al dominio de backup
    HAL_PWR_EnableBkUpAccess();

    // usar oscilador LSI
    RCC->CSR |= RCC_CSR_LSION;
    while (!(RCC->CSR & RCC_CSR_LSIRDY))
        ;

    // iniciar reloj LSI
    RTC_InitRTC_LSI();
}
*/

void activar_reloj_LSE()
{
    // Relojes de interfaz
    RCC->APBENR1 |= RCC_APBENR1_PWREN;
    RCC->APBENR1 |= RCC_APBENR1_RTCAPBEN;

    // Acceso al dominio de backup
    HAL_PWR_EnableBkUpAccess();

    // iniciar el reloj
    RTC_InitRTC_LSE();
}

void mostrarPistaActual()
{
    char buffer[32];
    sprintf(buffer, "Pista: %d", myDFPlayer.readCurrentFileNumber());
    menu.dibujarBarraEstado(buffer);
    delay(500);

    // Usar la variable global estadoActual (sin extern)
    if (estadoActual == ESTADO_MENU_PRINCIPAL)
    {
        menu.dibujarBarraEstado("MENU PRINCIPAL");
    }
}

void setup()
{
    uint16_t tiempoSplash = 500;
    uint32_t ahora = millis();

    Serial.begin(115200);
    delay(500);
    Serial.println("Iniciando Sistema...");

    // configuración
    config.begin();

    // reloj para RTC
    activar_reloj_LSE();

    Serial.print("Config cargada: ");
    Serial.println(config.isCargada() ? "SI" : "NO");

    iniciarPantalla();

    mostrarSplash(); // mientras se muestra, hacemos otras cosas...

    // configurarPulsadoresTTP223();
    iniciarBotonesTTP223();

    // Inicializar LEDs WS2812B
    iniciarLEDs();

    // Inicializar reproductor MP3
    iniciarMP3();

    // módulo bluetooth y autoconfigurar
    bt.begin(BTSerial, 0);             // inicializar el puerto serie
    uint32_t hash = getDeviceIDHash(); // Construir nombre Bluetooth único a partir del UID del chip
    const char *nombreBase = "aspiKntc-";
    snprintf(nombreBT, sizeof(nombreBT), "%s%lu", nombreBase, hash);
    // snprintf(nombreBT, sizeof(nombreBT), "Mesa-%04X", (uint16_t)(id & 0xFFFF));
    bt.autoConfig(nombreBT, "123456");

    Serial.println(nombreBT);

    Serial.print("Brillo: ");
    Serial.println(config.get().brillo);
    // brilloPantalla(50);

    // esperar el tiempo de splash
    while (millis() - ahora < tiempoSplash)
    {
        delay(10);
    }
    tft.fillScreen(ST7735_BLACK);

    // Mostrar barra de estado inicial
    barraEstado();

    // Aplicar ajustes guardados al hardware
    config.aplicarConfiguracion();

    tiempoInicio = millis();

    Serial.println("Setup finalizado");
}

void loop()
{
    unsigned long ahora = millis();

    // ACTUALIZAR INTERFAZ (si es necesario)
    static unsigned long ultimaActualizacionUI = 0;
    if (ahora - ultimaActualizacionUI >= 100)
    {
        ultimaActualizacionUI = ahora;
        actualizarInterfaz();
    }

    
    // ==============================================
    // LECTURA DE BOTONES TTP223
    // ==============================================
    bool ok_presionado = leerBotonTTP223(botonOK);
    bool derecha_presionado = leerBotonTTP223(botonDerecha);
    bool izquierda_presionado = leerBotonTTP223(botonIzquierda);
    bool arriba_presionado = leerBotonTTP223(botonArriba);
    bool abajo_presionado = leerBotonTTP223(botonAbajo);

    // ==============================================
    // CONTROL SEGÚN ESTADO ACTUAL
    // ==============================================
    if (estadoActual == ESTADO_MENU_PRINCIPAL)
    {
        // NAVEGACIÓN DEL MENÚ PRINCIPAL
        static unsigned long ultimoMovimiento = 0;

        if (ahora - ultimoMovimiento >= DEBOUNCE_DELAY)
        {
            if (arriba_presionado)
            {
                menu.moverSeleccion(0); // Arriba
                ultimoMovimiento = ahora;
            }
            else if (abajo_presionado)
            {
                menu.moverSeleccion(1); // Abajo
                ultimoMovimiento = ahora;
            }
            else if (izquierda_presionado)
            {
                menu.moverSeleccion(2); // Izquierda
                ultimoMovimiento = ahora;
            }
            else if (derecha_presionado)
            {
                menu.moverSeleccion(3); // Derecha
                ultimoMovimiento = ahora;
            }
            else if (ok_presionado)
            {
                menu.ejecutarAccion();
                ultimoMovimiento = ahora;
            }
        }
    }
    else
    {
        // CONTROL EN OTRAS PANTALLAS (volumen, ecualizador, etc.)
        switch (estadoActual)
        {
        case ESTADO_AJUSTE_VOLUMEN:
            if (arriba_presionado)
            {
                aumentarVolumen();
            }
            else if (abajo_presionado)
            {
                disminuirVolumen();
            }
            else if (ok_presionado || izquierda_presionado)
            {
                estadoActual = ESTADO_MENU_PRINCIPAL;
                menu.dibujarIconos();
                menu.dibujarSeleccion(menu.getIconoActivo());
            }
            break;

        case ESTADO_AJUSTE_BRILLO_RGB:
            if (arriba_presionado)
            {
                aumentarBrilloRGB();
            }
            else if (abajo_presionado)
            {
                disminuirBrilloRGB();
            }
            else if (ok_presionado)
            {
                estadoActual = ESTADO_MENU_PRINCIPAL;
                menu.dibujarIconos();
                menu.dibujarSeleccion(menu.getIconoActivo());
            }
            break;

            // Añadir más casos según necesites
        }

        // CONTROL DEL REPRODUCTOR MP3 (siempre activo en segundo plano)
        if (derecha_presionado)
        {
            myDFPlayer.next();
            mostrarPistaActual();
        }
        else if (izquierda_presionado)
        {
            myDFPlayer.previous();
            mostrarPistaActual();
        }
        else if (ok_presionado && estadoActual != ESTADO_MENU_PRINCIPAL)
        {
            // Play/Pausa (solo si no estamos en el menú)
            if (reproduciendoMP3())
            {
                myDFPlayer.pause();
                menu.dibujarBarraEstado("Pausa");
            }
            else
            {
                myDFPlayer.start();
                menu.dibujarBarraEstado("Reproduciendo");
            }
        }
    }

    // VERIFICAR ALARMA (cada segundo)
    static unsigned long ultimaVerificacionAlarma = 0;
    if (ahora - ultimaVerificacionAlarma >= 1000)
    {
        ultimaVerificacionAlarma = ahora;
        verificarAlarma();
    }

    // VERIFICAR APAGADO y ENCENDIDO PROGRAMADO (cada minuto)
    static unsigned long ultimaVerificacionApagado = 0;
    if (ahora - ultimaVerificacionApagado >= 60000)
    {
        ultimaVerificacionApagado = ahora;

        verificarApagadoProgramado();
        verificarEncendidoProgramado();
    }

    // ACTUALIZAR LEDs según modo
    actualizarLEDs();

    // PROCESAR COMANDOS BLUETOOTH
    procesarComandosBluetooth();

    // PROCESAR REPRODUCCIÓN MP3
    procesarMP3();

    
    // PEQUEÑO DELAY para no saturar el CPU
    delay(10);
}
