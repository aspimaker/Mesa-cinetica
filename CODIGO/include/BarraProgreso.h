// BarraProgreso.h
// Cabecera de la clase BarraProgreso para ST7735

#pragma once

#ifndef BarraProgreso_h
#define BarraProgreso_h

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "colores.h"

class BarraProgreso
{
private:
    // Propiedades
    Adafruit_ST7735 *_pantalla;
    int _x, _y;
    int _ancho, _alto;
    uint16_t _colorRelleno;
    uint16_t _colorFondo;
    uint16_t _colorBorde;
    uint16_t _colorTexto;
    String _texto;
    bool _mostrarPorcentaje;
    int _progresoActual;
    int _progresoObjetivo;
    bool _visible;
    bool _inicializada;
    int _ultimoPorcentaje;
    int _ultimoTextX;
    int _ultimoTextY;

    char _ultimosTexto[5];

    // Para el control de dígitos
    int _ultimosDigitos[4];  // Máximo 3 dígitos + %
    int _ultimasPosX[4];     // Posiciones X de cada dígito

    // Métodos privados
    int _calcularAnchoProgreso(int porcentaje);
    void _dibujarPorcentaje(int porcentaje);
    void _dibujarTextoPrincipal();

public:
    // Constructor
    BarraProgreso(Adafruit_ST7735 &pantalla,
                  int x,
                  int y,
                  int ancho,
                  int alto,
                  uint16_t colorRelleno,
                  uint16_t colorFondo = ST7735_GREY,
                  uint16_t colorBorde = ST7735_WHITE,
                  String texto = "",
                  bool mostrarPorcentaje = true);

    // Método para inicializar la barra (llamar DESPUÉS de initR)
    void begin();

    // Métodos principales
    void setProgreso(int nuevoProgreso);
    int getProgreso();
    void redibujarCompleto();
    void actualizar();
    void setProgresoYActualizar(int nuevoProgreso);
    void ocultar();
    void animarHasta(int progresoFinal, int pasoDelayMs = 20);

    // Setters dinámicos
    void setColorRelleno(uint16_t nuevoColor);
    void setColorFondo(uint16_t nuevoColor);
    void setColorBorde(uint16_t nuevoColor);
    void setTexto(String nuevoTexto);
    void setMostrarPorcentaje(bool mostrar);
    void setPosicion(int nuevaX, int nuevaY);
    void setTamanio(int nuevoAncho, int nuevoAlto);

    // Getters
    bool isVisible();
    bool isReady() { return _inicializada; } 
};

#endif