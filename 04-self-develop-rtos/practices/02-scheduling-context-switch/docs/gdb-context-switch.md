# Debug context switch với GDB

Breakpoint gợi ý:

```gdb
break SVC_Handler
break PendSV_Handler
break SysTick_Handler
break HardFault_Handler
break rtos_scheduler_commit_switch
```

Quan sát `$msp`, `$psp`, `$control`, `g_current_task` và vùng nhớ tại
`g_current_task->saved_sp`. Khi fault sau exception return, kiểm tra Thumb bit,
stack alignment, saved PC, EXC_RETURN và CFSR/HFSR.
