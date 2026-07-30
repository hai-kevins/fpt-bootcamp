# Lab 06 - Table-Driven State Machine

## Mục tiêu

- Biểu diễn transition bằng bảng dữ liệu tĩnh.
- Dispatch chuỗi signal 1, 2 và 3.
- Xác nhận state đi qua 0 -> 1 -> 2 -> 0.
- Quan sát handled count của FSM.

## Build và test

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/06-table-driven-state-machine
make test
make run
```

Có thể dùng:

```bash
make          # Build executable
make test     # Build rồi chạy bài kiểm tra
make run      # Build rồi chạy demo/test
make sanitize # Chạy với ASan/UBSan
make clean    # Chỉ xóa build/ của lab này
```

## Kết quả

Artifact chính nằm cục bộ trong lab:

```text
build/lab
```

Output dự kiến: `final=0 handled=3`. Ba transition trong bảng đều được tìm thấy và state quay về giá trị ban đầu.

## Câu hỏi

1. Table-driven FSM dễ review và test hơn switch-case ở điểm nào?
2. Làm sao phát hiện duplicate transition trong bảng?
3. Tra cứu tuyến tính ảnh hưởng WCET thế nào khi bảng lớn?
4. Có thể sinh bảng transition từ tài liệu thiết kế bằng cách nào?
