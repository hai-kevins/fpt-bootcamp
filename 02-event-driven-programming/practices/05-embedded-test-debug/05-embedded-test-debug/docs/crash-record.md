# Crash Record

Crash Record root chứa:

- Magic.
- Version và size.
- Build ID.
- Reset reason.
- Fatal code.
- Uptime.
- State, component và signal cuối.
- Queue overflow và pool failure counter.
- Checksum.

Trên Cortex-M có thể mở rộng thêm:

```text
R0 R1 R2 R3 R12 LR PC xPSR
CFSR HFSR MMFAR BFAR
```

HardFault handler phải nhỏ, không allocate memory và không phụ thuộc scheduler.
