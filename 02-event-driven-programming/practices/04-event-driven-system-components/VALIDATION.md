# Validation Report

Validation date: **2026-07-30**

## Environment

```text
Host: Linux x86_64
Compiler: GCC 14.2.0
Build: GNU Make 4.4.1
Language: C11
Warnings: -Wall -Wextra -Werror -Wpedantic -Wshadow -Wconversion -Wsign-conversion
Sanitizers: AddressSanitizer + UndefinedBehaviorSanitizer
```

## Root project

```text
Integrated host demo: PASS
Root unit tests: 9/9 PASS
ASan/UBSan root tests: PASS
```

Demo output:

```text
Event-Driven components demo
delivered=2 dispatched=2
display handled=1 value=1234
diagnostic handled=1 value=1234
pool high-water=2 used=0
```

## Labs

The following 15 labs were built, executed and checked with ASan/UBSan:

```text
01-active-object                       PASS
02-mailbox                             PASS
03-run-to-completion-scheduler         PASS
04-flat-state-machine                  PASS
05-hierarchical-state-machine          PASS
06-table-driven-state-machine          PASS
07-event-pool                          PASS
08-reference-count-ownership           PASS
09-router-direct-post                  PASS
10-publish-subscribe                   PASS
11-uart-datalink                       PASS
12-parser-state-machine                PASS
13-link-failure                        PASS
14-event-trace                         PASS
15-stress-test                         PASS
```

## Excluded topics

The repository intentionally does not implement:

```text
CAN event transport
Multi-MCU demo
Mini distributed game
```

## Scope

Validation covers the host implementation, unit tests, integrated demo and sanitizer checks. It does not claim validation on an STM32 board, physical UART wiring, CAN hardware or a multi-node target system.

Re-run:

```bash
./tools/validate.sh
```
