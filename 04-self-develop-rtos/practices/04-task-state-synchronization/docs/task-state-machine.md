# Task State Machine

Task state phải nhất quán với current pointer và list membership. READY nằm trong ready queue; BLOCKED không nằm ready queue; SUSPENDED không nằm ready, wait hoặc timeout list.

## Transitions

```text
CREATED -> READY -> RUNNING
RUNNING -> READY | BLOCKED | SUSPENDED
BLOCKED -> READY | SUSPENDED
SUSPENDED -> READY
```

Không sửa `task->state` trực tiếp ngoài transition helper.
