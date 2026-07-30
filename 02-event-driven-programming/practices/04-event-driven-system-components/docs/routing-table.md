# Routing Table

Local destinations are routed to a registered Active Object mailbox. Remote destinations are passed to a transport callback after serialization.

| Node | Object | Route |
|---:|---:|---|
| 1 | 1 | Local Display AO |
| 1 | 2 | Local Diagnostic AO |
| 2 | 4 | Remote Data-Link |
