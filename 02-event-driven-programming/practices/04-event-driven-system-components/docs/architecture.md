# Architecture

```text
Producer/ISR -> Event Pool -> Router/PubSub -> Mailbox -> Scheduler -> Active Object
                                                   |
                                                   +-> State Machine
                                                   +-> Data-Link -> Remote Node
```

The root demo is host-based and focuses on deterministic, run-to-completion processing.
