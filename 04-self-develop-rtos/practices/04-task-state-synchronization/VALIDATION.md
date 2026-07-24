# Validation

Validation thực hiện trong môi trường tạo artifact:

- structure: **PASS**
- 01-task-state-machine tests: **PASS**
- 02-efficient-blocking tests: **PASS**
- 03-delayed-list-tick-wrap tests: **PASS**
- 04-delay-until tests: **PASS**
- 05-semaphore-host tests: **PASS**
- 07-mutex-ownership tests: **PASS**
- 08-priority-inheritance tests: **PASS**
- 09-suspend-resume tests: **PASS**
- root cross-build: **PASS**
- lab06 cross-build: **PASS**
- lab10 cross-build: **PASS**

Root và target labs đã cross-build bằng Clang/LLD cho Cortex-M3.
Host labs đã chạy với AddressSanitizer và UndefinedBehaviorSanitizer.
Firmware chưa được flash lên STM32F103 vật lý; target behavior cần được xác nhận trên board thật.
