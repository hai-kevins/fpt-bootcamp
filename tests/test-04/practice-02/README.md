# Kiểm tra và tính biểu thức

## Mô tả

Cho một chuỗi biểu thức toán học.

Hãy kiểm tra biểu thức có hợp lệ hay không.

Nếu biểu thức hợp lệ, hãy tính giá trị của biểu thức rồi làm tròn về số nguyên gần nhất.

Biểu thức có thể gồm:

- Số không âm ở dạng số nguyên.
- Số không âm ở dạng số thập phân.
- Bốn toán tử nhị phân:
  - `+`
  - `-`
  - `*`
  - `/`
- Ngoặc tròn:
  - `(`
  - `)`
- Khoảng trắng tùy ý giữa các thành phần.

Một số thập phân hợp lệ phải có chữ số ở cả hai bên dấu chấm.

Ví dụ:

- `99.9` là hợp lệ.
- `0.25` là hợp lệ.
- `.5` là không hợp lệ.
- `5.` là không hợp lệ.

## Yêu cầu

1. Kiểm tra toàn bộ cú pháp của biểu thức.
2. Tính toán theo đúng thứ tự ưu tiên:
   - Ngoặc trước.
   - Sau đó đến `*` và `/`.
   - Cuối cùng là `+` và `-`.
3. Các toán tử cùng mức ưu tiên được xử lý từ trái sang phải.
4. Dấu `-` chỉ được dùng như toán tử nhị phân.
5. Không hỗ trợ số âm trực tiếp như:

   ```text
   -5
   1 + -2
   ```

6. Kết quả cuối cùng vẫn có thể là số âm.
7. Chia cho biểu thức có giá trị đúng bằng `0` là không hợp lệ.
8. Nếu hợp lệ, làm tròn kết quả tới số nguyên gần nhất.
9. Nếu không hợp lệ, in:

   ```text
   false 0
   ```

## Quy tắc làm tròn

Với kết quả không âm:

- Phần lẻ `>= 0.5` thì làm tròn lên.
- Phần lẻ `< 0.5` thì làm tròn xuống.

Ví dụ:

```text
1.5 → 2
1.4 → 1
```

Với kết quả âm, áp dụng cùng quy tắc theo độ lớn tuyệt đối.

Ví dụ:

```text
-1.5 → -2
-1.4 → -1
```

## Biểu thức không hợp lệ

Một biểu thức được xem là không hợp lệ nếu thuộc một trong các trường hợp:

- Chuỗi rỗng.
- Có ký tự lạ.
- Sai định dạng số.
- Thiếu toán hạng.
- Thiếu toán tử.
- Ngoặc không khớp.
- Chia cho `0`.

## Dữ liệu vào

Dòng đầu tiên chứa chuỗi:

```text
expression
```

Ràng buộc:

- `1 <= độ dài expression <= 1000`
- Phần nguyên của mỗi số không vượt quá `1000000`.
- Mỗi số có tối đa `6` chữ số sau dấu chấm.
- Các test hợp lệ được đảm bảo có kết quả sau khi làm tròn nằm trong phạm vi kiểu `long long`.

## Dữ liệu ra

Nếu biểu thức hợp lệ, in:

```text
true <result>
```

Trong đó `result` là giá trị sau khi làm tròn.

Nếu biểu thức không hợp lệ, in:

```text
false 0
```

## Ví dụ

### Input

```text
99.9 + 112 * (8 - 2)
```

### Output

```text
true 772
```

## Giải thích

1. Biểu thức có ngoặc hợp lệ.
2. Các toán tử xuất hiện đúng vị trí.
3. Phần trong ngoặc:

   ```text
   8 - 2 = 6
   ```

4. Biểu thức trở thành:

   ```text
   99.9 + 112 * 6
   ```

5. Giá trị thật:

   ```text
   99.9 + 672 = 771.9
   ```

6. Phần lẻ là `0.9 >= 0.5` nên làm tròn lên thành `772`.

## Kiến thức sử dụng

- Phân tích cú pháp biểu thức
- Recursive Descent Parser
- Độ ưu tiên toán tử
- Xử lý ngoặc
- Số hữu tỉ
- Làm tròn số
- Phát hiện lỗi cú pháp

## Gợi ý

Có thể xây dựng Recursive Descent Parser với ba mức:

```text
expression → xử lý + và -
term       → xử lý * và /
factor     → xử lý số hoặc biểu thức trong ngoặc
```

Để tránh sai số khi xử lý số thập phân và phép chia, có thể lưu giá trị dưới dạng phân số:

```text
a / b
```

## Gợi ý cấu trúc thư mục

```text
validate-and-evaluate-expression/
├── README.md
└── main.c
```
