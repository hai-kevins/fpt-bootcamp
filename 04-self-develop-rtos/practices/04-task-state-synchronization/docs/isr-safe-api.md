# ISR-safe API

ISR API không block, không gọi UART và không context-switch trực tiếp. `give_from_isr()` wake task trong critical section ngắn rồi pend PendSV.
