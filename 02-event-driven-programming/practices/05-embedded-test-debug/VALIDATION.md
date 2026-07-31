# Validation

Repository được kiểm tra theo các bước:

```bash
make clean
make test
make run
make sanitize
./tools/validate.sh
```

Tiêu chí:

- Root test runner: 12/12 PASS.
- Root integrated demo: exit code 0.
- Root ASan/UBSan: không báo lỗi.
- 18 lab `make run`: PASS.
- 18 lab `make sanitize`: PASS.
- ZIP integrity: PASS.

Lưu ý:

- Validation hiện chạy trên host Linux.
- Chưa kiểm thử trực tiếp trên STM32, UART vật lý hoặc HIL bench thật.
- `tools/run_hil_tests.py` là transcript validator, không điều khiển serial port thật.
