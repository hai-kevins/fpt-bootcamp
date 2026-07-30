# Labs - Active Kernel

Mỗi lab là một build unit độc lập và được build trong thư mục của chính lab.

```bash
cd labs/08-fsm
make test
make run
```

| Lab | Nội dung | Môi trường |
|---:|---|---|
| 01 | Đọc cấu trúc AK | Host |
| 02 | Hai task và message routing | Host |
| 03 | Pure Message | Host |
| 04 | Common Message | Host |
| 05 | Dynamic Message và pool | Host |
| 06 | One-shot Timer | Host |
| 07 | Periodic Timer | Host |
| 08 | Function-based FSM | Host |
| 09 | Table State Machine | Host |
| 10 | Command Line qua message | Host |
| 11 | Pool Stress Test | Host |
| 12 | Event Record | Host |
| 13 | Fatal Record | Host |

Root Makefile không build các lab. `make clean` trong một lab chỉ xóa `build/` của lab hiện tại.
