# State Machines

## Application

```text
BOOT --SYSTEM_START--> IDLE
IDLE --BUTTON_PRESSED--> ACTIVE
ACTIVE --BUTTON_PRESSED--> IDLE
IDLE/ACTIVE --ERROR_DETECTED--> ERROR
ERROR --RESET_REQUEST--> IDLE
```

## Button

Button giữ một stable state:

```text
Edge
  |
  v
Restart debounce timer 20 ms
  |
  v
Timeout
  |
  v
Read GPIO
  |
  +--> changed to pressed  -> BUTTON_PRESSED
  +--> changed to released -> BUTTON_RELEASED
  +--> unchanged           -> no event
```

## LED

```text
OFF --LED_ON--> ON
OFF/ON --LED_BLINK_START--> BLINKING
BLINKING --LED_TIMEOUT--> BLINKING + toggle
BLINKING --LED_BLINK_STOP--> OFF
ANY --LED_OFF--> OFF
```
