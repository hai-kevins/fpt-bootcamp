# Debug Tools

The root firmware provides:

- Task mailbox statistics.
- Message pool current and maximum usage.
- Timer active count and expiration count.
- 64-entry event record ring buffer.
- Fatal record in `.noinit`.
- UART command line.

Commands:

```text
help
task
pool
timer
state
led on
led off
led toggle
event dump
event clear
fatal show
fatal test
fatal clear
error
reset
```
