#ifndef ED_CRC16_H
#define ED_CRC16_H

#include <stddef.h>
#include <stdint.h>

uint16_t ed_crc16_ccitt(const uint8_t *data, size_t length);

#endif
