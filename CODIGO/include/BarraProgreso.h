#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "colores.h"

// Nota: el .cpp que use BarraProgreso debe incluir pantalla.h y globals.h
// en el orden adecuado.

class BarraProgreso
{
private:
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

    int _ultimosDigitos[4];
    int _ultimasPosX[4];

    int _calcularAnchoProgreso(int porcentaje);
    void _dibujarPorcentaje(int porcentaje);
    void _dibujarTextoPrincipal();

public:
    BarraProgreso(Adafruit_ST7735 &pantalla,
                  int x,
                  int y,
                  int ancho,
                  int alto,
                  uint16_t colorRelleno,
                  uint16_t colorFondo = ST7735_GRIS,
                  uint16_t colorBorde = ST7735_WHITE,
                  String texto = "",
                  bool mostrarPorcentaje = true);

    void begin();

    void setProgreso(int nuevoProgreso);
    int  getProgreso();
    void redibujarCompleto();
    void actualizar();
    void setProgresoYActualizar(int nuevoProgreso);
    void ocultar();
    void animarHasta(int progresoFinal, int pasoDelayMs = 20);

    void setColorRelleno(uint16_t nuevoColor);
    void setColorFondo(uint16_t nuevoColor);
    void setColorBorde(uint16_t nuevoColor);
    void setTexto(String nuevoTexto);
    void setMostrarPorcentaje(bool mostrar);
    void setPosicion(int nuevaX, int nuevaY);
    void setTamanio(int nuevoAncho, int nuevoAlto);

    bool isVisible();
    bool isReady() { return _inicializada; }
};