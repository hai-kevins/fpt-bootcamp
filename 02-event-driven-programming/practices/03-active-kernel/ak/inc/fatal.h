#ifndef AK_FATAL_H
#define AK_FATAL_H

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint32_t magic;
    uint32_t error_code;
    uint32_t timestamp;
    uint32_t restart_count;
    uint16_t signal;
    uint8_t task_id;
    uint8_t reserved;
    uint32_t checksum;
} ak_fatal_record_t;

void ak_fatal_boot(void);
void ak_fatal_clear(void);
bool ak_fatal_record_valid(void);
ak_fatal_record_t ak_fatal_record_get(void);
void ak_fatal_capture(uint32_t error_code, uint8_t task_id, uint16_t signal);
void ak_fatal_raise(uint32_t error_code, uint8_t task_id, uint16_t signal);

#endif
