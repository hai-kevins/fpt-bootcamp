# Architecture

```text
Interrupts / SysTick / UART / Button
              |
              v
        Message creation
              |
              v
+-----------------------------------+
| Active Kernel                     |
| Message Pool -> Task Mailboxes    |
| Priority Scheduler                |
| Run-to-Completion Dispatch        |
| Software Timers                   |
| FSM / TSM                         |
| Event Record / Fatal Record       |
+-----------------------------------+
              |
              v
+-----------------------------------+
| Application Tasks                 |
| Button | App | LED | Shell | Sensor|
+-----------------------------------+
              |
              v
         Drivers / Platform
```

The kernel is cooperative and run-to-completion. A task handles one message and returns. No task may block or call delay inside its handler.
