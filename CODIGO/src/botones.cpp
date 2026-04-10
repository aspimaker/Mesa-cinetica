// ============================================================
// botones.cpp — Gestión de botones táctiles TTP223
//
// El TTP223 es activo HIGH: HIGH = tocado, LOW = sin tocar.
// No necesita pull-up interno (pinMode INPUT es suficiente).
// El debounce evita lecturas múltiples por un solo toque.
// ============================================================

#include "globals.h"
#include "pines.h"

void iniciarBotonesTTP223()
{
    pinMode(BTN_OK, INPUT);
    pinMode(BTN_DERECHA, INPUT);
    pinMode(BTN_IZQUIERDA, INPUT);
    pinMode(BTN_ARRIBA, INPUT);
    pinMode(BTN_ABAJO, INPUT);
}

// Devuelve true UNA SOLA VEZ por cada pulsación.
// No bloquea — se llama en cada iteración del loop.
bool leerBotonTTP223(BotonTTP223 &boton)
{
    bool lecturaActual = (digitalRead(boton.pin) == HIGH);
    unsigned long ahora = millis();

    // Si ha cambiado el estado, reiniciar el temporizador de debounce
    if (lecturaActual != boton.estadoAnterior)
        boton.ultimoDebounce = ahora;

    // Solo procesar si el estado lleva estable más de DEBOUNCE_DELAY ms
    if (ahora - boton.ultimoDebounce >= DEBOUNCE_DELAY)
    {
        // Flanco de subida: estaba sin tocar y ahora está tocado
        if (lecturaActual == true && boton.estadoActual == false)
        {
            boton.estadoActual = true;
            boton.estadoAnterior = lecturaActual;
            return true; // ← evento de pulsación
        }

        // Flanco de bajada: soltado
        if (lecturaActual == false)
            boton.estadoActual = false;
    }

    boton.estadoAnterior = lecturaActual;
    return false;
}