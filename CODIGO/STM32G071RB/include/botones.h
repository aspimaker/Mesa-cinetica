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

// estructura para botones TTP223
struct BotonTTP223
{
    int pin;
    bool estadoActual;
    bool estadoAnterior;
    unsigned long ultimoDebounce;
};

BotonADC leerBotonADC();