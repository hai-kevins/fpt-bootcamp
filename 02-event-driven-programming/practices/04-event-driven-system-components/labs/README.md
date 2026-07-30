# Labs - Event-Driven System Components

Mỗi thư mục lab là một build unit độc lập.

- Không build lab từ Makefile gốc.
- Đi vào đúng thư mục lab rồi chạy Makefile của lab đó.
- Output được tạo trong `build/` bên trong chính lab.
- `make clean` chỉ xóa output của lab hiện tại.
- `make sanitize` chạy AddressSanitizer và UndefinedBehaviorSanitizer.
- Tất cả lab hiện được đánh số liên tục từ `01` đến `15`.

## Danh sách lab

| Số | Thư mục | Nội dung | Kết quả trọng tâm |
|---:|---|---|---|
| 01 | `01-active-object` | Active Object | Handler xử lý đúng một event |
| 02 | `02-mailbox` | Mailbox FIFO | FIFO, overflow và high-water mark |
| 03 | `03-run-to-completion-scheduler` | Scheduler | Priority cao được dispatch trước |
| 04 | `04-flat-state-machine` | Flat FSM | Transition và action |
| 05 | `05-hierarchical-state-machine` | HSM | Event bubbling lên parent |
| 06 | `06-table-driven-state-machine` | Table FSM | Chuỗi transition theo bảng |
| 07 | `07-event-pool` | Static Event Pool | Exhaustion và release toàn bộ |
| 08 | `08-reference-count-ownership` | Ownership | Retain/release đúng reference count |
| 09 | `09-router-direct-post` | Router | Local route tới đúng Active Object |
| 10 | `10-publish-subscribe` | PubSub | Hai subscriber nhận cùng signal |
| 11 | `11-uart-datalink` | Frame serialization | Encode/decode và CRC |
| 12 | `12-parser-state-machine` | Byte parser | Tạo frame sau khi nhận đủ byte |
| 13 | `13-link-failure` | Timeout/retry | Chuyển link sang DOWN |
| 14 | `14-event-trace` | Trace ring buffer | Overwrite record cũ nhất |
| 15 | `15-stress-test` | Stress test | Không leak event sau tải lớn |

## Quy trình chạy

```bash
cd labs/01-active-object
make
make run
make sanitize
make clean
```

Kiểm tra toàn bộ repository:

```bash
./tools/validate.sh
```

Không có target `make labXX` ở root. Cách tổ chức này tránh nhầm artifact root với artifact của từng lab.
