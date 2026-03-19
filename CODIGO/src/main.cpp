/*
Proyecto: mesa cinética
Autor: aspimaker
Fecha: 21/03/2026

Descripción: Este proyecto es una mesa cinética que utiliza motores paso a paso para crear
             patrones visuales sobre la arena. La bola de acero está controlada por un microcontrolador
             ST NUCLEO G071RB. Se encarga de mover los motores para generar los patrones.

Funcionalidades:
    - Control de motores paso a paso para mover la bola de acero sobre la arena.
    - Movimiento suave y preciso al utilizar un sistema polar para controlar la posición de la bola.
    - Iluminación led RGB
    - Reproductor de música MP3
    - Aplicación móvil para controlar la mesa cinética

Comunicación:
    - bluetooth BLE para recibir comandos de un dispositivo móvil

Hardware:
    - placa de desarrollo STM32 NUCLEO-64 G071RB (https://www.st.com/en/evaluation-tools/nucleo-g071rb.html)
    - motores paso a paso
    - sistema de movimiento polar
    - bluetooth BLE
    - amplificador de audio y altavoces
    - bola de acero
    - estructura de la mesa cinética
    - fuente de alimentación
    - pantalla oled 1.8" TFT RGB SPI 128x160 ST7735S  3V3

Driver para stlink: https://www.st.com/en/development-tools/stsw-link009.html
*/

#include <Arduino.h>

void setup()
{
}

void loop()
{
}
