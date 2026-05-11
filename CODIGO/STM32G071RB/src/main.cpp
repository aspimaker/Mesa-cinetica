/*
==================================================
Mesa Cinética - Kinetic Table
Copyright © 2026 aspimaker

https://github.com/aspimaker/Mesa-cinetica/

Este software está bajo licencia Creative Commons
Attribution-NonCommercial-ShareAlike 4.0 International
CC BY-NC-SA 4.0
 
Usted es libre de:
  • Compartir: copiar y redistribuir el material
  • Adaptar: remezclar, transformar y construir sobre el material

exclusivamente bajo los siguientes términos:
  • Atribución: debe dar crédito adecuado
  • No Comercial: no puede usar el material con fines comerciales
  • Compartir Igual: si remezcla o transforma, debe distribuir bajo la misma licencia
==================================================
*/

/* mapa de memoria
0x000000 - 0x000FFF   →  Cabecera (datetime sync, nº canciones, nº patrones...)
0x001000 - 0x00FFFF   →  Índice canciones (500 entradas x ~32 bytes = 16 KB)
0x010000 - 0x0FFFFF   →  Metadatos texto (500 x ~300 bytes = 150 KB)
0x100000 - 0x7FFFFF   →  Miniaturas (500 x ~15 KB = 7.5 MB)
0x800000 - 0xFFFFFF   →  Patrones (8 MB)
*/
bool debugEnabled = true;

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
#define BTN_ABAJO_PRUEBA PA15 // PB6

#include "pines.h"
#include "colores.h"
#include "iconos.h"
#include "logo.h"

#include "globals.h"
#include "depuracion.h"

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

// Depuración → USB-TTL en PB6/PB7 (pines morpho)
// HardwareSerial SerialDebug(UART_RX_DEBUG, UART_TX_DEBUG);

// TMC2209 UART → D2/D3
HardwareSerial SerialTMC(UART_RX_TMC2209, UART_TX_TMC2209);

// mp3 DFPlayer Mini → D0/D1
HardwareSerial mp3Serie(DFPLAYER_RX, DFPLAYER_TX);

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
    sprintf(buffer, "Pista: %d", myDFPlayer.getCurrentTrack());
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
    SerialTMC.begin(115200);
    mp3Serie.begin(115200);
    bt.begin(BTSerial, 9600);

    delay(200);
    dPln("");
    dPln("");
    dPln("Iniciando Sistema...");

    uint16_t tiempoSplash = 500;
    uint32_t ahora = millis();

    // reloj para RTC
    activar_reloj_LSE();

    // datos hardware
    mesaCinetica.version = MESA_VERSION;
    mesaCinetica.revision = MESA_REVISION;
    mesaCinetica.memoria = MESA_MEMORIA;
    mesaCinetica.mp3 = MESA_MP3;
    mesaCinetica.rgb = MESA_RGB;

    // cargar configuración
    config.begin();

    dP("Config cargada: ");
    dPln(config.isCargada() ? "SI" : "NO");

    iniciarPantalla();

    dInitPantalla();
    dPln("Sistema iniciado");
    dOkPantalla("Configuracion OK");
    dInfoPantalla("Esperando comandos...");
    dPf("Valor: %d", 42);
    
    mostrarSplash(); // mientras se muestra, hacemos otras cosas...

    iniciarMP3();

    // iniciarBotonesTTP223();

    iniciarLEDs();

    // módulo bluetooth y autoconfigurar
    uint32_t hash = getDeviceIDHash(); // construir nombre Bluetooth único a partir del UID del chip
    const char *nombreBase = "aspiKntc-";
    snprintf(nombreBT, sizeof(nombreBT), "%s%lu", nombreBase, hash);
    // snprintf(nombreBT, sizeof(nombreBT), "Mesa-%04X", (uint16_t)(id & 0xFFFF));

    bt.autoConfig(nombreBT, "123456");
    dPln(nombreBT);

    dP("Brillo: ");
    dPln(config.get().brillo);

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

    dPln("Setup finalizado");
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

    botonADC = leerBotonADC();
    botonADC = BOTON_NINGUNO;

    // dP("Botón ADC: ");
    // dPln(botonADC);


        // ==============================================
        // CONTROL SEGÚN ESTADO ACTUAL
        // ==============================================
        static unsigned long ultimoMovimiento = 0;

        if (ahora - ultimoMovimiento >= DEBOUNCE_DELAY)
        {
            switch (estadoActual)
            {

            case ESTADO_MENU_PRINCIPAL:

                switch (botonADC)
                {
                case (BotonADC)BOTON_ARRIBA:
                    dPln("arriba");
                    menu.moverSeleccion(0); // arriba
                    ultimoMovimiento = ahora;
                    break;

                case (BotonADC)BOTON_ABAJO:
                    dPln("abajo");
                    menu.moverSeleccion(1); // abajo
                    ultimoMovimiento = ahora;
                    break;

                case (BotonADC)BOTON_IZQUIERDA:
                    dPln("izquierda");
                    menu.moverSeleccion(2); // izquierda
                    ultimoMovimiento = ahora;
                    break;

                case (BotonADC)BOTON_DERECHA:
                    dPln("derecha");
                    menu.moverSeleccion(3); // derecha
                    ultimoMovimiento = ahora;
                    break;

                case (BotonADC)BOTON_OK:
                    dPln("OK");
                    menu.ejecutarAccion();
                    ultimoMovimiento = ahora;
                    break;

                default:
                    break;
                }
                break;

            case ESTADO_AJUSTE_VOLUMEN:

                switch (botonADC)
                {
                case (BotonADC)BOTON_IZQUIERDA:
                    dPln("izquierda");
                    aumentarVolumen();
                    ultimoMovimiento = ahora;
                    break;

                case (BotonADC)BOTON_DERECHA:
                    dPln("derecha");
                    disminuirVolumen();
                    ultimoMovimiento = ahora;
                    break;

                case (BotonADC)BOTON_OK:
                    dPln("OK");
                    estadoActual = ESTADO_MENU_PRINCIPAL;
                    menu.dibujarIconos();
                    menu.dibujarSeleccion(menu.getIconoActivo());
                    ultimoMovimiento = ahora;
                    break;

                default:
                    break;
                }
                break;

            case ESTADO_AJUSTE_BRILLO_RGB:

                switch (botonADC)
                {
                case (BotonADC)BOTON_IZQUIERDA:
                    dPln("izquierda");
                    aumentarBrilloRGB();
                    ultimoMovimiento = ahora;
                    break;

                case (BotonADC)BOTON_DERECHA:
                    dPln("derecha");
                    disminuirBrilloRGB();
                    ultimoMovimiento = ahora;
                    break;

                case (BotonADC)BOTON_OK:
                    dPln("OK");
                    estadoActual = ESTADO_MENU_PRINCIPAL;
                    menu.dibujarIconos();
                    menu.dibujarSeleccion(menu.getIconoActivo());
                    ultimoMovimiento = ahora;
                    break;

                default:
                    break;
                }
                break;

            case ESTADO_AJUSTE_ECUALIZADOR:

                switch (botonADC)
                {
                case (BotonADC)BOTON_IZQUIERDA:
                    dPln("izquierda");
                    // ecualizadorAnterior();
                    ultimoMovimiento = ahora;
                    break;

                case (BotonADC)BOTON_DERECHA:
                    dPln("derecha");
                    // ecualizadorSiguiente();
                    ultimoMovimiento = ahora;
                    break;

                case (BotonADC)BOTON_OK:
                    dPln("OK");
                    estadoActual = ESTADO_MENU_PRINCIPAL;
                    menu.dibujarIconos();
                    menu.dibujarSeleccion(menu.getIconoActivo());
                    ultimoMovimiento = ahora;
                    break;
                }
                break;
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
    //delay(10);
}