# Lab 09 - Router và Direct Post

## Mục tiêu

- Đăng ký Active Object vào local router.
- Allocate Dynamic Event có destination object.
- Route event local vào đúng mailbox.
- Dispatch event và xác nhận ownership được release.
- Phân biệt local route, unknown destination và remote route.

## Build và chạy

Makefile của lab này chỉ quản lý source và executable của chính lab. Từ thư mục root:

```bash
cd labs/09-router-direct-post
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
local=1 hit=1
```

Local route counter tăng, handler nhận đúng một event và Event Pool trở về `used_count == 0`.

Chương trình trả exit code `0` khi toàn bộ điều kiện kiểm tra của lab đều đúng.

## Câu hỏi

1. Router cần dùng node ID và object ID để quyết định route như thế nào?
2. Unknown destination nên tăng counter hay silently drop? Vì sao?
3. Ownership của event thay đổi thế nào khi `router_post()` thành công?
4. Remote route cần thêm transport callback và failure policy nào?
