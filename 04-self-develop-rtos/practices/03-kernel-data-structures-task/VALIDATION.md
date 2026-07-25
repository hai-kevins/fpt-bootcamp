# Validation Report

## Automated checks completed

The repository was validated in the generation environment with:

```bash
python3 tools/check_structure.py
tools/run_host_tests.sh
make clean all
make -C labs/10-target-task-inspector clean all
```

All host demo targets were also compiled and executed with `make run`.

## Results

- Repository structure check: PASS
- Lab 01 singly linked list tests/demo: PASS
- Lab 02 intrusive doubly list tests/demo: PASS
- Lab 03 list ownership and invariants tests/demo: PASS
- Lab 04 TCB layout/static assertions tests/demo: PASS
- Lab 05 static task creation tests/demo: PASS
- Lab 06 task stack initialization tests/demo: PASS
- Lab 07 all-task registry tests/demo: PASS
- Lab 08 per-priority ready queues tests/demo: PASS
- Lab 09 multiple-list TCB membership tests/demo: PASS
- Host AddressSanitizer/UndefinedBehaviorSanitizer checks: PASS
- Root RTOS Task Inspector Cortex-M3 cross-build: PASS
- Lab 10 standalone Task Inspector Cortex-M3 cross-build: PASS

Root cross-build size in this environment:

```text
text: 5804 bytes
data:    0 bytes
bss:  3368 bytes
```

## Hardware validation still required

The generated firmware was not flashed to a physical STM32F103C8T6 board in this
environment. The following items must still be confirmed on hardware:

- SVC starts the first task with PSP;
- PendSV preserves `R4-R11` across long runs;
- SysTick runs at 1 kHz from HSI 8 MHz;
- USART1 works at 9600 baud on PA9/PA10;
- PC13 LED toggles as expected;
- commands `h`, `t`, `r`, `c`, `s`, and `v` work;
- all stack guards remain valid;
- no HardFault occurs during extended execution.
