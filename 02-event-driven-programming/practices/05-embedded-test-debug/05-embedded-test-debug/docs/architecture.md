# Test & Debug Architecture

```text
Application / State Machine
        |
        +--> Event Queue
        +--> Software Timer
        +--> Event Pool
        |
        v
Test & Debug Services
        |
        +--> UART-style Shell
        +--> Binary Event Trace
        +--> Statistics
        +--> Crash Record
        +--> Fault Injection
        |
        v
Host Test / Target Adapter / HIL
```

Các component logic không phụ thuộc STM32 register. Platform thật chỉ cần cung cấp UART transport, monotonic tick, reset reason và persistent storage.
