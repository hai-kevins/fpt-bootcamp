from pathlib import Path
import sys

root = Path(__file__).resolve().parents[1]
required = [
    'README.md','Makefile','startup/startup.c','linker/memory.ld',
    'include/task.h','include/scheduler.h','include/timeout.h',
    'include/semaphore.h','include/mutex.h','src/task.c','src/scheduler.c',
    'src/timeout.c','src/semaphore.c','src/mutex.c',
    'labs/README.md','labs/01-task-state-machine/Makefile',
    'labs/10-target-synchronization-playground/Makefile'
]
missing = [item for item in required if not (root/item).exists()]
if missing:
    print('Structure check: FAIL')
    for item in missing: print('missing:', item)
    sys.exit(1)
print('Structure check: PASS')
