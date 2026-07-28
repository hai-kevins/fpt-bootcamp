# Labs — independent Makefiles

Mỗi thư mục lab là một build unit độc lập.

- Không build lab từ Makefile gốc.
- Không có rule `lab01`, `all-labs` hoặc `run-labs` tại root.
- Output nằm trong `build/` của chính lab.
- `make clean` chỉ xóa output của lab hiện tại.

## Host labs

```bash
cd labs/01-static-ring-buffer-queue && make test && make run
cd ../02-blocking-queue-host && make test && make run
cd ../04-message-pool && make test && make run
cd ../05-software-timer-list && make test && make run
```

## Target labs

```bash
cd labs/03-queue-from-isr && make
cd ../06-timer-service-task && make
cd ../07-timestamp-backends && make
cd ../08-context-switch-benchmark && make
cd ../09-event-message-latency && make
cd ../10-target-communication-benchmark-console && make
```
