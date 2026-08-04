# Xóa node bị chặn bởi giá trị lớn hơn

## Mô tả

Cho một danh sách liên kết đơn.

Với mỗi node, nếu ở phía bên phải node đó tồn tại một node khác có giá trị lớn hơn, node hiện tại phải bị xóa khỏi danh sách.

Hãy trả về danh sách còn lại sau khi xóa tất cả các node như vậy.

Thứ tự tương đối của các node được giữ lại phải giống như trong danh sách ban đầu.

Hệ thống sẽ:

1. Tự dựng Linked List từ dữ liệu vào.
2. Gọi hàm xử lý của thí sinh.
3. In danh sách kết quả.

## Yêu cầu

1. Xóa mọi node có ít nhất một node lớn hơn nằm ở phía bên phải.
2. Giữ nguyên thứ tự tương đối của các node còn lại.
3. Không làm thay đổi giá trị của các node được giữ lại.
4. Trả về head của danh sách sau khi xử lý.

## Dữ liệu vào

Dòng đầu tiên gồm số nguyên:

```text
n
```

Trong đó:

```text
1 <= n <= 100000
```

Dòng thứ hai gồm `n` số nguyên:

```text
nums[0] nums[1] ... nums[n - 1]
```

Trong đó:

```text
1 <= nums[i] <= 100000
```

Các giá trị biểu diễn nội dung của các node theo thứ tự từ trái sang phải.

## Dữ liệu ra

In giá trị của các node còn lại sau khi xử lý.

Các giá trị được cách nhau bằng một dấu cách.

## Ví dụ

### Input

```text
5
5 2 13 3 8
```

### Output

```text
13 8
```

## Giải thích

1. Node có giá trị `5` bị xóa vì bên phải có node `13` lớn hơn.
2. Node có giá trị `2` bị xóa vì bên phải có node `13` lớn hơn.
3. Node có giá trị `3` bị xóa vì bên phải có node `8` lớn hơn.
4. Node `13` không có node lớn hơn ở bên phải nên được giữ lại.
5. Node `8` không có node lớn hơn ở bên phải nên được giữ lại.

Danh sách kết quả là:

```text
13 8
```

## Kiến thức sử dụng

- Singly Linked List
- Con trỏ
- Đảo ngược danh sách liên kết
- Duyệt danh sách từ phải sang trái
- Giá trị lớn nhất hậu tố
- Monotonic Stack

## Gợi ý

Duyệt danh sách từ phải sang trái sẽ giúp biết giá trị lớn nhất đã xuất hiện ở phía bên phải.

Có thể sử dụng một trong hai cách:

### Cách 1: Đảo ngược Linked List

1. Đảo ngược danh sách.
2. Duyệt từ trái sang phải.
3. Giữ lại node nếu giá trị của node lớn hơn hoặc bằng giá trị lớn nhất đã gặp.
4. Đảo ngược danh sách thêm một lần nữa.

### Cách 2: Stack đơn điệu

Dùng stack để loại bỏ các node bị chặn bởi một giá trị lớn hơn ở bên phải.

## Gợi ý cấu trúc thư mục

```text
remove-nodes-with-greater-value-on-right/
├── README.md
└── main.c
```
