#pragma once

enum BotonADC
{
    BOTON_NINGUNO,
    BOTON_OK,
    BOTON_DERECHA,
    BOTON_IZQUIERDA,
    BOTON_ARRIBA,
    BOTON_ABAJO
};

// Estructura para botones TTP223
struct BotonTTP223
{
    int pin;
    bool estadoActual;
    bool estadoAnterior;
    unsigned long ultimoDebounce;
};

// botones
extern BotonTTP223 botonOK;
extern BotonTTP223 botonDerecha;
extern BotonTTP223 botonIzquierda;
extern BotonTTP223 botonArriba;
extern BotonTTP223 botonAbajo;

BotonADC leerBotonADC();
void iniciarBotonesTTP223();
bool leerBotonTTP223(BotonTTP223 &boton);