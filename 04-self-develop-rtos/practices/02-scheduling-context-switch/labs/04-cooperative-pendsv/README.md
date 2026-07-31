# Lab 04 - Cooperative PendSV Context Switch

## Mục tiêu

- Switch cooperative giữa hai task cùng priority bằng `rtos_task_yield()`.
- Kiểm tra PendSV lưu `R4-R11`, đổi `saved_sp` và phục hồi local context.
- Bỏ lưu một register có chủ đích rồi quan sát biến local hoặc task state bị hỏng.

## Build và chạy

Makefile của lab này chỉ build firmware của chính lab. Từ thư mục root:

```bash
cd labs/04-cooperative-pendsv
make
```

Các lệnh thường dùng:

```bash
make size
make disasm
make flash-stlink
make clean
```

Nếu không có GNU Arm Embedded Toolchain, Makefile tự chuyển sang Clang/LLD.

## Kết quả

Artifact chính nằm cục bộ trong lab:

```text
build/lab04_cooperative_pendsv.elf
```

Flash firmware và quan sát `g_task_a_value`, `g_task_b_value` cùng `g_context_switch_count` bằng GDB. Hai counter phải tiếp tục tăng qua nhiều lần switch.

## Câu hỏi

1. Tại sao CPU không tự lưu `R4-R11` khi exception entry?
2. Vì sao PendSV phù hợp hơn việc switch trực tiếp trong task?
3. Điều gì bảo đảm Task A và Task B dùng stack riêng?
4. Nếu bỏ restore một register callee-saved thì lỗi có thể xuất hiện ra sao?
