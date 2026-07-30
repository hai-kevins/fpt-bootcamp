#ifndef AK_H
#define AK_H

#include "ak_config.h"
#include "event_record.h"
#include "fatal.h"
#include "fsm.h"
#include "message.h"
#include "task.h"
#include "timer.h"
#include "tsm.h"

void ak_init(void);
bool ak_run_once(void);
uint32_t ak_run_until_idle(uint32_t maximum_steps);
void ak_process_time(uint32_t now_ms);

#endif
