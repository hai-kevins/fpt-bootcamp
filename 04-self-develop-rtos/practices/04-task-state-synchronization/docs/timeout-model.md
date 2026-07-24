# Timeout Model

HairRTOS dùng ba mode: `HR_NO_WAIT`, finite ticks và `HR_WAIT_FOREVER`. Kết quả wait gồm SUCCESS, TIMEOUT, CANCELLED và SUSPENDED.

Timeout và object event cạnh tranh theo single-winner rule trong critical section.
