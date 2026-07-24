# Semaphore

Binary semaphore có count 0/1. Counting semaphore có `0 <= count <= max_count`. Semaphore không có owner và phù hợp ISR-to-task signaling.

Khi give gặp waiter, wake waiter trực tiếp và không tăng count.
