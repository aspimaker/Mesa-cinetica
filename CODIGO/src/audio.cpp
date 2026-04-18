#include "audio.h"
#include "globals.h"
#include "configuracion.h"
#include "pines.h"

// Definición de objetos globales de audio
//HardwareSerial mp3Serie(DFPLAYER_RX, DFPLAYER_TX);
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


/*
    // --- Paso 1: Reset del DFPlayer y espera de inicialización ---
    // Es crucial darle tiempo al DFPlayer para que se encienda y estabilice.
    Serial.println("Enviando RESET...");
    byte resetCmd[] = {0x7E, 0xFF, 0x06, 0x0C, 0x00, 0x00, 0x00, 0xFE, 0xE4, 0xEF};
    mp3Serie.write(resetCmd, sizeof(resetCmd));
    
    // Espera 2 segundos completos. Esto es más que suficiente para que el módulo se reinicie.
    delay(2000);
    Serial.println("Reset enviado, espera completada.");
  
    // --- Paso 2: Envío del comando principal (Carpeta 2, Pista 1) ---
    // Se envía el comando que ya sabemos que funciona.
    byte playCmd[] = {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x01, 0xFE, 0xE9, 0xEF};
    
    // Enviamos el comando byte por byte.
    Serial.println("Enviando comando PLAY...");
    for (int i = 0; i < sizeof(playCmd); i++) {
      mp3Serie.write(playCmd[i]);
      // Este pequeño delay de 2ms entre bytes puede ayudar en casos de temporización muy estricta.
      delay(2); 
    }
    Serial.println("Comando PLAY enviado.");

    delay(999999);
   */

    // ACK desactivado (false) porque puede causar problemas con algunos módulos
     if (!myDFPlayer.begin(mp3Serie, true, true))
    //if (!myDFPlayer.begin(mp3Serie))
    {
        Serial.println("No se pudo iniciar el DFPlayer Mini");
    }
    else
    {
        Serial.println("DFPlayer Mini iniciado correctamente");
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
            Serial.printf("Carpeta: %d  Pista: %d\n", carpetaActual, pistaActual);
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