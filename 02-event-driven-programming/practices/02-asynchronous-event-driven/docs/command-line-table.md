# Command-line table pattern

## Mục tiêu

Module này giữ mô hình của AK command line:

```c
static const cmd_line_t command_table[] =
{
    {
        "command",
        command_handler,
        "command <argument> - description"
    },
    {
        NULL,
        NULL,
        NULL
    }
};
```

Luồng xử lý:

```text
UART line
    -> cmd_line_parser()
    -> tách command và arguments
    -> tra cứu cmd_line_t
    -> gọi function pointer
    -> trả cmd_line_status_t
```

## Các nâng cấp an toàn

- Kiểm tra table và input `NULL`.
- Luôn kết thúc command bằng `\0`, kể cả input không có newline.
- Không sửa chuỗi đầu vào.
- Bỏ khoảng trắng đầu dòng và trim arguments.
- Giới hạn command, arguments và số entry được duyệt.
- Callback trả status thật thay vì parser luôn báo thành công.
- Có helper so sánh argument và parse `uint32_t` với kiểm tra overflow.
- `info` trong table được dùng để sinh nội dung `help`.

## Quy ước handler

Handler nhận:

```c
cmd_line_status_t handler(
    const char *arguments,
    void *context
);
```

- `arguments` luôn là chuỗi hợp lệ và đã trim hai đầu.
- `context` dùng cho dữ liệu theo lần gọi, ví dụ timestamp của event.
- Không có argument thì `arguments[0] == '\0'`.
- Handler trả `CMD_INVALID_ARG` khi cú pháp argument không hợp lệ.
- Handler trả `CMD_EXECUTION_ERROR` nếu dispatch/post message thất bại.
