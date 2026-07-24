#ifndef HR_SYNCHRONIZATION_INSPECTOR_H
#define HR_SYNCHRONIZATION_INSPECTOR_H

#include "mutex.h"
#include "semaphore.h"

void hr_sync_inspector_bind(hr_semaphore_t *event_sem,
                            hr_semaphore_t *count_sem,
                            hr_mutex_t *resource_mutex);
void hr_sync_inspector_print_help(void);
void hr_sync_inspector_print_tasks(void);
void hr_sync_inspector_print_delayed(void);
void hr_sync_inspector_print_semaphores(void);
void hr_sync_inspector_print_mutex(void);
void hr_sync_inspector_print_validation(void);

#endif
