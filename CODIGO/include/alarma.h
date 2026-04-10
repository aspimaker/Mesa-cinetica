#pragma once

//#include <Arduino.h>
#include "globals.h"
//#include "configuracion.h"
//#include "audio.h"
//#include "leds.h"
//#include "pantalla.h"
//#include "pines.h"

// Funciones de alarma
void verificarAlarma();
void activarAlarma();
bool desactivarAlarmaPorUsuario();
void verificarApagadoProgramado();
void verificarEncendidoProgramado();
void encenderSistema();
void apagarSistema();
void entrarEnStandby();
void redibujarInterfaz();