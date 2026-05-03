/* mapa de memoria
0x000000 - 0x000FFF   →  Cabecera (datetime sync, nº canciones, nº patrones...)
0x001000 - 0x00FFFF   →  Índice canciones (500 entradas x ~32 bytes = 16 KB)
0x010000 - 0x0FFFFF   →  Metadatos texto (500 x ~300 bytes = 150 KB)
0x100000 - 0x7FFFFF   →  Miniaturas (500 x ~15 KB = 7.5 MB)
0x800000 - 0xFFFFFF   →  Patrones (8 MB)
*/

#include <Arduino.h>

#define MESA_VERSION 0;
#define MESA_REVISION 1;
#define MESA_MEMORIA 16;
#define MESA_MP3 1;
#define MESA_RGB 1;

#define BTN_OK_PRUEBA PC0
#define BTN_DERECHA_PRUEBA PC1
#define BTN_IZQUIERDA_PRUEBA PC2
#define BTN_ARRIBA_PRUEBA PC3
#define BTN_ABAJO_PRUEBA PB6

#include "pines.h"
#include "colores.h"
#include "iconos.h"
#include "logo.h"

#include "globals.h"

#include "audio.h"
#include "leds.h"
#include "utils.h"

#include "barraProgreso.h"
#include "configuracion.h"
#include "pantalla.h"
#include "menu.h"
#include "alarma.h"
#include "bluetooth.h"
#include "controles.h"
#include "botones.h"

extern Menu menu;

RTC_HandleTypeDef rtc;
Configuracion config;

// ============================================================
// VARIABLES GLOBALES (declaradas como extern)
// ============================================================
InformacionHardware mesaCinetica;
bool sistemaEncendido = true;
bool alarmaSonando = false;
unsigned long tiempoInicio = 0;
EstadoSistema estadoActual = ESTADO_MENU_PRINCIPAL;
BotonADC botonADC = (BotonADC)BOTON_NINGUNO;
unsigned long ultimoTiempoBoton = 0;

void activar_reloj_LSE()
{
    // reloj externo LSE para el RTC (requiere cristal de 32.768 kHz)
    // relojes de interfaz
    RCC->APBENR1 |= RCC_APBENR1_PWREN;
    RCC->APBENR1 |= RCC_APBENR1_RTCAPBEN;

    // acceso al dominio de backup
    HAL_PWR_EnableBkUpAccess();

    // iniciar el reloj
    RTC_InitRTC_LSE();
}

void activar_reloj_LSI()
{
    // NO SE UTILIZA POR EL ERROR DIARIO DE 1-2 MINUTOS
    //  reloj interno LSI para el RTC (no necesita cristal externo)

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

void mostrarPistaActual()
{
    char buffer[32];
    sprintf(buffer, "Pista: %d", myDFPlayer.readCurrentFileNumber());
    menu.dibujarBarraEstado(buffer);
    delay(500);

    if (estadoActual == ESTADO_MENU_PRINCIPAL)
    {
        menu.dibujarBarraEstado("MENU PRINCIPAL");
    }
}

void setup()
{
    Serial.begin(115200);
    delay(500);
    DEBUG_PRINTLN("Iniciando Sistema...");

    uint16_t tiempoSplash = 500;
    uint32_t ahora = millis();

    // configuracion hardware
    mesaCinetica.version = MESA_VERSION;
    mesaCinetica.revision = MESA_REVISION;
    mesaCinetica.memoria = MESA_MEMORIA;
    mesaCinetica.mp3 = MESA_MP3;
    mesaCinetica.rgb = MESA_RGB;

    // cargar configuración
    config.begin();

    // reloj para RTC
    activar_reloj_LSE();

    DEBUG_PRINT("Config cargada: ");
    DEBUG_PRINTLN(config.isCargada() ? "SI" : "NO");

    // inicializar reproductor MP3
    iniciarMP3();

    iniciarPantalla();

    mostrarSplash(); // mientras se muestra, hacemos otras cosas...

    // inicializar pulsadores TTP223
    iniciarBotonesTTP223();

    // inicializar LEDs WS2812B
    iniciarLEDs();

    // módulo bluetooth y autoconfigurar
    bt.begin(BTSerial, 0);             // inicializar el puerto serie
    uint32_t hash = getDeviceIDHash(); // construir nombre Bluetooth único a partir del UID del chip
    const char *nombreBase = "aspiKntc-";
    snprintf(nombreBT, sizeof(nombreBT), "%s%lu", nombreBase, hash);
    // snprintf(nombreBT, sizeof(nombreBT), "Mesa-%04X", (uint16_t)(id & 0xFFFF));
    bt.autoConfig(nombreBT, "123456");

    DEBUG_PRINTLN(nombreBT);

    DEBUG_PRINT("Brillo: ");
    DEBUG_PRINTLN(config.get().brillo);

    // esperar el tiempo de splash
    while (millis() - ahora < tiempoSplash)
    {
        delay(10);
    }
    tft.fillScreen(ST7735_BLACK);

    // mostrar barra de estado inicial
    barraEstado();

    // aplicar ajustes guardados al hardware
    config.aplicarConfiguracion();

    tiempoInicio = millis();

    DEBUG_PRINTLN("Setup finalizado");

    // pruebas
    // reproducirPista(2, 2);
    // subirVolumenMP3();
    // myDFPlayer.stop();

   // qr("hola mundo");
}

void loop()
{
    botonADC = leerBotonADC();

    // sólo para pruebas (pines morpho)
    pinMode(BTN_OK_PRUEBA, INPUT_PULLDOWN);
    pinMode(BTN_DERECHA_PRUEBA, INPUT_PULLDOWN);
    pinMode(BTN_IZQUIERDA_PRUEBA, INPUT_PULLDOWN);
    pinMode(BTN_ARRIBA_PRUEBA, INPUT_PULLDOWN);
    pinMode(BTN_ABAJO_PRUEBA, INPUT_PULLDOWN);
    //------------------------------------

    unsigned long ahora = millis();

    // ACTUALIZAR INTERFAZ (si es necesario)
    static unsigned long ultimaActualizacionUI = 0;
    if (ahora - ultimaActualizacionUI >= 100)
    {
        ultimaActualizacionUI = ahora;
        actualizarInterfaz();
    }

    // ==============================================
    // LECTURA DE BOTONES TTP223 - desactivado para las pruebas con pines morpho
    // ==============================================
    /*
    bool ok_presionado = leerBotonTTP223(botonOK);
    bool derecha_presionado = leerBotonTTP223(botonDerecha);
    bool izquierda_presionado = leerBotonTTP223(botonIzquierda);
    bool arriba_presionado = leerBotonTTP223(botonArriba);
    bool abajo_presionado = leerBotonTTP223(botonAbajo);
    */

    bool ok_presionado = digitalRead(BTN_OK_PRUEBA);
    bool derecha_presionado = digitalRead(BTN_DERECHA_PRUEBA);
    bool izquierda_presionado = digitalRead(BTN_IZQUIERDA_PRUEBA);
    bool arriba_presionado = digitalRead(BTN_ARRIBA_PRUEBA);
    bool abajo_presionado = digitalRead(BTN_ABAJO_PRUEBA);

    // DEBUG_PRINTLN(estadoActual);
    ;
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
                DEBUG_PRINTLN("arriba");
                menu.moverSeleccion(0); // arriba
                ultimoMovimiento = ahora;
            }
            else if (abajo_presionado)
            {
                DEBUG_PRINTLN("abajo");
                menu.moverSeleccion(1); // abajo
                ultimoMovimiento = ahora;
            }
            else if (izquierda_presionado)
            {
                DEBUG_PRINTLN("izquierda");
                menu.moverSeleccion(2); // izquierda
                ultimoMovimiento = ahora;
            }
            else if (derecha_presionado)
            {
                DEBUG_PRINTLN("derecha");
                menu.moverSeleccion(3); // derecha
                ultimoMovimiento = ahora;
            }
            else if (ok_presionado)
            {
                DEBUG_PRINTLN("OK");
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
            // play/pausa (solo si no estamos en el menú)
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
    bt.update();                 // 1. detecta conexión/desconexión
    procesarComandosBluetooth(); // 2. procesa comandos

    // PROCESAR REPRODUCCIÓN MP3
    procesarMP3();

    // PEQUEÑO DELAY para no saturar el CPU
    delay(10);
}
