#ifndef MODEL_H
#define MODEL_H
#include <stdbool.h>
typedef enum{READY=0,RUNNING,BLOCKED,SUSPENDED}state_t;typedef struct{state_t state;bool in_ready;unsigned priority;}task_t;
bool block_task(task_t*t);bool wake_task(task_t*t);bool suspend_task(task_t*t);bool resume_task(task_t*t);bool validate_task(const task_t*t);
#endif
