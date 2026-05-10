#include "audio.h"
#include "globals.h"
#include "configuracion.h"
#include "pines.h"

extern Configuracion config;
extern HardwareSerial mp3Serie;

DFMiniMp3<HardwareSerial, Mp3Notify> myDFPlayer(mp3Serie);

/************************************************
 * CONTROL DEL REPRODUCTOR MP3 (DFPlayer Mini)
 * Carpeta 01 reservada para avisos del sistema, con el formato 001.mp3 002.mp3 etc.
 * Los mp3 del usuario están en /mp3 con el formato 0001.mp3 0002.mp3 etc.
 ************************************************/

void iniciarMP3()
{
    pinMode(DFPLAYER_BUSY, INPUT); // para saber el estado del reproductor.

    myDFPlayer.begin();
    myDFPlayer.setVolume(0); // volumen mínimo al iniciar para evitar ruidos indeseados
    myDFPlayer.enableDac();  // habilitar salida DAC para mejor calidad de audio
    myDFPlayer.stop();       // asegurarse de que no esté reproduciendo nada al iniciar
    // myDFPlayer.reset(); //produce un ruido molesto al iniciar, mejor evitarlo
    dPln("Reproductor MP3 iniciado");
    // delay(100); // dar tiempo a que el módulo se estabilice
}

void procesarMP3()
{
    myDFPlayer.loop(); // procesa notificaciones pendientes
}

void reproducirAleatorio()
{
    dPln("Reproducción aleatoria");
    myDFPlayer.playRandomTrackFromAll();
}

void reproducirPista(uint16_t pista)
{
   
    uint16_t total = myDFPlayer.getTotalTrackCount();
    dP("Total pistas disponibles 0: ");
    dPln(total);    

    /*
    uint16_t totalUno = myDFPlayer.getFolderTrackCount(1);
    dP("Total pistas disponibles 1: ");
    dPln(totalUno);    
*/

    dPln("Reproduciendo pista: " + String(pista));
    myDFPlayer.playMp3FolderTrack(pista);

    config.set().ultimaPista = pista;
}

void reproducirAviso(uint16_t aviso)
{
    dPln("Reproduciendo aviso: " + String(aviso));

    // utiliza la carpeta /advert/ 0001.mp3, 0002.mp3
    myDFPlayer.playAdvertisement(aviso); // mutea el resto de pistas y reproduce el aviso, luego vuelve a la pista anterior

    // myDFPlayer.playFolderTrack(1, aviso); // utiliza la carpeta /01 para avisos del sistema
}

void subirVolumenMP3()
{
    dPln("Volumen +");
    uint8_t vol = myDFPlayer.getVolume();
    uint8_t nuevo = vol + pasoVolumen;

    if (nuevo <= 30)
    {
        dPln("Nuevo volumen: " + String(nuevo));
        myDFPlayer.setVolume(nuevo);
        return;
    }

    dPln("Volumen máximo alcanzado");
}

void bajarVolumenMP3()
{
    dPln("Volumen -");
    int8_t vol = myDFPlayer.getVolume();
    int8_t nuevo = vol - pasoVolumen;

    if (nuevo >= 0)
    {
        dPln("Nuevo volumen: " + String(nuevo));
        myDFPlayer.setVolume(nuevo);
       return;
    }
    dPln("Volumen mínimo alcanzado");
}

void detenerMP3()
{
    dPln("Stop MP3");
    myDFPlayer.stop();
}

void volumenMP3(uint8_t nivel)
{
    if (nivel > 30)
        nivel = 30;
    myDFPlayer.setVolume(nivel);
    config.setVolumen(nivel);
}

bool reproduciendoMP3()
{
    return digitalRead(DFPLAYER_BUSY) == LOW;
}

/*
NOTIFICACIONES del DFPlayer Mini: depende del modelo.
YX5200 - Soporte completo de eventos
GD3200D - Soporte completo de eventos
MH2024K-16SS - Soporte limitado de eventos
*/
void Mp3Notify::OnError(DFMiniMp3<HardwareSerial, Mp3Notify> &mp3, uint16_t errorCode)
{
    dPf("Error DFPlayer: %d\n", errorCode);
}

void Mp3Notify::OnPlayFinished(DFMiniMp3<HardwareSerial, Mp3Notify> &mp3, DfMp3_PlaySources source, uint16_t track)
{
    dPf("Pista terminada: %d\n", track);
    config.setUltimaPista(track);
}

void Mp3Notify::OnPlaySourceOnline(DFMiniMp3<HardwareSerial, Mp3Notify> &mp3, DfMp3_PlaySources source)
{
    dPln("DFPlayer online");
}

void Mp3Notify::OnPlaySourceInserted(DFMiniMp3<HardwareSerial, Mp3Notify> &mp3, DfMp3_PlaySources source)
{
    dPln("SD insertada");
}

void Mp3Notify::OnPlaySourceRemoved(DFMiniMp3<HardwareSerial, Mp3Notify> &mp3, DfMp3_PlaySources source)
{
    dPln("SD retirada");
}
