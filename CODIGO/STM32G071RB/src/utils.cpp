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

uint32_t getDeviceIDHash()
{
    uint32_t word0 = *(__IO uint32_t *)(UID_BASE_ADDR);
    uint32_t word1 = *(__IO uint32_t *)(UID_BASE_ADDR + 4);
    uint32_t word2 = *(__IO uint32_t *)(UID_BASE_ADDR + 8);

    return word0 ^ word1 ^ word2;
}