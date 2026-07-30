# Event Sequences

## Button press

```text
EXTI0 ISR
  -> BUTTON_EDGE to Button Task
  -> one-shot debounce timer 20 ms
  -> BUTTON_DEBOUNCE_TIMEOUT
  -> read physical input
  -> BUTTON_PRESSED to App Task
  -> App FSM transition
  -> LED command to LED Task
```

## UART command

```text
USART1 ISR -> UART_RX_BYTE -> Shell Task
Shell accumulates line -> parses command -> posts application message
```
