# Efficient Blocking

Busy-wait giữ CPU để kiểm tra điều kiện. Efficient blocking remove task khỏi ready queue và cho CPU chạy task khác.

Blocking transition phải atomic: remove ready, insert wait/timeout, set state, record object và request PendSV.
