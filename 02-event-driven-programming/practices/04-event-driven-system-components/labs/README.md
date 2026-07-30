# Labs — Independent Makefiles

Mỗi thư mục lab là một build unit độc lập.

- Không build lab từ Makefile root.
- Đi vào đúng thư mục lab rồi chạy Makefile của lab đó.
- Output được tạo trong `build/` bên trong chính lab.
- `make clean` chỉ xóa output của lab hiện tại.
- `make sanitize` chạy AddressSanitizer và UndefinedBehaviorSanitizer.
- Tất cả lab chạy trên host Linux.
- Thứ tự lab liên tục từ `01` đến `15`.

## Host labs

Ví dụ:

```bash
cd labs/01-active-object
make
make run
make sanitize
```

Danh sách:

| Lab | Artifact chính | Quan sát |
|---:|---|---|
| 01 | `build/lab` | `handled=1` |
| 02 | `build/lab` | FIFO, `hwm=2`, `overflow=1` |
| 03 | `build/lab` | `order=2,1` |
| 04 | `build/lab` | `state=0`, `actions=2` |
| 05 | `build/lab` | HSM bubbling |
| 06 | `build/lab` | `final=0`, `handled=3` |
| 07 | `build/lab` | Pool exhaustion và release |
| 08 | `build/lab` | Reference count về 0 |
| 09 | `build/lab` | Local route đúng object |
| 10 | `build/lab` | Hai subscriber nhận event |
| 11 | `build/lab` | Frame encode/decode và CRC |
| 12 | `build/lab` | Parser tạo frame hợp lệ |
| 13 | `build/lab` | Retry rồi link DOWN |
| 14 | `build/lab` | Trace ring buffer wraparound |
| 15 | `build/lab` | 1000 event không leak |

Các lệnh chung:

```bash
make          # Build executable
make run      # Build rồi chạy
make test     # Alias của make run
make sanitize # Build và chạy với ASan/UBSan
make clean    # Chỉ xóa build/ của lab hiện tại
```

## Kiểm tra toàn bộ

Từ thư mục root:

```bash
./tools/validate.sh
```

Script chạy:

```text
Root unit tests
Integrated root demo
Root sanitizer test
15 lab make run
15 lab make sanitize
```

Không có target `make labXX` ở root. Quy ước này tránh nhầm artifact root với executable của từng lab.
