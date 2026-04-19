#include "audio.h"
#include "globals.h"
#include "configuracion.h"
#include "pines.h"

// Definición de objetos globales de audio
// HardwareSerial mp3Serie(DFPLAYER_RX, DFPLAYER_TX);
HardwareSerial mp3Serie(14, 15);
DFRobotDFPlayerMini myDFPlayer;

extern Configuracion config;

// Carpeta 01 reservada para avisos del sistema; usuario: 02-99.
static uint8_t carpetaActual = 2;
static uint8_t pasoVolumen = 1; // paso de aumento/disminución de volumen

uint8_t getCarpetaActual()
{
    return carpetaActual;
}

void iniciarMP3()
{
    pinMode(DFPLAYER_BUSY, INPUT); // para saber el estado del reproductor.

    mp3Serie.begin(9600);

    if (!myDFPlayer.begin(mp3Serie, true, true))
    {
        DEBUG_PRINTLN("No se pudo iniciar el DFPlayer Mini");
    }
    else
    {
        DEBUG_PRINTLN("DFPlayer Mini iniciado correctamente");
    }

    // Restaurar carpeta de la última sesión, garantizando que no sea la 01
    uint8_t carpetaGuardada = config.get().ultimaCarpeta;
    carpetaActual = carpetaGuardada < 2 ? 2 : carpetaGuardada;

    myDFPlayer.volume(config.get().volumen);
}

void procesarMP3()
{
    static uint16_t ultimaPista = 0;

    if (myDFPlayer.available())
    {
        uint16_t pistaActual = myDFPlayer.readCurrentFileNumber();

        if (pistaActual != 0 && pistaActual != ultimaPista)
        {
            ultimaPista = pistaActual;
            config.setUltimaPista(pistaActual, carpetaActual);
            DEBUG_PRINTF("Carpeta: %d  Pista: %d\n", carpetaActual, pistaActual);
        }
    }
}

void reproducirPista(uint16_t pista, uint8_t carpeta)
{
    // Protección: nunca reproducir desde la carpeta de avisos del sistema
    if (carpeta < 2)
        carpeta = 2;
    carpetaActual = carpeta;
    myDFPlayer.playFolder(carpetaActual, pista);
}

void cambiarCarpeta(uint8_t carpeta)
{
    if (carpeta < 2)
        carpeta = 2;
    carpetaActual = carpeta;
    myDFPlayer.playFolder(carpetaActual, 1);
}

void subirVolumenMP3()
{
    int vol = myDFPlayer.readVolume();
    if (vol < 30)
    {
        myDFPlayer.volume(vol + pasoVolumen);
        config.setVolumen(vol + pasoVolumen);
    }
}

void bajarVolumenMP3()
{
    int vol = myDFPlayer.readVolume();
    if (vol > 0)
    {
        myDFPlayer.volume(vol - pasoVolumen);
        config.setVolumen(vol - pasoVolumen);
    }
}

void detenerMP3()
{
    myDFPlayer.stop();
}

void volumenMP3(uint8_t nivel)
{
    if (nivel > 30)
        nivel = 30;
    myDFPlayer.volume(nivel);
    config.setVolumen(nivel);
}

bool reproduciendoMP3()
{
    return digitalRead(DFPLAYER_BUSY) == LOW;
}