# Data Structures — Ring Buffer và ứng dụng

> Tài liệu học tập độc lập về **Ring Buffer**, dùng cho mục đích học tập và thực hành lập trình nhúng.

---

## Mục lục

1. [Ring Buffer là gì?](#1-ring-buffer-là-gì)
2. [Các cách hiện thực Ring Buffer](#2-các-cách-hiện-thực-ring-buffer)
3. [Các thành phần chính](#3-các-thành-phần-chính)
4. [Trạng thái của Ring Buffer](#4-trạng-thái-của-ring-buffer)
5. [Cơ chế quay vòng](#5-cơ-chế-quay-vòng)
6. [Minh họa quá trình thêm dữ liệu](#6-minh-họa-quá-trình-thêm-dữ-liệu)
7. [Minh họa quá trình lấy dữ liệu](#7-minh-họa-quá-trình-lấy-dữ-liệu)
8. [Khi Ring Buffer đầy](#8-khi-ring-buffer-đầy)
9. [Cài đặt Ring Buffer cho kiểu `char`](#9-cài-đặt-ring-buffer-cho-kiểu-char)
10. [Ví dụ sử dụng](#10-ví-dụ-sử-dụng)
11. [Ứng dụng với UART RX](#11-ứng-dụng-với-uart-rx)
12. [Ứng dụng tạo command line](#12-ứng-dụng-tạo-command-line)
13. [Ứng dụng lọc trung bình ADC](#13-ứng-dụng-lọc-trung-bình-adc)
14. [Chính sách khi đầy](#14-chính-sách-khi-đầy)
15. [Phân biệt `head == tail`](#15-phân-biệt-head--tail)
16. [Độ phức tạp](#16-độ-phức-tạp)
17. [Những lưu ý trong hệ thống nhúng](#17-những-lưu-ý-trong-hệ-thống-nhúng)
18. [Ưu điểm](#18-ưu-điểm)
19. [Hạn chế](#19-hạn-chế)
20. [Tổng kết](#20-tổng-kết)
21. [Tài liệu tham khảo](#tài-liệu-tham-khảo)

---

## 1. Ring Buffer là gì?

**Ring Buffer**, còn được gọi là **Circular Buffer** hay **bộ đệm vòng**, là một cấu trúc dữ liệu có kích thước cố định.

Dữ liệu thường được đọc theo nguyên tắc:

```text
FIFO — First In, First Out
```

Tuy nhiên, Ring Buffer khác hàng đợi FIFO thông thường ở chỗ: khi bộ đệm đã đầy, dữ liệu mới có thể ghi đè lên dữ liệu cũ nhất, tùy theo cách thiết kế.

Ring Buffer rất phổ biến trong lập trình nhúng vì:

- Không cần cấp phát bộ nhớ động trong lúc chương trình đang chạy.
- Thời gian thêm và lấy dữ liệu gần như cố định.
- Phù hợp với dữ liệu truyền nhận bất đồng bộ.
- Có thể sử dụng an toàn giữa ngắt và vòng lặp chính nếu xử lý đồng bộ đúng cách.
- Luôn giữ được một số lượng mẫu dữ liệu gần nhất.

Một số ví dụ sử dụng:

- Bộ đệm nhận dữ liệu UART.
- Bộ đệm truyền UART.
- Lưu các mẫu ADC gần nhất.
- Lọc trung bình trượt.
- Lưu sự kiện trong hệ thống event-driven.
- Trao đổi dữ liệu giữa interrupt và `main()`.
- Ghi log tạm thời.

---

## 2. Các cách hiện thực Ring Buffer

Ring Buffer có thể được xây dựng bằng:

1. **Mảng có kích thước cố định**
2. **Danh sách liên kết**

Trong hệ thống nhúng, cách dùng mảng thường được ưu tiên vì:

- Bộ nhớ được xác định trước.
- Không cần `malloc()` và `free()`.
- Dễ kiểm soát RAM.
- Tránh phân mảnh heap.
- Thời gian truy cập phần tử nhanh.

Tài liệu này tập trung vào Ring Buffer sử dụng mảng.

---

## 3. Các thành phần chính

Một Ring Buffer thường có các thành phần sau:

### `buffer`

Mảng dùng để lưu dữ liệu.

### `head`

Chỉ mục của phần tử tiếp theo sẽ được lấy ra khỏi bộ đệm.

```text
head → vị trí đọc dữ liệu
```

### `tail`

Chỉ mục của vị trí sẽ được dùng để ghi phần tử mới.

```text
tail → vị trí ghi dữ liệu
```

### `count`

Số phần tử hiện đang tồn tại trong Ring Buffer.

### `capacity`

Số phần tử tối đa Ring Buffer có thể chứa.

---

## 4. Trạng thái của Ring Buffer

### Buffer rỗng

```c
count == 0
```

Khi buffer rỗng, không thể lấy thêm dữ liệu.

### Buffer đầy

```c
count == capacity
```

Khi buffer đầy, chương trình phải lựa chọn một trong hai chính sách:

1. Từ chối dữ liệu mới.
2. Ghi đè dữ liệu cũ nhất.

---

## 5. Cơ chế quay vòng

Giả sử Ring Buffer có 5 phần tử:

```text
Index:   0   1   2   3   4
```

Ban đầu:

```text
head = 0
tail = 0
count = 0
```

Khi thêm một phần tử:

```text
buffer[tail] = data
tail = tail + 1
```

Khi `tail` đi đến cuối mảng, nó phải quay lại vị trí đầu tiên:

```c
tail = (tail + 1U) % capacity;
```

Tương tự đối với `head`:

```c
head = (head + 1U) % capacity;
```

Ví dụ:

```text
tail = 4
capacity = 5

tail mới = (4 + 1) % 5 = 0
```

Nhờ phép modulo, chỉ mục luôn nằm trong phạm vi của mảng.

---

## 6. Minh họa quá trình thêm dữ liệu

Ring Buffer có kích thước 5:

```text
[ _ ][ _ ][ _ ][ _ ][ _ ]
  H
  T
```

Thêm giá trị `7`:

```text
[ 7 ][ _ ][ _ ][ _ ][ _ ]
  H    T
```

Thêm tiếp `3` và `9`:

```text
[ 7 ][ 3 ][ 9 ][ _ ][ _ ]
  H              T
```

Lúc này:

```text
head  = 0
tail  = 3
count = 3
```

---

## 7. Minh họa quá trình lấy dữ liệu

Khi lấy một phần tử:

```text
Giá trị lấy ra: 7
```

Sau đó `head` chuyển sang vị trí tiếp theo:

```text
[ 7 ][ 3 ][ 9 ][ _ ][ _ ]
       H         T
```

Phần tử tại chỉ mục cũ có thể vẫn còn trong RAM, nhưng nó không còn được xem là dữ liệu hợp lệ vì đã nằm ngoài vùng logic của Ring Buffer.

---

## 8. Khi Ring Buffer đầy

Giả sử bộ đệm hiện đang đầy:

```text
[ 7 ][ 3 ][ 9 ][ 5 ][ 2 ]
  H
  T
```

Nếu áp dụng chính sách ghi đè và thêm giá trị `8`, phần tử cũ nhất sẽ bị thay thế:

```text
[ 8 ][ 3 ][ 9 ][ 5 ][ 2 ]
       H
       T
```

Khi ghi đè:

- `tail` di chuyển đến vị trí tiếp theo.
- `head` cũng phải di chuyển vì dữ liệu cũ nhất đã bị loại bỏ.
- `count` vẫn bằng `capacity`.

---

## 9. Cài đặt Ring Buffer cho kiểu `char`

### 9.1. File `ring_buffer.h`

```c
#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    char *data;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
} RingBuffer;

bool RingBuffer_Init(RingBuffer *rb, char *storage, size_t capacity);

bool RingBuffer_Put(RingBuffer *rb, char value);

bool RingBuffer_PutOverwrite(RingBuffer *rb, char value);

bool RingBuffer_Get(RingBuffer *rb, char *value);

bool RingBuffer_Peek(const RingBuffer *rb, char *value);

bool RingBuffer_IsEmpty(const RingBuffer *rb);

bool RingBuffer_IsFull(const RingBuffer *rb);

size_t RingBuffer_Size(const RingBuffer *rb);

size_t RingBuffer_Capacity(const RingBuffer *rb);

void RingBuffer_Clear(RingBuffer *rb);

#endif
```

---

### 9.2. File `ring_buffer.c`

```c
#include "ring_buffer.h"

bool RingBuffer_Init(RingBuffer *rb, char *storage, size_t capacity)
{
    if ((rb == NULL) || (storage == NULL) || (capacity == 0U))
    {
        return false;
    }

    rb->data = storage;
    rb->capacity = capacity;
    rb->head = 0U;
    rb->tail = 0U;
    rb->count = 0U;

    return true;
}

bool RingBuffer_IsEmpty(const RingBuffer *rb)
{
    if (rb == NULL)
    {
        return true;
    }

    return rb->count == 0U;
}

bool RingBuffer_IsFull(const RingBuffer *rb)
{
    if (rb == NULL)
    {
        return false;
    }

    return rb->count == rb->capacity;
}

size_t RingBuffer_Size(const RingBuffer *rb)
{
    if (rb == NULL)
    {
        return 0U;
    }

    return rb->count;
}

size_t RingBuffer_Capacity(const RingBuffer *rb)
{
    if (rb == NULL)
    {
        return 0U;
    }

    return rb->capacity;
}

void RingBuffer_Clear(RingBuffer *rb)
{
    if (rb == NULL)
    {
        return;
    }

    rb->head = 0U;
    rb->tail = 0U;
    rb->count = 0U;
}

bool RingBuffer_Put(RingBuffer *rb, char value)
{
    if ((rb == NULL) || (rb->data == NULL))
    {
        return false;
    }

    if (RingBuffer_IsFull(rb))
    {
        return false;
    }

    rb->data[rb->tail] = value;
    rb->tail = (rb->tail + 1U) % rb->capacity;
    rb->count++;

    return true;
}

bool RingBuffer_PutOverwrite(RingBuffer *rb, char value)
{
    if ((rb == NULL) || (rb->data == NULL) || (rb->capacity == 0U))
    {
        return false;
    }

    rb->data[rb->tail] = value;
    rb->tail = (rb->tail + 1U) % rb->capacity;

    if (rb->count == rb->capacity)
    {
        rb->head = (rb->head + 1U) % rb->capacity;
    }
    else
    {
        rb->count++;
    }

    return true;
}

bool RingBuffer_Get(RingBuffer *rb, char *value)
{
    if ((rb == NULL) || (value == NULL) || (rb->data == NULL))
    {
        return false;
    }

    if (RingBuffer_IsEmpty(rb))
    {
        return false;
    }

    *value = rb->data[rb->head];
    rb->head = (rb->head + 1U) % rb->capacity;
    rb->count--;

    return true;
}

bool RingBuffer_Peek(const RingBuffer *rb, char *value)
{
    if ((rb == NULL) || (value == NULL) || (rb->data == NULL))
    {
        return false;
    }

    if (RingBuffer_IsEmpty(rb))
    {
        return false;
    }

    *value = rb->data[rb->head];

    return true;
}
```

---

## 10. Ví dụ sử dụng

```c
#include <stdio.h>

#include "ring_buffer.h"

#define BUFFER_SIZE 8U

int main(void)
{
    RingBuffer rb;
    char storage[BUFFER_SIZE];
    char value;

    if (!RingBuffer_Init(&rb, storage, BUFFER_SIZE))
    {
        return 1;
    }

    RingBuffer_Put(&rb, 'A');
    RingBuffer_Put(&rb, 'B');
    RingBuffer_Put(&rb, 'C');

    while (RingBuffer_Get(&rb, &value))
    {
        printf("%c\n", value);
    }

    return 0;
}
```

Kết quả:

```text
A
B
C
```

---

## 11. Ứng dụng với UART RX

Một ứng dụng phổ biến là nhận dữ liệu UART trong interrupt và xử lý dữ liệu trong vòng lặp chính.

### Nguyên tắc

```text
UART RX Interrupt
        |
        v
Đọc byte từ thanh ghi UART
        |
        v
Đưa byte vào Ring Buffer
        |
        v
Thoát khỏi interrupt
        |
        v
main() lấy dữ liệu và xử lý
```

Interrupt chỉ nên thực hiện công việc ngắn:

1. Đọc byte vừa nhận.
2. Đưa byte vào Ring Buffer.
3. Xóa cờ ngắt nếu phần cứng yêu cầu.
4. Thoát khỏi hàm xử lý ngắt.

Các thao tác phân tích command, in log hoặc xử lý chuỗi nên được thực hiện bên ngoài interrupt.

### Ví dụ khái quát

```c
#define UART_RX_BUFFER_SIZE 128U

static RingBuffer uart_rx_buffer;
static char uart_rx_storage[UART_RX_BUFFER_SIZE];

void App_Init(void)
{
    RingBuffer_Init(
        &uart_rx_buffer,
        uart_rx_storage,
        UART_RX_BUFFER_SIZE
    );
}

void UART_RX_IRQHandler(void)
{
    char received_byte;

    received_byte = UART_ReadByte();

    RingBuffer_PutOverwrite(
        &uart_rx_buffer,
        received_byte
    );
}

void App_Process(void)
{
    char byte;

    while (RingBuffer_Get(&uart_rx_buffer, &byte))
    {
        CommandLine_ProcessByte(byte);
    }
}
```

> `UART_ReadByte()` và `CommandLine_ProcessByte()` chỉ là các hàm minh họa. Cách hiện thực phụ thuộc vào vi điều khiển và thư viện phần cứng đang sử dụng.

---

## 12. Ứng dụng tạo command line

Ring Buffer có thể dùng để lưu từng ký tự nhận từ UART.

Ví dụ người dùng gửi:

```text
led on\r\n
```

Các byte được đưa lần lượt vào Ring Buffer:

```text
'l' → 'e' → 'd' → ' ' → 'o' → 'n' → '\r' → '\n'
```

Vòng lặp chính lấy từng byte ra và ghép thành một dòng lệnh.

```c
#define COMMAND_MAX_LENGTH 32U

static char command[COMMAND_MAX_LENGTH];
static size_t command_length = 0U;

void CommandLine_ProcessByte(char byte)
{
    if ((byte == '\r') || (byte == '\n'))
    {
        if (command_length > 0U)
        {
            command[command_length] = '\0';

            CommandLine_Execute(command);

            command_length = 0U;
        }

        return;
    }

    if (command_length < (COMMAND_MAX_LENGTH - 1U))
    {
        command[command_length] = byte;
        command_length++;
    }
    else
    {
        command_length = 0U;
    }
}
```

---

## 13. Ứng dụng lọc trung bình ADC

Ring Buffer có thể lưu `N` mẫu ADC mới nhất.

Ví dụ cần lấy trung bình của 10 mẫu gần nhất:

```text
ADC sample mới
      |
      v
Ghi vào Ring Buffer
      |
      v
Nếu buffer đầy, ghi đè mẫu cũ nhất
      |
      v
Tính trung bình các mẫu đang có
```

Với vi điều khiển, có thể xây dựng Ring Buffer cho kiểu `uint16_t` thay vì `char`.

```c
#define ADC_SAMPLE_COUNT 10U

typedef struct
{
    uint16_t data[ADC_SAMPLE_COUNT];
    size_t head;
    size_t tail;
    size_t count;
} AdcRingBuffer;
```

Giá trị trung bình:

```c
uint32_t sum = 0U;

for (size_t i = 0U; i < rb.count; i++)
{
    size_t index = (rb.head + i) % ADC_SAMPLE_COUNT;
    sum += rb.data[index];
}

uint16_t average = (uint16_t)(sum / rb.count);
```

Cần kiểm tra `rb.count > 0` trước khi thực hiện phép chia.

---

## 14. Chính sách khi đầy

### Cách 1: Không ghi thêm

```c
if (RingBuffer_IsFull(rb))
{
    return false;
}
```

Phù hợp khi không được phép làm mất dữ liệu cũ.

Ví dụ:

- Gói dữ liệu giao tiếp quan trọng.
- Message queue cần đảm bảo thứ tự.
- Lệnh điều khiển không được phép bỏ qua.

### Cách 2: Ghi đè dữ liệu cũ nhất

```c
if (rb->count == rb->capacity)
{
    rb->head = (rb->head + 1U) % rb->capacity;
}
```

Phù hợp khi dữ liệu mới quan trọng hơn dữ liệu cũ.

Ví dụ:

- Dữ liệu cảm biến liên tục.
- Log gần nhất.
- Mẫu ADC mới nhất.
- Luồng dữ liệu thời gian thực.

---

## 15. Phân biệt `head == tail`

Nếu Ring Buffer chỉ có `head` và `tail`, trạng thái:

```c
head == tail
```

có thể mang hai ý nghĩa:

- Buffer đang rỗng.
- Buffer đang đầy.

Có ba cách phổ biến để giải quyết:

### Cách 1: Dùng biến `count`

```c
empty: count == 0
full:  count == capacity
```

Ưu điểm: dễ hiểu.

### Cách 2: Hy sinh một phần tử

```c
empty: head == tail

full:
(tail + 1) % capacity == head
```

Nếu mảng có `N` phần tử thì chỉ lưu tối đa `N - 1` phần tử.

### Cách 3: Dùng cờ `full`

```c
bool full;
```

Cờ này cho biết `head == tail` đang biểu diễn trạng thái đầy hay rỗng.

---

## 16. Độ phức tạp

| Thao tác | Độ phức tạp |
|---|---:|
| Thêm phần tử | `O(1)` |
| Lấy phần tử | `O(1)` |
| Xem phần tử đầu | `O(1)` |
| Kiểm tra rỗng | `O(1)` |
| Kiểm tra đầy | `O(1)` |
| Tính số phần tử khi có `count` | `O(1)` |
| Duyệt toàn bộ dữ liệu | `O(n)` |

---

## 17. Những lưu ý trong hệ thống nhúng

### 17.1. Truy cập từ interrupt và `main()`

Nếu interrupt ghi dữ liệu còn `main()` đọc dữ liệu, các biến dùng chung phải được xử lý cẩn thận.

Tùy kiến trúc MCU và kiểu dữ liệu, có thể cần:

- Khai báo biến dùng chung bằng `volatile`.
- Tạm khóa interrupt khi cập nhật nhiều biến liên quan.
- Dùng thao tác nguyên tử.
- Dùng cơ chế đồng bộ của RTOS.
- Thiết kế Single Producer – Single Consumer.

Không nên mặc định rằng thêm `volatile` sẽ tự động làm cấu trúc dữ liệu thread-safe.

### 17.2. Kích thước buffer

Buffer quá nhỏ có thể làm mất dữ liệu khi tốc độ ghi lớn hơn tốc độ xử lý.

Buffer quá lớn làm lãng phí RAM.

Kích thước nên được chọn dựa trên:

```text
Tốc độ dữ liệu đầu vào
×
Thời gian xử lý tồi nhất
```

Ví dụ, UART nhận khoảng 1000 byte mỗi giây và chương trình có thể bị bận tối đa 50 ms:

```text
1000 × 0.05 = 50 byte
```

Khi đó nên chọn buffer lớn hơn 50 byte để có vùng an toàn.

### 17.3. Dùng kích thước là lũy thừa của 2

Nếu `capacity` là lũy thừa của 2, có thể thay phép modulo bằng phép AND:

```c
index = (index + 1U) & (capacity - 1U);
```

Ví dụ:

```text
capacity = 128
mask     = 127
```

Cách này chỉ đúng khi:

```text
capacity = 2^n
```

Trên nhiều vi điều khiển hiện đại, phép modulo có thể đã đủ nhanh. Chỉ nên tối ưu khi đo đạc cho thấy đây là điểm nghẽn.

---

## 18. Ưu điểm

- Cấu trúc đơn giản.
- Thêm và lấy dữ liệu nhanh.
- Không phải di chuyển toàn bộ mảng.
- Bộ nhớ được xác định trước.
- Phù hợp với firmware bare-metal.
- Hữu ích trong giao tiếp bất đồng bộ.
- Thích hợp cho dữ liệu dạng luồng.
- Có thể giữ lại các mẫu dữ liệu mới nhất.

---

## 19. Hạn chế

- Kích thước thường cố định.
- Có thể mất dữ liệu nếu buffer đầy.
- Cần quy định rõ chính sách ghi đè.
- Cần đồng bộ khi nhiều ngữ cảnh cùng truy cập.
- Khó debug nếu quản lý `head`, `tail` và `count` không chính xác.
- Không phù hợp khi cần chèn hoặc xóa dữ liệu ở giữa.

---

## 20. Tổng kết

Ring Buffer là một cấu trúc dữ liệu quan trọng trong lập trình nhúng.

Các ý chính cần nhớ:

```text
head  → vị trí đọc
tail  → vị trí ghi
count → số phần tử hiện có
```

Chỉ mục được quay vòng bằng:

```c
index = (index + 1U) % capacity;
```

Ring Buffer đặc biệt phù hợp với:

- UART RX/TX.
- Dữ liệu cảm biến.
- ADC sampling.
- Event queue.
- Command line.
- Log buffer.
- Hệ thống producer–consumer.

Khi triển khai trong firmware thực tế, cần xác định rõ:

1. Buffer đầy thì từ chối hay ghi đè.
2. Có truy cập từ interrupt hay không.
3. Kích thước buffer bao nhiêu là phù hợp.
4. Kiểu dữ liệu nào sẽ được lưu.
5. Có cần thread-safe hoặc lock-free hay không.

---

## Tài liệu tham khảo

README này được biên soạn và diễn đạt lại độc lập dựa trên phạm vi chủ đề được trình bày trong:

- EPCB — **Data Structures - Ring buffer và ứng dụng**:  
  https://epcb.vn/blogs/ak-embedded-software/data-structures-ring-buffer-va-ung-dung

