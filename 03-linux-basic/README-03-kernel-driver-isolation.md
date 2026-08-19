# Chủ đề 3 — Kernel, Driver và Isolation
## Kernel space, user space, Linux device model, sysfs/devfs và fault boundaries

> Chương này giải thích Linux kernel từ góc nhìn của embedded engineer: kernel quản lý resource nào, driver nằm ở đâu, userspace tương tác với hardware qua những interface nào, isolation được tạo ra bằng CPU privilege + MMU + process model ra sao, và vì sao một bug trong kernel/driver có blast radius khác hoàn toàn một bug trong application.

---

## Mục lục

- [1. Linux kernel chịu trách nhiệm gì?](#1-linux-kernel-chịu-trách-nhiệm-gì)
- [2. Monolithic kernel không có nghĩa mọi thứ là một file](#2-monolithic-kernel-không-có-nghĩa-mọi-thứ-là-một-file)
- [3. Kernel space và user space](#3-kernel-space-và-user-space)
- [4. CPU privilege level và system call boundary](#4-cpu-privilege-level-và-system-call-boundary)
- [5. Virtual memory và process isolation](#5-virtual-memory-và-process-isolation)
- [6. Kernel virtual address space](#6-kernel-virtual-address-space)
- [7. Process, thread và execution context](#7-process-thread-và-execution-context)
- [8. Linux driver là gì?](#8-linux-driver-là-gì)
- [9. Driver nằm trong subsystem](#9-driver-nằm-trong-subsystem)
- [10. Linux device model](#10-linux-device-model)
- [11. Bus, device và driver](#11-bus-device-và-driver)
- [12. Probe và remove](#12-probe-và-remove)
- [13. Character, block và network device](#13-character-block-và-network-device)
- [14. `/dev`: device node là interface, không phải device](#14-dev-device-node-là-interface-không-phải-device)
- [15. sysfs `/sys`](#15-sysfs-sys)
- [16. procfs `/proc`](#16-procfs-proc)
- [17. sysfs khác `/dev` như thế nào?](#17-sysfs-khác-dev-như-thế-nào)
- [18. Interrupt context và process context](#18-interrupt-context-và-process-context)
- [19. Top-half, deferred work và concurrency](#19-top-half-deferred-work-và-concurrency)
- [20. Memory-mapped I/O trong driver](#20-memory-mapped-io-trong-driver)
- [21. DMA và isolation khó hơn MMIO](#21-dma-và-isolation-khó-hơn-mmio)
- [22. Kernel synchronization](#22-kernel-synchronization)
- [23. Driver bug có thể phá hệ thống như thế nào?](#23-driver-bug-có-thể-phá-hệ-thống-như-thế-nào)
- [24. User-space crash và kernel crash](#24-user-space-crash-và-kernel-crash)
- [25. Oops, panic và lockup](#25-oops-panic-và-lockup)
- [26. `dmesg` và kernel log](#26-dmesg-và-kernel-log)
- [27. Call trace và fault reasoning](#27-call-trace-và-fault-reasoning)
- [28. Process isolation không phải security hoàn chỉnh](#28-process-isolation-không-phải-security-hoàn-chỉnh)
- [29. Namespaces, capabilities và cgroups ở mức khái niệm](#29-namespaces-capabilities-và-cgroups-ở-mức-khái-niệm)
- [30. Driver interface design cho userspace](#30-driver-interface-design-cho-userspace)
- [31. Stable ABI và internal kernel API](#31-stable-abi-và-internal-kernel-api)
- [32. Mô hình causal khi debug driver](#32-mô-hình-causal-khi-debug-driver)
- [33. Các nguyên tắc cốt lõi](#33-các-nguyên-tắc-cốt-lõi)
- [34. Tài liệu tham khảo chính](#34-tài-liệu-tham-khảo-chính)

---

## 1. Linux kernel chịu trách nhiệm gì?

Kernel là resource manager và protection boundary trung tâm của Linux.

Các nhóm trách nhiệm lớn:

- scheduling CPU;
- virtual memory và page management;
- process/thread lifecycle;
- interrupt handling;
- device drivers;
- filesystem/VFS;
- networking;
- IPC;
- security/credentials;
- timekeeping;
- power management.

Application thường không tự arbitrate các resource này. Nó yêu cầu kernel thông qua system call hoặc subsystem API được userspace library wrap lại.

```text
Application
    |
    | libc / system call ABI
    v
+------------------------------+
|          Linux Kernel        |
| scheduler | mm | vfs | net   |
| drivers   | ipc| irq | ...   |
+-------------+----------------+
              |
              v
           Hardware
```

---

## 2. Monolithic kernel không có nghĩa mọi thứ là một file

Linux thường được gọi là monolithic kernel vì nhiều core subsystem và driver chạy trong cùng privileged kernel address space. Nhưng kiến trúc source/runtime vẫn modular mạnh:

- subsystem;
- loadable kernel module;
- driver model;
- object lifecycle;
- callback interfaces.

“Monolithic” ở đây quan trọng nhất về **failure domain và privilege**: code driver kernel thường có quyền truy cập rộng hơn userspace process.

---

## 3. Kernel space và user space

Hai miền này khác nhau về privilege và address mapping.

```text
+-------------------------+
|      User process A     |
| virtual address space A |
+-------------------------+
           |
           | syscall / exception
           v
+=========================+  privilege boundary
|      Linux Kernel       |
| shared kernel space     |
+=========================+
           ^
           | syscall / exception
+-------------------------+
|      User process B     |
| virtual address space B |
+-------------------------+
```

Process A không mặc định đọc memory process B. Cả hai không tùy ý access kernel memory.

---

## 4. CPU privilege level và system call boundary

Trên ARM, chi tiết exception level/mode phụ thuộc architecture generation, nhưng ý tưởng chung là:

- userspace chạy ở privilege thấp;
- kernel chạy privilege cao;
- syscall tạo controlled transition vào kernel;
- kernel validate pointer, permission và argument trước khi thao tác resource.

System call là một **controlled gate**, không phải function call bình thường trong cùng trust domain.

```text
userspace: read(fd, buf, n)
        |
        v
syscall entry
        |
        v
validate fd / pointer / permission
        |
        v
VFS / driver
        |
        v
copy result to userspace
```

---

## 5. Virtual memory và process isolation

Mỗi process nhìn thấy virtual address space của riêng nó. MMU dịch virtual page sang physical page theo page table do kernel quản lý.

```text
Process A VA 0x4000 ----> Physical page X
Process B VA 0x4000 ----> Physical page Y
```

Cùng virtual address nhưng physical backing có thể khác.

Page table permission có thể phân biệt:

- read/write/execute;
- user/kernel access;
- mapped/unmapped.

Khi process access address không hợp lệ, CPU fault vào kernel; kernel thường chuyển thành signal thích hợp cho process thay vì cho phép corruption lan tùy ý.

---

## 6. Kernel virtual address space

Kernel cũng sử dụng virtual address. Physical RAM, vmalloc region và MMIO mapping được quản lý theo architecture/subsystem.

Điều quan trọng với embedded engineer:

```text
Physical register address != normal C pointer usable everywhere
```

Driver phải dùng kernel mapping/API đúng để truy cập MMIO. Lý do không chỉ portability mà còn ordering, architecture semantics và resource ownership.

---

## 7. Process, thread và execution context

Process là resource container có address space/credentials/file descriptors và nhiều state khác. Thread là execution context có register/stack/scheduling state nhưng có thể chia sẻ address space với thread cùng process.

Từ góc nhìn kernel scheduling, schedulable entity thường là task/thread context, không phải “application binary” theo nghĩa đơn giản.

---

## 8. Linux driver là gì?

Device driver là kernel component bridge giữa:

```text
Hardware-specific mechanism
            ^
            |
         Driver
            |
            v
Kernel subsystem abstraction
```

Driver tốt không chỉ “đọc ghi register”. Nó phải tích hợp với subsystem contract:

- lifecycle;
- power management;
- concurrency;
- IRQ;
- DMA;
- error handling;
- userspace ABI nếu có.

---

## 9. Driver nằm trong subsystem

Ví dụ:

- GPIO controller driver → gpiolib;
- I2C adapter/client driver → I2C subsystem;
- SPI controller/device driver → SPI subsystem;
- network driver → netdev/network stack;
- block driver → block layer;
- input driver → input subsystem;
- V4L2 camera driver → media subsystem.

Một lỗi thiết kế phổ biến là tạo custom char device cho mọi thứ, bỏ qua subsystem đã có. Điều này có thể mất reuse và standard semantics.

---

## 10. Linux device model

Linux device model biểu diễn object và relationship giữa bus, device, driver, class và kobject.

```text
                +---------+
                |   bus   |
                +----+----+
                     |
          +----------+----------+
          |                     |
     +----v----+            +---v-----+
     | device A|            | device B|
     +----+----+            +---+-----+
          |                     |
       driver A              driver B
```

Device model cho lifecycle, enumeration, sysfs representation, power management và hotplug semantics một nền tảng chung.

---

## 11. Bus, device và driver

Ba khái niệm này phải tách rõ:

- **bus**: môi trường/kênh kết nối và matching model;
- **device**: instance phần cứng/phần mềm cụ thể;
- **driver**: code biết cách điều khiển class/identity device tương ứng.

Match có thể dựa vào:

- Device Tree compatible;
- ACPI identifier;
- PCI/USB ID;
- bus-specific table.

---

## 12. Probe và remove

`probe` là lifecycle callback khi driver được bind với device. Trong probe driver thường acquire resource và register interface.

Conceptual transaction:

```text
probe begin
   |
   +--> allocate driver state
   +--> acquire clocks/reset/regulator
   +--> map MMIO
   +--> request IRQ
   +--> initialize hardware
   +--> register subsystem object
   |
 success
```

Nếu lỗi ở bước giữa, resource đã acquire trước đó phải được unwind đúng.

`remove`/unbind path phải teardown theo lifecycle phù hợp. Resource lifetime là vấn đề kernel correctness cốt lõi.

---

## 13. Character, block và network device

Ba mô hình phổ biến:

### Character device

Byte/stream/control-oriented interface, thường thông qua file operations và `/dev` node.

### Block device

Random-access block storage, tích hợp block layer, partition và filesystem stack.

### Network device

Không chủ yếu expose như file read/write. Nó đăng ký network interface và tham gia networking stack.

Phân loại giúp chọn abstraction đúng thay vì ép mọi device về một API duy nhất.

---

## 14. `/dev`: device node là interface, không phải device

`/dev/ttyS0`, `/dev/i2c-1`, `/dev/mmcblk0` là filesystem-visible device nodes/interface handles.

Một device node thường encode device type + major/minor identity để VFS route operation đến driver/subsystem.

```text
open("/dev/...")
      |
      v
VFS resolves inode/device number
      |
      v
registered file operations / subsystem
      |
      v
driver/hardware
```

Xóa node `/dev` không nhất thiết làm physical device biến mất; ngược lại device có thể tồn tại trong kernel nhưng không expose interface dưới `/dev` theo kiểu đó.

---

## 15. sysfs `/sys`

sysfs là pseudo filesystem xuất kernel object, attribute và relationship cho userspace.

Nó phản ánh device model khá trực tiếp:

```text
kernel objects
    |
    v
kobject hierarchy
    |
    v
sysfs directories/files
    |
    v
userspace observation/configuration
```

sysfs attribute thường là **structured control/status interface**, không phải nơi truyền bulk data tốc độ cao.

---

## 16. procfs `/proc`

`/proc` ban đầu gắn mạnh với process information nhưng cũng chứa nhiều kernel/system interfaces truyền thống.

Conceptually:

- `/proc/<pid>/...` → process state/view;
- `/proc/meminfo`, `/proc/interrupts`, ... → system/kernel information;
- `/proc/sys/...` → sysctl-style tunables.

Không nên đánh đồng `/proc` và `/sys`: chúng có lịch sử và semantic boundary khác nhau.

---

## 17. sysfs khác `/dev` như thế nào?

Một simplification hữu ích:

```text
/sys : "device/object là gì, có attribute/relationship nào?"
/dev : "mở interface nào để thực hiện I/O/control như device file?"
```

Không phải mọi sysfs object có `/dev` node và không phải mọi operation nên đi qua sysfs.

---

## 18. Interrupt context và process context

Driver có thể chạy trong nhiều context:

```text
Userspace syscall
    -> process context

Hardware IRQ
    -> interrupt context

Deferred mechanism
    -> softirq/tasklet/workqueue/threaded IRQ/... tùy subsystem
```

Context quyết định:

- có được sleep không;
- synchronization primitive nào hợp lệ;
- latency constraint;
- memory allocation mode;
- function API nào được gọi.

Một bug phổ biến của kernel programming là dùng API sleeping trong atomic/interrupt context.

---

## 19. Top-half, deferred work và concurrency

ISR nên xử lý phần cần phản ứng sớm và defer công việc dài hơn.

```text
Hardware IRQ
    |
    v
IRQ handler
    |  acknowledge/capture minimal state
    v
schedule deferred work
    |
    v
later context
    |  heavier processing
    v
wake process / complete I/O
```

Linux có nhiều deferred mechanisms với semantics khác nhau; điều quan trọng là hiểu **latency vs context capability**.

---

## 20. Memory-mapped I/O trong driver

MMIO register nằm trong physical address space. Driver acquire resource, map nó vào kernel virtual address space, rồi dùng accessor phù hợp.

```text
DT/firmware reg resource
      |
      v
physical MMIO region
      |
    map
      v
kernel virtual I/O mapping
      |
      v
read/write accessor
      |
      v
peripheral register
```

Ordering và side effect của MMIO khác normal RAM. Compiler/CPU reordering và bus semantics phải được kernel architecture abstraction xử lý đúng.

---

## 21. DMA và isolation khó hơn MMIO

DMA-capable device có thể đọc/ghi memory mà không cần CPU execute từng load/store.

```text
Device DMA engine <------> System memory
       ^                         ^
       |                         |
       +---- configured by ------+
                driver
```

Do đó driver phải quản lý:

- buffer ownership;
- cache coherency;
- DMA address mapping;
- lifetime;
- IOMMU nếu có;
- synchronization với CPU/device.

MMU bảo vệ CPU process access không tự động bảo vệ mọi DMA master; IOMMU là cơ chế tương ứng cho device address translation/isolation trên hệ hỗ trợ.

---

## 22. Kernel synchronization

Kernel code concurrent từ:

- multiple CPUs;
- interrupt;
- process context;
- worker threads;
- timer callbacks.

Primitive có thể gồm spinlock, mutex, semaphore, completion, atomic, RCU... nhưng không thể chọn dựa vào tên. Phải xét:

```text
Can context sleep?
Can interrupt access data?
How long is critical section?
Is data read-mostly?
Is ordering required?
What is lifetime rule?
```

Synchronization sai trong kernel có thể gây deadlock hoặc corruption toàn hệ thống.

---

## 23. Driver bug có thể phá hệ thống như thế nào?

Kernel/driver có privilege cao và shared kernel address space. Một bug có thể:

- dereference invalid pointer;
- overwrite kernel memory;
- program DMA sai address;
- giữ spinlock rồi không release;
- disable interrupt quá lâu;
- deadlock subsystem;
- access MMIO invalid;
- use-after-free object dùng chung.

Blast radius thường rộng hơn application bug.

```text
User bug ---> usually process-local failure boundary
Kernel bug -> shared kernel state -> system-wide failure possible
```

---

## 24. User-space crash và kernel crash

### User-space crash

Kernel thường vẫn sống; process nhận signal/terminate. Service manager có thể restart.

### Kernel-space crash

Có thể tạo oops/panic/hang/reboot tùy lỗi và policy. Vì kernel sở hữu scheduler/memory/driver, recovery tại userspace có thể không còn khả thi.

Đây là lý do nên giữ logic sản phẩm ở userspace khi không cần privilege/kernel latency trực tiếp.

---

## 25. Oops, panic và lockup

- **Oops**: kernel phát hiện fault nghiêm trọng trong kernel context và log diagnostic; hệ có thể tiếp tục nhưng state có thể đã compromised tùy lỗi.
- **Panic**: kernel quyết định không thể tiếp tục an toàn và dừng/reboot theo policy.
- **Soft/hard lockup**: CPU/task bị giữ quá lâu hoặc không scheduling/servicing expected timer/interrupt.

Không nên đánh đồng mọi “kernel crash” thành cùng một failure mode.

---

## 26. `dmesg` và kernel log

Kernel log là event stream từ boot và runtime. Nó có thể chứa:

- driver probe;
- IRQ/resource errors;
- filesystem/storage;
- networking;
- warnings;
- stack traces;
- OOM;
- panic.

`dmesg` là một cách đọc kernel ring buffer; tùy system, logging daemon có thể ingest/forward log này.

Điểm quan trọng khi debug là timestamp/order và **dòng đầu tiên thay đổi causal state**, không chỉ dòng cuối.

---

## 27. Call trace và fault reasoning

Một call trace cho biết execution path gần fault, nhưng frame cuối chưa chắc root cause.

Ví dụ use-after-free:

```text
Earlier event: object freed incorrectly
          |
          | time passes
          v
Later code dereferences stale pointer
          |
          v
fault + call trace here
```

Root cause nằm ở lifetime violation trước đó, không nhất thiết ở function faulting.

Kernel debugging vì vậy cần combine trace, object lifetime, concurrency và prior warnings.

---

## 28. Process isolation không phải security hoàn chỉnh

Virtual memory isolation chỉ là một lớp. Security còn liên quan:

- credentials UID/GID;
- file permissions;
- Linux capabilities;
- LSM policies;
- namespaces;
- seccomp;
- integrity/verified boot;
- attack surface của privileged services.

Process isolation giảm blast radius nhưng không tự động tạo một secure product.

---

## 29. Namespaces, capabilities và cgroups ở mức khái niệm

### Namespaces

Cho process có view riêng của một số global resource như PID, mount, network, UTS, user... tùy namespace.

### Capabilities

Tách đặc quyền “root” thành các capability nhỏ hơn thay vì all-or-nothing.

### Cgroups

Nhóm process để accounting/control resource theo subsystem/configuration.

Sơ đồ khái niệm:

```text
Process
  |
  +--> VM/page permissions       memory isolation
  +--> credentials/capabilities privilege control
  +--> namespaces               resource view isolation
  +--> cgroups                  resource accounting/control
```

---

## 30. Driver interface design cho userspace

Interface tốt phải trả lời:

- data plane hay control plane?
- synchronous hay asynchronous?
- blocking semantics?
- error model?
- versioning/ABI?
- concurrency?
- permission?

Linux đã có nhiều standard subsystem; sử dụng đúng subsystem thường cho userspace API nhất quán hơn custom ioctl tùy ý.

Nếu phải thiết kế custom ABI, nó cần được coi như public contract lâu dài.

---

## 31. Stable ABI và internal kernel API

User-space system call ABI của Linux có mục tiêu compatibility mạnh. Ngược lại **internal kernel API/ABI cho out-of-tree module không được đảm bảo stable như userspace ABI**.

Điều này giải thích vì sao out-of-tree driver có thể cần adaptation khi kernel version thay đổi, dù application userspace vẫn chạy.

Embedded product cần quản lý kernel + module source/config/version như một coherent release.

---

## 32. Mô hình causal khi debug driver

```text
Hardware exists?
   |
DT/firmware describes it correctly?
   |
Device object created?
   |
Driver matches?
   |
probe resources available?
   |
probe initializes hardware?
   |
subsystem interface registered?
   |
userspace sees expected API?
   |
runtime IRQ/DMA/data flow correct?
```

Mỗi bước có observable evidence khác nhau. Debug có hệ thống đi theo chain này thay vì sửa ngẫu nhiên driver source.

---

## 33. Các nguyên tắc cốt lõi

1. **Kernel là privileged resource manager và failure domain dùng chung.**
2. **Userspace ↔ kernel đi qua controlled ABI, không phải direct register access mặc định.**
3. **Driver nên sống trong đúng kernel subsystem.**
4. **Device Tree/device model/driver probe là ba tầng liên kết nhưng khác nhau.**
5. **`/sys` mô tả object/attribute; `/dev` cung cấp device-file interface khi phù hợp.**
6. **Context quyết định API nào được sleep, lock và allocate.**
7. **DMA tạo thêm vấn đề ownership/coherency/isolation ngoài CPU MMU.**
8. **Kernel bug có blast radius lớn; giữ logic không cần privilege ở userspace là nguyên tắc kiến trúc quan trọng.**
9. **`dmesg` và call trace phải được đọc theo causal chain và lifetime, không chỉ frame cuối.**

---

## 34. Tài liệu tham khảo chính

- Linux Kernel Documentation — Driver infrastructure: https://docs.kernel.org/driver-api/infrastructure.html
- Linux Kernel Documentation — Basic Device Structure: https://docs.kernel.org/driver-api/driver-model/device.html
- Linux Kernel Documentation — sysfs: https://docs.kernel.org/filesystems/sysfs.html
- Linux Kernel Documentation — kobjects/ksets/ktypes: https://docs.kernel.org/core-api/kobject.html
- Linux Kernel Documentation — Driver Basics: https://docs.kernel.org/driver-api/basics.html
- Linux man-pages — namespaces overview: https://man7.org/linux/man-pages/man7/namespaces.7.html
- Linux man-pages — mount namespaces: https://man7.org/linux/man-pages/man7/mount_namespaces.7.html
- Linux man-pages — strace: https://man7.org/linux/man-pages/man1/strace.1.html

