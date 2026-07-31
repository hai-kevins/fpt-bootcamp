#!/usr/bin/env python3
from pathlib import Path
import sys

root = Path(__file__).resolve().parents[1]
required = [
    'README.md', 'Makefile', 'linker/memory.ld', 'startup/startup.c',
    'include/task.h', 'include/scheduler.h', 'include/cortex_m3_port.h',
    'src/task.c', 'src/scheduler.c', 'src/cortex_m3_portasm.S',
    'labs/01-exception-stack-inspection/Makefile',
    'labs/02-initial-task-stack/Makefile',
    'labs/03-svc-first-task/Makefile',
    'labs/04-cooperative-pendsv/Makefile',
    'labs/05-priority-scheduler-host/Makefile',
    'labs/06-fixed-priority-target/Makefile',
    'labs/07-systick-time-slice/Makefile',
    'labs/08-isr-preemption/Makefile',
    'labs/09-round-robin/Makefile',
    'labs/10-context-switch-stress/Makefile',
]
missing = [item for item in required if not (root / item).exists()]
if missing:
    print('Structure check: FAIL')
    for item in missing:
        print(f'  missing: {item}')
    sys.exit(1)
print('Structure check: PASS')
