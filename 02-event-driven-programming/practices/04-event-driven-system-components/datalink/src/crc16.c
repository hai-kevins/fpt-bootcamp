#include "crc16.h"

uint16_t ed_crc16_ccitt(const uint8_t *data, size_t length)
{
    uint16_t crc = 0xFFFFU;

    if ((data == NULL) && (length > 0U))
    {
        return 0U;
    }

    for (size_t i = 0U; i < length; i++)
    {
        crc ^= (uint16_t)((uint16_t) data[i] << 8U);
        for (uint8_t bit = 0U; bit < 8U; bit++)
        {
            if ((crc & 0x8000U) != 0U)
            {
                crc = (uint16_t)(((uint32_t) crc << 1U) ^ UINT32_C(0x1021));
            }
            else
            {
                crc = (uint16_t)((uint32_t) crc << 1U);
            }
        }
    }
    return crc;
}
