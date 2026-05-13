// ============================================================
// botones.cpp — Gestión de botones táctiles TTP223
//
// El TTP223 es activo HIGH: HIGH = tocado, LOW = sin tocar.
// No necesita pull-up interno
// El debounce evita lecturas múltiples por un solo toque.
// ============================================================

#include "botones.h"

#include "globals.h"
#include "pines.h"

BotonADC leerBotonADC()
{
    int v = analogRead(Pinout::UI::BTN_ADC);

    if (v < 100)
        return (BotonADC)BOTON_NINGUNO;
    if (v < 500)
        return (BotonADC)BOTON_OK;
    if (v < 1000)
        return (BotonADC)BOTON_DERECHA;
    if (v < 1600)
        return (BotonADC)BOTON_IZQUIERDA;
    if (v < 2500)
        return (BotonADC)BOTON_ARRIBA;
    return (BotonADC)BOTON_ABAJO;
}

void iniciarBotonesTTP223()
{
    pinMode(Pinout::UI::BTN_ADC, INPUT);
}

/*
// devuelve true UNA SOLA VEZ por cada pulsación.
// no bloquea — se llama en cada iteración del loop.
bool leerBotonTTP223(BotonTTP223 &boton)
{
    bool lecturaActual = (digitalRead(boton.pin) == HIGH);
    unsigned long ahora = millis();

    // si ha cambiado el estado, reiniciar el temporizador de debounce
    if (lecturaActual != boton.estadoAnterior)
        boton.ultimoDebounce = ahora;

    // solo procesar si el estado lleva estable más de DEBOUNCE_DELAY ms
    if (ahora - boton.ultimoDebounce >= DEBOUNCE_DELAY)
    {
        // flanco de subida -> pulsado
        if (lecturaActual == true && boton.estadoActual == false)
        {
            boton.estadoActual = true;
            boton.estadoAnterior = lecturaActual;
            return true;
        }

        // flanco de bajada -> liberado
        if (lecturaActual == false)
            boton.estadoActual = false;
    }

    boton.estadoAnterior = lecturaActual;
    return false;
}
*/
