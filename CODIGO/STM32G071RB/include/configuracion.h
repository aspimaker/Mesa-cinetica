#pragma once

#include "depuracion.h"
#include <Arduino.h>
#include "depuracion.h"
// #include <DFRobotDFPlayerMini.h> // DFRobotDFPlayerMini
#include <Adafruit_NeoPixel.h> // Adafruit_NeoPixel (usado en aplicarConfiguracion via rgb)
#include "pines.h"             // Pinout::TFT::BLK
#include "audio.h"

// extern DFRobotDFPlayerMini myDFPlayer;
extern Adafruit_NeoPixel rgb;
extern RTC_HandleTypeDef rtc;

struct __attribute__((packed)) DatosConfiguracion
{
    // pantalla TFT
    uint8_t brillo; // 0-255

    // DFPlayer Mini
    uint8_t volumen;          // 0-30
    uint8_t ecualizador;      // 0-5
    uint8_t modoReproduccion; // 0-3
    uint16_t ultimaPista;
    bool mute;

    // leds WS2812B
    uint8_t ledModo;   // 0-4
    uint8_t ledBrillo; // 0-255
    uint32_t ledColor;
    uint8_t ledVelocidad;

    // temporizadores
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
        mute = false;
        ledModo = 2;
        ledBrillo = 128;
        ledColor = 0x00FF00;
        ledVelocidad = 100;
        autoApagado = 0;
        alarmaHora = 0;
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
            // _inicializarRTC();
            _cargarDesdeBackup();
    }

    DatosConfiguracion &get() { return _datos; }

    DatosConfiguracion &set() { 
        _datos.actualizarChecksum();
        return _datos; 
    }


    void save()
    {
        _datos.actualizarChecksum();
        _guardarEnBackup();
        _cargada = true;
    }

    void setBrillo(uint8_t valor)
    {
        _datos.brillo = (uint8_t)constrain(valor, 0, 255);
        analogWrite(Pinout::TFT::BLK, _datos.brillo);
        // save();
    }

    void setVolumen(uint8_t valor)
    {
        _datos.volumen = (uint8_t)constrain(valor, 0, 30);
        myDFPlayer.setVolume(_datos.volumen);
        // save();
    }

    void setUltimaPista(uint16_t pista)
    {
        _datos.ultimaPista = pista;
        _datos.checksum;
        //_datos.ultimaCarpeta = carpeta;
        // save();
    }

    uint16_t getUltimaPista() { return _datos.ultimaPista; }
    // uint16_t getUltimaCarpeta() { return _datos.ultimaCarpeta; }

    void setAutoApagado(uint8_t minutos)
    {
        _datos.autoApagado = (uint8_t)constrain(minutos, 0, 60);
        save();
    }

    void setLedModo(uint8_t modo)
    {
        _datos.ledModo = (uint8_t)constrain(modo, 0, 4);
        // save();
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

    void aplicarConfiguracion()
    {
        analogWrite(Pinout::TFT::BLK, _datos.brillo); // brillo de la pantalla tft

        myDFPlayer.setVolume(_datos.volumen);
        myDFPlayer.setEq((DfMp3_Eq)_datos.ecualizador);

        _datos.modoReproduccion = 3;

        dP("modoReproduccion: (");
        dP(_datos.modoReproduccion);
        dP(")");

        
        switch (_datos.modoReproduccion)
        {
        case 0:
            dPln("repetir todas");
            myDFPlayer.setRepeatPlayAllInRoot(true);
            break;

        case 1:
            dPln("aleatorio");
            myDFPlayer.playRandomTrackFromAll();
            break;

        case 2:
            dPln("repetir una");
            if (_datos.ultimaPista > 0)
                myDFPlayer.setRepeatPlayCurrentTrack(true);
            break;

        case 3:
            dPln("normal");
            myDFPlayer.setRepeatPlayAllInRoot(false);
            break;
        }

        myDFPlayer.stop();

        rgb.setBrightness(_datos.ledBrillo);
    }

    void setLedBrillo(uint8_t brillo)
    {
        _datos.ledBrillo = brillo;
        // save();
    }
    uint8_t getLedBrillo() { return _datos.ledBrillo; }
    bool isCargada() { return _cargada; }
};