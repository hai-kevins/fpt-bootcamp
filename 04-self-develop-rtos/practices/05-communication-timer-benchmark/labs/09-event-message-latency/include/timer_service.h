#ifndef HR_TIMER_SERVICE_H
#define HR_TIMER_SERVICE_H

#include "semaphore.h"

void hr_timer_service_bind(hr_semaphore_t *signal);
void hr_timer_service_task(void *argument);

#endif
