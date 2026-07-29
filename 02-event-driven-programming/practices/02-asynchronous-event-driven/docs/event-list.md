# Event List

| Signal | Source | Destination | Parameter |
|---|---|---|---|
| `SYSTEM_START` | System | App | Không |
| `BUTTON_EDGE` | EXTI0 ISR | Button | Không |
| `BUTTON_DEBOUNCE_TIMEOUT` | Timer | Button | Không |
| `BUTTON_PRESSED` | Button | App | Không |
| `BUTTON_RELEASED` | Button | App | Không |
| `LED_ON` | App/UART | LED | Không |
| `LED_OFF` | App/UART | LED | Không |
| `LED_BLINK_START` | App/UART | LED | Chu kỳ ms |
| `LED_BLINK_STOP` | UART | LED | Không |
| `LED_TIMEOUT` | Timer | LED | Không |
| `UART_RX_BYTE` | USART1 ISR | UART | Byte |
| `UART_STATUS_REQUEST` | UART | App | Không |
| `UART_STATUS_*` | App | UART | Không |
| `ERROR_DETECTED` | UART/System | App | Error code tùy chọn |
| `RESET_REQUEST` | UART/System | App | Không |

## Ownership

`event_t` được copy vào queue. Producer không truyền pointer tới biến local qua event. Queue sở hữu bản copy cho tới khi dispatcher lấy event ra.
