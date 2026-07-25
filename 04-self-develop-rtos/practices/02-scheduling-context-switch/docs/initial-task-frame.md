# Initial task frame

Task mới chưa có exception frame thật nên `rtos_task_create_static()` tạo một
frame giả. xPSR phải có Thumb bit, PC trỏ task entry, LR trỏ task-return panic
handler và R0 chứa argument. Phần R4-R11 được đặt ngay trước hardware frame để
SVC/PendSV dùng cùng một restore path.
