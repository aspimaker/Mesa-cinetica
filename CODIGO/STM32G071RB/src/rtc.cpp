#include "rtc.h"
#include "desactivaLog.h"

void RTC_InitRTC_LSI()
{
    // utilizar el oscilador interno LSI

    // 1. Activar relojes de bus y energía
    RCC->APBENR1 |= RCC_APBENR1_PWREN;
    RCC->APBENR1 |= RCC_APBENR1_RTCAPBEN;
    HAL_PWR_EnableBkUpAccess();

    // 2. SIEMPRE rearrancar el LSI, independientemente de todo
    RCC->CSR |= RCC_CSR_LSION;
    while (!(RCC->CSR & RCC_CSR_LSIRDY))
        ;

    // 3. ¿Ya estaba configurado el RTC alguna vez?
    if ((RCC->BDCR & RCC_BDCR_RTCEN) && (TAMP->BKP0R == 0x4224))
    {
        // RTC ya configurado — solo esperar resincronización
        RTC->WPR = 0xCA;
        RTC->WPR = 0x53;
        RTC->ICSR &= ~RTC_ICSR_RSF;
        while (!(RTC->ICSR & RTC_ICSR_RSF))
            ;
        RTC->WPR = 0xFF;

        DEBUG_PRINTLN("RTC: Manteniendo conteo previo.");
        return;
    }

    // 4. Configuración desde cero
    DEBUG_PRINTLN("RTC: Configurando por primera vez...");

    uint32_t bdcr = RCC->BDCR;
    bdcr &= ~RCC_BDCR_RTCSEL;
    bdcr |= RCC_BDCR_RTCSEL_1; // LSI
    bdcr |= RCC_BDCR_RTCEN;
    RCC->BDCR = bdcr;

    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    RTC->ICSR |= RTC_ICSR_INIT;
    while (!(RTC->ICSR & RTC_ICSR_INITF))
        ;

    RTC->PRER = (127 << 16) | 249;

    // Fecha/hora inicial solo si nunca se ha grabado
    DEBUG_PRINTLN("RTC: Grabando fecha/hora inicial...");
    _fechaHoraPorDefecto();

    TAMP->BKP0R = 0x4224;

    RTC->ICSR &= ~RTC_ICSR_INIT;
    RTC->WPR = 0xFF;
}

void RTC_InitRTC_LSE()
{
    // utilizar el oscilador externo LSE

    RCC->APBENR1 |= RCC_APBENR1_PWREN;
    RCC->APBENR1 |= RCC_APBENR1_RTCAPBEN;
    HAL_PWR_EnableBkUpAccess();

    // Drive mínimo — condensadores externos 6.8pF
    RCC->BDCR &= ~RCC_BDCR_LSEDRV;

    // Encender LSE con timeout y fallback a LSI
    RCC->BDCR |= RCC_BDCR_LSEON;
    uint32_t t = HAL_GetTick();
    bool usandoLSE = true;
    while (!(RCC->BDCR & RCC_BDCR_LSERDY))
    {
        if (HAL_GetTick() - t > 2000)
        {
            DEBUG_PRINTLN("LSE timeout! Usando LSI...");
            RCC->CSR |= RCC_CSR_LSION;
            while (!(RCC->CSR & RCC_CSR_LSIRDY))
                ;
            usandoLSE = false;
            break;
        }
    }

    if (usandoLSE)
        DEBUG_PRINTLN("LSE listo.");

    // ¿Ya estaba configurado?
    if ((RCC->BDCR & RCC_BDCR_RTCEN) && (TAMP->BKP0R == 0x4225))
    {
        RTC->WPR = 0xCA;
        RTC->WPR = 0x53;
        RTC->ICSR &= ~RTC_ICSR_RSF;
        while (!(RTC->ICSR & RTC_ICSR_RSF))
            ;
        RTC->WPR = 0xFF;
        DEBUG_PRINTLN("RTC: Manteniendo fecha/hora.");
        return;
    }

    // Configurar desde cero
    DEBUG_PRINTLN("RTC: Configurando por primera vez...");

    uint32_t bdcr = RCC->BDCR;
    bdcr &= ~RCC_BDCR_RTCSEL;
    bdcr |= usandoLSE ? RCC_BDCR_RTCSEL_0 : RCC_BDCR_RTCSEL_1;
    bdcr |= RCC_BDCR_RTCEN;
    RCC->BDCR = bdcr;

    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    RTC->ICSR |= RTC_ICSR_INIT;
    while (!(RTC->ICSR & RTC_ICSR_INITF))
        ;

    if (usandoLSE)
        RTC->PRER = (127 << 16) | 255; // 32768 Hz → 1 Hz exacto
    else
        RTC->PRER = (127 << 16) | 249; // LSI ~32 kHz

    DEBUG_PRINTLN("RTC: Grabando fecha/hora inicial...");
    _fechaHoraPorDefecto();

    TAMP->BKP0R = 0x4225; // Magic number nuevo para forzar reconfiguración

    RTC->ICSR &= ~RTC_ICSR_INIT;
    RTC->WPR = 0xFF;
}

void RTC_GetFechaHora(FechaHora &t)
{
    /*
        // Esperar sincronización — obligatorio antes de leer TR/DR
        RTC->WPR = 0xCA;
        RTC->WPR = 0x53;
        RTC->ICSR &= ~RTC_ICSR_RSF;          // limpiar flag
        RTC->WPR = 0xFF;

        uint32_t timeout = HAL_GetTick();
        while (!(RTC->ICSR & RTC_ICSR_RSF))
        {
            if (HAL_GetTick() - timeout > 200)
            {
                DEBUG_PRINTLN("RTC: timeout RSF");
                return;
            }
        }
    */

    // Leer PRIMERO TR, LUEGO DR — regla de hardware STM32
    uint32_t tr = RTC->TR;
    uint32_t dr = RTC->DR;

    // Hora
    t.horas = ((tr >> 16) & 0xF) + ((tr >> 20) & 0x3) * 10;
    t.minutos = ((tr >> 8) & 0xF) + ((tr >> 12) & 0x7) * 10;
    t.segundos = (tr & 0xF) + ((tr >> 4) & 0x7) * 10;

    // Fecha
    t.dia = (dr & 0xF) + ((dr >> 4) & 0x3) * 10;
    t.mes = ((dr >> 8) & 0xF) + ((dr >> 12) & 0x1) * 10;
    t.año = ((dr >> 16) & 0xF) + ((dr >> 20) & 0xF) * 10 + 2000;
    t.diaSemana = (dr >> 13) & 0x7; // 1=Lunes ... 7=Domingo
}

void RTC_SetFechaHora(uint8_t d, uint8_t mes, uint16_t a, uint8_t h, uint8_t min, uint8_t s, uint8_t diaSemana)
{
    // diaSemana: 1=Lunes ... 7=Domingo (formato RTC)

    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    RTC->ICSR |= RTC_ICSR_INIT;
    while (!(RTC->ICSR & RTC_ICSR_INITF))
        ;

    // Convertir a BCD
    uint32_t d_bcd = ((d / 10) << 4) | (d % 10);
    uint32_t mes_bcd = ((mes / 10) << 4) | (mes % 10);
    uint32_t a_bcd = (((a - 2000) / 10) << 4) | ((a - 2000) % 10);
    uint32_t h_bcd = ((h / 10) << 4) | (h % 10);
    uint32_t min_bcd = ((min / 10) << 4) | (min % 10);
    uint32_t s_bcd = ((s / 10) << 4) | (s % 10);

    // Hora
    RTC->TR = (h_bcd << RTC_TR_HU_Pos) | (min_bcd << RTC_TR_MNU_Pos) | (s_bcd << RTC_TR_SU_Pos);

    // Fecha (día semana obligatorio)
    RTC->DR = (a_bcd << RTC_DR_YU_Pos) | (mes_bcd << RTC_DR_MU_Pos) | (d_bcd << RTC_DR_DU_Pos) | (diaSemana << RTC_DR_WDU_Pos);

    RTC->ICSR &= ~RTC_ICSR_INIT;
    RTC->WPR = 0xFF;
}

void _fechaHoraPorDefecto()
{
    RTC_SetFechaHora(6, 4, 2026, 12, 0, 0, 1);
}