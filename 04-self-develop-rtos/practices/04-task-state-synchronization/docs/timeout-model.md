# Timeout Model

RTOS dùng ba mode: `RTOS_NO_WAIT`, finite ticks và `RTOS_WAIT_FOREVER`. Kết quả wait gồm SUCCESS, TIMEOUT, CANCELLED và SUSPENDED.

Timeout và object event cạnh tranh theo single-winner rule trong critical section.
