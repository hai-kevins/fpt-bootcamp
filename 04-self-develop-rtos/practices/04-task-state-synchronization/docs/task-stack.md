# Task Stack

Stack Cortex-M3 tăng xuống. Initial frame gồm software frame R4-R11 và hardware frame R0-R3, R12, LR, PC, xPSR. xPSR phải có Thumb bit. Stack guard phát hiện overflow chạm đáy; high-water quét pattern để ước lượng usage lớn nhất.
