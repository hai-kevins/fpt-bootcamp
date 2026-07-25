# Scheduling models

RTOS trong project này dùng fixed-priority preemptive scheduling:

- Priority số nhỏ hơn là cao hơn.
- Mỗi priority có một FIFO ready queue.
- Ready bitmap cho biết queue nào không rỗng.
- Task cùng priority dùng time slice và round-robin.
- Idle task luôn READY ở priority thấp nhất.

Scheduler policy được tách khỏi cơ chế save/restore context trong PendSV.
