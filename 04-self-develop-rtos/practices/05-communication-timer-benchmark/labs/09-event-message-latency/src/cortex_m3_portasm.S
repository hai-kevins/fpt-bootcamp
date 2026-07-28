.syntax unified
.cpu cortex-m3
.thumb

.extern g_current_task
.extern hr_scheduler_commit_switch

.global SVC_Handler
.type SVC_Handler, %function
.thumb_func
SVC_Handler:
    cpsid   i
    ldr     r0, =g_current_task
    ldr     r0, [r0]
    ldr     r0, [r0]
    ldmia   r0!, {r4-r11}
    msr     psp, r0
    isb
    movs    r0, #2
    msr     control, r0
    isb
    ldr     lr, =0xFFFFFFFD
    cpsie   i
    bx      lr
.size SVC_Handler, .-SVC_Handler

.global PendSV_Handler
.type PendSV_Handler, %function
.thumb_func
PendSV_Handler:
    cpsid   i
    mrs     r0, psp
    isb
    stmdb   r0!, {r4-r11}

    ldr     r1, =g_current_task
    ldr     r2, [r1]
    str     r0, [r2]

    push    {r3, lr}
    bl      hr_scheduler_commit_switch
    pop     {r3, lr}

    ldr     r1, =g_current_task
    ldr     r2, [r1]
    ldr     r0, [r2]
    ldmia   r0!, {r4-r11}
    msr     psp, r0
    isb
    cpsie   i
    bx      lr
.size PendSV_Handler, .-PendSV_Handler
