#pragma once

// ============================================================
// controles.h — Funciones de control para cada estado de la
// máquina de estados principal (loop).
//
// Cada función corresponde a una acción de botón dentro de
// un estado concreto. Se implementan en controles.cpp.
// ============================================================

// ── Volumen ───────────────────────────────────────────────
void aumentarVolumen();
void disminuirVolumen();

// ── Brillo LEDs WS2812B ───────────────────────────────────
void aumentarBrilloRGB();
void disminuirBrilloRGB();

// ── Brillo pantalla TFT ───────────────────────────────────
void aumentarBrilloPantalla();
void disminuirBrilloPantalla();

// ── Ecualizador ───────────────────────────────────────────
void siguienteEcualizador();
void anteriorEcualizador();