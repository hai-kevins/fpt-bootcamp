# Comparison Worksheet

| Tiêu chí | Super-loop | Event-Driven |
|---|---|---|
| Event source | Polling | ISR/Timer/Event |
| Chờ thời gian | Delay/counter | Timer Event |
| Module coupling | Dễ tăng | Qua signal/destination |
| Unit test | Khó cô lập hơn | Handler/SM test độc lập |
| Queue RAM | Không | Có |
| Drop policy | Thường không rõ | Cần định nghĩa |
| Handler WCET | Không tách rõ | Phải run-to-completion |

Các con số từ demo chỉ minh họa. Khi đánh giá project thật, cần đo bằng trace, cycle counter và memory report.
