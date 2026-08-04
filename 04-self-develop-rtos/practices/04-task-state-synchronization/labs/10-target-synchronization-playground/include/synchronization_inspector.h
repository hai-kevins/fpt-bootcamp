#ifndef RTOS_SYNCHRONIZATION_INSPECTOR_H
#define RTOS_SYNCHRONIZATION_INSPECTOR_H

#include "mutex.h"
#include "semaphore.h"

void rtos_sync_inspector_bind(rtos_semaphore_t *event_sem, rtos_semaphore_t *count_sem, rtos_mutex_t *resource_mutex);
void rtos_sync_inspector_print_help(void);
void rtos_sync_inspector_print_tasks(void);
void rtos_sync_inspector_print_delayed(void);
void rtos_sync_inspector_print_semaphores(void);
void rtos_sync_inspector_print_mutex(void);
void rtos_sync_inspector_print_validation(void);

#endif
