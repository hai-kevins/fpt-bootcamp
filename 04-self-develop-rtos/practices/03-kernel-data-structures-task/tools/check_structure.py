#!/usr/bin/env python3
from pathlib import Path
import sys
root=Path(__file__).resolve().parents[1]
required=['README.md','Makefile','include/list.h','include/task.h','include/task_registry.h','include/ready_queue.h','src/list.c','src/task.c','src/task_registry.c','src/ready_queue.c','src/main.c','labs/README.md']
required += [f'labs/{i:02d}-{name}/README.md' for i,name in [
(1,'singly-linked-list'),(2,'intrusive-doubly-list'),(3,'list-invariants'),(4,'tcb-layout'),(5,'static-task-creation'),(6,'task-stack-initialization'),(7,'all-task-registry'),(8,'priority-ready-queues'),(9,'tcb-multiple-lists'),(10,'target-task-inspector')]]
missing=[p for p in required if not (root/p).exists()]
if missing:
 print('Structure check: FAIL'); [print('  missing:',p) for p in missing]; sys.exit(1)
print('Structure check: PASS')
