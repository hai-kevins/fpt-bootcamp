# Memory Pools

The training kernel uses a fixed pool of 16 message slots. Each slot can carry up to 96 payload bytes.

- Pure message: zero payload.
- Common message: up to 24 bytes.
- Dynamic message: up to 96 bytes.
- Reference count allows one message to be retained by more than one owner.
- Allocation failure is explicit and counted.
- Pool high-water mark is available through `ak_message_pool_stats()`.

This implementation prioritizes visibility and deterministic bounds over memory efficiency. A production design may use multiple pools with different block sizes.
