# Failure Policy

| Failure | Policy |
|---|---|
| Mailbox full | Reject newest and increment overflow counter |
| Pool exhausted | Return `NULL` and increment failure counter |
| CRC mismatch | Drop frame and increment parser error |
| ACK timeout | Retry up to configured limit |
| Retry exhausted | Enter `ED_LINK_DOWN` |
| Unknown route | Return false and increment route failure |
