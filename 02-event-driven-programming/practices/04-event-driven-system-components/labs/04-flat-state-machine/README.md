# Lab 04 - Flat State Machine

## Mục tiêu

- Mô tả transition bằng current state, signal, next state và action.
- Dispatch hai event để đi từ state 0 sang 1 rồi trở về 0.
- Xác nhận action được gọi đúng số lần.
- Quan sát handled count và state cuối.
- Xử lý trường hợp signal không có transition.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/04-flat-state-machine
make
make run
```

Có thể dùng:

```bash
make          # Build executable
make run      # Build rồi chạy lab
make test     # Alias của make run
make sanitize # Chạy với ASan/UBSan
make clean    # Chỉ xóa build/ của lab hiện tại
```

## Kết quả

Artifact chính nằm cục bộ trong lab:

```text
build/lab
```

Output dự kiến:

```text
state=0 actions=2
```

Hai transition hợp lệ được thực hiện và cả hai action được gọi.

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đều đúng.

## Câu hỏi

1. Flat FSM phù hợp khi số state và transition ở mức nào?
2. Điều gì xảy ra khi không tìm thấy transition cho current state và signal?
3. Action nên thực hiện trước hay sau khi cập nhật state? Có trade-off gì?
4. Làm sao tạo ma trận coverage state-signal cho FSM này?
