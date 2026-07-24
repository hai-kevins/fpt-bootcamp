# Mutex

Mutex có owner và chỉ owner được unlock. Phiên bản này non-recursive. Mutex không được give từ ISR.

Waiters được sắp theo effective priority, FIFO giữa task cùng priority.
