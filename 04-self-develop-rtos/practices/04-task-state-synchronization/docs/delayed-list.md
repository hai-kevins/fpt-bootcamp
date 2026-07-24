# Delayed Task List

Delayed list được sắp theo `wake_tick`. SysTick chỉ xử lý head tới khi task đầu chưa tới hạn. Finite wait dùng `timeout_node`; pure delay không cần object wait list.

Tick wrap được xử lý bằng signed difference và timeout horizon nhỏ hơn `INT32_MAX`.
