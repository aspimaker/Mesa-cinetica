#include "utils.h"

void resetearDispositivo()
{
    HAL_NVIC_SystemReset();
}

String getDeviceID()
{
    uint32_t word0 = *(__IO uint32_t *)(UID_BASE_ADDR);
    uint32_t word1 = *(__IO uint32_t *)(UID_BASE_ADDR + 4);
    uint32_t word2 = *(__IO uint32_t *)(UID_BASE_ADDR + 8);

    char uid[27];
    snprintf(uid, sizeof(uid), "%08lX-%08lX-%08lX", word0, word1, word2);

    return String(uid);
}

/*
uint32_t getDeviceIDHash()
{
    uint32_t word0 = *(__IO uint32_t *)(UID_BASE_ADDR);
    uint32_t word1 = *(__IO uint32_t *)(UID_BASE_ADDR + 4);
    uint32_t word2 = *(__IO uint32_t *)(UID_BASE_ADDR + 8);

    return word0 ^ word1 ^ word2;
}
*/

uint32_t getDeviceIDHash()
{
    // Tratamos la dirección como un array de 3 palabras de 32 bits
    auto* uid = reinterpret_cast<const uint32_t*>(UID_BASE_ADDR);

    return uid[0] ^ uid[1] ^ uid[2];
}