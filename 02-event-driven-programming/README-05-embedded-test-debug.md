# Chủ đề 5 - Xây dựng hệ thống kiểm thử và gỡ lỗi cho Embedded Programming
## Test, Debug, Trace và Automated Test trong hệ thống Event-Driven

> Mục tiêu của chủ đề này là xây dựng một hệ thống kiểm thử và gỡ lỗi có thể sử dụng trong suốt vòng đời của firmware, từ phát triển, tích hợp phần cứng đến vận hành thực tế.
>
> Nội dung trọng tâm gồm Unit Test và UART Command Line, Event Log chi phí thấp, Trace/Record Event, Crash Record và tự động hóa kiểm thử use case.

---

## Mục lục

1. [Mục tiêu học tập](#1-mục-tiêu-học-tập)
2. [Tại sao Embedded cần một hệ thống Test/Debug riêng](#2-tại-sao-embedded-cần-một-hệ-thống-testdebug-riêng)
3. [Các cấp độ kiểm thử firmware](#3-các-cấp-độ-kiểm-thử-firmware)
4. [Kiến trúc Test/Debug tổng thể](#4-kiến-trúc-testdebug-tổng-thể)
5. [Design for Testability](#5-design-for-testability)
6. [Tách Application, Driver và Platform](#6-tách-application-driver-và-platform)
7. [Dependency Injection trong C](#7-dependency-injection-trong-c)
8. [Test Double: Stub, Fake, Mock và Spy](#8-test-double-stub-fake-mock-và-spy)
9. [Unit Test trên host PC](#9-unit-test-trên-host-pc)
10. [Unit Test trên target MCU](#10-unit-test-trên-target-mcu)
11. [Command Line qua UART](#11-command-line-qua-uart)
12. [Kiến trúc Shell Task trong Event-Driven](#12-kiến-trúc-shell-task-trong-event-driven)
13. [Thiết kế Command Table](#13-thiết-kế-command-table)
14. [Các command kiểm thử cần có](#14-các-command-kiểm-thử-cần-có)
15. [Ví dụ Shell điều khiển LED](#15-ví-dụ-shell-điều-khiển-led)
16. [Ví dụ Shell phát event giả lập](#16-ví-dụ-shell-phát-event-giả-lập)
17. [Logging trong Embedded](#17-logging-trong-embedded)
18. [Các mức log](#18-các-mức-log)
19. [Text Log và Binary Log](#19-text-log-và-binary-log)
20. [Event Trace có chi phí thấp](#20-event-trace-có-chi-phí-thấp)
21. [Thiết kế Event Record](#21-thiết-kế-event-record)
22. [Ring Buffer cho Event Log](#22-ring-buffer-cho-event-log)
23. [Trace điểm nào trong hệ thống](#23-trace-điểm-nào-trong-hệ-thống)
24. [Đo Queue Latency và Handler Execution Time](#24-đo-queue-latency-và-handler-execution-time)
25. [Theo dõi Mailbox và Event Pool](#25-theo-dõi-mailbox-và-event-pool)
26. [Record Event khi thiết bị đã triển khai](#26-record-event-khi-thiết-bị-đã-triển-khai)
27. [Crash Record và Reset Reason](#27-crash-record-và-reset-reason)
28. [HardFault Debug](#28-hardfault-debug)
29. [Watchdog và Last-Gasp Record](#29-watchdog-và-last-gasp-record)
30. [Trace trên Flash ngoài hoặc vùng nhớ giữ lại](#30-trace-trên-flash-ngoài-hoặc-vùng-nhớ-giữ-lại)
31. [Automated Test trong Embedded](#31-automated-test-trong-embedded)
32. [Kiến trúc Test Runner](#32-kiến-trúc-test-runner)
33. [Mô tả Test Case](#33-mô-tả-test-case)
34. [Tự động kiểm thử use case sản phẩm](#34-tự-động-kiểm-thử-use-case-sản-phẩm)
35. [Fake Time và kiểm thử Timer](#35-fake-time-và-kiểm-thử-timer)
36. [Kiểm thử State Machine](#36-kiểm-thử-state-machine)
37. [Kiểm thử Event Queue và Event Pool](#37-kiểm-thử-event-queue-và-event-pool)
38. [Kiểm thử Data-Link Layer](#38-kiểm-thử-data-link-layer)
39. [Fault Injection](#39-fault-injection)
40. [Stress Test và Soak Test](#40-stress-test-và-soak-test)
41. [Hardware-in-the-Loop](#41-hardware-in-the-loop)
42. [Regression Test](#42-regression-test)
43. [Continuous Integration cho firmware](#43-continuous-integration-cho-firmware)
44. [Coverage và các giới hạn](#44-coverage-và-các-giới-hạn)
45. [Test Report](#45-test-report)
46. [Debug Workflow thực tế](#46-debug-workflow-thực-tế)
47. [Best Practices](#47-best-practices)
48. [Anti-pattern và lỗi thường gặp](#48-anti-pattern-và-lỗi-thường-gặp)
49. [Ví dụ framework Test/Debug tối thiểu](#49-ví-dụ-framework-testdebug-tối-thiểu)
50. [Bài thực hành](#50-bài-thực-hành)
51. [Tiêu chí hoàn thành](#51-tiêu-chí-hoàn-thành)
52. [Cấu trúc repository đề xuất](#52-cấu-trúc-repository-đề-xuất)
53. [Tài liệu tham khảo](#53-tài-liệu-tham-khảo)
54. [Tổng kết](#54-tổng-kết)

---

# 1. Mục tiêu học tập

Sau khi hoàn thành chủ đề này, người học cần có khả năng:

- Giải thích được sự khác nhau giữa:
  - Unit Test.
  - Integration Test.
  - System Test.
  - Hardware-in-the-Loop Test.
  - Regression Test.
  - Stress Test.
  - Soak Test.
- Thiết kế firmware có khả năng kiểm thử từ đầu.
- Tách logic nghiệp vụ khỏi phần cứng.
- Dùng Stub, Fake, Mock và Spy đúng mục đích.
- Viết Unit Test cho:
  - Event Queue.
  - Event Pool.
  - Timer.
  - State Machine.
  - Event Handler.
  - Data-Link Parser.
- Tạo Command Line qua UART để:
  - Đọc trạng thái.
  - Phát event.
  - Điều khiển module.
  - Chạy test case.
  - Dump trace.
- Thiết kế hệ thống log có thể bật/tắt theo module và mức độ.
- Ghi event trace dưới dạng binary record để tiết kiệm bộ nhớ.
- Theo dõi:
  - Queue depth.
  - Event Pool usage.
  - Timer usage.
  - Handler execution time.
  - Event latency.
- Lưu lại các event cuối cùng trước khi crash hoặc watchdog reset.
- Thu thập:
  - Reset reason.
  - Fatal code.
  - Current task.
  - Current signal.
  - CPU register.
- Xây dựng Test Runner tự động.
- Mô phỏng thao tác người dùng bằng event.
- Tự động đánh giá expected result.
- Tích hợp build và test vào CI.
- Tạo test report có thể lưu trên GitHub.

---

# 2. Tại sao Embedded cần một hệ thống Test/Debug riêng

Firmware không chạy trong môi trường đơn giản như một chương trình console thông thường.

Một lỗi có thể liên quan tới:

- Thời gian.
- Ngắt.
- Race condition.
- Peripheral.
- DMA.
- Nguồn điện.
- Reset.
- Watchdog.
- Link truyền thông.
- Memory corruption.
- Event bị mất.
- Timer cũ.
- State Machine sai.
- Queue đầy.
- Event Pool cạn.

Ví dụ một lỗi chỉ xuất hiện sau 12 giờ:

```text
Timer periodic
    |
    v
Event được post nhanh hơn tốc độ xử lý
    |
    v
Mailbox tăng dần
    |
    v
Event Pool cạn
    |
    v
Message quan trọng không được tạo
    |
    v
State Machine đứng
```

Nếu chỉ có `printf()` rải rác, rất khó xác định nguyên nhân.

Một hệ thống Test/Debug tốt cần trả lời được:

```text
Điều gì đã xảy ra?
Xảy ra lúc nào?
Event đến từ đâu?
Event được gửi tới đâu?
State trước và sau là gì?
Queue lúc đó sâu bao nhiêu?
Pool còn bao nhiêu block?
Task nào đang chạy?
Thiết bị reset vì nguyên nhân nào?
```

---

# 3. Các cấp độ kiểm thử firmware

## 3.1 Unit Test

Kiểm thử một đơn vị nhỏ:

- Hàm.
- Module.
- State Machine.
- Queue.
- Parser.
- Timer logic.

Ví dụ:

```text
Input:
    state = IDLE
    event = START_SIG

Expected:
    next state = RUNNING
    START_MOTOR event được phát
```

## 3.2 Integration Test

Kiểm thử nhiều module cùng hoạt động:

```text
Button AO
    |
    v
Application AO
    |
    v
Display AO
```

Mục tiêu:

- Event được route đúng.
- Payload đúng.
- Timer đúng.
- Ownership đúng.
- Không overflow.

## 3.3 System Test

Kiểm thử toàn bộ sản phẩm:

- MCU.
- Driver.
- Sensor.
- Display.
- Button.
- Communication.
- Firmware.

## 3.4 Hardware-in-the-Loop

Một máy test điều khiển phần cứng thật:

```text
Test PC
   |
   +--> UART command
   +--> GPIO stimulus
   +--> Power control
   +--> CAN traffic
   |
   v
Device Under Test
```

## 3.5 Regression Test

Chạy lại các test cũ sau mỗi thay đổi để chắc chắn không phá chức năng đã đúng.

## 3.6 Stress Test

Đẩy hệ thống tới tải cao:

- Event burst.
- UART liên tục.
- CAN traffic lớn.
- Button spam.
- Timer dày.
- Pool gần cạn.

## 3.7 Soak Test

Chạy trong thời gian dài:

```text
24 giờ
72 giờ
7 ngày
```

Mục tiêu:

- Memory leak.
- Counter overflow.
- Timer drift.
- Rare race condition.
- Queue tăng dần.
- Watchdog reset.

---

# 4. Kiến trúc Test/Debug tổng thể

```text
+--------------------------------------------------+
|                 Application                      |
| Active Objects / State Machines / Event Handlers |
+-----------------------+--------------------------+
                        |
                        v
+--------------------------------------------------+
|              Test & Debug Services               |
|                                                  |
|  +------------+ +------------+ +--------------+  |
|  | Shell UART | | Event Trace| | Test Runner  |  |
|  +------------+ +------------+ +--------------+  |
|                                                  |
|  +------------+ +------------+ +--------------+  |
|  | Assertions | | Crash Log  | | Statistics   |  |
|  +------------+ +------------+ +--------------+  |
+-----------------------+--------------------------+
                        |
                        v
+--------------------------------------------------+
| Platform / Driver / Hardware Abstraction         |
+--------------------------------------------------+
```

Luồng debug qua shell:

```text
PC Terminal
    |
    v
UART RX
    |
    v
Shell Task
    |
    v
Command Parser
    |
    +--> Query statistics
    +--> Post event
    +--> Run test
    +--> Dump trace
```

Luồng trace:

```text
Event posted
    |
    v
Write binary record
    |
    v
RAM ring buffer
    |
    +--> UART dump
    +--> Save on fatal
```

---

# 5. Design for Testability

Testability không nên được thêm vào sau khi firmware đã hoàn thành.

Cần thiết kế ngay từ đầu.

## Nguyên tắc

- Logic nghiệp vụ không truy cập thanh ghi trực tiếp.
- Driver có interface rõ ràng.
- Time source có thể thay thế.
- Hardware input có thể giả lập.
- Event output có thể quan sát.
- State nội bộ có thể query qua API kiểm thử.
- Không dùng biến global công khai.
- Không để hàm dài phụ thuộc nhiều peripheral.
- Handler phải deterministic.
- Side effect phải đi qua interface.

## Code khó test

```c
bool app_check_temperature(void)
{
    uint16_t raw = ADC1->DR;

    if (raw > 3000U)
    {
        GPIOA->ODR |= (1U << 5);
        return true;
    }

    return false;
}
```

Code này phụ thuộc trực tiếp ADC và GPIO.

## Code dễ test

```c
typedef struct
{
    uint16_t (*read_temperature_raw)(void);
    void (*set_warning_led)(bool enabled);
} app_port_t;
```

```c
bool app_check_temperature(
    const app_port_t *port
)
{
    const uint16_t raw =
        port->read_temperature_raw();

    const bool warning = (raw > 3000U);

    port->set_warning_led(warning);

    return warning;
}
```

Trong test, thay driver thật bằng fake.

---

# 6. Tách Application, Driver và Platform

Cấu trúc nên có:

```text
Application Logic
      |
      v
Port Interface
      |
      v
Platform Adapter
      |
      v
Hardware Driver
```

Ví dụ:

```text
app/
  app_controller.c

ports/
  temperature_port.h
  output_port.h

platform/stm32/
  temperature_adc_stm32.c
  output_gpio_stm32.c
```

Application chỉ include interface:

```c
#include "temperature_port.h"
```

Không include:

```c
#include "stm32f1xx.h"
```

trong logic nghiệp vụ.

---

# 7. Dependency Injection trong C

Dependency Injection trong C có thể dùng:

- Con trỏ hàm.
- Struct interface.
- Hàm callback.
- Link-time substitution.
- Weak symbol.
- Macro build configuration.

## Interface bằng struct

```c
typedef struct
{
    uint32_t (*get_tick_ms)(void);
    bool (*button_read)(uint8_t id);
    void (*led_write)(bool level);
} app_dependencies_t;
```

Object:

```c
typedef struct
{
    const app_dependencies_t *deps;
    uint8_t state;
} app_t;
```

Khởi tạo:

```c
void app_init(
    app_t *app,
    const app_dependencies_t *deps
)
{
    app->deps = deps;
    app->state = APP_STATE_IDLE;
}
```

Trong test:

```c
static uint32_t fake_time;
static bool fake_button;
static bool spy_led_level;
```

---

# 8. Test Double: Stub, Fake, Mock và Spy

## Stub

Trả về dữ liệu cố định.

```c
uint16_t stub_adc_read(void)
{
    return 3200U;
}
```

## Fake

Có behavior đơn giản nhưng hoạt động thực.

Ví dụ fake Flash trong RAM:

```c
static uint8_t fake_flash[1024];
```

## Spy

Ghi lại lời gọi.

```c
typedef struct
{
    uint32_t call_count;
    bool last_level;
} led_spy_t;
```

```c
void spy_led_write(bool level)
{
    g_led_spy.call_count++;
    g_led_spy.last_level = level;
}
```

## Mock

Kiểm tra lời gọi phải đúng kỳ vọng.

Ví dụ:

```text
Expected:
    led_write(true) được gọi đúng 1 lần
```

## Chọn loại nào?

| Loại | Mục đích |
|---|---|
| Stub | Cung cấp input cố định |
| Fake | Mô phỏng subsystem đơn giản |
| Spy | Ghi lại output/call |
| Mock | Xác nhận interaction cụ thể |

---

# 9. Unit Test trên host PC

Host-based test chạy trên Linux hoặc Windows.

Ưu điểm:

- Chạy nhanh.
- Không cần board.
- Dễ dùng sanitizer.
- Dễ tích hợp CI.
- Có thể chạy hàng nghìn test.

Nhược điểm:

- Không kiểm tra peripheral thật.
- Kiến trúc CPU khác MCU.
- Kích thước kiểu dữ liệu hoặc alignment có thể khác.
- Không phát hiện mọi lỗi timing.

## Build test

```bash
gcc \
    -std=c11 \
    -Wall \
    -Wextra \
    -Werror \
    -fsanitize=address,undefined \
    -Iinclude \
    src/event_queue.c \
    tests/test_event_queue.c \
    -o build/test_event_queue
```

Chạy:

```bash
./build/test_event_queue
```

## Test đơn giản

```c
#include <assert.h>

void test_add(void)
{
    assert(add(2, 3) == 5);
}
```

---

# 10. Unit Test trên target MCU

Target test chạy trực tiếp trên board.

Dùng để kiểm tra:

- Register-level driver.
- Interrupt.
- Timer hardware.
- DMA.
- Flash.
- UART.
- GPIO.
- Timing thật.

Kết quả có thể gửi qua UART:

```text
[PASS] gpio_output
[PASS] timer_tick
[FAIL] uart_loopback line=123
```

## Test mode

Firmware có thể hỗ trợ:

```text
Normal Mode
Test Mode
Manufacturing Mode
Diagnostic Mode
```

Chọn mode bằng:

- Build flag.
- GPIO strap.
- Command UART.
- NVM configuration.
- Boot command.

---

# 11. Command Line qua UART

Command Line là công cụ kiểm thử có giá trị cao trong embedded.

Nó cho phép:

- Gọi chức năng mà không cần viết lại firmware.
- Kiểm tra state.
- Phát event.
- Mô phỏng sensor.
- Xem pool.
- Xem queue.
- Dump trace.
- Chạy automated test.

Luồng:

```text
Terminal
   |
   v
UART Driver
   |
   v
RX Ring Buffer
   |
   v
Shell Active Object
   |
   v
Command Parser
   |
   v
Command Handler
```

---

# 12. Kiến trúc Shell Task trong Event-Driven

Shell không nên điều khiển hardware trực tiếp.

Không tốt:

```c
if (strcmp(cmd, "led on") == 0)
{
    GPIOA->ODR |= LED_PIN;
}
```

Tốt:

```c
if (strcmp(cmd, "led on") == 0)
{
    event_post_static(
        AO_LED_ID,
        &g_led_on_event
    );
}
```

Như vậy:

- Logic đi qua đúng architecture.
- Có event trace.
- State Machine vẫn kiểm soát.
- Test shell cũng là test integration.

## Shell Active Object

```c
typedef struct
{
    active_object_t super;

    char buffer[64];
    uint16_t length;
} shell_ao_t;
```

Shell nhận:

```text
SHELL_RX_BYTE_SIG
SHELL_LINE_READY_SIG
SHELL_EXECUTE_SIG
```

---

# 13. Thiết kế Command Table

```c
typedef int (*shell_command_fn_t)(
    int argc,
    char **argv
);

typedef struct
{
    const char *name;
    shell_command_fn_t handler;
    const char *help;
} shell_command_t;
```

Bảng:

```c
static const shell_command_t g_commands[] =
{
    {
        "help",
        shell_cmd_help,
        "Show command list"
    },
    {
        "state",
        shell_cmd_state,
        "Show application state"
    },
    {
        "event",
        shell_cmd_event,
        "Post or dump events"
    },
    {
        "pool",
        shell_cmd_pool,
        "Show pool usage"
    },
    {
        "test",
        shell_cmd_test,
        "Run automated tests"
    }
};
```

Tìm command:

```c
static const shell_command_t *
shell_find_command(const char *name)
{
    for (size_t i = 0U;
         i < ARRAY_SIZE(g_commands);
         i++)
    {
        if (strcmp(name, g_commands[i].name) == 0)
        {
            return &g_commands[i];
        }
    }

    return NULL;
}
```

---

# 14. Các command kiểm thử cần có

## Thông tin hệ thống

```text
version
build
reset_reason
uptime
```

## Task và Active Object

```text
task
state
mailbox
```

## Memory

```text
pool
heap
stack
ram
```

## Event

```text
event post
event dump
event clear
event stats
```

## Timer

```text
timer list
timer start
timer stop
timer stats
```

## Driver

```text
gpio read
gpio write
uart stats
can stats
adc read
```

## Test

```text
test list
test run all
test run queue
test run app_startup
test report
```

## Fault Injection

```text
fault crc
fault timeout
fault pool
fault watchdog
```

---

# 15. Ví dụ Shell điều khiển LED

```c
static int shell_cmd_led(
    int argc,
    char **argv
)
{
    if (argc < 2)
    {
        shell_printf(
            "usage: led on|off|toggle\n"
        );
        return -1;
    }

    if (strcmp(argv[1], "on") == 0)
    {
        event_post_static(
            AO_LED_ID,
            &g_led_on_event
        );
        return 0;
    }

    if (strcmp(argv[1], "off") == 0)
    {
        event_post_static(
            AO_LED_ID,
            &g_led_off_event
        );
        return 0;
    }

    if (strcmp(argv[1], "toggle") == 0)
    {
        event_post_static(
            AO_LED_ID,
            &g_led_toggle_event
        );
        return 0;
    }

    shell_printf("invalid LED option\n");
    return -1;
}
```

Kết quả:

```text
> led on
OK

> state led
LED state: ON
```

---

# 16. Ví dụ Shell phát event giả lập

Command:

```text
event post app BUTTON_PRESSED 1
```

Parser:

```c
static int shell_cmd_event_post(
    int argc,
    char **argv
)
{
    if (argc < 4)
    {
        shell_printf(
            "usage: event post <ao> <sig> <param>\n"
        );
        return -1;
    }

    const uint8_t destination =
        shell_parse_ao(argv[1]);

    const uint16_t signal =
        shell_parse_signal(argv[2]);

    const uint32_t parameter =
        shell_parse_u32(argv[3]);

    event_t *event =
        event_alloc(sizeof(parameter_event_t));

    if (event == NULL)
    {
        shell_printf("event allocation failed\n");
        return -1;
    }

    parameter_event_t *parameter_event =
        (parameter_event_t *)event;

    parameter_event->super.signal = signal;
    parameter_event->super.destination =
        destination;

    parameter_event->parameter = parameter;

    if (!event_post(event))
    {
        event_release(event);
        shell_printf("event post failed\n");
        return -1;
    }

    shell_printf("OK\n");
    return 0;
}
```

Đây là cách mô phỏng thao tác người dùng mà không cần chạm phần cứng thật.

---

# 17. Logging trong Embedded

Logging cần cân bằng:

```text
Thông tin hữu ích
vs.
RAM, Flash, CPU và bandwidth
```

Một log quá nặng có thể thay đổi timing của firmware và làm lỗi biến mất hoặc xuất hiện.

## Mục tiêu

- Hỗ trợ debug.
- Không block handler.
- Có thể tắt trong release.
- Có module filter.
- Có level filter.
- Có timestamp.
- Không dùng chuỗi dài nếu không cần.

---

# 18. Các mức log

```c
typedef enum
{
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE
} log_level_t;
```

Ý nghĩa:

| Level | Mục đích |
|---|---|
| FATAL | Hệ thống không thể tiếp tục |
| ERROR | Lỗi chức năng |
| WARN | Bất thường nhưng vẫn chạy |
| INFO | Trạng thái quan trọng |
| DEBUG | Chi tiết phục vụ phát triển |
| TRACE | Luồng event chi tiết |

Compile-time filter:

```c
#ifndef LOG_COMPILE_LEVEL
#define LOG_COMPILE_LEVEL LOG_LEVEL_INFO
#endif
```

Macro:

```c
#define LOG_INFO(module, fmt, ...) \
    log_write(                    \
        LOG_LEVEL_INFO,           \
        module,                   \
        fmt,                      \
        ##__VA_ARGS__             \
    )
```

---

# 19. Text Log và Binary Log

## Text Log

Ví dụ:

```text
[10234][APP][INFO] state IDLE -> RUNNING
```

Ưu điểm:

- Dễ đọc.
- Dùng terminal trực tiếp.

Nhược điểm:

- Tốn Flash vì format string.
- Tốn CPU do format.
- Tốn UART bandwidth.
- Khó ghi tốc độ cao.

## Binary Log

Ví dụ record:

```c
typedef struct
{
    uint32_t timestamp;
    uint16_t event_id;
    uint16_t argument;
} binary_log_t;
```

Ưu điểm:

- Nhỏ.
- Ghi nhanh.
- Phù hợp event trace.
- Có thể decode trên PC.

Nhược điểm:

- Cần tool decode.
- Cần version map.

## Khuyến nghị

Dùng kết hợp:

```text
Text Log:
    Lỗi và thông tin người dùng cần đọc.

Binary Event Trace:
    Luồng event tốc độ cao.
```

---

# 20. Event Trace có chi phí thấp

Trong Event-Driven System, không cần log chuỗi cho mọi event.

Chỉ cần lưu ID:

```text
timestamp
source
destination
signal
phase
```

Ví dụ:

```c
typedef enum
{
    TRACE_EVENT_POST = 0,
    TRACE_EVENT_DISPATCH,
    TRACE_EVENT_DONE,
    TRACE_EVENT_DROP,
    TRACE_STATE_TRANSITION
} trace_type_t;
```

Record:

```c
typedef struct
{
    uint32_t timestamp;
    uint16_t signal;
    uint8_t source;
    uint8_t destination;
    uint8_t type;
    uint8_t data;
} event_trace_t;
```

Kích thước có thể chỉ 12 byte hoặc ít hơn tùy packing.

---

# 21. Thiết kế Event Record

Record cần cố định kích thước để ghi nhanh.

```c
typedef struct
{
    uint32_t timestamp;
    uint16_t signal;

    uint8_t source;
    uint8_t destination;

    uint8_t trace_type;
    uint8_t state;

    uint16_t argument;
} event_record_t;
```

## Tránh pointer

Không lưu pointer vì:

- Không còn ý nghĩa sau reset.
- Khó decode.
- Có thể lộ địa chỉ nội bộ.
- Không ổn định giữa build.

## Dùng ID

```text
Task ID
Signal ID
State ID
Error ID
```

PC tool dùng map:

```text
3 -> AO_DISPLAY
42 -> DISPLAY_REFRESH_SIG
2 -> DISPLAY_STATE_ACTIVE
```

---

# 22. Ring Buffer cho Event Log

```c
#define EVENT_RECORD_CAPACITY (128U)

typedef struct
{
    event_record_t records[
        EVENT_RECORD_CAPACITY
    ];

    uint16_t head;
    uint16_t count;
    uint32_t overwritten;
} event_record_buffer_t;
```

Ghi:

```c
void event_record_write(
    event_record_buffer_t *buffer,
    const event_record_t *record
)
{
    if ((buffer == NULL) ||
        (record == NULL))
    {
        return;
    }

    buffer->records[buffer->head] = *record;

    buffer->head =
        (uint16_t)((buffer->head + 1U) %
                   EVENT_RECORD_CAPACITY);

    if (buffer->count <
        EVENT_RECORD_CAPACITY)
    {
        buffer->count++;
    }
    else
    {
        buffer->overwritten++;
    }
}
```

## Ưu điểm ring buffer

- Không cần heap.
- Luôn giữ event mới nhất.
- Không bị đầy vĩnh viễn.
- Phù hợp crash investigation.

---

# 23. Trace điểm nào trong hệ thống

Các trace point nên có:

## Event lifecycle

```text
ALLOC
POST
ENQUEUE
DEQUEUE
DISPATCH_START
DISPATCH_END
RELEASE
DROP
```

## State Machine

```text
STATE_BEFORE
TRANSITION
STATE_AFTER
UNHANDLED_EVENT
```

## Timer

```text
TIMER_ARM
TIMER_CANCEL
TIMER_EXPIRE
```

## Data-Link

```text
FRAME_TX
FRAME_RX
CRC_ERROR
TIMEOUT
RETRY
DROP
```

## Fault

```text
ASSERT
FATAL
WATCHDOG
HARDFAULT
POOL_EXHAUSTED
MAILBOX_OVERFLOW
```

Không nhất thiết bật tất cả trong release.

---

# 24. Đo Queue Latency và Handler Execution Time

## Queue latency

```text
Event post time
      |
      | waiting in mailbox
      v
Handler start time
```

```text
queue_latency =
    handler_start - post_timestamp
```

## Handler execution time

```text
handler_time =
    handler_end - handler_start
```

## Record mở rộng

```c
typedef struct
{
    uint32_t post_time;
    uint32_t dispatch_time;
    uint32_t complete_time;
} event_timing_t;
```

## Đo bằng cycle counter

```c
const uint32_t start =
    cpu_cycle_counter_get();

handler(event);

const uint32_t elapsed =
    cpu_cycle_counter_get() - start;
```

## Dùng để phát hiện

- Handler chạy quá lâu.
- Priority sai.
- Queue congestion.
- Event burst.
- Display render chậm.
- Communication blocking.

---

# 25. Theo dõi Mailbox và Event Pool

## Mailbox statistics

```c
typedef struct
{
    uint16_t current_depth;
    uint16_t maximum_depth;
    uint32_t post_count;
    uint32_t dispatch_count;
    uint32_t overflow_count;
} mailbox_stats_t;
```

## Event Pool statistics

```c
typedef struct
{
    uint16_t total_blocks;
    uint16_t used_blocks;
    uint16_t maximum_used;
    uint32_t allocation_count;
    uint32_t allocation_failures;
    uint32_t release_count;
} pool_stats_t;
```

Shell output:

```text
> mailbox

AO          CUR   MAX   POST   DROP
APP           2     7   1204      0
DISPLAY       1     9   3421      3
COMM          0    12   9802      0

> pool

POOL       USED   MAX   TOTAL   FAIL
SMALL         3    18      32      0
MEDIUM        1     6      12      0
LARGE         0     3       4      2
```

---

# 26. Record Event khi thiết bị đã triển khai

Thiết bị ngoài hiện trường không luôn kết nối debugger.

Cần record đủ để phân tích sau:

- Event cuối cùng.
- State cuối cùng.
- Fatal code.
- Reset reason.
- Firmware version.
- Build ID.
- Uptime.
- Pool statistics.
- Queue statistics.
- Communication statistics.

## Storage options

- Backup SRAM.
- Retention RAM.
- Internal Flash.
- External SPI Flash.
- EEPROM.
- File system trên MPU.
- Gửi telemetry lên server.

## Yêu cầu

- Ghi nhanh.
- Chịu mất nguồn.
- Có CRC.
- Có version.
- Không làm mòn Flash quá nhanh.
- Không lưu dữ liệu nhạy cảm ngoài ý muốn.

---

# 27. Crash Record và Reset Reason

```c
typedef struct
{
    uint32_t magic;
    uint16_t version;
    uint16_t size;

    uint32_t firmware_version;
    uint32_t build_id;

    uint32_t reset_reason;
    uint32_t fatal_code;
    uint32_t uptime_ms;

    uint8_t current_ao;
    uint8_t current_state;
    uint16_t current_signal;

    uint32_t mailbox_overflow_count;
    uint32_t pool_failure_count;

    uint32_t crc;
} crash_record_t;
```

Khi boot:

```c
void crash_record_boot_check(void)
{
    if (crash_record_is_valid())
    {
        shell_printf(
            "Previous crash detected\n"
        );

        crash_record_print();
    }
}
```

Sau khi đọc thành công có thể:

- Giữ lại.
- Đánh dấu uploaded.
- Clear theo command.
- Gửi về backend.

---

# 28. HardFault Debug

Khi HardFault xảy ra, cần lưu các register quan trọng:

```text
R0
R1
R2
R3
R12
LR
PC
xPSR
CFSR
HFSR
MMFAR
BFAR
```

Pseudo-code:

```c
typedef struct
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t xpsr;

    uint32_t cfsr;
    uint32_t hfsr;
    uint32_t mmfar;
    uint32_t bfar;
} hardfault_record_t;
```

Sau reset, dùng:

```bash
arm-none-eabi-addr2line \
    -e firmware.elf \
    0x08001234
```

để map `PC` về source line.

## Không nên

- `printf()` dài trong HardFault.
- Ghi Flash phức tạp.
- Dùng function phụ thuộc scheduler.
- Allocate memory.

HardFault handler phải rất nhỏ và dùng đường lưu an toàn.

---

# 29. Watchdog và Last-Gasp Record

Khi watchdog sắp reset, thường không còn nhiều thời gian.

Có thể lưu “last-gasp record” định kỳ thay vì chờ đúng lúc reset.

Ví dụ cập nhật mỗi 1 giây:

```c
typedef struct
{
    uint32_t heartbeat;
    uint32_t uptime;
    uint8_t current_state;
    uint8_t current_ao;
    uint16_t last_signal;
} last_gasp_t;
```

Nếu boot thấy watchdog reset:

```text
Reset reason: WATCHDOG
Last AO: COMM
Last signal: FRAME_RX
Last state: WAIT_ACK
```

## Watchdog supervision

Mỗi Active Object có heartbeat:

```text
APP alive
COMM alive
DISPLAY alive
STORAGE alive
```

Supervisor chỉ kick watchdog khi mọi module quan trọng đều tiến triển.

---

# 30. Trace trên Flash ngoài hoặc vùng nhớ giữ lại

## Internal Flash

Ưu điểm:

- Không cần phần cứng thêm.

Nhược điểm:

- Giới hạn write cycle.
- Erase theo page.
- Ghi chậm.
- Có thể block.

## External SPI Flash

Ưu điểm:

- Dung lượng lớn.
- Có thể lưu nhiều session.

Nhược điểm:

- Cần driver.
- Cần wear management.
- Cần integrity.

## Retention RAM

Ưu điểm:

- Ghi nhanh.
- Không làm mòn Flash.

Nhược điểm:

- Không giữ khi mất nguồn hoàn toàn.
- Dung lượng nhỏ.

## Chiến lược phù hợp

```text
Event trace chạy trong RAM ring buffer.
Khi fatal:
    Copy snapshot sang retention RAM hoặc Flash.
Sau reset:
    Dump hoặc upload.
```

---

# 31. Automated Test trong Embedded

Automated Test là khả năng chạy test mà không cần tester thao tác tay.

Ví dụ thay vì tester:

```text
Nhấn button
Chờ LED sáng
Nhấn tiếp
Kiểm tra màn hình
```

Test Runner sẽ:

```text
Inject BUTTON_PRESSED event
Run scheduler
Assert LED_ON event
Advance fake time
Assert DISPLAY state
```

Lợi ích:

- Lặp lại chính xác.
- Chạy nhanh.
- Không phụ thuộc người thao tác.
- Dễ regression.
- Có report tự động.
- Phát hiện lỗi sớm.

---

# 32. Kiến trúc Test Runner

```text
Test Command / CI
       |
       v
Test Runner
       |
       +--> Setup
       +--> Inject Input
       +--> Advance Time
       +--> Run Scheduler
       +--> Observe Output
       +--> Assert
       +--> Teardown
       |
       v
Test Report
```

Cấu trúc:

```c
typedef struct
{
    const char *name;

    bool (*setup)(void);
    bool (*run)(void);
    void (*teardown)(void);
} test_case_t;
```

Result:

```c
typedef enum
{
    TEST_RESULT_PASS = 0,
    TEST_RESULT_FAIL,
    TEST_RESULT_SKIP
} test_result_t;
```

---

# 33. Mô tả Test Case

Một test case cần có:

```text
ID
Tên
Mục tiêu
Điều kiện ban đầu
Input
Expected event
Expected state
Timeout
Cleanup
```

Ví dụ:

```text
ID: APP-TC-001
Tên: Start application from IDLE

Precondition:
    App state = IDLE

Input:
    START_BUTTON_PRESSED_SIG

Expected:
    App state = RUNNING
    MOTOR_START_SIG posted
    DISPLAY_RUNNING_SIG posted
    Timer APP_TICK armed
```

## Code

```c
static bool test_app_start(void)
{
    app_test_reset();
    event_spy_reset();
    fake_timer_reset();

    app_test_set_state(APP_STATE_IDLE);

    const event_t event =
    {
        .signal = START_BUTTON_PRESSED_SIG
    };

    app_handle_event(&event);

    TEST_ASSERT_EQ(
        APP_STATE_RUNNING,
        app_test_get_state()
    );

    TEST_ASSERT_TRUE(
        event_spy_contains(MOTOR_START_SIG)
    );

    TEST_ASSERT_TRUE(
        fake_timer_is_armed(APP_TICK_SIG)
    );

    return true;
}
```

---

# 34. Tự động kiểm thử use case sản phẩm

## Use case

```text
Mở thiết bị
-> vào menu
-> chọn game
-> chơi
-> game over
-> quay về menu
```

Có thể mô phỏng:

```text
SYSTEM_START
BUTTON_OK
BUTTON_RIGHT
BUTTON_OK
GAME_TICK x N
PLAYER_COLLISION
BUTTON_BACK
```

Expected:

```text
BOOT -> MENU -> GAME_SELECT -> PLAYING -> GAME_OVER -> MENU
```

## Test sequence

```c
typedef struct
{
    uint32_t time_ms;
    uint16_t signal;
    uint32_t parameter;
} test_step_t;
```

```c
static const test_step_t game_use_case[] =
{
    {0U,    SYSTEM_START_SIG, 0U},
    {100U,  BUTTON_OK_SIG,    0U},
    {200U,  BUTTON_RIGHT_SIG, 0U},
    {300U,  BUTTON_OK_SIG,    0U},
    {1000U, COLLISION_SIG,    0U},
    {1200U, BUTTON_BACK_SIG,  0U}
};
```

Runner:

```text
Advance fake time
Inject event
Run until idle
Check assertion
```

---

# 35. Fake Time và kiểm thử Timer

Không nên đợi thật:

```c
delay_ms(5000U);
```

Trong test.

Dùng fake time:

```c
static uint32_t g_fake_time_ms;

uint32_t fake_time_now(void)
{
    return g_fake_time_ms;
}

void fake_time_advance(uint32_t delta_ms)
{
    g_fake_time_ms += delta_ms;
    timer_process(g_fake_time_ms);
}
```

Test:

```c
timer_start(
    AO_APP_ID,
    APP_TIMEOUT_SIG,
    5000U
);

fake_time_advance(4999U);

TEST_ASSERT_FALSE(
    event_spy_contains(APP_TIMEOUT_SIG)
);

fake_time_advance(1U);

TEST_ASSERT_TRUE(
    event_spy_contains(APP_TIMEOUT_SIG)
);
```

## Cần test

- One-shot.
- Periodic.
- Cancel.
- Restart.
- Wraparound.
- Multiple timers.
- Timer cùng deadline.
- Timeout cũ sau transition.

---

# 36. Kiểm thử State Machine

## Transition test

```text
Given state A
When event X
Then state B
And action Y
```

## Unhandled event

```text
Given state IDLE
When FRAME_RX_SIG
Then state remains IDLE
And no side effect
```

## Entry/Exit

Kiểm tra:

- Exit action được gọi.
- Timer cũ bị cancel.
- Entry action khởi tạo state mới.
- Không có duplicate transition.

## Transition coverage matrix

| State | Event 1 | Event 2 | Event 3 |
|---|---|---|---|
| IDLE | Tested | Tested | N/A |
| RUNNING | Tested | Tested | Tested |
| ERROR | Tested | N/A | Tested |

---

# 37. Kiểm thử Event Queue và Event Pool

## Event Queue

Cần test:

- Empty.
- One element.
- FIFO.
- Full.
- Wraparound.
- Overflow policy.
- ISR producer.
- High-water mark.

## Event Pool

Cần test:

- Allocate.
- Free.
- Exhaustion.
- Reuse.
- Double free.
- Invalid pointer.
- Reference count.
- Multiple pool sizes.
- High-water mark.
- Allocation failure counter.

## Invariant

```text
allocated - released = current_used
```

Sau mỗi test:

```text
current_used phải bằng 0
```

Nếu khác 0, có memory leak.

---

# 38. Kiểm thử Data-Link Layer

Cần test:

- Encode/decode.
- Endianness.
- CRC đúng.
- CRC sai.
- Frame ngắn.
- Length vượt giới hạn.
- Version không hỗ trợ.
- Duplicate sequence.
- Missing sequence.
- Fragmentation.
- Reassembly timeout.
- Retry.
- ACK/NACK.
- Link down.
- Buffer full.

## Fuzz input đơn giản

Cho parser nhận:

- Byte ngẫu nhiên.
- SOF liên tục.
- Length cực lớn.
- CRC ngẫu nhiên.
- Frame bị cắt.
- Frame nối liền không khoảng trống.

Parser không được:

- Crash.
- Đọc vượt buffer.
- Loop vô hạn.
- Allocate vô hạn.

---

# 39. Fault Injection

Fault Injection chủ động tạo lỗi để kiểm tra khả năng phục hồi.

## Ví dụ

- Event Pool allocation failure.
- Mailbox full.
- CRC error.
- UART timeout.
- CAN bus-off.
- Sensor disconnected.
- Flash write failure.
- Watchdog timeout.
- Invalid state.
- Power loss giữa khi ghi.

## API

```c
typedef enum
{
    FAULT_NONE = 0,
    FAULT_EVENT_POOL_FAIL,
    FAULT_CRC_ERROR,
    FAULT_SENSOR_TIMEOUT,
    FAULT_FLASH_WRITE_FAIL
} fault_id_t;
```

```c
void fault_injection_enable(
    fault_id_t fault
);
```

Trong driver fake:

```c
if (fault_injection_is_enabled(
        FAULT_FLASH_WRITE_FAIL))
{
    return FLASH_ERROR;
}
```

## Lưu ý

Fault Injection phải:

- Chỉ bật trong test/debug build.
- Có command clear.
- Không vô tình bật trong production.
- Được trace.

---

# 40. Stress Test và Soak Test

## Stress Test

Tạo tải cực đại:

```text
1000 event/s
UART full speed
CAN full traffic
Display refresh tối đa
Timer dày đặc
```

Theo dõi:

- CPU usage.
- Handler max time.
- Queue max depth.
- Pool max used.
- Event drop.
- Link retry.
- Watchdog.
- Stack.

## Soak Test

Chạy dài hạn:

```text
24 giờ
72 giờ
168 giờ
```

Theo dõi theo thời gian:

```text
pool current
queue current
event count
error count
reset count
uptime
```

Dấu hiệu leak:

```text
Pool current tăng dần và không giảm.
```

---

# 41. Hardware-in-the-Loop

HIL dùng phần cứng ngoài để điều khiển DUT.

```text
Test PC
  |
  +--> USB-UART
  +--> USB-CAN
  +--> GPIO Controller
  +--> Programmable Power Supply
  |
  v
Device Under Test
```

Test có thể:

1. Flash firmware.
2. Reset board.
3. Gửi command.
4. Kích GPIO.
5. Đọc UART.
6. Kiểm tra CAN.
7. Tắt/bật nguồn.
8. Tạo brownout.
9. Thu test result.

## Ví dụ use case

```text
Power on
Wait boot banner
Send "test run startup"
Expect PASS
Toggle input GPIO
Expect BUTTON_PRESSED event
Read LED output
Expect HIGH
```

---

# 42. Regression Test

Mỗi bug nên tạo một test mới.

Quy trình:

```text
Bug report
   |
   v
Reproduce
   |
   v
Write failing test
   |
   v
Fix code
   |
   v
Test passes
   |
   v
Add to regression suite
```

Tên test:

```text
test_timer_cancel_prevents_stale_timeout
test_uart_parser_recovers_after_bad_crc
test_pool_releases_event_after_broadcast
```

Không chỉ sửa bug mà không để lại test.

---

# 43. Continuous Integration cho firmware

CI có thể chạy:

```text
Format check
Static analysis
Host unit tests
Sanitizer tests
Build all configurations
Size check
Documentation check
Optional hardware tests
```

## Pipeline

```text
Push / Pull Request
        |
        v
Compile host tests
        |
        v
Run unit tests
        |
        v
Run sanitizer
        |
        v
Build firmware
        |
        v
Check warnings
        |
        v
Generate test report
```

## Make targets

```makefile
.PHONY: test test-sanitize firmware size clean

test:
	$(MAKE) -C tests run

test-sanitize:
	$(MAKE) -C tests sanitize

firmware:
	$(MAKE) -C target all

size:
	arm-none-eabi-size build/firmware.elf
```

## CI phải fail khi

- Warning.
- Test fail.
- Build fail.
- Flash/RAM vượt budget.
- Static analysis có lỗi nghiêm trọng.
- Test report thiếu.

---

# 44. Coverage và các giới hạn

Coverage thường gồm:

- Function coverage.
- Line coverage.
- Branch coverage.
- State transition coverage.
- Requirement coverage.

## Không nên hiểu sai

100% line coverage không có nghĩa firmware không còn lỗi.

Có thể vẫn thiếu:

- Timing.
- Race condition.
- Hardware behavior.
- Power failure.
- EMI.
- Memory corruption.
- Requirement sai.

## Coverage hữu ích nhất cho Event-Driven

- Signal x State matrix.
- Transition coverage.
- Error path coverage.
- Queue overflow path.
- Pool exhaustion path.
- Timeout path.
- Retry path.

---

# 45. Test Report

Test report nên có:

```text
Build ID
Firmware version
Target
Compiler
Date
Commit
Test suite
Pass
Fail
Skip
Duration
Coverage
Flash/RAM size
```

Ví dụ:

```text
Test Summary
============

Commit      : a1b2c3d
Target      : host-linux
Compiler    : gcc 15
Tests       : 128
Passed      : 126
Failed      : 2
Skipped     : 0
Duration    : 1.42 s
```

Mỗi failure:

```text
[FAIL] test_timer_wraparound
File: tests/test_timer.c
Line: 142
Expected: timeout event
Actual: no event
```

---

# 46. Debug Workflow thực tế

## Bước 1 — Reproduce

Ghi rõ:

- Build.
- Board.
- Input.
- Thời gian.
- Tần suất.
- Điều kiện nguồn.
- Giao tiếp.

## Bước 2 — Thu thập dữ liệu

- Reset reason.
- Event trace.
- Pool stats.
- Queue stats.
- State.
- Crash record.
- Link counters.

## Bước 3 — Khoanh vùng

Xác định lỗi thuộc:

- Driver.
- Event route.
- Mailbox.
- Timer.
- State Machine.
- Data-Link.
- Ownership.
- Memory.

## Bước 4 — Viết test tái hiện

Ưu tiên host test nếu logic thuần.

## Bước 5 — Sửa

Không chỉ thêm delay hoặc tăng queue.

## Bước 6 — Regression

Giữ test trong suite.

## Bước 7 — Stress lại

Đảm bảo fix không gây lỗi mới.

---

# 47. Best Practices

Các nguyên tắc nên áp dụng:

## Thiết kế testability ngay từ đầu

Không chờ project lớn mới thêm test.

## Shell chỉ giao tiếp qua event

Không bypass architecture.

## Log không được block

Dùng buffer và task TX riêng.

## Event trace dùng binary record

Chuỗi chỉ dùng khi cần đọc trực tiếp.

## Mọi pool và mailbox có statistics

Ít nhất:

```text
current
maximum
failure
```

## Mỗi bug có regression test

Đây là quy tắc rất quan trọng.

## Test phải deterministic

Không phụ thuộc delay thực nếu có thể dùng fake time.

## Kiểm tra cleanup

Sau test:

```text
Pool used = 0
No timer pending
No event pending
State reset
```

## Test cả error path

Không chỉ happy path.

## Version hóa record và protocol

Để tool cũ không decode sai dữ liệu mới.

## Không ghi Flash cho mọi event

Dùng RAM ring buffer và snapshot khi cần.

---

# 48. Anti-pattern và lỗi thường gặp

Các anti-pattern và lỗi sau cần được tránh:

## Chỉ dùng `printf()`

Không có cấu trúc, không có filter, không có crash record.

## Log quá nhiều trong ISR

Làm sai timing và mất event.

## Test bằng thao tác tay duy nhất

Không lặp lại chính xác và khó regression.

## Dùng delay trong test

Test chậm và không deterministic.

## Shell điều khiển hardware trực tiếp

Bypass State Machine và ownership.

## Không kiểm tra pool sau test

Memory leak không bị phát hiện.

## Chỉ test happy path

Không biết hệ thống phản ứng khi queue đầy hoặc CRC sai.

## Tăng pool để che lỗi

Pool cạn có thể do event leak hoặc handler chậm.

## Ghi raw pointer vào crash log

Không hữu ích sau reset.

## HardFault handler gọi hàm phức tạp

Có thể fault thêm lần nữa.

## Test phụ thuộc thứ tự chạy

Test này để lại state cho test sau.

Mỗi test phải độc lập.

---

# 49. Ví dụ framework Test/Debug tối thiểu

## 49.1 Test assertion

```c
#ifndef TEST_ASSERT_H
#define TEST_ASSERT_H

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint32_t passed;
    uint32_t failed;

    const char *current_test;
    const char *failure_file;
    uint32_t failure_line;
} test_context_t;

extern test_context_t g_test_context;

#define TEST_ASSERT_TRUE(condition)               \
    do                                            \
    {                                             \
        if (!(condition))                         \
        {                                         \
            g_test_context.failed++;              \
            g_test_context.failure_file =         \
                __FILE__;                         \
            g_test_context.failure_line =         \
                (uint32_t)__LINE__;               \
            return false;                         \
        }                                         \
    } while (0)

#define TEST_ASSERT_EQ(expected, actual)           \
    TEST_ASSERT_TRUE((expected) == (actual))

#endif
```

## 49.2 Test case

```c
typedef bool (*test_fn_t)(void);

typedef struct
{
    const char *name;
    test_fn_t function;
} test_entry_t;
```

## 49.3 Test runner

```c
void test_run_all(
    const test_entry_t *tests,
    size_t count
)
{
    for (size_t i = 0U; i < count; i++)
    {
        g_test_context.current_test =
            tests[i].name;

        const bool passed =
            tests[i].function();

        if (passed)
        {
            g_test_context.passed++;

            test_output(
                "[PASS] %s\n",
                tests[i].name
            );
        }
        else
        {
            test_output(
                "[FAIL] %s %s:%lu\n",
                tests[i].name,
                g_test_context.failure_file,
                (unsigned long)
                    g_test_context.failure_line
            );
        }
    }
}
```

## 49.4 Event spy

```c
#define EVENT_SPY_CAPACITY (32U)

typedef struct
{
    uint16_t signals[EVENT_SPY_CAPACITY];
    uint16_t count;
} event_spy_t;
```

```c
void event_spy_record(uint16_t signal)
{
    if (g_event_spy.count <
        EVENT_SPY_CAPACITY)
    {
        g_event_spy.signals[
            g_event_spy.count
        ] = signal;

        g_event_spy.count++;
    }
}
```

```c
bool event_spy_contains(uint16_t signal)
{
    for (uint16_t i = 0U;
         i < g_event_spy.count;
         i++)
    {
        if (g_event_spy.signals[i] == signal)
        {
            return true;
        }
    }

    return false;
}
```

## 49.5 Trace record API

```c
void trace_event_post(
    uint8_t source,
    uint8_t destination,
    uint16_t signal
)
{
    const event_record_t record =
    {
        .timestamp = system_tick_get(),
        .signal = signal,
        .source = source,
        .destination = destination,
        .trace_type = TRACE_EVENT_POST,
        .state = 0U,
        .argument = 0U
    };

    event_record_write(
        &g_event_record_buffer,
        &record
    );
}
```

## 49.6 Shell test command

```c
static int shell_cmd_test(
    int argc,
    char **argv
)
{
    if (argc < 2)
    {
        shell_printf(
            "usage: test list|run <name|all>\n"
        );
        return -1;
    }

    if (strcmp(argv[1], "list") == 0)
    {
        test_print_list();
        return 0;
    }

    if ((argc >= 3) &&
        (strcmp(argv[1], "run") == 0))
    {
        return test_run_by_name(argv[2]);
    }

    return -1;
}
```

---

# 50. Bài thực hành

Các bài thực hành được sắp xếp từ cơ bản đến tích hợp:

## Bài 1 — Unit Test Event Queue

Test:

- Empty.
- FIFO.
- Full.
- Wraparound.
- Overflow counter.

## Bài 2 — Unit Test Event Pool

Test:

- Allocate all.
- Exhaustion.
- Free.
- Reuse.
- Double free.
- High-water mark.

## Bài 3 — Unit Test State Machine

State:

```text
IDLE
RUNNING
PAUSED
ERROR
```

Test mọi transition.

## Bài 4 — Event Spy

Tạo spy để kiểm tra handler post đúng signal.

## Bài 5 — Fake Timer

Không dùng thời gian thật.

Test one-shot và periodic timer.

## Bài 6 — UART Shell

Command:

```text
help
state
pool
mailbox
event
test
```

## Bài 7 — Binary Event Trace

Tạo ring buffer 128 record.

Command:

```text
trace dump
trace clear
trace stats
```

## Bài 8 — Queue Latency

Đo thời gian từ post đến dispatch.

Hiển thị:

```text
avg
max
min
```

## Bài 9 — Handler Execution Time

Phát hiện handler vượt ngưỡng.

```text
DISPLAY handler max: 1420 us
```

## Bài 10 — Crash Record

Lưu:

- Fatal code.
- Task.
- Signal.
- State.
- Uptime.
- Reset reason.

## Bài 11 — HardFault Record

Lưu PC, LR, xPSR và Fault Status Register.

## Bài 12 — Automated Use Case

Mô phỏng:

```text
Boot
Button press
Mode change
Timeout
Error
Recovery
```

Không cần thao tác tay.

## Bài 13 — Fault Injection

Mô phỏng:

- Pool fail.
- CRC error.
- Sensor timeout.
- Flash fail.

## Bài 14 — Regression Test

Chọn một bug cũ và viết test tái hiện.

## Bài 15 — Stress Test

Tạo event burst và đo:

- Drop count.
- Queue max.
- Pool max.
- Handler max time.

## Bài 16 — Soak Test

Chạy ít nhất 12 giờ.

Ghi statistics định kỳ.

## Bài 17 — CI

Tạo pipeline:

```text
Build
Unit test
Sanitizer
Firmware build
Size check
Artifact report
```

## Bài 18 — Hardware-in-the-Loop

PC script:

- Reset board.
- Gửi shell command.
- Đọc kết quả.
- Xuất report PASS/FAIL.

---

# 51. Tiêu chí hoàn thành

Người học được xem là hoàn thành chủ đề khi có thể:

- Phân biệt các cấp độ test.
- Thiết kế module có khả năng test.
- Dùng dependency injection trong C.
- Viết Stub.
- Viết Fake.
- Viết Spy.
- Viết Unit Test trên host.
- Viết Unit Test trên target.
- Tạo UART Shell.
- Shell giao tiếp qua event.
- Tạo command table.
- Tạo event injection command.
- Phân biệt text log và binary log.
- Tạo event trace ring buffer.
- Ghi trace lifecycle của event.
- Đo queue latency.
- Đo handler execution time.
- Theo dõi mailbox high-water mark.
- Theo dõi Event Pool high-water mark.
- Lưu crash record.
- Đọc reset reason.
- Lưu HardFault register.
- Tạo fake time.
- Test timer wraparound.
- Test State Machine transition coverage.
- Test queue overflow.
- Test pool exhaustion.
- Test Data-Link lỗi CRC.
- Thực hiện Fault Injection.
- Chạy Stress Test.
- Chạy Soak Test.
- Tạo Regression Test cho bug.
- Tích hợp CI.
- Xuất test report.
- Có tài liệu Test Plan trên GitHub.

---

# 52. Cấu trúc repository đề xuất

Cấu trúc repository đề xuất:

```text
05-embedded-test-debug/
├── README.md
├── Makefile
├── application/
│   ├── app.c
│   ├── app.h
│   ├── app_events.h
│   └── app_state_machine.c
├── framework/
│   ├── event_queue.c
│   ├── event_pool.c
│   ├── timer.c
│   ├── active_object.c
│   └── scheduler.c
├── debug/
│   ├── include/
│   │   ├── log.h
│   │   ├── trace.h
│   │   ├── crash_record.h
│   │   ├── fault_injection.h
│   │   └── statistics.h
│   └── src/
│       ├── log.c
│       ├── trace.c
│       ├── crash_record.c
│       ├── hardfault_record.c
│       ├── fault_injection.c
│       └── statistics.c
├── shell/
│   ├── shell.c
│   ├── shell.h
│   ├── shell_commands.c
│   └── shell_transport_uart.c
├── tests/
│   ├── framework/
│   │   ├── test_event_queue.c
│   │   ├── test_event_pool.c
│   │   └── test_timer.c
│   ├── application/
│   │   ├── test_app_state_machine.c
│   │   └── test_use_cases.c
│   ├── datalink/
│   │   ├── test_frame_parser.c
│   │   └── test_crc.c
│   ├── doubles/
│   │   ├── fake_time.c
│   │   ├── fake_flash.c
│   │   ├── event_spy.c
│   │   └── driver_stubs.c
│   ├── runner/
│   │   ├── test_runner.c
│   │   └── test_report.c
│   └── Makefile
├── tools/
│   ├── decode_trace.py
│   ├── run_hil_tests.py
│   ├── collect_uart_log.py
│   └── generate_report.py
├── platform/
│   ├── stm32/
│   └── host/
├── docs/
│   ├── test-plan.md
│   ├── test-cases.md
│   ├── debug-architecture.md
│   ├── trace-format.md
│   ├── crash-record-format.md
│   └── test-report.md
└── build/
```

## Nội dung README của project
- Mục tiêu kiểm thử.
- Phạm vi test.
- Kiến trúc Test/Debug.
- Cách build host test.
- Cách build target firmware.
- Cách chạy shell.
- Danh sách command.
- Cách dump event trace.
- Cách decode trace.
- Cách đọc crash record.
- Cách chạy automated use case.
- Cách chạy HIL.
- Cách chạy CI.
- Test summary.
- Known limitations.

---

# 53. Tài liệu tham khảo

Các tài liệu nên tham khảo:

## Active Kernel Base Kit

- https://github.com/ak-embedded-software/ak-base-kit-stm32l151

## Các nội dung cần đọc thêm

- Unit Testing Embedded C.
- Test-Driven Development for Embedded C.
- Dependency Injection in C.
- Event Trace.
- Circular Buffer.
- Crash Dump.
- ARM Cortex-M Fault Handling.
- Watchdog Supervision.
- Hardware-in-the-Loop Testing.
- Continuous Integration for Embedded.
- Static Analysis.
- Address Sanitizer và Undefined Behavior Sanitizer trên host.

## Tài liệu phần cứng

- Cortex-M Generic User Guide.
- Fault Status Registers.
- Reset and Clock Control.
- Independent Watchdog.
- Window Watchdog.
- Backup SRAM.
- Internal Flash programming.
- UART và DMA.
- SWO/ITM nếu MCU hỗ trợ.

---

# 54. Tổng kết

Một hệ thống Test/Debug tốt không chỉ là một vài lệnh `printf()`.

Kiến trúc hoàn chỉnh cần có:

```text
Testable Application Design
        +
Host Unit Tests
        +
Target Tests
        +
UART Command Line
        +
Binary Event Trace
        +
Mailbox/Pool Statistics
        +
Crash Record
        +
Fault Injection
        +
Automated Use-Case Tests
        +
Continuous Integration
```

Luồng kiểm thử tự động:

```text
Test Runner
    |
    v
Inject Event
    |
    v
Run Scheduler
    |
    v
Observe State / Output Event
    |
    v
Assert
    |
    v
Generate Report
```

Luồng debug ngoài hiện trường:

```text
Runtime Events
    |
    v
Binary Ring Buffer
    |
    v
Fatal / Watchdog / User Request
    |
    v
Snapshot
    |
    v
Reboot or Upload
    |
    v
Decode on PC
```

Các nguyên tắc quan trọng nhất:

1. Thiết kế testability từ đầu.
2. Tách logic nghiệp vụ khỏi phần cứng.
3. Shell phải đi qua kiến trúc event.
4. Không log chuỗi dài cho mọi event.
5. Dùng binary record để trace luồng tốc độ cao.
6. Mọi mailbox và pool phải có high-water mark.
7. Mọi bug nên có regression test.
8. Dùng fake time thay vì delay thật trong test.
9. Test cả error path, timeout và pool exhaustion.
10. Crash record phải có build ID, reset reason và event cuối.
11. HardFault handler phải nhỏ và an toàn.
12. Automated test phải chạy được không cần thao tác của tester.
13. CI phải build, test và kiểm tra memory budget.
14. Stress Test và Soak Test là bắt buộc với firmware dài hạn.
15. Hệ thống debug phải hỗ trợ cả giai đoạn phát triển và thiết bị ngoài thực tế.

Sau chủ đề này, người học cần có khả năng xây dựng một firmware không chỉ “chạy được”, mà còn **đo được, kiểm thử được, tái hiện lỗi được và phân tích được**.
