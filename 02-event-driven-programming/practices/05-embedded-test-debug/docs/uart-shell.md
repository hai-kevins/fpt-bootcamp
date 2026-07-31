# UART Shell

Shell trong repository dùng chuỗi command nhưng không phụ thuộc driver UART thật.

Command:

```text
help
state
event start
event stop
event fail
event reset
stats
trace
fault pool on
fault pool off
```

Trên target, UART RX nên đưa byte vào ring buffer hoặc post event cho Shell Active Object. Command handler nên inject event thay vì truy cập hardware/application state trực tiếp.
