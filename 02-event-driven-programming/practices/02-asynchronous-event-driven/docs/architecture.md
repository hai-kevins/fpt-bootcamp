# Architecture

## Tổng quan

```text
Hardware IRQ / Software Timer / Application
                   |
                   v
               Event
                   |
                   v
            Circular Queue
                   |
                   v
              Dispatcher
       +-----------+-----------+
       |           |           |
       v           v           v
   Button SM    App SM       LED SM
       |           |           |
       +-----------+-----------+
                   |
                   v
              UART Service
```

## Trách nhiệm

| Thành phần | Trách nhiệm |
|---|---|
| `event_queue` | Lưu event FIFO bằng bộ nhớ tĩnh |
| `dispatcher` | Post, lấy event và route theo destination |
| `software_timer` | Chuyển thời gian thành timeout event |
| `button` | Debounce và phát pressed/released |
| `app` | Quản lý BOOT/IDLE/ACTIVE/ERROR |
| `led` | Quản lý OFF/ON/BLINKING |
| `uart_service` | Gom dòng lệnh và phát command event |
| `event_trace` | Ghi vòng đời event vào ring buffer |
| `platform` | GPIO, UART, SysTick, critical section |

## Run-to-completion

Mỗi handler phải kết thúc nhanh:

```text
Get event
  |
  v
Call one handler
  |
  v
Handler updates state / posts event / arms timer
  |
  v
Return to dispatcher
```

Không dùng delay blocking trong handler.
