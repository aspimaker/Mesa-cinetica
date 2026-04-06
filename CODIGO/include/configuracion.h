#pragma once

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h> // DFRobotDFPlayerMini
#include <Adafruit_NeoPixel.h>   // Adafruit_NeoPixel (usado en aplicarConfiguracion via rgb)
#include "pines.h"               // TFT_BLK

// Objetos externos que este header usa directamente en sus métodos inline.
// Se definen en main.cpp / globals definición.
extern DFRobotDFPlayerMini myDFPlayer;
extern Adafruit_NeoPixel rgb;

// Declaración del manejador RTC (se define en main.cpp)
extern RTC_HandleTypeDef rtc;

// ============================================
// DATOS DE CONFIGURACIÓN
// ============================================

struct __attribute__((packed)) DatosConfiguracion
{
    // Pantalla TFT
    uint8_t brillo; // 0-255

    // DFPlayer Mini
    uint8_t volumen;          // 0-30
    uint8_t ecualizador;      // 0-5
    uint8_t modoReproduccion; // 0-3
    uint16_t ultimaPista;
    uint16_t ultimaCarpeta;

    // TPA3110 (amplificador)
    uint8_t volumenAmplificador; // 0-100
    bool mute;

    // LEDs WS2812B
    uint8_t ledModo;   // 0-4
    uint8_t ledBrillo; // 0-255
    uint32_t ledColor;
    uint8_t ledVelocidad;

    // Temporizadores
    uint8_t autoApagado; // 0=off, 1-60 minutos
    uint8_t alarmaHora;
    uint8_t alarmaMinuto;
    bool alarmaActivada;

    uint8_t checksum;

    DatosConfiguracion()
    {
        brillo = 255;
        volumen = 20;
        ecualizador = 0;
        modoReproduccion = 0;
        ultimaPista = 1;
        ultimaCarpeta = 2;
        volumenAmplificador = 70;
        mute = false;
        ledModo = 2;
        ledBrillo = 128;
        ledColor = 0x00FF00;
        ledVelocidad = 100;
        autoApagado = 0;
        alarmaHora = 10;
        alarmaMinuto = 0;
        alarmaActivada = false;
        checksum = 0;
    }

    uint8_t calcularChecksum()
    {
        uint8_t sum = 0;
        uint8_t *ptr = (uint8_t *)this;
        for (size_t i = 0; i < sizeof(DatosConfiguracion) - 1; i++)
        {
            sum += ptr[i];
        }
        return sum;
    }

    bool isValid()
    {
        return (checksum == calcularChecksum());
    }

    void actualizarChecksum()
    {
        checksum = calcularChecksum();
    }
};

class Configuracion
{
private:
    DatosConfiguracion _datos;
    bool _cargada;
    bool _rtcInicializado;

    void _guardarEnBackup()
    {
        uint32_t word;
        uint8_t *ptr = (uint8_t *)&_datos;
        int words = (int)(sizeof(DatosConfiguracion) / 4);

        for (int i = 0; i < words && i < 5; i++)
        {
            memcpy(&word, ptr + i * 4, 4);
            HAL_RTCEx_BKUPWrite(&rtc, RTC_BKP_DR0 + i, word);
        }
    }

    void _cargarDesdeBackup()
    {
        if (!_rtcInicializado) // ← añadir este guard
        {
            _datos = DatosConfiguracion();
            _datos.actualizarChecksum();
            _cargada = false;
            return;
        }

        uint32_t word;
        uint8_t *ptr = (uint8_t *)&_datos;
        int words = (int)(sizeof(DatosConfiguracion) / 4);
        for (int i = 0; i < words && i < 5; i++)
        {
            word = HAL_RTCEx_BKUPRead(&rtc, RTC_BKP_DR0 + i);
            memcpy(ptr + i * 4, &word, 4);
        }

        if (_datos.isValid() && _datos.brillo != 0xFF && _datos.brillo != 0)
            _cargada = true;
        else
        {
            _datos = DatosConfiguracion();
            _datos.actualizarChecksum();
            _cargada = false;
        }
    }

   
 

public:
    Configuracion() : _rtcInicializado(false), _cargada(false)
    {
    }

    void begin()
    {
        if (!_rtcInicializado)
            Serial.println("*******************");
        // _inicializarRTC();
        _cargarDesdeBackup();
    }

    DatosConfiguracion &get() { return _datos; }

    void save()
    {
        _datos.actualizarChecksum();
        _guardarEnBackup();
        _cargada = true;
    }

    // ── Acceso rápido ──────────────────────────────────────────

    void setBrillo(uint8_t valor)
    {
        _datos.brillo = (uint8_t)constrain(valor, 0, 255);
        analogWrite(TFT_BLK, _datos.brillo);
        save();
    }

    void setVolumen(uint8_t valor)
    {
        _datos.volumen = (uint8_t)constrain(valor, 0, 30);
        myDFPlayer.volume(_datos.volumen);
        save();
    }

    void setUltimaPista(uint16_t pista, uint16_t carpeta)
    {
        _datos.ultimaPista = pista;
        _datos.ultimaCarpeta = carpeta;
        save();
    }

    uint16_t getUltimaPista() { return _datos.ultimaPista; }
    uint16_t getUltimaCarpeta() { return _datos.ultimaCarpeta; }

    void setAutoApagado(uint8_t minutos)
    {
        _datos.autoApagado = (uint8_t)constrain(minutos, 0, 60);
        save();
    }

    void setLedModo(uint8_t modo)
    {
        _datos.ledModo = (uint8_t)constrain(modo, 0, 4);
        save();
    }

    void setAlarma(uint8_t hora, uint8_t minuto, bool activar)
    {
        _datos.alarmaHora = hora;
        _datos.alarmaMinuto = minuto;
        _datos.alarmaActivada = activar;

        if (activar)
        {
            RTC_AlarmTypeDef sAlarm = {0};
            sAlarm.AlarmTime.Hours = hora;
            sAlarm.AlarmTime.Minutes = minuto;
            sAlarm.AlarmTime.Seconds = 0;
            sAlarm.AlarmMask = RTC_ALARMMASK_SECONDS;
            sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
            sAlarm.AlarmDateWeekDay = 1;
            HAL_RTC_SetAlarm_IT(&rtc, &sAlarm, RTC_FORMAT_BIN);
        }
        else
        {
            HAL_RTC_DeactivateAlarm(&rtc, RTC_ALARM_A);
        }
        save();
    }

    void getDateTime(uint8_t &hora, uint8_t &minuto, uint8_t &segundo,
                     uint8_t &dia, uint8_t &mes, uint16_t &año)
    {
        RTC_TimeTypeDef sTime;
        RTC_DateTypeDef sDate;
        HAL_RTC_GetTime(&rtc, &sTime, RTC_FORMAT_BIN);
        HAL_RTC_GetDate(&rtc, &sDate, RTC_FORMAT_BIN);

        hora = sTime.Hours;
        minuto = sTime.Minutes;
        segundo = sTime.Seconds;
        dia = sDate.Date;
        mes = sDate.Month;
        año = sDate.Year;
    }
    
    void aplicarConfiguracion()
    {
        analogWrite(TFT_BLK, _datos.brillo); // brillo de la pantalla tft

        myDFPlayer.volume(_datos.volumen);
        myDFPlayer.EQ(_datos.ecualizador);

        switch (_datos.modoReproduccion)
        {
        case 0:
            myDFPlayer.enableLoop();
            break; // Repetir todas
        case 1:
            myDFPlayer.randomAll();
            break; // Aleatorio
        case 2:
            if (_datos.ultimaPista > 0)
                myDFPlayer.loop(_datos.ultimaPista);
            break; // Repetir una
        case 3:
            myDFPlayer.disableLoop();
            break; // Normal
        }

        rgb.setBrightness(_datos.ledBrillo);
    }

    void setLedBrillo(uint8_t brillo)
    {
        _datos.ledBrillo = brillo;
        save();
    }
    uint8_t getLedBrillo() { return _datos.ledBrillo; }
    bool isCargada() { return _cargada; }
};