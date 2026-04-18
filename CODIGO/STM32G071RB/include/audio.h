#pragma once

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
// #include <SoftwareSerial.h>

extern HardwareSerial mp3Serie;
extern DFRobotDFPlayerMini myDFPlayer;

// Funciones de audio
void iniciarMP3();
void procesarMP3();
void reproducirPista(uint16_t pista, uint8_t carpeta = 2);
void subirVolumenMP3();
void bajarVolumenMP3();
void detenerMP3();
void volumenMP3(uint8_t nivel);
bool reproduciendoMP3();
uint8_t getCarpetaActual();