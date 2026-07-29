# Sequence Diagrams

## Button press

```text
User      EXTI0 ISR     Queue      Button SM      Timer      App SM      LED SM
 |            |           |            |            |           |           |
 | press      |           |            |            |           |           |
 |----------->|           |            |            |           |           |
 |            | BUTTON_EDGE            |            |           |           |
 |            |---------->|----------->|            |           |           |
 |            |           |            | arm 20 ms  |           |           |
 |            |           |            |----------->|           |           |
 |            |           |            | timeout    |           |           |
 |            |           |<-----------|<-----------|           |           |
 |            |           |----------->| read GPIO  |           |           |
 |            |           |            | BUTTON_PRESSED         |           |
 |            |           |            |----------------------->|           |
 |            |           |            |            |           | BLINK     |
 |            |           |            |            |           |---------->|
```

## UART command

```text
USART1 ISR -> UART_RX_BYTE -> UART Service
UART Service -> parse "blink 500"
UART Service -> LED_BLINK_START(parameter=500) -> LED State Machine
LED State Machine -> arm periodic timer
Timer -> LED_TIMEOUT -> LED State Machine -> toggle PC13
```
