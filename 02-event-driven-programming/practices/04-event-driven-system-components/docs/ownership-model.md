# Ownership Model

- Static events are never returned to the pool.
- Dynamic allocation starts with reference count 1.
- A successful mailbox post transfers one reference to the mailbox.
- The scheduler releases the dynamic event after the handler returns.
- Shared ownership requires an explicit retain before another owner receives the same event.
- Publish-subscribe in this repository allocates one independent copy per subscriber.
