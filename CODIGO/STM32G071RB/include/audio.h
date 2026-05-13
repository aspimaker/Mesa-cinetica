#pragma once

#include <Arduino.h>
//#include <DFRobotDFPlayerMini.h>
#include <DFMiniMp3.h>
#include <depuracion.h>

// Notificaciones (obligatorio definir la clase)
class Mp3Notify {
public:
static void OnError(DFMiniMp3<HardwareSerial, Mp3Notify>& mp3, uint16_t errorCode);
static void OnPlayFinished(DFMiniMp3<HardwareSerial, Mp3Notify>& mp3, DfMp3_PlaySources source, uint16_t track);
static void OnPlaySourceOnline(DFMiniMp3<HardwareSerial, Mp3Notify>& mp3, DfMp3_PlaySources source);
static void OnPlaySourceInserted(DFMiniMp3<HardwareSerial, Mp3Notify>& mp3, DfMp3_PlaySources source);
static void OnPlaySourceRemoved(DFMiniMp3<HardwareSerial, Mp3Notify>& mp3, DfMp3_PlaySources source);
};

extern HardwareSerial mp3Serie;
//extern DFRobotDFPlayerMini myDFPlayer;
extern DFMiniMp3<HardwareSerial, Mp3Notify> myDFPlayer;

// Funciones de audio
void iniciarMP3();
void procesarMP3();
void reproducirAleatorio();
void reproducirPista(uint16_t pista);
void reproducirAviso(uint16_t aviso);
void subirVolumenMP3();
void bajarVolumenMP3();
void detenerMP3();
void volumenMP3(uint8_t nivel);
bool reproduciendoMP3();
void ecualizacionMP3(uint8_t modo);
void modoReproduccionMP3(uint8_t modo);
//uint8_t getCarpetaActual();