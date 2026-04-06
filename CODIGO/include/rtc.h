#ifndef RTC_H
#define RTC_H

#include <Arduino.h>
#include <stm32g0xx_hal.h>

// Estructura para manejar la hora de forma sencilla
struct FechaHora
{
    uint8_t dia;
    uint8_t mes;
    uint16_t año;
    uint8_t horas;
    uint8_t minutos;
    uint8_t segundos;
    uint8_t diaSemana;
};

// Funciones principales
void RTC_InitRTC_LSI();
void RTC_InitRTC_LSE();
void RTC_GetFechaHora(FechaHora &t);
void RTC_SetFechaHora(uint8_t d, uint8_t mes, uint16_t a, uint8_t h, uint8_t min, uint8_t s, uint8_t diaSemana);
void _fechaHoraPorDefecto();
#endif