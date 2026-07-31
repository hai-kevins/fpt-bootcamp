# Binary Event Trace Format

Record:

```c
typedef struct
{
    uint32_t timestamp_ms;
    uint16_t signal;
    uint16_t argument;
    uint8_t type;
    uint8_t source;
    uint8_t destination;
    uint8_t state;
} event_trace_record_t;
```

Nguyên tắc:

- Kích thước cố định.
- Dùng ID thay pointer.
- Write path ngắn.
- Ring buffer overwrite-oldest.
- Decode tên signal/state trên PC bằng map tương ứng build.
