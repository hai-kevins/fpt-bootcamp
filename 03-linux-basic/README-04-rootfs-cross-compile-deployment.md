# Chủ đề 4 — RootFS, Cross-Compile và Deployment
## Root filesystem, ABI/sysroot, toolchain, Buildroot/Yocto và reliable software update

> Chương này trình bày userspace của Embedded Linux từ nền tảng: root filesystem là gì, executable tìm library ra sao, cross-compilation thực chất giải quyết khác biệt host/target như thế nào, Buildroot và Yocto tổ chức build ra sao, và deployment/update phải bảo vệ tính nhất quán của toàn bộ image như thế nào. Phần OTA chỉ phân tích architecture và reliability semantics, không phải bài thực hành.

---

## Mục lục

- [1. Root filesystem là gì?](#1-root-filesystem-là-gì)
- [2. RootFS không đồng nghĩa storage partition](#2-rootfs-không-đồng-nghĩa-storage-partition)
- [3. Filesystem hierarchy quan trọng trong embedded](#3-filesystem-hierarchy-quan-trọng-trong-embedded)
- [4. `/bin`, `/sbin`, `/usr`](#4-bin-sbin-usr)
- [5. `/etc`, `/var`, `/run`, `/tmp`](#5-etc-var-run-tmp)
- [6. `/dev`, `/proc`, `/sys`](#6-dev-proc-sys)
- [7. Static và dynamic linking](#7-static-và-dynamic-linking)
- [8. Dynamic loader và shared library dependency](#8-dynamic-loader-và-shared-library-dependency)
- [9. ABI là contract host-target quan trọng](#9-abi-là-contract-host-target-quan-trọng)
- [10. Cross-compilation là gì?](#10-cross-compilation-là-gì)
- [11. Build, host và target machine](#11-build-host-và-target-machine)
- [12. Cross toolchain gồm những gì?](#12-cross-toolchain-gồm-những-gì)
- [13. Sysroot](#13-sysroot)
- [14. Headers, libraries và target ABI](#14-headers-libraries-và-target-abi)
- [15. Common cross-compile failure modes](#15-common-cross-compile-failure-modes)
- [16. BusyBox và minimal userspace](#16-busybox-và-minimal-userspace)
- [17. Init system trong RootFS](#17-init-system-trong-rootfs)
- [18. Read-only và writable partition design](#18-read-only-và-writable-partition-design)
- [19. Persistent data và stateless rootfs](#19-persistent-data-và-stateless-rootfs)
- [20. Buildroot: model và output](#20-buildroot-model-và-output)
- [21. Yocto Project: model và metadata](#21-yocto-project-model-và-metadata)
- [22. Buildroot và Yocto khác nhau ở bản chất nào?](#22-buildroot-và-yocto-khác-nhau-ở-bản-chất-nào)
- [23. Reproducible build và version provenance](#23-reproducible-build-và-version-provenance)
- [24. Deployment là release transaction](#24-deployment-là-release-transaction)
- [25. Rủi ro khi deploy sai RootFS](#25-rủi-ro-khi-deploy-sai-rootfs)
- [26. Atomic update](#26-atomic-update)
- [27. A/B update](#27-ab-update)
- [28. Bootloader và update state machine](#28-bootloader-và-update-state-machine)
- [29. Rollback](#29-rollback)
- [30. Health check và commit](#30-health-check-và-commit)
- [31. Authenticity và integrity của update](#31-authenticity-và-integrity-của-update)
- [32. Version compatibility](#32-version-compatibility)
- [33. Data migration](#33-data-migration)
- [34. OTA system end-to-end](#34-ota-system-end-to-end)
- [35. Build/deploy artifact graph](#35-builddeploy-artifact-graph)
- [36. Các nguyên tắc cốt lõi](#36-các-nguyên-tắc-cốt-lõi)
- [37. Tài liệu tham khảo chính](#37-tài-liệu-tham-khảo-chính)

---

## 1. Root filesystem là gì?

Linux VFS trình bày một filesystem namespace bắt đầu từ `/`. Filesystem được mount tại `/` là root filesystem.

RootFS chứa userspace artifacts cần để hệ vận hành:

- init program;
- executable/utilities;
- shared libraries;
- configuration;
- device management data;
- service definitions;
- application;
- mount points.

Sơ đồ:

```text
/
├── bin/
├── sbin/
├── etc/
├── lib/
├── usr/
├── var/
├── run/
├── tmp/
├── dev/   <- usually virtual/device-managed
├── proc/  <- procfs mount
└── sys/   <- sysfs mount
```

Một kernel có thể boot đến điểm mount root nhưng không thể tạo userspace hữu ích nếu rootfs thiếu executable/init/library cần thiết.

---

## 2. RootFS không đồng nghĩa storage partition

Root filesystem là **logical mounted filesystem**. Nó có thể đến từ:

- physical partition;
- initramfs;
- NFS;
- UBI volume;
- read-only compressed filesystem;
- device-mapper stack;
- A/B slot.

Do đó:

```text
"rootfs" != "/dev/mmcblk0p2"
```

`/dev/mmcblk0p2` chỉ là một possible backing block device.

---

## 3. Filesystem hierarchy quan trọng trong embedded

Embedded Linux có thể tối giản hierarchy, nhưng semantic separation vẫn quan trọng:

```text
immutable product software
        |
        +--> /usr, /bin, /lib ... tùy layout

configuration
        |
        +--> /etc

persistent mutable state
        |
        +--> /var /data ...

runtime ephemeral state
        |
        +--> /run /tmp
```

Nếu trộn immutable software với mutable data trong cùng vùng update, recovery và rollback trở nên khó hơn.

---

## 4. `/bin`, `/sbin`, `/usr`

Layout cụ thể phụ thuộc distribution và xu hướng merged-/usr, nhưng về mặt conceptual chúng chứa executable và user-space resources.

Điều cần hiểu hơn tên folder là **rootfs phải chứa executable đúng architecture + loader/library tương thích**.

---

## 5. `/etc`, `/var`, `/run`, `/tmp`

### `/etc`

System configuration và service/application configuration theo policy distribution.

### `/var`

Mutable/persistent state như logs, database, spool tùy product.

### `/run`

Runtime state từ boot hiện tại, thường tmpfs/ephemeral.

### `/tmp`

Temporary data, lifecycle/persistence tùy mount policy.

Embedded product nên định nghĩa rõ dữ liệu nào survive reboot/update.

---

## 6. `/dev`, `/proc`, `/sys`

Ba directory này thường là mount/interface đặc biệt, không phải static ordinary files được copy đầy đủ vào rootfs image.

```text
/dev  -> device nodes / device manager output
/proc -> procfs, kernel/process view
/sys  -> sysfs, kernel object/device model view
```

RootFS cần mount points và userspace logic để chúng xuất hiện đúng.

---

## 7. Static và dynamic linking

### Static linking

Phần library code cần thiết được link vào executable. Deployment đơn giản hơn ở một số trường hợp nhưng binary lớn và update library duplication có tradeoff.

### Dynamic linking

Executable chứa dependency/relocation metadata; runtime dynamic loader map shared libraries.

```text
Executable
   |
   | PT_INTERP / dynamic metadata
   v
Dynamic loader
   |
   +--> libc.so
   +--> libm.so
   +--> app shared libs
   v
process image ready
```

Một file executable có thể tồn tại và có permission execute nhưng vẫn fail chạy nếu dynamic loader path không tồn tại hoặc ABI mismatch.

---

## 8. Dynamic loader và shared library dependency

Khi chạy ELF dynamically linked:

1. kernel nhận ELF;
2. interpreter path chỉ tới target dynamic loader;
3. loader resolve/load shared libraries;
4. relocation được xử lý;
5. control chuyển vào program startup.

Vì vậy lỗi “No such file or directory” đôi khi không có nghĩa executable path thiếu; interpreter trong ELF có thể thiếu.

Bản chất deployment phải đảm bảo **closure của runtime dependencies**.

---

## 9. ABI là contract host-target quan trọng

ABI bao gồm các quy ước binary như:

- instruction set/architecture;
- calling convention;
- register usage;
- ELF format details;
- endianness;
- word size;
- floating-point ABI;
- libc/kernel UAPI expectations.

Hai binary cùng “ARM” chưa chắc tương thích nếu khác ABI profile.

---

## 10. Cross-compilation là gì?

Cross-compilation nghĩa compiler chạy trên một architecture/platform nhưng tạo code cho target khác.

```text
Host x86_64 Linux
      |
      | aarch64-linux-gnu-gcc
      v
ELF AArch64 binary
      |
      v
Target ARM64 board
```

Điểm cốt lõi không phải prefix command mà là **compiler, headers, libraries và linker đều phải nhìn thế giới của target**.

---

## 11. Build, host và target machine

Trong build-system terminology đầy đủ có thể phân biệt:

- **build machine**: machine thực hiện build;
- **host machine**: machine binary đang build sẽ chạy trên;
- **target**: trong compiler context có thể chỉ architecture tạo code, hoặc target của toolchain; thuật ngữ thay đổi tùy project.

Trong embedded workflow thường nói ngắn:

```text
Development/build host --> cross-build --> embedded target
```

Khi build tool chạy trong quá trình compile và binary sản phẩm chạy trên target, hai loại artifact phải được tách rõ.

---

## 12. Cross toolchain gồm những gì?

Một toolchain điển hình:

```text
Compiler frontend/backend
        |
Assembler
        |
Linker + binutils
        |
Target C library + startup objects
        |
Kernel UAPI headers / sysroot headers
```

Compiler một mình không đủ để build userspace Linux đúng.

---

## 13. Sysroot

Sysroot là một directory hierarchy đại diện target development environment cho compiler/linker.

Nó thường chứa:

- target headers;
- target libraries;
- startup objects;
- pkg-config metadata được adjusted;
- ABI-specific files.

```text
Cross compiler
    |
    +--> target include from sysroot
    +--> target libs from sysroot
    |
    v
Target executable
```

Nếu build vô tình lấy `/usr/include` hoặc `/usr/lib` của host, artifact có thể compile sai hoặc link sai architecture.

---

## 14. Headers, libraries và target ABI

Header chỉ mô tả declaration/layout compile-time. Library cung cấp implementation binary.

Một coherent sysroot cần chúng tương thích với nhau và với target runtime.

Ví dụ mismatch nguy hiểm:

```text
new headers + incompatible old target libc
old kernel UAPI assumptions + incompatible userspace library
wrong hard-float/soft-float ABI
wrong architecture library selected by linker
```

Cross-build correctness là bài toán consistency của entire dependency graph.

---

## 15. Common cross-compile failure modes

### Compiler not found

Build system prefix/path không trỏ toolchain tồn tại.

### `Exec format error` trên host

Đã cố chạy target executable trên build host.

### Linker cannot find library

Sysroot/search path/dependency target chưa đúng.

### Binary chạy target báo loader missing

ELF interpreter path hoặc target libc không có trong rootfs.

### Undefined symbol/version

Runtime shared library khác version/API/ABI so với library dùng lúc link.

Những symptom này đều quay về separation host ↔ target.

---

## 16. BusyBox và minimal userspace

BusyBox kết hợp nhiều Unix utilities nhỏ thành một executable có thể expose qua applet/symlink.

Conceptual layout:

```text
/bin/sh ----+
/ bin/ls ---+--> busybox executable
/sbin/init -+
```

BusyBox phù hợp embedded vì configurable và footprint thấp. Nhưng BusyBox không thay thế kernel, device tree hay toàn bộ rootfs policy; nó chủ yếu cung cấp một tập userspace utilities/applet.

---

## 17. Init system trong RootFS

RootFS phải cung cấp initial userspace program và cấu hình service startup.

Tùy hệ:

```text
kernel
  |
  v
PID 1
  |
  +--> mount pseudo FS
  +--> device management
  +--> network
  +--> daemons
  +--> product application
```

Một embedded appliance đơn giản có thể dùng BusyBox init/custom init; hệ phức tạp có thể dùng systemd hoặc solution khác.

---

## 18. Read-only và writable partition design

Một architecture mạnh thường tách software image khỏi mutable data.

```text
+----------------------+  read-only / verified
| OS / RootFS slot     |
+----------------------+
| Persistent data      |  writable
+----------------------+
| Boot metadata        |  small, controlled
+----------------------+
```

Lợi ích:

- giảm corruption;
- rollback image không ghi đè user data;
- integrity verification đơn giản hơn;
- factory reset rõ semantic hơn.

---

## 19. Persistent data và stateless rootfs

Nếu rootfs được coi gần stateless/immutable, application state nằm ở dedicated writable location.

Cần phân loại:

```text
Firmware-owned config
User config
Calibration
Identity/keys
Logs
Database
Cache
Temporary files
```

Mỗi loại cần update/migration/backup policy khác nhau. Không phân loại data sớm sẽ làm OTA khó rollback an toàn.

---

## 20. Buildroot: model và output

Buildroot là build system tạo embedded Linux system qua cross-compilation. Nó có thể build:

- cross toolchain;
- userspace packages;
- root filesystem image;
- kernel;
- bootloader.

Mô hình khái niệm:

```text
Buildroot configuration
        |
        v
package dependency graph
        |
        +--> host tools
        +--> cross toolchain
        +--> target packages
        +--> kernel/bootloader
        |
        v
images / rootfs / binaries
```

Buildroot đặc biệt phù hợp khi muốn một image firmware tương đối self-contained và build flow trực tiếp.

---

## 21. Yocto Project: model và metadata

Yocto Project dùng OpenEmbedded/BitBake ecosystem để xây custom Linux distributions từ metadata.

Các khái niệm quan trọng:

- recipe;
- layer;
- machine configuration;
- distro configuration;
- image recipe;
- task/dependency graph;
- shared state/cache.

```text
Layers / recipes / configs
          |
          v
        BitBake
          |
      task graph
  +-------+--------+
  |       |        |
fetch  compile   package
  |       |        |
  +-------+--------+
          |
          v
 packages / SDK / images
```

Yocto mạnh khi product cần distribution-level customization, package metadata, multiple machine/distro variants và long-term scalable build structure.

---

## 22. Buildroot và Yocto khác nhau ở bản chất nào?

Không nên so sánh bằng câu “cái nào tốt hơn”. Hai công cụ tối ưu cho workflow khác nhau.

### Buildroot

Mental model thiên về **generate complete embedded system/image** bằng configuration tương đối trực tiếp.

### Yocto/OpenEmbedded

Mental model thiên về **construct custom distribution from metadata/tasks/packages/layers**.

Tradeoff khái niệm:

```text
             simpler mental model <------> richer distro metadata model
Buildroot --------------------------------------------- Yocto/OE
```

Project nhỏ không tự động phải dùng Buildroot; project lớn không tự động phải dùng Yocto. Quyết định phụ thuộc product lifecycle, variants, packaging, team và maintenance.

---

## 23. Reproducible build và version provenance

Một firmware release không chỉ là `rootfs.ext4`. Cần biết nó được tạo từ:

- source revision;
- build system revision;
- configuration;
- toolchain;
- package versions;
- patches;
- kernel config;
- DTB;
- build environment.

Artifact provenance model:

```text
source + metadata + config + toolchain
                |
                v
             build
                |
                v
        versioned artifacts
                |
                v
           release manifest
```

Nếu không reconstruct được image đã deploy, debugging production regression trở nên khó đáng kể.

---

## 24. Deployment là release transaction

Copy binary lên target chỉ là một dạng deployment nhỏ. Với product, deployment là transaction chuyển thiết bị từ release A sang B.

Transaction đúng phải xét:

- dependency consistency;
- power loss;
- reboot;
- schema/data migration;
- bootloader compatibility;
- rollback;
- authenticity;
- hardware revision.

Một update được ghi thành công chưa chắc là update thành công nếu image mới không boot hoặc application health fail.

---

## 25. Rủi ro khi deploy sai RootFS

### Mixed-version system

Một số library mới, executable cũ hoặc ngược lại.

### Missing dependency

Application không start do shared library/loader/config thiếu.

### Wrong architecture/ABI

Artifact build cho target khác.

### Data corruption

Update ghi vào active filesystem rồi mất nguồn giữa chừng.

### Boot incompatibility

Kernel/DTB/rootfs/bootloader không còn compatible như một release set.

Do đó versioning nên áp dụng cho **release bundle**, không chỉ từng binary rời rạc.

---

## 26. Atomic update

Atomicity nghĩa quan sát bên ngoài thấy update hoặc ở trạng thái cũ hợp lệ, hoặc trạng thái mới hợp lệ; không dừng ở mixed half-installed state.

```text
State A --prepare/write--> Candidate B
   |                         |
   | power loss              | verify
   |                         v
   +--------------------> A valid
                             |
                             v
                           commit
                             |
                             v
                          State B
```

A/B slots là một cách đạt atomicity ở image level, nhưng không phải cách duy nhất.

---

## 27. A/B update

A/B giữ hai system slots hoặc hai copies phù hợp.

```text
Currently booted: Slot A

+---------+        update        +---------+
| Slot A  | -------------------> | Slot B  |
| active  |                      | inactive|
+---------+                      +---------+
                                     |
                                  verify
                                     |
                                  mark try
                                     v
                                  reboot
```

Ưu điểm: active copy không bị ghi đè trong khi update; rollback có candidate cũ.

Chi phí: storage lớn hơn, boot metadata và state machine phức tạp hơn.

---

## 28. Bootloader và update state machine

Bootloader thường cần biết slot nào:

- active/good;
- candidate;
- remaining boot attempts;
- fallback.

State machine khái niệm:

```text
GOOD_A
  |
  | install B
  v
TRY_B
  | \
  |  \ fail/timeout
  |   v
  | ROLLBACK_A
  |
  | health success
  v
GOOD_B
```

Điều quan trọng là metadata update phải chịu được power loss và không tạo state ambiguous.

---

## 29. Rollback

Rollback là khả năng trở lại release known-good khi release candidate không đạt acceptance condition.

Rollback cần định nghĩa rõ:

- ai phát hiện failure;
- failure bao nhiêu lần thì rollback;
- data migration có backward-compatible không;
- bootloader có còn compatible old kernel/rootfs không;
- anti-rollback security policy có áp dụng không.

A/B image mà data schema bị migrate irreversible vẫn có thể **không rollback thực sự**.

---

## 30. Health check và commit

Boot thành công không đủ. Candidate cần được xác nhận ở mức product health.

```text
Boot candidate
   |
Kernel alive?
   |
RootFS mounted?
   |
Critical services ready?
   |
Hardware interfaces healthy?
   |
Application self-check OK?
   |
mark slot GOOD
```

Nếu commit quá sớm, image lỗi có thể được coi là good. Nếu commit quá muộn hoặc condition không xác định, hệ có thể rollback dù image tốt.

---

## 31. Authenticity và integrity của update

Hai câu hỏi khác nhau:

- **Integrity**: dữ liệu có bị thay đổi/corrupt không?
- **Authenticity**: artifact có thực sự do authority tin cậy phát hành không?

Hash có thể phát hiện corruption nhưng không tự chứng minh publisher nếu attacker có thể thay cả file và hash. Digital signature với trusted key material giải quyết authenticity ở release bundle.

```text
Release artifact + metadata
          |
       signature
          |
          v
Device trust anchor --> verify --> install allowed?
```

---

## 32. Version compatibility

Firmware bundle có thể chứa:

- bootloader;
- kernel;
- DTB;
- rootfs;
- application;
- MCU/coproc firmware;
- persistent data schema.

Compatibility matrix cần tránh mix không hợp lệ.

```text
Bootloader B1 -> supports kernel K1..K3
Kernel K3     -> expects DT schema/device layout D3
App A4        -> expects data schema V7
```

Update system nên validate hardware revision và release compatibility trước commit.

---

## 33. Data migration

Persistent data là phần khó nhất của rollback.

Migration phải xét:

- forward migration;
- backward compatibility;
- transactional database update;
- backup/restore;
- version marker;
- partial migration after power loss.

Một pattern an toàn hơn là schema evolution compatible qua ít nhất một release boundary thay vì destructive migration ngay lập tức.

---

## 34. OTA system end-to-end

OTA không chỉ là network download.

```text
Release CI
   |
   v
Signed firmware bundle
   |
   v
Distribution backend
   |
   v
Device updater
   |
   +--> hardware/version check
   +--> signature verify
   +--> write inactive target
   +--> verify written data
   +--> set boot candidate
   v
Reboot
   |
   v
Bootloader selects candidate
   |
   v
OS + app health check
   |          \
 success       failure
   |             |
 commit          rollback
```

Mỗi edge là một failure boundary cần explicit state.

---

## 35. Build/deploy artifact graph

Một release coherent có thể hình dung:

```text
                    Source revisions
                          |
          +---------------+---------------+
          |               |               |
       kernel            apps          bootloader
          |               |               |
         DTB          userspace libs      |
          |               |               |
          +-------+-------+---------------+
                  |
            RootFS / image build
                  |
                  v
             Release bundle
                  |
            sign + manifest
                  |
                  v
              deployment
                  |
                  v
               device
```

Quản lý version ở graph này quan trọng hơn việc “binary nào mới nhất”.

---

## 36. Các nguyên tắc cốt lõi

1. **RootFS là userspace root hierarchy, không đồng nghĩa một partition cụ thể.**
2. **Cross-compilation đúng cần coherent toolchain + sysroot + target ABI.**
3. **Executable tồn tại chưa đảm bảo chạy được; dynamic loader và dependency closure phải đúng.**
4. **BusyBox cung cấp minimal userspace utilities, không phải toàn bộ Embedded Linux stack.**
5. **Buildroot và Yocto giải quyết bài toán image/distribution build với mental model khác nhau.**
6. **Release cần provenance và versioning của toàn bộ artifact graph.**
7. **Deployment là transaction, không chỉ copy file.**
8. **Reliable OTA cần atomicity, health check, rollback và authenticity.**
9. **Persistent data/schema phải được thiết kế cùng rollback strategy.**

---

## 37. Tài liệu tham khảo chính

- Buildroot User Manual: https://buildroot.org/downloads/manual/manual.html
- Buildroot — project overview: https://buildroot.org/
- Yocto Project Documentation: https://docs.yoctoproject.org/
- Yocto Project — Overview and Concepts: https://docs.yoctoproject.org/overview-manual/yp-intro.html
- Yocto Project — Building: https://docs.yoctoproject.org/dev-manual/building.html
- BusyBox Documentation: https://busybox.net/BusyBox.html
- Linux Kernel Documentation — rootfs/initramfs: https://docs.kernel.org/filesystems/ramfs-rootfs-initramfs.html
- RAUC Documentation — Advanced / Software Deployment: https://rauc.readthedocs.io/en/latest/advanced.html
- SWUpdate Documentation — Software Management: https://sbabic.github.io/swupdate/overview.html
- SWUpdate Documentation — Best Practice: https://sbabic.github.io/swupdate/swupdate-best-practise.html

