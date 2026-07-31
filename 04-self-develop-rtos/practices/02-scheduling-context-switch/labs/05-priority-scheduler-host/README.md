# Lab 05 - Priority Scheduler Policy on Host

## Mục tiêu

- Kiểm thử fixed-priority selection, FIFO queue, ready bitmap và rotation trên host.
- Tách scheduler policy khỏi Cortex-M3 assembly và PendSV.
- Tạo lỗi duplicate insertion hoặc bitmap sai có chủ đích rồi kiểm tra invariant.

## Build và chạy

Makefile của lab này chỉ quản lý source, test và executable của chính lab. Từ thư mục root:

```bash
cd labs/05-priority-scheduler-host
make test
make run
```

Có thể dùng:

```bash
make       # Build và chạy test
make test  # Chỉ build/chạy unit test
make run   # Build rồi chạy demo
make clean # Chỉ xóa build/ của lab này
```

## Kết quả

Artifact demo chính nằm cục bộ trong lab:

```text
build/lab05_priority_scheduler-demo
```

Chạy trực tiếp bằng `./build/lab05_priority_scheduler-demo` hoặc `make run`.

Demo in task được chọn trước rotation, sau rotation và sau khi task priority cao trở thành READY. Unit test chạy với ASan/UBSan.

## Câu hỏi

1. Vì sao scheduler policy nên được test độc lập với context-switch assembly?
2. Ready bitmap giúp giảm công việc tìm priority cao nhất như thế nào?
3. FIFO queue bảo đảm điều gì cho các task cùng priority?
4. Điều gì xảy ra nếu bitmap báo READY nhưng queue tương ứng lại rỗng?
