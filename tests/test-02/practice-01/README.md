# Lối đi ít khúc cua nhất trong mê cung

## Mô tả

Cho một mê cung kích thước `M x N` gồm các ô có giá trị `0` hoặc `1`.

Trong đó:

- Ô `0` là đường đi.
- Ô `1` là tường.
- Điểm bắt đầu là ô `(0, 0)`.
- Điểm kết thúc là ô `(M - 1, N - 1)`.

Tại mỗi bước, chỉ được di chuyển sang một ô kề cạnh theo một trong bốn hướng:

- Lên
- Xuống
- Trái
- Phải

Một **khúc cua** xuất hiện khi hai bước đi liên tiếp có hướng khác nhau.

Bước đầu tiên chỉ dùng để chọn hướng xuất phát nên không được tính là một khúc cua.

Hãy tìm số khúc cua ít nhất cần có để đi từ ô bắt đầu tới ô kết thúc.

Lưu ý rằng đường đi có ít khúc cua nhất không nhất thiết là đường đi có ít bước nhất.

## Yêu cầu

1. Chỉ được đi qua các ô có giá trị `0`.
2. Chỉ được di chuyển theo bốn hướng lên, xuống, trái hoặc phải.
3. Mỗi lần đổi hướng được tính là một khúc cua.
4. Bước đầu tiên không được tính là khúc cua.
5. Nếu không tồn tại đường đi hợp lệ, trả về `-1`.

## Dữ liệu vào

Dòng đầu tiên gồm hai số nguyên:

```text
M N
```

Trong đó:

- `8 <= M, N <= 30`
- `M` là số hàng của mê cung.
- `N` là số cột của mê cung.

`M` dòng tiếp theo, mỗi dòng gồm `N` số nguyên `0` hoặc `1`.

Hai ô sau luôn có giá trị `0`:

```text
(0, 0)
(M - 1, N - 1)
```

## Dữ liệu ra

In ra một số nguyên:

- Số khúc cua ít nhất trên một đường đi hợp lệ.
- Hoặc `-1` nếu không tồn tại đường đi từ điểm bắt đầu tới điểm kết thúc.

## Ví dụ

### Input

```text
8 8
0 0 0 0 0 0 0 0
1 1 1 1 1 1 1 0
1 1 1 1 1 1 1 0
1 1 1 1 1 1 1 0
1 1 1 1 1 1 1 0
1 1 1 1 1 1 1 0
1 1 1 1 1 1 1 0
1 1 1 1 1 1 1 0
```

### Output

```text
1
```

## Giải thích

1. Đi sang phải từ `(0, 0)` đến `(0, 7)`.
2. Đoạn đầu tiên chưa tạo ra khúc cua.
3. Tại `(0, 7)`, đổi hướng từ phải sang xuống nên có `1` khúc cua.
4. Tiếp tục đi xuống đến `(7, 7)`.
5. Không cần đổi hướng thêm nên kết quả là `1`.

## Kiến thức sử dụng

- Đồ thị trên lưới
- Breadth-First Search
- 0-1 BFS
- Dijkstra
- Trạng thái gồm vị trí và hướng di chuyển
- Tối ưu chi phí đường đi

## Gợi ý

Lưu trạng thái dưới dạng:

```text
(row, col, direction)
```

Trong đó:

- Đi tiếp cùng hướng có chi phí `0`.
- Đổi sang hướng khác có chi phí `1`.

Có thể giải bằng:

- 0-1 BFS
- Dijkstra

## Gợi ý cấu trúc thư mục

```text
minimum-turns-maze/
├── README.md
└── main.c
```
