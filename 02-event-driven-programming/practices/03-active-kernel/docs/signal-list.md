# Signal List

| Signal | Source | Destination | Message type | Payload |
|---|---|---|---|---|
| `SYSTEM_START` | Startup | App | Pure | None |
| `BUTTON_EDGE` | EXTI ISR | Button | Pure | None |
| `BUTTON_DEBOUNCE_TIMEOUT` | Timer | Button | Pure | None |
| `BUTTON_PRESSED` | Button | App | Pure | None |
| `LED_ON/OFF/TOGGLE` | App/Shell | LED | Pure | None |
| `LED_BLINK_START` | App | LED | Common | `uint32_t period_ms` |
| `LED_BLINK_TICK` | Timer | LED | Pure | None |
| `UART_RX_BYTE` | USART ISR | Shell | Common | One byte |
| `SENSOR_DATA` | Sensor | App | Common | Sensor struct |
| `APP_ERROR/RESET` | Shell | App | Pure | None |
